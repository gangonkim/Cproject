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
	if (OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
		printf("존재: \n");
	}
	else {
		printf("안존재: \n");
	}
	quit_env();
};
void insert_holding(HOLDING* holding) {
};
void update_holding(HOLDING* holding1, HOLDING* holding2) {
}
;


