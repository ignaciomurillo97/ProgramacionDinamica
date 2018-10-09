#include "reemplazo.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//TODO
double calcularCosto(int tInicio, int tFinal, struct modeloProblema* problema) {
  int deltaTiempo = tFinal - tInicio;
  if (deltaTiempo > problema->vidaUtil) {
    return -1;
  }
  double resultado = problema->costoInicial;
  for (int i = 0; i < deltaTiempo; i++) {
    resultado += (problema->mantenimiento[i] - problema->ganancia[i]);
  }
  resultado -= problema->venta[deltaTiempo - 1];
  return resultado;
}

//TODO
resultLine* reemplazosOptimos (modeloProblema* problema) {
  resultLine* resultado = (resultLine*)malloc(sizeof(resultLine) * problema->plazo - 1);

  resultado[problema->plazo].ganancia = 0;

  for (int i = problema->plazo - 1; i >= 0; i--) {
    resultado[i].ganancia = 0;
    for (int j = 0; j < problema->vidaUtil; j++) {
      if (i + j >= problema->plazo) break;
      double current = calcularCosto(i, i + j + 1, problema) + resultado[i + j + 1].ganancia;
      if (current < resultado[i].ganancia) resultado[i].ganancia = current;
    }
  }

  for (int i = 0; i < problema->plazo + 1; i++) {
    printf("%f, ", resultado[i].ganancia);
  }

  return NULL;
}
