#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "registro_utils.h"
#include "utilidades.h"
#include "validaciones.h"

void pantalla_pacientes(){
	system("cls");
	printf("========================================\n");
	printf("        MENÚ DE PACIENTES\n");
	printf("========================================\n");
	printf("¡Hola! Has ingresado al sistema.\n\n");
	// Aquí puedes agregar las funcionalidades del menú de pacientes
	continuar();
}
	
	// Función para obtener el último ID automáticamente
	int obtener_ultimo_id(void) {
		FILE *archivo;
		datos_registro usuario;
		int ultimo_id = 0;
		
		archivo = fopen("Registros_usuarios.dat", "rb");
		if (archivo == NULL) {
			return 0; // Si el archivo no existe, empezamos desde 0
		}
		
		while (fread(&usuario, sizeof(datos_registro), 1, archivo)) {
			if (usuario.id_paciente > ultimo_id) {
				ultimo_id = usuario.id_paciente;
			}
		}
		
		fclose(archivo);
		return ultimo_id;
	}
	
	void pantalla_registro(void) {
		FILE *archivo;
		datos_registro usuario;
		datos_registro usuario_ingresado;
		int encontrado = 0;
		char opcion_registro;
		
		system("cls");
		printf("\n========================================\n");
		printf("        INICIO DE SESION\n");
		printf("========================================\n\n");
		
		// Solicitar email
		printf("Ingrese su correo: ");
		fgets(usuario_ingresado.email, 100, stdin);
		usuario_ingresado.email[strcspn(usuario_ingresado.email, "\n")] = 0;
		
		// Validar email
		while (!validar_email(usuario_ingresado.email)) {
			printf("Email no válido. Ingrese un email válido (ejemplo@dominio.com): ");
			fgets(usuario_ingresado.email, 100, stdin);
			usuario_ingresado.email[strcspn(usuario_ingresado.email, "\n")] = 0;
		}
		
		// Solicitar contraseña
		printf("Ingrese su contrasena: ");
		fgets(usuario_ingresado.contrasena, 50, stdin);
		usuario_ingresado.contrasena[strcspn(usuario_ingresado.contrasena, "\n")] = 0;
		
		// Abrir archivo para verificar si el usuario existe
		archivo = fopen("Registros_usuarios.dat", "rb");
		if (archivo == NULL) {
			printf("\nNo hay usuarios registrados en el sistema.\n");
			encontrado = 0;
		} else {
			while (fread(&usuario, sizeof(datos_registro), 1, archivo)) {
				usuario.email[strcspn(usuario.email, "\n")] = 0;
				usuario.contrasena[strcspn(usuario.contrasena, "\n")] = 0;
				
				if (strcmp(usuario.email, usuario_ingresado.email) == 0 && 
					strcmp(usuario.contrasena, usuario_ingresado.contrasena) == 0) {
					encontrado = 1;
					break;
				}
			}
			fclose(archivo);
		}
		
		if (encontrado) {
			printf("\n========================================\n");
			printf("   ¡Inicio de sesión exitoso!\n");
			printf("   Bienvenido/a, %s\n", usuario.nombre);
			printf("========================================\n\n");
			continuar();
			
			pantalla_pacientes();
		} else {
			printf("\n========================================\n");
			printf("   Usuario no encontrado o credenciales incorrectas\n");
			printf("========================================\n\n");
			
			printf("¿Desea registrarse? (S/N): ");
			scanf(" %c", &opcion_registro);
			limpiar_buffer_entrada();
			
			if (toupper(opcion_registro) == 'S') {
				pantalla_registro_completo();
			} else {
				printf("\nRegreso al menú principal.\n");
				continuar();
			}
		}
	}
	
	void pantalla_registro_completo(void) {
		FILE *Registros;
		datos_registro nuevo_usuario;
		char confirmar;
		char temp_input[100];
		
		system("cls");
		printf("\n========================================\n");
		printf("        REGISTRO DE NUEVO USUARIO\n");
		printf("========================================\n\n");
		
		// Generar ID automático
		nuevo_usuario.id_paciente = obtener_ultimo_id() + 1;
		printf("ID asignado automáticamente: %d\n\n", nuevo_usuario.id_paciente);
		
		// Solicitar y validar nombre
		do {
			printf("Ingrese su nombre completo: ");
			fgets(nuevo_usuario.nombre, 100, stdin);
			nuevo_usuario.nombre[strcspn(nuevo_usuario.nombre, "\n")] = 0;
			
			if (!validar_nombre(nuevo_usuario.nombre)) {
				printf("Error: El nombre solo puede contener letras y espacios.\n");
			}
		} while (!validar_nombre(nuevo_usuario.nombre));
		
		// Solicitar y validar email
		do {
			printf("Ingrese su email: ");
			fgets(nuevo_usuario.email, 100, stdin);
			nuevo_usuario.email[strcspn(nuevo_usuario.email, "\n")] = 0;
			
			if (!validar_email(nuevo_usuario.email)) {
				printf("Error: Email no válido. Formato: ejemplo@dominio.com\n");
			}
		} while (!validar_email(nuevo_usuario.email));
		
		// Solicitar y validar cédula
		do {
			printf("Ingrese su cédula (10 dígitos): ");
			fgets(nuevo_usuario.cedula, 11, stdin);
			nuevo_usuario.cedula[strcspn(nuevo_usuario.cedula, "\n")] = 0;
			
			// Limpiar buffer si se ingresaron más de 10 caracteres
			if (strlen(nuevo_usuario.cedula) == 10 && getchar() != '\n') {
				limpiar_buffer_entrada();
			}
			
			if (!validar_cedula_ecuador(nuevo_usuario.cedula)) {
				printf("Error: Cédula no válida. Debe tener 10 dígitos válidos para Ecuador.\n");
			}
		} while (!validar_cedula_ecuador(nuevo_usuario.cedula));
		
		// Solicitar y validar contraseña
		do {
			printf("Ingrese su contraseña (mínimo 6 caracteres): ");
			fgets(nuevo_usuario.contrasena, 50, stdin);
			nuevo_usuario.contrasena[strcspn(nuevo_usuario.contrasena, "\n")] = 0;
			
			if (!validar_contrasena(nuevo_usuario.contrasena)) {
				printf("Error: La contraseña debe tener al menos 6 caracteres.\n");
			}
		} while (!validar_contrasena(nuevo_usuario.contrasena));
		
		// Mostrar resumen del registro
		printf("\n========================================\n");
		printf("          RESUMEN DEL REGISTRO\n");
		printf("========================================\n");
		printf("ID: %d\n", nuevo_usuario.id_paciente);
		printf("Nombre: %s\n", nuevo_usuario.nombre);
		printf("Email: %s\n", nuevo_usuario.email);
		printf("Cédula: %s\n", nuevo_usuario.cedula);
		printf("========================================\n\n");
		
		// Confirmar registro
		printf("¿Confirmar registro? (S/N): ");
		scanf(" %c", &confirmar);
		limpiar_buffer_entrada();
		
		if (toupper(confirmar) != 'S') {
			printf("\nRegistro cancelado.\n");
			continuar();
			return;
		}
		
		// Guardar en archivo
		Registros = fopen("Registros_usuarios.dat", "ab");
		if (Registros == NULL) {
			printf("Error al abrir el archivo para guardar.\n");
			continuar();
			return;
		}
		
		fwrite(&nuevo_usuario, sizeof(datos_registro), 1, Registros);
		fclose(Registros);
		
		printf("\n========================================\n");
		printf("   ¡REGISTRO EXITOSO!\n");
		printf("========================================\n");
		printf("Su usuario ha sido registrado con éxito.\n");
		printf("Guarde su ID: %d\n", nuevo_usuario.id_paciente);
		printf("========================================\n\n");
		
		continuar();
		
		// Iniciar sesión automáticamente
		printf("Iniciando sesión automáticamente...\n");
		continuar();
		pantalla_pacientes();
	}
	
	// Ejemplo de menú principal
	void menu_principal(void) {
		int opcion;
		
		do {
			system("cls");
			printf("\n========================================\n");
			printf("        SISTEMA DE PACIENTES\n");
			printf("========================================\n");
			printf("1. Iniciar sesión\n");
			printf("2. Registrarse\n");
			printf("3. Salir\n");
			printf("========================================\n");
			printf("Seleccione una opción: ");
			
			if (scanf("%d", &opcion) != 1) {
				limpiar_buffer_entrada();
				printf("Opción no válida.\n");
				continuar();
				continue;
			}
			limpiar_buffer_entrada();
			
			switch(opcion) {
			case 1:
				pantalla_registro();
				break;
			case 2:
				pantalla_registro_completo();
				break;
			case 3:
				printf("\n¡Hasta luego!\n");
				break;
			default:
				printf("Opción no válida. Intente de nuevo.\n");
				continuar();
			}
		} while(opcion != 3);
	}


