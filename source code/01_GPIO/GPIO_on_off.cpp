
/*
 * 파일명: GPIO_on_off.cpp
 * ----------------------
 * 리눅스 시스템에서 GPIO 핀을 쉘 명령어로 ON/OFF 제어하는 예제
 * - popen을 이용해 /sys/class/gpio 경로에 echo 명령어 실행
 * - GPIO 핀 export, 방향 설정, 값 제어, unexport 기능 포함
 * - 각 줄마다 기능 설명 주석 추가
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수


// 지정한 GPIO 핀을 ON(출력 HIGH)으로 설정하는 함수
void gpio_on(int pin) {
    FILE* fp;           // 명령 실행용 파일 포인터
    char buffer[256];   // 명령어 문자열 버퍼

    // GPIO 핀 export (사용 가능하게 만듦)
    sprintf(buffer, "echo \"%d\" > /sys/class/gpio/export", pin);
    fp = popen(buffer, "r");
    pclose(fp);

    // GPIO 핀을 출력 모드로 설정
    sprintf(buffer, "echo \"out\" > /sys/class/gpio/gpio%d/direction", pin);
    fp = popen(buffer, "r");
    pclose(fp);

    // GPIO 핀에 HIGH(1) 출력
    sprintf(buffer, "echo \"1\" > /sys/class/gpio/gpio%d/value", pin);
    fp = popen(buffer, "r");
    pclose(fp);
}


// 지정한 GPIO 핀을 OFF(출력 LOW)로 설정하고 해제하는 함수
void gpio_off(int pin) {
    FILE* fp;           // 명령 실행용 파일 포인터
    char buffer[256];   // 명령어 문자열 버퍼

    // GPIO 핀에 LOW(0) 출력
    sprintf(buffer, "echo \"0\" > /sys/class/gpio/gpio%d/value", pin);
    fp = popen(buffer, "r");
    pclose(fp);

    // GPIO 핀 unexport (사용 해제)
    sprintf(buffer, "echo \"%d\" > /sys/class/gpio/unexport", pin);
    fp = popen(buffer, "r");
    pclose(fp);
}


// 메인 함수: GPIO ON/OFF 테스트
int main() {
    int gpio_pin = 18; // 사용할 GPIO 핀 번호

    gpio_on(gpio_pin); // GPIO를 ON(HIGH)으로 설정
    printf("GPIO가 켜졌습니다.\n"); // 안내 메시지 출력
    getchar(); // 사용자 입력 대기 (엔터 입력 시 진행)
    gpio_off(gpio_pin); // GPIO를 OFF(LOW)로 설정 및 해제
    printf("GPIO가 꺼졌습니다.\n"); // 안내 메시지 출력

    return 0; // 프로그램 종료
}
