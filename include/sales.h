#ifndef SALES_H
#define SALES_H

#include <stdio.h>

#define SALES_F  "sales.txt"
#define NAME_LEN 40
#define REG_LEN  20
#define LINE_MAX 256

typedef union {
    char phone[16];
    char licence[24];
} Extra;

typedef struct {
    int id;
    char date[11];
    char vehReg[REG_LEN];
    char driver[NAME_LEN];
    char fuel[16];
    double liters;
    double rate;
    double total;
    int prefType;
    Extra ex;
    char payMethod[12];
} Sale;

/* function declarations */
char *today_date();
int next_sale_id();
void save_sale(Sale *s);
void make_sale();
void view_sales_today();
void totals_today();
void cancel_sale();

#endif