#include "utilidades.h"
#include <stdio.h>
#include <stdlib.h>

void pausar(){
while (getchar() != '\n');
printf("Presione Enter para continuar...");
getchar();

}



void continuar(){

printf("Presione Enter para continuar...");
getchar();
system("cls");
}


//Herramienta para ller opciones en rangos definidos
int leer_opcion(){

    int opcion;
    while(1){
    if(scanf("%d", &opcion) == 1){

    if ( opcion < 1 || opcion > 3) {
            printf("Opcion invalida.\n");
            // limpiar scanf

    pausar();
    return -1;

}
        pausar();
        return opcion;

    }
 // Si llega aquí, la lectura fue inválida (letras o basura)
        printf("Error: ingrese solo números!\n");
        printf("Intente nuevamente: ");

        // Limpiar buffer
        while (getchar() != '\n');

}


}



//herramienta para escribir en un archivo


