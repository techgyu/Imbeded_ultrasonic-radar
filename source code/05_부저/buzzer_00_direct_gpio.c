/*
 * buzzer_00_direct_gpio.c
 * -----------------
 * 라즈베리파이의 /dev/mem을 직접 접근하여 GPIO를 제어, 부저(Buzzer)로 음계 연주하는 예제 코드
 * - 메모리 매핑 방식으로 GPIO 레지스터 직접 제어
 * - 여러 음계(도~시)를 반복적으로 연주
 * - 피에조 부저의 진동수 및 옥타브 계산 함수 예시 포함
 */

#include <stdio.h>      // 표준 입출력 함수 사용
#include <stdlib.h>     // 표준 라이브러리 함수 사용
#include <fcntl.h>      // 파일 제어 관련 함수 사용
#include <sys/mman.h>   // 메모리 매핑 관련 함수 사용
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)

#define BCM2837_PERI_BASE 0x3F000000 // 라즈베리파이3의 주변장치 베이스 주소
#define GPIO_BASE (BCM2837_PERI_BASE + 0x200000) /* GPIO 컨트롤러 베이스 주소 */
#define BUZZER_PIN 18 /* GPIO 18, 라즈베리파이 40핀 헤더의 12번 핀 */
#define NOTE_C4  262 // 도(C4) 음의 주파수(Hz)
#define NOTE_D4  294 // 레(D4) 음의 주파수(Hz)
#define NOTE_E4  330 // 미(E4) 음의 주파수(Hz)
#define NOTE_F4  349 // 파(F4) 음의 주파수(Hz)
#define NOTE_G4  392 // 솔(G4) 음의 주파수(Hz)
#define NOTE_A4  440 // 라(A4) 음의 주파수(Hz)
#define NOTE_B4  494 // 시(B4) 음의 주파수(Hz)

// GPIO 핀에 값을 쓰는 함수 (메모리 매핑 방식)
void gpio_set_value(int gpio, int value) {
  int fd, offset; // 파일 디스크립터, 오프셋
  void *gpio_map; // 매핑된 메모리 주소
  volatile unsigned *gpio_reg; // GPIO 레지스터 포인터
  fd = open("/dev/mem", O_RDWR | O_SYNC); // /dev/mem 파일 열기
  gpio_map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE); // GPIO 레지스터 매핑
  gpio_reg = (volatile unsigned *)gpio_map; // 포인터 변환
  offset = gpio / 32; // 핀 번호에 따른 오프셋 계산
  if (value)
    *(gpio_reg + 7 + offset) = 1 << (gpio % 32); // 핀 ON
  else
    *(gpio_reg + 10 + offset) = 1 << (gpio % 32); // 핀 OFF
  munmap(gpio_map, 4096); // 메모리 매핑 해제
  close(fd); // 파일 닫기
}

// 부저로 한 음을 재생하는 함수
// note: 재생할 음의 주파수(Hz), duration: 재생 시간(ms)
void playNote(int note, int duration) {
  int period = 1000000 / note; // 한 주기의 마이크로초(us) 계산
  int halfPeriod = period / 2; // 주기의 절반 (ON/OFF 시간)
  int cycles = duration * note / 1000; // 전체 재생 시간 동안 반복할 주기 수 계산
  for (int i = 0; i < cycles; i++) {
    gpio_set_value(BUZZER_PIN, 1); // 부저 ON
    usleep(halfPeriod);            // 절반 주기만큼 대기
    gpio_set_value(BUZZER_PIN, 0); // 부저 OFF
    usleep(halfPeriod);            // 절반 주기만큼 대기
  }
}

// 멜로디(음 배열)를 재생하는 함수
// notes[]: 음 배열, durations[]: 각 음의 재생 시간 배열, numNotes: 음 개수
void playMelody(int notes[], int durations[], int numNotes) {
  for (int i = 0; i < numNotes; i++) {
    playNote(notes[i], durations[i]); // 해당 음을 지정된 시간만큼 재생
    usleep(10000); // 음 사이에 10ms 쉬어줌 (겹침 방지)
  }
}

int main(void) {
  gpio_set_value(BUZZER_PIN, 0); // 부저 OFF (초기화)
  usleep(1000000); // 1초 대기 후 멜로디 시작
  int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4 }; // 연주할 음 배열
  int durations[] = { 200, 200, 200, 200, 200, 200, 200 }; // 각 음의 재생 시간(ms)
  int numNotes = sizeof(melody) / sizeof(melody[0]); // 음 개수 계산
  playMelody(melody, durations, numNotes); // 멜로디 재생
  gpio_set_value(BUZZER_PIN, 0); // 멜로디 끝나면 부저 OFF
  return 0; // 프로그램 정상 종료
}

// 화음별 피에조 부저 진동수 정의 및 옥타브에 따른 값 변경 함수 예시
double ledcWriteNote(note_t note, uint8_t octave){
  const uint16_t noteFrequencyBase[12] = {
  //  1      2      3      4      5      6     7       8     9       10    11      12
  //  C      C#     D      Eb     E      F     F#      G     G#      A     Bb      B
     4186,  4435,  4699,  4978,  5274,  5588,  5920,  6272,  6645,  7040,  7459,  7902
  };
  if(octave > 8 || note >= NOTE_MAX){ // 옥타브 범위 및 note 값 체크
    return 0;
  }
  double noteFreq =  (double)noteFrequencyBase[note] / (double)(1 << (8-octave)); // 옥타브에 따른 주파수 계산
  return noteFreq;
}

int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};  // 4옥타브 음 배열 예시

#define C 262   // 도
#define D 294   // 레
#define E 330   // 미
#define _F 349  // 파
#define G 392   // 솔
#define A 440   // 라
#define B 494   // 시
#define H 523   // 높은 도