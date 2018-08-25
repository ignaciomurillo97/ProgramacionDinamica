struct FloydData {
  int nodeCount;
  float** routes;
  float*** optimizedDistances;
} FloydData;

struct FloydData *initFloydData(int pNodeCount, float **graph);
void optimize(struct FloydData* data);
