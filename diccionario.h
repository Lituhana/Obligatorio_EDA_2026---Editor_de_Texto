#ifndef DICCIONARIO_H
#define DICCIONARIO_H

// ============================================================
// diccionario.h
// Diccionario ortográfico implementado como ABB (árbol binario
// de búsqueda). Se elige ABB porque:
//   - insertar es O(log n) promedio  (op. 12)
//   - recorrido in-order da orden alfabético en O(n)  (op. 14)
// El orden se define comparando palabras IGNORANDO mayúsculas
// (se compara con strcasecmp de <strings.h>).
// No lleva descriptor: el tipo Diccionario es directamente el
// puntero a la raíz (NULL = diccionario vacío).
// ============================================================

#include "definiciones.h"

typedef struct _nodoDic *Diccionario;   // puntero a la raíz del ABB

Diccionario crearDiccionario();
// Post: devuelve un diccionario vacío (NULL).

TipoRetorno insertarEnDiccionario(Diccionario &d, Cadena palabra);
// Pre:  palabra != NULL.
// Post: si la palabra NO estaba, la agrega (guardando una COPIA, tal
//       como fue ingresada) y devuelve OK.
//       Si ya estaba (ignorando mayúsculas) no modifica nada y
//       devuelve ERROR.
// Nota: va por referencia (&) porque si d es NULL hay que cambiar
//       a dónde apunta.

TipoRetorno borrarDeDiccionario(Diccionario &d, Cadena palabra);
// Pre:  palabra != NULL.
// Post: si la palabra estaba, la elimina (liberando memoria, ojo con
//       los 3 casos: hoja, un hijo, dos hijos) y devuelve OK.
//       Si no estaba devuelve ERROR.

bool perteneceADiccionario(Diccionario d, Cadena palabra);
// Pre:  palabra != NULL.
// Post: true si la palabra está (ignorando mayúsculas). No modifica d.
//       Lo usa ImprimirTextoIncorrecto (op. 15).

void imprimirDiccionario(Diccionario d);
// Post: imprime una palabra por línea, ordenadas alfabéticamente.
//       Si d es vacío imprime "Diccionario vacio".
//       Debe ser O(n) peor caso.
//       OJO: el mensaje de vacío se decide una sola vez (no dentro de
//       la recursión, o lo imprimirías en cada hoja).

void destruirDiccionario(Diccionario &d);
// Post: libera TODOS los nodos y las palabras copiadas. d queda NULL.

#endif