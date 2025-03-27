#include "Env.h"
#define OCI_UTF8ID 871 // Add this line to define OCI_UTF8ID
char* username = "C##COOLFIN";
char* password = "1234";
char* dbname = "localhost:1521/xe";

void set_env() {
	// ȯ�� �ڵ� ����
	//if (OCIEnvCreate(&envhp, OCI_DEFAULT, NULL, NULL, NULL, NULL, 0, NULL) !=
	//	OCI_SUCCESS) {
	//	printf("OCIEnvCreate failed\n");
	//	return -1;
	//}
	if (OCIEnvNlsCreate(&envhp, OCI_DEFAULT, NULL, NULL, NULL, NULL, 0, NULL, OCI_UTF8ID, OCI_UTF8ID) != OCI_SUCCESS) {
		printf("OCIEnvNlsCreate failed\n");
		return -1;
	}
	// ���� �ڵ� ����
	if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&errhp, OCI_HTYPE_ERROR, (size_t)0,
		(dvoid**)NULL) != OCI_SUCCESS) {
		printf("OCIHandleAlloc failed for error handle\n");
		return -1;
	}
	// ���� �ڵ� ����
	if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&srvhp, OCI_HTYPE_SERVER, (size_t)0,
		(dvoid**)NULL) != OCI_SUCCESS) {
		printf("OCIHandleAlloc failed for server handle\n");
		return -1;
	}
	// ���� ����
	if (OCIServerAttach(srvhp, errhp, (text*)dbname, strlen(dbname), OCI_DEFAULT) !=
		OCI_SUCCESS) {
		check_error(errhp);
		return -1;
	}
	// ���� ���ؽ�Ʈ ����
	if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&svchp, OCI_HTYPE_SVCCTX, (size_t)0,
		(dvoid**)NULL) != OCI_SUCCESS) {
		check_error(errhp);
		return -1;
	}
	// ���� �ڵ� ���� �� ����
	if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&usrhp, OCI_HTYPE_SESSION, (size_t)0,
		(dvoid**)NULL) != OCI_SUCCESS) {
		check_error(errhp);
		return -1;
	}
	if (OCILogon2(envhp, errhp, &svchp,
		(OraText*)username, (ub4)strlen(username),
		(OraText*)password, (ub4)strlen(password),
		(OraText*)dbname, (ub4)strlen(dbname),
		OCI_DEFAULT /* ������ �μ� �߰� */
	) != OCI_SUCCESS) {
		check_error(errhp);
		return -1;
	}
	//printf("Oracle Database connected successfully.\n");
}

void check_error(OCIError* errhp) {
	text errbuf[512];
	sb4 errcode = 0;
	OCIErrorGet((dvoid*)errhp, (ub4)1, (text*)NULL, &errcode, errbuf, (ub4)sizeof(errbuf),
		OCI_HTYPE_ERROR);
	printf("Error: %s\n", errbuf);
}

void quit_env() {
	OCILogoff(svchp, errhp);
	OCIHandleFree((dvoid*)usrhp, OCI_HTYPE_SESSION);
	OCIHandleFree((dvoid*)svchp, OCI_HTYPE_SVCCTX);
	OCIHandleFree((dvoid*)srvhp, OCI_HTYPE_SERVER);
	OCIHandleFree((dvoid*)errhp, OCI_HTYPE_ERROR);
	OCIHandleFree((dvoid*)envhp, OCI_HTYPE_ENV);
}