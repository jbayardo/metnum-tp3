#include "Matrix.h"
#include <vector>
#include <math.h>       /* pow */
#include <iostream>
#define COEFICIENTES_POL_CUBICO 4
using namespace std;
class TrazadorCubico
{
public:
	TrazadorCubico(int _k, vector<int>& x_table, vector<int>& y_table) : k(_k), min_x(x_table[0]), x_values(x_table)
	{
		// TODO: Teniamos problemas para usar bien los constructores de vector
		// TODO: Matrices como esta implementada no es double, y solo es de ints.
		// 		Por eso usamos vector de vectores de double
		// coeficientes(vector<double>(0.0, x_table.size()-1), COEFICIENTES_POL_CUBICO)
		for (int i = 0; i < x_table.size()-1; i++)
		{
			vector<double> vec (COEFICIENTES_POL_CUBICO, 0.0);
			coeficientes.push_back(vec);
		}

		//coeficientes = new Matrix(x_table.size()-1, COEFICIENTES_POL_CUBICO);
		
		// Este trazador exige S''(x_0)  = S''(x_n) = 0;

		vector<double> vec;
		for (int i = 0; i < x_table.size()+1; i++)
			vec.push_back(0.0);

		vector< vector<double> > c_coef;
		for (int i = 0; i < x_table.size()+1; i++)
			c_coef.push_back(vec);


		//Matrix c_coef(x_table.size()+1, x_table.size()+1);

		c_coef[0][0] = 1;
		c_coef[c_coef.size()-1][c_coef.size()-1] = 1;

		for (int i = 1; i < c_coef.size()-1; i++)
		{
			int hi_menos_uno = x_table[i] - x_table[i-1]; 
			int hi = x_table[i+1] - x_table[i];

			c_coef[i][i] = 2* (hi_menos_uno + hi);
			c_coef[i][i-1] = hi_menos_uno;
			c_coef[i][i+1] = hi;
		}

		vector<double> b_sistema(x_table.size()+1, 0);
	
		b_sistema[0] = 0;
		b_sistema[x_table.size()] = 0;

		for (int i = 1; i < b_sistema.size()-1; i++)
		{
			int hi = x_table[i+1] - x_table[i];
			int hi_menos_uno = x_table[i] - x_table[i-1]; 
			b_sistema[i] = 3*((y_table[i+1] - y_table[i])/hi) - 3*((y_table[i]+y_table[i-1])/hi_menos_uno);
		}

		EliminacionGaussiana(c_coef,b_sistema);
		vector<double> c_sol(x_table.size()+1,0);
		BackwardSubstituion(c_coef, b_sistema, c_sol);

		
		for (int i = 0; i < coeficientes.size()-1; i++)
		{
			// el valor del a
			coeficientes[i][0] = y_table[i]; 
			// el valor del c
			coeficientes[i][2] = c_sol[i];
			// el valor del d
			coeficientes[i][3] = (c_sol[i+1] - 2 * c_sol[i]) / (6*(x_table[i+1] - x_table[i]));
			// el valor del b
			coeficientes[i][1] = (y_table[i+1] - y_table[i] - c_sol[i] * pow((x_table[i+1] - x_table[i]),2) - coeficientes[i][3]* pow(x_table[i+1] - x_table[i], 3))/ (x_table[i+1] - x_table[i]);
		}

		for (int i = 0; i < coeficientes.size(); i++)
		{
			for (int j = 0; j < 4; j++)
				cout << coeficientes[i][j] << " ";
			cout << endl;
		}
	}

	//~TrazadorCubico()
	//{
	//}
	// evaluar en numero de columna de imagen aumentada, el normaliza solito.
	// cuiadado al querer evaluar en valores ya conocidos, podria fallar en esos casos.
	int Evaluar(int x);
private:

	// Eliminacion optimizada para casos particulares, como el despeje de c
	void EliminacionGaussiana(vector<vector<double> >& m, vector<double>& b);
	void BackwardSubstituion(const vector< vector< double > >& m, const vector<double>& b, vector<double> sol);
	// cada fila son los coeficientes de un spline
	vector< vector<double> > coeficientes;
	const int k;
	const int min_x;
	const vector<int> x_values; 
};