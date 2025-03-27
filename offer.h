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
	char createdAt[20];
} OFFER;

//void convert_to_utf8(const wchar_t* wstr, char* utf8str, int utf8str_size);
int insert_offer(OFFER*);