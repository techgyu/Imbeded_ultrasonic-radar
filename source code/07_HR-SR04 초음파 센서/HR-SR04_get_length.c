
/*
 * 파일명: HR-SR04_get_length.c
 * -----------------
 * HC-SR04 초음파 센서를 이용해 거리를 측정하는 예제 코드
 * - GPIO를 sysfs로 직접 제어
 * - 측정된 거리를 주기적으로 출력
 * - 각 함수와 주요 코드에 상세 설명 주석 추가
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 미지원)
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include <fcntl.h>      // 파일 제어 함수
#include <sys/time.h>   // 시간 측정 함수
#include <signal.h>     // 시그널 처리 함수


#define TRIG_PIN 21    // HC-SR04의 TRIG 핀에 연결된 GPIO 핀 번호
#define ECHO_PIN 20    // HC-SR04의 ECHO 핀에 연결된 GPIO 핀 번호


// 지정된 GPIO 핀을 활성화하는 함수
// sysfs에 핀 번호를 기록하여 GPIO를 export
void gpio_export(int pin) {
    char buffer[3];
    int len = snprintf(buffer, sizeof(buffer), "%d", pin); // 핀 번호 문자열 변환
    int fd = open("/sys/class/gpio/export", O_WRONLY); // export 파일 열기
    write(fd, buffer, len); // 번호 기록
    close(fd); // 파일 닫기
}


// 지정된 GPIO 핀을 비활성화하는 함수
// sysfs에 핀 번호를 기록하여 GPIO를 unexport
void gpio_unexport(int pin) {
    char buffer[3];
    int len = snprintf(buffer, sizeof(buffer), "%d", pin); // 핀 번호 문자열 변환
    int fd = open("/sys/class/gpio/unexport", O_WRONLY); // unexport 파일 열기
    write(fd, buffer, len); // 번호 기록
    close(fd); // 파일 닫기
}


// 지정된 GPIO 핀의 방향을 설정하는 함수 (0: in, 1: out)
void gpio_direction(int pin, int dir) {
    const char* directions_str[] = { "in", "out" };
    char path[35];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin); // 방향 파일 경로 생성
    int fd = open(path, O_WRONLY); // 방향 파일 열기
    write(fd, directions_str[dir], dir ? 3 : 2); // 방향 기록
    close(fd); // 파일 닫기
}


// 지정된 GPIO 핀의 값을 설정하는 함수 (0: LOW, 1: HIGH)
void gpio_write(int pin, int value) {
    char path[30];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin); // value 파일 경로 생성
    int fd = open(path, O_WRONLY); // value 파일 열기
    if (value)
        write(fd, "1", 1); // HIGH 출력
    else
        write(fd, "0", 1); // LOW 출력
    close(fd); // 파일 닫기
}


// 지정된 GPIO 핀의 값을 읽는 함수 (0 또는 1 반환)
int gpio_read(int pin) {
    char path[30];
    char value_str[3];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin); // value 파일 경로 생성
    int fd = open(path, O_RDONLY); // value 파일 열기
    read(fd, value_str, 3); // 값 읽기
    close(fd); // 파일 닫기
    return atoi(value_str); // 읽은 값 반환
}


// GPIO 핀을 초기화하는 함수
// TRIG/ECHO 핀을 export 및 방향 설정
void setup() {
    gpio_export(TRIG_PIN);      // TRIG 핀 export
    gpio_export(ECHO_PIN);      // ECHO 핀 export
    gpio_direction(TRIG_PIN, 1); // TRIG 핀 출력
    gpio_direction(ECHO_PIN, 0); // ECHO 핀 입력
}


// GPIO 핀을 비활성화하는 함수
// TRIG/ECHO 핀 unexport
void cleanup() {
    gpio_unexport(TRIG_PIN);   // TRIG 핀 unexport
    gpio_unexport(ECHO_PIN);   // ECHO 핀 unexport
}


// HC-SR04 초음파 센서를 사용하여 거리를 측정하는 함수
double getDistance() {
    struct timeval start, end;

    // TRIG 핀을 HIGH로 설정하여 초음파 펄스 전송
    gpio_write(TRIG_PIN, 1);
    usleep(10); // 10µs 대기
    gpio_write(TRIG_PIN, 0);

    // ECHO 핀이 HIGH가 될 때까지 대기 (초음파 펄스 전송 시작)
    while (gpio_read(ECHO_PIN) == 0);

    // start 시간 기록 (초음파 펄스 전송 시작 시간)
    gettimeofday(&start, NULL);

    // ECHO 핀이 LOW가 될 때까지 대기 (초음파 펄스 수신 완료)
    while (gpio_read(ECHO_PIN) == 1);

    // end 시간 기록 (초음파 펄스 수신 완료 시간)
    gettimeofday(&end, NULL);

    // 시간 차 계산 (초 단위)
    double timeElapsed = ((end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec)) / 1000000.0;

    // 거리 계산 (음속 34300 cm/s, 왕복 시간이므로 2로 나눔)
    double distance = (timeElapsed * 34300) / 2.0;

    return distance; // 계산된 거리 반환
}


// SIGINT(CTRL+C) 시그널 핸들러 함수
void sigint_handler(int signum) {
    printf("Interrupt signal received. Cleaning up and exiting...\n"); // 안내 메시지 출력
    cleanup(); // GPIO 핀 비활성화
    exit(0); // 프로그램 종료
}


// 메인 함수: 거리 측정 및 출력 루프
int main(void) {
    signal(SIGINT, sigint_handler); // SIGINT 시그널 핸들러 등록

    setup(); // GPIO 핀 초기화

    while (1) {
        double distance = getDistance(); // 거리 측정
        printf("Distance: %.2f cm\n", distance); // 측정된 거리 출력
        usleep(500000); // 500ms 대기 (0.5초)
    }

    return 0; // 프로그램 종료 (실제로는 무한 루프)
}