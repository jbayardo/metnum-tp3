#include "ItPixelesNuevos.h"

bool ItPixelesNuevos::end()
{
    return c == -1 && f == -1;  // este punto ya es viejo, detengase!
}

std::pair<int, int> ItPixelesNuevos::get()
{
    return std::make_pair(f,c);
}

void ItPixelesNuevos::operator++()
{
	// se que los puntos nuevos son multiplos de (k+1) tanto sus filas como columnas
	
    if (c == tColumnas-2 && f == tFilas -1) // el ultimo nuevo agregado
    {
        // Los otros casos no contemplan este, y harian un loop infinito.
        // no puedo avanzar mas
        f = f;
        c++;
    } else if (c == tColumnas-2 && f % (k+1) == 0 && (c+1) % (k+1) == 0) // estoy en una fila que tiene puntos viejos, pero estoy parado en el ultimo nuevo?
    {
        f++;
        c = 0;
    } else if (c == tColumnas-1) // estoy por saltar a otra fila
    {
        if ((f+1) % (k+1) != 0) // es una fila sin viejos,
        {
            f++;
            c =0;
        } else // es una fila con viejos, por eso empiezo corrido
        {
            f++;
            c = 1;
        }
    } else if ( f % (k+1) == 0 && (c+1) % (k+1) == 0) // el proximo es un viejo lo salteo
        c = c+2;
    else // el proximo es nuevo
        c++;
}