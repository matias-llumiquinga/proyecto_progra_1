#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funciones.h"

void inicializarZonas(QuitoZona zonas[])
{
    const char *nombres[ZONAS] = {"Nayon", "Granados", "Inaquito", "Carcelen", "El Bosque"};
    for (int i = 0; i < ZONAS; i++)
    {
        strcpy(zonas[i].nombre, nombres[i]);
        for (int j = 0; j < DIAS_MES; j++)
        {
            zonas[i].contaminantes[j] = rand() % 50 + 1;
        }
        zonas[i].actual = 0.0;
        zonas[i].prediccion = 0.0;
        zonas[i].limite_PM25 = 25.0;
        zonas[i].limite_NO2 = 40.0;
        zonas[i].limite_SO2 = 20.0;
        zonas[i].limite_CO = 10.0;
    }
}

void mostrarFechaHoraActual()
{
    time_t t;
    struct tm *tm;
    t = time(NULL);
    tm = localtime(&t);
    printf("Fecha y hora actual: %02d/%02d/%04d %02d:%02d:%02d\n",
           tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
           tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void calcularPromediosHistoricos(QuitoZona *zona)
{
    float suma = 0.0;
    for (int i = 0; i < DIAS_MES; i++)
    {
        suma += zona->contaminantes[i];
    }
    float promedio = suma / DIAS_MES;
    printf("Promedio mensual de %s: %.2f (Limite: %.2f)\n", zona->nombre, promedio, zona->limite_PM25);
}

void predecirContaminacion(QuitoZona zonas[], float factoresClimaticos[3])
{
    for (int i = 0; i < ZONAS; i++)
    {
        float ponderado = 0.0;
        for (int j = 0; j < DIAS_MES; j++)
        {
            float peso = (float)(j + 1) / DIAS_MES;
            ponderado += zonas[i].contaminantes[j] * peso;
        }
        zonas[i].prediccion = ponderado / DIAS_MES +
                              factoresClimaticos[0] * 0.1 +
                              factoresClimaticos[1] * 0.2 -
                              factoresClimaticos[2] * 0.3;
    }
}

void generarAlertas(QuitoZona zonas[])
{
    for (int i = 0; i < ZONAS; i++)
    {
        printf("\nGenerando alerta para la zona %s...\n", zonas[i].nombre);

        if (zonas[i].prediccion > zonas[i].limite_PM25)
        {
            printf("ALERTA: Zona %s superara el limite de PM2.5. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_PM25);
        }

        if (zonas[i].prediccion > zonas[i].limite_NO2)
        {
            printf("ALERTA: Zona %s superara el limite de NO2. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_NO2);
        }

        if (zonas[i].prediccion > zonas[i].limite_SO2)
        {
            printf("ALERTA: Zona %s superara el limite de SO2. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_SO2);
        }

        if (zonas[i].prediccion > zonas[i].limite_CO)
        {
            printf("ALERTA: Zona %s superara el limite de CO. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_CO);
        }
    }
}

void generarRecomendaciones(QuitoZona zonas[])
{
    for (int i = 0; i < ZONAS; i++)
    {
        printf("\nGenerando recomendaciones para la zona %s...\n", zonas[i].nombre);

        if (zonas[i].prediccion > zonas[i].limite_PM25)
        {
            printf("ALERTA PM2.5: La zona %s ha superado el limite de PM2.5. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_PM25);
            printf("Recomendaciones:\n");
            printf("- Reducir el trafico vehicular en las cercanías.\n");
            printf("- Restringir actividades industriales en la zona.\n");
            printf("- Evitar que personas vulnerables salgan al aire libre.\n");
        }

        if (zonas[i].prediccion > zonas[i].limite_NO2)
        {
            printf("ALERTA NO2: La zona %s ha superado el limite de NO2. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_NO2);
            printf("Recomendaciones:\n");
            printf("- Mejorar la calidad de los combustibles utilizados en la zona.\n");
            printf("- Incrementar el transporte público y la movilidad sostenible.\n");
            printf("- Reforzar el monitoreo y control de emisiones industriales.\n");
        }

        if (zonas[i].prediccion > zonas[i].limite_SO2)
        {
            printf("ALERTA SO2: La zona %s ha superado el limite de SO2. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_SO2);
            printf("Recomendaciones:\n");
            printf("- Suspender actividades que generen SO2, como la quema de combustibles fósiles.\n");
            printf("- Implementar medidas de filtración de gases en las industrias cercanas.\n");
            printf("- Evitar la exposición de la población vulnerable a la contaminación.\n");
        }

        if (zonas[i].prediccion > zonas[i].limite_CO)
        {
            printf("ALERTA CO: La zona %s ha superado el limite de CO. Prediccion: %.2f (Limite: %.2f)\n",
                   zonas[i].nombre, zonas[i].prediccion, zonas[i].limite_CO);
            printf("Recomendaciones:\n");
            printf("- Evitar el uso de vehículos de motor durante las horas pico.\n");
            printf("- Aumentar el número de estaciones de monitoreo de CO en la zona.\n");
            printf("- Implementar planes de evacuación para las zonas de mayor concentración de CO.\n");
        }
    }
}

void exportarReporte(QuitoZona zonas[], const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL)
    {
        perror("Error al crear el archivo");
        return;
    }
    fprintf(archivo, "Reporte de contaminacion\n");
    for (int i = 0; i < ZONAS; i++)
    {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombre);
        fprintf(archivo, "Nivel actual: %.2f\n", zonas[i].actual);
        fprintf(archivo, "Prediccion: %.2f\n", zonas[i].prediccion);
    }
    fclose(archivo);
    printf("Reporte guardado en %s\n", nombreArchivo);
}

void editarLimitesHistoricos(QuitoZona zonas[])
{
    char nombre[50];
    int encontrado = 0, flag=0;
    float valor = 0.0;

    printf("\n--- EDITAR LIMITES HISTORICOS DE CONTAMINANTES ---\n");
    encontrado = buscarZona(zonas, nombre);

    if (encontrado == 1)
    {
        for (int i = 0; i < ZONAS; i++)
        {
            if (strcmp(zonas[i].nombre, nombre) == 0)
            {
                do
                {
                    printf("\nLimite actual de PM2.5 para la zona %s: %.2f. Ingrese nuevo limite: ", zonas[i].nombre, zonas[i].limite_PM25);
                    scanf("%f", &valor);
                    if (valor <= 0)
                    {
                        printf("No se aceptan limites negativos o nulos.\n");
                    }
                    else
                    {
                        zonas[i].limite_PM25 = valor;
                        flag=1;
                    }
                } while (flag == 0);

                valor=0;
                flag=0;

                do
                {
                    printf("\nLimite actual de NO2 para la zona %s: %.2f. Ingrese nuevo limite: ", zonas[i].nombre, zonas[i].limite_NO2);
                    scanf("%f", &valor);
                    if (valor <= 0)
                    {
                        printf("No se aceptan limites negativos o nulos.\n");
                    }
                    else
                    {
                        zonas[i].limite_NO2 = valor;
                        flag=1;
                    }
                } while (flag == 0);

                valor=0;
                flag=0;

                do
                {
                    printf("\nLimite actual de SO2 para la zona %s: %.2f. Ingrese nuevo limite: ", zonas[i].nombre, zonas[i].limite_SO2);
                    scanf("%f", &valor);
                    if (valor <= 0)
                    {
                        printf("No se aceptan limites negativos o nulos.\n");
                    }
                    else
                    {
                        zonas[i].limite_SO2 = valor;
                        flag=1;
                    }
                } while (flag == 0);

                valor=0;
                flag=0;

                do
                {
                    printf("\nLimite actual de CO para la zona %s: %.2f. Ingrese nuevo limite: ", zonas[i].nombre, zonas[i].limite_CO);
                    scanf("%f", &valor);
                    if (valor <= 0)
                    {
                        printf("No se aceptan limites negativos o nulos.\n");
                    }
                    else
                    {
                        zonas[i].limite_CO = valor;
                        flag=1;
                    }
                } while (flag == 0);

                valor=0;
            }
        }

        printf("\nLimites actualizados correctamente.\n");
    }
}

int buscarZona(QuitoZona zonas[], char nombre[50])
{
    printf("--- BUSCAR ZONA ---\n");
    int len = 0;
    do
    {
        strcpy(nombre, "");
        printf("Ingrese el nombre de la zona a buscar: ");
        while (nombre[0] == '\0' || nombre[0] == '\n')
        {
            fgets(nombre, 50, stdin);
        }
        len = strlen(nombre) - 1;
        if (nombre[len] != '\n')
        {
            printf("Ha excecido el limite de caracteres.\n");
            while (getchar() != '\n')
                ;
        }

    } while (nombre[len] != '\n');

    nombre[len] = '\0';

    for (int i = 0; i < ZONAS; i++)
    {
        if (strcmp(zonas[i].nombre, nombre) == 0)
        {
            printf("Zona %s encontrada.\n", nombre);
            return 1;
        }
    }
    printf("Zona %s no encontrada.\n", nombre);
    return 0;
}