#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "citas.h"
#include "utilidades.h"

#define ARCHIVO_CITAS "citas.txt"
#define ARCHIVO_DOCTORES "doctores.txt"
#define ARCHIVO_REGISTRO "registro.txt"

// ============================================
// DECLARACIONES DE FUNCIONES INTERNAS (PROTOTIPOS)
// ============================================

// Funciones auxiliares
void obtener_nombre_paciente(const char *email, char *nombre_destino);
void obtener_nombre_doctor(int doctor_id, char *nombre_destino);
int obtener_id_doctor_por_email(const char *email);
int generar_id_cita();
void mostrar_doctores_por_especialidad(int especialidad_id);
char* obtener_especialidad_por_id(int id);
int validar_fecha(const char *fecha);
int validar_hora(const char *hora);
// ============================================
// FUNCIONES AUXILIARES (INTERNAS)
// ============================================
void agregar_nuevo_doctor();
void modificar_disponibilidad_doctor();
void cancelar_cita_medico() ;
// Obtener nombre del paciente desde registro.txt
void obtener_nombre_paciente(const char *email, char *nombre_destino) {
	FILE *f = fopen(ARCHIVO_REGISTRO, "r");
	char linea[300];
	char nombre[100], email_arch[100];
	
	strcpy(nombre_destino, "Desconocido");
	
	if (f) {
		while (fgets(linea, sizeof(linea), f)) {
			if (sscanf(linea, "%[^;];%[^;]", nombre, email_arch) == 2) {
				if (strcmp(email, email_arch) == 0) {
					strcpy(nombre_destino, nombre);
					break;
				}
			}
		}
		fclose(f);
	}
}

// Obtener nombre del doctor por ID
void obtener_nombre_doctor(int doctor_id, char *nombre_destino) {
	FILE *f = fopen(ARCHIVO_DOCTORES, "r");
	char linea[200];
	int id;
	char nombre[100], especialidad[50], horario[50];
	int disponible;
	
	strcpy(nombre_destino, "Doctor no encontrado");
	
	if (f) {
		while (fgets(linea, sizeof(linea), f)) {
			if (sscanf(linea, "%d;%[^;];%[^;];%[^;];%d", 
					   &id, nombre, especialidad, horario, &disponible) == 5) {
				if (id == doctor_id) {
					strcpy(nombre_destino, nombre);
					break;
				}
			}
		}
		fclose(f);
	}
}

// Obtener ID del doctor por email (para médicos)
int obtener_id_doctor_por_email(const char *email) {
	FILE *f = fopen(ARCHIVO_REGISTRO, "r");
	char linea[300];
	char nombre[100], email_arch[100], contrasena[100];
	int edad, tipo_usuario;
	char cedula[20];
	
	if (f) {
		while (fgets(linea, sizeof(linea), f)) {
			if (sscanf(linea, "%[^;];%[^;];%[^;];%d;%[^;];%d",
					   nombre, email_arch, contrasena, &edad, cedula, &tipo_usuario) == 6) {
				if (strcmp(email, email_arch) == 0 && tipo_usuario == 2) {
					fclose(f);
					// Buscar en doctores.txt el doctor con este nombre
					FILE *f_doctores = fopen(ARCHIVO_DOCTORES, "r");
					char linea_doc[200];
					int id_doc;
					char nombre_doc[100], especialidad[50], horario[50];
					int disponible;
					
					if (f_doctores) {
						while (fgets(linea_doc, sizeof(linea_doc), f_doctores)) {
							if (sscanf(linea_doc, "%d;%[^;];%[^;];%[^;];%d",
									   &id_doc, nombre_doc, especialidad, horario, &disponible) == 5) {
								if (strstr(nombre_doc, nombre) != NULL) {
									fclose(f_doctores);
									return id_doc;
								}
							}
						}
						fclose(f_doctores);
					}
				}
			}
		}
		fclose(f);
	}
	return -1; // No encontrado
}

// Generar ID único para cita
int generar_id_cita() {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	int id = 1;
	char linea[300];
	
	if (f) {
		while (fgets(linea, sizeof(linea), f)) {
			id++;
		}
		fclose(f);
	}
	return id;
}

// Mostrar doctores por especialidad
void mostrar_doctores_por_especialidad(int especialidad_id) {
	FILE *f = fopen(ARCHIVO_DOCTORES, "r");
	char linea[200];
	char *nombre_especialidad = "";
	
	// Determinar nombre de especialidad
	switch(especialidad_id) {
	case 1: nombre_especialidad = "Medicina General"; break;
	case 2: nombre_especialidad = "Psicologia"; break;
	case 3: nombre_especialidad = "Traumatologia"; break;
	}
	
	printf("\n=== DOCTORES DE %s ===\n", nombre_especialidad);
	printf("ID | Nombre              | Horario              | Disponible\n");
	printf("---|---------------------|----------------------|------------\n");
	
	if (!f) {
		printf("No hay doctores registrados\n");
		return;
	}
	
	while (fgets(linea, sizeof(linea), f)) {
		int id;
		char nombre[100], especialidad[50], horario[50];
		int disponible;
		
		if (sscanf(linea, "%d;%[^;];%[^;];%[^;];%d", 
				   &id, nombre, especialidad, horario, &disponible) == 5) {
			
			if (strcmp(especialidad, nombre_especialidad) == 0) {
				printf("%-2d | %-19s | %-20s | %s\n",
					   id, nombre, horario, 
					   disponible ? "Si" : "No");
			}
		}
	}
	
	fclose(f);
}

// Obtener especialidad por ID
char* obtener_especialidad_por_id(int id) {
	switch(id) {
	case 1: return "Medicina General";
	case 2: return "Psicologia";
	case 3: return "Traumatologia";
	default: return "Desconocida";
	}
}

// Validar formato de fecha DD/MM/AAAA
int validar_fecha(const char *fecha) {
	if (strlen(fecha) != 10) return 0;
	if (fecha[2] != '/' || fecha[5] != '/') return 0;
	
	// Verificar que sean números
	for (int i = 0; i < 10; i++) {
		if (i == 2 || i == 5) continue;
		if (!isdigit(fecha[i])) return 0;
	}
	
	return 1;
}

// Validar formato de hora HH:MM
int validar_hora(const char *hora) {
	if (strlen(hora) != 5) return 0;
	if (hora[2] != ':') return 0;
	
	// Verificar que sean números
	for (int i = 0; i < 5; i++) {
		if (i == 2) continue;
		if (!isdigit(hora[i])) return 0;
	}
	
	// Verificar horas válidas (0-23) y minutos (0-59)
	int hh = (hora[0] - '0') * 10 + (hora[1] - '0');
	int mm = (hora[3] - '0') * 10 + (hora[4] - '0');
	
	if (hh < 0 || hh > 23) return 0;
	if (mm < 0 || mm > 59) return 0;
	
	return 1;
}

// ============================================
// FUNCIONES PÚBLICAS PARA PACIENTES
// ============================================

void ver_especialidades() {
	system("cls");
	printf("=== ESPECIALIDADES DISPONIBLES ===\n");
	printf("1. Medicina General\n");
	printf("2. Psicologia\n");
	printf("3. Traumatologia\n");
}

void agendar_cita(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "a");
	int esp, doctor_id;
	char fecha[20], hora[10], motivo[200];
	char paciente_nombre[100];
	char doctor_nombre[100];
	char especialidad_nombre[50];
	
	if (!f) {
		printf("Error al abrir archivo de citas\n");
		pausar();
		return;
	}
	
	system("cls");
	printf("=== AGENDAR NUEVA CITA ===\n\n");
	
	// Obtener nombre del paciente
	obtener_nombre_paciente(email, paciente_nombre);
	printf("Paciente: %s\n", paciente_nombre);
	printf("Email: %s\n\n", email);
	
	// Seleccionar especialidad
	ver_especialidades();
	printf("\nSeleccione la especialidad (1-3): ");
	esp = leer_opcion(1, 3);
	
	// Asignar nombre de especialidad
	strcpy(especialidad_nombre, obtener_especialidad_por_id(esp));
	
	// Mostrar doctores disponibles para esa especialidad
	mostrar_doctores_por_especialidad(esp);
	
	printf("\nIngrese el ID del doctor: ");
	scanf("%d", &doctor_id);
	while (getchar() != '\n'); // Limpiar buffer
	
	// Verificar si el doctor existe
	obtener_nombre_doctor(doctor_id, doctor_nombre);
	if (strcmp(doctor_nombre, "Doctor no encontrado") == 0) {
		printf("\nError: Doctor no encontrado\n");
		fclose(f);
		pausar();
		return;
	}
	
	// Solicitar fecha (con validación)
	do {
		printf("\nIngrese fecha (DD/MM/AAAA): ");
		fgets(fecha, sizeof(fecha), stdin);
		fecha[strcspn(fecha, "\n")] = 0;
		
		if (!validar_fecha(fecha)) {
			printf("Formato de fecha incorrecto. Use DD/MM/AAAA\n");
		}
	} while (!validar_fecha(fecha));
	
	// Solicitar hora (con validación)
	do {
		printf("Ingrese hora (HH:MM, 24h): ");
		fgets(hora, sizeof(hora), stdin);
		hora[strcspn(hora, "\n")] = 0;
		
		if (!validar_hora(hora)) {
			printf("Formato de hora incorrecto. Use HH:MM (0-23:0-59)\n");
		}
	} while (!validar_hora(hora));
	
	// Solicitar motivo
	printf("Ingrese motivo de la cita: ");
	fgets(motivo, sizeof(motivo), stdin);
	motivo[strcspn(motivo, "\n")] = 0;
	
	// Generar ID y guardar cita
	int id = generar_id_cita();
	
	// Formato: id;email_paciente;nombre_paciente;id_doctor;nombre_doctor;especialidad;fecha;hora;motivo;estado;activa
	fprintf(f, "%d;%s;%s;%d;%s;%s;%s;%s;%s;pendiente;1\n", 
			id, email, paciente_nombre, doctor_id, doctor_nombre, 
			especialidad_nombre, fecha, hora, motivo);
	
	fclose(f);
	
	printf("\n========================================\n");
	printf("       CITA AGENDADA EXITOSAMENTE\n");
	printf("========================================\n");
	printf("ID de cita: %d\n", id);
	printf("Doctor: %s\n", doctor_nombre);
	printf("Especialidad: %s\n", especialidad_nombre);
	printf("Fecha: %s  Hora: %s\n", fecha, hora);
	printf("========================================\n");
	pausar();
}

void mostrar_citas_paciente(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	char linea[300];
	int encontradas = 0;
	char paciente_nombre[100];
	
	if (!f) {
		printf("No hay citas registradas\n");
		pausar();
		return;
	}
	
	system("cls");
	
	// Obtener nombre del paciente
	obtener_nombre_paciente(email, paciente_nombre);
	printf("=== CITAS DE %s ===\n\n", paciente_nombre);
	
	printf("ID  | Doctor             | Especialidad       | Fecha      | Hora   | Estado\n");
	printf("----|--------------------|--------------------|------------|--------|--------\n");
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, doctor_id, activa;
		char paciente_email[100], nombre_paciente[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		// Leer con el nuevo formato
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, nombre_paciente, &doctor_id, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (strcmp(email, paciente_email) == 0 && activa == 1) {
				printf("%-3d | %-18s | %-18s | %-10s | %-6s | %s\n",
					   id, doctor_nombre, especialidad, fecha, hora, estado);
				encontradas++;
			}
		}
	}
	
	fclose(f);
	
	if (!encontradas) {
		printf("\nNo tienes citas agendadas\n");
	}
	
	printf("\n");
	pausar();
}

void cancelar_cita_paciente(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	FILE *temp = fopen("temp_citas.txt", "w");
	char linea[300];
	int id_cita;
	int encontrada = 0;
	
	if (!f || !temp) {
		printf("Error al abrir archivos\n");
		return;
	}
	
	system("cls");
	printf("=== CANCELAR CITA ===\n\n");
	
	// Mostrar citas del paciente primero
	mostrar_citas_paciente(email);
	
	printf("\nIngrese el ID de la cita a cancelar: ");
	scanf("%d", &id_cita);
	while (getchar() != '\n'); // Limpiar buffer
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, doctor_id, activa;
		char paciente_email[100], nombre_paciente[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, nombre_paciente, &doctor_id, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id == id_cita && strcmp(email, paciente_email) == 0) {
				// Cambiar estado a "cancelada" y activa=0
				fprintf(temp, "%d;%s;%s;%d;%s;%s;%s;%s;%s;cancelada;0\n",
						id, paciente_email, nombre_paciente, doctor_id, doctor_nombre,
						especialidad, fecha, hora, motivo);
				encontrada = 1;
				printf("\n¡Cita cancelada exitosamente!\n");
			} else {
				fputs(linea, temp);
			}
		}
	}
	
	fclose(f);
	fclose(temp);
	
	if (encontrada) {
		remove(ARCHIVO_CITAS);
		rename("temp_citas.txt", ARCHIVO_CITAS);
	} else {
		remove("temp_citas.txt");
		printf("\nError: Cita no encontrada o no te pertenece\n");
	}
	
	pausar();
}

void reagendar_cita_paciente(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	FILE *temp = fopen("temp_citas.txt", "w");
	char linea[300];
	int id_cita;
	char nueva_fecha[20], nueva_hora[10];
	int encontrada = 0;
	
	if (!f || !temp) {
		printf("Error al abrir archivos\n");
		return;
	}
	
	system("cls");
	printf("=== REAGENDAR CITA ===\n\n");
	
	// Mostrar citas del paciente
	mostrar_citas_paciente(email);
	
	printf("\nIngrese el ID de la cita a reagendar: ");
	scanf("%d", &id_cita);
	while (getchar() != '\n');
	
	// Solicitar nueva fecha
	do {
		printf("\nIngrese nueva fecha (DD/MM/AAAA): ");
		fgets(nueva_fecha, sizeof(nueva_fecha), stdin);
		nueva_fecha[strcspn(nueva_fecha, "\n")] = 0;
		
		if (!validar_fecha(nueva_fecha)) {
			printf("Formato de fecha incorrecto. Use DD/MM/AAAA\n");
		}
	} while (!validar_fecha(nueva_fecha));
	
	// Solicitar nueva hora
	do {
		printf("Ingrese nueva hora (HH:MM, 24h): ");
		fgets(nueva_hora, sizeof(nueva_hora), stdin);
		nueva_hora[strcspn(nueva_hora, "\n")] = 0;
		
		if (!validar_hora(nueva_hora)) {
			printf("Formato de hora incorrecto. Use HH:MM (0-23:0-59)\n");
		}
	} while (!validar_hora(nueva_hora));
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, doctor_id, activa;
		char paciente_email[100], nombre_paciente[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, nombre_paciente, &doctor_id, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id == id_cita && strcmp(email, paciente_email) == 0 && activa == 1) {
				// Actualizar fecha y hora
				fprintf(temp, "%d;%s;%s;%d;%s;%s;%s;%s;%s;%s;%d\n",
						id, paciente_email, nombre_paciente, doctor_id, doctor_nombre,
						especialidad, nueva_fecha, nueva_hora, motivo, estado, activa);
				encontrada = 1;
				printf("\n¡Cita reagendada exitosamente!\n");
			} else {
				fputs(linea, temp);
			}
		}
	}
	
	fclose(f);
	fclose(temp);
	
	if (encontrada) {
		remove(ARCHIVO_CITAS);
		rename("temp_citas.txt", ARCHIVO_CITAS);
	} else {
		remove("temp_citas.txt");
		printf("\nError: Cita no encontrada, no te pertenece o está cancelada\n");
	}
	
	pausar();
}

// ============================================
// FUNCIONES PARA MÉDICOS
// ============================================

void mostrar_citas_doctor(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	char linea[300];
	int encontradas = 0;
	
	// Obtener ID del doctor
	int doctor_id = obtener_id_doctor_por_email(email);
	if (doctor_id == -1) {
		printf("No se pudo identificar al doctor\n");
		pausar();
		return;
	}
	
	// Obtener nombre del doctor
	char nombre_doctor[100];
	obtener_nombre_doctor(doctor_id, nombre_doctor);
	
	system("cls");
	printf("=== CITAS ASIGNADAS A %s ===\n\n", nombre_doctor);
	
	if (!f) {
		printf("No hay citas registradas\n");
		pausar();
		return;
	}
	
	printf("ID  | Paciente           | Fecha      | Hora   | Estado     | Motivo\n");
	printf("----|--------------------|------------|--------|------------|--------\n");
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, id_doc, activa;
		char paciente_email[100], paciente_nombre[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, paciente_nombre, &id_doc, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id_doc == doctor_id && activa == 1 && strcmp(estado, "cancelada") != 0) {
				printf("%-3d | %-18s | %-10s | %-6s | %-10s | %s\n",
					   id, paciente_nombre, fecha, hora, estado, motivo);
				encontradas++;
			}
		}
	}
	
	fclose(f);
	
	if (!encontradas) {
		printf("\nNo hay citas asignadas\n");
	}
	
	printf("\n");
	pausar();
}

void confirmar_cita_doctor(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	FILE *temp = fopen("temp_citas.txt", "w");
	char linea[300];
	int id_cita;
	int encontrada = 0;
	int doctor_id = obtener_id_doctor_por_email(email);
	
	if (!f || !temp) {
		printf("Error al abrir archivos\n");
		return;
	}
	
	system("cls");
	printf("=== CONFIRMAR CITA ===\n\n");
	
	// Mostrar citas pendientes del doctor
	printf("Citas pendientes:\n");
	printf("ID  | Paciente           | Fecha      | Hora   | Motivo\n");
	printf("----|--------------------|------------|--------|--------\n");
	
	// Primera pasada: mostrar citas pendientes
	while (fgets(linea, sizeof(linea), f)) {
		int id, id_doc, activa;
		char paciente_email[100], paciente_nombre[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, paciente_nombre, &id_doc, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id_doc == doctor_id && activa == 1 && strcmp(estado, "pendiente") == 0) {
				printf("%-3d | %-18s | %-10s | %-6s | %s\n",
					   id, paciente_nombre, fecha, hora, motivo);
			}
		}
	}
	
	rewind(f); // Volver al inicio del archivo
	
	printf("\nIngrese el ID de la cita a confirmar: ");
	scanf("%d", &id_cita);
	while (getchar() != '\n');
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, id_doc, activa;
		char paciente_email[100], paciente_nombre[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, paciente_nombre, &id_doc, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id == id_cita && id_doc == doctor_id && strcmp(estado, "pendiente") == 0) {
				// Cambiar estado a "confirmada"
				fprintf(temp, "%d;%s;%s;%d;%s;%s;%s;%s;%s;confirmada;%d\n",
						id, paciente_email, paciente_nombre, id_doc, doctor_nombre,
						especialidad, fecha, hora, motivo, activa);
				encontrada = 1;
				printf("\n¡Cita confirmada exitosamente!\n");
			} else {
				fputs(linea, temp);
			}
		}
	}
	
	fclose(f);
	fclose(temp);
	
	if (encontrada) {
		remove(ARCHIVO_CITAS);
		rename("temp_citas.txt", ARCHIVO_CITAS);
	} else {
		remove("temp_citas.txt");
		printf("\nError: Cita no encontrada o no está pendiente\n");
	}
	
	pausar();
}

void completar_cita_doctor(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	FILE *temp = fopen("temp_citas.txt", "w");
	char linea[300];
	int id_cita;
	int encontrada = 0;
	int doctor_id = obtener_id_doctor_por_email(email);
	
	if (!f || !temp) {
		printf("Error al abrir archivos\n");
		return;
	}
	
	system("cls");
	printf("=== COMPLETAR CITA ===\n\n");
	
	// Mostrar citas confirmadas del doctor
	printf("Citas confirmadas:\n");
	printf("ID  | Paciente           | Fecha      | Hora   | Motivo\n");
	printf("----|--------------------|------------|--------|--------\n");
	
	// Primera pasada: mostrar citas confirmadas
	while (fgets(linea, sizeof(linea), f)) {
		int id, id_doc, activa;
		char paciente_email[100], paciente_nombre[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, paciente_nombre, &id_doc, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id_doc == doctor_id && activa == 1 && strcmp(estado, "confirmada") == 0) {
				printf("%-3d | %-18s | %-10s | %-6s | %s\n",
					   id, paciente_nombre, fecha, hora, motivo);
			}
		}
	}
	
	rewind(f); // Volver al inicio del archivo
	
	printf("\nIngrese el ID de la cita a completar: ");
	scanf("%d", &id_cita);
	while (getchar() != '\n');
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, id_doc, activa;
		char paciente_email[100], paciente_nombre[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, paciente_nombre, &id_doc, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id == id_cita && id_doc == doctor_id && strcmp(estado, "confirmada") == 0) {
				// Cambiar estado a "completada"
				fprintf(temp, "%d;%s;%s;%d;%s;%s;%s;%s;%s;completada;%d\n",
						id, paciente_email, paciente_nombre, id_doc, doctor_nombre,
						especialidad, fecha, hora, motivo, activa);
				encontrada = 1;
				printf("\n¡Cita marcada como completada!\n");
			} else {
				fputs(linea, temp);
			}
		}
	}
	
	fclose(f);
	fclose(temp);
	
	if (encontrada) {
		remove(ARCHIVO_CITAS);
		rename("temp_citas.txt", ARCHIVO_CITAS);
	} else {
		remove("temp_citas.txt");
		printf("\nError: Cita no encontrada o no está confirmada\n");
	}
	
	pausar();
}
void mostrar_todas_las_citas() {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	char linea[300];
	
	if (!f) {
		printf("No hay citas\n");
		pausar();
		return;
	}
	
	system("cls");
	printf("=== TODAS LAS CITAS ===\n\n");
	
	printf("ID  | Paciente           | Doctor             | Fecha      | Hora   | Estado     | Activa\n");
	printf("----|--------------------|--------------------|------------|--------|------------|-------\n");
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, doctor_id, activa;
		char paciente_email[100], nombre_paciente[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, nombre_paciente, &doctor_id, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			printf("%-3d | %-18s | %-18s | %-10s | %-6s | %-10s | %s\n",
				   id, nombre_paciente, doctor_nombre, fecha, hora, estado,
				   activa ? "Si" : "No");
		}
	}
	
	fclose(f);
	pausar();
}
void cancelar_cita_admin() {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	FILE *temp = fopen("temp_citas.txt", "w");
	char linea[300];
	int id_cancelar;
	int encontrada = 0;
	
	if (!f || !temp) return;
	
	system("cls");
	printf("=== CANCELAR CITA (ADMIN) ===\n\n");
	
	mostrar_todas_las_citas();
	printf("\nIngrese ID de la cita a cancelar: ");
	scanf("%d", &id_cancelar);
	while (getchar() != '\n');
	
	while (fgets(linea, sizeof(linea), f)) {
		int id, doctor_id, activa;
		char paciente_email[100], nombre_paciente[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea, "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, nombre_paciente, &doctor_id, doctor_nombre,
				   especialidad, fecha, hora, motivo, estado, &activa) == 11) {
			
			if (id == id_cancelar) {
				fprintf(temp, "%d;%s;%s;%d;%s;%s;%s;%s;%s;cancelada;0\n", 
						id, paciente_email, nombre_paciente, doctor_id, doctor_nombre,
						especialidad, fecha, hora, motivo);
				encontrada = 1;
			} else {
				fputs(linea, temp);
			}
		}
	}
	
	fclose(f);
	fclose(temp);
	
	remove(ARCHIVO_CITAS);
	rename("temp_citas.txt", ARCHIVO_CITAS);
	
	if (encontrada)
		printf("\nCita cancelada correctamente\n");
	else
		printf("\nCita no encontrada\n");
	
	pausar();
}

void gestionar_doctores() {
	int op;
	
	do {
		system("cls");
		printf("=== GESTION DE DOCTORES ===\n");
		printf("1. Ver todos los doctores\n");
		printf("2. Agregar nuevo doctor\n");
		printf("3. Modificar disponibilidad de doctor\n");
		printf("4. Volver al menu anterior\n");
		printf("Seleccione una opcion: ");
		
		op = leer_opcion(1, 4);
		
		switch (op) {
		case 1:
			ver_doctores_disponibles();
			break;
		case 2:
			agregar_nuevo_doctor();
			break;
		case 3:
			modificar_disponibilidad_doctor();
			break;
		}
	} while (op != 4);
}

void ver_doctores_disponibles() {
	FILE *f = fopen(ARCHIVO_DOCTORES, "r");
	char linea[200];
	int contador = 0;
	
	if (!f) {
		printf("No hay doctores registrados\n");
		pausar();
		return;
	}
	
	system("cls");
	printf("=== LISTA COMPLETA DE DOCTORES ===\n\n");
	printf("ID | Nombre              | Especialidad        | Horario              | Disponible\n");
	printf("---|---------------------|---------------------|----------------------|------------\n");
	
	while (fgets(linea, sizeof(linea), f)) {
		int id;
		char nombre[100], especialidad[50], horario[50];
		int disponible;
		
		if (sscanf(linea, "%d;%[^;];%[^;];%[^;];%d", 
				   &id, nombre, especialidad, horario, &disponible) == 5) {
			
			printf("%-2d | %-19s | %-19s | %-20s | %s\n",
				   id, nombre, especialidad, horario, 
				   disponible ? "Si" : "No");
			contador++;
		}
	}
	
	fclose(f);
	
	if (contador == 0) {
		printf("No hay doctores registrados\n");
	} else {
		printf("\nTotal de doctores: %d\n", contador);
	}
	
	pausar();
}

// Función para agregar nuevo doctor (solo admin)
void agregar_nuevo_doctor() {
	FILE *f = fopen(ARCHIVO_DOCTORES, "a");
	Doctor nuevo_doctor;
	char opcion;
	
	if (!f) {
		printf("Error al abrir archivo de doctores\n");
		pausar();
		return;
	}
	
	system("cls");
	printf("=== AGREGAR NUEVO DOCTOR ===\n\n");
	
	// Generar ID automático
	FILE *f_temp = fopen(ARCHIVO_DOCTORES, "r");
	int max_id = 0;
	char linea_temp[200];
	int id_temp;
	
	if (f_temp) {
		while (fgets(linea_temp, sizeof(linea_temp), f_temp)) {
			if (sscanf(linea_temp, "%d;", &id_temp) == 1) {
				if (id_temp > max_id) max_id = id_temp;
			}
		}
		fclose(f_temp);
	}
	nuevo_doctor.id = max_id + 1;
	
	// Solicitar datos del doctor
	printf("ID asignado automaticamente: %d\n", nuevo_doctor.id);
	
	printf("Ingrese nombre del doctor: ");
	fgets(nuevo_doctor.nombre, sizeof(nuevo_doctor.nombre), stdin);
	nuevo_doctor.nombre[strcspn(nuevo_doctor.nombre, "\n")] = 0;
	
	// Seleccionar especialidad
	printf("\nSeleccione especialidad:\n");
	printf("1. Medicina General\n");
	printf("2. Psicologia\n");
	printf("3. Traumatologia\n");
	printf("Opcion: ");
	int esp;
	esp = leer_opcion(1, 3);
	
	switch(esp) {
	case 1: strcpy(nuevo_doctor.especialidad, "Medicina General"); break;
	case 2: strcpy(nuevo_doctor.especialidad, "Psicologia"); break;
	case 3: strcpy(nuevo_doctor.especialidad, "Traumatologia"); break;
	}
	
	printf("Ingrese horario (ej: Lunes-Viernes 8:00-16:00): ");
	fgets(nuevo_doctor.horario, sizeof(nuevo_doctor.horario), stdin);
	nuevo_doctor.horario[strcspn(nuevo_doctor.horario, "\n")] = 0;
	
	printf("¿Está disponible? (1=Si, 0=No): ");
	scanf("%d", &nuevo_doctor.disponible);
	while (getchar() != '\n');
	
	// Confirmar
	printf("\n=== RESUMEN DEL DOCTOR ===\n");
	printf("ID: %d\n", nuevo_doctor.id);
	printf("Nombre: %s\n", nuevo_doctor.nombre);
	printf("Especialidad: %s\n", nuevo_doctor.especialidad);
	printf("Horario: %s\n", nuevo_doctor.horario);
	printf("Disponible: %s\n\n", nuevo_doctor.disponible ? "Si" : "No");
	
	printf("¿Confirmar registro? (s/n): ");
	opcion = getchar();
	while (getchar() != '\n');
	
	if (opcion == 's' || opcion == 'S') {
		// Guardar en archivo
		fprintf(f, "%d;%s;%s;%s;%d\n",
				nuevo_doctor.id,
				nuevo_doctor.nombre,
				nuevo_doctor.especialidad,
				nuevo_doctor.horario,
				nuevo_doctor.disponible);
		
		printf("\n¡Doctor registrado exitosamente!\n");
	} else {
		printf("\nRegistro cancelado\n");
	}
	
	fclose(f);
	pausar();
}

// Función para modificar disponibilidad de doctor
void modificar_disponibilidad_doctor() {
	FILE *f = fopen(ARCHIVO_DOCTORES, "r");
	FILE *temp = fopen("temp_doctores.txt", "w");
	char linea[200];
	int id_doctor, nueva_disponibilidad;
	int encontrado = 0;
	
	if (!f || !temp) {
		printf("Error al abrir archivos\n");
		pausar();
		return;
	}
	
	system("cls");
	printf("=== MODIFICAR DISPONIBILIDAD DE DOCTOR ===\n\n");
	
	// Mostrar doctores
	ver_doctores_disponibles();
	
	printf("\nIngrese ID del doctor a modificar: ");
	scanf("%d", &id_doctor);
	while (getchar() != '\n');
	
	printf("Ingrese nueva disponibilidad (1=Disponible, 0=No disponible): ");
	scanf("%d", &nueva_disponibilidad);
	while (getchar() != '\n');
	
	// Procesar archivo
	while (fgets(linea, sizeof(linea), f)) {
		int id;
		char nombre[100], especialidad[50], horario[50];
		int disponible;
		
		if (sscanf(linea, "%d;%[^;];%[^;];%[^;];%d", 
				   &id, nombre, especialidad, horario, &disponible) == 5) {
			
			if (id == id_doctor) {
				// Actualizar disponibilidad
				fprintf(temp, "%d;%s;%s;%s;%d\n",
						id, nombre, especialidad, horario, nueva_disponibilidad);
				encontrado = 1;
				printf("\n¡Disponibilidad actualizada exitosamente!\n");
			} else {
				fputs(linea, temp);
			}
		}
	}
	
	fclose(f);
	fclose(temp);
	
	if (encontrado) {
		remove(ARCHIVO_DOCTORES);
		rename("temp_doctores.txt", ARCHIVO_DOCTORES);
	} else {
		remove("temp_doctores.txt");
		printf("\nError: Doctor no encontrado\n");
	}
	
	pausar();
}


void cancelar_cita_medico(const char *email) {
	FILE *f = fopen(ARCHIVO_CITAS, "r");
	FILE *temp = fopen("temp_citas.txt", "w");
	char linea[300];
	int id_cancelar;
	int encontrada = 0;
	int doctor_id = obtener_id_doctor_por_email(email);
	
	if (!f || !temp) {
		printf("Error al abrir archivos\n");
		if (f) fclose(f);
		if (temp) fclose(temp);
		return;
	}
	
	system("cls");
	printf("=== CANCELAR CITA (MEDICO) ===\n\n");
	
	printf("Citas pendientes:\n");
	printf("ID  | Paciente           | Fecha      | Hora   | Motivo\n");
	printf("----|--------------------|------------|--------|--------\n");
	
	/* Primera pasada: mostrar citas pendientes */
	while (fgets(linea, sizeof(linea), f)) {
		int id, id_doc, activa;
		char paciente_email[100], paciente_nombre[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea,
				   "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, paciente_nombre, &id_doc,
				   doctor_nombre, especialidad, fecha, hora,
				   motivo, estado, &activa) == 11) {
			
			if (id_doc == doctor_id && activa == 1 && strcmp(estado, "pendiente") == 0) {
				printf("%-3d | %-18s | %-10s | %-6s | %s\n",
					   id, paciente_nombre, fecha, hora, motivo);
			}
		}
	}
	
	printf("\nIngrese ID de la cita a cancelar: ");
	if (scanf("%d", &id_cancelar) != 1) {
		printf("ID inválido\n");
		fclose(f);
		fclose(temp);
		return;
	}
	while (getchar() != '\n');
	
	/* Volver al inicio del archivo */
	rewind(f);
	
	/* Segunda pasada: cancelar cita */
	while (fgets(linea, sizeof(linea), f)) {
		int id, id_doc, activa;
		char paciente_email[100], nombre_paciente[100];
		char doctor_nombre[100], especialidad[50];
		char fecha[20], hora[10], motivo[200], estado[20];
		
		if (sscanf(linea,
				   "%d;%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d",
				   &id, paciente_email, nombre_paciente, &id_doc,
				   doctor_nombre, especialidad, fecha, hora,
				   motivo, estado, &activa) == 11) {
			
			if (id == id_cancelar && id_doc == doctor_id) {
				fprintf(temp,
						"%d;%s;%s;%d;%s;%s;%s;%s;%s;cancelada;0\n",
						id, paciente_email, nombre_paciente,
						id_doc, doctor_nombre, especialidad,
						fecha, hora, motivo);
				encontrada = 1;
			} else {
				fputs(linea, temp);
			}
		}
	}
	
	fclose(f);
	fclose(temp);
	
	if (encontrada) {
		remove(ARCHIVO_CITAS);
		rename("temp_citas.txt", ARCHIVO_CITAS);
		printf("\nCita cancelada correctamente\n");
	} else {
		remove("temp_citas.txt");
		printf("\nCita no encontrada\n");
	}
	
	pausar();
}

