#include <stdio.h>
#include <stdlib.h>
#include "citas.h"
#include "utilidades.h"

/* Verifica si una hora está ocupada */
int hora_ocupada(int mes, int dia, int hora) {
	FILE *f = fopen("citas.dat", "rb");
	Cita c;
	
	if (!f) return 0;
	
	while (fread(&c, sizeof(Cita), 1, f)) {
		if (c.mes == mes && c.dia == dia && c.hora == hora) {
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}

/* Verifica si el día está lleno */
int dia_lleno(int mes, int dia) {
	int ocupadas = 0;
	FILE *f = fopen("citas.dat", "rb");
	Cita c;
	
	if (!f) return 0;
	
	while (fread(&c, sizeof(Cita), 1, f)) {
		if (c.mes == mes && c.dia == dia &&
			c.hora >= HORA_INICIO && c.hora <= HORA_FIN) {
			ocupadas++;
		}
	}
	fclose(f);
	
	return ocupadas >= (HORA_FIN - HORA_INICIO + 1);
}

/* Agregar cita */
void agregar_cita(int id_paciente) {
	Cita nueva;
	FILE *f;
	
	nueva.id_paciente = id_paciente;
	
	while (1) {
		system("cls");
		printf("=== AGENDAR CITA ===\n");
		
		printf("Mes (1-12): ");
		scanf("%d", &nueva.mes);
		
		printf("Día: ");
		scanf("%d", &nueva.dia);
		
		if (dia_lleno(nueva.mes, nueva.dia)) {
			printf("\nEse día está completamente ocupado.\n");
			continuar();
			continue;
		}
		
		printf("Hora (7 a 14): ");
		scanf("%d", &nueva.hora);
		limpiar_buffer_entrada();
		
		if (nueva.hora < HORA_INICIO || nueva.hora > HORA_FIN) {
			printf("\nHorario inválido.\n");
			continuar();
			continue;
		}
		
		if (hora_ocupada(nueva.mes, nueva.dia, nueva.hora)) {
			printf("\nEsa hora ya está ocupada.\n");
			continuar();
			continue;
		}
		
		break;
	}
	
	f = fopen("citas.dat", "ab");
	if (!f) {
		printf("Error al guardar la cita.\n");
		continuar();
		return;
	}
	
	fwrite(&nueva, sizeof(Cita), 1, f);
	fclose(f);
	
	printf("\nCita registrada exitosamente.\n");
	continuar();
}

/* Menú de citas */
void menu_citas(int id_paciente) {
	int opcion;
	
	do {
		system("cls");
		printf("=== MENÚ DE CITAS ===\n");
		printf("1. Agendar cita\n");
		printf("2. Volver\n");
		printf("Seleccione: ");
		scanf("%d", &opcion);
		limpiar_buffer_entrada();
		
		switch (opcion) {
		case 1:
			agregar_cita(id_paciente);
			break;
		}
	} while (opcion != 2);
}
