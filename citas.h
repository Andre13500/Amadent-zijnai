#ifndef CITAS_H
#define CITAS_H

#define HORA_INICIO 7
#define HORA_FIN 14

typedef struct {
	int id_paciente;
	int mes;
	int dia;
	int hora;
} Cita;



/* Funciones públicas */
void menu_citas(int id_paciente);
void agregar_cita(int id_paciente);

#endif
