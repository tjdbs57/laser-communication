// client_tx.c

#include "laser_comm_lib.h" 

int main() {
    if (gpioInitialise() < 0) return 1;
    gpioSetMode(TX_PIN, PI_OUTPUT);
    gpioSetMode(RX_PIN, PI_INPUT); 
    gpioWrite(TX_PIN, 0); 

    printf("--- [광통신 Client (Tx)] ---\n");

    char input_str[256];
    
    while(1) {
        printf("\n전송할 메시지 입력 > ");
        if (fgets(input_str, sizeof(input_str), stdin) == NULL) break;
        input_str[strcspn(input_str, "\n")] = 0;
        
        int retry_count = 0;
        int success = 0;

        while(retry_count < MAX_RETRY && !success) {
            send_data_packet(input_str, strlen(input_str));
            printf("   [Wait] ACK 대기 중...\n");
            
            unsigned char control_byte = receive_control_byte_with_timeout(RX_TIMEOUT_S);
            
            if (control_byte == ACK_BYTE) {
                printf(" -> [수신] ACK! 전송 성공.\n");
                success = 1;
            } else if (control_byte == NACK_BYTE) {
                 printf(" -> [수신] NACK. 데이터 깨짐.\n");
            } else { 
                 printf(" -> [오류] 응답 없음 (Timeout).\n");
            }

            if(!success) {
                printf("   [Retry] %d번째 재전송...\n", ++retry_count);
                sleep(1); 
            }
        }
        if(!success) printf(">>> 최종 전송 실패 <<<\n");
    }

    gpioTerminate();
    return 0;
}