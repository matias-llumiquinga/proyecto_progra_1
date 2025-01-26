#define ZONAS 5
#define DIAS_MES 30
#define DIAS_ANIO 365
#define  NOMBRE_ARCHIVO_REGISTRO "__REGISTRO_DATOS_HISTORICOS_.data"

struct FactoresClimaticos
{
    float temperatura;
    float humedad;
    float velocidadViento;
};

struct Contaminantes
{
    float PM25;
    float NO2;
    float SO2;
    float CO2;
};

struct LimitesContaminantes
{
    float limite_PM25;
    float limite_NO2;
    float limite_SO2;
    float limite_CO2;
};


struct ZonasQuito{
    char nombre[50];
    struct FactoresClimaticos factoresClimaticos[DIAS_ANIO];
    struct Contaminantes contaminantesRegistrados[DIAS_ANIO];
    struct Contaminantes contaminantesPredecidos[DIAS_ANIO];
    struct LimitesContaminantes limitesContaminantes;
    int autoRegistro[DIAS_ANIO];
    //float contaminantes[DIAS_MES];
    //float actual;
    //float prediccion;
    /*float limite_PM25;
    float limite_NO2;
    float limite_SO2;
    float limite_CO;*/
};

void SaveDatosZona(struct ZonasQuito *zonas, int nuevosDatos);
void inicializarZonas();
int calcularDiasTranscurridos();
void exportarReporte();
void ingresarDatosActuales(int indiceZona);
int menuIngresarDatosActuales();



int obtenerDiaActual();
int obtenerMesActual();
int obtenerAnioActual();


/*void inicializarZonas(QuitoZona zonas[]);
void mostrarFechaHoraActual();
void calcularPromediosHistoricos(QuitoZona *zona);
void predecirContaminacion(QuitoZona zonas[], float factoresClimaticos[3]);
void generarAlertas(QuitoZona zonas[]);
void generarRecomendaciones(QuitoZona zonas[]);
void exportarReporte(QuitoZona zonas[], const char *nombreArchivo);
void editarLimitesHistoricos(QuitoZona zonas[]);
int buscarZona(QuitoZona zonas[], char nombre[50]);
*/