/*
 *	MODULO: fragmenta
 *	Creado por David Perez, Alex Salvatierra, Alejandro Martinez y Alvaro Larraya el 28/11/20.
 *  DESCRIPCION: Este modulo exporta las funcionalidades necesarias para trocear cadenas. Se tomará una cadena de texto de 
 *               y se fragmentará de tal modo que se ajuste a la estructura de datos
 *               que posteriormente empleará para rellenar los parametros de la tabla 
 */

#ifndef FRAGMENTA_H
#define FRAGMENTA_H

#include <string.h>

/*
 *  FUNCION: fragmenta
 *  ENTRADA: apuntador a caracter
 *  REQUISITOS: entrada no nula
 *  SALIDA: doble apuntador a caracter de la cadena fragmentada por comas ','
 */
char **fragmenta(const char*);  

/*
 *  FUNCION: borrarg
 *  ENTRADA: doble apuntador a caracter
 *  SALIDA: libera la memoria utilizada
 */
void borrarg(char **arg); 

#endif
