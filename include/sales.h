#ifndef SALES_H
#define SALES_H

#define SALES_F "sales.csv"
#define LINE_MAX 200
#define NAME_LEN 30
#define REG_LEN 20

typedef struct {
    int id;
    char date[11];
    char vehReg[REG_LEN];
    char driver[NAME_LEN];
    char fuel[10];
    double liters;
    double rate;
    double total;
} Sale;

char* today_date();
int next_sale_id();
void save_sale(Sale *s);
void make_sale();
void view_sales_today();
void totals_today();

#endif
