#include "registro_utils"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validarCedula(const char *cedula) {
	if (!cedula) return 0;
	if (strlen(cedula) != 10) return 0;
	for (int i = 0; i < 10; i++)
		if (!isdigit((unsigned char)cedula[i]))
			return 0;
	return 1;
}

// ---- Validar nombre (solo letras y espacios) ----
int validarNombre(const char *nombre) {
	if (!nombre || strlen(nombre) < 3) return 0;
	
	for (int i = 0; nombre[i]; i++) {
		char c = nombre[i];
		if (!isalpha((unsigned char)c) && c != ' ')
			return 0;
	}
	return 1;
}

// ---- Validar edad (rango humano razonable) ----
int validarEdad(int edad) {
	return (edad >= 0 && edad <= 120);
}

// ---- Validar celularéfono (solo dígitos, mínimo 7) ----
int validarcelular(const char *celular) {
	if (!celular || strlen(celular) < 7 || strlen(celular) > 20) return 0;
	
	for (int i = 0; celular[i]; i++)
		if (!isdigit((unsigned char)celular[i]))
			return 0;
	
	return 1;
}
int validarCorreo(const char *correo) {
	if (!correo || strlen(correo) < 5) return 0;
	
	int tieneArroba = 0, tienePunto = 0;
	
	for (int i = 0; correo[i]; i++) {
		if (correo[i] == '@') tieneArroba = 1;
		if (correo[i] == '.') tienePunto = 1;
	}
	
	return (tieneArroba && tienePunto);
}
int validarPacienteCompleto(const Paciente *p) {
	if (!validarCedula(p->cedula)) return 0;
	if (!validarNombre(p->nombre)) return 0;
	if (!validarEdad(p->edad)) return 0;
	if (!validarcelular(p->celular)) return 0;
	if (!validarCorreo(p->correo)) return 0;
	
	return 1;
}
int existePaciente(const char *cedula, const char *archivo_pacientes) {
	FILE *f = fopen(archivo_pacientes, "r");
	if (!f) return 0;
	
	char line[512], ced[11];
	while (fgets(line, sizeof(line), f)) {
		sscanf(line, "%10[^|]|", ced);
		if (strcmp(ced, cedula) == 0) {
			fclose(f);
			return 1;
		}
	}
	
	fclose(f);
	return 0;
}

// ---- Registrar paciente ----
int registrarPaciente(const Paciente *p, const char *archivo_pacientes) {
	
	if (!validarPacienteCompleto(p)) {
		printf("? Error: Datos inválidos\n");
		return 0;
	}
	
	if (existePaciente(p->cedula, archivo_pacientes)) {
		printf("? Error: Ya existe un paciente con esa cédula.\n");
		return 0;
	}
	
	FILE *f = fopen(archivo_pacientes, "a");
	if (!f) return 0;
	
	fprintf(f, "%s|%s|%d|%s|%s\n",
			p->cedula, p->nombre, p->edad, p->celular, p->correo);
	
	fclose(f);
	return 1;
}

// ---- Leer paciente por cédula ----
int leerPaciente(const char *cedula, Paciente *out, const char *archivo_pacientes) {
	FILE *f = fopen(archivo_pacientes, "r");
	if (!f) return 0;
	
	char line[512];
	while (fgets(line, sizeof(line), f)) {
		char ced[11], nom[100], celular[20], corr[100];
		int edad;
		
		if (sscanf(line, "%10[^|]|%99[^|]|%d|%19[^|]|%99[^\n]",
				   ced, nom, &edad, celular, corr) == 5) {
			
			if (strcmp(ced, cedula) == 0) {
				strcpy(out->cedula, ced);
				strcpy(out->nombre, nom);
				out->edad = edad;
				strcpy(out->celular, celular);
				strcpy(out->correo, corr);
				
				fclose(f);
				return 1;
			}
		}
	}
	
	fclose(f);
	return 0;
}

// ---- Listar todos los pacientes ----
void listarPacientes(const char *archivo_pacientes) {
	FILE *f = fopen(archivo_pacientes, "r");
	if (!f) {
		printf("No hay pacientes registrados.\n");
		return;
	}
	
	char line[512];
	while (fgets(line, sizeof(line), f)) {
		printf("%s", line);
	}
	
	fclose(f);
}
