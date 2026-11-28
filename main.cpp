#include <iostream>
#include "disco/disco.h"
#include "asignacion/asignacion.h"
#include <random>
#include <vector>
#include <chrono>
#include <thread>
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
    cout << "Espacio sin formar bloque: " << espacioRestante << " KB" << endl;


    cout << "Mecanismo: ";


    if (mecanismo == 1)
        cout << "Contigua" << endl;
    else if (mecanismo == 2)
        cout << "Enlazada" << endl;
    else
        cout << "Indexada" << endl;


    cout << "Rango de archivos: "<< archivoMinKB << " - " << archivoMaxKB << " KB" << endl;

    std::mt19937 generador(std::random_device{}());
    std::uniform_int_distribution<int> distribucion(archivoMinKB, archivoMaxKB);
    std::vector<Archivo> archivos;

    int siguienteId = 1;
    int archivosEliminados = 0;
    long long totalAccesos = 0;
    long long cantidadAccesos = 0;

    auto inicio = std::chrono::steady_clock::now();
    auto proximaCreacion = inicio + std::chrono::seconds(2);
    auto proximaEliminacion = inicio + std::chrono::seconds(30);
    auto proximoAcceso = inicio + std::chrono::seconds(30);



    while (true)
    {
        auto ahora = std::chrono::steady_clock::now();

        if (ahora >= proximaCreacion)
        {
            int tamanoArchivo = distribucion(generador);

            Archivo nuevoArchivo = {};
            bool creado = false;

            if (mecanismo == 1)
            {
                creado = asignarContigua(disco, nuevoArchivo, siguienteId, tamanoArchivo);
            }
            else if (mecanismo == 2)
            {
                creado = asignarEnlazada(disco, nuevoArchivo, siguienteId, tamanoArchivo);
            }
            else if (mecanismo == 3)
            {
                creado = asignarIndexada(disco, nuevoArchivo, siguienteId, tamanoArchivo);
            }

            if (!creado)
            {
                cout << "\nNo se pudo crear el archivo." << endl;
                cout << "Tamano solicitado: " << tamanoArchivo << " KB" << endl;
                cout << "Bloques libres restantes: " << contarBloquesLibres(disco) << endl;
                break;
            }

            archivos.push_back(nuevoArchivo);
            siguienteId++;

            cout << "\n=== Archivo creado ===" << endl;
            cout << "ID: " << nuevoArchivo.id << endl;
            cout << "Tamano: " << nuevoArchivo.tamanoKB << " KB" << endl;
            cout << "Bloques de datos: ";
            for (long long bloque : nuevoArchivo.bloques)
            {
                cout << bloque << " ";
            }
            cout << endl;

            if (!nuevoArchivo.bloquesIndice.empty())
            {
                cout << "Bloques indice: ";
                for (long long bloque : nuevoArchivo.bloquesIndice)
                {
                    cout << bloque << " ";
                }
                cout << endl;
            }

            cout << "Archivos almacenados: " << archivos.size() << endl;
            cout << "Bloques libres restantes: " << contarBloquesLibres(disco) << endl;
            proximaCreacion += std::chrono::seconds(2);
        }

        if (ahora >= proximaEliminacion)
        {
            if (!archivos.empty())
            {
                std::uniform_int_distribution<int> distribucionEliminacion(0, static_cast<int>(archivos.size()) - 1);

                int posicion = distribucionEliminacion(generador);
                int idEliminado = archivos[posicion].id;

                for (long long bloque : archivos[posicion].bloques)
                {
                    disco.bloques[bloque] = 0;
                }

                for (long long bloque : archivos[posicion].bloquesIndice)
                {
                    disco.bloques[bloque] = 0;
                }

                archivos.erase(archivos.begin() + posicion);
                archivosEliminados++;

                cout << "\n=== Archivo eliminado ===" << endl;
                cout << "ID: " << idEliminado << endl;
                cout << "Archivos eliminados: " << archivosEliminados << endl;
                cout << "Archivos almacenados: " << archivos.size() << endl;
                cout << "Bloques libres restantes: " << contarBloquesLibres(disco) << endl;
            }

                  proximaEliminacion += std::chrono::seconds(5);
        }

        if (ahora >= proximoAcceso)
        {
            if (!archivos.empty())
            {
                std::uniform_int_distribution<int> distribucionArchivo(0, static_cast<int>(archivos.size()) - 1);

                int posicion = distribucionArchivo(generador);

                const Archivo &archivo = archivos[posicion];

                long long tamanoBytes = static_cast<long long>(archivo.tamanoKB) * 1024;

                std::uniform_int_distribution<long long> distribucionPosicion(0, tamanoBytes - 1);

                long long posicionLogica = distribucionPosicion(generador);
                long long tamanoBloque = static_cast<long long>(disco.tamanoBloqueKB) * 1024;
                long long espacioDatos = tamanoBloque;

                if (mecanismo == 2)
                {
                    espacioDatos -= 8;
                }

                long long bloqueLogico = posicionLogica / espacioDatos;
                long long bloqueFisico = 0;
                long long accesos = 0;

                if (mecanismo == 1)
                {
                    bloqueFisico = archivo.bloques[0] + bloqueLogico;
                    accesos = 1;
                }
                else if (mecanismo == 2)
                {
                    for (long long i = 0; i <= bloqueLogico; i++)
                    {
                        bloqueFisico = archivo.bloques[i];
                        accesos++;
                    }
                }
                else if (mecanismo == 3)
                {
                    long long referenciasPorIndice = tamanoBloque / 8;
                    long long numeroIndice = bloqueLogico / referenciasPorIndice;
                    long long entradaIndice = bloqueLogico % referenciasPorIndice;

                    cout << "\nBloque indice consultado: "<< archivo.bloquesIndice[numeroIndice] << endl;

                    bloqueFisico = archivo.bloques[
                    numeroIndice * referenciasPorIndice + entradaIndice];

                    accesos = 2;
                }

                totalAccesos += accesos;
                cantidadAccesos++;

                cout << "\n=== Acceso a archivo ===" << endl;
                cout << "ID: " << archivo.id << endl;
                cout << "Posicion logica: " << posicionLogica << " bytes" << endl;
                cout << "Bloque logico: " << bloqueLogico << endl;
                cout << "Bloque fisico: " << bloqueFisico << endl;
                cout << "Accesos necesarios: " << accesos << endl;
            }

            proximoAcceso += std::chrono::seconds(5);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cout << "\n=== Simulacion finalizada ===" << endl;
    cout << "Solicitudes de acceso realizadas: " << cantidadAccesos << endl;
    cout << "Total de accesos al disco: " << totalAccesos << endl;

    return 0;

}