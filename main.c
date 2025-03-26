#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "transactions.h"

void menu() {
    int choice;
    initOCI();
    loadFromFile();
    loadFromStocksFile();

    while (1) {
        printf("\n==== 주식 거래 관리 시스템 ====\n");
        printf("1. 거래 추가\n2. 거래 조회\n3. 고객 거래 검색\n4. 거래 수정\n5. 거래 삭제\n6. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: 
            addTransaction(); 
            saveToFile();
            saveToStocksFile(); 
            break;
        case 2: viewAllTransactions(); break;
        case 3: searchTransactionsByCustomer(); break;
        case 4: 
            updateTransaction(); 
            saveToFile();
            saveToStocksFile(); 
            break;
        case 5: 
            deleteTransaction(); 
            saveToFile();
            saveToStocksFile(); break;
        case 6:
            closeOCI();
            saveToFile();
            saveToStocksFile();
            freeMemory();
            printf("프로그램 종료.\n");
            return;
        default: printf("잘못된 입력입니다.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
