#include "reemplazo.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double myPow(double value, int power) {
  double result = 1;
  for (int i = 0; i < power; i++) {
    result *= value;
  }
  return result;
}

//TODO
double calcularCosto(int tInicio, int tFinal, struct modeloProblema* problema) {
  int deltaTiempo = tFinal - tInicio;
  if (deltaTiempo > problema->vidaUtil) {
    return -1;
  }
  double resultado = problema->costoInicial * myPow(problema->inflacion, tInicio);
  for (int i = 0; i < deltaTiempo; i++) {
    resultado += (problema->mantenimiento[i] - problema->ganancia[i]) * problema->inflacion;
  }
  resultado -= problema->venta[deltaTiempo - 1] * myPow(problema->inflacion, tFinal);
  return resultado;
}

//TODO
resultLine** reemplazosOptimos (modeloProblema* problema) {
  resultLine** resultado = (resultLine**)malloc(sizeof(resultLine*) * problema->plazo - 1);

  for (int i = 0; i <= problema->plazo; i++)
    resultado[i] = (resultLine*)malloc(sizeof(resultLine));
  resultado[problema->plazo]->ganancia = 0;

  for (int i = problema->plazo - 1; i >= 0; i--) {
    resultado[i]->ganancia = INFINITY;
    int optimalSolutions = 0;
    for (int j = 0; j < problema->vidaUtil; j++) {
      if (i + j >= problema->plazo) break;
      double current = calcularCosto(i, i + j + 1, problema) + resultado[i + j + 1]->ganancia;
      if (current < resultado[i]->ganancia){
        resultado[i]->ganancia = current;
        optimalSolutions = 1;
      } else if (current == resultado[i]->ganancia) {
        optimalSolutions++;
      }
    }
    int ultimo = 1;
    resultado[i]->reemplazosOptimos = (int*)malloc(sizeof(int) * optimalSolutions + 1);
    resultado[i]->reemplazosOptimos[0] = optimalSolutions;
    for (int j = 0; j < problema->vidaUtil; j++) {
      if (i + j >= problema->plazo) break;
      double current = calcularCosto(i, i + j + 1, problema) + resultado[i + j + 1]->ganancia;
      if (current == resultado[i]->ganancia){
        resultado[i]->reemplazosOptimos[ultimo] = j + i + 1;
        ultimo ++;
      } 
    }
  }

  for (int i = 0; i < problema->plazo; i++) {
    printf("%f: ", resultado[i]->ganancia);
    for (int j = 1; j <= resultado[i]->reemplazosOptimos[0]; j++) {
      printf("%d, ", resultado[i]->reemplazosOptimos[j]);
    }
    printf("\n");
  }
  
  return resultado;
}
