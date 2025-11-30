#include "sales.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Get today's date in YYYY-MM-DD format */
char* today_date() {
    static char d[11];
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    sprintf(d, "%04d-%02d-%02d",
            now->tm_year + 1900,
            now->tm_mon + 1,
            now->tm_mday);

    return d;
}

/* Find next sale ID */
int next_sale_id() {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) return 1;

    int id = 1;
    int last_id = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,", &last_id);
        id = last_id + 1;
    }

    fclose(fp);
    return id;
}

/* Save sale to sales.csv */
void save_sale(Sale *s) {
    FILE *fp = fopen(SALES_F, "a");

    if (!fp) {
        printf("Error opening sales file!\n");
        return;
    }

    fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%.2f,%.2f\n",
            s->id, s->date, s->vehReg, s->driver,
            s->fuel, s->liters, s->rate, s->total);

    fclose(fp);
}

/* Make a new sale */
void make_sale() {
    Sale s;
    char fuel[10];

    printf("Vehicle number: ");
    scanf("%s", s.vehReg);

    printf("Driver name: ");
    scanf("%s", s.driver);

    printf("Fuel type (Petrol/Diesel): ");
    scanf("%s", s.fuel);

    printf("Liters: ");
    scanf("%lf", &s.liters);

    if (strcmp(s.fuel, "Petrol") == 0)
        s.rate = 110;
    else if (strcmp(s.fuel, "Diesel") == 0)
        s.rate = 95;
    else
        s.rate = 100;

    s.total = s.liters * s.rate;

    s.id = next_sale_id();
    strcpy(s.date, today_date());

    save_sale(&s);

    printf("\nSale saved! Total Amount = %.2f\n", s.total);
}

/* View today's sales */
void view_sales_today() {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) {
        printf("No sales file found!\n");
        return;
    }

    char today[11];
    strcpy(today, today_date());

    char line[200];
    int id;
    char date[11], veh[20], driver[30], fuel[10];
    double liters, rate, total;

    printf("\nToday's Sales (%s):\n", today);

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%10[^,],%19[^,],%29[^,],%9[^,],%lf,%lf,%lf",
               &id, date, veh, driver, fuel, &liters, &rate, &total);

        if (strcmp(date, today) == 0) {
            printf("%d  %s  %s  %.2f L  Rs %.2f\n",
                   id, veh, fuel, liters, total);
        }
    }

    fclose(fp);
}

/* Show totals for today */
void totals_today() {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) {
        printf("No sales file!\n");
        return;
    }

    char today[11];
    strcpy(today, today_date());

    char line[200];
    char date[11], fuel[10];
    double liters;
    int id;

    double petrol = 0, diesel = 0, other = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%10[^,],%*[^,],%*[^,],%9[^,],%lf",
               &id, date, fuel, &liters);

        if (strcmp(date, today) == 0) {
            if (strcmp(fuel, "Petrol") == 0)
                petrol += liters;
            else if (strcmp(fuel, "Diesel") == 0)
                diesel += liters;
            else
                other += liters;
        }
    }

    fclose(fp);

    printf("\nToday's Totals:\n");
    printf("Petrol: %.2f L\n", petrol);
    printf("Diesel: %.2f L\n", diesel);
    printf("Other : %.2f L\n", other);
}
