
/*
 * 파일명: GPIO_read_12.c
 * ------
 * GPIO 12번 핀의 입력값을 sysfs로 읽어 반복 출력하는 예제 코드
 * - GPIO export, 방향 설정, 값 읽기
 * - 주요 코드와 연산에 상세 설명 주석 추가
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 및 sysfs 경로 미지원)
 */

#include <stdio.h>      // 표준 입출력 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)


#define GPIO_PIN 12 // 사용할 GPIO 핀 번호


int main() {
    int fd;
    char value;

    // GPIO export: sysfs에 GPIO 12번 등록
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "12", 2);
    close(fd);

    // GPIO 방향 설정: 입력(in)으로 설정
    fd = open("/sys/class/gpio/gpio12/direction", O_WRONLY);
    write(fd, "in", 3);
    close(fd);

    while (1) {
        // GPIO 값 읽기: 현재 입력 상태를 읽음
        fd = open("/sys/class/gpio/gpio12/value", O_RDONLY);
        read(fd, &value, 1);
        close(fd);

        // 읽은 값 출력 (0: LOW, 1: HIGH)
        printf("GPIO 입력 값: %c\n", value);

        usleep(500000); // 0.5초 대기
    }

    return 0;
}
