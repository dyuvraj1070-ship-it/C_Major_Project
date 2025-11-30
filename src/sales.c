#include "sales.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>
#include <stdio.h>

/* get today's date */
char *today_date(void) {
    static char buf[12];   // YYYY-MM-DD + NUL
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             tm_info->tm_year + 1900,
             tm_info->tm_mon + 1,
             tm_info->tm_mday);
    return buf;
}

/* next ID */
int next_sale_id(void) {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) return 1;

    char line[LINE_MAX], last[LINE_MAX];
    last[0] = 0;

    while (fgets(line, sizeof(line), fp))
        strcpy(last, line);

    fclose(fp);

    int id = 0;
    if (sscanf(last, "%d,", &id) == 1 && id > 0)
        return id + 1;

    return 1;
}

/* save sale */
void save_sale(Sale *s) {
    FILE *fp = fopen(SALES_F, "a");
    if (!fp) {
        perror("File open error");
        return;
    }

    if (s->prefType == 1)
        fprintf(fp,"%d,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d,%s,%s\n",
            s->id,s->date,s->vehReg,s->driver,s->fuel,
            s->liters,s->rate,s->total,s->prefType,
            s->ex.phone,s->payMethod);

    else if (s->prefType == 2)
        fprintf(fp,"%d,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d,%s,%s\n",
            s->id,s->date,s->vehReg,s->driver,s->fuel,
            s->liters,s->rate,s->total,s->prefType,
            s->ex.licence,s->payMethod);

    else
        fprintf(fp,"%d,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d,,%s\n",
            s->id,s->date,s->vehReg,s->driver,s->fuel,
            s->liters,s->rate,s->total,s->prefType,
            s->payMethod);

    fclose(fp);
}

/* make a sale */
void make_sale(void) {
    Sale s;
    char fuel[16];

    printf("Vehicle reg: ");
    scanf("%19s", s.vehReg); getchar();

    printf("Driver name: ");
    fgets(s.driver, NAME_LEN, stdin);
    s.driver[strcspn(s.driver,"\n")] = 0;

    printf("Fuel type: ");
    fgets(fuel, sizeof(fuel), stdin);
    fuel[strcspn(fuel,"\n")] = 0;
    strncpy(s.fuel, fuel, sizeof(s.fuel)-1);

    printf("Liters: ");
    if (scanf("%lf", &s.liters) != 1) { int c; while ((c=getchar())!=EOF && c!='\n'); printf("Invalid liters\n"); return; }
    getchar();

    if (strcasecmp(fuel,"Petrol")==0) s.rate=110;
    else if (strcasecmp(fuel,"Diesel")==0) s.rate=95;
    else s.rate=100;

    s.total = s.liters * s.rate;

    printf("Payment method: ");
    fgets(s.payMethod, sizeof(s.payMethod), stdin);
    s.payMethod[strcspn(s.payMethod,"\n")] = 0;

    printf("Extra info? 0-none 1-phone 2-licence: ");
    if (scanf("%d",&s.prefType) != 1) { int c; while ((c=getchar())!=EOF && c!='\n'); s.prefType = 0; }
    getchar();

    if (s.prefType == 1) {
        printf("Phone: ");
        fgets(s.ex.phone,sizeof(s.ex.phone),stdin);
        s.ex.phone[strcspn(s.ex.phone,"\n")] = 0;
    }
    else if (s.prefType == 2) {
        printf("Licence: ");
        fgets(s.ex.licence,sizeof(s.ex.licence),stdin);
        s.ex.licence[strcspn(s.ex.licence,"\n")] = 0;
    }

    s.id = next_sale_id();
    strncpy(s.date, today_date(), sizeof(s.date)-1);

    save_sale(&s);

    printf("\nSale Successful! Bill Amount: %.2f\n", s.total);
}

/* today sales */
void view_sales_today(void) {
    FILE *fp = fopen(SALES_F,"r");
    if (!fp) { printf("No sales file!\n"); return; }

    char line[LINE_MAX], td[12];
    strncpy(td, today_date(), sizeof(td)-1);

    printf("\nToday's Sales (%s)\n", td);

    while (fgets(line,sizeof(line),fp)) {
        int id,pref;
        char date[12],veh[REG_LEN],drv[NAME_LEN],fuel[16],pv[24],pay[12];
        double l,r,t;

        if (sscanf(line,"%d,%10[^,],%19[^,],%39[^,],%15[^,],%lf,%lf,%lf,%d,%23[^,],%11[^\n]",
            &id,date,veh,drv,fuel,&l,&r,&t,&pref,pv,pay)==11) {

            if (strcmp(date,td) == 0)
                printf("%d  %s  %s  %.2f  %.2f\n",id,veh,fuel,l,t);
        }
    }
    fclose(fp);
}

/* totals */
void totals_today(void) {
    FILE *fp = fopen(SALES_F,"r");
    if (!fp) return;

    char td[12]; strncpy(td,today_date(),sizeof(td)-1);
    char line[LINE_MAX];
    double p=0,d=0,o=0;

    while (fgets(line,sizeof(line),fp)) {
        int id,pref; char date[12],fuel[16];
        double l,r,t;

        if (sscanf(line,"%d,%10[^,],%*[^,],%*[^,],%15[^,],%lf,%lf,%lf,%d",
               &id,date,fuel,&l,&r,&t,&pref) >= 5) {

            if (strcmp(date,td) == 0) {
                if (!strcasecmp(fuel,"Petrol")) p+=l;
                else if (!strcasecmp(fuel,"Diesel")) d+=l;
                else o+=l;
            }
        }
    }
    fclose(fp);

    printf("\nTotals: Petrol=%.2f  Diesel=%.2f  Other=%.2f\n",p,d,o);
}
