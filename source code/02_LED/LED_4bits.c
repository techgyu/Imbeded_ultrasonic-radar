
/*
 * 파일명: LED_4bits.cpp
 * ---------------------
 * 4비트 정수(1~15)를 입력받아 각 비트에 해당하는 GPIO LED를 켜는 예제
 * - sysfs를 통한 GPIO 제어
 * - 각 함수와 주요 코드에 상세 설명 주석 추가
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 미지원)
 */

#include<stdio.h>      // 표준 입출력 함수
#include<stdlib.h>     // 표준 라이브러리 함수
#include<fcntl.h>      // 파일 제어 함수
#include<string.h>     // 문자열 처리 함수
#include<unistd.h>     // 유닉스 시스템 호출 (open, write, close 등)


// 입력된 정수(num)를 4비트 배열(bits)로 변환하는 함수
// num: 1~15, bits[0]~bits[3]에 각 비트값 저장
void int_to_bits(int num, int* bits) {
    if (num > 15) {
        printf("You input bigger than 15.\n"); // 15 초과 입력 시 경고
        return;
    }
    if (num >= 8) {
        bits[0] = 1; // 8의 자리 비트
        num = num - 8;
    }
    if (num >= 4) {
        bits[1] = 1; // 4의 자리 비트
        num = num - 4;
    }
    if (num >= 2) {
        bits[2] = 1; // 2의 자리 비트
        num = num - 2;
    }
    if (num >= 1) {
        bits[3] = 1; // 1의 자리 비트
        num = num - 1;
    }
}


// 지정된 GPIO 핀을 export하고, 방향을 out으로 설정 후 LED를 켜는 함수
void turn_on(int gpio) {
    char buf[50];
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY); // GPIO export
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);

    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
    fd = open(buf, O_WRONLY | O_TRUNC); // 방향 설정
    write(fd, "out", 3);
    close(fd);

    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY | O_TRUNC); // 값 설정
    write(fd, "1", 1); // LED ON
    close(fd);
}


// 메인 함수: 사용자 입력을 받아 4비트 LED를 제어
int main() {
    int int_num;
    int bit_num[4] = {0, 0, 0, 0}; // 비트 배열 초기화

    printf("input 1~16: "); // 사용자 입력 안내
    scanf("%d", &int_num); // 정수 입력

    int_to_bits(int_num, bit_num); // 입력값을 4비트로 변환
    printf("[0]%d [1]%d [2]%d [3]%d\n", bit_num[0], bit_num[1], bit_num[2], bit_num[3]); // 변환 결과 출력

    // 각 비트에 해당하는 GPIO LED를 켬
    if (bit_num[0] == 1) {
        turn_on(21); // 8의 자리 LED
    }
    else if (bit_num[1] == 1) {
        turn_on(20); // 4의 자리 LED
    }
    else if (bit_num[2] == 1) {
        turn_on(17); // 2의 자리 LED
    }
    else if (bit_num[3] == 1) {
        turn_on(19); // 1의 자리 LED
    }

    return 0;
}
