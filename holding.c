#include "holding.h"
#include "Env.h"

void get_holding(HOLDING* holding) {
	set_env();
	char* select_sql = "SELECT ACCOUNTNUMBER, STOCKTICKER, QUANTITY, PURCHASE_PRICE, CURRENT_PRICE, VALUATIONPL, EARNINGS_RATE FROM HOLDINGS where accountnumber = :1 and stockticker = :2";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)select_sql, (ub4)strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);

	//바인드 변수
	OCIBind* bnd1 = NULL, * bnd2 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, holding->accountNum, sizeof(holding->accountNum), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd2, errhp, 2, holding->ticker, sizeof(holding->ticker), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIStmtExecute(svchp, stmthp, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);
	
	


	// Define 변수
	OCIDefine* def1 = NULL, * def2 = NULL, * def3 = NULL, * def4 = NULL, * def5 = NULL, * def6 = NULL, * def7 = NULL;

	// 컬럼 매핑
	OCIDefineByPos(stmthp, &def1, errhp, 1, holding->accountNum, sizeof(holding->accountNum), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def2, errhp, 2, holding->ticker, sizeof(holding->ticker), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def3, errhp, 3, &holding->quantiy, sizeof(holding->quantiy), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def4, errhp, 4, &holding->purchasePrice, sizeof(holding->purchasePrice), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def5, errhp, 5, &holding->currentPrice, sizeof(holding->currentPrice), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def6, errhp, 6, &holding->valuation_pl, sizeof(holding->valuation_pl), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
	OCIDefineByPos(stmthp, &def7, errhp, 7, &holding->earnings_rate, sizeof(holding->earnings_rate), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
	if (OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT) != OCI_SUCCESS) {
		//check_error(errhp);
	}
	
	quit_env();
};

void insert_holding(HOLDING* holding) {
	set_env();
	char* insert_sql = "INSERT INTO holdings (ACCOUNTNUMBER, STOCKTICKER, QUANTITY, PURCHASE_PRICE, CURRENT_PRICE, VALUATIONPL, EARNINGS_RATE) VALUES (:1, :2, :3, :4, :5, :6, :7)";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)insert_sql, strlen(insert_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);


	OCIBind* bnd1 = NULL, * bnd2 = NULL, * bnd3 = NULL, * bnd4 = NULL, * bnd5 = NULL, * bnd6 = NULL, * bnd7 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, holding->accountNum, sizeof(holding->accountNum), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd2, errhp, 2, holding->ticker, sizeof(holding->ticker), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd3, errhp, 3, &holding->quantiy, sizeof(holding->quantiy), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd4, errhp, 4, &holding->purchasePrice, sizeof(holding->purchasePrice), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd5, errhp, 5, &holding->currentPrice, sizeof(holding->currentPrice), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd6, errhp, 6, &holding->valuation_pl, sizeof(holding->valuation_pl), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd7, errhp, 7, &holding->earnings_rate, sizeof(holding->earnings_rate), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS) != OCI_SUCCESS) {
		check_error(errhp);
	}
	else {
		//printf(" 데이터 삽입 완료!\n");
	}
	quit_env();
};

void update_holding(HOLDING* holding1, HOLDING* holding2, int type) {
	HOLDING result;
	strcpy(result.accountNum, holding2->accountNum);
	strcpy(result.ticker, holding2->ticker);
	if(!type){
		result.quantiy = holding1->quantiy - holding2->quantiy;
		result.currentPrice = holding2->purchasePrice;
		if (result.quantiy <= 0) {
			delete_holding(holding1);
			return;
		}
		result.purchasePrice =
			(holding1->purchasePrice * holding1->quantiy - holding2->purchasePrice * holding2->quantiy) / result.quantiy;

	} 
	else {
		result.quantiy = holding1->quantiy+ holding2->quantiy;
		result.currentPrice = holding2->purchasePrice;
		result.purchasePrice =
			(holding1->purchasePrice * holding1->quantiy + holding2->purchasePrice * holding2->quantiy) / result.quantiy;
	}
	result.valuation_pl =  result.currentPrice * result.quantiy - result.purchasePrice * result.quantiy;
	result.earnings_rate = ((result.currentPrice - result.purchasePrice) / result.purchasePrice) * 100;
	/////////////////////////////////////////////////
	set_env();
	char* update_sql = 
		"UPDATE holdings SET QUANTITY = :1, PURCHASE_PRICE = :2, CURRENT_PRICE = :3, VALUATIONPL = :4, EARNINGS_RATE = :5  WHERE accountnumber = :6 AND stockticker = :7";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)update_sql, strlen(update_sql),
		OCI_NTV_SYNTAX, OCI_DEFAULT);



	OCIBind* bnd1 = NULL, * bnd2 = NULL, * bnd3 = NULL, * bnd4 = NULL, * bnd5 = NULL, * bnd6 = NULL, * bnd7 = NULL;

	OCIBindByPos(stmthp, &bnd1, errhp, 1, &result.quantiy, sizeof(result.quantiy), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd2, errhp, 2, &result.purchasePrice, sizeof(result.purchasePrice), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd3, errhp, 3, &result.currentPrice, sizeof(result.currentPrice), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd4, errhp, 4, &result.valuation_pl, sizeof(result.valuation_pl), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd5, errhp, 5, &result.earnings_rate, sizeof(result.earnings_rate), SQLT_FLT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd6, errhp, 6, result.accountNum, sizeof(result.accountNum), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd7, errhp, 7, result.ticker, sizeof(result.ticker), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS) != OCI_SUCCESS) {
		check_error(errhp);
	}
	else {
		//printf(" 데이터 수정 완료!\n");
	}
	quit_env();

};

void delete_holding(HOLDING* holding1) {
	set_env();
	char* delete_sql = "DELETE FROM holdings WHERE accountnumber = :1 AND stockticker = :2";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)delete_sql, strlen(delete_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);

	OCIBind* bnd1 = NULL, *bnd2 = NULL;

	OCIBindByPos(stmthp, &bnd1, errhp, 1, holding1->accountNum, sizeof(holding1->accountNum), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd2, errhp, 2, holding1->ticker, sizeof(holding1->ticker), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL,
		OCI_COMMIT_ON_SUCCESS) != OCI_SUCCESS) {
		check_error(errhp);
	}
	else {
		//printf(" 데이터 삭제 완료!\n");
	}
	quit_env();
}
