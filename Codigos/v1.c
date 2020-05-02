

#include <stdio.h>
#include <stdlib.h>

#define N 1

void iniCuatR(double* cuat);
void iniCuat(double* cuat);
void prod(double* a, double* b, double* c);
void sum(double* a, double* b, double* c);
void opera(double* c, double* dp);

struct quaternion {
	double el1;
	double el2;
	double el3;
	double el4;
};
typedef struct quaternion quaternion;

int main(int argc, char const *argv[])
{
	quaternion *a, *b, *c, *dp;

	a = (quaternion *)aligned_alloc(sizeof(quaternion), N * sizeof(quaternion));	//Entrada 1
	b = (quaternion *)aligned_alloc(sizeof(quaternion), N * sizeof(quaternion));	//Entrada 2
	c = (quaternion *)aligned_alloc(sizeof(quaternion), N * sizeof(quaternion));	//Auxiliar
	dp = (quaternion *)aligned_alloc(sizeof(quaternion), sizeof(quaternion));

	srand(321);		//Establecemos semente

	//Inicializamos as dúas entradas
	for (int i = 0; i < N; ++i)
	{
		//iniCuatR(a[i]);
		//iniCuatR(b[i]);
	}

	for (int i = 0; i < N; ++i)
	{
		c[i].el1 = a[i].el1*b[i].el1 - a[i].el2*b[i].el2 - a[i].el3*b[i].el3 - a[i].el4*b[i].el4;
		c[i].el2 = a[i].el1*b[i].el2 + a[i].el2*b[i].el1 + a[i].el3*b[i].el4 - a[i].el4*b[i].el3;
		c[i].el3 = a[i].el1*b[i].el3 - a[i].el2*b[i].el4 + a[i].el3*b[i].el2 + a[i].el4*b[i].el2;
		c[i].el4 = a[i].el1*b[i].el4 + a[i].el2*b[i].el3 - a[i].el3*b[i].el2 + a[i].el4*b[i].el1;
	}

	//iniCuat(dp);

	for (int i = 0; i < N; ++i)
	{
		dp[i].el1 += c[i].el1*c[i].el1 - c[i].el2*c[i].el2 - c[i].el3*c[i].el3 - c[i].el4*c[i].el4;
		dp[i].el2 += c[i].el1*c[i].el2 + c[i].el2*b[i].el1 + c[i].el3*c[i].el4 - c[i].el4*c[i].el3;
		dp[i].el3 += c[i].el1*c[i].el3 - c[i].el2*b[i].el4 + c[i].el3*c[i].el2 + c[i].el4*c[i].el2;
		dp[i].el4 += c[i].el1*c[i].el4 + c[i].el2*b[i].el3 - c[i].el3*c[i].el2 + c[i].el4*c[i].el1;
	}


	return 0;
}

void iniCuatR(double* cuat)
{
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			cuat[i] = rand()/rand();
		}
	}
	
}

void iniCuat(double* cuat)
{
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			cuat[i] = 0;
		}
	}
	
}

void prod(double* a, double* b, double* c)
{

	for (int i = 0; i < N; ++i)
	{
		c[i].el1 = a[i].el1*b[i].el1 - a[i].el2*b[i].el2 - a[i].el3*b[i].el3 - a[i].el4*b[i].el4;
		c[i].el2 = a[i].el1*b[i].el2 + a[i].el2*b[i].el1 + a[i].el3*b[i].el4 - a[i].el4*b[i].el3;
		c[i].el3 = a[i].el1*b[i].el3 - a[i].el2*b[i].el4 + a[i].el3*b[i].el2 + a[i].el4*b[i].el2;
		c[i].el4 = a[i].el1*b[i].el4 + a[i].el2*b[i].el3 - a[i].el3*b[i].el2 + a[i].el4*b[i].el1;
	}
	
}

void opera(double* c, double* dp)
{

	for (int i = 0; i < N; ++i)
	{
		dp[i].el1 += c[i].el1*c[i].el1 - c[i].el2*c[i].el2 - c[i].el3*c[i].el3 - c[i].el4*c[i].el4;
		dp[i].el2 += c[i].el1*c[i].el2 + c[i].el2*b[i].el1 + c[i].el3*c[i].el4 - c[i].el4*c[i].el3;
		dp[i].el3 += c[i].el1*c[i].el3 - c[i].el2*b[i].el4 + c[i].el3*c[i].el2 + c[i].el4*c[i].el2;
		dp[i].el4 += c[i].el1*c[i].el4 + c[i].el2*b[i].el3 - c[i].el3*c[i].el2 + c[i].el4*c[i].el1;
	}
	
}