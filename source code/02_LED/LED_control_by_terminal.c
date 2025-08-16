
/*
 * 파일명: LED_control_by_terminal.c
 * -------------------
 * 명령행 인자로 GPIO 번호와 ON/OFF 값을 받아 해당 LED를 제어하는 예제 코드
 * - sysfs를 통한 GPIO 제어
 * - 각 함수와 주요 코드에 상세 설명 주석 추가
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 미지원)
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (open, write, close 등)


// 지정된 GPIO 핀을 제어하는 함수 (onOff: 1=ON, 0=OFF)
int ledControl(int gpio,  int onOff)
{
	int fd;
	char buf[128];

	fd = open("/sys/class/gpio/export", O_WRONLY); // GPIO export
	sprintf(buf, "%d", gpio); // GPIO 번호 문자열 변환
	write(fd, buf, strlen(buf));
	close(fd);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY); // 방향 설정
	write(fd, "out", 4);      // 출력 모드
	close(fd);

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(buf, O_WRONLY); // 값 설정
	if( onOff == 1)
		write(fd,"1",2);     // LED ON
	else
		write(fd,"0",2);     // LED OFF
	close(fd);
	getchar(); // 키보드 입력 대기 (LED 상태 확인용)

	fd = open("/sys/class/gpio/unexport", O_WRONLY); // GPIO unexport
	sprintf(buf, "%d", gpio);
	write(fd, buf,strlen(buf));
	close(fd);
	return 0;
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