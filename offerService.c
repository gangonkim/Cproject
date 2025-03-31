
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
		//free(account);  
	}


	strcpy(offer.ticker, "005930");//이건 넘겨 받아야 할듯

	printf("거래 유형(0:매수, 1:매도): \n");
	scanf("%d", &type);
	if (type) strcpy(offer.status, "매수");
	else strcpy(offer.status, "매도");

	printf("거래 수량: \n");
	scanf("%d", &offer.quantiy);
	printf("거래 가격: \n");
	scanf("%d", &offer.price);




	int offerId = insert_offer(&offer);
	//가격 불러오기
	//가격 비교 (sor) 
	
	// 반대케이스 저장
	//체결 테이블 작성
	/////////////////////////////////넘겨 받는 데이터
	TRADE trade;
	trade.offernumber = offerId;
	trade.exchangeactual = 0;
	trade.quantiy = 10;
	trade.price = 2000;
	trade.charge = 50;
	////////////////////////////////
	insert_trade(&trade);
	
	//보유 주식 변경
	////////////////////////////////////////////////
	// 이건 확인용 생성(기존 데이터)
	HOLDING h1;
	strcpy(h1.accountNum, account);
	strcpy(h1.ticker, "005930");
	h1.quantiy = 0;
	get_holding(&h1);
	printf("%d", h1.quantiy);

	//여기는 전달용(실제 저장 데이터, 신규 데이터)
	HOLDING h2;
	strcpy(h2.accountNum, account);
	strcpy(h2.ticker, "005930");
	h2.quantiy = trade.quantiy;
	h2.purchasePrice = trade.price;
	h2.currentPrice = trade.price;
	h2.valuation_pl = 0;
	h2.earnings_rate = 0;
	
	//////////////////////////////////////////////////
	if (h1.quantiy == 0) {
		//데이터 삽입
		insert_holding(&h2);
	}
	else {
		//데이터 수정
		update_holding(&h1, &h2, type);
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