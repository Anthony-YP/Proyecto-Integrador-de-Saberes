#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <windows.h> // Biblioteca para manejar comunicación serial en Windows
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//  Empezamos declarando y haciendo las funciones necesarias para el programa //
int Calcular_Numero_Dias(int meses, int dias, int anios)
{
    int N_Dias = 0;
    // Iteramos cada uno de los meses transcurridos y sumamos los dias de cada mes pasado a nuestra variable N_Dias encargada de almacenar los dias transcurridos //
    for (int i = 1; i <= meses; i++)
    {
        if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
        {
            N_Dias += 31;
        }
        else if (i == 4 || i == 6 || i == 9 || i == 11)
        {
            N_Dias += 30;
        }
        else
        {
            N_Dias += 28;
        }
    }

    // A los dias sumados de los meses que han pasado le sumamos los dias transcurridos del mes actual //
    N_Dias = N_Dias + dias;
    // Verificamos si el año es biciesto y de ser asi le agregamos un dia al resultado final de los dias trancurridos //
    if (anios % 4 == 0)
    {
        if (anios % 100 == 0)
        {
            if (anios % 400 == 0)
            {
                N_Dias = N_Dias + 1;
            }
        }
        else
        {
            N_Dias = N_Dias + 1;
        }
    }
    N_Dias+= 1;
    return N_Dias;
}

double Calcular_Angulo_Declinacion_Solar(int N_Dias)
{
    double Termino1, Termino2, DS_Radianes, Coseno, Declinacion_Solar;
    Termino1 = -23.44;
    Termino2 = (360.0 / 365.0) * (N_Dias + 10);
    DS_Radianes = Termino2 * (M_PI / 180.0);
    Coseno = cos(DS_Radianes);
    Declinacion_Solar = Termino1 * Coseno;
    return Declinacion_Solar;
}

double Ecuacion_Del_Tiempo(int N_Dias)
{
    double B, EoT;

    B = ((360.0 / 365.0) * (N_Dias - 81));
    B = B * (M_PI / 180.0);
    EoT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
    return EoT;
}

double Calcular_Tiempo_Solar_Verdadero(int horas, int minutos, double Longitud, double EoT)
{
    double Zona_Horaria;
    double Hora_Local, Longitud_Estandar, TSV;

    Hora_Local = horas + (minutos / 60.0);
    Zona_Horaria = -5;
    Longitud_Estandar = Zona_Horaria * 15;
    TSV = Hora_Local + (((4 * (Longitud - Longitud_Estandar)) + EoT) / 60.0);
    return TSV;
}

double Calcular_Angulo_Horario(double TSV)
{
    double Angulo_Horario;
    Angulo_Horario = 15 * (TSV - 12);
    Angulo_Horario = Angulo_Horario * (M_PI / 180.0);
    return Angulo_Horario;
}

double Calcular_Altitud_Solar(double Declinacion_Solar, double Latitud, double Angulo_Horario)
{
    double Altitud_Solar;
    Altitud_Solar = asin(sin(Declinacion_Solar) * sin(Latitud) + cos(Declinacion_Solar) * cos(Latitud) * cos(Angulo_Horario));
    return Altitud_Solar;
}

double Calcular_Azimut(double Declinacion_Solar, double Altitud_Solar, double Latitud, double Angulo_Horario)
{
    double Azimut;
    Azimut = acos((sin(Declinacion_Solar) - sin(Altitud_Solar) * sin(Latitud)) / (cos(Altitud_Solar) * cos(Latitud)));
    if (Angulo_Horario > 0)
    {
        Azimut = 2.0 * M_PI - Azimut;
    }
    return Azimut;
}

double Pedir_Datos(const char *mensaje)
{
    double valor;
    int resultado;

    while (1)
    {
        printf("%s", mensaje);
        resultado = scanf("%lf", &valor);

        // Limpiar el dato de entrada para manejar las entradas incorrectas
        while (getchar() != '\n');

        if (resultado == 1)
        {
            return valor;
        }
        else
        {
            printf("*** Dato invalido, por favor ingrese bien el dato que se a solicitado ****\n");
        }
    }
}

void vector_final(double *x2, double *y2, double *z2, double Azimut, double Altitud_Solar) {
    *x2 = cos(Altitud_Solar) * sin(Azimut);
    *y2 = cos(Altitud_Solar) * cos(Azimut);
    *z2 = sin(Altitud_Solar);
}

void vector_inicial(double *x1, double *y1, double *z1, double Longitud, double Latitud) {
    int horas, minutos, segundos, anios, meses, dias;
    time_t tiempoahora;
    time(&tiempoahora);
    struct tm *mitiempo = localtime(&tiempoahora);

    // Obtener la fecha actual del sistema
    anios = mitiempo->tm_year + 1900;
    meses = mitiempo->tm_mon + 1; // Meses desde 0
    dias = mitiempo->tm_mday;

    // Solicitar la hora, minutos y segundos al usuario
    printf("Por favor ingrese los siguientes datos para obtener la posicion inicial\n");
    printf("Ingrese la hora: ");
    scanf("%d", &horas);
    printf("Ingrese los minutos: ");
    scanf("%d", &minutos);
    printf("Ingrese los segundos: ");
    scanf("%d", &segundos);

    // Calcular el número de días del año
    int N_Dias = Calcular_Numero_Dias(meses - 1, dias, anios);
    double Declinacion_Solar = Calcular_Angulo_Declinacion_Solar(N_Dias);
    double EoT = Ecuacion_Del_Tiempo(N_Dias);
    double TSV = Calcular_Tiempo_Solar_Verdadero(horas, minutos, Longitud, EoT);
    double Angulo_Horario = Calcular_Angulo_Horario(TSV);
    double Altitud_Solar = Calcular_Altitud_Solar(Declinacion_Solar, Latitud, Angulo_Horario);
    double Azimut = Calcular_Azimut(Declinacion_Solar, Altitud_Solar, Latitud, Angulo_Horario);

    *x1 = cos(Altitud_Solar) * sin(Azimut);
    *y1 = cos(Altitud_Solar) * cos(Azimut);
    *z1 = sin(Altitud_Solar);
}

double angulo_placa(double x2, double x1, double y2, double y1, double z2, double z1) {
    double producto_punto = x2 * x1 + y2 * y1 + z2 * z1;
    double magnitud_inicial = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
    double magnitud_final = sqrt(x2 * x2 + y2 * y2 + z2 * z2);
    double cos_angulo = producto_punto / (magnitud_final * magnitud_inicial);
    cos_angulo = fmax(-1.0, fmin(1.0, cos_angulo));
    return acos(cos_angulo) * (180.0 / M_PI);
}

double Calcular_Angulo_Base(double azimutSolar) {
    double anguloBase = azimutSolar;
    if (anguloBase < 0) anguloBase = 0;
    if (anguloBase >= 360) anguloBase = 360;
    return anguloBase;
}

void ingreso_de_usuario(int *horas, int *minutos, int *segundos) {
    printf("Ingrese la hora: ");
    scanf("%d", horas);
    printf("Ingrese los minutos: ");
    scanf("%d", minutos);
    printf("Ingrese los segundos: ");
    scanf("%d", segundos);
}

void opcion_menu() {
    printf("MENU\n");
    printf("Opcion 1: Posicion automatica [1]\n");
    printf("Opcion 2: Posicion manual [2]\n");
}

void calcularhora(int *horas, int *minutos, int *segundos, int *anios, int *meses, int *dias) {
    time_t tiempoahora;
    time(&tiempoahora);
    struct tm *mitiempo = localtime(&tiempoahora);
    *horas = mitiempo->tm_hour;
    *minutos = mitiempo->tm_min;
    *segundos = mitiempo->tm_sec;
    *anios = mitiempo->tm_year + 1900;
    *meses = mitiempo->tm_mon;
    *dias = mitiempo->tm_mday;
}

void incluir_resultados(double Longitud, double Latitud, double x2, double y2, double z2, double x1, double y1, double z1, double angulo, int horas, int minutos, int segundos, int N_Dias, int anios, int meses, int dias, double Declinacion_Solar, double EoT, double TSV, double Angulo_Horario, double Altitud_Solar, double Orientacion_Solar, double Azimut, double Hora_Solar, double Minutos_Solar, int opcion) {

    if (opcion == 2) {
        ingreso_de_usuario(&horas, &minutos, &segundos);
    } else {
        calcularhora(&horas, &minutos, &segundos, &anios, &meses, &dias);
    }

    printf("====================================================\n");
    printf("// FECHA Y HORA ACTUAL DEL SISTEMA //\n");
    printf("Horas => %d\n", horas);
    printf("Minutos => %d\n", minutos);
    printf("Segundos => %d\n", segundos);
    printf("Anio => %d\n", anios);
    printf("Mes => %d\n", meses + 1);
    printf("Dia => %d\n", dias);

    // Iniciamos el Programa llamando y asando los valores a cada una de las funciones antes definidas //
    printf("====================================================\n");
    printf("// VERIFICACION DE DATOS //\n");

    // Calculamos los dias transcurridos desde que inicio el año hasta el dia actual usando la funcion correspondiente antes definida //
    N_Dias = Calcular_Numero_Dias(meses, dias, anios);
    printf("> Dias transcurridos desde que inicio el anio => %d\n", N_Dias);

    // Calculamos la Declinacion Solar usando la funcion correspondiente y pasando los parametros por valores necesarios para la operacion //
    Declinacion_Solar = Calcular_Angulo_Declinacion_Solar(N_Dias);
    printf("> Declinacion Solar => %f\n", Declinacion_Solar);

    // Calculamos la ecuacion del tiempo (EoT) y pasamos los parametros necesarios para el calculo //
    EoT = Ecuacion_Del_Tiempo(N_Dias);
    printf("> Ecuacion del tiempo => %f\n", EoT);

    // Calculamos el tiempo solar verdadero pasando como referencia los datos necesarios para el calculo //
    TSV = Calcular_Tiempo_Solar_Verdadero(horas, minutos, Longitud, EoT);
    Hora_Solar = trunc(TSV);
    Minutos_Solar = round(60 * (TSV - Hora_Solar));
    printf("> Tiempor solar verdadero => %f\n", TSV);

    // Calculamos el angulo horario usando como parametro el tiempo solar verdadero (TSV) //
    Angulo_Horario = Calcular_Angulo_Horario(TSV);
    printf("> Angulo Horario => %f\n", Angulo_Horario);

    // Pasamos la Declinacion Solar y la Latitud de grados a radianes //
    Declinacion_Solar = Declinacion_Solar * (M_PI / 180.0);
    Latitud = Latitud * (M_PI / 180.0);
    printf("> Declinacion Solar en radianes => %f\n", Declinacion_Solar);
    printf("> Latitud en radianes => %f\n", Latitud);

    // Calculamos la Altitud Solar en radianes //
    Altitud_Solar = Calcular_Altitud_Solar(Declinacion_Solar, Latitud, Angulo_Horario);
    // Pasamos la Altitud Solar a grados //
    Altitud_Solar = Altitud_Solar * (180.0 / M_PI);

    // Calculamos la orientacion Solar o Azimut en radianes//
    Azimut = Calcular_Azimut(Declinacion_Solar, Altitud_Solar * (M_PI / 180.0), Latitud, Angulo_Horario);
    // Pasamos la orientacion solar a grados //
    Azimut = Azimut * (180.0 / M_PI);

    // Finalmente presentamos los resultados de altitud y orientacion del sol actual //
    printf("====================================================\n");
    printf("// RESULTADOS FINALES DE ALTITUD DEL SOL Y SU ORIENTACION //\n");
    printf("> La posicion de la altitud del sol es de => %f grados de elevacion desde el piso\n", Altitud_Solar);
    printf("> La posicion de la orientacion del sol es de => %f grados desde el norte\n", Azimut);
    printf("========================================================\n");

    printf("====================================================\n");
    printf("// ROTACION DE LOS SERVO MOTORES //\n");

    vector_final(&x2, &y2, &z2, Azimut, Altitud_Solar);
    angulo = angulo_placa(x2, x1, y2, y1, z2, z1);
    printf("> El angulo de rotacion de la placa fotovoltaica es: %f\n", angulo);

    printf("====================================================\n");
    printf("// ROTACION DE LA BASE //\n");
    Azimut = Calcular_Angulo_Base(Azimut);
    printf("> El angulo de rotacion de la base es: %f\n", Azimut);
}

void Comunicacion_Serial()
{
}

int main() {
    double Longitud, Latitud, x2, y2, z2, x1, y1, z1, angulo;
    int horas, minutos, segundos, N_Dias, anios, meses, dias, opcion, contador, servidor;
    double Declinacion_Solar, EoT, TSV, Angulo_Horario, Altitud_Solar, Orientacion_Solar, Azimut, Hora_Solar, Minutos_Solar;

    printf("// Bienvenido, Por favor ingrese su Longitud y Latitud actual //\n");
    Longitud = Pedir_Datos("1. Longitud (En un rango de -180 a 180 grados) => ");
    Latitud = Pedir_Datos("2. Latitud (En un rango de -90 a 90 grados) => ");
    vector_inicial(&x1, &y1, &z1, Longitud, Latitud);
    char salida;

    while (salida != 'S' || salida != 's')
    {   
    opcion_menu();
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    if (opcion == 1) {       
        while (contador < 288 )
        {        
        incluir_resultados(Longitud, Latitud, x2, y2, z2, x1, y1, z1, angulo, horas, minutos, segundos, N_Dias, anios, meses, dias, Declinacion_Solar, EoT, TSV, Angulo_Horario, Altitud_Solar, Orientacion_Solar, Azimut, Hora_Solar, Minutos_Solar, opcion);
        contador++;
        sleep(10);
        }
    } else if (opcion == 2) {
        incluir_resultados(Longitud, Latitud, x2, y2, z2, x1, y1, z1, angulo, horas, minutos, segundos, N_Dias, anios, meses, dias, Declinacion_Solar, EoT, TSV, Angulo_Horario, Altitud_Solar, Orientacion_Solar, Azimut, Hora_Solar, Minutos_Solar, opcion);
    } else {
        printf("Opcion no valida. Intente de nuevo.\n");
    }
    printf("Si desea salir presione S o s sino presione cualquier tecla para continuar\n");
    scanf("%s", &salida);
    servidor++;
    }
    return 0;
}
