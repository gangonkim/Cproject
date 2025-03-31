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

    TRADE** trade_array = sor(offer.ticker, offer.status, offer.price, offer.quantiy, &trade_count);
    if (!trade_array) {
        printf("Failed to retrieve trade array\n");
        return;
    }

    for (int i = 0; i < trade_count; i++) {
        if (!trade_array[i]) {
            printf("Trade %d is NULL. Skipping...\n", i + 1);
            continue; // NULL 방어 처리
        }
        printf("Trade %d: Quantity=%d, Price=%.2f, Charge=%.4f, Comparison=%.4f\n",
            i + 1,
            trade_array[i]->quantity,  // -> 사용
            trade_array[i]->price,
            trade_array[i]->charge,
            trade_array[i]->comparison);
    }

    // 개별 할당된 TRADE 구조체 해제
    for (int i = 0; i < trade_count; i++) {
        free(trade_array[i]); // 개별 TRADE 구조체 메모리 해제
    }

    // trade_array 자체 해제
    free(trade_array);

    
    //insert_trade(trade);

    //// 보유 주식 변경
    //HOLDING* h1 = (HOLDING*)malloc(sizeof(HOLDING));
    //if (h1 == NULL) {
    //    printf("메모리 할당 실패\n");
    //    return;
    //}

    //strcpy(h1->accountNum, account);
    //strcpy(h1->ticker, "005930");
    //h1->quantiy = 0;
    //get_holding(h1);
    //printf("현재 보유 수량: %d\n", h1->quantiy);

    //HOLDING* h2 = (HOLDING*)malloc(sizeof(HOLDING));
    //if (h2 == NULL) {
    //    printf("메모리 할당 실패\n");
    //    free(h1);
    //    return;
    //}

    //strcpy(h2->accountNum, account);
    //strcpy(h2->ticker, "005930");
    //h2->quantiy = trade->quantity;
    //h2->purchasePrice = trade->price;
    //h2->currentPrice = trade->price;
    //h2->valuation_pl = 0;
    //h2->earnings_rate = 0;

    //if (h1->quantiy == 0) {
    //    insert_holding(h2);
    //}
    //else {
    //    update_holding(h1, h2, type);
    //}
    // ESC 키 입력 대기
    printf("주문이 완료되었습니다.\n");
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