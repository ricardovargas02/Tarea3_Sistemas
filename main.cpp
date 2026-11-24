#include <iostream>
#include "disco/disco.h"
using namespace std;

int main()
{
    int discoMB;
    int bloqueKB;
    int mecanismo;
    int archivoMinKB;
    int archivoMaxKB;

    cout << "=== Simulador de asignacion de archivos ===" << endl;
    cout << "Tamano del disco (MB): ";

    if (!(cin >> discoMB) || discoMB <= 0)
    {
        cout << "Error: tamano de disco invalido." << endl;
        return 1;
    }

    cout << "Tamano de cada bloque (KB): ";
    if (!(cin >> bloqueKB) || bloqueKB <= 0)
    {
        cout << "Error: tamano de bloque invalido." << endl;
        return 1;
    }

    cout << "\nMecanismo de asignacion:" << endl;
    cout << "1. Contigua" << endl;
    cout << "2. Enlazada" << endl;
    cout << "3. Indexada" << endl;
    cout << "Seleccione una opcion: ";

    if (!(cin >> mecanismo) || mecanismo < 1 || mecanismo > 3)
    {
        cout << "Error: mecanismo invalido." << endl;
        return 1;
    }

    cout << "\nTamano minimo de archivo (KB): ";
    if (!(cin >> archivoMinKB) || archivoMinKB <= 0)
    {
        cout << "Error: tamano minimo invalido." << endl;
        return 1;
    }

    cout << "Tamano maximo de archivo (KB): ";
    if (!(cin >> archivoMaxKB) || archivoMaxKB < archivoMinKB)
    {
        cout << "Error: rango de archivos invalido." << endl;
        return 1;
    }

    long long discoKB = static_cast<long long>(discoMB) * 1024;
    long long cantidadBloques = discoKB / bloqueKB;
    long long espacioRestante = discoKB % bloqueKB;

    if (cantidadBloques == 0)
    {
        cout << "Error: el bloque es mayor que el disco." << endl;
        return 1;
    }

    Disco disco;
    iniciarDisco(disco, bloqueKB, cantidadBloques);

    long long bloquesLibres = contarBloquesLibres(disco);
    long long bloquesOcupados = cantidadBloques - bloquesLibres;


    cout << "\n=== Configuracion del disco ===" << endl;
    cout << "Tamano del disco: " << discoMB << " MB" << endl;
    cout << "Tamano del bloque: " << bloqueKB << " KB" << endl;
    cout << "Cantidad de bloques: " << cantidadBloques << endl;
    cout << "Bloques libres: " << bloquesLibres << endl;
    cout << "Bloques ocupados: " << bloquesOcupados << endl;
    cout << "Espacio sin formar bloque: "
         << espacioRestante << " KB" << endl;


    cout << "Mecanismo: ";


    if (mecanismo == 1)
        cout << "Contigua" << endl;
    else if (mecanismo == 2)
        cout << "Enlazada" << endl;
    else
        cout << "Indexada" << endl;


    cout << "Rango de archivos: "
         << archivoMinKB << " - "
         << archivoMaxKB << " KB" << endl;


    return 0;
}