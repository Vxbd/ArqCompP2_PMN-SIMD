

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct quaternion{
	double q[4];
}quaternion_t;

#define N 1

void iniCuatR(quaternion_t* cuat);
void iniCuat(quaternion_t* cuat);
void prod(double* a, double* b, double* c);
void sum(double* a, double* b, double* c);
void opera(double* c, double* dp);
void imprimirCuat(quaternion_t cuat);


int main(int argc, char const *argv[])
{
	quaternion_t a[N], b[N], c[N], dp;

	srand(69);		//Establecemos semente

	//Inicializamos as dúas entradas
	iniCuatR(a);
	iniCuatR(b);


	//Imprimimos los vectores de cuaterniones
	printf("Vector A\n");
	for (int i = 0; i < N; ++i)
	{
		imprimirCuat(a[i]);
	}
	printf("Vector B\n");
	for (int i = 0; i < N; ++i)
	{
		imprimirCuat(b[i]);
	}


	for (int i = 0; i < N; ++i)
	{
		c[i].q[0] = a[i].q[0]*b[i].q[0] - a[i].q[1]*b[i].q[1] - a[i].q[2]*b[i].q[2] - a[i].q[3]*b[i].q[3];
		c[i].q[1] = a[i].q[0]*b[i].q[1] + a[i].q[1]*b[i].q[0] + a[i].q[2]*b[i].q[3] - a[i].q[3]*b[i].q[2];
		c[i].q[2] = a[i].q[0]*b[i].q[2] - a[i].q[1]*b[i].q[3] + a[i].q[2]*b[i].q[1] + a[i].q[3]*b[i].q[1];
		c[i].q[3] = a[i].q[0]*b[i].q[3] + a[i].q[1]*b[i].q[2] - a[i].q[2]*b[i].q[1] + a[i].q[3]*b[i].q[0];
	}

	iniCuat(&dp);

	printf("Vector C\n");
	for (int i = 0; i < N; ++i)
	{
		imprimirCuat(c[i]);
	}

	for (int i = 0; i < N; ++i)
	{
		dp.q[0] += c[i].q[0]*c[i].q[0] - c[i].q[1]*c[i].q[1] - c[i].q[2]*c[i].q[2] - c[i].q[3]*c[i].q[3];
		dp.q[1] += c[i].q[0]*c[i].q[1] + c[i].q[1]*b[i].q[0] + c[i].q[2]*c[i].q[3] - c[i].q[3]*c[i].q[2];
		dp.q[2] += c[i].q[0]*c[i].q[2] - c[i].q[1]*b[i].q[3] + c[i].q[2]*c[i].q[1] + c[i].q[3]*c[i].q[1];
		dp.q[3] += c[i].q[0]*c[i].q[3] + c[i].q[1]*b[i].q[2] - c[i].q[2]*c[i].q[1] + (c[i].q[3])*(c[i].q[0]);
	}

	printf("Cuaterninon\n");
	imprimirCuat(dp);

	return 0;
}


void iniCuatR(quaternion_t* cuat)
{
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			printf("i-> %d\nj-> %d\n", i,j);
			cuat[j].q[i] = rand()/rand();
		}
	}
}

void iniCuat(quaternion_t* cuat)
{
	for (int i = 0; i < 4; ++i)
	{
		cuat->q[i] = 0;
	}
}

void imprimirCuat(quaternion_t cuat)
{
	printf(" Cuat: (%lf, %lf, %lf, %lf)\n", cuat.q[0], cuat.q[1], cuat.q[2], cuat.q[3]);
}


/*void prod(double* a, double* b, double* c)
{

	for (int i = 0; i < N; ++i)
	{
		c[i].q[0] = a[i].q[0]*b[i].q[0] - a[i].q[1]*b[i].q[1] - a[i].q[2]*b[i].q[2] - a[i].q[3]*b[i].q[3];
		c[i].q[1] = a[i].q[0]*b[i].q[1] + a[i].q[1]*b[i].q[0] + a[i].q[2]*b[i].q[3] - a[i].q[3]*b[i].q[2];
		c[i].q[2] = a[i].q[0]*b[i].q[2] - a[i].q[1]*b[i].q[3] + a[i].q[2]*b[i].q[1] + a[i].q[3]*b[i].q[1];
		c[i].q[3] = a[i].q[0]*b[i].q[3] + a[i].q[1]*b[i].q[2] - a[i].q[2]*b[i].q[1] + a[i].q[3]*b[i].q[0];
	}
	
}

void opera(double* c, double* dp)
{

	for (int i = 0; i < N; ++i)
	{
		dp.q[0] += c[i].q[0]*c[i].q[0] - c[i].q[1]*c[i].q[1] - c[i].q[2]*c[i].q[2] - c[i].q[3]*c[i].q[3];
		dp.q[1] += c[i].q[0]*c[i].q[1] + c[i].q[1]*b[i].q[0] + c[i].q[2]*c[i].q[3] - c[i].q[3]*c[i].q[2];
		dp.q[2] += c[i].q[0]*c[i].q[2] - c[i].q[1]*b[i].q[3] + c[i].q[2]*c[i].q[1] + c[i].q[3]*c[i].q[1];
		dp.q[3] += c[i].q[0]*c[i].q[3] + c[i].q[1]*b[i].q[2] - c[i].q[2]*c[i].q[1] + c[i].q[3]*c[i].q[0];
	}
	
}*/