
/*
 * 파일명: LED_on_off_each_one.c
 * ---------------------------
 * 여러 GPIO LED를 하나씩 켜고 끄는 테스트 예제 코드
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


// 여러 LED를 하나씩 켜고 끄는 테스트 함수
void led_test_each_one() {
    int sleep_time = 1; // 각 LED ON/OFF 후 대기 시간(초)

    // 각 GPIO LED를 순서대로 켬
    ledControl(21, 1); sleep(sleep_time);
    ledControl(20, 1); sleep(sleep_time);
    ledControl(16, 1); sleep(sleep_time);
    ledControl(26, 1); sleep(sleep_time);
    ledControl(19, 1); sleep(sleep_time);
    ledControl(13, 1); sleep(sleep_time);
    ledControl(6, 1);  sleep(sleep_time);

    // 각 GPIO LED를 순서대로 끔
    ledControl(21, 0); sleep(sleep_time);
    ledControl(20, 0); sleep(sleep_time);
    ledControl(16, 0); sleep(sleep_time);
    ledControl(26, 0); sleep(sleep_time);
    ledControl(19, 0); sleep(sleep_time);
    ledControl(13, 0); sleep(sleep_time);
    ledControl(6, 0);  sleep(sleep_time);
}


// 메인 함수: LED 테스트 함수 호출
int main()
{
    led_test_each_one(); // LED ON/OFF 테스트 실행
}
