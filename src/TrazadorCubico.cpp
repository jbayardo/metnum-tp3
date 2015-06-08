#include "TrazadorCubico.h"
void TrazadorCubico::EliminacionGaussiana(vector< vector < double > >& m, vector<double>& b)
{
	for (int i = 0; i < m.size()-1; i++)
	{
		if (fabs(m[i+1][i+1]) < 0.001)
			std::cout << "SALTO" << std::endl;
		double coef = m[i+1][i] / ((double)m[i][i]);
		m[i+1][i] = 0.0;
		//m[i+1][i+1] = m[i+1][i+1] - (double)(m[i][i+1]) * coef;
		m[i+1][i+1] -= (m[i][i+1]*m[i+1][i])/((double)(m[i][i]));
		b[i+1] = b[i+1] - b[i]*coef;
	}
}
void TrazadorCubico::BackwardSubstituion(const vector< vector < double > >& m, const vector<double>& b, vector<double>& sol)
{
	sol[0] = 0;
	sol[m.size()-1] = 0;

	for (int i = m.size()-2; i >= 1; i--)
	{
		double res = (b[i] - m[i][i+1]*sol[i+1])/((double)m[i][i]);
		sol[i]=res;
	} 
}

int TrazadorCubico::Evaluar(int x)
{
	// normalizo el parametro para saber el spline
	//si evaluo en un punto conocido, me devuelve el segundo spline en el que aparece
	//para el ultimo punto, va a dar un indice fuera de rango
	int i = (x - x_values[0]) / (k+1);

	int _x = (x - x_values[i]);

	int res = coeficientes[i].a + coeficientes[i].b * _x + coeficientes[i].c * pow(_x, 2) + coeficientes[i].d * pow(_x, 3);

	return res;
}
void TrazadorCubico::spline::Imprimir()
{
	std::cout << "a: " << a << " b: " << b << " c: " << c << " d: " << d << endl;
}