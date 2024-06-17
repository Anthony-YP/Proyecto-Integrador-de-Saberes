// Programa para calcular la posición solar a partir de la longitud y latitud ingresadas por el usuario //
/**
 * @file Calcular_Posición_Solar.c
 * @author Anthony_YP (anthonyjoseyp@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-15
 * @copyright Copyright (c) 2024
 */

// Importamos las librerias necesarias para facilitarnos la programacion //
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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
        switch (i)
        {
        case 1:
            N_Dias = N_Dias + 31;
            break;
        case 2:
            N_Dias = N_Dias + 28;
            break;
        case 3:
            N_Dias = N_Dias + 31;
            break;
        case 4:
            N_Dias = N_Dias + 30;
            break;
        case 5:
            N_Dias = N_Dias + 31;
            break;
        case 6:
            N_Dias = N_Dias + 30;
            break;
        case 7:
            N_Dias = N_Dias + 31;
            break;
        case 8:
            N_Dias = N_Dias + 31;
            break;
        case 9:
            N_Dias = N_Dias + 30;
            break;
        case 10:
            N_Dias = N_Dias + 31;
            break;
        case 11:
            N_Dias = N_Dias + 30;
            break;
        case 12:
            N_Dias = N_Dias + 31;
            break;
        }
    }
    // A los dias sumados de los meses que han pasado le sumamos los dias transcurridos del mes actual //
    N_Dias = N_Dias + dias;
    // Verificamos si el año es biciesto y de serlo le agregamos un dia al resultado final de los dias trancurridos //
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
    Zona_Horaria = -5.0;
    Longitud_Estandar = Zona_Horaria * 15.0;
    TSV = Hora_Local + (((4.0 * (Longitud - Longitud_Estandar)) + EoT) / 60.0);
    return TSV;
}

double Calcular_Angulo_Horario(double TSV)
{
    double Angulo_Horario;
    Angulo_Horario = 15.0 * (TSV - 12.0);
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

int main()
{
    system("@cls||clear");
    // Definimos todas las variables necesarias para nuestro programa //
    int horas, minutos, segundos;
    int anios, meses, dias;
    int N_Dias;
    double Longitud, Latitud;
    double Declinacion_Solar, EoT, TSV, Angulo_Horario, Altitud_Solar, Orientacion_Solar, Azimut, Hora_Solar, Minutos_Solar;
    /* Recuperamos la fecha y hora actual del sistema con ayuda de la libreria "time.h" y metemos cada uno de los datos en su respectiva
    variable*/
    time_t tiempoahora;
    time(&tiempoahora);
    struct tm *mitiempo = localtime(&tiempoahora);
    horas = mitiempo->tm_hour;
    minutos = mitiempo->tm_min;
    segundos = mitiempo->tm_sec;
    anios = mitiempo->tm_year + 1900;
    meses = mitiempo->tm_mon;
    dias = mitiempo->tm_mday;

    // Solicitamos al usuario su Longitud y Latitud actual //
    printf("// Bienvenido. Por favor ingrese su Longitud y Latitud //\n");
    printf("1. Longitud => ");
    scanf("%lf", &Longitud);
    printf("2. Latitud => ");
    scanf("%lf", &Latitud);
    getchar();
    // Presentamos las varibales de hora y fecha para verificar que los datos esten correctos //
    printf("====================================================\n");
    printf("// FECHA Y HORA ACTUAL DEL SISTEMA //\n");
    printf("Horas => %d\n", horas);
    printf("Minutos => %d\n", minutos);
    printf("Segundos => %d\n", segundos);
    printf("Anio => %d\n", anios);
    printf("Mes => %d\n", meses);
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
    Minutos_Solar = round(60.0 * (TSV - Hora_Solar));
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
    getchar();
    return 0;
}