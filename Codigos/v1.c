/*
Autores: Barreiro Dominguez, Víctor Xesús e Castro Castreje, Helena
Titulo: 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath.h>


typedef struct quaternion{
	double q[4];
}quaternion_t;

#define N 5	

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
		c[i].q[2] = a[i].q[0]*b[i].q[2] - a[i].q[1]*b[i].q[3] + a[i].q[2]*b[i].q[0] + a[i].q[3]*b[i].q[1];
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
		printf("Itersions :%d\n", i);
		dp.q[0] += c[i].q[0]*c[i].q[0] - c[i].q[1]*c[i].q[1] - c[i].q[2]*c[i].q[2] - c[i].q[3]*c[i].q[3];
		dp.q[1] += c[i].q[0]*c[i].q[1] + c[i].q[1]*c[i].q[0] + c[i].q[2]*c[i].q[3] - c[i].q[3]*c[i].q[2];
		dp.q[2] += c[i].q[0]*c[i].q[2] - c[i].q[1]*c[i].q[3] + c[i].q[2]*c[i].q[0] + c[i].q[3]*c[i].q[1];
		dp.q[3] += c[i].q[0]*c[i].q[3] + c[i].q[1]*c[i].q[2] - c[i].q[2]*c[i].q[1] + c[i].q[3]*c[i].q[0];
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
			cuat[j].q[i] = (rand() % 1000000) * 0.00001;
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
	printf(" Cuat: (%.3lf, %.3lf, %.3lf, %.3lf)\n", cuat.q[0], cuat.q[1], cuat.q[2], cuat.q[3]);
}


/*
void prod(double* a, double* b, double* c)
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
}
*/