# Timing Issue and Solution

## Problem

- Linux 사용자 영역에서 실행 → 정확한 타이밍 보장 어려움
- CdS 센서 응답 속도 느림
- 비트 샘플링 시점이 어긋나 데이터 깨짐 발생

## Solution

### 1. 중앙 샘플링
Start bit 이후 1.5T 지연 후 샘플링

### 2. precise delay
busy-wait 기반 마이크로초 단위 지연 구현

### 3. DMA 송신
pigpio waveform 사용하여 정확한 타이밍 확보

## Result

- 비트 오류 감소
- 안정적인 데이터 수신 가능