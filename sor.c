#define _CRT_SECURE_NO_WARNINGS
#include "Env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KRX_CHARGE 1.5
#define ATS_CHARGE 1.45

typedef struct {
    int id;
    int offernumber;
    int exchangeactual;
    char selleraccountnumber[20];
    char buyeraccountnumber[20];
    int quantity;
    int price;
    double charge;
    int comparison;
    char createdAt[20];

} TRADE;

TRADE* sor(int offerId, char stockcode[], char status[], int price, int quantity, int *trade_count, TRADE* trades) {

    set_env(); // 환경 설정

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
        "WHERE STOCK_CODE = '%s'", stockcode);

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


    OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT);

    //while ((OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT)) == OCI_SUCCESS || OCI_SUCCESS_WITH_INFO) {
        int remaining_qty = quantity;

        if (strcmp(status, "매수") == 0) {
           
            for (int i = 0; i < 10; i++) {
                if (remaining_qty > 0 && sell_price[i] <= price) {
                    // ATS 부터 주문 실행
                    int ats_trade_qty = (remaining_qty < ats_sell_quantity[i]) ? remaining_qty : ats_sell_quantity[i];

                    trades[*trade_count] = (TRADE){
                        .offernumber = offerId,
                        .quantity = ats_trade_qty,
                        .price = sell_price[i],
                        .exchangeactual = 1,
                        .charge = ats_trade_qty * ATS_CHARGE,
                        .comparison = ats_trade_qty * (KRX_CHARGE - ATS_CHARGE)
                    };

                    // 트레이드 저장 후 printf를 실행합니다
                    printf("Trade번호# %d, offerId=%d, 체결수량=%d, 체결가=%d, 거래소=%d, 수수료=%.6f, 한국거래소 대비 손익=%d\n",
                        *trade_count,offerId, trades[*trade_count].quantity, trades[*trade_count].price,
                        "NXT", trades[*trade_count].charge, trades[*trade_count].comparison);

                    (*trade_count)++;  // trade_count를 저장 후에 증가시킵니다
                    remaining_qty -= ats_trade_qty;  // 여기서 remaining_qty를 감소시킵니다

                    if (remaining_qty == 0) {
                        //printf("매수주문 완료 (remaining_qty가 0입니다)\n");
                        break;  // remaining_qty가 0이면 매수주문 완료 후 반복문 종료
                    }

                    // 정규거래소 주문 실행
                    int trade_qty = (remaining_qty < sell_quantity[i]) ? remaining_qty : sell_quantity[i];

                    //printf("sell_quantity: %d\n", sell_quantity[i]);
                    //printf("remaining_qty: %d\n", remaining_qty);
                    //printf("trade_qty: %d\n", trade_qty);

                    trades[*trade_count] = (TRADE){
                        .offernumber = offerId,
                        .quantity = trade_qty,
                        .price = sell_price[i],
                        .exchangeactual = 0,
                        .charge = trade_qty * ATS_CHARGE,
                        .comparison = 0
                    };

                    // 트레이드 저장 후 printf를 실행합니다
                    printf("Trade번호# %d, offerId=%d, 체결수량=%d, 체결가=%d, 거래소=%d, 수수료=%.6f, 한국거래소 대비 손익=%d\n",
                        *trade_count, offerId, trades[*trade_count].quantity, trades[*trade_count].price,
                        "NXT", trades[*trade_count].charge, trades[*trade_count].comparison);

                    (*trade_count)++;  // trade_count를 저장 후에 증가시킵니다
                    remaining_qty -= trade_qty;  // 여기서도 remaining_qty를 감소시킵니다

                    if (remaining_qty == 0) {
                        //printf("매수주문 완료 (remaining_qty가 0입니다)\n");
                        break;  // remaining_qty가 0이면 매수주문 완료 후 반복문 종료
                    }

                }
                printf("내부에서 숫자 조회 %d", trades[0].quantity);
                if (remaining_qty == 0) {
                    //printf("매수주문완료 (remaining_qty가 0입니다)\n");
                    break;  // remaining_qty가 0이면 매수주문 완료 후 for문 종료
                }
            }
        } if (strcmp(status, "매도") == 0) {

            for (int i = 0; i < 10; i++) {

                if (remaining_qty > 0 && buy_price[i] >= price) {
                    // ATS 부터 주문 실행
                    int ats_trade_qty = (remaining_qty < ats_buy_quantity[i]) ? remaining_qty : ats_buy_quantity[i];

                    //printf("buy_price[i]: %d", buy_price[i]);
                    //printf("ats_buy_quantity: %d\n", ats_buy_quantity[i]);
                    //printf("remaining_qty: %d\n", remaining_qty);
                    //printf("trade_qty: %d\n", ats_trade_qty);
                    //printf("i: %d\n", i);

                    trades[*trade_count] = (TRADE){
                        .offernumber = offerId,
                        .quantity = ats_trade_qty,
                        .price = buy_price[i],
                        .exchangeactual = 1,
                        .charge = ats_trade_qty * ATS_CHARGE,
                        .comparison = ats_trade_qty * (KRX_CHARGE - ATS_CHARGE)
                    };

                    // 트레이드 저장 후 printf를 실행합니다
                    //printf("Trade #%d: Quantity=%d, Price=%d, ExchangeActual=%d, Charge=%.6f\n",
                    //    *trade_count, trades[*trade_count].quantity, trades[*trade_count].price,
                    //    trades[*trade_count].exchangeactual, trades[*trade_count].charge);

                    (*trade_count)++;  // trade_count를 저장 후에 증가시킵니다
                    remaining_qty -= ats_trade_qty;  // 여기서 remaining_qty를 감소시킵니다

                    if (remaining_qty == 0) {
                        //printf("매도주문 완료 (remaining_qty가 0입니다)\n");
                        break;  // remaining_qty가 0이면 매수주문 완료 후 반복문 종료
                    }

                    // 정규거래소 주문 실행
                    int trade_qty = (remaining_qty < buy_quantity[i]) ? remaining_qty : buy_quantity[i];

                    //printf("buy_quantity: %d\n", buy_quantity[i]);
                    //printf("remaining_qty: %d\n", remaining_qty);
                    //printf("trade_qty: %d\n", trade_qty);

                    trades[*trade_count] = (TRADE){
                        .offernumber = offerId,
                        .quantity = trade_qty,
                        .price = buy_price[i],
                        .exchangeactual = 0,
                        .charge = trade_qty * ATS_CHARGE,
                        .comparison = 0
                    };

                    // 트레이드 저장 후 printf를 실행합니다
                    //printf("Trade #%d: Quantity=%d, Price=%d, ExchangeActual=%d, Charge=%.6f\n",
                    //    *trade_count, trades[*trade_count].quantity, trades[*trade_count].price,
                    //    trades[*trade_count].exchangeactual, trades[*trade_count].charge);

                    (*trade_count)++;  // trade_count를 저장 후에 증가시킵니다
                    remaining_qty -= trade_qty;  // 여기서도 remaining_qty를 감소시킵니다

                    if (remaining_qty == 0) {/*
                        printf("매도주문 완료 (remaining_qty가 0입니다)\n");*/
                        break;  // remaining_qty가 0이면 매수주문 완료 후 반복문 종료
                    }

                }

                if (remaining_qty == 0) {
                    //printf("매도주문완료 (remaining_qty가 0입니다)\n");
                    break;  // remaining_qty가 0이면 매수주문 완료 후 for문 종료
                }
            }
        }

    // 환경 종료
    quit_env();
    return trades;
}
