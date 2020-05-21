/*
Autores: Barreiro Dominguez, Víctor Xesús e Castro Castreje, Helena
Titulo:-
-mavx
*/

#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct quaternion {
  double q[4];
} quaternion_t;

#define CLS 64

void iniCuatR(quaternion_t *cuat);
void iniCuat(quaternion_t *cuat);
void prod(double *a, double *b, double *c);
void sum(double *a, double *b, double *c);
void opera(double *c, double *dp);
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

  /*CUATERNION A*/
  /*  double *a_double = (double *)aligned_alloc(32, 4 * N * sizeof(double));
    for (int i = 0; i < 4 * N; i++) {
      a_double[i] = i + 1;
    }
    __m256d a = _mm256_load_pd(a[i]);

    printf("A\n");
    for (int i = 0; i < N; ++i) {
      double *d = (double *)&a;
      printf("DATO: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    }


    double *b_double = (double *)aligned_alloc(32, 4 * N * sizeof(double));
    for (int i = 0; i < 4 * N; i++) {
      b_double[i] = 4 * N - i;
    }
    __m256d b = _mm256_load_pd(b_double);

    printf("B\n");
    for (int i = 0; i < N; ++i) {
      double *d = (double *)&b;
      printf("DATO: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    }*/
  iniCuat(dp);

  /*CUATERNION C*/
  __m256d avec, bvec, cvec, dpvec;
  printf("C\n");
  start_counter();
  for (int i = 0; i < N; i++) {
    avec = _mm256_setr_pd(a[i].q[0], a[i].q[1], a[i].q[2], a[i].q[3]);
    bvec = _mm256_setr_pd(b[i].q[0], b[i].q[1], b[i].q[2], b[i].q[3]);

    __m256d a1vec = _mm256_permute4x64_pd(avec, 0b01001011);
    __m256d a2vec = _mm256_permute4x64_pd(avec, 0b00011110);
    __m256d b1vec = _mm256_permute4x64_pd(bvec, 0b01000100);
    __m256d b2vec = _mm256_permute4x64_pd(bvec, 0b10111011);

    __m256d resta = _mm256_hsub_pd(_mm256_mul_pd(a1vec, b1vec),
                                   _mm256_mul_pd(a2vec, b2vec));
    __m256d suma = _mm256_hadd_pd(_mm256_mul_pd(a1vec, b2vec),
                                  _mm256_mul_pd(a2vec, b1vec));

    __m256d var1 = _mm256_shuffle_pd(resta, suma, 0b0011);
    __m256d final1 = _mm256_permute4x64_pd(var1, 0b11010010);

    __m256d var2 = _mm256_shuffle_pd(resta, suma, 0b1100);
    __m256d final2 = _mm256_permute4x64_pd(var2, 0b00101101);

    cvec = _mm256_addsub_pd(final1, final2); // esto seria c

    __m256d c1vec = _mm256_permute4x64_pd(cvec, 0b01001011);
    __m256d c2vec = _mm256_permute4x64_pd(cvec, 0b00011110);
    __m256d c3vec = _mm256_permute4x64_pd(cvec, 0b01000100);
    __m256d c4vec = _mm256_permute4x64_pd(cvec, 0b10111011);

    __m256d cresta = _mm256_hsub_pd(_mm256_mul_pd(c1vec, c3vec),
                                    _mm256_mul_pd(c2vec, c4vec));
    __m256d csuma = _mm256_hadd_pd(_mm256_mul_pd(c1vec, c4vec),
                                   _mm256_mul_pd(c2vec, c3vec));

    __m256d cvar1 = _mm256_shuffle_pd(cresta, csuma, 0b0011);
    __m256d cfinal1 = _mm256_permute4x64_pd(cvar1, 0b11010010);

    __m256d cvar2 = _mm256_shuffle_pd(cresta, csuma, 0b1100);
    __m256d cfinal2 = _mm256_permute4x64_pd(cvar2, 0b00101101);

    dpvec = _mm256_addsub_pd(cfinal1, cfinal2); // esto seria dp

    double *k = (double *)&dpvec;
    dp->q[0] += k[0];
    dp->q[1] += k[1];
    dp->q[2] += k[2];
    dp->q[3] += k[3];
  }

  ck = get_counter();
  /*-----------Fin codigo a medir-----------*/

  printf("\n Clocks=%1.10lf\n", ck);

  /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas
   * start_counter/get_counter */
  mhz(1, 1);

  printf("Cuaterninon\n");
  imprimirCuat(*dp);

  printf("%d,%1.10lf,\n", q, ck / N);
  fprintf(fichero, "%d,%1.10lf,\n", q, ck / N);

  fclose(fichero);

  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
  _mm_free(dp);

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
