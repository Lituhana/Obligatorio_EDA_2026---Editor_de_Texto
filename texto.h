#ifndef TEXTO_H
#define TEXTO_H

// ============================================================
// texto.h
// El texto es una lista enlazada de líneas con DESCRIPTOR que
// guarda: primera, ultima y cantLineas.
//   - ultima    -> InsertarLinea en O(1) peor caso (op. 1)
//   - cantLineas -> validar posiciones sin recorrer
// Este módulo conoce la relación ENTRE líneas, por eso maneja el
// desborde de InsertarPalabra y la compresión.
// ============================================================

#include "definiciones.h"
#include "linea.h"
#include "diccionario.h"

typedef struct _texto *Texto;   // Texto = puntero al descriptor

Texto crearTexto();
// Post: devuelve un texto vacío (0 líneas).

unsigned int cantLineas(Texto t);
// Pre:  t != NULL.
// Post: devuelve la cantidad de líneas. O(1).

TipoRetorno insertarLineaAlFinal(Texto t);
// Pre:  t != NULL.
// Post: agrega una línea vacía al final. Siempre OK. O(1) peor caso.

TipoRetorno insertarLineaEnPosicion(Texto t, Posicion posLinea);
// Pre:  t != NULL.
// Post: inserta una línea vacía en posLinea y corre las siguientes.
//       ERROR si posLinea < 1 o posLinea > cantLineas+1.
//       Cuidado: si posLinea == cantLineas+1 es equivalente a
//       insertar al final (hay que actualizar 'ultima').

TipoRetorno borrarLineaEnTexto(Texto t, Posicion posLinea);
// Pre:  t != NULL.
// Post: elimina la línea (y sus palabras, liberando memoria).
//       ERROR si posLinea < 1 o posLinea > cantLineas.
//       Cuidado: si borrás la última hay que actualizar 'ultima'.

TipoRetorno borrarTodoElTexto(Texto t);
// Pre:  t != NULL.
// Post: borra todas las líneas (el texto queda vacío). Siempre OK.

TipoRetorno insertarPalabraEnTexto(Texto t, Posicion posLinea,
                                   Posicion posPalabra, Cadena palabra);
// Pre:  t != NULL, palabra != NULL.
// Post: inserta la palabra y, si la línea se pasa de
//       MAX_CANT_PALABRAS_X_LINEA, empuja la última palabra al
//       principio de la línea siguiente (y así en cascada); si no hay
//       línea siguiente se crea una nueva al final.
//       ORDEN: primero se INSERTA la palabra (la línea puede quedar con
//       MAX+1) y recién después se saca la última (sacarUltimaPalabra).
//       Hacerlo al revés falla al insertar en la posición MAX+1 de una
//       línea llena: ahí la palabra que se empuja es la nueva misma.
//       ERROR si posLinea no existe o posPalabra no es válida
//       (posPalabra en [1, cantPalabras+1]).

TipoRetorno borrarPalabraEnTexto(Texto t, Posicion posLinea, Posicion posPalabra);
// Pre:  t != NULL.
// Post: ERROR si la línea o la palabra no existen. OK si borró.
//       (Usa obtenerLinea + borrarPalabraEnLinea.)

TipoRetorno borrarOcurrenciasEnLineaDeTexto(Texto t, Posicion posLinea, Cadena palabra);
// Pre:  t != NULL, palabra != NULL.
// Post: ERROR si la línea no existe; OK si no (aunque no haya borrado nada).

TipoRetorno borrarOcurrenciasEnTexto(Texto t, Cadena palabra);
// Pre:  t != NULL, palabra != NULL.
// Post: recorre todas las líneas borrando la palabra. Siempre OK.

TipoRetorno imprimirTexto(Texto t);
// Pre:  t != NULL.
// Post: imprime "N: palabras..." por cada línea.
//       Si no hay líneas imprime "Texto vacio". Siempre OK.

TipoRetorno imprimirLineaDeTexto(Texto t, Posicion posLinea);
// Pre:  t != NULL.
// Post: imprime "N: palabras..." de esa línea.
//       ERROR si la línea no existe.

TipoRetorno imprimirTextoIncorrecto(Texto t, Diccionario d);
// Pre:  t != NULL.
// Post: igual que imprimirTexto pero solo con las palabras que NO
//       están en d. Si no hay líneas imprime "Texto vacio".
//       (Recorre cada línea con palabraEnPosicion y consulta
//        perteneceADiccionario: así linea no depende de diccionario.)

TipoRetorno comprimirTexto(Texto t);
// Pre:  t != NULL.
// Post: reacomoda las palabras SIN crear un texto nuevo (reusá los
//       nodos / usá sacarPrimeraPalabra) para que todas las líneas
//       menos la última queden llenas y no queden líneas vacías.

void destruirTexto(Texto &t);
// Post: libera todo (líneas, palabras, descriptor). t queda NULL.

#endif