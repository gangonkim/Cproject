#define _CRT_SECURE_NO_WARNINGS
#include "Env.h"
#include <stdio.h>

// ANSI 색상 코드
#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[32m"  // 수익 (EARNINGS_RATE > 0)
#define RED_COLOR "\033[31m"    // 손실 (EARNINGS_RATE < 0)

void showholdings() {
    set_env(); // 환경 설정

    char* select_sql = "SELECT ACCOUNTNUMBER, STOCKTICKER, QUANTITY, PURCHASE_PRICE, CURRENT_PRICE, VALUATIONPL, EARNINGS_RATE, EVALUATED_PRICE FROM C##DEV.HOLDINGS";
    OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
    OCIStmtPrepare(stmthp, errhp, (text*)select_sql, (ub4)strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);
    OCIStmtExecute(svchp, stmthp, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

    // Define 변수
    OCIDefine* def1 = NULL, * def2 = NULL, * def3 = NULL, * def4 = NULL, * def5 = NULL, * def6 = NULL, * def7 = NULL;
    char account_number[64], stock_ticker[10];
    int quantity;
    double purchase_price, current_price, valuation_pl, earnings_rate, evaluated_price;

    // 컬럼 매핑
    OCIDefineByPos(stmthp, &def1, errhp, 1, account_number, sizeof(account_number), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &def2, errhp, 2, stock_ticker, sizeof(stock_ticker), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &def3, errhp, 3, &quantity, sizeof(quantity), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &def4, errhp, 4, &purchase_price, sizeof(purchase_price), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &def5, errhp, 5, &current_price, sizeof(current_price), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &def6, errhp, 6, &valuation_pl, sizeof(valuation_pl), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &def7, errhp, 7, &earnings_rate, sizeof(earnings_rate), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);

    // 출력


    while (OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
        system("cls");  // 화면 초기화
        printf("-------------------------------------------------------------------------------\n");
        printf("계좌번호: %-15s\n", account_number);
        printf("-------------------------------------------------------------------------------\n");
        printf("| %-10s   | %-8s   | %-12s    | %-12s    | %-10s   | %-8s   |\n",
            "종목코드", "수량", "매입가", "현재가", "손익", "수익률");
        printf("-------------------------------------------------------------------------------\n");
        const char* color = earnings_rate >= 0 ? GREEN_COLOR : RED_COLOR;
        printf("| %-10s | %-8d | %-12.2f | %-12.2f | %s%-10.2f%s | %s%-7.2f%%%s |\n",
            stock_ticker, quantity, purchase_price, current_price,
            color, valuation_pl, RESET_COLOR, color, earnings_rate, RESET_COLOR);
    }

    printf("-------------------------------------------------------------------------------\n");

    // 환경 종료
    quit_env();

    // ESC 키 입력 대기
    printf("메뉴 선택 화면으로 돌아가기(ESC)...\n");
    while (1) {
        if (_kbhit()) {  // 키 입력 감지
            char key = _getch();  // 입력된 키 값을 읽음
            if (key == 27) {  // ESC 키(ASCII 27) 확인
                handle_menu_selection();  // 메뉴 선택 함수 실행
                break;  // 루프 종료
            }
        }
    }

}