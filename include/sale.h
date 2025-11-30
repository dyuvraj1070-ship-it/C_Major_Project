#ifndef SALES_H
#define SALES_H

#include "utils.h"

#define SALES_F  "sales.txt"
#define NAME_LEN 40
#define REG_LEN  20
#define LINE_MAX 512

typedef union { char phone[16]; char licence[24]; } Extra;

typedef struct {
    int id;
    char date[11]; // YYYY-MM-DD
    char vehReg[REG_LEN];
    char driver[NAME_LEN];
    char fuel[16]; // Petrol/Diesel etc
    double liters;
    double rate;
    double total;
    int prefType; // 0 none, 1 phone, 2 licence
    Extra ex;
    char payMethod[12]; // Cash/Card/UPI
} Sale;

/* core functions */
int next_sale_id(void);
void save_sale(const Sale *s);
void make_sale(void);
void view_sales_today(void);
void totals_today(void);
void cancel_sale(void);

#endif /* SALES_H */

