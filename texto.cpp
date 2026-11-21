// ============================================================
// texto.cpp - implementación de texto.h
// ============================================================
#include <stdio.h>
#include "texto.h"
#include <stdlib.h>   // NULL

// MEMORIA: cada nodo se pide con "new _nodoLinea" y se libera con "delete".
// Lista de líneas: typedef ANTES del struct (convención de clase).
typedef struct _nodotexto *ListaLineas;

struct _nodotexto {
    Linea linea;             // la línea (con sus palabras)
    ListaLineas sig;         // siguiente línea (NULL si es la última)
};

// DESCRIPTOR del texto.
struct _texto {
    ListaLineas primera;     // primera línea (NULL si el texto está vacío)
    ListaLineas ultima;      // última línea: permite insertar al final en O(1)
    unsigned int cantidad;   // contador de líneas: se actualiza en cada insertar/borrar
};

Texto crearTexto () {
    Texto t = new _texto;
    t -> primera = NULL;
    t -> ultima = NULL;
    t -> cantidad = 0;
    return t;
}

unsigned int cantLineas(Texto t) {
    return t -> cantidad;
}

// Auxiliar PRIVADA: saca 'actual' de la lista y lo libera.
// 'anterior' es su predecesor (NULL si actual es la primera).
static void eliminarNodoLinea (Texto t, ListaLineas anterior, ListaLineas actual) {
    if (anterior == NULL) t -> primera = actual -> sig;
    else                  anterior -> sig = actual -> sig;
    if (actual == t -> ultima) t -> ultima = anterior;
    destruirLinea (actual -> linea);
    delete actual;
    t -> cantidad--;
}

// Auxiliar PRIVADA: imprime "N:" + palabras (cada una precedida de un espacio) + salto.
static void imprimirLineaNumerada (Linea l, Posicion pos) {
    printf ("%u:", pos);
    if (cantPalabras (l) > 0) {
        printf (" ");
        imprimirLinea (l);
    }
    printf ("\n");
}

TipoRetorno insertarLineaAlFinal (Texto t) {
    // PRE: t no está vacío.
    // POS: Inserta una nueva línea vacía al final del texto.

    ListaLineas nuevo = new _nodotexto; // Reservamos memoria para el nodo
    nuevo -> linea = crearLinea();
    nuevo -> sig = NULL;

    if (t -> ultima == NULL) { // Si el texto está vacío, la nueva línea es la primera
        t -> primera = nuevo;
    } else {
        t -> ultima -> sig = nuevo; // La nueva línea se engancha al final de la lista
    }
    t -> ultima = nuevo; // La nueva línea pasa a ser la última
    t -> cantidad++;
    return OK;
}

TipoRetorno insertarLineaEnPosicion(Texto t, Posicion posLinea) {
    // PRE: t != NULL.
    // POS: Inserta una línea vacía en posLinea y corre las siguientes una posición.
    //      Devuelve ERROR si posLinea < 1 o posLinea > cantidad + 1 (sin modificar nada).

    if (posLinea < 1 || posLinea > t -> cantidad + 1) // Posición inválida
        return ERROR;

    if (posLinea == t -> cantidad + 1) // Insertar al final (también cubre el texto vacío)
        return insertarLineaAlFinal (t);

    ListaLineas nuevo = new _nodotexto; // Reservamos memoria para el nodo
    nuevo -> linea = crearLinea();

    if (posLinea == 1) { // Insertamos al principio (el texto no está vacío)
        nuevo -> sig = t -> primera;
        t -> primera = nuevo;
    } else { // Insertamos en el medio
        ListaLineas ant = t -> primera;
        for (Posicion i = 1; i < posLinea - 1; i++) // Avanzamos hasta la línea anterior a la posición pedida
            ant = ant -> sig;
        nuevo -> sig = ant -> sig;
        ant -> sig = nuevo;
    }
    // 'ultima' no cambia: en este caso nunca se inserta después de ella.
    t -> cantidad++;
    return OK;
}

TipoRetorno borrarLineaEnTexto (Texto t, Posicion posLinea) {
    // PRE: t no está vacío.
    // POS: Borra la línea en la posición posLinea del texto.

    if (posLinea < 1 || posLinea > t -> cantidad) // La posición ingresada por el usuario es menor al rango
        return ERROR;

    ListaLineas ant = NULL; 
    ListaLineas aux = t -> primera; 
    for (Posicion i = 1; i < posLinea; i++) { // Avanzamos hasta la línea que queremos borrar
        ant = aux;
        aux = aux -> sig;
    }

    if (ant == NULL) // Aux es la primera línea del texto
        t -> primera = aux -> sig; 
    else // Aux está en el medio o es la última línea del texto
        ant -> sig = aux -> sig; 

    if (aux == t -> ultima) // Aux es la última línea del texto
        t -> ultima = ant;

    destruirLinea (aux->linea);
    delete aux;
    t -> cantidad--;
    return OK;
}

TipoRetorno borrarTodoElTexto(Texto t) {
    // PRE: t != NULL.
    // POS: Borra todas las líneas (liberando memoria). El texto queda vacío pero
    //      utilizable (primera = ultima = NULL, cantidad = 0). Siempre OK.

    while (t -> primera != NULL) {
        ListaLineas aux = t -> primera;
        t -> primera = aux -> sig;
        destruirLinea (aux -> linea); // Libera las palabras y el descriptor de la línea
        delete aux;                   // Libera el nodo
    }
    t -> ultima = NULL;
    t -> cantidad = 0;
    return OK;
}

TipoRetorno insertarPalabraEnTexto (Texto t, Posicion posLinea, Posicion posPalabra, Cadena palabra) { // Aux Función 8
    // PRE: t no está vacío.
    // POS: Inserta la palabra en la posición posPalabra de la línea posLinea
    
    Posicion pos = 1; // Usamos Posicion para evitar warnings de tipo signed/unsigned

    if (posLinea < 1) 
        return ERROR;

    ListaLineas iter = t -> primera; // Puntero auxiliar que apunta al primer nodo de la lista de líneas
    while (iter != NULL && pos < posLinea) {
        iter = iter -> sig; // Avanzo la línea 
        pos++; //Aumento la posición, hasta llegar a la posicion ingresda por el usuario (PosLinea)
    }
    if (iter == NULL) { // Llegué al final del texto y no encontré la posLinea
        return ERROR; 
    }

    if (insertarPalabraEnLinea (iter -> linea, posPalabra, palabra) != OK) { // La posPalabra es inválida (o es menor a 1 o es mayor a la cantidad + 1), me salgo en el primer if.
        return ERROR;
    }
    
    while (cantPalabras (iter -> linea) > MAX_CANT_PALABRAS_X_LINEA) { // Manejo del "desborde" - Tope 3 palabras por línea
        Cadena aux = sacarUltimaPalabra (iter -> linea); // Sacamos la última palabra de la línea (la que se desborda)
        if (iter -> sig == NULL) // Si no hay línea siguiente, la creamos
            insertarLineaAlFinal (t); 
        iter = iter -> sig;  // Avanzamos a la línea siguiente, la recién creada y recibe la palabra desbordada
        insertarPalabraEnLinea (iter -> linea, 1, aux);  // Insertamos la palabra desbordada al principio de la siguiente línea, que me copié en sacarUltimaPalabra.
        delete [] aux;  // insertarPalabraEnLinea guarda una COPIA, se libera la original (se hace para evitar fugas de memoria)
    }
    return OK;
}

TipoRetorno borrarPalabraEnTexto (Texto t, Posicion posLinea, Posicion posPalabra) { // Aux Función 9
    // PRE: t != NULL.
    // POS: Borra la palabra en la posición posPalabra de la línea posLinea

    Posicion pos = 1; // Usamos Posicion

    if (posLinea < 1 || posPalabra < 1)  // posLinea o posPalabra son inválidas
        return ERROR;

    ListaLineas iter = t -> primera; // Puntero auxiliar que arranca en la primera línea del texto
    while (iter != NULL && pos < posLinea) { // Avanzamos hasta la lí nea que contiene la palabra a borrar
        iter = iter -> sig; 
        pos++; 
    }
    if (iter == NULL) // Llegamos al final del texto y la línea no existe
        return ERROR;

    return borrarPalabraEnLinea (iter -> linea, posPalabra); // Me salí del while y llamo a la función borrarPalabraEnLinea para borrar la palabra en la línea.
}

TipoRetorno borrarOcurrenciasEnLineaDeTexto (Texto t, Posicion posLinea, Cadena palabra) { // Aux Función 10
    // PRE: t != NULL.
    // POS: Borra todas las ocurrencias de la palabra en la línea posLinea del texto.

    Posicion pos = 1;

    if (posLinea < 1) // La posición de la línea es menor a 1
        return ERROR;

    ListaLineas iter = t -> primera; // Puntero auxiliar que arranca en la primera línea del texto
    while (iter != NULL && pos < posLinea) { // Mientras el nodo no sea nulo y pos sea menor a posLinea solicitada
        iter = iter -> sig;
        pos++;
    }
    if (iter == NULL) { // Llegamos al final de la lista y posLínea no existe.
        return ERROR;
    }
    borrarOcurrenciasEnLinea (iter -> linea, palabra); // El borrado lo hacemos llamando a la función borrarOcurrenciasEnLinea delinea.cpp
    return OK;
}

TipoRetorno imprimirLineaDeTexto (Texto t, Posicion posLinea) { // Aux Función 11
    // PRE: T no está vacío.
    // POS: Imprime la línea posLinea del texto.

    Posicion pos = 1; // Usamos Posicion

    if (posLinea < 1) { 
        return ERROR;
    } else {
        ListaLineas iter = t -> primera; // Puntero auxiliar que comienza apuntando a la primera línea del texto
        while (iter != NULL && pos < posLinea) { 
            iter = iter -> sig;
            pos++;
        }
        if (iter == NULL) 
            return ERROR;

        imprimirLineaNumerada (iter -> linea, posLinea); // Llamamos a la función auxiliar que imprime la línea con su número delante.
        return OK;
    }
}

// ---------------------- OPERACIÓN 5 ----------------------
TipoRetorno borrarOcurrenciasEnTexto(Texto t, Cadena palabra) {
    // PRE: t != NULL, palabra != NULL.
    // POS: Recorre todas las líneas borrando la palabra (ignorando mayúsculas).
    //      Las líneas que queden vacías NO se eliminan. Siempre OK.

    ListaLineas iter = t -> primera;
    while (iter != NULL) {
        borrarOcurrenciasEnLinea (iter -> linea, palabra);
        iter = iter -> sig;
    }
    return OK;
}

// ---------------------- OPERACIÓN 6 ----------------------
TipoRetorno imprimirTexto(Texto t) {
    // PRE: t != NULL.
    // POS: Imprime "N: palabras..." por cada línea. Si no hay líneas imprime "Texto vacio".

    if (t -> primera == NULL) { 
        printf ("Texto vacio\n");
        return OK;
    }

    Posicion pos = 1;
    ListaLineas iter = t -> primera;
    while (iter != NULL) {
        imprimirLineaNumerada (iter -> linea, pos);
        iter = iter -> sig;
        pos++;
    }
    return OK;
}


TipoRetorno imprimirTextoIncorrecto(Texto t, Diccionario d) {
    // PRE: t != NULL.  
    // POS: Imprime cada línea con solo las palabras que no están en el diccionario. 
    //      Si no hay líneas, imprime "Texto vacio".
    if (t -> primera == NULL){
        printf("Texto vacio\n");
        return OK;
    }

    Posicion posLinea = 1;
    ListaLineas iter = t -> primera;
    while (iter != NULL) {
        printf ("%u:", posLinea);

        for (Posicion i = 1; i <= cantPalabras (iter -> linea); i++) {
            Cadena p = palabraEnPosicion (iter -> linea, i);
            if (!perteneceADiccionario(d, p)) {
                printf (" %s", p);
            }
        }

        printf ("\n");
        iter = iter -> sig;
        posLinea++;
    }
    return OK;
}

TipoRetorno comprimirTexto (Texto t) {
    // PRE: t != NULL.
    // FASE 1: eliminar todas las líneas vacías.
    ListaLineas anterior = NULL;
    ListaLineas actual = t -> primera;
    while (actual != NULL) {
        ListaLineas siguiente = actual -> sig;
        if (cantPalabras (actual -> linea) == 0)
            eliminarNodoLinea (t, anterior, actual);   // 'anterior' no avanza
        else
            anterior = actual;
        actual = siguiente;
    }

    // FASE 2: llenar cada línea con palabras de la siguiente.
    // Invariante: a esta altura ninguna línea está vacía.
    ListaLineas destino = t -> primera;
    while (destino != NULL) {
        while (!lineaLlena (destino -> linea) && destino -> sig != NULL) {
            ListaLineas fuente = destino -> sig;
            Cadena p = sacarPrimeraPalabra (fuente -> linea);
            // insertarPalabraEnLinea guarda una COPIA, por eso se libera la original.
            insertarPalabraEnLinea (destino -> linea, cantPalabras (destino -> linea) + 1, p);
            delete [] p;
            if (cantPalabras (fuente -> linea) == 0)     // la fuente quedó vacía: se elimina
                eliminarNodoLinea (t, destino, fuente);
        }
        destino = destino -> sig;
    }
    return OK;
}

void destruirTexto(Texto &t) {
    borrarTodoElTexto(t);
    delete t;
    t = NULL;
}
