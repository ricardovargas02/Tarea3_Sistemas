#ifndef ASIGNACION_H
#define ASIGNACION_H

#include <vector>
#include "../disco/disco.h"

struct Archivo
{
    int id;
    int tamanoKB;
    std::vector<long long> bloques;
    std::vector<long long> bloquesIndice;
};

bool asignarContigua(Disco &disco, Archivo &archivo, int id, int tamanoKB);
bool asignarEnlazada(Disco &disco, Archivo &archivo, int id, int tamanoKB);
bool asignarIndexada(Disco &disco, Archivo &archivo, int id, int tamanoKB);

#endif