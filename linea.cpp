// ============================================================
// linea.cpp - implementación de linea.h
//
// MEMORIA (resumen):
//   - nodo nuevo:       new _nodoPalabra       -> se libera con delete
//   - descriptor:       new _linea             -> se libera con delete
//   - texto de palabra: new char[strlen(p)+1]  -> se libera con delete[]
//     (se copia con strcpy; NUNCA guardar el puntero que te pasan)
//
// COMPARAR PALABRAS: ignorando mayúsculas con strcasecmp(a, b) == 0
// (de <strings.h>; el main del profe también la usa).
// ============================================================
#include <stdio.h>
#include <string.h>
#include <strings.h>   // strcasecmp
#include "linea.h"

// Lista de palabras: typedef ANTES del struct (convención de clase).
typedef struct _nodolinea *ListaPalabras;

struct _nodolinea {
    Cadena palabra;        // copia propia de la palabra
    ListaPalabras sig;     // siguiente palabra (NULL si es la última)
};

// DESCRIPTOR de la línea.
struct _linea {
    ListaPalabras primera; // primer nodo (NULL si la línea está vacía)
    unsigned int cantidad; // contador: se actualiza en CADA insertar/borrar/sacar
};

// ---------- Auxiliar PRIVADA (static: solo existe en este .cpp) ----------

static Cadena copiarPalabra (Cadena origen) {
    Cadena copia = new char [strlen(origen) + 1];
    strcpy (copia, origen);
    return copia;
}

// ---------- Operaciones públicas ----------

Linea crearLinea () {
    Linea l = new _linea; // Reservo memoria
    l -> primera = NULL; // La línea se crea vacía
    l -> cantidad = 0;
    return l;
}

unsigned int cantPalabras (Linea l) {
    return l -> cantidad;
}

bool lineaLlena (Linea l) {
    return l -> cantidad >= MAX_CANT_PALABRAS_X_LINEA;
}

TipoRetorno insertarPalabraEnLinea (Linea l, Posicion pos, Cadena palabra) { // Aux función 8
    // PRE: l != NULL, palabra != NULL.
    // POS: Inserta la palabra en la posición pos.

    if (pos < 1 || pos > l -> cantidad + 1) // posici+on fuera de rango (Permite el manejo del desborde en insertarPalabraEnTexto)
        return ERROR;

    ListaPalabras aux = new _nodolinea; // Asignamos memoria al nuevo nodo
    aux -> palabra = copiarPalabra (palabra); // Auxiliar privada que copia la palabra

    if (pos == 1) {                // Insertamos la palabra al principio de la línea
        aux -> sig = l -> primera; // El nodo nuevo apunta a la que era la primera palabra
        l -> primera = aux;        // Ahora el nuevo nodo es la primera palabra y l → primera apunta a él

    } else {                                     // Insertamos la palabra al final o en el medio de la línea
        ListaPalabras ant = l -> primera;        // ant (variable auxiliar) que representa al nodo que queda ANTES de aux
        for (Posicion i = 1; i < pos - 1; i++) { // Avanzamos hasta la palabra pos - 1 
            ant = ant -> sig; 
        }
        aux -> sig = ant -> sig;     // Primero el nuevo apunta a lo que venía después
        ant -> sig = aux;            // Después el anterior apunta al nuevo
    }
    l -> cantidad++;                 // Hay una palabra más en la línea
    return OK;
}

Cadena sacarUltimaPalabra (Linea l) { // Aux Función 8
    // PRE: l no está vacía.
    // POS: Saca la última palabra de la línea l y devuelve su texto.

    ListaPalabras ant = NULL;
    ListaPalabras iter = l -> primera; // iter empieza apuntando a la primera palabra

    while (iter -> sig != NULL) { // Avanzamos iter hasta la última palabra de la línea
        ant = iter;
        iter = iter -> sig;
        
    } if (ant == NULL) { // iter es la única palabra de la línea
        l -> primera = NULL;
    } else { // iter es la última palabra de la línea
        ant -> sig = NULL;
    }

    Cadena aux = iter -> palabra; // Guardamos el texto de la palabra para devolverlo (NO se libera)
    delete iter; // Liberamos solo el nodo
    l -> cantidad--;
    return aux;
}

TipoRetorno borrarPalabraEnLinea (Linea l, Posicion pos) { // Aux Función 9
    // PRE:  l no está vacía.
    // POS: Elimina la palabra en pos (liberando memoria) y corre las siguientes una posición hacia atrás.

    Posicion posPalabra = 1; // Usamos Posicion para evitar warnings de signed/unsigned

    if (pos < 1) // La posición ingresada por el usuario es menor a 1
        return ERROR;

    ListaPalabras ant = NULL; // Puntero auxiliar para ir guardando el nodo anterior a aux
    ListaPalabras aux = l -> primera; // Apunto a la primera palabra de la línea

    while (aux != NULL && posPalabra < pos) { // Mientras el nodo no sea nulo y la posición sea menor a pos
        ant = aux;
        aux = aux -> sig; // Avanzo al siguiente nodo
        posPalabra++;
    }
    if (aux == NULL) // Llegamos al final de la línea (La palabra no existe)
        return ERROR;

    // REENGANCHE DE PUNTEROS (aux es la palabra que quiero borrar)

    if (aux -> sig == NULL && ant == NULL) { // 1. Aux es la única palabra de la línea
        l -> primera = NULL;            // La primera palabra de la línea ahora es NULL
        delete [] aux -> palabra;       // Texto de la palabra (se hace para evitar fugas de memoria)
        delete aux; 
        l -> cantidad--;                // Disminute la cantidad de palabras de la línea l
        return OK;

    } else if (aux -> sig == NULL) {        // 2. Aux es la última palabra de la línea
        ant -> sig = NULL;
        delete [] aux -> palabra;       // Texto de la palabra (se hace para evitar fugas de memoria)
        delete aux;
        l -> cantidad--;               // Disminuye la cantidad de palabras de la línea l
        return OK;

    } else if (ant == NULL) {              // 3. Aux es la primera palabra de la línea
        l -> primera = aux -> sig;
        delete [] aux -> palabra;       // Texto de la palabra (se hace para evitar fugas de memoria)
        delete aux;
        l -> cantidad--;
        return OK;

    } else {                              // 4. Aux está en el medio
        ant -> sig = aux -> sig;
        delete [] aux -> palabra;       // Texto de la palabra (se hace para evitar fugas de memoria)
        delete aux;
        l -> cantidad--;
        return OK;
    }
}

// ---------- OPERACIÓN 5 y 10 (parte de línea) ----------
void borrarOcurrenciasEnLinea (Linea l, Cadena palabra) {
    // PRE: l != NULL, palabra != NULL.
    // POS: Elimina TODAS las apariciones de palabra (ignorando mayúsculas), liberando memoria y corriendo las siguientes. Si no hay ninguna, no hace nada.
    
    ListaPalabras ant = NULL; // Puntero auxiliar para ir guardando el nodo anterior a aux
    ListaPalabras aux = l -> primera; // Puntero auxiliar que comienza en la primera palabra de la línea

    while (aux != NULL) { // Mientras tenga palabras en la línea
        if (strcasecmp (aux -> palabra, palabra) == 0) { // Si la palabra que estoy apuntando es = a la palabra que quiero borrar (ignorando mayúsculas)
            ListaPalabras borrar = aux;
            aux = aux -> sig; // Avanzo antes de liberar
            if (ant == NULL) // Era la primera palabra de la línea
                l -> primera = aux; // Ahora el puntero apunta a la siguiente palabra, es nuestra nueva primera palabra
            else // Estaba en el medio o era la última
                ant -> sig = aux;
            delete [] borrar -> palabra; // Libero el texto de la palabra
            delete borrar;              // Libero el nodo
            l -> cantidad--;
        } else { // La palabra que estoy apuntando NO es = a la palabra que quiero borrar.
            ant = aux;
            aux = aux -> sig;
        }
    }
}

Cadena palabraEnPosicion (Linea l, Posicion pos) {
    // PRE: l != NULL.
    // POS: Devuelve la palabra que está en la posición pos de la línea.
    //      Si esa posición no existe, devuelve NULL.

    if (pos < 1 || pos > l -> cantidad) {
        return NULL;
    }

    ListaPalabras aux = l -> primera;
    for (Posicion i=1; i < pos; i++) {
        aux = aux -> sig;
    }
    return aux -> palabra;
}

Cadena sacarPrimeraPalabra (Linea l) {
    // PRE: l -> cantidad > 0. Devuelve la palabra SIN liberarla (el que llama pasa a ser su dueño).
    ListaPalabras nodo = l -> primera;
    Cadena p = nodo -> palabra;
    l -> primera = nodo -> sig;
    delete nodo;                 // se libera el nodo, no el texto de la palabra
    l -> cantidad--;
    return p;
}

// ---------- OPERACIÓN 6 (parte de línea) ----------
void imprimirLinea (Linea l) {
    // PRE: l != NULL.
    // POS: Imprime las palabras separadas por UN espacio, sin espacio final.
    ListaPalabras aux = l -> primera;

    while (aux != NULL) { // Mientras exista una palabra en la línea
        printf("%s", aux -> palabra); // Imprime la palabra
        if (aux -> sig != NULL)
            printf(" "); // Espacio solo ENTRE palabras
        aux = aux -> sig;
    }
}

// ---------- Necesaria para las operaciones 3 y 4 ----------
void destruirLinea (Linea &l) {
    // PRE: l != NULL.
    // POS: Libera todas las palabras, los nodos y el descriptor. l queda NULL.
    ListaPalabras aux = l -> primera;
    while (aux != NULL) {
        ListaPalabras sig = aux -> sig; // Guardo el siguiente antes de liberar
        delete [] aux -> palabra;       // Texto de la palabra (se pidió con new char[])
        delete aux;                     // Nodo
        aux = sig;
    }
    delete l;  // Descriptor
    l = NULL;
}
