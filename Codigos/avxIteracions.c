/*
Autores: Barreiro Dominguez, Víctor Xesús e Castro Castreje, Helena
Titulo:

*/

#include <immintrin.h>
#include <math.h>
#include <mm_malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct quaternion {
  double q[4];
} quaternion_t;

//#define N 1
#define CLS 64

void iniCuatR(quaternion_t *cuat);
void iniCuat(quaternion_t *cuat);
void prod(double *a, double *b, double *c);
void sum(double *a, double *b, double *c);
void opera(double *c, double **dp);
void imprimirCuat(quaternion_t cuat);
void start_counter();
double get_counter();
double mhz();

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

int N;

/*MAIN*/
int main(int argc, char const *argv[]) {
  double ck = 0;
  quaternion_t *a, *b, *c, *dp;

  int q = 7;

  if (argc == 2) {
    q = atoi(argv[1]);
  }

  N = pow(10, q);

  FILE *fichero;
  fichero = fopen("opt.csv", "a+");

  a = (quaternion_t *)_mm_malloc(sizeof(quaternion_t) * N, CLS);
  b = (quaternion_t *)_mm_malloc(sizeof(quaternion_t) * N, CLS);
  c = (quaternion_t *)_mm_malloc(sizeof(quaternion_t) * N, CLS);
  dp = (quaternion_t *)_mm_malloc(sizeof(quaternion_t), CLS);

  int i;     // Variable de bucles
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
    // imprimirCuat(a[i]);
  }

  // TODO: deberiamos crear vectores "vacios" (de 0) hasta que N%4 == 0
  printf("Vector B\n");
  for (int i = 0; i < N; ++i) {
    // imprimirCuat(b[i]);
  }

  iniCuat(dp);

  /*Creo cuatro vectores, uno para cada termino*/
  __m256d primeroA, segundoA, terceroA, cuartoA;
  __m256d primeroB, segundoB, terceroB, cuartoB;
  __m256d primeroC, segundoC, terceroC, cuartoC;
  __m256d primeroDP, segundoDP, terceroDP, cuartoDP;
  __m256d dos = _mm256_set1_pd(2);
  double *dp0 = (double *)_mm_malloc(sizeof(quaternion_t), CLS);
  double *dp1 = (double *)_mm_malloc(sizeof(quaternion_t), CLS);
  double *dp2 = (double *)_mm_malloc(sizeof(quaternion_t), CLS);
  double *dp3 = (double *)_mm_malloc(sizeof(quaternion_t), CLS);

  start_counter();
  /*---------Inicio codigo a medir---------*/
  for (i = 0; i < N; i += 4) {

    // Componentes de los primeros 4 cuaterniones de a
    primeroA =
        _mm256_setr_pd(a[i].q[0], a[i + 1].q[0], a[i + 2].q[0], a[i + 3].q[0]);
    segundoA =
        _mm256_setr_pd(a[i].q[1], a[i + 1].q[1], a[i + 2].q[1], a[i + 3].q[1]);
    terceroA =
        _mm256_setr_pd(a[i].q[2], a[i + 1].q[2], a[i + 2].q[2], a[i + 3].q[2]);
    cuartoA =
        _mm256_setr_pd(a[i].q[3], a[i + 1].q[3], a[i + 2].q[3], a[i + 3].q[3]);

    // Componentes de los primeros 4 cuaterniones de b
    primeroB =
        _mm256_setr_pd(b[i].q[0], b[i + 1].q[0], b[i + 2].q[0], b[i + 3].q[0]);
    segundoB =
        _mm256_setr_pd(b[i].q[1], b[i + 1].q[1], b[i + 2].q[1], b[i + 3].q[1]);
    terceroB =
        _mm256_setr_pd(b[i].q[2], b[i + 1].q[2], b[i + 2].q[2], b[i + 3].q[2]);
    cuartoB =
        _mm256_setr_pd(b[i].q[3], b[i + 1].q[3], a[i + 2].q[3], b[i + 3].q[3]);

    // Calculamos c
    primeroC = _mm256_sub_pd(
        _mm256_fmsub_pd(primeroA, primeroB, _mm256_mul_pd(segundoA, segundoB)),
        _mm256_fmsub_pd(terceroA, terceroB, _mm256_mul_pd(cuartoA, cuartoB)));
    segundoC = _mm256_add_pd(
        _mm256_fmadd_pd(primeroA, segundoB, _mm256_mul_pd(segundoA, primeroA)),
        _mm256_fmsub_pd(terceroA, cuartoB, _mm256_mul_pd(cuartoA, terceroA)));
    terceroC = _mm256_add_pd(
        _mm256_fmsub_pd(primeroA, terceroB, _mm256_mul_pd(segundoA, cuartoB)),
        _mm256_fmadd_pd(terceroA, primeroB, _mm256_mul_pd(cuartoA, terceroB)));
    cuartoC = _mm256_sub_pd(
        _mm256_fmadd_pd(primeroA, cuartoB, _mm256_mul_pd(segundoA, terceroB)),
        _mm256_fmadd_pd(terceroA, segundoB, _mm256_mul_pd(cuartoA, primeroB)));

    // Calculamos dp
    primeroDP = _mm256_sub_pd(
        _mm256_fmsub_pd(primeroC, primeroC, _mm256_mul_pd(segundoC, segundoC)),
        _mm256_fmsub_pd(terceroC, terceroC, _mm256_mul_pd(cuartoC, cuartoC)));
    segundoDP = _mm256_mul_pd(_mm256_mul_pd(primeroC, segundoC), dos);
    terceroDP = _mm256_mul_pd(_mm256_mul_pd(primeroC, terceroC), dos);
    cuartoDP = _mm256_mul_pd(_mm256_mul_pd(primeroC, cuartoC), dos);

    // Calculamos la acumulacion
    dp0 = &primeroDP;
    dp1 = &segundoDP;
    dp2 = &terceroDP;
    dp3 = &cuartoDP;

    for (int j = 0; j < 4; j++) {
      dp->q[0] += dp0[0 + j];
      dp->q[1] += dp1[0 + j];
      dp->q[2] += dp2[0 + j];
      dp->q[3] += dp3[0 + j];
    }
  }

  ck = get_counter();
  /*-----------Fin codigo a medir-----------*/

  printf("\n Clocks=%1.10lf\n", ck);

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas
   * start_counter/get_counter */
  mhz(1, 1);

  printf("Cuaterninon\n");
  imprimirCuat(*dp);

  printf("%d,%1.10lf,\n", q, ck);
  fprintf(fichero, "%d,%1.10lf,\n", q, ck);

  fclose(fichero);

  free(a);
  free(b);
  free(c);
  free(dp);

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
