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


  data->routes = (int**)malloc(sizeof(int*) * data->nodeCount);
  for (i = 0; i < data->nodeCount; i++) {
    data->routes[i] = (int*)malloc(sizeof(int) * data->nodeCount);
    for (j = 0; j < data->nodeCount; j++) {
      data->routes[i][j] = -1;
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
          data->routes[j][k] = currNode;
        } else {
          currentList[j][k] = old;
        }
      }
    }
  }
}

void printOptimal(int tableIndex, struct FloydData *data) {
  float*** matrices = data->optimizedDistances;
  int i, j;
  for (i = 0; i < data->nodeCount; i++) {
    for (j = 0; j < data->nodeCount; j++) {
      printf("%f, ", matrices[tableIndex][i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void printRoutes(struct FloydData *data) {
  int **routes = data->routes;
  int i, j;

  for (i = 0; i < data->nodeCount; i++) {
    for (j = 0; j < data->nodeCount; j++) {
      printf("%i, ", routes[i][j]);
    }
    printf("\n");
  }
}
