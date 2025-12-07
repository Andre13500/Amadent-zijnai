#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "registro_utils.h"
#include "utilidades.h"


void pantalla_registro(void) {  // ← Añade (void)

  FILE *Registros, *temporal;
    datos_registro registros;
    int opcion, idbus, encontro;

do{


    printf("\nBienvenido, escoge una opcion\n");
        printf("1. Agregar usuario\n");
        printf("2. consultar usuario\n");
        printf("3. Eliminar usuario\n");
        printf("4.Volver al menu principal");
        printf("5. Salir\n");

      printf("Ingrese su opción: ");


      if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida.\n");
            return;
        }
    getchar();

    switch(opcion){
case 1:

 system("cls");

    Registros = fopen("Registros_usuarios.dat", "ab");
                if (Registros == NULL) {
                    printf("Error al abrir el archivo.\n");
                    return;
                }

                printf("Ingrese su ID: ");
                if (scanf("%d", &registros.id_paciente) != 1) {
                    printf("Error: Entrada no válida.\n");
                    break;
                }
                getchar();

                printf("Ingrese el nombre: ");
                fgets(registros.nombre, 100, stdin);

                printf("Ingrese su email: ");
               fgets(registros.email, 100, stdin);

                printf("Ingrese su cedula : ");
                fgets(registros.cedula, 100, stdin);

                fwrite(&registros, sizeof(registros), 1, Registros);
                fclose(Registros);
                printf("Producto agregado correctamente.\n");
                continuar();
                break;

 case 2:

     system("cls");
                Registros = fopen("Registros_usuarios.dat", "rb");
                if (Registros == NULL) {
                    printf("No se pudo abrir el inventario\n");
                    break;
                }

                printf("\nPresentacion de los datos\n");
                while (fread(&registros, sizeof(registros), 1, Registros)) {
                    printf("ID: %d\n------------\nNombre: %s\n------------\nEmail: %s\n------------\nCedula: %s\n------------------------\n",
                           registros.id_paciente, registros.nombre, registros.email, registros.cedula);
                }
                fclose(Registros);

                continuar();
                break;

            case 3:

                system("cls");


                Registros = fopen("Registros_usuarios.dat", "rb");
                temporal = fopen("temporal.dat", "wb");

                if (Registros == NULL || temporal == NULL) {
                    printf("Error al abrir archivos.\n");
                    return;
                }

                printf("Ingrese ID a eliminar: ");
                scanf("%d", &idbus);

                encontro = 0;
                while (fread(&registros, sizeof(registros), 1, Registros)) {
                    if (registros.id_paciente != idbus)
                        fwrite(&registros, sizeof(registros), 1, temporal);
                    else
                        encontro = 1;
                }

                fclose(Registros);
                fclose(temporal);
                remove("Registros_usuarios.dat");
                rename("temporal.dat", "Registros_usuarios.dat");

                if (encontro)
                    printf("Producto eliminado.\n");
                else
                    printf("No existe un producto con ese ID.\n");
                    continuar();
                break;



                break;
            case 4:
                continuar();
                return;

            default:
                printf("Opción no válida, intente de nuevo.\n");
        }



}while(opcion != 4);


}








