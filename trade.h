#pragma once


typedef struct {
    int id;
    int offernumber;
    int exchangeactual;
    char selleraccountnumber[20];
    char buyeraccountnumber[20];
    int quantity;
    int price;
    double charge;
    int comparison;
    char createdAt[20];

} TRADE;


void insert_trade(TRADE*);
char* get_account(char* );

