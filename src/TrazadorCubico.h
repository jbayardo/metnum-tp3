#include "Matrix.h"
#include <vector>
#include <math.h>       /* pow */
#include <iostream>
#define COEFICIENTES_POL_CUBICO 4
using namespace std;
class TrazadorCubico
{
public:
	TrazadorCubico(int _k, vector<int>& x_table, vector<int>& y_table) : k(_k), min_x(x_table[0]), x_values(x_table), coeficientes(vector<spline>(x_table.size()-1, spline()))
	{
		// TODO: Matrices como esta implementada no es double, y solo es de ints.
		// 		Por eso usamos vector de vectores de double

		vector< vector<double> > c_coef(x_table.size(), vector<double>(x_table.size(), 0.0));

		c_coef[0][0] = 1.0;
		c_coef[c_coef.size()-1][c_coef.size()-1] = 1.0;

		for (int i = 1; i < c_coef.size()-1; i++)
		{
			double hi_menos_uno = x_table[i] - x_table[i-1]; 
			double hi = x_table[i+1] - x_table[i];

			c_coef[i][i] = 2.0* (hi_menos_uno + hi);
			c_coef[i][i-1] = hi_menos_uno;
			c_coef[i][i+1] = hi;
		}

		vector<double> b_sistema(x_table.size(), 0.0);
	
		b_sistema[0] = 0.0;
		b_sistema[b_sistema.size()-1] = 0.0;

		for (int i = 1; i < b_sistema.size()-1; i++)
		{
			double hi = x_table[i+1] - x_table[i];
			double hi_menos_uno = x_table[i] - x_table[i-1]; 
			b_sistema[i] = (3.0*y_table[i+1] - 3.0*y_table[i])/hi - (3.0*y_table[i]-3.0*y_table[i-1])/hi_menos_uno;

		}

		EliminacionGaussiana(c_coef,b_sistema);
		vector<double> c_sol(x_table.size(),0.0);
		BackwardSubstituion(c_coef, b_sistema, c_sol);

		for (int i = 0; i < coeficientes.size(); i++)
		{
			double hi =(x_table[i+1] - x_table[i]);
			// el valor del a
			coeficientes[i].a = y_table[i]; 
			// el valor del c
			coeficientes[i].c = c_sol[i];
			// el valor del d
			coeficientes[i].d = (c_sol[i+1]-c_sol[i])/(3.0*hi);
			// el valor del b
			coeficientes[i].b = ((y_table[i+1] - y_table[i])/hi)  - (hi*(2*c_sol[i] + c_sol[i+1]))/3.0;
		}

		// DEBUG
		for (int i = 0; i < coeficientes.size(); i++)
			coeficientes[i].Imprimir();
	}

	// evaluar en numero de columna de imagen aumentada, el normaliza solito.
	// cuiadado al querer evaluar en valores ya conocidos, podria fallar en esos casos.
	int Evaluar(int x);
private:
	struct spline
	{
		double a;
		double b;
		double c;
		double d;
		spline(): a(0.0), b(0.0), c(0.0), d(0.0){}

		void Imprimir();
	};

	// Eliminacion optimizada para casos particulares, como el despeje de c
	void EliminacionGaussiana(vector< vector<double> >& m, vector<double>& b);
	void BackwardSubstituion(const vector< vector<double> >& m, const vector<double>& b, vector<double>& sol);
	// cada fila son los coeficientes de un spline
	vector<spline> coeficientes;
	const int k;
	const int min_x;
	const vector<int> x_values; 
};