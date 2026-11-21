// ============================================================
// ultimas.cpp - implementación de ultimas.h
//
// MEMORIA (resumen):
//   - nodo nuevo:       new _nodoultimas       -> se libera con delete
//   - descriptor:       new _ultimas           -> se libera con delete
//   - texto de palabra: new char[strlen(p)+1]  -> se libera con delete[]
//     (se copia con strcpy; NUNCA guardar el puntero que te pasan,
//      porque el main reutiliza el buffer del comando)
// ============================================================
#include <stdio.h>
#include <string.h>
#include "ultimas.h"

// Lista de palabras: typedef ANTES del struct (convención de clase).
typedef struct _nodoultimas *ListaUltimas;

struct _nodoultimas {
    Cadena palabra;         // copia propia de la palabra
    ListaUltimas sig;       // palabra más vieja (NULL si es la última de la lista)
};

// DESCRIPTOR del registro.
struct _ultimas {
    ListaUltimas primera;   // la más nueva (NULL si no hay palabras)
    unsigned int cantidad;  // contador: se actualiza en CADA registrar/descartar
};

// ---------- Auxiliares PRIVADAS (static: solo existen en este .cpp) ----------

static Cadena copiarPalabra (Cadena origen) {
    Cadena copia = new char [strlen(origen) + 1];
    strcpy (copia, origen);
    return copia;
}

static void descartarMasVieja (Ultimas u) {
    // PRE: u -> cantidad > 0.
    // POS: Elimina la última palabra de la lista (la más vieja), liberando su memoria.

    ListaUltimas ant = NULL;
    ListaUltimas iter = u -> primera;

    while (iter -> sig != NULL) { // Avanzamos iter hasta la última palabra
        ant = iter;
        iter = iter -> sig;
    }

    if (ant == NULL) { // iter es la única palabra
        u -> primera = NULL;
    } else {           // iter es la última palabra
        ant -> sig = NULL;
    }

    delete [] iter -> palabra; // Texto de la palabra
    delete iter;               // Nodo
    u -> cantidad--;
}

// ---------- Operaciones públicas ----------

Ultimas crearUltimas () {
    // POS: Devuelve un registro vacío.
    Ultimas u = new _ultimas; // Reservo memoria
    u -> primera = NULL;      // Se crea vacío
    u -> cantidad = 0;
    return u;
}

void registrarUltima (Ultimas u, Cadena palabra) {
    // PRE: u != NULL, palabra != NULL.
    // POS: Guarda una copia de palabra como la más nueva. Si ya había
    //      MAX_CANT_ULTIMAS_PALABRAS, descarta antes la más vieja.

    if (u -> cantidad == MAX_CANT_ULTIMAS_PALABRAS) { // Está lleno: se descarta la más vieja
        descartarMasVieja (u);
    }

    ListaUltimas nuevo = new _nodoultimas;      // Asignamos memoria al nuevo nodo
    nuevo -> palabra = copiarPalabra (palabra); // Copia propia
    nuevo -> sig = u -> primera;                // El nuevo apunta a la que era la más nueva
    u -> primera = nuevo;                       // Ahora el nuevo es la más nueva
    u -> cantidad++;                            // Hay una palabra más guardada
}

void imprimirUltimas (Ultimas u) {
    // PRE: u != NULL.
    // POS: Imprime las palabras de la más nueva a la más vieja, una por línea.
    //      Si no hay ninguna, imprime "No se ingresaron palabras".

    if (u -> primera == NULL) {
        printf ("No se ingresaron palabras\n");
        return;
    }

    ListaUltimas aux = u -> primera;
    while (aux != NULL) { // Mientras exista una palabra
        printf ("%s\n", aux -> palabra);
        aux = aux -> sig;
    }
}

void destruirUltimas (Ultimas &u) {
    // PRE: u != NULL.
    // POS: Libera todas las palabras, los nodos y el descriptor. u queda NULL.

    ListaUltimas aux = u -> primera;
    while (aux != NULL) {
        ListaUltimas sig = aux -> sig; // Guardo el siguiente antes de liberar
        delete [] aux -> palabra;      // Texto de la palabra (se pidió con new char[])
        delete aux;                    // Nodo
        aux = sig;
    }
    delete u; // Descriptor
    u = NULL;
}
