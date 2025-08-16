
/*
 * 파일명: LED_pwm_control_bright.c
 * -----------------
 * GPIO를 이용해 LED에 PWM(펄스폭변조) 신호를 주어 밝기를 제어하는 예제 코드
 * - gpiolib.c의 GPIO 제어 함수 활용
 * - 각 함수와 주요 코드에 상세 설명 주석 추가
 *
 * ※ 리눅스 기반에서만 동작 (Windows에서는 일부 헤더 미지원)
 */

#include <stdio.h>      // 표준 입출력 함수
#include <unistd.h>     // 유닉스 시스템 호출 (usleep 등)
#include "gpiolib.c"   // GPIO 제어 함수 포함


// 지정된 GPIO 핀에 PWM 신호를 주어 LED 밝기를 제어하는 함수
// gpio: 제어할 GPIO 번호
// dutyRate: 듀티비(%) - ON 시간 비율
// dutyT: PWM 주기(ms)
// cnt: 반복 횟수
int pwmControl(int gpio, int dutyRate, int dutyT, int cnt)
{
    int dutyOn = (dutyT * dutyRate) / 100;       // ON 시간(ms)
    int dutyOff = dutyT - dutyOn;                // OFF 시간(ms)

    gpioExport(gpio);         // GPIO export
    gpioDirection(gpio, 1);  // 방향 설정 (출력)

    while (cnt--)
    {
        gpioWrite(gpio, 1);              // LED ON
        usleep(dutyOn * 1000);           // ON 시간 대기
        gpioWrite(gpio, 0);              // LED OFF
        usleep(dutyOff * 1000);          // OFF 시간 대기
    }

    gpioUnexport(gpio);       // GPIO unexport

    return 0;
}


// 메인 함수: PWM 제어 함수 호출
int main()
{
    int dutyRate = 50; // 듀티비(%) - LED 밝기 제어 (0~100)
    int dutyT = 33;    // PWM 주기(ms)
    int cnt = 100;     // 반복 횟수

    printf("PWM 테스트 !!!\n"); // 안내 메시지 출력

    printf("dutyRate: %d dutyT: %d cnt: %d\n", dutyRate, dutyT, cnt); // 파라미터 출력
    pwmControl(26, dutyRate, dutyT, cnt); // GPIO 26번 핀에 PWM 신호
    sleep(1); // 1초 대기

    return 0;
}
