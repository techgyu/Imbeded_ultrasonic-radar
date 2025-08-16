
/*
 * buzzer_05_play_melody_ToMyX.c
 * -----------------------------
 * 가수 경서의 '나의 X에게' 멜로디를 부저(Buzzer)로 연주하는 코드
 * - gpiolib.c의 GPIO 제어 함수 활용
 * - 여러 음계와 멜로디 배열을 순서대로 재생
 * - 라즈베리파이 등 리눅스 기반 GPIO 환경에서 사용
 */

#include <stdio.h>      // 표준 입출력 함수 (printf 등)
#include <stdlib.h>     // 표준 라이브러리 함수
#include <string.h>     // 문자열 처리 함수
#include <fcntl.h>      // 파일 제어 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include <sys/mman.h>   // 메모리 매핑 함수
#include "gpiolib.c"   // GPIO 제어 함수 포함

#define BUZZER_PIN 5 /* GPIO 19: 부저 연결 핀 번호 */

#define NOTE_DO  523 // 도 (C)
#define NOTE_RE  587 // 레 (D)
#define NOTE_MI  659 // 미 (E)
#define NOTE_FA  698 // 파 (F)
#define NOTE_SOL 784 // 솔 (G)
#define NOTE_LA  880 // 라 (A)
#define NOTE_SI  987 // 시 (B)
#define NOTE_HIGH_DO 1046 // 높은 도 (C)
#define NOTE_HIGH_RE 1175 // 높은 레 (D)
#define NOTE_HIGH_MI 1319 // 높은 미 (E)
#define NOTE_HIGH_FA 1397 // 높은 파 (F)
#define NOTE_HIGH_SOL 1568 // 높은 솔 (G)
#define NOTE_HIGH_LA 1760 // 높은 라 (A)
#define NOTE_HIGH_SI 1976 // 높은 시 (B)
#define REST 0 // 쉼표 (무음)


// 한 음을 재생하는 함수
// note: 재생할 음의 주파수(Hz), duration: 재생 시간(ms)
void playNote(int note, int duration) {
    printf("note: %d, duration: %d\n", note, duration); // 현재 재생 중인 음과 시간 출력
    int period = 1000000 / note; // 한 주기의 마이크로초(us) 계산
    int halfPeriod = period / 2; // 주기의 절반 (ON/OFF 시간)
    int cycles = duration * note / 1000; // 전체 재생 시간 동안 반복할 주기 수 계산
    printf("period: %d, halfPeriod: %d, cycles: %d\n", period, halfPeriod, cycles); // 계산값 출력
    for (int i = 0; i < cycles; i++) {
        gpioWrite(BUZZER_PIN, 1); // 부저 ON (전압 인가)
        usleep(halfPeriod);       // 절반 주기만큼 대기
        gpioWrite(BUZZER_PIN, 0); // 부저 OFF (전압 제거)
        usleep(halfPeriod);       // 절반 주기만큼 대기
    }
}


// 멜로디(음 배열)를 재생하는 함수
// notes[]: 음 배열, durations[]: 각 음의 재생 시간 배열, numNotes: 음 개수
void playMelody(int notes[], int durations[], int numNotes) {
    for (int i = 0; i < numNotes; i++) {
        playNote(notes[i], durations[i]); // 해당 음을 지정된 시간만큼 재생
        usleep(0); // 음 사이에 잠깐 쉬어줌 (겹침 방지)
    }
}


int main(void) {
    int length = 200; // 각 음의 기본 재생 시간(ms)

    // 첫 번째 멜로디 음 배열 (솔, 파, 미, 솔, 솔, 미, 레, 쉼표)
    int melody_1[] = { NOTE_SOL, NOTE_FA, NOTE_MI, NOTE_SOL, NOTE_SOL, NOTE_MI, NOTE_RE, REST };
    // 첫 번째 멜로디 각 음의 재생 시간 배열
    int duration_1[] = { length, length, length, length, length, length, length, length / 2 };
    // 첫 번째 멜로디 음 개수
    int numNotes_1 = sizeof(melody_1) / sizeof(melody_1[0]);

    // 두 번째 멜로디 음 배열 (솔, 높은 도, 시, 라, 솔, 미, 쉼표)
    int melody_2[] = { NOTE_SOL, NOTE_HIGH_DO, NOTE_SI, NOTE_LA, NOTE_SOL, NOTE_MI, REST };
    // 두 번째 멜로디 각 음의 재생 시간 배열
    int duration_2[] = { length, length, length, length, length, length, length };
    // 두 번째 멜로디 음 개수
    int numNotes_2 = sizeof(melody_2) / sizeof(melody_2[0]);

    // 세 번째 멜로디 음 배열 (도, 레, 미, ... 등)
    int melody_3[] = { NOTE_DO, NOTE_RE, NOTE_MI, NOTE_RE, NOTE_DO, NOTE_DO, NOTE_FA, NOTE_MI, NOTE_FA, NOTE_SOL, NOTE_RE, NOTE_SOL, NOTE_FA, NOTE_MI, NOTE_SOL, NOTE_SOL, NOTE_MI, NOTE_RE, REST };
    // 세 번째 멜로디 각 음의 재생 시간 배열
    int duration_3[] = { length, length, length, length, length, length, length, length, length, length, length, length, length, length, length, length, length, length, length / 2 };
    // 세 번째 멜로디 음 개수
    int numNotes_3 = sizeof(melody_3) / sizeof(melody_3[0]);

    // 네 번째 멜로디 음 배열 (솔, 높은 도, 시, 라, 높은 도, 솔, 솔, 쉼표)
    int melody_4[] = { NOTE_SOL, NOTE_HIGH_DO, NOTE_SI, NOTE_LA, NOTE_HIGH_DO, NOTE_SOL, NOTE_SOL, REST };
    // 네 번째 멜로디 각 음의 재생 시간 배열
    int duration_4[] = { length, length, length, length, length, length, length };
    // 네 번째 멜로디 음 개수
    int numNotes_4 = sizeof(melody_4) / sizeof(melody_4[0]);

    // 다섯 번째 멜로디 음 배열 (도, 레, 미, ... 등)
    int melody_5[] = { NOTE_DO, NOTE_RE, NOTE_MI, NOTE_RE, NOTE_DO, NOTE_DO, NOTE_FA, NOTE_MI, NOTE_RE, NOTE_DO, NOTE_RE, NOTE_MI, NOTE_DO };
    // 다섯 번째 멜로디 각 음의 재생 시간 배열
    int duration_5[] = { length, length, length, length, length, length, length, length, length, length, length, length, length };
    // 다섯 번째 멜로디 음 개수
    int numNotes_5 = sizeof(melody_5) / sizeof(melody_5[0]);

    int gpio = BUZZER_PIN; // 사용할 GPIO 핀 번호

    gpioExport(gpio);      // GPIO 핀을 export하여 사용 가능하게 만듦
    gpioDirection(gpio, 1); // GPIO 핀을 출력 모드로 설정

    playMelody(melody_1, duration_1, numNotes_1); // 첫 번째 멜로디 재생
    playMelody(melody_2, duration_2, numNotes_2); // 두 번째 멜로디 재생
    playMelody(melody_3, duration_3, numNotes_3); // 세 번째 멜로디 재생
    playMelody(melody_4, duration_4, numNotes_4); // 네 번째 멜로디 재생
    playMelody(melody_5, duration_5, numNotes_5); // 다섯 번째 멜로디 재생

    gpioUnexport(gpio);   // GPIO 핀을 unexport하여 정리

    return 0; // 프로그램 정상 종료
}
