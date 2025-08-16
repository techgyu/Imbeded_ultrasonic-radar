
/*
 * 파일명: HR-SR04_Buzzer.c
 * ------------------------
 * HC-SR04 초음파 센서와 부저(Buzzer)를 GPIO로 제어하는 거리 측정/음 출력 예제
 * - gpiolib.c의 GPIO 제어 함수 활용
 * - 거리 측정 후, 거리에 따라 부저 음 높이 변경
 * - 각 함수와 주요 코드에 기능 설명 주석 추가
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include <sys/mman.h>   // 메모리 매핑 함수
#include <signal.h>     // 시그널 처리 함수
#include "gpiolib.c"   // GPIO 제어 함수 포함


#define TRIG_PIN 17    // HC-SR04의 TRIG 핀에 연결된 GPIO 핀 번호
#define ECHO_PIN 18    // HC-SR04의 ECHO 핀에 연결된 GPIO 핀 번호
#define BUZZER_PIN 5   // 부저(Buzzer) 연결 GPIO 핀 번호


#define NOTE_DO  523        // 도
#define NOTE_RE  587        // 레
#define NOTE_MI  659        // 미
#define NOTE_FA  698        // 파
#define NOTE_SOL 784        // 솔
#define NOTE_LA  880        // 라
#define NOTE_SI  987        // 시
#define NOTE_HIGH_DO 1046   // 높은 도
#define NOTE_HIGH_RE 1175   // 높은 레
#define NOTE_HIGH_MI 1319   // 높은 미
#define NOTE_HIGH_FA 1397   // 높은 파
#define NOTE_HIGH_SOL 1568  // 높은 솔
#define NOTE_HIGH_LA 1760   // 높은 라
#define NOTE_HIGH_SI 1976   // 높은 시
#define REST 0              // 쉼표(무음)


// 하나의 음표를 재생하는 함수
// note: 음 높이(주파수), duration: 재생 시간(ms)
void playNote(int note, int duration) {
    printf("note: %d, duration: %d\n", note, duration); // 현재 재생 중인 음과 시간 출력
    if (note == REST) {
        usleep(duration * 1000); // 쉼표는 대기 시간으로 처리
        return;
    }
    int period = 1000000 / note; // 주파수에 따른 주기 계산
    int halfPeriod = period / 2; // 주기의 절반 계산
    int cycles = duration * 1000 / period; // 주어진 시간동안 주파수를 몇 번 재생할 것인지 계산
    for (int i = 0; i < cycles; i++) {
        gpioWrite(BUZZER_PIN, 1); // 부저 ON
        usleep(halfPeriod);       // 반 주기만큼 대기
        gpioWrite(BUZZER_PIN, 0); // 부저 OFF
        usleep(halfPeriod);       // 반 주기만큼 대기
    }
}


// 지정된 GPIO 핀을 활성화하는 함수
void gpio_export(int pin) {
    char buffer[3];
    int len = snprintf(buffer, sizeof(buffer), "%d", pin); // 핀 번호 문자열 변환
    int fd = open("/sys/class/gpio/export", O_WRONLY); // export 파일 열기
    write(fd, buffer, len); // 번호 기록
    close(fd); // 파일 닫기
}

// 지정된 GPIO 핀을 비활성화하는 함수
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
void gpio_write1(int pin, int value) {
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
void setup() {
    gpio_export(TRIG_PIN);      // TRIG 핀 export
    gpio_export(ECHO_PIN);      // ECHO 핀 export
    gpio_export(BUZZER_PIN);    // BUZZER 핀 export
    gpio_direction(TRIG_PIN, 1); // TRIG 핀 출력
    gpio_direction(ECHO_PIN, 0); // ECHO 핀 입력
    gpio_direction(BUZZER_PIN, 1); // BUZZER 핀 출력
}

// GPIO 핀을 비활성화하는 함수
void cleanup() {
    gpio_unexport(TRIG_PIN);   // TRIG 핀 unexport
    gpio_unexport(ECHO_PIN);   // ECHO 핀 unexport
    gpio_unexport(BUZZER_PIN); // BUZZER 핀 unexport
}


// HC-SR04 초음파 센서를 사용하여 거리를 측정하는 함수
double getDistance() {
    struct timeval start, end;

    // TRIG 핀을 HIGH로 설정하여 초음파 펄스 전송
    gpio_write1(TRIG_PIN, 1);
    usleep(10); // 10µs 대기
    gpio_write1(TRIG_PIN, 0);

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


// 측정된 거리에 따라 부저 음을 다르게 출력하는 함수
void beepBasedOnDistance(double distance) {
    int note;

    // 거리에 따라 주파수를 할당 (가까울수록 높은 음)
    if (distance < 50) {
        note = NOTE_HIGH_SI;
    }
    else if (distance < 100) {
        note = NOTE_HIGH_LA;
    }
    else if (distance < 150) {
        note = NOTE_HIGH_SOL;
    }
    else if (distance < 200) {
        note = NOTE_HIGH_FA;
    }
    else if (distance < 250) {
        note = NOTE_HIGH_MI;
    }
    else if (distance < 300) {
        note = NOTE_HIGH_RE;
    }
    else if (distance < 350) {
        note = NOTE_HIGH_DO;
    }
    else if (distance < 400) {
        note = NOTE_SI;
    }
    else if (distance < 450) {
        note = NOTE_LA;
    }
    else if (distance < 500) {
        note = NOTE_SOL;
    }
    else if (distance < 550) {
        note = NOTE_FA;
    }
    else if (distance < 600) {
        note = NOTE_MI;
    }
    else if (distance < 650) {
        note = NOTE_RE;
    }
    else if (distance < 700) {
        note = NOTE_DO;
    }
    else {
        note = REST; // 700cm 이상일 때는 무음
    }

    playNote(note, 200); // 200ms 동안 음 재생
    usleep(200);         // 잠깐 대기
    playNote(note, 200); // 200ms 동안 음 재생
}


// 메인 함수: 거리 측정 및 부저 제어 루프
int main(void) {
    signal(SIGINT, sigint_handler); // SIGINT 시그널 핸들러 등록

    setup(); // GPIO 핀 초기화

    while (1) {
        double distance = getDistance(); // 거리 측정
        printf("Distance: %.2f cm\n", distance); // 측정된 거리 출력
        beepBasedOnDistance(distance); // 거리 기반 부저 제어
        usleep(500000); // 500ms 대기 (0.5초)
    }

    return 0; // 프로그램 종료 (실제로는 무한 루프)
}
