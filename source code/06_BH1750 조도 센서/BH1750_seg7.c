
/*
 * 파일명: BH1750_seg7.c
 * ------
 * BH1750 광센서(I2C)로 측정한 조도(lux)를 7-Segment LED와 부저로 출력하는 예제 코드
 * - gpiolib.c의 GPIO 제어 함수 활용
 * - 주요 함수 및 코드에 상세 설명 주석 추가
 * - 센서값에 따라 7-Segment에 숫자 표시, 부저로 음계 출력
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 미지원)
 */

#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include <sys/mman.h>   // 메모리 매핑
#include <linux/i2c-dev.h> // I2C 디바이스 제어
#include <sys/ioctl.h>  // IO 제어
#include <unistd.h>     // usleep 등
#include <signal.h>     // 시그널 처리
#include "gpiolib.c"   // GPIO 제어 함수 포함


#define BH1750_ADDR 0x23 // BH1750 광센서 I2C 주소


#define A 11   // 세그먼트 A에 연결된 GPIO 번호
#define B 9    // 세그먼트 B에 연결된 GPIO 번호
#define C 10   // 세그먼트 C에 연결된 GPIO 번호
#define D 4    // 세그먼트 D에 연결된 GPIO 번호
#define E 17   // 세그먼트 E에 연결된 GPIO 번호
#define F 27   // 세그먼트 F에 연결된 GPIO 번호
#define G 22   // 세그먼트 G에 연결된 GPIO 번호
#define DP 14  // 소수점(DP)에 연결된 GPIO 번호


#define BUZZER_PIN 5 // 부저에 연결된 GPIO 번호


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
#define REST 0              // 쉼표


int gpio = BUZZER_PIN; // 부저 제어용 GPIO


// 한 음을 지정된 시간만큼 부저로 출력하는 함수
void playNote(int note, int duration) {
    int period = 1000000 / note; // 한 주기(μs) 계산
    int halfPeriod = period / 2; // 반 주기(μs)
    int cycles = duration * note / 1000; // 전체 반복 횟수 계산
    for (int i = 0; i < cycles; i++) {
        gpioWrite(BUZZER_PIN, 1); // 부저 ON
        usleep(halfPeriod);       // 반 주기 대기
        gpioWrite(BUZZER_PIN, 0); // 부저 OFF
        usleep(halfPeriod);       // 반 주기 대기
    }
}


// 음계와 메시지를 함께 출력하는 함수
void playNoteWithMessage(int note, int duration, char* message) {
    printf("%s\n", message); // 메시지 출력
    playNote(note, duration); // 음계 출력
}


// 여러 음을 순서대로 연주하는 함수
void playMelody(int notes[], int durations[], int numNotes) {
    for (int i = 0; i < numNotes; i++) {
        playNote(notes[i], durations[i]); // 각 음 연주
        usleep(50000); // 음 사이 간격
    }
}


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
    ledControl(A, 0); // A OFF
    ledControl(B, 0); // B OFF
    ledControl(C, 0); // C OFF
    ledControl(D, 0); // D OFF
    ledControl(E, 0); // E OFF
    ledControl(F, 0); // F OFF
    ledControl(G, 0); // G OFF
    ledControl(DP, 0); // DP OFF
}


// 입력받은 숫자(num)에 따라 7-Segment LED 점등 및 부저 음계 출력
void seg7_num(int num) {
    int length = 100; // 각 음계의 연주 시간(ms)

    switch (num) {
    case 0:
        playNoteWithMessage(NOTE_DO, length, "Playing NOTE_DO");
        ledControl(A, 1); ledControl(B, 1); ledControl(C, 1); ledControl(D, 1); ledControl(E, 1); ledControl(F, 1); ledControl(G, 0); ledControl(DP, 0);
        break;
    case 1:
        playNoteWithMessage(NOTE_RE, length, "Playing NOTE_RE");
        ledControl(A, 0); ledControl(B, 1); ledControl(C, 1); ledControl(D, 0); ledControl(E, 0); ledControl(F, 0); ledControl(G, 0); ledControl(DP, 0);
        break;
    case 2:
        playNoteWithMessage(NOTE_MI, length, "Playing NOTE_MI");
        ledControl(A, 1); ledControl(B, 1); ledControl(C, 0); ledControl(D, 1); ledControl(E, 1); ledControl(F, 0); ledControl(G, 1); ledControl(DP, 0);
        break;
    case 3:
        playNoteWithMessage(NOTE_FA, length, "Playing NOTE_FA");
        ledControl(A, 1); ledControl(B, 1); ledControl(C, 1); ledControl(D, 1); ledControl(E, 0); ledControl(F, 0); ledControl(G, 1); ledControl(DP, 0);
        break;
    case 4:
        playNoteWithMessage(NOTE_SOL, length, "Playing NOTE_SOL");
        ledControl(A, 0); ledControl(B, 1); ledControl(C, 1); ledControl(D, 0); ledControl(E, 0); ledControl(F, 1); ledControl(G, 1); ledControl(DP, 0);
        break;
    case 5:
        playNoteWithMessage(NOTE_LA, length, "Playing NOTE_LA");
        ledControl(A, 1); ledControl(B, 0); ledControl(C, 1); ledControl(D, 1); ledControl(E, 0); ledControl(F, 1); ledControl(G, 1); ledControl(DP, 0);
        break;
    case 6:
        playNoteWithMessage(NOTE_SI, length, "Playing NOTE_SI");
        ledControl(A, 1); ledControl(B, 0); ledControl(C, 1); ledControl(D, 1); ledControl(E, 1); ledControl(F, 1); ledControl(G, 1); ledControl(DP, 0);
        break;
    case 7:
        playNoteWithMessage(NOTE_HIGH_DO, length, "Playing NOTE_HIGH_DO");
        ledControl(A, 1); ledControl(B, 1); ledControl(C, 1); ledControl(D, 0); ledControl(E, 0); ledControl(F, 1); ledControl(G, 1); ledControl(DP, 0);
        break;
    case 8:
        playNoteWithMessage(NOTE_HIGH_RE, length, "Playing NOTE_HIGH_RE");
        ledControl(A, 1); ledControl(B, 1); ledControl(C, 1); ledControl(D, 1); ledControl(E, 1); ledControl(F, 1); ledControl(G, 1); ledControl(DP, 0);
        break;
    case 9:
        playNoteWithMessage(NOTE_HIGH_MI, length, "Playing NOTE_HIGH_MI");
        ledControl(A, 1); ledControl(B, 1); ledControl(C, 1); ledControl(D, 1); ledControl(E, 0); ledControl(F, 1); ledControl(G, 1); ledControl(DP, 0);
        break;
    }
}


// 메인 함수: BH1750 센서값을 읽어 7-Segment와 부저로 출력
int main()
{
    gpioExport(gpio);         // 부저용 GPIO export
    gpioDirection(gpio, 1);   // 부저 GPIO 출력 설정

    int fd;                   // I2C 디바이스 파일 디스크립터
    int luxRaw, temp_seg7;    // 센서 raw값, 7-Segment 표시값
    float lux;                // lux 변환값
    unsigned char buf[3] = { 0 }; // I2C 통신용 버퍼

    // I2C 디바이스 오픈
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        perror("open");
        exit(1);
    }

    // I2C 슬레이브 주소 설정
    if (ioctl(fd, I2C_SLAVE, BH1750_ADDR) < 0) {
        perror("ioctl");
        exit(1);
    }

    while (1) {
        // 센서 측정 명령 전송
        buf[0] = 0x23; // One time L-Resolution measurement
        if (write(fd, buf, 1) != 1) {
            perror("write");
            exit(1);
        }

        // 측정 준비 대기 (120ms)
        usleep(120000);

        // 측정값 읽기
        if (read(fd, buf, 2) != 2) {
            perror("read");
            exit(1);
        }
        // raw값을 lux로 변환
        luxRaw = (buf[0] << 8) | buf[1];
        lux = (float)luxRaw / 1.2;

        // lux값 출력 및 7-Segment 표시값 계산
        printf("Lux: %d(%7.3f) :: %x, %x\n", luxRaw, lux, buf[0], buf[1]);
        temp_seg7 = luxRaw / 6553;
        if (temp_seg7 > 9) {
            temp_seg7 = 9;
        }
        seg7_num(temp_seg7); // 7-Segment 및 부저 출력

        usleep(1000); // 1ms 대기
    }
    // I2C 디바이스 닫기
    close(fd);
    return 0;
}
