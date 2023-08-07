#include "umbral.h"

void eligeCaracteristicaYUmbral(tablon* tabla, int tamanioDatos,int* columnaElegida,double *umbralElegido,bool *hijoIzqTieneMasDiabeticos){
	if(tamanioDatos == 1)
	{
		perror("no se puede dividir un arbol con un elemento");
		return;
	}
	else
	{
		//a la entropia minima la inicializamos a 2 porque en nuestro 
		//conjunto de datos la entropia toma valores siempre menores que 2
		double umbral;
		float maxGananciaInfo = -1,gananciaInfo;
		//calculamos el mejor umbral por cada caracteristica del paciente
		//y de entre esos nos quedamos con el que mejor nos divida los datos
		for(int i = 1; i<9;i++)
		{
			devuelveUmbral(tabla,i,tamanioDatos,&umbral,&gananciaInfo,&(*hijoIzqTieneMasDiabeticos));
			if(gananciaInfo>maxGananciaInfo)
			{
				maxGananciaInfo = gananciaInfo;
				*umbralElegido = umbral;
				*columnaElegida = i;
			}
		}
	}
}

void devuelveUmbral(tablon* tabla,int columnaAnalizar,int tamanioDatos,double *umbralElegido,float *maxGananciaInfo,bool *hijoIzqTieneMasDiabeticos){
    int i,casosSi = 0,tamanioHijoDerecho = 0,contadorCasosSiIzquierdo = 0,contadorCasosSiDerecho = 0,tamanioHijosIzquierdo = 0;
    tablon aux;
    float gananciaInfo,entropiaActual,entropiaDividiendoAtributo,entropiaHijoDerecho = 10,entropiaHijoIzquierdo = 10;
    double umbral,umbralAnterior = -1;
    *maxGananciaInfo = -1;
    *hijoIzqTieneMasDiabeticos = false;
    //ordenamos primero los datos
    switch (columnaAnalizar)
	{
		case 1:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].age>tabla[j].age){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].age + tabla[i].age)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].age < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		case 2:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].bmi>tabla[j].bmi){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].bmi + tabla[i].bmi)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].bmi < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		case 3:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].bloodpressure>tabla[j].bloodpressure){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].bloodpressure + tabla[i].bloodpressure)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].bloodpressure < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		case 4:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].glucose>tabla[j].glucose){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].glucose + tabla[i].glucose)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].glucose < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		case 5:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].pedigree>tabla[j].pedigree){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].pedigree + tabla[i].pedigree)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].pedigree < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		case 6:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].insuline>tabla[j].insuline){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].insuline + tabla[i].insuline)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].insuline < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		case 7:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].pregnancies>tabla[j].pregnancies){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].pregnancies + tabla[i].pregnancies)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].pregnancies < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		case 8:
			//ordenamos el conjunto de datos
			for(i=0;i<tamanioDatos-1;i++){
				for(int j = tamanioDatos-1;j>=i+1;j--){
					if(tabla[j-1].skinthickness>tabla[j].skinthickness){
						aux = tabla[j-1];
						tabla[j-1] = tabla[j];
						tabla[j] = aux;
					}
				}
			}
			for(int j=0;j<tamanioDatos;j++)
			{
				if(tabla[j].siono)
					casosSi++;
			}
			entropiaActual = calculaEntropia(casosSi,tamanioDatos);
			//calculamos el ratio de ganancia de informacion, actualizamos el maximo ratio de ganancia hasta ahora
			//y el umbral con el que lo conseguimos
			for(i=1;i<tamanioDatos;i++)
			{
				if(tabla[i-1].siono != tabla[i].siono)
				{
					umbral = (float)(tabla[i-1].skinthickness + tabla[i].skinthickness)/2;
					if(umbral != umbralAnterior)
					{
						for(int j=0;j<tamanioDatos;j++)
						{
							if(tabla[j].skinthickness < umbral)
							{
								tamanioHijosIzquierdo++;
								if(tabla[j].siono)
									contadorCasosSiIzquierdo++;
							}
							else
							{
								tamanioHijoDerecho++;
								if(tabla[j].siono)
									contadorCasosSiDerecho++;
							}
						}
						if(tamanioHijoDerecho != 0)
							entropiaHijoDerecho = calculaEntropia(contadorCasosSiDerecho, tamanioHijoDerecho);
						if(tamanioHijosIzquierdo != 0)
							entropiaHijoIzquierdo = calculaEntropia(contadorCasosSiIzquierdo,tamanioHijosIzquierdo);
						entropiaDividiendoAtributo = (((float)tamanioHijosIzquierdo/tamanioDatos)*entropiaHijoIzquierdo) + (((float)tamanioHijoDerecho/tamanioDatos)*entropiaHijoDerecho);
						gananciaInfo = entropiaActual - entropiaDividiendoAtributo;
						if(gananciaInfo>*maxGananciaInfo){
							*maxGananciaInfo = gananciaInfo;
							*umbralElegido = umbral;
						}
						umbralAnterior = umbral;
						if(contadorCasosSiIzquierdo > contadorCasosSiDerecho)
							*hijoIzqTieneMasDiabeticos = true;
						contadorCasosSiDerecho = 0;
						contadorCasosSiIzquierdo = 0;
						tamanioHijosIzquierdo = 0;
						tamanioHijoDerecho = 0;
					}
				}
			}
			break;
		default:
			perror("la columna no existe");
	}
}

float calculaEntropia(int casosSi, int cuantosHay){
    float entropia;
    int casosNo = cuantosHay - casosSi;
    if(casosSi != 0 && casosNo!= 0)
		entropia = ((float)casosSi/cuantosHay)*(-((float)logf((float)casosSi/cuantosHay)/logf(2))) + ((float)casosNo/cuantosHay)*(-((float)logf((float)casosNo/cuantosHay)/logf(2)));
	else
		entropia = 10;
    return entropia;
}
