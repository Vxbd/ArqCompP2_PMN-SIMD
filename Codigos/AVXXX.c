/*
Autores: Barreiro Dominguez, Víctor Xesús e Castro Castreje, Helena
Titulo:-


-mavx
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <immintrin.h>

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
__m256d __mm_cross4_pd(__m256d xyzw, __m256d abcd);

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
  double ck = 0;
  //quaternion_t a[N], b[N], c[N], dp;

  __m256d* a = _mm_malloc(sizeof(__m256d)*N, CLS);
  __m256d* b = _mm_malloc(sizeof(__m256d)*N, CLS);
  __m256d* c = _mm_malloc(sizeof(__m256d)*N, CLS);  

  srand(69);

  //(rand() % 1000000) * 0.00001,

  for (int i = 0; i < N; ++i)
  {
    b[i] = _mm256_set_pd(4,3,2,1);
  }
  for (int i = 0; i < N; ++i)
  {
    a[i] = _mm256_set_pd(1,2,3,4);
  }

  for (int i = 0; i < N; ++i)
  {
    c[i]= __mm_cross4_pd(a[i], b[i]);
  }

  printf("A\n");
  for (int i = 0; i < N; ++i)
  {
    double* d= (double*) &a[i];
    printf("DATO: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
  }

  printf("B\n");
  for (int i = 0; i < N; ++i)
  {
    double* d= (double*) &b[i];
    printf("DATO: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
  }

  printf("C\n");
  for (int i = 0; i < N; ++i)
  {
    double* d= (double*) &c[i];
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

/*
void prod(double* a, double* b, double* c)
{
        for (int i = 0; i < N; ++i)
        {
                c[i].q[0] = a[i].q[0]*b[i].q[0] - a[i].q[1]*b[i].q[1] -
a[i].q[2]*b[i].q[2] - a[i].q[3]*b[i].q[3]; c[i].q[1] = a[i].q[0]*b[i].q[1] +
a[i].q[1]*b[i].q[0] + a[i].q[2]*b[i].q[3] - a[i].q[3]*b[i].q[2]; c[i].q[2] =
a[i].q[0]*b[i].q[2] - a[i].q[1]*b[i].q[3] + a[i].q[2]*b[i].q[1] +
a[i].q[3]*b[i].q[1]; c[i].q[3] = a[i].q[0]*b[i].q[3] + a[i].q[1]*b[i].q[2] -
a[i].q[2]*b[i].q[1] + a[i].q[3]*b[i].q[0];
        }
}

void opera(double* c, double* dp)
{
        for (int i = 0; i < N; ++i)
        {
                dp.q[0] += c[i].q[0]*c[i].q[0] - c[i].q[1]*c[i].q[1] -
c[i].q[2]*c[i].q[2] - c[i].q[3]*c[i].q[3]; dp.q[1] += c[i].q[0]*c[i].q[1] +
c[i].q[1]*b[i].q[0] + c[i].q[2]*c[i].q[3] - c[i].q[3]*c[i].q[2]; dp.q[2] +=
c[i].q[0]*c[i].q[2] - c[i].q[1]*b[i].q[3] + c[i].q[2]*c[i].q[1] +
c[i].q[3]*c[i].q[1]; dp.q[3] += c[i].q[0]*c[i].q[3] + c[i].q[1]*b[i].q[2] -
c[i].q[2]*c[i].q[1] + c[i].q[3]*c[i].q[0];
        }
}
*/

__m256d __mm_cross4_pd(__m256d xyzw, __m256d abcd){

  __m256d wzyx = _mm256_shuffle_pd(xyzw,xyzw,_MM_SHUFFLE(0,1,2,3));
  __m256d baba =_mm256_shuffle_pd(abcd,abcd,_MM_SHUFFLE(0,1,0,1));
  __m256d dcdc =_mm256_shuffle_pd(abcd,abcd,_MM_SHUFFLE(2,3,2,3));

  __m256d ZnXWY = _mm256_hsub_pd(_mm256_mul_pd(xyzw, baba), _mm256_mul_pd(wzyx, dcdc));

  __m256d XZYnW = _mm256_hadd_pd(_mm256_mul_pd(xyzw, dcdc), _mm256_mul_pd(wzyx, baba));

  __m256d XZWY = _mm256_addsub_pd(_mm256_shuffle_pd(XZYnW, ZnXWY, _MM_SHUFFLE(3,2,1,0)),
    _mm256_shuffle_pd(ZnXWY, XZYnW, _MM_SHUFFLE(2,3,0,1)));

  return _mm256_shuffle_pd(XZWY, XZWY, _MM_SHUFFLE(2,1,3,0));

}