#pragma once

typedef struct {
	char accountNum[20];
	char ticker[20];
	int quantiy;
	float purchasePrice;
	float currentPrice;
	float valuation_pl;
	float earnings_rate;
	
} HOLDING;

void get_holding(HOLDING*);
void insert_holding(HOLDING*);
void update_holding(HOLDING*, HOLDING*, int type);
void delete_holding(HOLDING*);

