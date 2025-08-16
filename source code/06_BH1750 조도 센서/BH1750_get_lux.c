
/*
 * 파일명: BH1750_get_lux.c
 * ------
 * BH1750 광센서(I2C)로 조도(lux)를 측정하는 예제 코드
 * - I2C 통신으로 센서 초기화 및 데이터 읽기
 * - 주요 코드와 연산에 상세 설명 주석 추가
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 미지원)
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include <sys/ioctl.h>  // IO 제어
#include <linux/i2c-dev.h> // I2C 디바이스 제어


#define I2C_BUS "/dev/i2c-1"  // 라즈베리파이 I2C 디바이스 경로
#define BH1750_ADDRESS 0x23    // BH1750 센서 I2C 주소
#define BH1750_POWER_ON 0x01   // 센서 전원 ON 명령
#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10 // 연속 고해상도 측정 모드 명령


int main() {
    int file;
    char* filename = I2C_BUS;

    // I2C 버스 오픈
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    // BH1750 센서 슬레이브 주소 설정
    if (ioctl(file, I2C_SLAVE, BH1750_ADDRESS) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        exit(1);
    }

    // BH1750 센서 전원 ON
    if (write(file, &BH1750_POWER_ON, 1) != 1) {
        perror("Failed to power on the sensor");
        exit(1);
    }

    // 연속 고해상도 측정 모드 설정
    if (write(file, &BH1750_CONTINUOUS_HIGH_RES_MODE, 1) != 1) {
        perror("Failed to set continuous high resolution mode");
        exit(1);
    }

    // 200ms 대기 (측정 준비)
    usleep(200000);

    // 센서 데이터 2바이트 읽기
    unsigned char data[2];
    if (read(file, data, 2) != 2) {
        perror("Failed to read from the sensor");
        exit(1);
    }

    // lux 값 계산 (raw값 → lux 변환)
    int lux = (data[0] << 8) | data[1];
    float illuminance = (float)lux / 1.2;

    // 결과 출력
    printf("조도 값: %.2f lux\n", illuminance);

    // I2C 디바이스 닫기
    close(file);

    return 0;
}
