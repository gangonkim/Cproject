#include <stdio.h>
#include <stdlib.h>

//입력값: 주식코드, 매수/매도구분, 가격, 수량
int* sor(char stockcode[], char status[], int price, int quantity) {

    int* arr = (int*)malloc(4 * sizeof(int));

    if (arr == NULL) {
        printf("메모리 할당 실패\n");
        return NULL;
    }

    //sor알고리즘 구현

    // 리턴 배열
    arr[0]; // 정규거래소 주문 실행 가격
    arr[1]; // 정규거래소 주문 실행 수량
    arr[2]; // ats 주문 실행 가격
    arr[3]; // ats 주문 실행 수량

    return arr;
}
