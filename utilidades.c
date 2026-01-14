#include "utilidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void pausar() {
	printf("Presione Enter para continuar...");
	while (getchar() != '\n');
	getchar();
}

void continuar() {
	printf("Presione Enter para continuar...");
	while (getchar() != '\n');
	system("cls");
}

int validar_numeros(int comparador) {
	if (comparador != 1) {
		printf("Error: ¡Ingrese solo numeros!\n");
		while (getchar() != '\n');
		continuar();
		return 0;
	}
	return 1;
}

int leer_opcion(int min, int max) {
	int opcion;
	int resultado;
	
	while (1) {
		printf("Seleccione una opcion (%d - %d): ", min, max);
		resultado = scanf("%d", &opcion);
		
		if (!validar_numeros(resultado)) {
			continue;
		}
		
		while (getchar() != '\n');
		
		if (opcion < min || opcion > max) {
			printf("Error: Opcion fuera de rango (%d a %d).\n", min, max);
			continuar();
			continue;
		}
		
		return opcion;
	}
}

int deteccion_correo(char correo[]) {
	if (strchr(correo, '@') != NULL) {
		printf("Correo valido!!!\n");
		return 1;
	}
	
	printf("Ingrese un correo valido que contenga '@'!!!\n");
	continuar();
	return 0;
}

void salida() {
	printf("\nEl programa ha finalizado.\n");
	exit(0);
}

void limpiar_pantalla() {
	system("cls");
}




void verificar_archivos_sistema() {
	// Verificar y crear archivos esenciales si no existen
	
	// 1. doctores.txt (con algunos doctores de ejemplo si está vacío)
	FILE *f = fopen("doctores.txt", "a+");
	if (f) {
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		
		if (size == 0) {
			// Archivo vacío, agregar doctores de ejemplo
			fprintf(f, "1;Dr. Juan Pérez;Medicina General;Lunes-Viernes 8:00-16:00;1\n");
			fprintf(f, "2;Dra. María Gómez;Psicologia;Lunes-Jueves 9:00-17:00;1\n");
			fprintf(f, "3;Dr. Carlos López;Traumatologia;Martes-Viernes 10:00-18:00;1\n");
			printf("Archivo doctores.txt creado con ejemplos\n");
		}
		fclose(f);
	}
	
	// 2. medicos.txt (solo estructura si no existe)
	f = fopen("medicos.txt", "a");
	if (f) fclose(f);
	
	// 3. administradores.txt (solo estructura si no existe)
	f = fopen("administradores.txt", "a");
	if (f) fclose(f);
	
	// 4. citas.txt (solo estructura si no existe)
	f = fopen("citas.txt", "a");
	if (f) fclose(f);
}
