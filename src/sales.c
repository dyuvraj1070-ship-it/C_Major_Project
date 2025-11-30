#include "sales.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>

/* get today's date */
char *today_date() {
    static char buf[12];   // 11 + 1 extra for safety
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             tm_info->tm_year + 1900,
             tm_info->tm_mon + 1,
             tm_info->tm_mday);
    return buf;
}

/* next ID */
int next_sale_id() {
    FILE *fp = fopen(SALES_F, "r");
    if (!fp) return 1;

    char line[LINE_MAX], last[LINE_MAX];
    last[0] = 0;

    while (fgets(line, sizeof(line), fp))
        strcpy(last, line);

    fclose(fp);

    int id = 0;
    if (sscanf(last, "%d,", &id) == 1)
        return id + 1;

    return 1;
}

/* save sale */
void save_sale(Sale *s) {
    FILE *fp = fopen(SALES_F, "a");
    if (!fp) {
        printf("File open error!\n");
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
void make_sale() {
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
    strcpy(s.fuel, fuel);

    printf("Liters: ");
    scanf("%lf", &s.liters); getchar();

    if (strcasecmp(fuel,"Petrol")==0) s.rate=110;
    else if (strcasecmp(fuel,"Diesel")==0) s.rate=95;
    else s.rate=100;

    s.total = s.liters * s.rate;

    printf("Payment method: ");
    fgets(s.payMethod, sizeof(s.payMethod), stdin);
    s.payMethod[strcspn(s.payMethod,"\n")] = 0;

    printf("Extra info? 0-none 1-phone 2-licence: ");
    scanf("%d",&s.prefType); getchar();

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
    strcpy(s.date, today_date());

    save_sale(&s);

    printf("\nSale Successful! Bill Amount: %.2f\n", s.total);
}

/* today sales */
void view_sales_today() {
    FILE *fp = fopen(SALES_F,"r");
    if (!fp) { printf("No sales file!\n"); return; }

    char line[LINE_MAX], td[12];
    strcpy(td, today_date());

    printf("\nToday's Sales (%s)\n", td);

    while (fgets(line,sizeof(line),fp)) {
        int id,pref;
        char date[12],veh[REG_LEN],drv[NAME_LEN],fuel[16],pv[24],pay[12];
        double l,r,t;

        if (sscanf(line,"%d,%10[^,],%19[^,],%39[^,],%15[^,],%lf,%lf,%lf,%d,%23[^,],%11[^\n]",
            &id,date,veh,drv,fuel,&l,&r,&t,&pref,pv,pay)==11) {

            if (!strcmp(date,td))
                printf("%d  %s  %s  %.2f  %.2f\n",id,veh,fuel,l,t);
        }
    }
    fclose(fp);
}

/* totals */
void totals_today() {
    FILE *fp = fopen(SALES_F,"r");
    if (!fp) return;

    char td[12]; strcpy(td,today_date());
    char line[LINE_MAX];
    double p=0,d=0,o=0;

    while (fgets(line,sizeof(line),fp)) {
        int id,pref; char date[12],fuel[16];
        double l,r,t;

        sscanf(line,"%d,%10[^,],%*[^,],%*[^,],%15[^,],%lf,%lf,%lf,%d",
               &id,date,fuel,&l,&r,&t,&pref);

        if (!strcmp(date,td)) {
            if (!strcasecmp(fuel,"Petrol")) p+=l;
            else if (!strcasecmp(fuel,"Diesel")) d+=l;
            else o+=l;
        }
    }
    fclose(fp);

    printf("\nTotals: Petrol=%.2f  Diesel=%.2f  Other=%.2f\n",p,d,o);
}

/* cancel */
void cancel_sale() {
    int rid;
    printf("Enter ID to cancel: ");
    scanf("%d",&rid);

    FILE *fp=fopen(SALES_F,"r");
    FILE *tp=fopen("tmp.txt","w");
    if (!fp||!tp) return;

    char line[LINE_MAX]; int found=0;

    while (fgets(line,sizeof(line),fp)) {
        int id;
        sscanf(line,"%d,",&id);
        if (id==rid) { found=1; continue; }
        fputs(line,tp);
    }

    fclose(fp); fclose(tp);

    if (found) {
        remove(SALES_F);
        rename("tmp.txt",SALES_F);
        printf("Sale Deleted.\n");
    }
    else {
        remove("tmp.txt");
        printf("ID Not Found.\n");
    }
}