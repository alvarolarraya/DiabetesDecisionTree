#include <stdlib.h>
#include "fragmenta.h"

char **fragmenta (const char* arg) {
    if (arg!=NULL){
        //se reserva memoria dos veces para hacer dos copias de la cadena original
        char* arg1=(char*)malloc(sizeof(char)*(strlen(arg)+1));
        strcpy(arg1, arg);
        char* arg2=(char*)malloc(sizeof(char)*(strlen(arg)+1));
        strcpy(arg2, arg);
        //tokeniza una de las copias de la cadena original siendo el caracter coma el delilmitador
        char* token=strtok(arg1, ",");
        //contamos el numero de fragmentos para luego darles valor
        int n=0;
        while( token != NULL ) {
            n++;  
            token=strtok(NULL, ",");
        }
        free(arg1);
        char** frags=(char**)malloc(sizeof(char*)*(n+1));
        token=strtok(arg2, ",");
        int i=0;
        while (i<n){
            if (strlen(token)>0) {
                frags[i]=(char*)malloc(sizeof(char)*(strlen(token)+1));  
                strcpy(frags[i], token);
                token=strtok(NULL, ",");
            }
            i++;
        }
        frags[n]=NULL;
        return frags;
    }
}
     
void borrarg(char **arg){
    int i=0;
    //haremos free de todos los elementos del apuntador doble
    while (arg[i]!=NULL){
        free(arg[i]);
        i++;
    }
}
