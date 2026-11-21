#ifndef LINEA_H
#define LINEA_H

// ============================================================
// linea.h
// Una línea es una lista simplemente enlazada de palabras, con
// DESCRIPTOR: un struct que además de apuntar al primer nodo
// guarda la cantidad de palabras. Así validar posiciones y saber
// si la línea está llena es O(1), sin recorrer.
//
// Este módulo es INDEPENDIENTE: no conoce al diccionario ni al texto.
//
// Reglas del enunciado:
//   - palabras en posiciones consecutivas desde la 1 (sin huecos)
//   - el máximo es MAX_CANT_PALABRAS_X_LINEA, pero ESTE módulo NO lo
//     hace cumplir: insertarPalabraEnLinea NO verifica el máximo y puede
//     dejar la línea con MAX+1 palabras por un instante. Quien conoce
//     la línea siguiente (texto.cpp) debe corregirlo en seguida con
//     sacarUltimaPalabra. Así funciona también el caso borde de insertar
//     en la posición MAX+1 de una línea llena.
//   - las palabras se guardan como se ingresaron y se comparan
//     ignorando mayúsculas.
// ============================================================

#include "definiciones.h"

typedef struct _linea *Linea;   // Linea = puntero al descriptor

Linea crearLinea();
// Post: devuelve una línea vacía (0 palabras).

unsigned int cantPalabras(Linea l);
// Pre:  l != NULL.
// Post: devuelve la cantidad de palabras. O(1).

bool lineaLlena(Linea l);
// Pre:  l != NULL.
// Post: true si cantPalabras(l) >= MAX_CANT_PALABRAS_X_LINEA.

TipoRetorno insertarPalabraEnLinea(Linea l, Posicion pos, Cadena palabra);
// Pre:  l != NULL, palabra != NULL.
// Post: inserta una COPIA de palabra en la posición pos y corre una
//       posición adelante las siguientes. Devuelve OK.
//       Si pos < 1 o pos > cantPalabras+1 devuelve ERROR sin modificar.
//       NO controla MAX_CANT_PALABRAS_X_LINEA (ver encabezado).

TipoRetorno borrarPalabraEnLinea(Linea l, Posicion pos);
// Pre:  l != NULL.
// Post: elimina la palabra en pos (liberando memoria) y corre las
//       siguientes una posición hacia adelante. Devuelve OK.
//       Si pos < 1 o pos > cantPalabras devuelve ERROR.

void borrarOcurrenciasEnLinea(Linea l, Cadena palabra);
// Pre:  l != NULL, palabra != NULL.
// Post: elimina TODAS las apariciones de palabra (ignorando
//       mayúsculas). Si no hay ninguna, no hace nada.

Cadena palabraEnPosicion(Linea l, Posicion pos);
// Pre:  l != NULL.
// Post: devuelve la palabra en la posición pos SIN copiarla (es el puntero
//       interno: NO modificarlo ni liberarlo). Devuelve NULL si pos < 1
//       o pos > cantPalabras. Sirve a texto para ImprimirTextoIncorrecto
//       sin que linea dependa del diccionario.

Cadena sacarUltimaPalabra(Linea l);
// Pre:  l != NULL y cantPalabras(l) > 0.
// Post: quita la última palabra de la línea y la DEVUELVE SIN LIBERAR
//       (el que la recibe pasa a ser su dueño). Sirve para el
//       desborde de InsertarPalabra.

Cadena sacarPrimeraPalabra(Linea l);
// Pre:  l != NULL y cantPalabras(l) > 0.
// Post: igual que la anterior pero con la primera. Sirve para
//       ComprimirTexto (traer palabras de la línea siguiente).

void imprimirLinea(Linea l);
// Pre:  l != NULL.
// Post: imprime las palabras separadas por un espacio, SIN número de
//       línea y SIN salto de línea final (el número y el salto los
//       agrega quien llama, que conoce la posición).
//       Revisá los espacios finales contra la salida esperada.

void destruirLinea(Linea &l);
// Post: libera todas las palabras, nodos y el descriptor. l queda NULL.

#endif