
/*
 * 파일명: 3LED_mix_color.c
 * -----------------
 * RGB LED를 GPIO로 제어하며 PWM 방식으로 다양한 색상 출력 예제
 * - gpiolib.c의 GPIO 제어 함수 활용
 * - 3색(RGB) LED를 PWM으로 밝기/색상 조절
 * - 라즈베리파이 등 리눅스 기반 GPIO 환경에서 사용
 * - 각 줄마다 기능 설명 주석 추가
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)

#include "gpiolib.c"   // GPIO 제어 함수 포함


// 단일 LED를 ON/OFF 제어하는 함수
int ledControl(int gpio, int onOff)
{
    int fd;
    char buf[128];

    gpioExport(gpio);      // 해당 GPIO 핀을 export하여 사용 가능하게 만듦
    gpioDirection(gpio, 1); // GPIO 핀을 출력 모드로 설정
    gpioWrite(gpio, onOff); // GPIO 핀에 값(ON/OFF) 출력
    gpioUnexport(gpio);    // 사용 후 GPIO 핀 unexport로 정리
    return 0;
}


#define gpioR   21 // 빨간색(R) LED GPIO 핀 번호 (pin 36)
#define gpioG   20 // 초록색(G) LED GPIO 핀 번호 (pin 38)
#define gpioB   16 // 파란색(B) LED GPIO 핀 번호 (pin 40)


// RGB 3개 핀을 모두 export
void gpioExport3()
{
    gpioExport(gpioR); // R 핀 export
    gpioExport(gpioG); // G 핀 export
    gpioExport(gpioB); // B 핀 export
}
// RGB 3개 핀을 모두 출력 모드로 설정
void gpioDirection3()
{
    gpioDirection(gpioR, 1); // R 핀 출력
    gpioDirection(gpioG, 1); // G 핀 출력
    gpioDirection(gpioB, 1); // B 핀 출력
}
// RGB 3개 핀을 모두 unexport
void gpioUnexport3()
{
    gpioUnexport(gpioR); // R 핀 unexport
    gpioUnexport(gpioG); // G 핀 unexport
    gpioUnexport(gpioB); // B 핀 unexport
}


// RGB LED를 PWM 방식으로 제어하는 함수
void pwmColorLed(int port1, int port2, int port3, int dutyRate1, int dutyRate2, int dutyRate3, int dutyT)
{
    int cnt; // 반복 횟수
    int dutyOn1, dutyOn2, dutyOn3, dutyOff; // 각 색상별 ON/OFF 시간

    gpioExport3();      // RGB 3개 핀 export
    gpioDirection3();   // RGB 3개 핀 출력 모드 설정

    cnt = 10; // 10회 반복
    dutyOn1 = (dutyT * dutyRate1 / 100) * 1000; // R LED ON 시간 계산
    dutyOn2 = (dutyT * dutyRate2 / 100) * 1000; // G LED ON 시간 계산
    dutyOn3 = (dutyT * dutyRate3 / 100) * 1000; // B LED ON 시간 계산
    dutyOff = (dutyT * (100 - dutyRate1) / 100) * 1000; // OFF 시간 계산
    while (cnt--) {
        ledControl(port1, 1); // R LED ON
        ledControl(port2, 1); // G LED ON
        ledControl(port3, 1); // B LED ON

        usleep(dutyOn3);      // B LED ON 시간만큼 대기
        ledControl(port3, 0); // B LED OFF

        usleep(dutyOn2 - dutyOn3); // G LED ON 시간만큼 대기
        ledControl(port2, 0); // G LED OFF

        usleep(dutyOn1 - dutyOn2); // R LED ON 시간만큼 대기
        ledControl(port1, 0); // R LED OFF

        usleep(dutyOff);      // OFF 시간만큼 대기
    }

    gpioUnexport3(); // RGB 3개 핀 unexport로 정리
}


// 메인 함수: RGB LED PWM 테스트
int main(int argc, char** argv)
{
    int vol; // 밝기 조절용 변수

    printf("Start PWM !!!\n"); // 시작 메시지 출력

    vol = 200; // Duty Rate 초기값
    while (vol) {
        pwmColorLed(21, 20, 16, 70, 10, 5, 3); // RGB LED를 PWM으로 제어
        vol = vol - 5; // Duty Rate 감소
    }

    return 0; // 프로그램 종료
}
