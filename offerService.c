#include "offer.h"  
#include "OfferService.h"  


void add_offer() {
	OFFER offer;
	int type;
	printf("계좌 번호: \n");
	scanf("%s", offer.accountNum);
	printf("종목명: \n");
	scanf("%s", offer.ticker);
	printf("거래 유형(0:매수, 1:매도): \n");
	scanf("%d", &type);
	if (type) strcpy(offer.status, "매수");
	else strcpy(offer.status, "매도");
	printf("거래 수량: \n");
	scanf("%d", &offer.quantiy);
	printf("거래 가격: \n");
	scanf("%d", &offer.price);


	// 이거 정리해서 넘겨야함
	offer.exchangeCode = 0;

	insert_offer(&offer);
	//가격 불러오기
	//가격 비교 (sor) 
	// 반대케이스 저장
	//체결 테이블 작성
	//보유 주식 변경

}
