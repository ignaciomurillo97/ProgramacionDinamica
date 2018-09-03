#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "knapsack.h"

// item_t items[] = {
//     {"map",                      9,   150,   1},
//     {"compass",                 13,    35,   1},
//     {"water",                  153,   200,   2},
//     {"sandwich",                50,    60,   2},
//     {"glucose",                 15,    60,   2},
//     {"tin",                     68,    45,   3},
//     {"banana",                  27,    60,   3},
//     {"apple",                   39,    40,   3},
//     {"cheese",                  23,    30,   1},
//     {"beer",                    52,    10,   3},
//     {"suntan cream",            11,    70,   1},
//     {"camera",                  32,    30,   1},
//     {"T-shirt",                 24,    15,   2},
//     {"trousers",                48,    10,   2},
//     {"umbrella",                73,    40,   1},
//     {"waterproof trousers",     42,    70,   1},
//     {"waterproof overclothes",  43,    75,   1},
//     {"note-case",               22,    80,   1},
//     {"sunglasses",               7,    20,   1},
//     {"towel",                   18,    12,   2},
//     {"socks",                    4,    50,   1},
//     {"book",                    30,    10,   2},
// };

// item_t items[] = {
//     {"Candelero",                5,    10000, 1},
//     {"Radio",                    3,   9000,   1},
//     {"Póster de Elvis",          4,    5000,  1},
//     {"Anillo",                   1,  15000,   1},
// };

// item_t items[] = {
//     {"Agua",                   4,  11,   100},
//     {"Calcetines",             3,    7,  100},
//     {"Galletas",               5,  12,   100},
// };

int *knapsack (int knapsackCapacity, item_t* items, int n) {
    int i, j, k, v, *mm, **tabla, *s, *colorR, **color;
    mm = calloc((n + 1) * (knapsackCapacity + 1), sizeof (int));
    tabla = malloc((n + 1) * sizeof (int *));
    colorR = calloc((n + 1) * (knapsackCapacity + 1), sizeof (int));
    color = malloc((n + 1) * sizeof (int *));
    tabla[0] = mm;
    //Knapsack
    for (i = 1; i <= n; i++) {
        tabla[i] = &mm[i * (knapsackCapacity + 1)];
        color[i] = &colorR[i * (knapsackCapacity + 1)];
        for (j = 0; j <= knapsackCapacity; j++) {
            tabla[i][j] = tabla[i - 1][j];
            for (k = 1; k <= items[i - 1].count; k++) {
                if (k * items[i - 1].weight > j) {
                    break;
                }
                v = tabla[i - 1][j - k * items[i - 1].weight] + k * items[i - 1].value;
                if (v > tabla[i][j]) {
                    color[i][j]++;
                    tabla[i][j] = v;
                }
            }
        }
    }

    for (i = 1; i <= n; i++) {
        for (j = 0; j <= knapsackCapacity; j++) {
            printf("%d ",tabla[i][j]);
        }
        printf("\n ");
    }
    
    printf("COLORES \n ");
    for (i = 1; i <= n; i++) {
        for (j = 0; j <= knapsackCapacity; j++) {
            printf("%d ",color[i][j]);
        }
        printf("\n ");
    }

    //Respuesta
    s = calloc(n, sizeof (int));
    for (i = n, j = knapsackCapacity; i > 0; i--) {
        int v = tabla[i][j];
        for (k = 0; v != tabla[i - 1][j] + k * items[i - 1].value; k++) {
            s[i - 1]++;   
            j -= items[i - 1].weight;
        }
    }
    

    free(mm);
    free(tabla);
    return s;
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
 
