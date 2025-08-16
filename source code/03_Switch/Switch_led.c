
/*
 * 파일명: Switch_led.c
 * -----------------
 * GPIO를 이용해 LED와 스위치를 제어하는 예제 코드
 * - /sys/class/gpio 경로를 직접 접근하여 export, 방향 설정, 읽기, 쓰기, unexport 수행
 * - 스위치 입력에 따라 LED ON/OFF 제어
 * - 각 함수와 주요 코드에 기능 설명 주석 추가
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (open, close, read, write 등)


// GPIO 핀을 export하여 사용 가능하게 만드는 함수
int gpioExport(int gpio) {
	int fd;
	char buf[128];

	fd = open("/sys/class/gpio/export", O_WRONLY); // export 파일 열기
	if(fd < 0) {
		perror("GPIO ERR : export\n"); // 에러 출력
		return(fd);
	}
	sprintf(buf, "%d", gpio); // GPIO 번호 문자열로 변환
	printf("gpio#:%s\n", buf); // 디버그용 출력
	write(fd, buf, strlen(buf)); // 번호 기록
	close(fd); // 파일 닫기
}


// GPIO 핀의 방향을 입력(in) 또는 출력(out)으로 설정하는 함수
int gpioDirection(int gpio, int dir) {
	int fd;
	char buf[128];

	printf("INput gpio_Direction !!!\n"); // 디버그용 출력
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio); // 방향 파일 경로 생성
	fd = open(buf, O_WRONLY); // 방향 파일 열기
	if(fd < 0) {
		perror("GPIO ERR : direction\n"); // 에러 출력
		return(fd);
	}
	if(dir == 0)
		write(fd, "in", 3);  // 입력 모드로 설정
	else
		write(fd, "out", 4); // 출력 모드로 설정
	close(fd); // 파일 닫기
}
 

// GPIO 핀의 값을 읽어오는 함수 (0 또는 1 반환)
int gpioRead(int gpio) {
	int fd;
	char buf[128], inCh;

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio); // value 파일 경로 생성
	fd = open(buf, O_RDONLY); // value 파일 열기
	if(fd < 0) {
		perror("GPIO ERR : value\n"); // 에러 출력
		return(fd);
	}
	read(fd,&inCh,1); // 값 읽기
	printf("INput gpio%d[%x] !!!\n", gpio, inCh); // 디버그용 출력
	inCh = inCh - '0'; // 문자 '0' 또는 '1'을 숫자로 변환
	close(fd); // 파일 닫기
	return(inCh); // 읽은 값 반환
}


// GPIO 핀에 값을 쓰는 함수 (0: LOW, 1: HIGH)
int gpioWrite(int gpio, int val) {
	int fd;
	char buf[128];

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio); // value 파일 경로 생성
	fd = open(buf, O_WRONLY); // value 파일 열기
	if(fd < 0) {
		perror("GPIO ERR : value\n"); // 에러 출력
		return(fd);
	}
	if(val == 0)
		write(fd, "0", 2); // LOW 출력
	else
		write(fd, "1", 2); // HIGH 출력
	close(fd); // 파일 닫기
	return(0); // 성공 반환
}


// GPIO 핀을 unexport하여 사용 해제하는 함수
int gpioUnexport(int gpio) {
	int fd;
	char buf[128];
	fd = open("/sys/class/gpio/unexport", O_WRONLY); // unexport 파일 열기
	if(fd < 0) {
		perror("GPIO ERR : unexport\n"); // 에러 출력
		return(fd);
	}
	sprintf(buf, "%d", gpio); // GPIO 번호 문자열로 변환
	write(fd, buf,strlen(buf)); // 번호 기록
	close(fd); // 파일 닫기
}


// 단일 LED를 ON/OFF 제어하는 함수
int ledControl(int gpio,  int onOff)
{
	int fd;
	char buf[128];

	gpioExport(gpio);      // 해당 GPIO 핀 export
	gpioDirection(gpio, 1); // 출력 모드로 설정
	gpioWrite(gpio, onOff); // 값(ON/OFF) 출력
	gpioUnexport(gpio);    // 사용 후 unexport
	return 0;
}


// 스위치 입력에 따라 LED를 제어하는 함수
int ledSW_Control(int gpio,  int onOff)
{
	int fd;
	char buf[128];
	char inCh;

	gpioExport(gpio);      // 스위치 GPIO 핀 export
	gpioDirection(gpio, 0); // 입력 모드로 설정

	while(1) {
		ledControl(21, 0); // LED OFF
		while(gpioRead(gpio) == 0) // 스위치가 눌리면
			ledControl(21, 1); // LED ON
	}

	gpioUnexport(gpio);   // 사용 후 unexport
	return 0;
}


// 메인 함수: 스위치 입력에 따라 LED 제어 테스트
int main(int argc, char **argv) 
{
	int gno;
	int onOff;
	/*
	if(argc != 3){ // 인자 개수 확인
		printf("Usage: %s GPIO_NO\n", argv[0]);
		return -1;
	}
	gno = atoi(argv[1]); // GPIO 번호 입력
	onOff = atoi(argv[2]); // ON/OFF 값 입력
	*/
	printf("Start Led SW !!!\n"); // 시작 메시지 출력
	ledSW_Control(20, 0); // GPIO20 스위치 입력에 따라 LED 제어
	return 0; // 프로그램 종료
}
