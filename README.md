# Laser Communication System (Raspberry Pi)

라즈베리파이와 레이저, CdS 센서를 이용하여 광통신 시스템을 구현하고, 데이터 무결성과 재전송을 고려한 커스텀 통신 프로토콜을 설계한 프로젝트입니다.

---
## Overview

* Laser + CdS 센서를 이용한 광 기반 통신 시스템 구현
* Linux 사용자 공간에서 동작하는 소프트웨어 통신 구조 설계
* 오류 검출 및 재전송 메커니즘을 통한 데이터 신뢰성 확보

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

## 📖 Documentation

- [Protocol Design](docs/protocol_design.md)
- [Timing Issue](docs/timing_issue.md)
- [Reliability](docs/reliability.md)

---