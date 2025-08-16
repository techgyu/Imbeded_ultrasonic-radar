/*
 * buzzer_02_basic_note.c
 * -----------------
 * 부저(Buzzer)를 GPIO로 제어하여 단순 음계(도~도)를 연주하는 예제 코드
 * - 각 음계에 해당하는 주파수로 부저를 PWM 방식으로 울림
 * - 한 멜로디 배열을 순서대로 재생
 * - 라즈베리파이 등 리눅스 기반 GPIO 환경에서 사용
 */

#include <stdio.h>      // 표준 입출력 함수 사용 (printf 등)
#include <stdlib.h>     // 표준 라이브러리 함수 사용
#include <string.h>     // 문자열 처리 함수 사용
#include <fcntl.h>      // 파일 제어 관련 함수 사용
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include <sys/mman.h>   // 메모리 매핑 관련 함수 사용
#include "gpiolib.c"   // GPIO 제어 함수 포함

#define BUZZER_PIN 5 /* GPIO 5 */ // 부저가 연결된 GPIO 핀 번호 정의

#define NOTE_C4  100 // 도(C4) 음의 주파수(Hz) - 실제 음계와 다름, 예제용
#define NOTE_D4  200 // 레(D4) 음의 주파수(Hz)
#define NOTE_E4  300 // 미(E4) 음의 주파수(Hz)
#define NOTE_F4  400 // 파(F4) 음의 주파수(Hz)
#define NOTE_G4  500 // 솔(G4) 음의 주파수(Hz)
#define NOTE_A4  600 // 라(A4) 음의 주파수(Hz)
#define NOTE_B4  700 // 시(B4) 음의 주파수(Hz)
#define NOTE_C5  800 // 높은 도(C5) 음의 주파수(Hz)
/* 실제 음계 주파수로 사용하려면 아래 주석을 사용
#define NOTE_C4  262 // 도(C4) 실제 주파수(Hz)
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
*/

// 부저로 한 음을 재생하는 함수
// note: 재생할 음의 주파수(Hz), duration: 재생 시간(ms)
void playNote(int note, int duration) {
    printf("note: %d, duration: %d\n", note, duration); // 현재 재생 중인 음과 시간 출력
    int period = 1000000 / note; // 한 주기의 마이크로초(us) 계산
    int halfPeriod = period / 2; // 주기의 절반 (ON/OFF 시간)
    int cycles = duration * note / 1000; // 전체 재생 시간 동안 반복할 주기 수 계산
    printf("period: %d, halfPeriod: %d, cycles: %d\n", period, halfPeriod, cycles); // 계산값 출력
    for (int i = 0; i < cycles; i++) { // 주기 수만큼 반복
        gpioWrite(BUZZER_PIN, 1); // 부저 ON (전압 인가)
        usleep(halfPeriod);       // 절반 주기만큼 대기
        gpioWrite(BUZZER_PIN, 0); // 부저 OFF (전압 제거)
        usleep(halfPeriod);       // 절반 주기만큼 대기
    }
}

// 멜로디(음 배열)를 재생하는 함수
// notes[]: 음 배열, durations[]: 각 음의 재생 시간 배열, numNotes: 음 개수
void playMelody(int notes[], int durations[], int numNotes) {
    for (int i = 0; i < numNotes; i++) { // 모든 음에 대해 반복
        playNote(notes[i], durations[i]); // 해당 음을 지정된 시간만큼 재생
        usleep(500000); // 음 사이에 500ms 쉬어줌 (겹침 방지)
    }
}

int main(void) {
    // (참고) 부저를 미리 끄고 1초 대기하는 코드 (주석 처리)
    // gpio_set_value(BUZZER_PIN, 0); /* 핀을 출력 모드로 설정하고 부저 끔 */
    // usleep(1000000); /* 1초 대기 후 멜로디 재생 시작 */

    int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5}; // 연주할 음 배열
    int durations[] = { 500, 500, 500, 500, 500, 500, 500 }; // 각 음의 재생 시간(ms)
    int numNotes = sizeof(melody) / sizeof(melody[0]); // 음 개수 계산

    int gpio = BUZZER_PIN; // 사용할 GPIO 핀 번호

    gpioExport(gpio);      // GPIO 핀을 export하여 사용 가능하게 만듦
    gpioDirection(gpio, 1); // GPIO 핀을 출력 모드로 설정

    playMelody(melody, durations, numNotes); // 멜로디 재생

    gpioUnexport(gpio);   // GPIO 핀을 unexport하여 정리

    return 0; // 프로그램 정상 종료
}

