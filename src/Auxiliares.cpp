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

// Toma la imagen original y una copia de ella (mismo tamaño)
// Modifica la de output y le pone 0 en los lugares que serian los pixeles aumentados para un k dado
void reducir(const Matrix& input, Matrix& output, int k)
{
    ItPixelesNuevos it(output.rows(), output.columns(), k);
    while(!it.end())
    {
        int i = it.get().first;
        int j = it.get().second;

        output(i,j) = 0;
        ++it;
    }
}
