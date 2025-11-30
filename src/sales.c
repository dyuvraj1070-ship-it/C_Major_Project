#include "sales.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/* --------------------------------------------------------
   Function: today_date()
   Purpose : Returns today's date in YYYY-MM-DD format.
   Used in: Saving sale date, showing today's sales.
   -------------------------------------------------------- */
char* today_date() {
    static char d[11];         // stores final date string
    time_t t = time(NULL);     // get current system time
    struct tm *now = localtime(&t);  // convert to date format

    // format the date into YYYY-MM-DD
    sprintf(d, "%04d-%02d-%02d",
            now->tm_year + 1900,
            now->tm_mon + 1,
            now->tm_mday);

    return d;   // return the date string
}

/* --------------------------------------------------------
   Function: next_sale_id()
   Purpose : Finds the next ID by reading the last entry
             from the sales.csv file.
   -------------------------------------------------------- */
int next_sale_id() {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) return 1;   // if file doesn't exist → start from ID 1

    int id = 1;
    int last_id = 0;
    char line[200];

    // read until last line to get last ID
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,", &last_id);
        id = last_id + 1;
    }

    fclose(fp);
    return id;
}

/* --------------------------------------------------------
   Function: save_sale()
   Purpose : Writes a sale record to the sales.csv file.
   -------------------------------------------------------- */
void save_sale(Sale *s) {
    FILE *fp = fopen(SALES_F, "a");   // open file in append mode

    if (!fp) {
        printf("Error opening sales file!\n");
        return;
    }

    // Write sale details in CSV format
    fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%.2f,%.2f\n",
            s->id, s->date, s->vehReg, s->driver,
            s->fuel, s->liters, s->rate, s->total);

    fclose(fp);
}

/* --------------------------------------------------------
   Function: make_sale()
   Purpose : Takes input from user and creates a sale.
             Automatically calculates rate & total.
   -------------------------------------------------------- */
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

    // Set fuel rate based on type
    if (strcmp(s.fuel, "Petrol") == 0)
        s.rate = 110;
    else if (strcmp(s.fuel, "Diesel") == 0)
        s.rate = 95;
    else
        s.rate = 100;  // other fuel type

    s.total = s.liters * s.rate;  // calculate total cost

    s.id = next_sale_id();        // get next ID
    strcpy(s.date, today_date()); // attach today's date

    save_sale(&s);   // save the sale to file

    printf("\nSale saved! Total Amount = %.2f\n", s.total);
}

/* --------------------------------------------------------
   Function: view_sales_today()
   Purpose : Reads the file and displays only today's sales.
   -------------------------------------------------------- */
void view_sales_today() {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) {
        printf("No sales file found!\n");
        return;
    }

    char today[11];
    strcpy(today, today_date());   // get today's date

    char line[200];
    int id;
    char date[11], veh[20], driver[30], fuel[10];
    double liters, rate, total;

    printf("\nToday's Sales (%s):\n", today);

    // Read every line from sales.csv
    while (fgets(line, sizeof(line), fp)) {

        // extract CSV values
        sscanf(line, "%d,%10[^,],%19[^,],%29[^,],%9[^,],%lf,%lf,%lf",
               &id, date, veh, driver, fuel, &liters, &rate, &total);

        // only show sales with today's date
        if (strcmp(date, today) == 0) {
            printf("%d  %s  %s  %.2f L  Rs %.2f\n",
                   id, veh, fuel, liters, total);
        }
    }

    fclose(fp);
}

/* --------------------------------------------------------
   Function: totals_today()
   Purpose : Calculates total liters of Petrol, Diesel,
             and Other fuels sold today.
   -------------------------------------------------------- */
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

    // read all sales
    while (fgets(line, sizeof(line), fp)) {

        // extract required fields
        sscanf(line, "%d,%10[^,],%*[^,],%*[^,],%9[^,],%lf",
               &id, date, fuel, &liters);

        // only count today's sales
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

    // print totals
    printf("\nToday's Totals:\n");
    printf("Petrol: %.2f L\n", petrol);
    printf("Diesel: %.2f L\n", diesel);
    printf("Other : %.2f L\n", other);
}
