#include <stdio.h> 
#include <stdlib.h>
#include "registro_utils.h"
#include "utilidades.h"
#include <string.h>
#include <ctype.h>
#include "modulos.h"
#include <time.h>  // <-- AGREGA ESTA LÍNEA
void iniciarSesion();
void pantalla_registro();

datos_registro datos;
int correo_detector;


char email_logueado[100];
void limpiarSaltoLinea(char cadena[]) {
	cadena[strcspn(cadena, "\n")] = '\0';
}


void iniciarSesion() {
	datos_registro temp;
	char linea[150];
	FILE *archivo;
	int encontrado = 0;
	
	archivo = fopen("registro.txt", "r");
	
	if (archivo == NULL) {
		pantalla_registro();
		return;
	}
	
	// PEDIR EMAIL
	do {
		printf("Ingrese su email: ");
		fgets(datos.email, sizeof(datos.email), stdin);
		limpiarSaltoLinea(datos.email);
		correo_detector = deteccion_correo(datos.email);
	} while (correo_detector == 0);
	
	// PEDIR CONTRASEÑA
	printf("Ingrese su contraseña: ");
	fgets(datos.contrasena, sizeof(datos.contrasena), stdin);
	limpiarSaltoLinea(datos.contrasena);
	
	// BUSCAR EN ARCHIVO - FORMATO CORREGIDO
	while (fgets(linea, sizeof(linea), archivo)) {
		linea[strcspn(linea, "\n")] = 0;
		
		// Formato CORRECTO: nombre;email;contrasena;edad;cedula;tipo_usuario
		if (sscanf(linea, "%[^;];%[^;];%[^;];%d;%[^;];%d",
				   temp.nombre,
				   temp.email,
				   temp.contrasena,
				   &temp.edad,
				   temp.cedula,
				   &temp.tipo_usuario) == 6) {
			
			if (strcmp(datos.email, temp.email) == 0 &&
				strcmp(datos.contrasena, temp.contrasena) == 0) {
				encontrado = 1;
				
				// Guardar email para usar después
				strcpy(email_logueado, temp.email);
				
				break;
			}
		}
	}
	
	fclose(archivo);
	
	if (encontrado) {
		printf("\nSesion iniciada correctamente\n");
		pausar();
		
		switch (temp.tipo_usuario) {
		case 1:
			modulo_paciente(temp.email);  // ? CORREGIDO: usar temp.email
			break;
		case 2:
			modulo_medico(temp.email);    // ? CORREGIDO: usar temp.email
			break;
		case 3:
			modulo_administrador(temp.email); // ? CORREGIDO: usar temp.email
			break;
		}
	} else {
		int opcion_registro;
		printf("\nUsuario no registrado. desea reistrarse?? 1. SI // 2. NO \n");
		opcion_registro = leer_opcion(1,2);
		 if (opcion_registro == 1){
		 continuar();
		 pantalla_registro();}
		 
		
	}
}
void pantalla_registro(){


	
FILE *archivo;



archivo = fopen("registro.txt", "a");

if (archivo == NULL) {
	printf("Error al abrir archivo\n");
	return;
}



printf("======PANTALLA DE REGISTRO======\n");

printf("Ingrese su nombre: ");
fgets(datos.nombre, sizeof(datos.nombre), stdin);
limpiarSaltoLinea(datos.nombre);


do{
	
	
	printf("\nIngrese su email: ");
	fgets(datos.email, sizeof(datos.email), stdin);
	limpiarSaltoLinea(datos.email);
	correo_detector = deteccion_correo(datos.email);
}while(correo_detector == 0);


printf("\nIngrese su contraseña: ");
fgets(datos.contrasena, sizeof(datos.contrasena), stdin);
limpiarSaltoLinea(datos.contrasena);


int resultado;
do {
	printf("\nIngrese su edad: ");
	resultado = scanf("%d", &datos.edad);
	
} while (!validar_numeros(resultado));
	

//variables para la validacion de la cedula//
int validacion_cedula;
int i; 

do {
	while (getchar() != '\n');// Asumimos que es válida al inicio
	validacion_cedula = 1;
	printf("\nIngrese su cedula (10 digitos): ");
	fgets(datos.cedula, sizeof(datos.cedula), stdin);
	limpiarSaltoLinea(datos.cedula);
	
	int longitud = strlen(datos.cedula);
	
	// --- LÓGICA DE VALIDACIÓN CON if/else if ---
	if (longitud != 10) {
		// Opción A: Error de longitud
		printf("Error: La cedula debe tener exactamente 10 caracteres.\n");
		
		validacion_cedula = 0;
		
	} else {
		// Opción B: Si tiene 10 caracteres, verificamos que sean números.
		for (i = 0; i < 10; i++) {
			if (!isdigit((unsigned char)datos.cedula[i])) {
				printf("\nError: la cedula solo debe contener numeros.\n");
				validacion_cedula = 0; // Marca como inválida
				break; // Detenemos el for, ya encontramos un error
			}
		}
	}
	// --- Fin de la lógica ---
	
} while (validacion_cedula == 0);






  do{
printf("\ningrese el tipo de usuario 1. Paciente 2.Medico 3. Administrador: ");
datos.tipo_usuario = leer_opcion(1, 3);
	
	

  }while(datos.tipo_usuario < 1 || datos.tipo_usuario > 3);

fprintf(archivo, "%s;%s;%s;%d;%s;%d\n", datos.nombre, datos.email, datos.contrasena, datos.edad, datos.cedula, datos.tipo_usuario);
	
fclose(archivo);

if (datos.tipo_usuario == 2) { // MÉDICO
	printf("\n=== REGISTRO DE MÉDICO ===\n");
	
	int opcion_especialidad;
	char especialidad[50];
	char horario[50];
	
	printf("\n=== SELECCIONAR ESPECIALIDAD ===\n");
	printf("1. Medicina General\n");
	printf("2. Psicologia\n");
	printf("3. Traumatologia\n");
	printf("Seleccione especialidad (1-3): ");
	
	opcion_especialidad = leer_opcion(1, 3);
	
	switch(opcion_especialidad) {
	case 1:
		strcpy(especialidad, "Medicina General");
		strcpy(horario, "Lunes-Viernes 8:00-16:00");
		break;
	case 2:
		strcpy(especialidad, "Psicologia");
		strcpy(horario, "Lunes-Jueves 9:00-17:00");
		break;
	case 3:
		strcpy(especialidad, "Traumatologia");
		strcpy(horario, "Martes-Viernes 10:00-18:00");
		break;
	}
	
	printf("\nHorario por defecto: %s\n", horario);
	printf("¿Desea usar este horario? (s/n): ");
	char opcion_horario = getchar();
	while (getchar() != '\n');
	
	if (opcion_horario == 'n' || opcion_horario == 'N') {
		printf("Ingrese su horario personalizado: ");
		fgets(horario, sizeof(horario), stdin);
		horario[strcspn(horario, "\n")] = '\0';
	}
	
	/* ===== GUARDAR EN MEDICOS.TXT (YA CON ESPECIALIDAD REAL) ===== */
	FILE *f_medico = fopen("medicos.txt", "a");
	if (f_medico) {
		fprintf(f_medico, "%s;%s;%s\n",
				datos.email, datos.nombre, especialidad);
		fclose(f_medico);
	}
	
	/* ===== GUARDAR EN DOCTORES.TXT ===== */
	FILE *f_doctores = fopen("doctores.txt", "a+");
	if (f_doctores) {
		int max_id = 0;
		char linea[200];
		int id_temp, disponible_temp;
		char nombre_temp[100], especialidad_temp[50], horario_temp[50];
		
		rewind(f_doctores);
		
		while (fgets(linea, sizeof(linea), f_doctores)) {
			if (sscanf(linea, "%d;%[^;];%[^;];%[^;];%d",
					   &id_temp, nombre_temp, especialidad_temp,
					   horario_temp, &disponible_temp) == 5) {
				if (id_temp > max_id) max_id = id_temp;
			}
		}
		
		fprintf(f_doctores, "%d;Dr. %s;%s;%s;1\n",
				max_id + 1, datos.nombre, especialidad, horario);
		
		fclose(f_doctores);
	}
	
	printf("\nDoctor registrado exitosamente\n");
} else if (datos.tipo_usuario == 3) { // ADMINISTRADOR
	printf("\n=== REGISTRO DE ADMINISTRADOR ===\n");
	
	FILE *f_admin = fopen("administradores.txt", "a");
	if (f_admin) {
		// Obtener fecha actual
		time_t t = time(NULL);
		struct tm *tm_info = localtime(&t);
		char fecha_registro[20];
		strftime(fecha_registro, 20, "%d/%m/%Y", tm_info);
		
		fprintf(f_admin, "%s;%s;%s;todos\n", 
				datos.email, datos.nombre, fecha_registro);
		fclose(f_admin);
		printf("? Registrado en administradores.txt\n");
	}
}

printf("\n¡Registro completado exitosamente!!\n");
printf("\nregistro guardado exitosamente!!\n");

	
}



