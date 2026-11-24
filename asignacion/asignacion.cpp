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