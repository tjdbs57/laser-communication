// laser_comm_lib.c

#include "laser_comm_lib.h" 

void precise_delay(uint32_t us) {
    uint32_t start = gpioTick();
    while ((gpioTick() - start) < us);
}

// ★ 최종 해결책이 적용된 수신 함수 (Timing & Inversion)
unsigned char read_byte(int pin, uint32_t bit_time_us) {
    // 1. Start Bit 대기
    while (gpioRead(pin) == 0); 
    
    // 2. Start Bit 진동(Bounce)을 완전히 넘기기 위해 1.0T 대기
    precise_delay(bit_time_us * 1.0); 

    // 3. 첫 데이터 비트의 정확히 중앙을 잡기 위해 0.5T 추가 대기
    precise_delay(bit_time_us * 0.5); 
    
    unsigned char received_byte = 0;
    for (int i = 0; i < 8; i++) {
        // ★ 논리 반전 로직: CdS 센서가 0(LOW)일 때 논리적 1로 해석
        if (gpioRead(pin) == 0) { 
            received_byte |= (1 << i);
        }
        // 다음 비트의 중앙을 향해 1.0T 지연
        precise_delay(bit_time_us); 
    }

    precise_delay(bit_time_us * 0.5); // Stop bit 처리용 여유 시간
    return received_byte;
}

// DMA 송신 로직
void send_bytes_dma(const unsigned char *data, int len) {
    gpioPulse_t pulses[len * 12]; 
    int p = 0;

    for (int i = 0; i < len; i++) {
        unsigned char c = data[i];

        pulses[p++] = (gpioPulse_t){(1<<TX_PIN), 0, BIT_TIME_US}; // Start Bit
        
        for (int b = 0; b < 8; b++) {
            if ((c >> b) & 1) pulses[p++] = (gpioPulse_t){0, (1<<TX_PIN), BIT_TIME_US}; // 1 -> OFF
            else              pulses[p++] = (gpioPulse_t){(1<<TX_PIN), 0, BIT_TIME_US}; // 0 -> ON
        }
        
        pulses[p++] = (gpioPulse_t){0, (1<<TX_PIN), BIT_TIME_US}; // Stop Bit
        pulses[p++] = (gpioPulse_t){0, (1<<TX_PIN), INTER_BYTE_DELAY_US}; // Inter-byte delay
    }

    gpioWaveClear();
    gpioWaveAddGeneric(p, pulses);
    int wave_id = gpioWaveCreate();
    if (wave_id >= 0) {
        gpioWaveTxSend(wave_id, PI_WAVE_MODE_ONE_SHOT);
        while (gpioWaveTxBusy()) time_sleep(0.001);
        gpioWaveDelete(wave_id);
    }
}

unsigned char receive_control_byte_with_timeout(double timeout_s) {
    uint32_t start_tick = gpioTick();
    uint32_t timeout_us = (uint32_t)(timeout_s * 1000000);
    
    while (gpioRead(RX_PIN) == 0) {
        if (gpioTick() - start_tick > timeout_us) return 0x00; 
    }
    return read_byte(RX_PIN, BIT_TIME_US);
}

unsigned char calculate_checksum(const unsigned char *data, int length) {
    unsigned char sum = 0;
    for (int i = 0; i < length; i++) sum += data[i];
    return sum;
}

void send_ack_or_nack(unsigned char type) {
    unsigned char packet[1] = {type};
    send_bytes_dma(packet, 1);
    gpioWrite(TX_PIN, 0); 
    usleep(LASER_COOLDOWN_US); 
}

int send_data_packet(const char *data, int len) {
    int packet_len = len + 3; 
    unsigned char *packet = (unsigned char *)malloc(packet_len);
    packet[0] = STX;
    memcpy(&packet[1], data, len);
    packet[len + 1] = calculate_checksum((unsigned char *)data, len);
    packet[len + 2] = ETX;

    send_bytes_dma(packet, packet_len);
    free(packet);
    gpioWrite(TX_PIN, 0);
    usleep(LASER_COOLDOWN_US);
    return packet_len;
}

// 쓰레기값 중복 감지 방지 
int receive_data_packet(unsigned char *buffer, int max_len) {
    while (gpioRead(RX_PIN) == 0); 
    
    unsigned char stx = read_byte(RX_PIN, BIT_TIME_US);
    if (stx != STX) {
        usleep(LASER_COOLDOWN_US); 
        return -1; 
    }
    
    int i = 0;
    while(i < max_len - 1) {
        unsigned char byte = read_byte(RX_PIN, BIT_TIME_US);
        if (byte == ETX) break; 
        buffer[i++] = byte;
    }
    return i; 
}