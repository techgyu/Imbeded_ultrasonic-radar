
/*
 * 파일명: Seg7_print_num.c
 * ------
 * 7-Segment LED에 숫자를 표시하는 예제 코드
 * - 각 세그먼트(A~G, DP)를 GPIO로 제어
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


// 지정된 GPIO 핀을 제어하는 함수 (onOff: 1=ON, 0=OFF)
int ledControl(int gpio, int onOff)
{
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
    ledControl(A, 0);
    ledControl(B, 0);
    ledControl(C, 0);
    ledControl(D, 0);
    ledControl(E, 0);
    ledControl(F, 0);
    ledControl(G, 0);
    ledControl(DP, 0);
}


// 입력받은 숫자(num)에 따라 7-Segment LED를 점등하는 함수
void seg7_num( ) {
    int num;
    printf("Input Num: "); // 사용자에게 숫자 입력 요청
    scanf("%d", &num);

    // 입력값에 따라 각 세그먼트 점등
    switch (num) {
    case 0:
        // 0: A, B, C, D, E, F 점등
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        ledControl(F, 1);
        break;
    case 1:
        // 1: B, C 점등
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 2:
        // 2: A, B, G, E, D 점등
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(G, 1);
        ledControl(E, 1);
        ledControl(D, 1);
        break;
    case 3:
        // 3: A, B, G, C, D 점등
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        break;
    case 4:
        // 4: F, G, B, C 점등
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 5:
        // 5: A, F, G, C, D 점등
        ledControl(A, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        break;
    case 6:
        // 6: A, F, G, C, D, E 점등
        ledControl(A, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        break;
    case 7:
        // 7: F, A, B, C 점등
        ledControl(F, 1);
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        break;
    case 8:
        // 8: 모든 세그먼트 점등
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(E, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        break;
    case 9:
        // 9: A, B, C, D, F, G 점등
        ledControl(A, 1);
        ledControl(B, 1);
        ledControl(C, 1);
        ledControl(D, 1);
        ledControl(F, 1);
        ledControl(G, 1);
        break;
    }
}


// 메인 함수: 숫자 입력 받아 7-Segment LED에 표시
int main()
{
    seg7_num(); // 숫자 입력 및 표시
}