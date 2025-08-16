
/*
 * 파일명: Switch_simple_led.c
 * ------
 * 스위치 입력으로 LED를 제어하는 예제 코드
 * - gpiolib.c의 GPIO 제어 함수 활용
 * - 주요 함수 및 코드에 상세 설명 주석 추가
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 미지원)
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include "gpiolib.c"   // GPIO 제어 함수 포함


// 지정된 GPIO 핀을 제어하는 함수 (onOff: 1=ON, 0=OFF)
int ledControl(int gpio, int onOff)
{
    int fd;
    char buf[128];

    gpioExport(gpio);         // GPIO export
    gpioDirection(gpio, 1);   // 방향 설정 (출력)
    gpioWrite(gpio, onOff);   // LED ON/OFF
    gpioUnexport(gpio);       // GPIO unexport

    return 0;
}


// 스위치 입력으로 LED를 제어하는 함수
// gpio: 스위치가 연결된 GPIO 번호, onOff: LED 초기 상태
int ledSW_Control(int gpio, int onOff) {
    gpioExport(gpio);         // 스위치용 GPIO export
    gpioDirection(gpio, 0);  // 입력으로 설정

    while (1) {
        ledControl(26, 0);   // LED OFF (GPIO 26)
        while (gpioRead(gpio) == 0) // 스위치가 눌린 상태면
            ledControl(26, 1); // LED ON (GPIO 26)
    }

    gpioUnexport(gpio);      // GPIO unexport

    return 0;
}


// 메인 함수: 스위치 입력으로 LED 제어 시작
int main(void) {
    int gpio = 12; // 스위치가 연결된 GPIO 번호
    int onOff = 1; // LED 초기 상태 (1: ON, 0: OFF)

    printf("Start LED Control with Switch !!!\n");

    ledSW_Control(gpio, onOff); // 스위치로 LED 제어

    return 0;
}
