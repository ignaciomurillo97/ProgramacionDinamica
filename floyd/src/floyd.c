#include <stdlib.h>
#include <math.h>
#include "floyd.h"
#include <stdio.h>


struct FloydData *initFloydData(int pNodeCount, float **graph) {
  int i, j;
  struct FloydData *data = (struct FloydData*)malloc(sizeof(struct FloydData*));
  data->nodeCount = pNodeCount;

  // Se reservan los espacios para las n matrices + la matriz original
  data->optimizedDistances = (float***)malloc(sizeof(float**) * (data->nodeCount + 1));
  data->optimizedDistances[0] = graph;
  for (i = 1; i <= data->nodeCount; i++) {
    data->optimizedDistances[i] = (float**)malloc(sizeof(float*) * data->nodeCount);
    for (j = 0; j < data->nodeCount; j ++) {
      data->optimizedDistances[i][j] = (float*)malloc(sizeof(float) * data->nodeCount);
    }
  }
  return data;
}

void optimize(struct FloydData *data) {
  int i, j, k;
  for (i = 1; i < data->nodeCount + 1; i++) {
    float** previousList = data->optimizedDistances[i - 1];
    float** currentList = data->optimizedDistances[i];
    int currNode = i - 1;
    for (j = 0; j < data->nodeCount; j++) {
      for (k = 0; k < data->nodeCount; k++) {
        float new = previousList[j][currNode] + previousList[currNode][k];
        float old = previousList[j][k];
        if (new < old) {
          currentList[j][k] = new;
        } else {
          currentList[j][k] = old;
        }
      }
    }
  }
}

void printOptimal(int tableIndex, struct FloydData *data) {
  float*** matrices = data->optimizedDistances;
  for (int i = 0; i < data->nodeCount; i++) {
    for (int j = 0; j < data->nodeCount; j++) {
      printf("%f, ", matrices[tableIndex][i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
