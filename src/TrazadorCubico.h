#include "Matrix.h"
#include <vector>
#include <math.h>       /* pow */
#include <iostream>
#define COEFICIENTES_POL_CUBICO 4
using namespace std;
class TrazadorCubico
{
public:
	TrazadorCubico(int _k, vector<int>& x_table, vector<int>& y_table) : k(_k), min_x(x_table[0]), x_values(x_table), coeficientes(vector<vector<double> >(x_table.size()-1, vector<double>(COEFICIENTES_POL_CUBICO, 0.0)))
	{
		// TODO: Teniamos problemas para usar bien los constructores de vector
		// TODO: Matrices como esta implementada no es double, y solo es de ints.
		// 		Por eso usamos vector de vectores de double
		// coeficientes(vector<double>(0.0, x_table.size()-1), COEFICIENTES_POL_CUBICO)

		//coeficientes: Aca se almacenan los coeficientes de los splines
		int n = x_table.size();

		coeficientes.resize(n+1);

		//coeficientes = new Matrix(x_table.size()-1, COEFICIENTES_POL_CUBICO);
		
		// Este trazador exige S''(x_0)  = S''(x_n) = 0;

		//vec: aca se inicializa un vector con n posiciones
		vector<double> vec;
		vec.resize(n, 0.0);

		//c_coef: se guarda el sistema de ecuaciones para calcular los Cs
		vector< vector<double> > c_coef;
		c_coef.resize(n+1, vec);

		//Matrix c_coef(x_table.size()+1, x_table.size()+1);
		c_coef[0][0] = 1;
		c_coef[n][n] = 1;

		for (int i = 1; i < n; i++)
		{
			int hi_menos_uno = x_table[i] - x_table[i-1]; 
			int hi = x_table[i+1] - x_table[i];

			c_coef[i][i] = 2* (hi_menos_uno + hi);
			c_coef[i][i-1] = hi_menos_uno;
			c_coef[i][i+1] = hi;
		}

		vector<double> b_sistema(n+1, 0);
	
		b_sistema[0] = 0;
		b_sistema[n] = 0;

		for (int i = 1; i < n; i++)
		{
			int hi = x_table[i+1] - x_table[i];
			int hi_menos_uno = x_table[i] - x_table[i-1]; 
			b_sistema[i] = 3*(((y_table[i+1] - y_table[i])/hi) - ((y_table[i] - y_table[i-1])/hi_menos_uno));
		}

		EliminacionGaussiana(c_coef, b_sistema);
		vector<double> c_sol(n + 1, 0);
		BackwardSubstituion(c_coef, b_sistema, c_sol);

		for (int i = 0; i <= n; i++)
		{
			int hi = x_table[i+1] - x_table[i];
			// el valor del a
			coeficientes[i].a = y_table[i]; 
			// el valor del c
			coeficientes[i].c = c_sol[i];
			// el valor del d
			coeficientes[i].d = (c_sol[i+1] - c_sol[i]) / (3*(hi));
			// el valor del b
			coeficientes[i].b = ((y_table[i+1] - y_table[i])/hi)  - (hi/3)*(2*c_sol[i] + c_sol[i+1]);
		}

		for (int i = 0; i < coeficientes.size(); i++)
		{
			for (int j = 0; j < 4; j++){
				cout << "a:" << coeficientes[i].a << " b:" << coeficientes[i].b << " c:" << coeficientes[i].c << " d:" << coeficientes[i].d << endl;
			}
		}
	}

	//~TrazadorCubico()
	//{
	//}
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
		spline(): a(0), b(0), c(0), d(0){}
	};
	// Eliminacion optimizada para casos particulares, como el despeje de c
	void EliminacionGaussiana(vector<vector<double> >& m, vector<double>& b);
	void BackwardSubstituion(const vector< vector< double > >& m, const vector<double>& b, vector<double>& sol);
	// cada fila son los coeficientes de un spline
	vector< spline > coeficientes;
	const int k;
	const int min_x;
	const vector<int> x_values; 
};