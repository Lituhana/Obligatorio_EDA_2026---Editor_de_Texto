// Estructuras de Datos y Algoritmos - Obligatorio 2026
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// editor.c
// Modulo de implementacion de editor.

#include "editor.h"
// Modulos realizados.
#include "texto.h"
#include "linea.h"
#include "diccionario.h"
#include "ultimas.h"
#include <string.h>
#include <iostream>

using namespace std;

struct nodo_editor{
	Texto texto;          // Líneas y sus palabras (módulo texto)
	Diccionario dic;      // Diccionario ortográfico, un ABB (módulo diccionario)
	Ultimas ultimas;  // Ultimas palabras ingresadas al texto
};


editor CrearEditor () {
// Crea la estructura editor.
	editor e = new (nodo_editor);
	e -> texto = crearTexto ();
	e -> dic = crearDiccionario ();
	e -> ultimas = crearUltimas ();
	return e;
}

TipoRetorno InsertarLinea (editor & e) {
// Inserta una nueva línea vacía al final del texto.
// Este requerimiento debe ser resuelto en O(1) peor caso.
	return insertarLineaAlFinal (e -> texto);
}

TipoRetorno InsertarLineaEnPosicion(editor & e, Posicion posicionLinea){
// Inserta una nueva línea vacía en la posición indicada.
// Ver más detalles en la letra del obligatorio.
	return insertarLineaEnPosicion (e -> texto, posicionLinea);
}

TipoRetorno BorrarLinea(editor & e, Posicion posicionLinea){
// Borra la línea en la posición indicada.
// Ver más detalles en la letra del obligatorio.
	return borrarLineaEnTexto (e -> texto, posicionLinea);
}

TipoRetorno BorrarTodo(editor & e){
// Borra todas las líneas del texto.
// Ver más detalles en la letra del obligatorio.
	return borrarTodoElTexto (e -> texto);
}

TipoRetorno BorrarOcurrenciasPalabraEnTexto(editor & e, Cadena palabraABorrar){
// Borra todas las ocurrencias de una palabra en el texto.
// Ver más detalles en la letra del obligatorio.
	return borrarOcurrenciasEnTexto (e -> texto, palabraABorrar);
}

TipoRetorno ImprimirTexto(editor & e){
// Imprime el texto por pantalla.
// Ver más detalles en la letra del obligatorio.
	return imprimirTexto (e -> texto);
}

TipoRetorno ComprimirTexto(editor & e){
// Comprime las palabras del texto. Para implementar esta operación no debe generarse un nuevo documento.
// Ver más detalles en la letra del obligatorio.
	return comprimirTexto(e -> texto);
}

TipoRetorno InsertarPalabra (editor & e, Posicion posicionLinea, Posicion posicionPalabra, Cadena palabraAIngresar) { // Función 8
// Inserta la palabra palabraAIngresar en la línea posicionLinea y dentro de la línea en la posición posicionPalabra.
// PRE: palabraAIngresar != NULL.
// POS: Si devuelve OK, la palabra quedó en el texto y se registró como la última ingresada.
//      Si devuelve ERROR, no se modifica el texto ni las últimas palabras.

	TipoRetorno ret = insertarPalabraEnTexto (e -> texto, posicionLinea, posicionPalabra, palabraAIngresar);

	if (ret == OK) {
		registrarUltima (e -> ultimas, palabraAIngresar);
	}
	return ret;
}

TipoRetorno BorrarPalabra (editor & e, Posicion posicionLinea, Posicion posicionPalabra) { // Función 9
// Borra la palabra en la posición posicionPalabra de la línea posicionLinea y mueve todas las palabras (si las hay) en las posiciones posteriores de dicha línea un lugar hacia adelante.
	return borrarPalabraEnTexto (e -> texto, posicionLinea, posicionPalabra); 
}

TipoRetorno BorrarOcurrenciasPalabraEnLinea (editor & e, Posicion posicionLinea, Cadena palabraABorrar) { // Función 10
// Borra todas las ocurrencias de la palabra palabraABorrar en la línea indicada.
	return borrarOcurrenciasEnLineaDeTexto ( e-> texto, posicionLinea, palabraABorrar);
}

TipoRetorno ImprimirLinea (editor & e, Posicion posicionLinea) { // Función 11
// Imprime la línea con todas sus palabras.
	return imprimirLineaDeTexto (e -> texto, posicionLinea);
}

TipoRetorno IngresarPalabraDiccionario (editor & e, Cadena palabraAIngresar) { // Función 12
// Ingresa una palabra al diccionario si ésta no se encuentra en el mismo.
	 return insertarEnDiccionario (e -> dic, palabraAIngresar);
}

TipoRetorno BorrarPalabraDiccionario(editor & e, Cadena palabraABorrar){
// Borra una palabra del diccionario.
// Ver más detalles en la letra del obligatorio.
	return borrarDeDiccionario(e->dic, palabraABorrar);
}

TipoRetorno ImprimirDiccionario(editor & e){
// Muestra las palabras del diccionario alfabéticamente.
// Esta operación debe realizarse en O(n) peor caso.
// Ver más detalles en la letra del obligatorio.
	imprimirDiccionario(e->dic);
	return OK;
}

TipoRetorno ImprimirTextoIncorrecto(editor & e){
// Muestra las palabras del texto que no se encuentran en el diccionario.
// Ver más detalles en la letra del obligatorio.
	return imprimirTextoIncorrecto(e->texto, e->dic);
}

TipoRetorno ImprimirUltimasPalabras(editor & e){
// Imprime las últimas MAX_CANT_ULTIMAS_PALABRAS palabras ingresadas al texto.
// Ver más detalles en la letra del obligatorio.
	imprimirUltimas (e -> ultimas);
	return OK;
}

TipoRetorno DestruirEditor(editor & e){
// Destruye la estructura editor y libera la memoria asociada.
	destruirUltimas (e -> ultimas);
	destruirTexto (e -> texto);
	destruirDiccionario (e -> dic);
	delete e;
	e = NULL;
	return OK;
}
