#include "Offer.h"
#include "Env.h"



void insert_offer(OFFER* offer) {
	set_env();
	char* insert_sql = "INSERT INTO offers (\"accountNumber\", \"stockTicker\", \"exchangeCode\", \"status\", \"quantity\", \"price\") VALUES (:1, :2, :3, :4, :5, :6)";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)insert_sql, strlen(insert_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);




	OCIBind* bnd1 = NULL, * bnd2 = NULL, * bnd3 = NULL, * bnd4 = NULL, * bnd5 = NULL, * bnd6 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, offer->accountNum, sizeof(offer->accountNum), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd2, errhp, 2, offer->ticker, sizeof(offer->ticker), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd3, errhp, 3, &offer->exchangeCode, sizeof(offer->exchangeCode), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd4, errhp, 4, offer->status, sizeof(offer->status), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd5, errhp, 5, &offer->quantiy, sizeof(offer->quantiy), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd6, errhp, 6, &offer->price, sizeof(offer->price), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS) != OCI_SUCCESS) {
		check_error(errhp);
	}
	else {
		printf(" 데이터 삽입 완료!\n");
	}
	quit_env;
};