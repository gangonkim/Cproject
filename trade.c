#include "trade.h"
#include "Env.h"

char* get_account(const char* name) {
	set_env();

	// 동적 할당으로 변경 (반환 가능)
	char* acc_num = (char*)malloc(20);
	if (!acc_num) {
		printf("메모리 할당 실패\n");
		return NULL;
	}

	char* select_sql = "SELECT ACCOUNTNUMBER FROM accounts WHERE USERID = :1";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)select_sql, strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);

	OCIBind* bnd1 = NULL;
	OCIBindByPos(stmthp, &bnd1, errhp, 1, (void*)name, strlen(name) + 1, SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

	OCIStmtExecute(svchp, stmthp, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

	OCIDefine* def1 = NULL;
	OCIDefineByPos(stmthp, &def1, errhp, 1, acc_num, 20, SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);

	// 데이터가 존재하면 저장
	if (OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
		printf("계좌번호: %s\n", acc_num);
	}
	else {
		strcpy(acc_num, "UNKNOWN");
	}

	quit_env();  // 함수 호출 형태로 변경

	return acc_num;  // 동적 할당된 메모리 반환
};

void insert_trade(TRADE* trade) {
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

