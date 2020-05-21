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

#define N 1
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
__m256d multiplicacion(__m256d xyzw, __m256d abcd);

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

int main(int argc, char const *argv[]) {
  //  double ck = 0;
  // srand(69);

  /*CUATERNION A*/
  double *a_double = (double *)aligned_alloc(32, 4 * N * sizeof(double));
  for (int i = 0; i < 4 * N; i++) {
    a_double[i] = i + 1;
  }
  __m256d a = _mm256_load_pd(a_double);

  printf("A\n");
  for (int i = 0; i < N; ++i) {
    double *d = (double *)&a;
    printf("DATO: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
  }

  /*CUATERNION B*/
  double *b_double = (double *)aligned_alloc(32, 4 * N * sizeof(double));
  for (int i = 0; i < 4 * N; i++) {
    b_double[i] = 4 * N - i;
  }
  __m256d b = _mm256_load_pd(b_double);

  printf("B\n");
  for (int i = 0; i < N; ++i) {
    double *d = (double *)&b;
    printf("DATO: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
  }

  /*CUATERNION C*/
  __m256d c = multiplicacion(a, b);
  printf("C\n");
  for (int i = 0; i < N; ++i) {
    double *d = (double *)&c;
    printf("DATO: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
  }

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

__m256d multiplicacion(__m256d xyzw, __m256d abcd) {
  __m256d wzxy = _mm256_permute4x64_pd(xyzw, 0b01001011);
  __m256d zwyx = _mm256_permute4x64_pd(xyzw, 0b00011110);
  __m256d abab = _mm256_permute4x64_pd(abcd, 0b01000100);
  __m256d dcdc = _mm256_permute4x64_pd(abcd, 0b10111011);

  __m256d resta =
      _mm256_hsub_pd(_mm256_mul_pd(wzxy, abab), _mm256_mul_pd(zwyx, dcdc));
  __m256d suma =
      _mm256_hadd_pd(_mm256_mul_pd(wzxy, dcdc), _mm256_mul_pd(zwyx, abab));

  __m256d var1 = _mm256_shuffle_pd(resta, suma, 0b0011);
  __m256d final1 = _mm256_permute4x64_pd(var1, 0b11010010);

  __m256d var2 = _mm256_shuffle_pd(resta, suma, 0b1100);
  __m256d final2 = _mm256_permute4x64_pd(var2, 0b00101101);

  return _mm256_addsub_pd(final1, final2);
}
