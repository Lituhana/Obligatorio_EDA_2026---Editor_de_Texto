// ============================================================
// diccionario.cpp - implementación de diccionario.h
//
// MEMORIA (resumen):
//   - nodo nuevo:       new _nodoDic            -> se libera con delete
//   - texto de palabra: new char[strlen(p)+1]   -> se libera con delete[]
//
// COMPARAR PALABRAS: strcasecmp(a, b) de <strings.h> ignora mayúsculas
// y devuelve <0, 0 o >0. Se usa tanto para decidir izquierda/derecha en
// el ABB como para detectar palabras repetidas (== 0).
// ============================================================
#include <stdio.h>
#include <string.h>
#include <strings.h>   // strcasecmp
#include "diccionario.h"

// Estructura PRIVADA: solo se define acá, el resto del sistema
// solo conoce el typedef del .h (encapsulamiento).
struct _nodoDic {
    Cadena palabra;      // copia propia de la palabra (con sus mayúsculas originales)
    Diccionario izq;     // palabras menores
    Diccionario der;     // palabras mayores
};

// ---------- Auxiliares PRIVADAS ----------

//Para Funcion insertar y borrar.
static Cadena copiarPalabra (Cadena origen) {
    // Pide new char[strlen(origen)+1] y copia con strcpy.
    // (NO usar strdup: reserva con malloc y acá se libera con delete[].)
    if (origen == NULL) {
        return NULL;
    }
    Cadena copia = new char [strlen(origen) + 1];
    strcpy (copia, origen);
    return copia;
}

//Para la Funcion Imprimir.
static void imprimirO(Diccionario d){
    // POS: imprime diccionario en orden
    if(d != NULL){
        imprimirO(d -> izq);
        printf("%s\n", d -> palabra);
        imprimirO(d -> der);
    }
}


// ---------- Operaciones públicas ----------

Diccionario crearDiccionario() {
    // TODO
    return NULL;
}

TipoRetorno insertarEnDiccionario (Diccionario &d, Cadena palabra) { // Aux función 12
    // PRE: Palabra != NULL.
    // POS: Si la palabra no estaba en el diccionario (ignorando mayúsculas), la agrega guardando una copia y devuelve OK. Si ya estaba, devuelve ERROR.

    if (d == NULL) { //Conseguimos un lugar vacío en el diccionario
        d = new _nodoDic;
        d -> palabra = copiarPalabra (palabra);
        d -> izq = NULL;
        d -> der = NULL;
        return OK;
    } 

    int aux = strcasecmp (palabra, d -> palabra); // Comparo la palabra ingresada con la palabra guardada en el nodo actual

    if (aux == 0) { // Son la misma palabra
        return ERROR;

    } else if (aux > 0) { // La palabra a ingresar va después de la palabra que está en el nodo actual (Es un valor mayor en el ABB)
        return insertarEnDiccionario (d -> der, palabra);

    } else { // La palabra va antes de la palabra del nodo (Es un valor menor en el ABB)
        return insertarEnDiccionario (d -> izq, palabra);
    }
}

TipoRetorno borrarDeDiccionario(Diccionario &d, Cadena palabra) {
    // PRE: palabra != NULL.
    // POS: Si la palabra estaba (ignorando mayúsculas), la elimina liberando
    //      la memoria y devuelve OK. Si no estaba, no modifica nada y devuelve ERROR.

    if (d == NULL){                                  // La palabra no está en el diccionario
        return ERROR;
    }

    int aux = strcasecmp (palabra, d -> palabra);     

    if (aux < 0){                                    // Buscando entre las menores
        return borrarDeDiccionario (d -> izq, palabra);

    }else if (aux > 0){                             // Buscando entre las mayores
        return borrarDeDiccionario (d -> der, palabra);

    }else{                                          // Encontramos el nodo a borrar
        if (d -> izq == NULL){                       // Hoja o solo hijo derecho
            Diccionario borrar = d;
            d = d -> der;                             // El padre pasa a apuntar al hijo derecho (puede ser NULL)
            delete [] borrar -> palabra;
            delete borrar;

        }else if (d -> der == NULL){                // Solo hijo izquierdo
            Diccionario borrar = d;
            d = d -> izq;                             // El padre pasa a apuntar al hijo izquierdo
            delete [] borrar -> palabra;
            delete borrar;

        }else{                                      // Dos hijos
            Diccionario suc = d -> der;               // Buscamos el sucesor in-order (el menor del subárbol derecho)
            while (suc -> izq != NULL) {
                suc = suc -> izq;
            }
            Cadena nueva = copiarPalabra (suc -> palabra); // Copiamos ANTES de borrar el sucesor
            delete [] d -> palabra;                   // Liberamos la palabra vieja del nodo
            d -> palabra = nueva;                     // El nodo ahora guarda la palabra del sucesor
            borrarDeDiccionario (d -> der, nueva);    // Borramos el sucesor del subárbol derecho
        }
        return OK;
    }
}

bool perteneceADiccionario(Diccionario d, Cadena palabra) {
    // PRE: palabra != NULL.
    // POS: Devuelve true si palabra está en d, false si no.

	if (d == NULL){
		return false;
    }

    int aux = strcasecmp (palabra, d -> palabra);

	if (aux == 0){
		return true;
    }else if (aux < 0){
		return perteneceADiccionario(d->izq, palabra);
    }else{
		return perteneceADiccionario(d->der, palabra);
    }
}

void imprimirDiccionario(Diccionario d) {
    // Pos: Imprime una palabra por linea en orden alfabetico. 
    //      Si d es vacio imprime "Diccionario vacio"
    if(d == NULL){
        printf("Diccionario vacio\n");
    }else{
        imprimirO(d);
    }
}

void destruirDiccionario(Diccionario &d) {
    // PRE: ninguna (d puede ser vacío).
    // POS: Libera todos los nodos y las palabras copiadas. d queda NULL.
    if (d != NULL) {
        destruirDiccionario (d -> izq);  // Primero los hijos
        destruirDiccionario (d -> der);
        delete [] d -> palabra;          // Después la palabra
        delete d;                        // Y por último el nodo
        d = NULL;
    }
}
