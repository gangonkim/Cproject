#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <oci.h>

#define MAX_NAME 50
#define DB_USER "C##DEV"
#define DB_PASS "sys1234"
#define DB_CONN "localhost:1521/xe"
#define FILE_NAME "transactions.csv"

// 거래 구조체 정의
typedef struct Transaction {
    int id;
    char customer[MAX_NAME];
    char stock[MAX_NAME];
    int type;
    int quantity;
    double price;
    struct Transaction* next;
} Transaction;

// 전역 변수 선언
extern Transaction* head;
extern int transactionCount;

// CSV 관련 함수
void saveToFile();
void loadFromFile();
void freeMemory();
void searchTransactionsByCustomer();

// DAT 관련 함수
void saveToStocksFile();
void loadFromStocksFile();

// DB 관련 함수
void initOCI();
void closeOCI();
void addTransaction();
void viewAllTransactions();
void updateTransaction();
void deleteTransaction();

#endif // TRANSACTIONS_H
