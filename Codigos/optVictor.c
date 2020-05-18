/*
Autores: Barreiro Dominguez, Víctor Xesús e Castro Castreje, Helena
Titulo:

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct quaternion {
  double q[4];
} quaternion_t;

#define N 5
#define HILOS 4

void iniCuatR(quaternion_t *cuat);
void iniCuat(quaternion_t *cuat);
void prod(double *a, double *b, double *c);
void sum(double *a, double *b, double *c);
void opera(double *c, double *dp);
void imprimirCuat(quaternion_t cuat);
void start_counter();
double get_counter();
double mhz();

//Version Víctor
void *opera1(quaternion_t* a, quaternion_t* b, quaternion_t* c, int inicio, int n);
void calculoDeHilos(int hilos, int n, int* res);


/* Initialize the cycle counter */
static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

/* Set *hi and *lo to the high and low order bits of the cycle counter.
Implementation requires assembly code to use the rdtsc instruction. */
void access_counter(unsigned *hi, unsigned *lo) {
  asm("rdtsc; movl %%edx,%0; movl %%eax,%1" /* Read cycle counter */
      : "=r"(*hi), "=r"(*lo)                /* and move results to */
      : /* No input */                      /* the two outputs */
      : "%edx", "%eax");
}

/* Record the current value of the cycle counter. */
void start_counter() { access_counter(&cyc_hi, &cyc_lo); }

/* Return the number of cycles since the last call to start_counter. */
double get_counter() {
  unsigned ncyc_hi, ncyc_lo;
  unsigned hi, lo, borrow;
  double result;

  /* Get cycle counter */
  access_counter(&ncyc_hi, &ncyc_lo);

  /* Do double precision subtraction */
  lo = ncyc_lo - cyc_lo;
  borrow = lo > ncyc_lo;
  hi = ncyc_hi - cyc_hi - borrow;
  result = (double)hi * (1 << 30) * 4 + lo;
  if (result < 0) {
    fprintf(stderr, "Error: counter returns neg value: %.0f\n", result);
  }
  return result;
}

double mhz(int verbose, int sleeptime) {
  double rate;

  start_counter();
  sleep(sleeptime);
  rate = get_counter() / (1e6 * sleeptime);
  if (verbose)
    printf("\n Processor clock rate = %.1f MHz\n", rate);
  return rate;
}

quaternion_t a[N], b[N], c[N], dp;

int main(int argc, char const *argv[]) {
  double ck = 0;
  
  int param[2];
  int i = 0;
  pthread_t hilos[HILOS];


  srand(69); // Establecemos semente

  // Inicializamos as dúas entradas
  iniCuatR(a);
  iniCuatR(b);

  /*
  a[0].q[0] = 1;
  a[0].q[1] = 2;
  a[0].q[2] = 3;
  a[0].q[3] = 4;

  b[0].q[0] = 4;
  b[0].q[1] = 3;
  b[0].q[2] = 2;
  b[0].q[3] = 1;
  */

  // Imprimimos los vectores de cuaterniones
  printf("Vector A\n");
  for (int i = 0; i < N; ++i) {
    imprimirCuat(a[i]);
  }
  printf("Vector B\n");
  for (int i = 0; i < N; ++i) {
    imprimirCuat(b[i]);
  }
  start_counter();
  /*---------Inicio codigo a medir---------*/
  /*
  for (i = 0; i < N; ++i) {
    c[i].q[0] = a[i].q[0] * b[i].q[0] - a[i].q[1] * b[i].q[1] -
                a[i].q[2] * b[i].q[2] - a[i].q[3] * b[i].q[3];
    c[i].q[1] = a[i].q[0] * b[i].q[1] + a[i].q[1] * b[i].q[0] +
                a[i].q[2] * b[i].q[3] - a[i].q[3] * b[i].q[2];
    c[i].q[2] = a[i].q[0] * b[i].q[2] - a[i].q[1] * b[i].q[3] +
                a[i].q[2] * b[i].q[0] + a[i].q[3] * b[i].q[1];
    c[i].q[3] = a[i].q[0] * b[i].q[3] + a[i].q[1] * b[i].q[2] -
                a[i].q[2] * b[i].q[1] + a[i].q[3] * b[i].q[0];
  }*/

  calculoDeHilos(HILOS, N, param);
  opera1(a, b, c, 0, N);



  iniCuat(&dp);

  printf("Vector C\n");
  for (i = 0; i < N; ++i) {
    imprimirCuat(c[i]);
  }

  for (i = 0; i < N; ++i) {
    printf("Itersions :%d\n", i);
    dp.q[0] += c[i].q[0] * c[i].q[0] - c[i].q[1] * c[i].q[1] -
               c[i].q[2] * c[i].q[2] - c[i].q[3] * c[i].q[3];
    dp.q[1] += c[i].q[0] * c[i].q[1] + c[i].q[1] * c[i].q[0] +
               c[i].q[2] * c[i].q[3] - c[i].q[3] * c[i].q[2];
    dp.q[2] += c[i].q[0] * c[i].q[2] - c[i].q[1] * c[i].q[3] +
               c[i].q[2] * c[i].q[0] + c[i].q[3] * c[i].q[1];
    dp.q[3] += c[i].q[0] * c[i].q[3] + c[i].q[1] * c[i].q[2] -
               c[i].q[2] * c[i].q[1] + c[i].q[3] * c[i].q[0];
  }

  ck = get_counter();
  /*-----------Fin codigo a medir-----------*/

  printf("\n Clocks=%1.10lf\n", ck);

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas
   * start_counter/get_counter */
  mhz(1, 1);

  printf("Cuaterninon\n");
  imprimirCuat(dp);

  return 0;
}

void iniCuatR(quaternion_t *cuat) {
  for (int j = 0; j < N; ++j) {
    for (int i = 0; i < 4; ++i) {
      cuat[j].q[i] = (rand() % 1000000) * 0.00001;
    }
  }
}

void iniCuat(quaternion_t *cuat) {
  for (int i = 0; i < 4; ++i) {
    cuat->q[i] = 0;
  }
}

void imprimirCuat(quaternion_t cuat) {
  printf(" Cuat: (%.3lf, %.3lf, %.3lf, %.3lf)\n", cuat.q[0], cuat.q[1],
         cuat.q[2], cuat.q[3]);
}

/*
Parámetros:
  - hilos := Número de hilos del sistema
  - n := Número de componentes
  - res := vector de 2 enteros para almacenar el resultado
Salida: 
  res devolverá en la primera posición el valor medio de iteraciones a cubrir por 
  hilo y en el segundo el valor que deberá cubrir el último de los hilos si es 
  necesario que sea distinto.

*/
void calculoDeHilos(int hilos, int n, int* res)
{
    int hilosfunc = min(hilos, n);

    res[0] = floor(n/hilosfunc);

    res[1] = n - (hilosfunc - 1)*res[0];
}


void *opera1(quaternion_t* a, quaternion_t* b, quaternion_t* c, int inicio, int n)
{
  for (int i = inicio; i < n; ++i) {
    c[i].q[0] = a[i].q[0] * b[i].q[0] - a[i].q[1] * b[i].q[1] -
                a[i].q[2] * b[i].q[2] - a[i].q[3] * b[i].q[3];
    c[i].q[1] = a[i].q[0] * b[i].q[1] + a[i].q[1] * b[i].q[0] +
                a[i].q[2] * b[i].q[3] - a[i].q[3] * b[i].q[2];
    c[i].q[2] = a[i].q[0] * b[i].q[2] - a[i].q[1] * b[i].q[3] +
                a[i].q[2] * b[i].q[0] + a[i].q[3] * b[i].q[1];
    c[i].q[3] = a[i].q[0] * b[i].q[3] + a[i].q[1] * b[i].q[2] -
                a[i].q[2] * b[i].q[1] + a[i].q[3] * b[i].q[0];
  }
}
