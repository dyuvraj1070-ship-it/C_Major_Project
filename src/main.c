#include "sales.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int ch;

    while (1) {
        printf("\n--- PETROL PUMP SYSTEM ---\n");
        printf("1. Make Sale\n");
        printf("2. View Today Sales\n");
        printf("3. Totals Today\n");
        printf("4. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &ch) != 1) {
            /* clear bad input */
            int c; while ((c = getchar()) != EOF && c != '\n');
            printf("Invalid input\n");
            continue;
        }

        switch (ch) {
            case 1: make_sale(); break;
            case 2: view_sales_today(); break;
            case 3: totals_today(); break;
            case 4: printf("Bye!\n"); exit(0);
            default: printf("Invalid Option!\n");
        }
    }
    return 0;
}
