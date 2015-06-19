#include "Auxiliares.h"

void loadImage(Matrix& m, const string& s)
{
    fstream file;
    file.open(s, std::ios_base::in);
    string line;
    int i = 0;

    //itero por filas
    while(getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        int j = 0;
        while(getline(iss, token, ','))
        {
            m(i, j) = stoi(token);
            j++;
        }
        
        i++;
    }

    file.close();

}

int norma1(int i, int j, int ki, int kj)
{
    return abs(i - ki) + abs(j - kj);
}

// Vos le tiras la imagen de test como input y el te retorna la matriz reducida, que le tenes que pasar a nuestros algoritmos.
// Ej: Agarro la imagen A de nuestro test, la paso a matriz.
// reducir agarra A, la reduce dado un k correcto (generando A').
// agarras un algoritmo nuestro de zoom y le decis que aumente A', luego comparas el resultado que intento zoomear A' contra A.
// NO OLVIDARSE DE HACER FREEE
Matrix* reducir(const Matrix& input, int k)
{
    // calculo nuevas dimensiones

    // nos basamos en la relacion
    // fila de la aumentada = fila de la original * (k+1)
    // calculo la posicion de la ultima fila, y le aumento en uno asi obtengo el size de la fila
    int f = ((input.columns()-1) / (k+1) ) + 1;
    int c = ((input.rows()-1) / (k+1)) + 1;
    Matrix *small = new Matrix(f, c);

    for (int i = 0; i < input.rows(); i = i + k+1)
    {
        for (int j = 0; j < input.columns(); j = j+k+1)
        {
            // traducimos de la zoomeada a la 'original'
            int f_ = i / (k+1);
            int c_ = j / (k+1);

            (*small)(f_, c_) = input(i,j);
        }
    }

    // Esto ya lo hacen nuestros algoritmos al recibir una matriz, la copian, la agrandan y hacen estas cosas con los ceros.
    // En parte esta funcion se vuelve un poco redundante, pero no va a hacer las cosas mucho mas lentas.
    // Preferible confiar en lo que ya anda, que ir metiendo mano inecesariamente.
    /*ItPixelesNuevos it(output.rows(), output.columns(), k);
    while(!it.end())
    {
        int i = it.get().first;
        int j = it.get().second;

        output(i,j) = 0;
        ++it;
    }*/

    return small;
}
