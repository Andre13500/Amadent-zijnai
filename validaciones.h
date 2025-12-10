// validaciones.h
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stdbool.h>

// Funciones de validación
bool validar_nombre(const char *nombre);
bool validar_solo_numeros(const char *cadena);
bool validar_email(const char *email);
bool validar_longitud(const char *cadena, int min, int max);
bool validar_contrasena(const char *contrasena);
bool validar_cedula_ecuador(const char *cedula);
void limpiar_buffer_entrada(void);

#endif
