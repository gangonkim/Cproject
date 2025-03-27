
#include "offer.h"  
#include "trade.h"
#include "holding.h"
#include "env.h"
#include "OfferService.h"  



void add_offer() {
	OFFER offer;
	int type;

	// 계좌 번호 가져오기
	char* account = get_account(authId);
	if (account) {
		strcpy(offer.accountNum, account); 
		free(account);  
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
	TRADE trade;
	trade.offernumber = offerId;
	trade.exchangeactual = 0;
	trade.quantiy = 10;
	trade.price = 2000;
	trade.charge = 50;
	insert_trade(&trade);
	
	//보유 주식 변경
	HOLDING h1;
	strcpy(h1.accountNum, account);
	strcpy(h1.ticker, "005930");
	h1.quantiy = 0;
	get_holding(&h1);
	if (h1.quantiy == 0) {
		//데이터 삽입
	}
	else {
		//데이터 수정
	}
	
}
