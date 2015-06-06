#include "Matrix.h"
#include <vector>
#define COEFICIENTES_POL_CUBICO 4
using namespace std;
class TrazadorCubico
{
	TrazadorCubico(int _k, vector<int>& x_table, vector<int>& y_table) : k(_k)
	{
		coeficientes = new Matrix(x_table.rows()-1, COEFICIENTES_POL_CUBICO);
		
		// Este trazador exige S''(x_0)  = S''(x_n) = 0;

		Matrix c_coef(x_tables.rows()+1, x_tables.rows()+1);

		c_coef(0,0) = 1;
		c_coef(c_coef.rows()-1, c_coef.columns()-1) = 1;

		for (int i = 1; i < c_coef.rows()-1; i++)
		{
			int hi_menos_uno = x_table[i] - x_table[i-1]; 
			int hi = x_table[i+1] - x_table[i];

			c_coef(i,i) = 2* (hi_menos_uno + hi);
			c_coef(i, i-1) = hi_menos_uno;
			c_coef(i,i+1) = hi;
		}

		// TODO: GAUSS
		vector<int> c_sol; // asignarle el de gauss

		
	}

	~TrazadorCubico()
	{
		delete coeficientes;
	}

private:
	Matrix* coeficientes;
	const int k;
};