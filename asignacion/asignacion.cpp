#include "asignacion.h"

bool asignarContigua(Disco &disco, Archivo &archivo, int id, int tamanoKB)
{
    if (tamanoKB <= 0 || disco.tamanoBloqueKB <= 0 || !archivo.bloques.empty())
    {
        return false;
    }

    long long bloquesNecesarios = (static_cast<long long>(tamanoKB) + disco.tamanoBloqueKB - 1) / disco.tamanoBloqueKB;

    if (bloquesNecesarios > static_cast<long long>(disco.bloques.size()))
    {
        return false;
    }

    long long consecutivos = 0;

    for (long long i = 0; i < static_cast<long long>(disco.bloques.size()); i++)
    {
        if (disco.bloques[i] == 0)
        {
            consecutivos++;
        }
        else
        {
            consecutivos = 0;
        }

        if (consecutivos == bloquesNecesarios)
        {
            long long inicio = i - bloquesNecesarios + 1;
            archivo.id = id;
            archivo.tamanoKB = tamanoKB;

            for (long long j = inicio; j < inicio + bloquesNecesarios; j++)
            {
                disco.bloques[j] = 1;
                archivo.bloques.push_back(j);
            }


            return true;
        }
    }

    return false;
}

bool asignarEnlazada(Disco &disco, Archivo &archivo, int id, int tamanoKB)
{
    if (tamanoKB <= 0 || disco.tamanoBloqueKB <= 0 ||
        !archivo.bloques.empty())
    {
        return false;
    }

    long long tamanoBloque = static_cast<long long>(
        disco.tamanoBloqueKB) * 1024;


    long long espacioDatos = tamanoBloque - 8;


    if (espacioDatos <= 0)
    {
        return false;
    }


    long long tamanoArchivo =
        static_cast<long long>(tamanoKB) * 1024;


    long long bloquesNecesarios =
        (tamanoArchivo + espacioDatos - 1) / espacioDatos;

    if (contarBloquesLibres(disco) < bloquesNecesarios)
    {
        return false;
    }


    archivo.id = id;
    archivo.tamanoKB = tamanoKB;


    for (long long i = 0;
         i < static_cast<long long>(disco.bloques.size()); i++)
    {
        if (disco.bloques[i] == 0)
        {
            disco.bloques[i] = 1;
            archivo.bloques.push_back(i);


            if (static_cast<long long>(archivo.bloques.size())
                == bloquesNecesarios)
            {
                break;
            }
        }
    }


    return true;
}

bool asignarIndexada(Disco &disco, Archivo &archivo, int id, int tamanoKB)
{
    if (tamanoKB <= 0 || disco.tamanoBloqueKB <= 0 ||
        !archivo.bloques.empty() ||
        !archivo.bloquesIndice.empty())
    {
        return false;
    }

    long long tamanoBloque = static_cast<long long>(disco.tamanoBloqueKB) * 1024;
    long long referenciasPorIndice = tamanoBloque / 8;


    if (referenciasPorIndice == 0)
    {
        return false;
    }


    long long bloquesDatos = (static_cast<long long>(tamanoKB) + disco.tamanoBloqueKB - 1) / disco.tamanoBloqueKB;


    long long bloquesIndice = (bloquesDatos + referenciasPorIndice - 1) / referenciasPorIndice;


    long long bloquesNecesarios = bloquesDatos + bloquesIndice;


    if (contarBloquesLibres(disco) < bloquesNecesarios)
    {
        return false;
    }

    archivo.id = id;
    archivo.tamanoKB = tamanoKB;

    for (long long i = 0; i < static_cast<long long>(disco.bloques.size()); i++)
    {
        if (disco.bloques[i] == 0)
        {
            disco.bloques[i] = 1;

            if (static_cast<long long>(archivo.bloquesIndice.size()) < bloquesIndice)
            {
                archivo.bloquesIndice.push_back(i);
            }
            else
            {
                archivo.bloques.push_back(i);
            }

            if (static_cast<long long>(archivo.bloques.size()) == bloquesDatos)
            {
                break;
            }
        }
    }

    return true;
}