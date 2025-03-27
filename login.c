#include "Env.h"
void printLogo() {
    // DAOUFIT 로고 출력 (▣ 문자 사용)
    printf(" ▣▣▣▣▣▣      ▣▣▣▣▣     ▣▣▣▣▣    ▣   ▣   ▣▣▣▣▣    ▣▣▣  ▣▣▣▣▣ \n");
    printf(" ▣     ▣    ▣     ▣   ▣     ▣   ▣   ▣   ▣         ▣     ▣   \n");
    printf(" ▣     ▣    ▣ ▣▣▣▣▣   ▣     ▣   ▣   ▣   ▣▣▣▣      ▣     ▣   \n");
    printf(" ▣     ▣    ▣     ▣   ▣     ▣   ▣   ▣   ▣         ▣     ▣   \n");
    printf(" ▣▣▣▣▣▣     ▣     ▣    ▣▣▣▣▣    ▣▣▣▣▣   ▣         ▣     ▣   \n");

    printf("\n");
}


void displayWelcomeMessage() {
    // 환영 메시지 출력
    printf("******************************************************\n");
    printf("*                                                    *\n");
    printf("*                DAOUFIT에 오신 것을                 *\n");
    printf("*             환영합니다! 로그인해주세요.            *\n");
    printf("*                                                    *\n");
    printf("******************************************************\n");
}

void login() {
    set_env();
	printLogo();
	displayWelcomeMessage();
    char username[50];
    char password[50];

    // 로그인 입력 받기
    printf("아이디를 입력하세요: ");
    scanf("%s", username);

    printf("비밀번호를 입력하세요: ");
    int i = 0;
    while (1) {
        char ch = getch();
        if (ch == 13) {      // Enter 키를 누르면 입력 종료
            break;
        }
        if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        }
        else {
            password[i++] = ch;
            printf("*");  // 입력된 문자는 화면에 '*'로 표시
        }
    }

    password[i] = '\0';  // 문자열 종료

    char* select_sql = "SELECT userid, name FROM USERS WHERE userid = :1 AND password = :2";
    OCIHandleAlloc(envhp, (void**)&stmthp, OCI_HTYPE_STMT, 0, NULL);
    OCIStmtPrepare(stmthp, errhp, (text*)select_sql, strlen(select_sql), OCI_NTV_SYNTAX, OCI_DEFAULT);
    //OCIStmtExecute(svchp, stmthp, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

    //바인딩
    OCIBind* bind1 = NULL, * bind2 = NULL;
    OCIBindByPos(stmthp, &bind1, errhp, 1, username, sizeof(username), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
    OCIBindByPos(stmthp, &bind2, errhp, 2, password, sizeof(password), SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);


	if (OCIStmtExecute(svchp, stmthp, errhp, 0, 0, NULL, NULL, OCI_DEFAULT) != OCI_SUCCESS) {
        quit_env();
        cls;
        printf("로그인 실패! 아이디 또는 비밀번호를 확인해 주세요.\n");
        //check_error(errhp);
        login(stmthp, errhp, svchp);
    }

    else {
        char name[50], userId[50];
        OCIDefine* def1 = NULL, * def2 = NULL;

        OCIDefineByPos(stmthp, &def1, errhp, 1, userId, sizeof(userId), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT); 
        OCIDefineByPos(stmthp, &def1, errhp, 2, name, sizeof(name), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);

    
        if ((status = OCIStmtFetch2(stmthp, errhp, 1, OCI_DEFAULT, 0, OCI_DEFAULT)) == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {

            strcpy(authUser, name);
            strcpy(authId, userId);

            printf("\n\n%s 님, 환영합니다!\n", name);
            printf("아무 키나 누르면 계속 진행합니다.");

            pause;
            cls;
            quit_env();
        }
        else {
			quit_env();
			cls;
			printf("로그인 실패! 아이디 또는 비밀번호를 확인해 주세요.\n");
			login(stmthp, errhp, svchp);
        }
    }
}