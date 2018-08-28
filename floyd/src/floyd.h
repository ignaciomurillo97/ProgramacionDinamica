struct FloydData {
  int nodeCount;
  int** routes;
  float*** optimizedDistances;
} FloydData;

struct FloydData *initFloydData(int pNodeCount, float **graph);
void optimize(struct FloydData* data);
void printOptimal(int tableIndex, struct FloydData *data);
void printRoutes(struct FloydData *data);
