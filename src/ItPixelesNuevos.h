#ifndef METNUM_TP3_ITPIXELESNUEVOS_H
#define METNUM_TP3_ITPIXELESNUEVOS_H
#include <utility>      // std::pair

// EJEMPLO
/*

// 5x5 es la NUEVA matriz que se hizo al usar K=1
ItPixelesNuevos it(5,5,1);

while (!it.end())
{
    cout << it.get().first << " " << it.get().second << endl;
    ++it;
}

*/

class ItPixelesNuevos
{
    public:
        // cantidad total de nuevos filas, cantidad total de nuevas columnas, k usado.
        ItPixelesNuevos(int _tFilas, int _tColumnas, int _k) : f(0), c(1), tFilas(_tFilas), tColumnas(_tColumnas), k(_k) { }
        void operator++();
        std::pair<int, int> get();
        bool end();
    private:
        const int tFilas, tColumnas, k;
        int f, c;
};
#endif //METNUM_TP3_ITPIXELESNUEVOS_H
