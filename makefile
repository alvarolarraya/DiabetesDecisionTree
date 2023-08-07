all: compilaFragmenta compilaUmbral compilaMain ejecuta
compilaFragmenta:
	gcc -c fragmenta.c
compilaUmbral:
	gcc -c umbral.c
compilaMain:
	gcc csvprograma.c fragmenta.c umbral.c -o ejercicio -lm
clean:
	rm ejercicio hijoDerecho.csv hijoIzquierdo.csv registro.txt umbral.o fragmenta.o
ejecuta:
	./ejercicio data_randomized.csv
