/*
 * 3LED_pwm_lowfreq.c
 *
 * 라즈베리파이 GPIO를 이용해 3색(RGB) LED를 저주파 PWM 방식으로 제어하는 예제 코드입니다.
 * 각 색상(빨강, 초록, 파랑) LED에 대해 듀티비(밝기)를 개별적으로 설정할 수 있으며,
 * 저주파 PWM을 직접 구현하여 LED의 밝기를 조절합니다.
 *
 * 주요 기능:
 *  - 각 LED의 GPIO 핀을 export/unexport 및 출력 모드로 설정
 *  - 원하는 듀티비(%)로 PWM 신호를 생성하여 LED 밝기 제어
 *  - 반복적으로 LED를 켜고 끄는 방식으로 PWM 동작 구현
 *
 * 사용 예시:
 *  - 빨강 70%, 초록 10%, 파랑 5%의 밝기로 3색 LED를 제어
 *
 * 참고: gpiolib.c 파일에 GPIO 제어 함수가 정의되어 있어야 합니다.
 */
// 3색(RGB) LED를 저주파 PWM 방식으로 제어하는 예제

// 표준 입출력, 파일 제어, 문자열, 유닉스 시스템 호출 관련 헤더 포함
#include <stdio.h>      // printf 등 표준 입출력 함수
#include <stdlib.h>     // 일반 함수들 (예: malloc, free)
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 옵션
#include <unistd.h>     // 유닉스 시스템 호출 (예: usleep)

// GPIO 제어 함수가 정의된 파일 포함
#include "gpiolib.c"

// 단일 LED를 제어하는 함수
// gpio: 제어할 GPIO 핀 번호
// onOff: 1(켜기), 0(끄기)
int ledControl(int gpio, int onOff)
{
    int fd;             // 파일 디스크립터 (사용하지 않음)
    char buf[128];      // 임시 버퍼 (사용하지 않음)

    gpioExport(gpio);       // GPIO 핀을 export하여 사용 가능하게 만듦
    gpioDirection(gpio, 1); // GPIO 핀을 출력 모드로 설정
    gpioWrite(gpio, onOff); // GPIO 핀에 값 쓰기 (LED ON/OFF)
    gpioUnexport(gpio);     // GPIO 핀을 unexport하여 정리

    return 0;               // 정상 종료
}

// RGB LED에 연결된 GPIO 핀 번호 정의
#define gpioR   21 // 빨간색 LED (핀 36)
#define gpioG   20 // 초록색 LED (핀 38)
#define gpioB   16 // 파란색 LED (핀 40)

// RGB LED의 3개 핀을 모두 export하는 함수
void gpioExport3()
{
    gpioExport(gpioR); // 빨간색 LED 핀 export
    gpioExport(gpioG); // 초록색 LED 핀 export
    gpioExport(gpioB); // 파란색 LED 핀 export
}

// RGB LED의 3개 핀을 모두 출력 모드로 설정하는 함수
void gpioDirection3()
{
    gpioDirection(gpioR, 1); // 빨간색 LED 핀 출력
    gpioDirection(gpioG, 1); // 초록색 LED 핀 출력
    gpioDirection(gpioB, 1); // 파란색 LED 핀 출력
}

// RGB LED의 3개 핀을 모두 unexport하는 함수
void gpioUnexport3()
{
    gpioUnexport(gpioR); // 빨간색 LED 핀 unexport
    gpioUnexport(gpioG); // 초록색 LED 핀 unexport
    gpioUnexport(gpioB); // 파란색 LED 핀 unexport
}

// RGB LED를 PWM 방식으로 제어하는 함수
// port1, port2, port3: 각 LED에 연결된 GPIO 핀 번호
// dutyRate1, dutyRate2, dutyRate3: 각 LED의 듀티비(%)
// dutyT: PWM 주기 (기본 단위)
void pwmColorLed(int port1, int port2, int port3, int dutyRate1, int dutyRate2, int dutyRate3, int dutyT)
{
    int cnt;                // 반복 횟수
    int dutyOn1, dutyOn2, dutyOn3, dutyOff; // 각 LED의 ON/OFF 시간

    gpioExport3();          // 3개 핀 모두 export
    gpioDirection3();       // 3개 핀 모두 출력 모드 설정

    cnt = 10;               // 10번 반복
    // 각 LED의 ON 시간 계산 (us 단위)
    dutyOn1 = (dutyT * dutyRate1 / 100) * 100; // 빨간색 LED ON 시간
    dutyOn2 = (dutyT * dutyRate2 / 100) * 100; // 초록색 LED ON 시간
    dutyOn3 = (dutyT * dutyRate3 / 100) * 100; // 파란색 LED ON 시간
    dutyOff = (dutyT * (100 - dutyRate1) / 100) * 100; // OFF 시간 (빨간색 기준)

    while (cnt--) {
        // 3개 LED 모두 켜기
        ledControl(port1, 1); // 빨간색 LED ON
        ledControl(port2, 1); // 초록색 LED ON
        ledControl(port3, 1); // 파란색 LED ON

        usleep(dutyOn3);      // 파란색 LED ON 시간만큼 대기
        ledControl(port3, 0); // 파란색 LED OFF

        usleep(dutyOn2 - dutyOn3); // 초록색 LED ON 시간만큼 추가 대기
        ledControl(port2, 0); // 초록색 LED OFF

        usleep(dutyOn1 - dutyOn2); // 빨간색 LED ON 시간만큼 추가 대기
        ledControl(port1, 0); // 빨간색 LED OFF

        usleep(dutyOff);      // OFF 시간만큼 대기
    }

    gpioUnexport3();          // 3개 핀 모두 unexport
}

// 프로그램의 진입점 (메인 함수)
int main(int argc, char** argv)
{
    int vol; // 듀티비 조절 변수

    printf("Start PWM !!!\n"); // 프로그램 시작 메시지 출력

    vol = 200; // 듀티비 초기값 (퍼센트 단위)
    while (vol) {
        // RGB LED를 PWM 방식으로 제어 (빨강 70%, 초록 10%, 파랑 5%, 주기 3)
        pwmColorLed(21, 20, 16, 70, 10, 5, 3);
        vol = vol - 5; // 듀티비 감소
    }

    return 0; // 프로그램 정상 종료
}
