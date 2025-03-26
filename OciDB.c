//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <oci.h>
//
//#define MAX_NAME 50
//#define DB_USER "C##DEV"
//#define DB_PASS "sys1234"
//#define DB_CONN "localhost:1521/xe"
//#define FILE_NAME "transactions.csv"
//
////----------------CSV 코드-------------------
//
//// 거래 구조체 정의
//typedef struct Transaction {
//    int id;
//    char customer[MAX_NAME];
//    char stock[MAX_NAME];
//    int type;  // "매수" 또는 "매도"
//    int quantity;
//    double price;
//    struct Transaction* next;
//} Transaction;
//
//Transaction* head = NULL;  // 거래 목록의 시작 포인터
//int transactionCount = 1;  // 거래 ID 자동 증가
//
//// 특정 고객의 거래 검색
//void searchTransactionsByCustomer() {
//    char customerName[MAX_NAME];
//    printf("검색할 고객 이름: ");
//    scanf("%s", customerName);
//
//    Transaction* current = head;
//    int found = 0;
//
//    printf("\n=== %s님의 거래 내역 ===\n", customerName);
//    while (current) {
//        if (strcmp(current->customer, customerName) == 0) {
//            printf("ID: %d, 종목: %s, 유형: %d, 수량: %d, 가격: %.2lf\n",
//                current->id, current->stock, current->type,
//                current->quantity, current->price);
//            found = 1;
//        }
//        current = current->next;
//    }
//
//    if (!found) printf("해당 고객의 거래 내역이 없습니다.\n");
//}
//
//// 거래 내역을 파일에 저장
//void saveToFile() {
//    FILE* file = fopen(FILE_NAME, "w");
//    if (!file) {
//        printf("파일 저장 오류!\n");
//        return;
//    }
//
//    Transaction* current = head;
//    while (current) {
//        fprintf(file, "%d,%s,%s,%d,%d,%.2lf\n", current->id,
//            current->customer, current->stock, current->type,
//            current->quantity, current->price);
//        current = current->next;
//    }
//
//    fclose(file);
//    printf("파일 저장 완료.\n");
//}
//
//// 파일에서 거래 내역 불러오기
//void loadFromFile() {
//    FILE* file = fopen(FILE_NAME, "r");
//    if (!file) return;
//
//    while (!feof(file)) {
//        Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
//        if (!newTransaction) return;
//
//        if (fscanf(file, "%d,%49[^,],%49[^,],%d,%d,%lf\n",
//            &newTransaction->id, newTransaction->customer, newTransaction->stock,
//            &newTransaction->type, &newTransaction->quantity, &newTransaction->price) != 6) {
//            free(newTransaction);
//            break;
//        }
//
//        newTransaction->next = head;
//        head = newTransaction;
//        if (newTransaction->id >= transactionCount) {
//            transactionCount = newTransaction->id + 1;
//        }
//    }
//
//    fclose(file);
//}
//
//// 프로그램 종료 시 메모리 해제
//void freeMemory() {
//    Transaction* current = head;
//    while (current) {
//        Transaction* temp = current;
//        current = current->next;
//        free(temp);
//    }
//}
//
////----------------DAT 코드-------------------
//
//
//// 거래 내역을 stocks.dat 파일에 저장
//void saveToStocksFile() {
//    FILE* file = fopen("stocks.dat", "wb");
//    if (!file) {
//        printf("파일 저장 오류!\n");
//        return;
//    }
//
//    Transaction* current = head;
//    while (current) {
//        fwrite(current, sizeof(Transaction), 1, file);
//        current = current->next;
//    }
//
//    fclose(file);
//    printf("stocks.dat 파일에 거래 내역 저장 완료.\n");
//}
//
//// stocks.dat 파일에서 거래 내역 불러오기
//void loadFromStocksFile() {
//    FILE* file = fopen("stocks.dat", "rb");
//    if (!file) return;
//
//    while (!feof(file)) {
//        Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
//        if (!newTransaction) return;
//
//        // 데이터 읽기
//        if (fread(newTransaction, sizeof(Transaction), 1, file) != 1) {
//            free(newTransaction);
//            break;
//        }
//
//        newTransaction->next = head;
//        head = newTransaction;
//        if (newTransaction->id >= transactionCount) {
//            transactionCount = newTransaction->id + 1;
//        }
//    }
//
//    fclose(file);
//}
//
//
////----------------DB 코드-------------------
//
//
//// OCI 환경 변수
//OCIEnv* env;
//OCIError* err;
//OCISvcCtx* svc;
//OCIStmt* stmt;
//OCIDefine* defn;
//
//// OCI 초기화 함수
//void initOCI() {
//    OCIInitialize(OCI_DEFAULT, NULL, NULL, NULL, NULL);
//    OCIEnvInit(&env, OCI_DEFAULT, 0, NULL);
//    OCIHandleAlloc(env, (dvoid**)&err, OCI_HTYPE_ERROR, 0, NULL);
//    OCIHandleAlloc(env, (dvoid**)&svc, OCI_HTYPE_SVCCTX, 0, NULL);
//
//    OCILogon(env, err, &svc, (const OraText*)DB_USER, strlen(DB_USER),
//        (const OraText*)DB_PASS, strlen(DB_PASS),
//        (const OraText*)DB_CONN, strlen(DB_CONN));
//}
//
//// OCI 종료 함수
//void closeOCI() {
//    OCILogoff(svc, err);
//    OCIHandleFree((dvoid*)err, OCI_HTYPE_ERROR);
//    OCIHandleFree((dvoid*)svc, OCI_HTYPE_SVCCTX);
//}
//
//// 새로운 거래 추가
//void addTransaction() {
//    Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
//    if (newTransaction == NULL) {
//        printf("메모리 할당 실패\n");
//        return;
//    }
//
//    printf("고객 이름: ");
//    scanf("%s", newTransaction->customer);
//    printf("주식 종목명: ");
//    scanf("%s", newTransaction->stock);
//    printf("거래 유형 (매수: 0/매도: 1): ");
//    scanf("%d", &newTransaction->type);
//    printf("거래 수량: ");
//    scanf("%d", &newTransaction->quantity);
//    printf("거래 가격: ");
//    scanf("%lf", &newTransaction->price);
//
//    // Linked List에 추가
//    newTransaction->id = transactionCount++;
//    newTransaction->next = head;
//    head = newTransaction;
//
//    // 데이터베이스에 추가
//    char sql[256];
//    sprintf(sql, "INSERT INTO transaction (id, customer, stock, type, quantity, price) VALUES (TRANSACTION_SEQ.NEXTVAL, '%s', '%s', %d, %d, %.2f)",
//        newTransaction->customer, newTransaction->stock, newTransaction->type, newTransaction->quantity, newTransaction->price);
//
//    OCIHandleAlloc(env, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);
//    OCIStmtPrepare(stmt, err, (const OraText*)sql, strlen(sql), OCI_NTV_SYNTAX, OCI_DEFAULT);
//    OCIStmtExecute(svc, stmt, err, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS);
//    OCIHandleFree(stmt, OCI_HTYPE_STMT);
//    OCITransCommit(svc, err, OCI_DEFAULT);
//
//    printf("거래 추가 완료! 거래 ID: %d\n", newTransaction->id);
//}
//
//
////거래 목록 가져오기
//void viewAllTransactions() {
//
//    // 데이터베이스에서 모든 거래 출력
//    char sql[] = "SELECT id, customer, stock, type, quantity, price FROM transaction";
//    OCIHandleAlloc(env, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);
//    OCIStmtPrepare(stmt, err, (const OraText*)sql, strlen(sql), OCI_NTV_SYNTAX, OCI_DEFAULT);
//    OCIStmtExecute(svc, stmt, err, 0, 0, NULL, NULL, OCI_DEFAULT);
//
//    int id, quantity, type;
//    double price;
//    char customer[MAX_NAME], stock[MAX_NAME];
//
//    OCIParam* param;
//    OCIDefine* defn1, * defn2, * defn3, * defn4, * defn5, * defn6;
//    OCIDefineByPos(stmt, &defn1, err, 1, &id, sizeof(id), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
//    OCIDefineByPos(stmt, &defn2, err, 2, customer, sizeof(customer), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
//    OCIDefineByPos(stmt, &defn3, err, 3, stock, sizeof(stock), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
//    OCIDefineByPos(stmt, &defn4, err, 4, 
//        &type, sizeof(type), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
//    OCIDefineByPos(stmt, &defn5, err, 5, &quantity, sizeof(quantity), SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
//    OCIDefineByPos(stmt, &defn6, err, 6, &price, sizeof(price), SQLT_FLT, NULL, NULL, NULL, OCI_DEFAULT);
//
//    printf("\n=== 모든 거래 내역 ===\n");
//    while (OCIStmtFetch2(stmt, err, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
//        printf("ID: %d, 고객: %s, 종목: %s, 유형: %d, 수량: %d, 가격: %.2lf\n",
//            id, customer, stock, type, quantity, price);
//    }
//
//    OCIHandleFree(stmt, OCI_HTYPE_STMT);
//}
//
//void updateTransaction() {
//    int id, quantity;
//    double price;
//
//    printf("수정할 거래 ID 입력: ");
//    scanf("%d", &id);
//
//    // 연결 리스트에서 거래 수정
//    Transaction* current = head;
//    while (current) {
//        if (current->id == id) {
//            printf("새로운 수량 입력: ");
//            scanf("%d", &quantity);
//            printf("새로운 가격 입력: ");
//            scanf("%lf", &price);
//
//            current->quantity = quantity;
//            current->price = price;
//
//            // 데이터베이스에서 거래 수정
//            char sql[256];
//            sprintf(sql, "UPDATE transaction SET quantity = %d, price = %.2lf WHERE id = %d", quantity, price, id);
//
//            OCIHandleAlloc(env, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);
//            OCIStmtPrepare(stmt, err, (const OraText*)sql, strlen(sql), OCI_NTV_SYNTAX, OCI_DEFAULT);
//            OCIStmtExecute(svc, stmt, err, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS);
//            OCIHandleFree(stmt, OCI_HTYPE_STMT);
//
//            printf("거래가 수정되었습니다.\n");
//            return;
//        }
//        current = current->next;
//    }
//
//    printf("해당 ID의 거래가 존재하지 않습니다.\n");
//}
//
//
//// 특정 거래 삭제
//void deleteTransaction() {
//    int id;
//    printf("삭제할 거래 ID 입력: ");
//    scanf("%d", &id);
//
//    // 연결 리스트에서 거래 삭제
//    Transaction* current = head;
//    Transaction* prev = NULL;
//    while (current) {
//        if (current->id == id) {
//            if (prev) {
//                prev->next = current->next;
//            }
//            else {
//                head = current->next;
//            }
//            free(current);
//            printf("연결 리스트에서 거래가 삭제되었습니다.\n");
//
//            // 데이터베이스에서 거래 삭제
//            char sql[256];
//            sprintf(sql, "DELETE FROM transaction WHERE id = %d", id);
//
//            OCIHandleAlloc(env, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);
//            OCIStmtPrepare(stmt, err, (const OraText*)sql, strlen(sql), OCI_NTV_SYNTAX, OCI_DEFAULT);
//            OCIStmtExecute(svc, stmt, err, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS);
//            OCIHandleFree(stmt, OCI_HTYPE_STMT);
//
//            printf("데이터베이스에서 거래가 삭제되었습니다.\n");
//            return;
//        }
//        prev = current;
//        current = current->next;
//    }
//
//    printf("해당 ID의 거래가 존재하지 않습니다.\n");
//}
//
//
//// 메인 메뉴
//void menu() {
//    int choice;
//    initOCI();
//    loadFromFile();
//    loadFromStocksFile();
//    while (1) {
//        printf("\n==== 주식 거래 관리 시스템 ====\n");
//        printf("1. 거래 추가\n2. 거래 조회\n3. 거래 수정\n4. 거래 삭제\n5. 종료\n");
//        printf("선택: ");
//        scanf("%d", &choice);
//
//        switch (choice) {
//        case 1: addTransaction(); break;
//        case 2: viewAllTransactions(); break;
//        case 3: updateTransaction(); break;
//        case 4: deleteTransaction(); break;
//        case 5:
//            closeOCI();
//            saveToFile();
//            saveToStocksFile();
//            freeMemory();
//            printf("프로그램 종료.\n");
//            return;
//        default: printf("잘못된 입력입니다.\n");
//        }
//    }
//}
//
//// 메인 함수
//int main() {
//    menu();
//    return 0;
//}
