#ifndef CITAS_H
#define CITAS_H

// Estructura para doctor
typedef struct {
	int id;
	char nombre[100];
	char especialidad[50];
	char horario[50];
	int disponible;
} Doctor;

// Funciones para pacientes
void agendar_cita(const char *email);
void mostrar_citas_paciente(const char *email);
void cancelar_cita_paciente(const char *email);
void reagendar_cita_paciente(const char *email);

// Funciones para médicos
void mostrar_citas_doctor(const char *email);
void confirmar_cita_doctor(const char *email);
void completar_cita_doctor(const char *email);
void cancelar_cita_medico();

// Funciones para administradores
void mostrar_todas_las_citas();
void cancelar_cita_admin();
void ver_especialidades();
void gestionar_doctores();
void ver_doctores_disponibles();

// Funciones auxiliares
int generar_id_cita();  // QUITAR static de la implementación
int obtener_id_doctor_por_email(const char *email);  // QUITAR static
char* obtener_especialidad_por_id(int id);  // QUITAR static

#endif
