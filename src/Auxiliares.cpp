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
