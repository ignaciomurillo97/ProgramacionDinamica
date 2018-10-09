typedef struct resultLine {
  int* reemplazosOptimos;
  double ganancia;
} resultLine;

typedef struct modeloProblema{
  int plazo;
  int vidaUtil;
  double costoInicial;
  double inflacion;
  double* venta;
  double* mantenimiento;
  double* ganancia;
} modeloProblema;

double calcularCosto(int tInicio, int tFinal, struct modeloProblema* problema);
struct resultLine* reemplazosOptimos(struct modeloProblema* problema);
