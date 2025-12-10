#include <stdio.h>
#include <stdlib.h>
#include "utilidades.h"
#include "registro_utils.h"





int main()
{
int opcion_menu1 = 0;
    do{
    system("cls");
    printf("========AMADENT========\n");
    printf("1.Iniciar sesion\n");
    printf("3.Salir \n");
    printf("Seleccione una opcion: ");

    opcion_menu1 = leer_opcion();






    switch(opcion_menu1){

    case 1:
		system("cls");
		pantalla_registro();
		pausar();
		break;

    case 2:
		salida();
		break;
    
        


    }


}while(opcion_menu1 != 3);



}
