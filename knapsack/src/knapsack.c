#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "knapsack.h"

knapsackResult *knapsack (int knapsackCapacity, item_t* items, int n) {
    int i, j, k, v, *mm;
    knapsackResult *result = malloc(sizeof(knapsackResult));
    result->n = n;
    result->knapsackCapacity = knapsackCapacity;
    mm = calloc((n + 1) * (knapsackCapacity + 1), sizeof (int));
    result->tabla = malloc((n + 1) * sizeof (int *));
    result->colorR = calloc((n + 1) * (knapsackCapacity + 1), sizeof (int));
    result->color = malloc((n + 1) * sizeof (int *));
    result->tabla[0] = mm;
    //Knapsack
    for (i = 1; i <= n; i++) {
        result->tabla[i] = &mm[i * (knapsackCapacity + 1)];
        result->color[i] = &result->colorR[i * (knapsackCapacity + 1)];
        for (j = 0; j <= knapsackCapacity; j++) {
            result->tabla[i][j] = result->tabla[i - 1][j];
            for (k = 1; k <= items[i - 1].count; k++) {
                if (k * items[i - 1].weight > j) {
                    break;
                }
                v = result->tabla[i - 1][j - k * items[i - 1].weight] + k * items[i - 1].value;
                if (v > result->tabla[i][j]) {
                    result->color[i][j]++;
                    result->tabla[i][j] = v;
                }
            }
        }
    }

    //for (i = 1; i <= n; i++) {
    //    for (j = 0; j <= knapsackCapacity; j++) {
    //        printf("%d ",result->tabla[i][j]);
    //    }
    //    printf("\n ");
    //}
    //
    //printf("COLORES \n ");
    //for (i = 1; i <= n; i++) {
    //    for (j = 0; j <= knapsackCapacity; j++) {
    //        printf("%d ",result->color[i][j]);
    //    }
    //    printf("\n ");
    //}

    //Respuesta
    result->s = calloc(n, sizeof (int));
    for (i = n, j = knapsackCapacity; i > 0; i--) {
        int v = result->tabla[i][j];
        for (k = 0; v != result->tabla[i - 1][j] + k * items[i - 1].value; k++) {
            result->s[i - 1]++;   
            j -= items[i - 1].weight;
        }
    }
    

    //free(mm);
    //free(result->tabla);
    result->items = items;
    return result;
}

//int main () {
//    clock_t begin = clock();
//    int i, tc = 0, tw = 0, tv = 0, *s;
//    s = knapsack(10);
//    for (i = 0; i < n; i++) {
//        if (s[i]) {
//            printf("%-22s %5d %5d %5d\n", items[i].name, s[i], s[i] * items[i].weight, s[i] * items[i].value);
//            tc += s[i];
//            tw += s[i] * items[i].weight;
//            tv += s[i] * items[i].value;
//        }
//    }
//    printf("%-22s %5d %5d %5d\n", "count, weight, value:", tc, tw, tv);
//    clock_t end = clock();
//    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//    printf("time: %f", time_spent);
//    return 0;
//}
 
