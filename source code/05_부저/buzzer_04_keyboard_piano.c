ss/*
 * buzzer_04_keyboard_piano.c
 * -----------------
 * 키보드 입력에 따라 부저(Buzzer)로 피아노 음계를 연주하는 예제 코드
 * - 각 키(a,s,d,...)를 누르면 해당 음계(도~높은 시)가 부저로 출력됨
 * - 실시간으로 키 입력을 감지하여 즉시 음을 재생
 * - 라즈베리파이 등 리눅스 기반 GPIO 환경에서 사용
 */

#include <stdio.h>      // 표준 입출력 함수 사용 (printf 등)
#include <stdlib.h>     // 표준 라이브러리 함수 사용
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include <termios.h>    // 터미널 제어 함수 (키 입력 감지)
#include <fcntl.h>      // 파일 제어 관련 함수 사용
#include <signal.h>     // 시그널 처리 함수
#include "gpiolib.c"   // GPIO 제어 함수 포함

#define BUZZER_PIN 5 /* GPIO 19 */ // 부저가 연결된 GPIO 핀 번호 정의

#define NOTE_DO  523 // 도 음의 주파수(Hz)
#define NOTE_RE  587 // 레 음의 주파수(Hz)
#define NOTE_MI  659 // 미 음의 주파수(Hz)
#define NOTE_FA  698 // 파 음의 주파수(Hz)
#define NOTE_SOL 784 // 솔 음의 주파수(Hz)
#define NOTE_LA  880 // 라 음의 주파수(Hz)
#define NOTE_SI  987 // 시 음의 주파수(Hz)
#define NOTE_HIGH_DO 1046 // 높은 도 음의 주파수(Hz)
#define NOTE_HIGH_RE 1175 // 높은 레 음의 주파수(Hz)
#define NOTE_HIGH_MI 1319 // 높은 미 음의 주파수(Hz)
#define NOTE_HIGH_FA 1397 // 높은 파 음의 주파수(Hz)
#define NOTE_HIGH_SOL 1568 // 높은 솔 음의 주파수(Hz)
#define NOTE_HIGH_LA 1760 // 높은 라 음의 주파수(Hz)
#define NOTE_HIGH_SI 1976 // 높은 시 음의 주파수(Hz)
#define REST 0 // 쉼표 (무음)

// 한 음을 재생하는 함수
// note: 재생할 음의 주파수(Hz), duration: 재생 시간(ms)
void playNote(int note, int duration) {
    int period = 1000000 / note; // 한 주기의 마이크로초(us) 계산
    int halfPeriod = period / 2; // 주기의 절반 (ON/OFF 시간)
    int cycles = duration * note / 1000; // 전체 재생 시간 동안 반복할 주기 수 계산
    for (int i = 0; i < cycles; i++) {
        gpioWrite(BUZZER_PIN, 1); // 부저 ON (전압 인가)
        usleep(halfPeriod);       // 절반 주기만큼 대기
        gpioWrite(BUZZER_PIN, 0); // 부저 OFF (전압 제거)
        usleep(halfPeriod);       // 절반 주기만큼 대기
    }
}

// 음을 재생할 때 메시지도 함께 출력하는 함수
// note: 재생할 음의 주파수, duration: 재생 시간, message: 출력할 메시지
void playNoteWithMessage(int note, int duration, char* message) {
    printf("%s\n", message); // 메시지 출력
    playNote(note, duration); // 음 재생
}

// 멜로디(음 배열)를 재생하는 함수 (사용하지 않지만 예시로 포함)
void playMelody(int notes[], int durations[], int numNotes) {
    for (int i = 0; i < numNotes; i++) {
        playNote(notes[i], durations[i]); // 각 음을 지정된 시간만큼 재생
        usleep(50000); // 음 사이에 50ms 쉬어줌 (겹침 방지)
    }
}

// 키보드 입력이 있는지 확인하는 함수
int kbhit(void) {
    struct termios oldt, newt; // 터미널 설정 구조체
    int ch; // 입력 문자
    int oldf; // 파일 제어 플래그

    tcgetattr(STDIN_FILENO, &oldt); // 현재 터미널 속성 저장
    newt = oldt; // 복사
    newt.c_lflag &= ~(ICANON | ECHO); // 입력 버퍼링/에코 비활성화
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 새 속성 적용
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0); // 파일 제어 플래그 저장
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // 논블록 모드로 변경

    ch = getchar(); // 입력 받기

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 원래 터미널 속성 복원
    fcntl(STDIN_FILENO, F_SETFL, oldf); // 파일 제어 플래그 복원

    if (ch != EOF) { // 입력이 있으면
        ungetc(ch, stdin); // 입력 버퍼에 다시 넣기
        return 1; // 입력 있음
    }

    return 0; // 입력 없음
}

int main(void) {
    int length = 100; // 각 음의 기본 재생 시간(ms)
    int gpio = BUZZER_PIN; // 사용할 GPIO 핀 번호
    char input; // 키보드 입력값 저장 변수

    gpioExport(gpio);      // GPIO 핀을 export하여 사용 가능하게 만듦
    gpioDirection(gpio, 1); // GPIO 핀을 출력 모드로 설정

    // 키와 음계 매핑 안내 출력
    printf("a: DO\ts:RE\n");
    printf("d: MI\tf: FA\n");
    printf("g: SOL\th: LA\n");
    printf("j: SI\tq: HIGH_DO\n");
    printf("w: HIGH_RE\te: HIGH_MI\n");
    printf("r: HIGH_FA\tt: HIGH_SOL\n");
    printf("y: HIGH_LA\tu: HIGH_SI\n");

    while (1) { // 무한 반복하며 키 입력을 받아 음 재생
        if (kbhit()) { // 키 입력이 있으면
            input = getchar(); // 입력값 받기

            switch (input) {
            case 'a': playNoteWithMessage(NOTE_DO, length, "Playing NOTE_DO"); break;
            case 's': playNoteWithMessage(NOTE_RE, length, "Playing NOTE_RE"); break;
            case 'd': playNoteWithMessage(NOTE_MI, length, "Playing NOTE_MI"); break;
            case 'f': playNoteWithMessage(NOTE_FA, length, "Playing NOTE_FA"); break;
            case 'g': playNoteWithMessage(NOTE_SOL, length, "Playing NOTE_SOL"); break;
            case 'h': playNoteWithMessage(NOTE_LA, length, "Playing NOTE_LA"); break;
            case 'j': playNoteWithMessage(NOTE_SI, length, "Playing NOTE_SI"); break;
            case 'q': playNoteWithMessage(NOTE_HIGH_DO, length, "Playing HIGH_DO"); break;
            case 'w': playNoteWithMessage(NOTE_HIGH_RE, length, "Playing HIGH_RE"); break;
            case 'e': playNoteWithMessage(NOTE_HIGH_MI, length, "Playing HIGH_MI"); break;
            case 'r': playNoteWithMessage(NOTE_HIGH_FA, length, "Playing HIGH_FA"); break;
            case 't': playNoteWithMessage(NOTE_HIGH_SOL, length, "Playing HIGH_SOL"); break;
            case 'y': playNoteWithMessage(NOTE_HIGH_LA, length, "Playing HIGH_LA"); break;
            case 'u': playNoteWithMessage(NOTE_HIGH_SI, length, "Playing HIGH_SI"); break;

            case 'A': playNoteWithMessage(NOTE_DO, length, "Playing NOTE_DO"); break;
            case 'S': playNoteWithMessage(NOTE_RE, length, "Playing NOTE_RE"); break;
            case 'D': playNoteWithMessage(NOTE_MI, length, "Playing NOTE_MI"); break;
            case 'F': playNoteWithMessage(NOTE_FA, length, "Playing NOTE_FA"); break;
            case 'G': playNoteWithMessage(NOTE_SOL, length, "Playing NOTE_SOL"); break;
            case 'H': playNoteWithMessage(NOTE_LA, length, "Playing NOTE_LA"); break;
            case 'J': playNoteWithMessage(NOTE_SI, length, "Playing NOTE_SI"); break;
            case 'Q': playNoteWithMessage(NOTE_HIGH_DO, length, "Playing HIGH_DO"); break;
            case 'W': playNoteWithMessage(NOTE_HIGH_RE, length, "Playing HIGH_RE"); break;
            case 'E': playNoteWithMessage(NOTE_HIGH_MI, length, "Playing HIGH_MI"); break;
            case 'R': playNoteWithMessage(NOTE_HIGH_FA, length, "Playing HIGH_FA"); break;
            case 'T': playNoteWithMessage(NOTE_HIGH_SOL, length, "Playing HIGH_SOL"); break;
            case 'Y': playNoteWithMessage(NOTE_HIGH_LA, length, "Playing HIGH_LA"); break;
            case 'U': playNoteWithMessage(NOTE_HIGH_SI, length, "Playing HIGH_SI"); break;

            case '\n': goto exit_loop; // 엔터키 입력 시 반복 종료
            default: printf("Invalid input.\n"); break; // 잘못된 입력 처리
            }
        }
    }

exit_loop:
    gpioUnexport(gpio);   // GPIO 핀을 unexport하여 정리
    return 0; // 프로그램 정상 종료
}
