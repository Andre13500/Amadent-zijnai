#include <stdio.h>
#include <stdlib.h>
#include "utilidades.h"
#include "registro_utils.h"




void iniciarSesion();
int main()
{
int opcion_menu1 = 0;
    do{
    system("cls");
    printf("========AMADENT========\n");
    printf("1.Iniciar sesion\n");
    printf("2.Salir \n");
    printf("Seleccione una opcion: ");

    opcion_menu1 = leer_opcion(1,2);






    switch(opcion_menu1){

    case 1:
		system("cls");
		iniciarSesion();
		pausar();
		break;

    case 2:
		salida();
		break;
    
        


    }


}while(opcion_menu1 != 2);


return 0;
}
