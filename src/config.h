// config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>
#include <time.h> 

// 통신 속도 및 시간 설정 
#define BAUD_RATE 30                    
#define BIT_TIME_US (1000000 / BAUD_RATE)   // 약 33333 us
#define MAX_RETRY 3                     
#define RX_TIMEOUT_S 0.5                
#define LASER_COOLDOWN_US 50000             // 자가 간섭 방지용 쿨다운 (50ms)
#define INTER_BYTE_DELAY_US (BIT_TIME_US * 5) 

// 핀 설정
#define TX_PIN 14       // 레이저 송신 핀 (GPIO 14)
#define RX_PIN 15       // 광센서 수신 핀 (GPIO 15)

// 프로토콜 제어 바이트
#define STX 0x02        // Start of Text
#define ETX 0x03        // End of Text
#define ACK_BYTE 0x06   // Acknowledge 
#define NACK_BYTE 0x15  // Negative Acknowledge 

#endif // CONFIG_H