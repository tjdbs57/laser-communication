# Protocol Design

## Overview
레이저 통신 환경에서 데이터 무결성을 보장하기 위해 
커스텀 프로토콜을 설계하였다.

## Packet Structure
[STX][DATA][CHECKSUM][ETX]

- STX: 시작 바이트
- DATA: 실제 데이터
- CHECKSUM: 오류 검출
- ETX: 종료 바이트

## Communication Flow

TX → DATA → RX  
TX ← ACK/NACK ← RX  

## Design Reason

- 단순 전송만으로는 데이터 깨짐 발생
- 오류 검출 및 재전송 필요
- 경량 프로토콜로 구현