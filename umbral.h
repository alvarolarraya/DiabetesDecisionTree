/*
 *	MODULO: umbral
 *	Creado por David Perez, Alex Salvatierra, Alejandro Martinez y Alvaro Larraya el 28/11/20.
 *  DESCRIPCION: Este modulo exporta las funcionalidades necesarias para dada una tabla de pacientes
 *				 con ocho datos cada uno (edad, bmi, presion sanguinea, glucosa, una funcion de pedigree,
 * 				 insulina, numero de embarazos y grosor de piel) entonces podemos calcular la mejor caracteristica 
 * 				 para dividir el arbol y con que umbral
 */

#ifndef UMBRAL_H
#define UMBRAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct tablon{
    double age;
    double bmi;
    double bloodpressure;
    double glucose;
    double pedigree;
    double insuline;
    double pregnancies;
    double skinthickness;
    double siono;
} tablon;

/*
 *  FUNCION: calculaEntropia
 *  ENTRADA: numero de casos y cuantos son positivos
 *  REQUISITOS: Los dos deben ser numeros positivos y el numero de casos positivos debe ser menor
 * 				que el total de casos
 *  SALIDA: la entropia de la muestra de datos
 */
float calculaEntropia(int , int );

/*
 *  ACCION: devuelveUmbral
 *  ENTRADA: la tabla con los datos de los pacientes y el tamaño de la misma, ademas de la caracteristica que se esta estudiando
 *  REQUISITOS: ninguno
 *  SALIDA: el umbral y la entropia que tendria el conjunto de datos si lo dividieramos por ese umbral
 */
void devuelveUmbral(tablon* ,int ,int ,double *,float *,bool *);

/*
 *  ACCION: eligeUmbral
 *  ENTRADA: la tabla con los datos de los pacientes y el tamaño de la misma
 *  REQUISITOS: ninguno
 *  SALIDA: la mejor caracteristica y el umbral para dividir el arbol lo más hetereogeneamente posible
 */
void eligeCaracteristicaYUmbral(tablon* , int ,int* ,double *,bool* );

#endif
