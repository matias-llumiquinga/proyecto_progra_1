#ifndef FUNCIONES_H
#define FUNCIONES_H

#define ZONAS 5
#define DIAS_MES 30

typedef struct {
    char nombre[50];
    float contaminantes[DIAS_MES];
    float actual;
    float prediccion;
    float limite_PM25;
    float limite_NO2;
    float limite_SO2;
    float limite_CO;
} QuitoZona;

void inicializarZonas(QuitoZona zonas[]);
void mostrarFechaHoraActual();
void calcularPromediosHistoricos(QuitoZona *zona);
void predecirContaminacion(QuitoZona zonas[], float factoresClimaticos[3]);
void generarAlertas(QuitoZona zonas[]);
void generarRecomendaciones(QuitoZona zonas[]);
void exportarReporte(QuitoZona zonas[], const char *nombreArchivo);
void editarLimitesHistoricos(QuitoZona zonas[]);
int buscarZona(QuitoZona zonas[], char nombre[50]);


#endif
