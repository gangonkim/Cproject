#include "offer.h"  
#include "trade.h"
#include "holding.h"
#include "env.h"
#include "OfferService.h"  

void add_offer(int c) {
    OFFER offer;
    int type;

    // 계좌 번호 가져오기
    char* account = get_account(authId);
    if (account) {
        strcpy(offer.accountNum, account);
    }

    strcpy(offer.ticker, "005930"); // 이건 넘겨 받아야 할 듯

    printf("거래 유형(매수: 1, 매도: 0): \n");
    scanf("%d", &type);
    strcpy(offer.status, type ? "매수" : "매도");

    printf("거래 수량: \n");
    scanf("%d", &offer.quantiy);
    printf("거래 가격: \n");
    scanf("%d", &offer.price);

    // offer 값 디버깅 출력
    printf("offer.ticker: %s\n", offer.ticker);
    printf("offer.status: %s\n", offer.status);
    printf("offer.price: %d\n", offer.price);
    printf("offer.quantity: %d\n", offer.quantiy);

    int offerId = insert_offer(&offer);
    int trade_count = 0;

    // 거래 배열 할당
    TRADE* trade_array = sor(offer.ticker, offer.status, offer.price, offer.quantiy, &trade_count);
    if (trade_array == NULL) {
        printf("sor()가 NULL을 반환하였습니다.\n");
        return;
    }

    printf("trade_count의 크기: %d\n", trade_count);

    for (int i = 0; i < trade_count; i++) {
        printf("trade_array[%d]:\n", i);
        printf("price: %.2f\n", trade_array[i].price);

        // 동적 할당을 통해 거래 객체 처리
        TRADE* trade = (TRADE*)malloc(sizeof(TRADE));
        if (trade == NULL) {
            printf("메모리 할당 실패\n");
            free(trade_array);
            return;
        }

        *trade = trade_array[i];
        trade->offernumber = offerId;
        trade->exchangeactual = 0;
        trade->charge = 50;

        insert_trade(trade);

        // 보유 주식 변경
        HOLDING* h1 = (HOLDING*)malloc(sizeof(HOLDING));
        if (h1 == NULL) {
            printf("메모리 할당 실패\n");
            free(trade);
            free(trade_array);
            return;
        }

        strcpy(h1->accountNum, account);
        strcpy(h1->ticker, "005930");
        h1->quantiy = 0;
        get_holding(h1);
        printf("현재 보유 수량: %d\n", h1->quantiy);

        HOLDING* h2 = (HOLDING*)malloc(sizeof(HOLDING));
        if (h2 == NULL) {
            printf("메모리 할당 실패\n");
            free(h1);
            free(trade);
            free(trade_array);
            return;
        }

        strcpy(h2->accountNum, account);
        strcpy(h2->ticker, "005930");
        h2->quantiy = trade->quantity;
        h2->purchasePrice = trade->price;
        h2->currentPrice = trade->price;
        h2->valuation_pl = 0;
        h2->earnings_rate = 0;

        if (h1->quantiy == 0) {
            insert_holding(h2);
        }
        else {
            update_holding(h1, h2, type);
        }

        // 메모리 해제
        free(h1);
        free(h2);
        free(trade);
    }

    free(trade_array);
}

void get_offer_history() {
	// 계좌 번호 가져오기
	char* account = get_account(authId);
	int n =  count_offers(account);
	system("cls");
	printf("===============================================================================================\n");
	printf("     계좌번호       | 주식 코드  | 주문내용   | 주문가격 |       주문시간      |  체결량  | 주문량  \n");
	printf("-----------------------------------------------------------------------------------------------\n");
	OFFER* offers = (OFFER*)malloc(sizeof(OFFER) * n); // 배열 동적 할당
	if (offers == NULL) {
		printf("메모리 할당 실패\n");
		quit_env();
		return NULL;
	}

	get_offers(account, offers , n);

	if (offers != NULL) {
		for (int i = 0; i < n; i++) {
			check_trade(&offers[i]);
		}
	}
	free(offers);
	
};