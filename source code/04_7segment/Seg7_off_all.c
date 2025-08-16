
/*
 * 파일명: Seg7_off_all.c
 * ------
 * 7-Segment LED의 모든 세그먼트(A~G, DP)를 끄는 예제 코드
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
void seg7_off() {
    ledControl(A, 0); // A OFF
    ledControl(B, 0); // B OFF
    ledControl(C, 0); // C OFF
    ledControl(D, 0); // D OFF
    ledControl(E, 0); // E OFF
    ledControl(F, 0); // F OFF
    ledControl(G, 0); // G OFF
    ledControl(DP, 0); // DP OFF
}


// 메인 함수: 모든 세그먼트 LED를 끄는 동작 수행
int main()
{
    seg7_off(); // 모든 세그먼트 OFF
}