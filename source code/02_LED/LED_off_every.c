
/*
 * 파일명: LED_off_every.c
 * ----------------------
 * 모든 GPIO LED를 한 번에 끄는 예제 코드
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


// 모든 LED를 한 번에 끄는 함수
int led_off_every() {
    printf("EVERY LED OFF!!!\n"); // 안내 메시지 출력
    // 각 GPIO 번호에 대해 LED OFF 실행
    ledControl(2, 0);
    ledControl(3, 0);
    ledControl(4, 0);
    ledControl(5, 0);
    ledControl(6, 0);
    ledControl(7, 0);
    ledControl(8, 0);
    ledControl(9, 0);
    ledControl(10, 0);
    ledControl(11, 0);
    ledControl(12, 0);
    ledControl(13, 0);
    ledControl(14, 0);
    ledControl(15, 0);
    ledControl(16, 0);
    ledControl(17, 0);
    ledControl(18, 0);
    ledControl(19, 0);
    ledControl(20, 0);
    ledControl(21, 0);
    ledControl(22, 0);
    ledControl(23, 0);
    ledControl(24, 0);
    ledControl(26, 0);
    ledControl(27, 0);

    return 0;
}


// 메인 함수: 모든 LED를 끄는 함수 호출
int main()
{
    led_off_every(); // LED OFF 실행
    return 0;
}
