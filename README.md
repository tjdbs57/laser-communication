# Laser Communication System (Raspberry Pi)

라즈베리파이와 레이저, CdS 센서를 이용하여 광통신 시스템을 구현하고,
하드웨어 제약(센서 응답 속도, OS 스케줄링)을 소프트웨어적으로 보완하여
신뢰성 있는 데이터 전송 구조를 설계한 프로젝트입니다.

---

## 📚 Tech Stack

<div align="center">

### 💻 Language
<img src="https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=black">

### ⚙️ Platform
<img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black">
<img src="https://img.shields.io/badge/Raspberry Pi-C51A4A?style=for-the-badge&logo=raspberrypi&logoColor=white">

### 🔧 Library / Interface
<img src="https://img.shields.io/badge/pigpio-DMA%20GPIO-blue?style=for-the-badge">

### 🛠 Tools
<img src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white">
<img src="https://img.shields.io/badge/GCC-Compiler-6DA55F?style=for-the-badge">

</div>

---
## Overview

* Laser + CdS 센서를 이용한 광 기반 통신 시스템 구현
* Linux 사용자 공간에서 동작하는 소프트웨어 통신 구조 설계
* 오류 검출 및 재전송 메커니즘을 통한 데이터 신뢰성 확보


---

## System Architecture

```
[TX Raspberry Pi]
        ↓
    Laser Module
        ↓
   Optical Signal
        ↓
     CdS Sensor
        ↓
[RX Raspberry Pi]
```

---

## Communication Protocol

```
[STX][DATA][CHECKSUM][ETX]
```

* STX: Start of Text (0x02)
* ETX: End of Text (0x03)
* Checksum: 데이터 바이트 합

### ACK/NACK Flow

```
TX → DATA PACKET → RX
TX ← ACK / NACK ← RX

(실패 시 재전송)
```

---

## Key Implementation

### 1. 비트 중앙 샘플링

Start bit 이후 정확한 시점(1.5T 지연 후)에 데이터를 샘플링하여 오류를 감소시킴

### 2. 논리 반전 처리

CdS 센서 특성:

* LOW → 빛 감지 → 논리 1로 해석

### 3. DMA 기반 송신

pigpio의 DMA waveform 기능을 활용하여 정확한 비트 타이밍 구현

### 4. 오류 검출 및 재전송

* Checksum 기반 데이터 무결성 검증
* ACK/NACK 기반 재전송 구조

---

## Execution

### 송신 (Client)

```bash
gcc client_tx.c laser_comm_lib.c -lpigpio -lrt -lpthread -o tx
sudo ./tx
```

### 수신 (Server)

```bash
gcc server_rx.c laser_comm_lib.c -lpigpio -lrt -lpthread -o rx
sudo ./rx
```

---

## Result

* 기존: 데이터 깨짐 및 통신 실패 빈번
* 개선 후: 안정적인 데이터 전송 가능
* 재전송 구조를 통해 오류 발생 시 복구 가능

---

## Project Structure

```
.
├── client_tx.c
├── server_rx.c
├── laser_comm_lib.c
├── laser_comm_lib.h
├── config.h
```

---

## Tech Stack

* C
* Raspberry Pi
* pigpio (DMA 기반 GPIO 제어)
* Linux

---