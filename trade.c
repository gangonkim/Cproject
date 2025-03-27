#include "trade.h"
#include "Env.h"



void trade(TRADE* trade) {
	set_env();
	char* insert_sql = "INSERT INTO trades (OFFERNUMBER, EXCHANGEACTUAL, QUANTITY, PRICE, CHARGE) VALUES (:1, :2, :4, :5, :6)";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)insert_sql, strlen(insert_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);


	OCIBind* bnd1 = NULL, * bnd2 = NULL, * bnd3 = NULL, * bnd4 = NULL, * bnd5 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, &trade->offernumber, sizeof(trade->offernumber), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd2, errhp, 2, &trade->exchangeactual, sizeof(trade->exchangeactual), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd4, errhp, 3, &trade->quantiy, sizeof(trade->quantiy), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd5, errhp, 4, &trade->price, sizeof(trade->price), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	OCIBindByPos(stmthp, &bnd5, errhp, 5, &trade->charge, sizeof(trade->charge), SQLT_INT, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	if (OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS) != OCI_SUCCESS) {
		check_error(errhp);
	}
	else {
		printf(" 데이터 삽입 완료!\n");
	}
	quit_env;
};