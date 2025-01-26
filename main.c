#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main() {
    int opc=0;
    /*struct ZonasQuito zonas[ZONAS];
    float factoresClimaticos[3] = {0.5, 0.3, 0.2};*/

    inicializarZonas();
    exportarReporte();
    //calcularDiasTranscurridos();
    do
    {
        opc = menuIngresarDatosActuales();
    } while (opc != 7);
    
    
    

    /*int opcion;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Calcular promedios historicos\n");
        printf("2. Predecir contaminacion\n");
        printf("3. Editar limites historicos\n");
        printf("4. Generar alertas\n");
        printf("5. Generar recomendaciones\n");
        printf("6. Exportar reporte\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                for (int i = 0; i < ZONAS; i++) {
                    calcularPromediosHistoricos(&zonas[i]);
                }
                break;
            case 2:
                predecirContaminacion(zonas, factoresClimaticos);
                printf("Contaminacion predicha.\n");
                break;
            case 3:
                editarLimitesHistoricos(zonas);
                break;
            case 4:
                generarAlertas(zonas);
                break;
            case 5:
                generarRecomendaciones(zonas);
                break;
            case 6:
                exportarReporte(zonas, "reporte_contaminacion.txt");
                break;
            case 7:
                printf("Saliendo...\n");
                exit(0);
            default:
                printf("Opcion no valida.\n");
        }
    }*/

    return 0;
}
