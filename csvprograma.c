/*
 *	MODULO: csvprograma
 *	Creado por David Perez, Alex Salvatierra, Alejandro Martinez y Alvaro Larraya el 28/11/20.
 *  DESCRIPCION: Este modulo es el programa principal que recibira una tabla, escribira en un fichero registro.txt
 *               las decisiones del arbol de decision y calculara el porcentaje de acierto del mismo
 */

#include <unistd.h>
#include <stdbool.h>
#include "umbral.h"
#include "fragmenta.h"

#define MAXCHAR 14

int size, izq, dch, nivelArbol=-1;
struct tablon* tIzq;
struct tablon* tDch;

typedef struct hijos{
    struct tablon* tIzq;
    struct tablon* tDch;
} hijos;

/*
 *  FUNCION: tSize
 *  ENTRADA: apuntador a tablon
 *  SALIDA: el tamaño del tablon
 */
int tSize (tablon* tabla){
    int i=0;
    while (tabla[i].age>0){
        i++;
    }
    return i;
}

/*
 *  FUNCION: fSize
 *  ENTRADA: el nombre de un fichero
 *  SALIDA: el tamaño (lineas) del fichero
 */
int fSize (char* fichero){
    FILE *f;
    char str[140];
    f=fopen(fichero, "r");
    int i=0;
    fgets(str, 140, f);
    while (feof(f)==0)
    {
        fgets(str, 140, f);
        i++;
    }
    fclose(f);
    return (i-1);
}

/*
 *  FUNCION: datosEntero
 *  ENTRADA: apuntador a tablon y el nombre de un fichero
 *  SALIDA: rellena toda la informacion de la tabla con la informacion del fichero .csv
 */
void datosEntero(tablon* tabla, char* fichero){
    FILE* f;
    int size=fSize(fichero);
    f=fopen(fichero, "r");
    char* input = (char*)malloc(sizeof(char)*10000);
    int i=0;
    fgets(input, 140, f);
    while (i<size){
        fgets(input, 140, f);
        char** arg = fragmenta(input);    
        tabla[i].pregnancies=atof(arg[0]);
        tabla[i].glucose=atof(arg[1]);
        tabla[i].bloodpressure=atof(arg[2]);
        tabla[i].skinthickness=atof(arg[3]);
        tabla[i].insuline=atof(arg[4]);
        tabla[i].bmi=atof(arg[5]);
        tabla[i].pedigree=atof(arg[6]);
        tabla[i].age=atof(arg[7]);
        tabla[i].siono=atof(arg[8]);
        i++;
    }
    tabla[i].age=-1;
    fclose(f);
    free(input);
}

/*
 *  FUNCION: porcPos
 *  ENTRADA: apuntador a tablon
 *  SALIDA: el porcentaje de positivos (diabeticos) de una tabla
 */
double porcPos(tablon* tabla){
    int nPos=0;
    for (int i=0; i<tSize(tabla); i++){
        if (tabla[i].siono==1){
            nPos++;
        }
    }
    return (double)((double)nPos/(double)tSize(tabla));
}

/*
 *  FUNCION: dividir2
 *  ENTRADA: el dato por el que se dividira la columna original, la columna por la que se divira, la tabla a dividir, el tamaño de la tabla y un entero que identificara que columna es la que divide 
 *  SALIDA: un struct hijos con los dos hijos resultantes de dividir la tabla
 */
struct hijos dividir2 (double dato, double* generadora, tablon* tabla, int size, int col){
    struct hijos hijos;
    //solo se dividira si el dato a dividir es distinto a 0 ya que en ese caso no seria relevante la division
    if (dato!=0){
        izq=0;
        dch=0;
        //accedemos al dato de la columna por la que se dividira la tabla por su posicion de memoria y dependiendo de su valor incrementaremos el tamaño bien de la tabla resultante izquierda (si el dato de la tabla es menor al dato por el que se dividira, el umbral) o derecha (viceversa)
        for (int i=0; i<size; i++){
            if (*((generadora)+i*sizeof(double)+i)<dato){
                izq++;
            }
            else{
                dch++;
            }
        }
        //reservamos memoria de las tablas resultantes sabiendo su tamaño
        tIzq=(tablon*)calloc(izq+1, sizeof(struct tablon));
        tDch=(tablon*)calloc(dch+1, sizeof(struct tablon));
        int l=0,r=0;
        for (int i=0; i<size; i++){
            if (*((generadora)+i*sizeof(double)+i)<dato){
                tIzq[l]=tabla[i];
                l++;
            }
            else{
                tDch[r]=tabla[i];
                r++;
            }
        }
        //se reserva una unidad mas de memoria (struct tablon) para las tablas resultantes ya que al final de cada tabla almacenaremos una edad negativa (imposible). Informacion con la cual en cualquier momento podremos saber el tamaño de la tabla llamando a una funcion que hace uso de esta informacion
        tIzq[l].age=-1;
        tDch[r].age=-1;
        
        //se crean los archivos .csv hijoIzquierdo e hijoDerecho de las tablas resultantes
        FILE *f;
        f=fopen("hijoIzquierdo.csv", "w");
        fprintf(f, "pregnancies,glucose,bloodpressure,skinthickness,insuline,bmi,pedigree,age,clase\n");
        for (int i=0; i<l;i++){
            fprintf(f,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n", tIzq[i].pregnancies, tIzq[i].glucose, tIzq[i].bloodpressure, tIzq[i].skinthickness, tIzq[i].insuline, tIzq[i].bmi, tIzq[i].pedigree,tIzq[i].age, tIzq[i].siono);
        }
        fclose(f);
        f=fopen("hijoDerecho.csv", "w");
        fprintf(f, "pregnancies,glucose,bloodpressure,skinthickness,insuline,bmi,pedigree,age,clase\n");
        for (int i=0; i<r;i++){
            fprintf(f,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n", tDch[i].pregnancies, tDch[i].glucose, tDch[i].bloodpressure, tDch[i].skinthickness, tDch[i].insuline, tDch[i].bmi, tDch[i].pedigree,tDch[i].age, tDch[i].siono);
            
        }
        fclose(f);
        hijos.tIzq=tIzq;
        hijos.tDch=tDch;
        //se almacena informacion adicional en el registro cada vez que se realiza una division
        f=fopen("registro.txt", "a");
        fprintf(f,"\n\nINFO DE MI:\nMi tamaño es: %d\nMi porcentaje de positivos: %f\nVoy a dividir por la columna %d con umbral %f\nINFO HIJO IZQUIERDO:\nTamaño: %d\nPorcentaje positivos: %f\nINFO HIJO DERECHO:\nTamaño: %d\nPorcentaje positivos: %f\n", tSize(tabla), porcPos(tabla), col, dato, izq, porcPos(hijos.tIzq), dch, porcPos(hijos.tDch));
        fclose(f);
        //se devuelve el struct hijos con las dos tablas que resultaron de la divison por la columna indicada en el umbral indicado
        return hijos;
    }
}

/*
 *  FUNCION: dividir
 *  ENTRADA: un entero que identifica la columna por la que se quiere dividir la tabla, el valor por el que se dividira la tabla, la tabla y el tamaño de ella
 *  SALIDA: llama a la funcion que dividira la tabla y devolvera su valor, esta funcion actua como intermediaria
 */
struct hijos dividir(int nTabla, double dato, tablon* tabla, int size){
    if (nTabla==1){ //age
        return dividir2(dato, &(tabla->age), tabla, size, nTabla);
    }
    else if (nTabla==2){ //bmi
        return dividir2(dato, &(tabla->bmi), tabla, size, nTabla);
    }
    else if (nTabla==3){ //bloodpressure
        return dividir2(dato, &(tabla->bloodpressure), tabla, size, nTabla);
    }
    else if (nTabla==4){ //glucose
        return dividir2(dato, &(tabla->glucose), tabla, size, nTabla);
    }
    else if (nTabla==5){ //pedigree
        return dividir2(dato, &(tabla->pedigree), tabla, size, nTabla);
    }
    else if (nTabla==6){ //insuline
        return dividir2(dato, &(tabla->insuline), tabla, size, nTabla);
    }
    else if (nTabla==7){ //pregnancies
        return dividir2(dato, &(tabla->pregnancies), tabla, size, nTabla);
    }
    else if (nTabla==8){ //skinthickness
        return dividir2(dato, &(tabla->skinthickness), tabla, size, nTabla);
    }
    else{
        perror("la columna por la que quieres dividir el arbol no existe");
    }
}

/*
 *  FUNCION: fRecur
 *  ENTRADA: la tabla con la informacion para hacer el arbol, un entero que identifica si es hijo izquierdo, otro que identifica la columna por la que su padre dividio, un double que identifica el valor (umbral)
 *           por el que su padre dividio, un entero para saber el nivel del arbol y otro entero tamaño del padre, es decir, informacion para identificar un nodo del arbol
 *  SALIDA: es una funcion recursiva que escribira en el fichero registro.txt la informacion de todos los nodos del arbol de decision
 */
void fRecur (tablon* tabla, int soyHijoIzquierdo, int columnaPadre, double umbralPadre, int nivel, int tamanoPadre){
    //se abre con flag append para sobreescribir el fichero registro sobre el que quedara, valga la redundancia, el registro de nodos con sus decisiones del arbol de decision
    FILE* f;
    f=fopen("registro.txt", "a");
    double umbralElegido;
    int columnaElegida;
    bool hijoIzqTieneMasDiabeticos;
    //cada vez que se entre a la funcion recursiva se almacenara informacion en el fichero registro para identificar el nodo
    fprintf(f, "\n\nEstoy en el nivel %d y soy hijo %d (izquierdo=1, derecho=0) de una tabla con tamano %d que dividio por el umbral %f en la columna %d\n", nivel, soyHijoIzquierdo, tamanoPadre, umbralPadre, columnaPadre);
    
    //se divide y almacena en el struct hijo los hijos resultantes de dividir segun los parametros optimos (columna y umbral) la tabla recibida como parametro de la funcion fRecur
    struct hijos hijo;
    eligeCaracteristicaYUmbral(tabla,tSize(tabla),&columnaElegida,&umbralElegido,&hijoIzqTieneMasDiabeticos);
    hijo=dividir(columnaElegida, umbralElegido, tabla, tSize(tabla));
    
    //se declarara como hoja aquel nodo cuya decision devuelva hijos con tamaño menor o igual a 24 o un porcentaje ambiguo con el que no podamos predecir su clase, porcentaje que establecemos entre el 10% y el 79%
    //bien vaya a ser hoja uno de los hijos o no, se va a registrar toda informacion en el fichero registro para posteriormente dibujar el arbol de decision
    
    if (porcPos(hijo.tIzq)>0.1 && porcPos(hijo.tIzq)<0.79 && tSize(hijo.tIzq)>24){
        fprintf(f, "Estoy en el nivel %d y voy a tener hijo izquierdo dividiendo en columna %d con umbral %f.\nMi tamaño es %d y el de mi hijo izquierdo %d\nMi porcentaje de positivos es %f y el de mi hijo izquierdo %f\n", nivel, columnaElegida, umbralElegido,tSize(tabla), tSize(hijo.tIzq), porcPos(tabla), porcPos(hijo.tIzq));
        fRecur(hijo.tIzq, 1, columnaElegida, umbralElegido, nivel+1, tSize(hijo.tIzq));
    }
    else if (porcPos(hijo.tIzq)<0.1 || porcPos(hijo.tIzq)>0.79 || tSize(hijo.tIzq)<24){
        fprintf(f, "\n\nAVISO HOJA: Estoy en el nivel %d y mi hijo izquierdo ser una hoja.\nMi tamaño es %d y el de mi hijo izquierdo %d\nMi porcentaje de positivos es %f y el de mi hijo izquierdo %f\n", nivel,tSize(tabla), tSize(hijo.tIzq), porcPos(tabla), porcPos(hijo.tIzq));
    }
    if (porcPos(hijo.tDch)>0.1 && porcPos(hijo.tDch)<0.79 && tSize(hijo.tDch)>24){
        fprintf(f, "Estoy en el nivel %d y voy a tener hijo derecho dividiendo en columna %d con umbral %f.\nMi tamaño es %d y el de mi hijo derecho %d\nMi porcentaje de positivos es %f y el de mi hijo derecho %f\n", nivel, columnaElegida, umbralElegido,tSize(tabla), tSize(hijo.tDch), porcPos(tabla), porcPos(hijo.tDch));
        fRecur(hijo.tDch, 0, columnaElegida, umbralElegido, nivel+1, tSize(hijo.tDch));
    }
    else if (porcPos(hijo.tDch)<0.1 || porcPos(hijo.tDch)>0.79 || tSize(hijo.tDch)<24){
        fprintf(f, "\n\nAVISO HOJA: Estoy en el nivel %d y mi hijo derecho ser una hoja.\nMi tamaño es %d y el de mi hijo derecho %d\nMi porcentaje de positivos es %f y el de mi hijo derecho %f\n", nivel,tSize(tabla), tSize(hijo.tIzq), porcPos(tabla), porcPos(hijo.tIzq));
    }
    //como informacion adicional para dibujar el arbol, en la variable nivelArbol se guardara el tamaño del arbol
    if (nivel>nivelArbol){
        nivelArbol=nivel;
    }
    fclose(f);
}

/*
 *  FUNCION: testeo
 *  ENTRADA: el nombre de un fichero con datos a testear
 *  SALIDA: el porcentaje de acierto del arbol de decision que se creo previamente a base de ifs anidados
 */
double testeo (char* fichero){ 
    double porcAcierto;
    int aciertos=0;
    //se obtiene el tamaño del fichero que contiene los datos a testear para reservar la memoria justa
    int sizeTesteo=fSize(fichero);
    //se reserva dicha memoria
    tablon* tt=(tablon*)calloc(sizeTesteo, sizeof(struct tablon));
    //se rellena la tabla con informacion del fichero que contiene los datos a testear
    datosEntero(tt, fichero);
    //se ira juzgando cada dato y su clase (siono) segun las decisiones del arbol de decision previamente creado, incrementando el numero de aciertos cuando se llegue a un nodo hoja y la clase del dato concuerde con la prevista
    for (int i=0; i<sizeTesteo; i++){       
        if (tt[i].glucose<127.5){
            if (tt[i].bmi<26.45){
                if (tt[i].siono==0){
                    aciertos++;
                }
            }
            else{
                if (tt[i].age<29){
                    if (tt[i].bmi<31){
                        if (tt[i].siono==0){
                            aciertos++;
                        }
                    }
                    else{
                        if (tt[i].insuline<99.5){
                            if (tt[i].bloodpressure<55){
                                if (tt[i].siono==1){
                                    aciertos++;
                                }
                            }
                            else{
                                if(tt[i].glucose<103){
                                    if (tt[i].bloodpressure<81){
                                        if (tt[i].siono==0){
                                            aciertos++;
                                        }
                                    }
                                    else{
                                        if (tt[i].siono==1){
                                            aciertos++;
                                        }
                                    }
                                }
                                else{
                                    if (tt[i].siono==0){
                                        aciertos++;
                                    }
                                }
                            }
                        }
                        else{
                            if (tt[i].siono==0){
                                aciertos++;
                            }
                        }
                    }
                }
                else{
                    if(tt[i].pedigree<0.625){
                        if (tt[i].glucose<100){
                            if (tt[i].siono==0){
                                aciertos++;
                            }
                        }
                        else{
                            if(tt[i].pedigree<0.2){
                                if (tt[i].siono==0){
                                    aciertos++;
                                }
                            }
                            else{
                                if(tt[i].bloodpressure<67){
                                    if (tt[i].siono==1){
                                        aciertos++;
                                    }
                                }
                                else{
                                    if(tt[i].bmi<31.6){
                                        if (tt[i].siono==1){
                                            aciertos++;
                                        }
                                    }
                                    else{
                                        if (tt[i].siono==0){
                                            aciertos++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else{
                        if(tt[i].pregnancies<7.5){
                            if (tt[i].siono==1){
                                aciertos++;
                            }
                        }
                        else{
                            if (tt[i].siono==1){
                                aciertos++;
                            }
                        }
                    }
                }
            }
        }
        else{
            if(tt[i].bmi<29.95){
                if(tt[i].glucose<154.5){
                    if(tt[i].pedigree<0.35){
                        if (tt[i].siono==0){
                            aciertos++;
                        }
                    }
                    else{
                        if (tt[i].siono==0){
                            aciertos++;
                        }
                    }
                }
                else{
                    if (tt[i].siono==1){
                        aciertos++;
                    }
                }
            }
            else{
                if(tt[i].glucose<165.5){
                    if(tt[i].age<24.5){
                        if (tt[i].siono==0){
                            aciertos++;
                        }
                    }
                    else{
                        if(tt[i].pedigree<0.726){
                            if(tt[i].bloodpressure<73){
                                if (tt[i].siono==1){
                                    aciertos++;
                                }
                            }
                            else{
                                if(tt[i].age<41){
                                    if(tt[i].bmi<42.2){
                                        if (tt[i].siono==0){
                                            aciertos++;
                                        }
                                    }
                                    else{
                                        if (tt[i].siono==1){
                                            aciertos++;
                                        }
                                    }
                                }
                                else{
                                    if (tt[i].siono==1){
                                        aciertos++;
                                    }
                                }
                            }
                        }
                        else{
                            if (tt[i].siono==1){
                                aciertos++;
                            }
                        }
                    }
                }
                else{
                    if (tt[i].siono==1){
                        aciertos++;
                    }
                }
            }
        }
    }
    //el numero de aciertos entre el numero total de datos testeados nos dara una proporcion de acierto
    return (double)aciertos/(double)sizeTesteo;
}



int main (int argc, char* argv[])
{
    //se obtiene el tamaño del fichero que contiene los datos para reservar la memoria justa
    int size=fSize(argv[1]);
    //se reserva dicha memoria
    tablon* tabla=(tablon*)calloc(size, sizeof(struct tablon));
    //se rellena la tabla con informacion del fichero que contiene los datos
    datosEntero(tabla, argv[1]);
    
    //abre y cierra el fichero en modo escritura para que cada ejecucion empiece con el fichero de resgistro vacio
    FILE* f;
    f=fopen("registro.txt", "w");
    fclose(f);  
    
    //se inicializan los parametros para realizar la primera llamada a la funcion recursiva
    int columnaElegida=0;
    double umbralElegido=0;
    fRecur(tabla, -1, columnaElegida, umbralElegido, 1, tSize(tabla));
    
    //se devuelve por pantalla el porcentaje de acierto del arbol de decision redondeado
    float porcentaje=(float)testeo("test.csv");
    int Rporcentaje=(int)((roundf(porcentaje * 100) / 100)*100);
    printf("El porcentaje de acierto es de: %d porciento\n", Rporcentaje);    
    
    free(tabla);
    return 0;
}   





