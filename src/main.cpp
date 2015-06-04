
#include "ItPixelesNuevos.h"
//#include "Matrix.h"
#include "Auxiliares.h"


void calculoFrecuencia(const Matrix& m, int* frecuencias)
{
    //Armamos un array de 16 intervalos, que representan la frecuencia de los distintos rangos de valores de la intensidad
    //frecuencias[i] = (i, i+15)  donde (i, i+15) es el intervalo de grises entre i e i+15   
    for(int i=0; i< m.rows(); i++)
        for(int j=0; j<m.columns(); j++)
            frecuencias[m(i, j)/ 16]++;

}

void ZoomBilineal(const Matrix& original, Matrix& output, int k)
{
    // recorremos las filas que tienen pixeles viejos y llenamos los pixeles nuevos entre ellos
    for (int i=0; i < original.rows(); i++)
    {
        // traduccion del indice a la nueva matrix
        int iNew = i * (k+1);
        for (int j=0; j < original.columns()-1; j++)
        {
            // agarramos los puntos viejos de a pares y llenamos los que hay entre ellos.
            int jNew = j*(k+1);

            int iNext = i;
            int jNext = (j+1)*(k+1);

            for (int _j = 1; _j < k+1; _j++)
            {
                //No sabemos si castear a double en algun momento para mejorar la precision
                output(iNew,jNew +_j) = ( output(iNew, jNext) - output(iNew,jNew))*(_j)/(k+1) + output(iNew, jNew);
                //Si 0 y k+1 son los puntos a interpolar, entonces la recta es: y= (c_(k+1) - c_0)/(k+1) * x + c_0
                //Donde x es el pixel para el que se quiere calcular la intensidad
            }
        }
    }

    // Las filas con puntos viejos, ya todas tienen un valor asignado (interpolado u original)

    // Hacemos la interpolacion que queda por columnas.

    for (int j = 0; j < output.columns(); j++)
    {
        // recorro de la matriz output, las filas con puntos viejos, y las agarro de a pares.
        // Por eso no agarro la ultima
        for (int i = 0; i < output.rows()-1; i = i+(k+1))
        {
            int iNext = i+(k+1);
            //int jNext = j; // es la misma columna

            // offset valores sin asignar entre valores ya asignados
            for (int _i = 1; _i < (k+1); _i++)
            {
                int value = (output(iNext, j) - output(i,j)) * _i / (k+1) + output(i,j);
                output(_i + i, j) = value;
            }
        } 
    }

    // interpolamos por columnas entre puntos originales
    /*for (int j=0; j < original.columns(); j++)
    {
        int jNew = j*(k+1);
        for (int i = 0; i < original.rows()-1; i++)
        {
            int iNew = i*(k+1);
            int jNext = j;
            int iNext = (i+1)*(k+1);

            for (int _i = 1; _i < k+1; _i++)
            {
                output(iNew+ _i, jNew) = (output(iNext, jNew) - output(iNew, jNew))*(_i)/(k+1) + output(iNew, jNew);
            }
        }
    }

    for (int i = 1; i < output.rows()-1; i = (i+1) % (k+1) == 0 ? i+2 : i+1)
    {
        // estoy parado en una fila i sin puntos viejos
        // recorro todas las columnas, menos la primera y la ultima que ya fueron estimadas
        for (int _j=1; _j < output.columns()-1; _j++)
            output(i,_j) = (output(i,output.columns()-1) - output(i,0)) * _j / (k+1) + output(i,0);
    }*/    
}

void ZoomVecinosMasCercanos(const Matrix& input, Matrix& output, int k)
{
    // Para este modo de zoom decidimos lo siguiente:
    //  A cada punto nuevo, le calculamos la distancia (manhattan) a puntos ya existentes.
    //  Nos quedamos con el de menor distancia.
    //  Si hay empate por distancia, verificamos cual es el que su rango de intensidad es el mas frecuente
    //  Sino se puede desempatar asi, optamos por el primero que encontramos de los empatados.

    //Lo creamos para llamar despues a calculoFrecuencia
    int frecuencias [16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   
    calculoFrecuencia(input, frecuencias);

    ItPixelesNuevos it(output.rows(), output.columns(), k);

    while(!it.end())
    {
        int i = it.get().first;
        int j = it.get().second;
        //std::cout << "Fila: " << i << "  Columna: " << j << "\r"; std::cout.flush(); //Para ver por donde anda

        int distancia = output.rows()*output.columns() + 1;
        int valor = 0;
        
        // Supongamos que el punto existente, en la img original es el (i,j)
        // Su nuevo (i', j') en la extendida es: (... Fabi tiene la cuenta)
        // Itero por los originales y comparo distancias.
        int ki = i - (i % (k+1));                                       //Fila con pixeles originales anterior
        int kil = (i + k) >= output.rows()? output.rows()-1 : i + k;    //Fila con pixeles originales posterior

        for(ki; ki <= kil; ki += + k + 1)
        {
            int kj = j - (j % (k+1));                                           //Columna con pixeles originales anterior
            int kjl = (j + k) >= output.columns()? output.columns()-1 : j + k;  //Columna con pixeles originales posterior

            for(kj; kj <= kjl; kj += k + 1 )
            {
                //actualizo el nuevo mas cercano
                int d = norma1(i,j,ki,kj);
                if(distancia > d)
                {
                    distancia = d;
                    valor = output(ki,kj);
                }
                else if(distancia == d && frecuencias[output(ki,kj)/16] > frecuencias[valor/16])
                        valor = output(ki,kj); // seteas tu nueva mejor clasificacion
                // else
                   // no hago nada decido quedarme con el actual
            }
        }
        output(i,j) = valor;
        ++it;
    }
}

int main(int argc, char *argv[]) {

    // primer parametro: path a la imagen a procesar
    // segundo parametro: altura de la imagen (pixeles)
    // tercer parametro: ancho de la imagen (pixeles)
    // cuarto parametro: k
    // quinto parametro: modo de operación

    if (argc < 5)
    {
        cout << "Te faltan parametros" << endl;
        return 1;
    }

    cout << "Imagen: " << argv[1] << std::endl;
    cout << "Filas: " << argv[2] << std::endl;
    cout << "Columnas: " << argv[3] << std::endl;
    cout << "Zoom: " << argv[4] << std::endl;
    cout << "Modo de operación: " << argv[5] << std::endl;

    int filas, columnas, k, op;
    filas = stoi(argv[2]);
    columnas = stoi(argv[3]);
    k = stoi(argv[4]);
    op = stoi(argv[5]);

    // Cargo la imagen como una matriz
    Matrix m(filas, columnas);
    loadImage(m,argv[1]);

    // con este constructor generamos la matriz para la imagen aumentada
    // y traspasamos los puntos ya conocidos
    Matrix output(m, k);

    switch(op)
    {
        case 0:
            ZoomVecinosMasCercanos(m,output,k);
            break;
        case 1:
            ZoomBilineal(m,output,k);
            break;
        default:
            cout << "MODO DE OPERACION NO DEFINIDO " << endl;
    }
    

    output.writeMatrix("output.csv");

    return 0;
}


