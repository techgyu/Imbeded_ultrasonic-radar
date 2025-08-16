
/*
 * 파일명: LED_off_single.c
 * -------------------------
 * 특정 GPIO LED 여러 개를 한 번에 끄는 예제 코드
 * - gpiolib.c의 GPIO 제어 함수 활용
 * - 각 함수와 주요 코드에 상세 설명 주석 추가
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
    printf("GPIO %d ", gpio); // 제어할 GPIO 번호 출력
    if (onOff == 1) {
        printf("On\n"); // ON 안내
    }
    else {
        printf("Off\n"); // OFF 안내
    }

    gpioExport(gpio);         // GPIO export
    gpioDirection(gpio, 4);   // 방향 설정 (출력)

    if (onOff == 1) {
        gpioWrite(gpio, 1);   // LED ON
    }
    else {
        gpioWrite(gpio, 0);   // LED OFF
    }

    gpioUnexport(gpio);       // GPIO unexport
}


// 특정 LED만 한 번에 끄는 함수
int led_off_single() {
    printf("EVERY SINGLE LED OFF!!!\n"); // 안내 메시지 출력
    ledControl(26, 0); // GPIO 26 LED OFF
    ledControl(19, 0); // GPIO 19 LED OFF
    ledControl(13, 0); // GPIO 13 LED OFF
    ledControl(6, 0);  // GPIO 6 LED OFF

    return 0;
}


// 메인 함수: 특정 LED를 끄는 함수 호출
int main()
{
    led_off_single(); // LED OFF 실행
}
