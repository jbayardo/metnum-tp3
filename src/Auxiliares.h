#ifndef METNUM_TP3_AUXILIARES_H
#define METNUM_TP3_AUXILIARES_H

#include "Matrix.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "ItPixelesNuevos.h"

using namespace std;

void loadImage(Matrix& m, const string& s);
int norma1(int i, int j, int ki, int kj);
void reducir(const Matrix& input, Matrix& output, int k);

#endif //METNUM_TP3_AUXILIARES_H