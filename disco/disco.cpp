#ifndef DISCO_H
#define DISCO_H
#include <vector>

struct Disco
{
    int tamanoBloqueKB;
    std::vector<char> bloques;
};

void iniciarDisco(Disco &disco, int bloqueKB, long long cantidadBloques);
long long contarBloquesLibres(const Disco &disco);

#endif

#include "disco.h"

void iniciarDisco(Disco &disco, int bloqueKB, long long cantidadBloques)
{
    disco.tamanoBloqueKB = bloqueKB;
    disco.bloques.assign(cantidadBloques, 0);
}

long long contarBloquesLibres(const Disco &disco)
{
    long long libres = 0;

    for (char bloque : disco.bloques)
    {
        if (bloque == 0)
        {
            libres++;
        }
    }

    return libres;
}