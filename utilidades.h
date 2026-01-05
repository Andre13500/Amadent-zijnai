#ifndef UTILIDADES_H
#define UTILIDADES_H

void pausar(void); //funcion de pausar pantalla para mantener orden y no se salten rapidamente las acciones


int leer_opcion(int min, int max); //permite que la opcion ingresada valide solo numeros (util para menus)


void continuar(void);


void salida(void);
int deteccion_correo(char correo[]);
int validar_numeros(int comparador);

#endif // LOGINUTILS_H_INCLUDED
