#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <oci.h>
#include <stdlib.h>

OCIEnv* envhp;
OCIError* errhp;
OCISvcCtx* svchp;
OCISession* usrhp;
OCIServer* srvhp;
OCIStmt* stmthp;
sword status;
OCIParam* param;

extern char* username;
extern char* password;
extern char* dbname;

void set_env();
void check_error(OCIError* errhp);
void quit_env();
