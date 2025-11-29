/* petrol_pump_nostock.c
   Petrol Pump Management System (NO stock)
   Uses: structs, union, file handling
   File used:
    - sales.txt  (id,date,veh,driver,fuel,liters,rate,total,prefType,prefVal,payMethod)
   Compile: gcc -o petrol_pump_nostock petrol_pump_nostock.c
   Run:     ./petrol_pump_nostock
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SALES_F  "sales.txt"
#define NAME_LEN 40
#define REG_LEN  20
#define LINE_MAX 256

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

/* get today's date */
char *today_date() {
    static char buf[11];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(buf, "%04d-%02d-%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
    return buf;
}

/* get next sale id by reading last line of sales file */
int next_sale_id() {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) return 1;
    char line[LINE_MAX], last[LINE_MAX]; last[0]=0;
    while (fgets(line, sizeof(line), fp)) strcpy(last, line);
    fclose(fp);
    if (last[0]==0) return 1;
    int id=0;
    if (sscanf(last, "%d,", &id)==1) return id+1;
    return 1;
}

/* append sale to file */
void save_sale(Sale *s) {
    FILE *fp = fopen(SALES_F, "a");
    if (!fp) { perror("save_sale"); return; }
    if (s->prefType==1)
        fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d,%s,%s\n",
                s->id,s->date,s->vehReg,s->driver,s->fuel,s->liters,s->rate,s->total,
                s->prefType,s->ex.phone,s->payMethod);
    else if (s->prefType==2)
        fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d,%s,%s\n",
                s->id,s->date,s->vehReg,s->driver,s->fuel,s->liters,s->rate,s->total,
                s->prefType,s->ex.licence,s->payMethod);
    else
        fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d,,%s\n",
                s->id,s->date,s->vehReg,s->driver,s->fuel,s->liters,s->rate,s->total,
                s->prefType,s->payMethod);
    fclose(fp);
}

/* make a sale (no stock checks) */
void make_sale() {
    char reg[REG_LEN], driver[NAME_LEN], pay[12], fuel[16];
    double liters;
    printf("Vehicle reg: "); scanf("%19s", reg); getchar();
    printf("Driver name: "); fgets(driver, NAME_LEN, stdin); driver[strcspn(driver, "\n")] = 0;
    printf("Fuel type (e.g. Petrol/Diesel): "); fgets(fuel, sizeof(fuel), stdin); fuel[strcspn(fuel, "\n")] = 0;
    printf("Liters to dispense: "); if (scanf("%lf",&liters)!=1) return;
    // simple fixed rates
    double rate;
    if (strcasecmp(fuel, "Petrol") == 0) rate = 110.0;
    else if (strcasecmp(fuel, "Diesel") == 0) rate = 95.0;
    else rate = 100.0;
    double total = liters * rate;
    getchar();
    printf("Payment method (Cash/Card/UPI): "); fgets(pay, sizeof(pay), stdin); pay[strcspn(pay,"\n")]=0;

    int pref; Extra ex; pref=0; ex.phone[0]=0; ex.licence[0]=0;
    printf("Add extra info? 0-none 1-phone 2-licence : ");
    if (scanf("%d",&pref)!=1) pref=0;
    getchar();
    if (pref==1) { printf("Phone: "); fgets(ex.phone, sizeof(ex.phone), stdin); ex.phone[strcspn(ex.phone,"\n")]=0; }
    else if (pref==2) { printf("Licence: "); fgets(ex.licence, sizeof(ex.licence), stdin); ex.licence[strcspn(ex.licence,"\n")]=0; }

    Sale s;
    s.id = next_sale_id();
    strncpy(s.date, today_date(), sizeof(s.date));
    strncpy(s.vehReg, reg, sizeof(s.vehReg));
    strncpy(s.driver, driver, sizeof(s.driver));
    strncpy(s.fuel, fuel, sizeof(s.fuel));
    s.liters = liters; s.rate = rate; s.total = total; s.prefType = pref;
    if (pref==1) strcpy(s.ex.phone, ex.phone); else if (pref==2) strcpy(s.ex.licence, ex.licence);
    strcpy(s.payMethod, pay);

    save_sale(&s);

    printf("\n--- BILL ---\nID:%d  Date:%s\nVeh:%s  Driver:%s\nFuel:%s  Liters:%.2f  Rate:%.2f\nTotal: %.2f\nPayment:%s\n-------------\n",
           s.id, s.date, s.vehReg, s.driver, s.fuel, s.liters, s.rate, s.total, s.payMethod);
}

/* view today's sales */
void view_sales_today() {
    FILE *fp = fopen(SALES_F,"r");
    if (!fp) { printf("No sales yet.\n"); return; }
    char line[LINE_MAX]; char td[12]; strncpy(td, today_date(), 11);
    int any=0;
    printf("Sales for %s:\nID | Veh   | Fuel    | L     | Total   | Pay\n-----------------------------------------------\n", td);
    while (fgets(line,sizeof(line),fp)) {
        int id; char date[12], veh[REG_LEN], drv[NAME_LEN], fuel[16], prefVal[40], pay[12];
        double l, r, tot; int pref;
        if (sscanf(line,"%d,%10[^,],%19[^,],%39[^,],%15[^,],%lf,%lf,%lf,%d,%39[^,],%11[^\n]",
                   &id,date,veh,drv,fuel,&l,&r,&tot,&pref,prefVal,pay) >= 9) {
            if (strcmp(date,td)!=0) continue;
            any=1;
            printf("%2d | %-5s | %-7s | %-5.2f | %-7.2f | %s\n", id, veh, fuel, l, tot, pay);
        }
    }
    fclose(fp);
    if (!any) printf("No sales for today.\n");
}

/* totals per fuel today */
void totals_today() {
    FILE *fp = fopen(SALES_F,"r");
    if (!fp) { printf("No sales yet.\n"); return; }
    char td[12]; strncpy(td, today_date(), 11);
    char line[LINE_MAX];
    double pet=0, dies=0, other=0; int cnt=0;
    while (fgets(line,sizeof(line),fp)) {
        int id; char date[12], fuel[16]; double l, r, tot; int pref;
        if (sscanf(line,"%d,%10[^,],%*[^,],%*[^,],%15[^,],%lf,%lf,%lf,%d,%*[^,],%*s",
                   &id,date,fuel,&l,&r,&tot,&pref) >= 7) {
            if (strcmp(date,td)!=0) continue;
            cnt++;
            if (strcasecmp(fuel,"Petrol")==0) pet += l;
            else if (strcasecmp(fuel,"Diesel")==0) dies += l;
            else other += l;
        }
    }
    fclose(fp);
    printf("Today's totals (%s): Sales=%d, Petrol=%.2fL, Diesel=%.2fL, Other=%.2fL\n", td, cnt, pet, dies, other);
}

/* cancel a sale by id (deletes from file) */
void cancel_sale() {
    int idrm; printf("Enter sale ID to cancel: "); if (scanf("%d",&idrm)!=1) return;
    FILE *fp = fopen(SALES_F,"r");
    if (!fp) { printf("No sales file.\n"); return; }
    FILE *tmp = fopen("tmp_sales.txt","w");
    if (!tmp) { fclose(fp); return; }
    char line[LINE_MAX]; int found=0;
    while (fgets(line,sizeof(line),fp)) {
        int id; 
        if (sscanf(line, "%d,", &id) >= 1) {
            if (id == idrm) { found = 1; continue; } // skip this sale
        }
        fputs(line, tmp);
    }
    fclose(fp); fclose(tmp);
    if (found) {
        remove(SALES_F);
        rename("tmp_sales.txt", SALES_F);
        printf("Sale %d cancelled.\n", idrm);
    } else {
        remove("tmp_sales.txt");
        printf("Sale id %d not found.\n", idrm);
    }
}

int main() {
    int ch;
    while (1) {
        printf("\n--- Petrol Pump Management (No Stock) ---\n");
        printf("1. Make sale\n2. View today's sales\n3. Today's totals\n4. Cancel sale\n5. Exit\nChoice: ");
        if (scanf("%d",&ch)!=1) break;
        getchar();
        switch(ch) {
            case 1: make_sale(); break;
            case 2: view_sales_today(); break;
            case 3: totals_today(); break;
            case 4: cancel_sale(); break;
            case 5: printf("Bye\n"); exit(0);
            default: printf("Invalid\n"); break;
        }
    }
    return 0;
}
