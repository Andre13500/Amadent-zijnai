#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulos.h"
#include "citas.h"
#include "utilidades.h"

extern char email_logueado[100];

void modulo_paciente() {
	int op;
	do {
		system("cls");
		printf("===================================\n");
		printf("        MODULO PACIENTE\n");
		printf("  Usuario: %s\n", email_logueado);
		printf("===================================\n");
		printf("1. Agendar nueva cita\n");
		printf("2. Ver mis citas\n");
		printf("3. Reagendar una cita\n");
		printf("4. Cancelar cita\n");
		printf("5. Volver al menu principal\n");
		printf("===================================\n");
		
		op = leer_opcion(1, 5);
		
		switch (op) {
		case 1:
			agendar_cita(email_logueado);
			break;
		case 2:
			mostrar_citas_paciente(email_logueado);
			break;
		case 3:
			reagendar_cita_paciente(email_logueado);
			break;
		case 4:
			cancelar_cita_paciente(email_logueado);
			break;
		case 5:
			printf("Volviendo al menu principal...\n");
			break;
		default:
			printf("Opcion no valida\n");
		}
		
		if (op != 5) {
			printf("\nPresione Enter para continuar...");
			while (getchar() != '\n');
			getchar();
		}
		
	} while (op != 5);
}

void modulo_medico() {
	int op;
	do {
		system("cls");
		printf("===================================\n");
		printf("         MODULO MEDICO\n");
		printf("  Usuario: %s\n", email_logueado);
		printf("===================================\n");
		printf("1. Ver mis citas asignadas\n");
		printf("2. Confirmar cita\n");
		printf("3. Marcar cita como completada\n");
		printf("4. Cancelar cita\n");
		printf("5. Volver al menu principal\n");
		printf("===================================\n");
		
		op = leer_opcion(1, 5);
		
		switch(op) {
		case 1:
			mostrar_citas_doctor(email_logueado);
			break;
		case 2:
			confirmar_cita_doctor(email_logueado);
			break;
		case 3:
			completar_cita_doctor(email_logueado);
			break;
		case 4:
			cancelar_cita_medico();
			break;
		case 5:
			printf("Volviendo al menu principal...\n");
			break;
		default:
			printf("Opcion no valida\n");
		}
		
		if (op != 5) {
			printf("\nPresione Enter para continuar...");
			while (getchar() != '\n');
			getchar();
		}
		
	} while (op != 5);
}

void modulo_administrador() {
	int op;
	do {
		system("cls");
		printf("===================================\n");
		printf("      MODULO ADMINISTRADOR\n");
		printf("  Usuario: %s\n", email_logueado);
		printf("===================================\n");
		printf("1. Ver todas las citas\n");
		printf("2. Cancelar cualquier cita\n");
		printf("3. Ver especialidades\n");
		printf("4. Gestionar doctores\n");
		printf("5. Ver doctores disponibles\n");
		printf("6. Volver al menu principal\n");
		printf("===================================\n");
		
		op = leer_opcion(1, 6);
		
		switch(op) {
		case 1:
			mostrar_todas_las_citas();
			break;
		case 2:
			cancelar_cita_admin();
			break;
		case 3:
			ver_especialidades();
			break;
		case 4:
			gestionar_doctores();
			break;
		case 5:
			ver_doctores_disponibles();
			break;
		case 6:
			printf("Volviendo al menu principal...\n");
			break;
		default:
			printf("Opcion no valida\n");
		}
		
		if (op != 6) {
			printf("\nPresione Enter para continuar...");
			while (getchar() != '\n');
			getchar();
		}
		
	} while (op != 6);
}

