#include <stdio.h>
#include <stdlib.h>
#include "utilidades.h"
#include "registro_utils.h"



void salida(){
printf("\nEl programa ha finalizado");
exit(0);


}

int main()
{
int opcion_menu1 = 0;
    do{
    system("cls");
    printf("========AMADENT========\n");
    printf("1.Registrarse\n");
    printf("2.Iniciar Sesion\n");
    printf("3.Salir \n");
    printf("Seleccione una opcion: ");

    opcion_menu1 = leer_opcion();






    switch(opcion_menu1){

    case 1:
        system("cls");
        printf("Pantalla de registro de sesion");
        pantalla_registro();

        pausar();
        break;

    case 2:
        system("cls");
        printf("Inicio de sesion");
        pausar();
        break;
    case 3:
        salida();
        break;


    }


}while(opcion_menu1 != 3);



}
