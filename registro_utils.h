#ifndef REGISTRO_UTILS_H_INCLUDED
#define REGISTRO_UTILS_H_INCLUDED


typedef enum{
 Paciente,
 Administrador,
 Medico


}tipo_usuario;

//estructura que define los datos de los usuarios
typedef struct {

  int id_paciente;
 char nombre [100];
 char email [100];
 int edad;
 char cedula[10];
 char contrasena[10];


}datos_registro;
void escribir_archivo(void);
void pantalla_registro(void);
void mostrar_contenido_archivo(void);


void pantalla_pacientes(void);




#endif // REGISTRO_UTILS_H_INCLUDED
