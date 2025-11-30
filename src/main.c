#include "sales.h"
#include <stdlib.h>

int main() {
    int ch;

    while (1) {
        printf("\n--- PETROL PUMP SYSTEM ---\n");
        printf("1. Make Sale\n");
        printf("2. View Today Sales\n");
        printf("3. Totals Today\n");
        printf("4. Cancel Sale\n");
        printf("5. Exit\n");
        printf("Choice: ");

        scanf("%d",&ch);

        switch(ch) {
            case 1: make_sale(); break;
            case 2: view_sales_today(); break;
            case 3: totals_today(); break;
            case 4: cancel_sale(); break;
            case 5: printf("Bye!\n"); exit(0);
            default: printf("Invalid Option!\n");
        }
    }
    return 0;
}