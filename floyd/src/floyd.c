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
    //float** previousList = data->optimizedDistances[i - 1];
    //float** currentList = data->optimizedDistances[i];
    int currNode = i - 1;
    for (j = 0; j < data->nodeCount; j++) {
      for (k = 0; k < data->nodeCount; k++) {
        float new = data->optimizedDistances[currNode][j][i - 1] + data->optimizedDistances[currNode][i - 1][k];
        float old = data->optimizedDistances[currNode][j][k];
        printf("i: %i, j: %i, k: %i; ", currNode, j, i - 1);
        printf("o i: %i, j: %i, k: %i; ", currNode, j, k );
        printf("\n");
        if (new < old) {
          data->optimizedDistances[i][j][k] = new;
          data->routes[j][k] = currNode;
          //printf("new: %f, ", data->optimizedDistances[i][j][k]);
        } else {
          data->optimizedDistances[i][j][k] = old;
          //printf("old: %f, ", data->optimizedDistances[i][j][k]);
        }
      }
    }
  }
}

int findOptimalRouteAux(int a, int b, int last, int* route, struct FloydData *data) {
  if (last == 0){route[last] = a; last++;};
  int intermediateNode = data->routes[a][b];
  if (intermediateNode == -1) {
    route[last] = b; last++;
  } else {
    last = findOptimalRouteAux(a, intermediateNode, last, route, data);
    last = findOptimalRouteAux(intermediateNode, b, last, route, data);
  }
  return last;
}

int* findOptimalRoute(int a, int b, struct FloydData *data) {
  int *tmp = (int*)malloc(sizeof(int) * data->nodeCount);
  int size = findOptimalRouteAux(a, b, 0, tmp, data);
  int *routeOut = (int*)malloc(sizeof(int) * size + 1);
  routeOut[0] = size;
  for(int i = 0; i < size; i++) {
    routeOut[i + 1] = tmp[i];
  }
  return routeOut;
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
