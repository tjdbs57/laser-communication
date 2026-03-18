// server_rx.c

#include "laser_comm_lib.h"

int main() {
    if (gpioInitialise() < 0) return 1;
    gpioSetMode(TX_PIN, PI_OUTPUT); 
    gpioSetMode(RX_PIN, PI_INPUT);  
    gpioWrite(TX_PIN, 0);

    printf("--- [광통신 Server (Rx)] ---\n");

    unsigned char buffer[256];

    while(1) {
        printf("\n[대기 중] 데이터 수신 대기...\n");
        int received_bytes = receive_data_packet(buffer, 256);
        
        if (received_bytes > 0) {
            int data_len = received_bytes - 1; 
            unsigned char received_checksum = buffer[data_len]; 
            unsigned char calculated_checksum = calculate_checksum(buffer, data_len);
            
            buffer[data_len] = '\0'; 
            
            if (received_checksum == calculated_checksum) {
                printf("[RX] 성공! Data: '%s' -> ACK 발사\n", buffer);
                send_ack_or_nack(ACK_BYTE); 
                usleep(100000); // 100ms 휴식 (중복 감지 방지)
            } else {
                printf("[RX] 실패! (CS 불일치) -> NACK 발사\n");
                send_ack_or_nack(NACK_BYTE); 
                usleep(50000); 
            }
        }
    }
    
    gpioTerminate();
    return 0;
}