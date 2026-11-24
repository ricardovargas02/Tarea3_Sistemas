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