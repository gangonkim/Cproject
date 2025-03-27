#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <oci.h>
#include <stdlib.h>

/* ▼▼▼▼▼ define 선언 ▼▼▼▼▼ */
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
char authUser[50];
char authId[50];
char authUserType[20];

#define pause system("pause > nul")  
#define cls system("cls")   
/* ▲▲▲▲▲ define 선언 ▲▲▲▲▲ */

void set_env();
void check_error(OCIError* errhp);
void quit_env();
void login();
void printProgressBar(int percent);
void exportJson(int n);