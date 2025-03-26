#define _CRT_SECURE_NO_WARNINGS
#include "Env.h"


int main() {
	set_env(); //초기 1 회

	//////////////////////////////////////////////////////////////////////
	char* select_sql = "SELECT * FROM users";
	OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
	OCIStmtPrepare(stmthp, errhp, (text*)select_sql, strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);


	OCIStmtExecute(svchp, stmthp, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);


	OCIDefine* def1 = NULL;
	char id[20];

	OCIDefineByPos(stmthp, &def1, errhp, 1, id, sizeof(id), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);

	printf("테이블 조회 결과:\n");


	while ((status = OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT)) == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {

		printf("|%-10s|\n", id);
	}
	printf("-------------------------------------------------------\n");
	//////////////////////////////////////////////////////////////////////
	
	quit_env; //사용후 종료

	return 0;
}