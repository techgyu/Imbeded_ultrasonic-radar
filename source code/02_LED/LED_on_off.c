
/*
 * 파일명: LED_on_off.c
 * -------------------
 * 명령행 인자로 GPIO 번호와 ON/OFF 값을 받아 해당 LED를 제어하는 예제 코드
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
int ledControl(int gpio,  int onOff)
{
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

// 메인 함수: 명령행 인자로 받은 GPIO 번호와 ON/OFF 값으로 LED 제어
// Usage : ./ledgpio 21 1/0
int main(int argc, char **argv) 
{
	int gno;
	int onOff;
 
	if(argc != 3){
		printf("Usage: %s GPIO_NO ON_OFF\n", argv[0]); // 사용법 안내
		return -1;
	}
 
	gno = atoi(argv[1]);      // 첫 번째 인자: GPIO 번호
	onOff = atoi(argv[2]);    // 두 번째 인자: ON/OFF 값
	ledControl(gno, onOff);   // LED 제어 함수 호출
 
	return 0;
}
