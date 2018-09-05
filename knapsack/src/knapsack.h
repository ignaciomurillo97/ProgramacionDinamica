typedef struct {
    char *name;
    int weight;
    int value;
    int count;
} item_t;

typedef struct {
  int **tabla;
  int **color;
  int *colorR;
  int *s;
  int knapsackCapacity;
  int n;
} knapsackResult;

knapsackResult *knapsack (int knapsackCapacity, item_t* items, int n);
