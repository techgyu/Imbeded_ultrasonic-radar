/*
 * buzzer_01_onoff.c
 * -----------------
 * GPIO 파일 시스템을 직접 제어하여 부저(Buzzer)를 켜고 끄는 예제 코드
 * - /sys/class/gpio 경로의 파일을 직접 열고, 쓰고, 닫는 방식
 * - 1초 간격으로 부저를 반복적으로 켜고 끔
 * - wiringPi 라이브러리 예제도 하단에 주석으로 포함
 */

#include <stdio.h>      // 표준 입출력 함수 사용 (printf, perror 등)
#include <stdlib.h>     // 표준 라이브러리 함수 사용 (exit 등)
#include <fcntl.h>      // 파일 제어 관련 함수 사용 (open 등)
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)

#define BUZZER_PIN "5" // 부저가 연결된 GPIO 핀 번호 (문자열로 사용)
#define DURATION 1000000 // 부저 음의 지속 시간(us), 1초

int main(void)
{
    int fd, ret;           // 파일 디스크립터, 반환값
    char buf[256];         // 경로 및 데이터 임시 저장 버퍼

    // 1. GPIO export 파일 열기 (핀 활성화)
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {          // 파일 열기 실패 시 오류 출력 후 종료
        perror("open");
        exit(1);
    }

    // 2. export 파일에 핀 번호 쓰기 (핀 등록)
    snprintf(buf, sizeof(buf), BUZZER_PIN); // 핀 번호를 버퍼에 저장
    ret = write(fd, buf, strlen(buf));      // 버퍼 내용을 파일에 쓰기
    if (ret < 0) {         // 쓰기 실패 시 오류 출력 후 종료
        perror("write");
        exit(1);
    }

    // 3. export 파일 닫기
    close(fd);

    // 4. direction 파일 열기 (핀 방향 설정)
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%s/direction", BUZZER_PIN); // 경로 생성
    fd = open(buf, O_WRONLY); // direction 파일 열기
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    // 5. direction 파일에 "out" 쓰기 (출력 모드)
    ret = write(fd, "out", 3);
    if (ret < 0) {
        perror("write");
        exit(1);
    }

    // 6. direction 파일 닫기
    close(fd);

    // 7. 무한 반복하며 부저 ON/OFF
    while (1) {
        // 7-1. value 파일 열기 (부저 ON)
        snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%s/value", BUZZER_PIN); // 경로 생성
        fd = open(buf, O_WRONLY); // value 파일 열기
        if (fd < 0) {
            perror("open");
            exit(1);
        }

        ret = write(fd, "1", 1); // value 파일에 "1" 쓰기 (부저 ON)
        if (ret < 0) {
            perror("write");
            exit(1);
        }

        close(fd); // value 파일 닫기

        usleep(DURATION); // 1초 대기 (부저 ON)

        // 7-2. value 파일 열기 (부저 OFF)
        snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%s/value", BUZZER_PIN); // 경로 생성
        fd = open(buf, O_WRONLY); // value 파일 열기
        if (fd < 0) {
            perror("open");
            exit(1);
        }

        ret = write(fd, "0", 1); // value 파일에 "0" 쓰기 (부저 OFF)
        if (ret < 0) {
            perror("write");
            exit(1);
        }

        close(fd); // value 파일 닫기

        usleep(DURATION); // 1초 대기 (부저 OFF)
    }

    return 0; // 프로그램 정상 종료
}

//////////////////////////////////////////////////////////////////////////////
// wiringPi 라이브러리 예제 코드 (참고용)

/*************************************************************************
#include <wiringPi.h> // wiringPi 라이브러리 포함

#define BUZZER_PIN 23 // 부저가 연결된 GPIO 핀 번호

int main(void)
{
    wiringPiSetupGpio(); // wiringPi 라이브러리 초기화

    pinMode(BUZZER_PIN, OUTPUT); // 부저 핀을 출력 모드로 설정

    // 무한 반복하며 부저 ON/OFF
    while (1) {
        digitalWrite(BUZZER_PIN, HIGH); // 부저 ON
        delay(1000); // 1초 대기
        digitalWrite(BUZZER_PIN, LOW); // 부저 OFF
        delay(1000); // 1초 대기
    }

    return 0;
}
************************************************************************/