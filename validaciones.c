// validaciones.c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "validaciones.h"

// Función para validar que un nombre solo contenga letras y espacios
bool validar_nombre(const char *nombre) {
	if (nombre == NULL || strlen(nombre) == 0) {
		return false;
	}
	
	// Verificar cada carácter
	for (int i = 0; nombre[i] != '\0'; i++) {
		// Permitir letras, espacios, apóstrofes, guiones y puntos
		if (!isalpha(nombre[i]) && nombre[i] != ' ' && nombre[i] != '\'' && 
			nombre[i] != '-' && nombre[i] != '.') {
			return false;
		}
	}
	
	return true;
}

// Función para validar que un string solo contenga números
bool validar_solo_numeros(const char *cadena) {
	if (cadena == NULL || strlen(cadena) == 0) {
		return false;
	}
	
	// Verificar cada carácter
	for (int i = 0; cadena[i] != '\0'; i++) {
		if (!isdigit(cadena[i])) {
			return false;
		}
	}
	
	return true;
}

// Función para validar formato de email básico
bool validar_email(const char *email) {
	if (email == NULL || strlen(email) == 0) {
		return false;
	}
	
	int arroba_count = 0;
	int punto_despues_arroba = 0;
	int longitud = strlen(email);
	
	// Verificar que no empiece ni termine con punto o arroba
	if (email[0] == '@' || email[0] == '.' || 
		email[longitud-1] == '@' || email[longitud-1] == '.') {
		return false;
	}
	
	// Recorrer el email
	for (int i = 0; email[i] != '\0'; i++) {
		// Contar arrobas
		if (email[i] == '@') {
			arroba_count++;
			// Verificar que haya al menos un carácter antes del @
			if (i == 0) {
				return false;
			}
		}
		
		// Verificar punto después del arroba
		if (arroba_count > 0 && email[i] == '.') {
			punto_despues_arroba++;
		}
		
		// Validar caracteres permitidos
		if (!isalnum(email[i]) && email[i] != '@' && email[i] != '.' && 
			email[i] != '_' && email[i] != '-' && email[i] != '+') {
			return false;
		}
	}
	
	// Debe tener exactamente un @ y al menos un punto después del @
	if (arroba_count != 1 || punto_despues_arroba == 0) {
		return false;
	}
	
	return true;
}

// Función para validar longitud mínima y máxima
bool validar_longitud(const char *cadena, int min, int max) {
	if (cadena == NULL) {
		return false;
	}
	
	int longitud = strlen(cadena);
	return (longitud >= min && longitud <= max);
}

// Función para validar contraseña (mínimo 6 caracteres)
bool validar_contrasena(const char *contrasena) {
	if (contrasena == NULL) {
		return false;
	}
	
	int longitud = strlen(contrasena);
	if (longitud < 6) {
		return false;
	}
	
	// Verificar que tenga al menos un número y una letra (opcional)
	bool tiene_numero = false;
	bool tiene_letra = false;
	
	for (int i = 0; contrasena[i] != '\0'; i++) {
		if (isdigit(contrasena[i])) {
			tiene_numero = true;
		}
		if (isalpha(contrasena[i])) {
			tiene_letra = true;
		}
	}
	
	// Al menos debe tener una letra o un número
	return (tiene_numero || tiene_letra);
}

// Función para validar cédula (10 dígitos para Ecuador)
bool validar_cedula_ecuador(const char *cedula) {
	if (cedula == NULL || strlen(cedula) != 10) {
		return false;
	}
	
	// Verificar que todos sean dígitos
	if (!validar_solo_numeros(cedula)) {
		return false;
	}
	
	// Validar provincia (primeros dos dígitos entre 01 y 24)
	char provincia_str[3] = {cedula[0], cedula[1], '\0'};
	int provincia = atoi(provincia_str);
	
	if (provincia < 1 || provincia > 24) {
		return false;
	}
	
	// Tercer dígito debe ser menor a 6
	if (cedula[2] > '5') {
		return false;
	}
	
	// Algoritmo de validación de dígito verificador
	int coeficientes[9] = {2, 1, 2, 1, 2, 1, 2, 1, 2};
	int suma = 0;
	
	for (int i = 0; i < 9; i++) {
		int valor = (cedula[i] - '0') * coeficientes[i];
		
		if (valor > 9) {
			valor -= 9;
		}
		
		suma += valor;
	}
	
	int decena_superior = ((suma / 10) + 1) * 10;
	int digito_verificador = decena_superior - suma;
	
	if (digito_verificador == 10) {
		digito_verificador = 0;
	}
	
	return (digito_verificador == (cedula[9] - '0'));
}

// Función para limpiar entrada
void limpiar_buffer_entrada(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
