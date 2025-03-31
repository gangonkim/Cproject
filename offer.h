#pragma once


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


typedef struct {
	int id;
	char accountNum[20];
	char ticker[20];
	char status[20];
	int quantiy;
	int price;
	char createdAt[50];
} OFFER;

//void convert_to_utf8(const wchar_t* wstr, char* utf8str, int utf8str_size);
int insert_offer(OFFER*);
int count_offers(char* accountNum);
void get_offers(const char* accountNum, OFFER *, int count);
void check_trade(OFFER*);
void check_detail(OFFER* offer);