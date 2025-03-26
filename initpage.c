#include <stdio.h>
#include <windows.h>
#include <oci.h>
#include <stdlib.h>

void printProgressBar(int percent) {
    int barWidth = 50; // 진행 바의 총 길이
    printf("[");

    int pos = barWidth * percent / 100; // 진행 비율에 따라 비어있는 부분을 계산
    for (int i = 0; i < barWidth; i++) {
        if (i < pos)
            printf("="); // 진행된 부분
        else
            printf(" "); // 비어있는 부분
    }

    printf("] %d%%\r", percent); // 진행 비율과 함께 출력하고 덮어쓰기를 위해 \r 사용
    fflush(stdout); // 즉시 출력 반영
}