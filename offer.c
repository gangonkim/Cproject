
#include "offer.h"
#include "Env.h"




int insert_offer(OFFER* offer) {

	set_env();
	int offerId;
	char* insert_sql = "INSERT INTO offers (accountNumber, stockTicker, status, quantity, price) VALUES (:1, :2, :3, :4, :5) RETURNING offerNumber INTO :offerId";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)insert_sql, strlen(insert_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);




	OCIBind* bnd1 = NULL, * bnd2 = NULL, * bnd3 = NULL, * bnd4 = NULL, * bnd5 = NULL, * bnd6 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, offer->accountNum, sizeof(offer->accountNum), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd2, errhp, 2, offer->ticker, sizeof(offer->ticker), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd3, errhp, 3, offer->status, sizeof(offer->status), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd4, errhp, 4, &offer->quantiy, sizeof(offer->quantiy), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd5, errhp, 5, &offer->price, sizeof(offer->price), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd6, errhp, 6, &offerId, sizeof(offerId), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS) != OCI_SUCCESS) {
		check_error(errhp);
	}
	else {
		printf("\n ***주문이 완료되었습니다***\n");
	}
	
	quit_env;
	return offerId;
};

int count_offers(char* accountNum) {
	set_env();
	int count = 0;
	char* count_sql = "SELECT COUNT(*) FROM offers WHERE accountNumber = :1";

	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)count_sql, strlen(count_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);

	OCIBind* bnd1 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, accountNum, strlen(accountNum)+1, SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

	OCIDefine* def1 = NULL;
	OCIDefineByPos(stmthp, &def1, errhp, 1, &count, sizeof(count), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);

	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_DEFAULT) != OCI_SUCCESS) {
		check_error(errhp);
	}
	OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT);

	quit_env();
	return count;
}

void get_offers(char* accountNum, OFFER* offers, int count) {
	set_env();

	if (count == 0) {
		quit_env();
		return NULL;
	}


	char* select_sql = "SELECT offerNumber, accountNumber, stockTicker, status, quantity, price, TO_CHAR(createdat, 'YYYY-MM-DD HH24:MI:SS') "
		"FROM offers WHERE accountNumber = :1";

	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)select_sql, strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);

	OCIBind* bnd1 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, accountNum, strlen(accountNum)+1, SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	// 중간 버퍼 선언
	int tmp_id;
	char tmp_accountNum[20];
	char tmp_ticker[20];
	char tmp_status[20];
	int tmp_quantity;
	int tmp_price;
	char tmp_createdAt[50];

	OCIDefine* def1 = NULL, * def2 = NULL, * def3 = NULL, * def4 = NULL, * def5 = NULL, * def6 = NULL, * def7 = NULL;


	OCIDefineByPos(stmthp, &def1, errhp, 1, &tmp_id, sizeof(tmp_id), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def2, errhp, 2, tmp_accountNum, sizeof(tmp_accountNum), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def3, errhp, 3, tmp_ticker, sizeof(tmp_ticker), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def4, errhp, 4, tmp_status, sizeof(tmp_status), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def5, errhp, 5, &tmp_quantity, sizeof(tmp_quantity), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def6, errhp, 6, &tmp_price, sizeof(tmp_price), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def7, errhp, 7, tmp_createdAt, sizeof(tmp_createdAt), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);

	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_DEFAULT) != OCI_SUCCESS) {
		check_error(errhp);
	}

	int index = 0;

	while (1) {

		offers[index].id = tmp_id;
		strcpy(offers[index].accountNum, tmp_accountNum);
		strcpy(offers[index].ticker, tmp_ticker);
		strcpy(offers[index].status, tmp_status);
		offers[index].quantiy = tmp_quantity;
		offers[index].price = tmp_price;
		strcpy(offers[index].createdAt, tmp_createdAt);

		index++;
		OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT);
		if (index >= count) {
			break;
		}

	}
	quit_env();

}
void check_trade(OFFER* offer) {
	set_env();
	
	char* select_sql = "SELECT quantity FROM trades WHERE offerNumber = :1";

	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)select_sql, strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);

	OCIBind* bnd1 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, &offer->id, sizeof(offer->id), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

	int tmp_quantity;
	int total = 0;
	OCIDefine* def1 = NULL;
	OCIDefineByPos(stmthp, &def1, errhp, 1, &tmp_quantity, sizeof(tmp_quantity), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);

	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_DEFAULT) != OCI_SUCCESS) {
		//check_error(errhp);
	}

	while (1) {
		total += tmp_quantity;
		status = OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT);
		if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO) {
			break; // status가 성공 상태가 아니면 탈출
		}
	}

	if (total <= 0) {
		printf("%-20s| %-10s | %-12s | %-8d |          | %-19s| %-8d | %-8d \n",
			offer->accountNum, offer->ticker,
			offer->status, offer->price, offer->createdAt, 0, offer->quantiy);
	}else {
		printf("%-20s| %-10s | %-12s | %-8d |          | %-19s| %-8d | %-8d \n",
		offer->accountNum, offer->ticker,
		offer->status, offer->price, offer->createdAt, total, offer->quantiy);
	}
	quit_env();
}

void check_detail(OFFER* offer) {
	set_env();

	char* select_sql = "SELECT quantity, price FROM trades WHERE offerNumber = :1";

	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)select_sql, strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);

	OCIBind* bnd1 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, &offer->id, sizeof(offer->id), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

	int quantity;
	int price;

	OCIDefine* def1 = NULL, * def2 = NULL;
	OCIDefineByPos(stmthp, &def1, errhp, 1, &quantity, sizeof(quantity), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def2, errhp, 2, &price, sizeof(price), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);


	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_DEFAULT) != OCI_SUCCESS) {
		//check_error(errhp);
	}

	while (1) {
		if (quantity > 0) {
			printf("                                                         |% -8d  |                    | % -8d \n",
				price, quantity);
		}
		status = OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT);
		if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO) {
			break; // status가 성공 상태가 아니면 탈출
		}
	}

	quit_env();
}