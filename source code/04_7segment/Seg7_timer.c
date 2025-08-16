
/*
 * 파일명: Seg7_timer.c
 * ------
 * 7-Segment LED에 타이머 기능을 구현하는 예제 코드
 * - 스위치 입력으로 타이머 시작/정지
 * - 1초마다 숫자 표시 (0~9)
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


#define A 11   // 세그먼트 A에 연결된 GPIO 번호
#define B 9    // 세그먼트 B에 연결된 GPIO 번호
#define C 10   // 세그먼트 C에 연결된 GPIO 번호
#define D 4    // 세그먼트 D에 연결된 GPIO 번호
#define E 17   // 세그먼트 E에 연결된 GPIO 번호
#define F 27   // 세그먼트 F에 연결된 GPIO 번호
#define G 22   // 세그먼트 G에 연결된 GPIO 번호
#define DP 3   // 소수점(DP)에 연결된 GPIO 번호

int gostop = 0;           // 타이머 제어용 플래그
int timer_running = 0;    // 타이머 실행 여부 플래그


// 지정된 GPIO 핀을 제어하는 함수 (onOff: 1=ON, 0=OFF)
int ledControl(int gpio, int onOff) {
    printf("GPIO %d ", gpio); // 현재 제어 중인 GPIO 번호 출력
    if (onOff == 1) {
        printf("On\n"); // ON 상태 출력
    }
    else {
        printf("Off\n"); // OFF 상태 출력
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


// 모든 세그먼트 LED를 끄는 함수
int seg7_off() {
    ledControl(A, 0); // A OFF
    ledControl(B, 0); // B OFF
    ledControl(C, 0); // C OFF
    ledControl(D, 0); // D OFF
    ledControl(E, 0); // E OFF
    ledControl(F, 0); // F OFF
    ledControl(G, 0); // G OFF
    ledControl(DP, 0); // DP OFF
}


// 입력받은 숫자(num)에 따라 7-Segment LED를 점등하는 함수
void seg7_num() {
    int num;
    printf("Input Num: "); // 사용자에게 숫자 입력 요청
    scanf("%d", &num);

    // 입력값에 따라 각 세그먼트 점등
    switch (num) {
    case 0:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        ledControl(F, 1);
        break;
    case 1:
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 2:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(G, 1);
        ledControl(E, 1);
        ledControl(D, 1);
        break;
    case 3:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        break;
    case 4:
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 5:
        ledControl(A, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        break;
    case 6:
        ledControl(A, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        break;
    case 7:
        ledControl(F, 1);
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 8:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        break;
    case 9:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        break;
    }
}


// 입력받은 숫자(num)에 따라 7-Segment LED를 점등하는 함수 (타이머용)
void seg7_num_second(int num) {
    // 입력값에 따라 각 세그먼트 점등
    switch (num) {
    case 0:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        ledControl(F, 1);
        break;
    case 1:
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 2:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(G, 1);
        ledControl(E, 1);
        ledControl(D, 1);
        break;
    case 3:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        break;
    case 4:
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 5:
        ledControl(A, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        break;
    case 6:
        ledControl(A, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        break;
    case 7:
        ledControl(F, 1);
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 8:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        break;
    case 9:
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        break;
    }
}


// 스위치 입력으로 타이머 시작/정지 제어 함수
void timer_control(int gpio) {
    gpioExport(gpio);         // 스위치용 GPIO export
    gpioDirection(gpio, 0);  // 입력으로 설정

    while (1) {
        if (gpioRead(gpio) == 0) { // 스위치가 눌렸을 때
            printf("on\n");
            timer_running = 1; // 타이머 시작
            gostop = 0;        // gostop 초기화
            while (gpioRead(gpio) == 0) {} // 스위치가 계속 눌린 상태 대기
            printf("off\n");
            timer_running = 0; // 스위치 뗄 때 타이머 정지
        }
    }

    gpioUnexport(gpio);      // GPIO unexport
}



// 타이머 동작: 스위치 입력에 따라 7-Segment에 0~9 표시
void seg7_timer(int gpio) {
    int onoff;
    printf("timer on?"); // 타이머 시작 여부 입력 요청
    scanf("%d", &onoff);

    timer_control(gpio); // 타이머 제어 함수 호출 (스위치 입력)

    if (onoff == 1) {
        while (timer_running) { // 타이머가 실행 중인 동안
            for (int i = 0; i < 10; i++) {
                seg7_num_second(i); // 숫자 표시
                usleep(1000000);    // 1초 대기
                seg7_off();         // 모든 세그먼트 OFF
            }
        }
    }
}


// 메인 함수: 스위치 GPIO를 이용해 타이머 기능 실행
int main() {
    int gpio_switch =  12;      // 스위치에 연결된 GPIO 번호
    seg7_timer(gpio_switch);    // 타이머 기능 실행
    return 0;
}
