#define _CRT_SECURE_NO_WARNINGS
#include "Env.h"

// ANSI 색상 코드
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"  // 빨간색
#define BLUE_COLOR "\033[34m" // 파란색

void showkrxstockprice(const char* stock_code_input) {
    set_env(); // 환경 설정

    printf("Stock Code Input: %s\n", stock_code_input);

    char select_sql[1500];
    sprintf(select_sql,
        "SELECT STOCK_CODE, TRADING_TIME, "
        "SELL_PRICE_1, SELL_QUANTITY_1, SELL_PRICE_2, SELL_QUANTITY_2, SELL_PRICE_3, SELL_QUANTITY_3, "
        "SELL_PRICE_4, SELL_QUANTITY_4, SELL_PRICE_5, SELL_QUANTITY_5, SELL_PRICE_6, SELL_QUANTITY_6, "
        "SELL_PRICE_7, SELL_QUANTITY_7, SELL_PRICE_8, SELL_QUANTITY_8, SELL_PRICE_9, SELL_QUANTITY_9, "
        "SELL_PRICE_10, SELL_QUANTITY_10, "
        "BUY_PRICE_1, BUY_QUANTITY_1, BUY_PRICE_2, BUY_QUANTITY_2, BUY_PRICE_3, BUY_QUANTITY_3, "
        "BUY_PRICE_4, BUY_QUANTITY_4, BUY_PRICE_5, BUY_QUANTITY_5, BUY_PRICE_6, BUY_QUANTITY_6, "
        "BUY_PRICE_7, BUY_QUANTITY_7, BUY_PRICE_8, BUY_QUANTITY_8, BUY_PRICE_9, BUY_QUANTITY_9, "
        "BUY_PRICE_10, BUY_QUANTITY_10, "
        "ATS_SELL_QUANTITY_1, ATS_SELL_QUANTITY_2, ATS_SELL_QUANTITY_3, ATS_SELL_QUANTITY_4, ATS_SELL_QUANTITY_5, "
        "ATS_SELL_QUANTITY_6, ATS_SELL_QUANTITY_7, ATS_SELL_QUANTITY_8, ATS_SELL_QUANTITY_9, ATS_SELL_QUANTITY_10, "
        "ATS_BUY_QUANTITY_1, ATS_BUY_QUANTITY_2, ATS_BUY_QUANTITY_3, ATS_BUY_QUANTITY_4, ATS_BUY_QUANTITY_5, "
        "ATS_BUY_QUANTITY_6, ATS_BUY_QUANTITY_7, ATS_BUY_QUANTITY_8, ATS_BUY_QUANTITY_9, ATS_BUY_QUANTITY_10 "
        "FROM STOCKASKINGPRICE "
        "WHERE STOCK_CODE = '%s'", stock_code_input);

    OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
    OCIStmtPrepare(stmthp, errhp, (text*)select_sql, strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);
    OCIStmtExecute(svchp, stmthp, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

    // Define 변수 선언
    OCIDefine* def1 = NULL;
    OCIDefine* def2 = NULL;
    OCIDefine* def3 = NULL;
    OCIDefine* def4 = NULL;
    OCIDefine* def5 = NULL;
    OCIDefine* def6 = NULL;
    OCIDefine* def7 = NULL;
    OCIDefine* def8 = NULL;
    OCIDefine* def9 = NULL;
    OCIDefine* def10 = NULL;
    OCIDefine* def11 = NULL;
    OCIDefine* def12 = NULL;
    OCIDefine* def13 = NULL;
    OCIDefine* def14 = NULL;
    OCIDefine* def15 = NULL;
    OCIDefine* def16 = NULL;
    OCIDefine* def17 = NULL;
    OCIDefine* def18 = NULL;
    OCIDefine* def19 = NULL;
    OCIDefine* def20 = NULL;
    OCIDefine* def21 = NULL;
    OCIDefine* def22 = NULL;
    OCIDefine* def23 = NULL;
    OCIDefine* def24 = NULL;
    OCIDefine* def25 = NULL;

    char stock_code[20];
    char trading_time[20];
    int sell_price[10], buy_price[10];
    int sell_quantity[10], buy_quantity[10];
    int ats_sell_quantity[10], ats_buy_quantity[10];

    // 각 컬럼을 매핑
    OCIDefineByPos(stmthp, &def1, errhp, 1, stock_code, sizeof(stock_code), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &def2, errhp, 2, trading_time, sizeof(trading_time), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    // Define 변수 배열 선언
    OCIDefine* def_sell_price[10];
    OCIDefine* def_sell_quantity[10];
    OCIDefine* def_buy_price[10];
    OCIDefine* def_buy_quantity[10];
    OCIDefine* def_ats_sell_quantity[10];
    OCIDefine* def_ats_buy_quantity[10];

    // 매도호가 1~10 및 수량 매핑
    for (int i = 0; i < 10; i++) {
        // 매도호가 1~10 및 수량 매핑
        OCIDefineByPos(stmthp, &def_sell_price[i], errhp, 3 + i * 2, &sell_price[i], sizeof(sell_price[i]), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
        OCIDefineByPos(stmthp, &def_sell_quantity[i], errhp, 4 + i * 2, &sell_quantity[i], sizeof(sell_quantity[i]), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);

        // 매수호가 1~10 및 수량 매핑
        OCIDefineByPos(stmthp, &def_buy_price[i], errhp, 23 + i * 2, &buy_price[i], sizeof(buy_price[i]), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
        OCIDefineByPos(stmthp, &def_buy_quantity[i], errhp, 24 + i * 2, &buy_quantity[i], sizeof(buy_quantity[i]), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);

        // ATS 매도 수량 매핑
        OCIDefineByPos(stmthp, &def_ats_sell_quantity[i], errhp, 43 + i, &ats_sell_quantity[i], sizeof(ats_sell_quantity[i]), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);

        // ATS 매수 수량 매핑
        OCIDefineByPos(stmthp, &def_ats_buy_quantity[i], errhp, 53 + i, &ats_buy_quantity[i], sizeof(ats_buy_quantity[i]), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
    }

    // 출력
    system("cls");  // 화면 초기화
    printf("----------------------\n");

    while ((status = OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT)) == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
        printf("| 종목코드 | %-8s \n|  종목명  | 삼성전자 \n", stock_code);
        printf("----------------------\n");
        printf("|  호 가   | KRX잔량 |\n");

        // 매도호가 1~10 및 수량 출력 (파란색)
        for (int i = 0; i < 10; i++) {
            printf("| %s %.0f%s   |  %-5d  |\n", BLUE_COLOR, (float)sell_price[i], RESET_COLOR, sell_quantity[i], ats_sell_quantity[i]);
        }

        // 매수호가 1~10 및 수량 출력 (빨간색)
        for (int i = 0; i < 10; i++) {
            printf("| %s %.0f%s   |  %-5d  |\n", RED_COLOR, (float)buy_price[i], RESET_COLOR, buy_quantity[i], ats_buy_quantity[i]);
        }

        printf("----------------------\n");
    }

    // 환경 종료
    quit_env();
}
