#ifndef ULTIMAS_H
#define ULTIMAS_H

// ============================================================
// ultimas.h
// Registro de las últimas MAX_CANT_ULTIMAS_PALABRAS palabras
// ingresadas al texto (op. 16). Igual que una línea, es una lista
// simplemente enlazada de palabras con DESCRIPTOR: un struct que
// además de apuntar al primer nodo guarda la cantidad de palabras.
//
// Las palabras se insertan siempre AL PRINCIPIO, así que la primera
// es la más nueva y la última es la más vieja. Recorrer desde el
// principio da el orden pedido (de la más nueva a la más vieja).
//
// Como nunca hay más de MAX_CANT_ULTIMAS_PALABRAS nodos (constante
// del sistema), registrar una palabra cuesta un tiempo constante.
//
// Este módulo es INDEPENDIENTE: no conoce al texto, a la línea
// ni al diccionario.
// ============================================================

#include "definiciones.h"

typedef struct _ultimas *Ultimas;   // Ultimas = puntero al descriptor

Ultimas crearUltimas();
// Post: devuelve un registro vacío (0 palabras).

void registrarUltima(Ultimas u, Cadena palabra);
// Pre:  u != NULL, palabra != NULL.
// Post: guarda una COPIA de palabra como la más nueva. Si ya había
//       MAX_CANT_ULTIMAS_PALABRAS, descarta la más vieja (liberando
//       su memoria).

void imprimirUltimas(Ultimas u);
// Pre:  u != NULL.
// Post: imprime una palabra por línea, de la más nueva a la más vieja.
//       Si no hay palabras guardadas imprime "No se ingresaron palabras".

void destruirUltimas(Ultimas &u);
// Post: libera todas las palabras, los nodos y el descriptor. u queda NULL.

#endif
