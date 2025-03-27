#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


typedef struct {
	int id;
	int offernumber;
	int exchangeactual;
	char selleraccountnumber[20];
	char buyeraccountnumber[20];
	int quantiy;
	int price;
	int charge;
	char createdAt[20];
} TRADE;


void insert_trade(TRADE*);
