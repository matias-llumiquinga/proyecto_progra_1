#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funciones.h"

void SaveDatosZona(struct ZonasQuito *zonas, int nuevosDatos)
{
    FILE *file = fopen(NOMBRE_ARCHIVO_REGISTRO, "ab+");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    else
    {
        fwrite(zonas, sizeof(struct ZonasQuito), 1, file); // putero de estructura, tamaño de la estructura, cantidad de estructuras, archivo

        if (nuevosDatos == 1)
        {
            printf("Datos guardados con exito\n");
        }
    }

    fclose(file);
}
void inicializarZonas()
{
    const char *nombres[ZONAS] = {"NAYON", "GRANADOS", "INAQUITO", "CARCELEN", "EL BOSQUE"};

    FILE *file = fopen(NOMBRE_ARCHIVO_REGISTRO, "rb");
    if (file == NULL)
    {
        printf("El archivo no existe, creandolo...\n");
        file = fopen(NOMBRE_ARCHIVO_REGISTRO, "wb"); // Abrir en modo escritura binaria
        if (file == NULL)
        {
            printf("Error al crear el archivo\n");
            return;
        }
    }
    fseek(file, 0, SEEK_END);
    int length = ftell(file);

    if (length > 0)
    {
        return;
    }
    else
    {
        struct ZonasQuito zonas[ZONAS] = {0};
        int diasTranscurridos = calcularDiasTranscurridos();

        for (int i = 0; i < ZONAS; i++)
        {
            strcpy(zonas[i].nombre, nombres[i]);
            for (int j = 0; j < diasTranscurridos - 1; j++)
            {
                zonas[i].factoresClimaticos[j].temperatura = rand() % 40 + 1;
                zonas[i].factoresClimaticos[j].humedad = rand() % 100 + 1;
                zonas[i].factoresClimaticos[j].velocidadViento = rand() % 100 + 1;
                zonas[i].contaminantesRegistrados[j].PM25 = rand() % 50 + 1;
                zonas[i].contaminantesRegistrados[j].NO2 = rand() % 50 + 1;
                zonas[i].contaminantesRegistrados[j].SO2 = rand() % 50 + 1;
                zonas[i].contaminantesRegistrados[j].CO2 = rand() % 50 + 1;
                zonas[i].contaminantesPredecidos[j].PM25 = 0.0;
                zonas[i].contaminantesPredecidos[j].NO2 = 0.0;
                zonas[i].contaminantesPredecidos[j].SO2 = 0.0;
                zonas[i].contaminantesPredecidos[j].CO2 = 0.0;
                zonas[i].autoRegistro[j] = 1;
            }
            zonas[i].limitesContaminantes.limite_PM25 = 25.0;
            zonas[i].limitesContaminantes.limite_NO2 = 40.0;
            zonas[i].limitesContaminantes.limite_SO2 = 20.0;
            zonas[i].limitesContaminantes.limite_CO2 = 10.0;

            SaveDatosZona(&zonas[i], 0);
        }
    }

    fclose(file);
}
int calcularDiasTranscurridos()
{
    int dia = obtenerDiaActual();
    int mes = obtenerMesActual();
    int diasPorMes[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int diasTranscurridos = 0;
    for (int i = 0; i < mes - 1; i++)
    {
        diasTranscurridos += diasPorMes[i];
    }
    diasTranscurridos += dia;
    // printf("Fecha actual: %d/%d\n", dia, mes);
    return diasTranscurridos;
}

int obtenerDiaActual()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday;
}
int obtenerMesActual()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mon + 1;
}
int obtenerAnioActual()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900;
}

void exportarReporte()
{
    FILE *fileBinary = fopen(NOMBRE_ARCHIVO_REGISTRO, "rb");
    struct ZonasQuito zonas[ZONAS];
    struct tm fecha = {0};
    int diasTranscurridos = calcularDiasTranscurridos();

    if (fileBinary == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    else
    {
        fread(zonas, sizeof(struct ZonasQuito), ZONAS, fileBinary);

        for (int i = 0; i < ZONAS; i++)
        {
            char nombreArchivo[50];
            fecha.tm_year = obtenerAnioActual();
            fecha.tm_mon = 0;
            fecha.tm_mday = 1;

            snprintf(nombreArchivo, sizeof(nombreArchivo), "_REPORTE_DE_%s.txt", zonas[i].nombre);
            FILE *fileText = fopen(nombreArchivo, "w");
            if (fileText == NULL)
            {
                printf("Error al abrir el archivo\n");
                continue;
            }
            fprintf(fileText, "%s : REPORTE DE ZONAS CLIMATICAS Y CONTAMINANTES EN QUITO\n", zonas[i].nombre);
            fprintf(fileText, "------------------------------------------------------------------\n");

            for (int j = 0; j < diasTranscurridos; j++)
            {
                mktime(&fecha);
                fprintf(fileText, "---------------------------%02d/%02d/%d---------------------------\n", fecha.tm_mday, fecha.tm_mon + 1, fecha.tm_year);
                fprintf(fileText, "Temperatura:\t\t\t%.2f\n", zonas[i].factoresClimaticos[j].temperatura);
                fprintf(fileText, "Humedad:\t\t\t\t%.2f\n", zonas[i].factoresClimaticos[j].humedad);
                fprintf(fileText, "Velocidad del viento:\t%.2f\n", zonas[i].factoresClimaticos[j].velocidadViento);

                if (zonas[i].autoRegistro[j] == 1)
                {
                    fprintf(fileText, "PM2.5:\t%.2f\n", zonas[i].contaminantesRegistrados[j].PM25);
                    fprintf(fileText, "NO2:\t%.2f\n", zonas[i].contaminantesRegistrados[j].NO2);
                    fprintf(fileText, "SO2:\t%.2f\n", zonas[i].contaminantesRegistrados[j].SO2);
                    fprintf(fileText, "CO2:\t%.2f\n", zonas[i].contaminantesRegistrados[j].CO2);
                }
                else
                {
                    fprintf(fileText, "PM2.5 actual:\t%.2f\n", zonas[i].contaminantesRegistrados[j].PM25);
                    fprintf(fileText, "NO2 actual:\t\t%.2f\n", zonas[i].contaminantesRegistrados[j].NO2);
                    fprintf(fileText, "SO2 actual:\t\t%.2f\n", zonas[i].contaminantesRegistrados[j].SO2);
                    fprintf(fileText, "CO2 actual:\t\t%.2f\n", zonas[i].contaminantesRegistrados[j].CO2);

                    fprintf(fileText, "PM2.5 predicho en 24h:\t%.2f\n", zonas[i].contaminantesPredecidos[j].PM25);
                    fprintf(fileText, "NO2 predicho en 24h:\t%.2f\n", zonas[i].contaminantesPredecidos[j].NO2);
                    fprintf(fileText, "SO2 predicho en 24h:\t%.2f\n", zonas[i].contaminantesPredecidos[j].SO2);
                    fprintf(fileText, "CO2 predicho en 24h:\t%.2f\n", zonas[i].contaminantesPredecidos[j].CO2);
                }

                fecha.tm_mday++;
            }

            fclose(fileText);
        }
    }
    fclose(fileBinary);

    printf("-REPORTES GENERADOS-\n");
}

void ingresarDatosActuales(int indiceZona)
{
    FILE *fileBinary = fopen(NOMBRE_ARCHIVO_REGISTRO, "rb+");
    struct ZonasQuito zonas[ZONAS];
    int diasTranscurridos = calcularDiasTranscurridos();
    diasTranscurridos = 0;

    if (fileBinary == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    else
    {
        fread(zonas, sizeof(struct ZonasQuito), ZONAS, fileBinary);

        if (zonas[indiceZona].autoRegistro[diasTranscurridos] == -1)
        {
            printf("Ya se han ingresado datos para la zona %s en la fecha actual.\n", zonas[indiceZona].nombre);
            return;
        }
        else
        {
            int posicion = indiceZona * sizeof(struct ZonasQuito);
            fseek(fileBinary, posicion, SEEK_SET);
            /*printf("-INGRESE LOS DATOS PARA LA ZONA %s-\n", zonaAModificar.nombre);
            printf("Temperatura:\t\t");
            scanf("%f", &zonaAModificar.factoresClimaticos[diasTranscurridos].temperatura);
            printf("Humedad:\t\t");
            scanf("%f", &zonaAModificar.factoresClimaticos[diasTranscurridos].humedad);
            printf("Velocidad del viento:\t");
            scanf("%f", &zonaAModificar.factoresClimaticos[diasTranscurridos].velocidadViento);
            printf("PM2.5:\t");
            scanf("%f", &zonaAModificar.contaminantesRegistrados[diasTranscurridos].PM25);
            printf("NO2:\t");
            scanf("%f", &zonaAModificar.contaminantesRegistrados[diasTranscurridos].NO2);
            printf("SO2:\t");
            scanf("%f", &zonaAModificar.contaminantesRegistrados[diasTranscurridos].SO2);
            printf("CO2:\t");
            scanf("%f", &zonaAModificar.contaminantesRegistrados[diasTranscurridos].CO2);*/
            strcpy(zonas[indiceZona].nombre, "oliva");
            zonas[indiceZona].factoresClimaticos[diasTranscurridos].temperatura = 40. + 1;
            zonas[indiceZona].factoresClimaticos[diasTranscurridos].humedad = 100. + 1;
            zonas[indiceZona].factoresClimaticos[diasTranscurridos].velocidadViento = 100. + 1;
            zonas[indiceZona].contaminantesRegistrados[diasTranscurridos].PM25 = 50. + 1;
            zonas[indiceZona].contaminantesRegistrados[diasTranscurridos].NO2 = 50. + 1;
            zonas[indiceZona].contaminantesRegistrados[diasTranscurridos].SO2 = 50. + 1;
            zonas[indiceZona].contaminantesRegistrados[diasTranscurridos].CO2 = 50. + 1;
            zonas[indiceZona].autoRegistro[diasTranscurridos] = -1;

            SaveDatosZona(&zonas[indiceZona], 1);
            printf("NOMBRE: %s\n", zonas[0].nombre);
            printf("NOMBRE: %s\n", zonas[1].nombre);
            printf("NOMBRE: %s\n", zonas[2].nombre);
            printf("NOMBRE: %s\n", zonas[3].nombre);
            printf("NOMBRE: %s\n", zonas[4].nombre);
        }
    }
    fclose(fileBinary);
}

int menuIngresarDatosActuales()
{
    FILE *fileBinary = fopen(NOMBRE_ARCHIVO_REGISTRO, "rb");
    struct ZonasQuito zonas[ZONAS];
    struct tm fecha = {0};
    int diasTranscurridos = calcularDiasTranscurridos();
    int opc = 0;

    if (fileBinary == NULL)
    {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    else
    {
        fread(zonas, sizeof(struct ZonasQuito), ZONAS, fileBinary);

        printf("--- INGRESAR DATOS ACTUALES ---\n");
        printf("1. %s\n", zonas[0].nombre);
        printf("2. %s\n", zonas[1].nombre);
        printf("3. %s\n", zonas[2].nombre);
        printf("4. %s\n", zonas[3].nombre);
        printf("5. %s\n", zonas[4].nombre);
        printf("6. TODOS\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        // scanf("%d", &opc);
        if (scanf("%d", &opc) != 1)
        {
            printf("Entrada no válida. Por favor ingrese un número.\n");
            opc = 0; // Para forzar la repetición del menú
            while (getchar() != '\n')
                ; // Limpiar el buffer
        }

        switch (opc)
        {
        case 1:
            ingresarDatosActuales(0);
            break;
        case 2:
            ingresarDatosActuales(1);
            break;
        case 3:
            ingresarDatosActuales(2);
            break;
        case 4:
            ingresarDatosActuales(3);
            break;
        case 5:
            ingresarDatosActuales(4);
            break;
        case 6:
            ingresarDatosActuales(0);
            ingresarDatosActuales(1);
            ingresarDatosActuales(2);
            ingresarDatosActuales(3);
            ingresarDatosActuales(4);
            break;
        case 7:
            printf("Regresando al menu principal...\n");
            break;
        default:
            printf("Opcion no valida.\n");
        }
    }
    return opc;
}

/*
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
}*/