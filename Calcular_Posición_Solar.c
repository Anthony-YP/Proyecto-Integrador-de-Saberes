// Programa para calcular la posición solar a partir de la longitud y latitud ingresadas por el usuario //
/**
 * @file Calcular_Posición_Solar.cpp
 * @author Anthony_YP (anthonyjoseyp@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-15
 * @copyright Copyright (c) 2024
 */

// Importamos las librerias necesarias para facilitarnos la programacion //
#include <stdio.h>
#include <math.h>
#include <time.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//  Empezamos declarando y haciendo las funciones necesarias para el programa //
int Calcular_Numero_Dias(int meses, int dias, int anios, int N_Dias)
{
    for (int i = 1; i < meses; i++)
    {
        switch (i)
        {
        case 1:
            N_Dias = N_Dias + 31;
        case 2:
            N_Dias = N_Dias + 28;
        case 3:
            N_Dias = N_Dias + 31;
        case 4:
            N_Dias = N_Dias + 30;
        case 5:
            N_Dias = N_Dias + 31;
        case 6:
            N_Dias = N_Dias + 30;
        case 7:
            N_Dias = N_Dias + 31;
        case 8:
            N_Dias = N_Dias + 31;
        case 9:
            N_Dias = N_Dias + 30;
        case 10:
            N_Dias = N_Dias + 31;
        case 11:
            N_Dias = N_Dias + 30;
        case 12:
            N_Dias = N_Dias + 31;
        default:
            break;
        }
    }
    N_Dias = N_Dias + dias;
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

float Calcular_Angulo_Declinacion_Solar(int N_Dias, float Declinacion_Solar)
{
    float Termino1, Termino2, DS_Radianes, Coseno;

    Termino1 = -23.44;
    Termino2 = ((360 / 365) * (N_Dias + 10));
    DS_Radianes = (Termino2 * (M_PI / 180));
    Coseno = cos(DS_Radianes);
    Declinacion_Solar = Termino1 * Coseno;
    return Declinacion_Solar;
}

float Ecuacion_Del_Tiempo(int N_Dias, float EoT)
{
    float B;

    B = ((360 / 365) * (N_Dias - 81));
    B = B * (M_PI / 180);
    EoT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
    return EoT;
}

float Calcular_Tiempo_Solar_Verdadero(int horas, int minutos, float Longitud, float EoT, float TSV)
{
    int Zona_Horaria;
    float Hora_Local, Longitud_Estandar;

    Zona_Horaria = -5;
    Hora_Local = horas + (minutos / 60);
    Longitud_Estandar = Zona_Horaria * 15;
    TSV = Hora_Local + (((4 * (Longitud - Longitud_Estandar)) + EoT) / 60);
    return TSV;
}

float Calcular_Angulo_Horario(float TSV, float Angulo_Horario)
{
    Angulo_Horario = 15 * (TSV - 12);
    Angulo_Horario = Angulo_Horario * (M_PI / 180);
    return Angulo_Horario;
}

float Calcular_Altitud_Solar(float Declinacion_Solar, float Latitud, float Angulo_Horario, float Altitud_Solar)
{
    Altitud_Solar = asin(sin(Declinacion_Solar)) * sin(Latitud) + cos(Declinacion_Solar) * cos(Latitud) * cos(Angulo_Horario);
    return Altitud_Solar;
}

float Calcular_Azimut(float Declinacion_Solar, float Altitud_Solar, float Latitud, float Angulo_Horario, float Azimut)
{
    Azimut = acos((sin(Declinacion_Solar) - sin(Altitud_Solar) * sin(Latitud)) / (cos(Altitud_Solar) * cos(Latitud)));
    if (Angulo_Horario > 0)
    {
        Azimut = 2 * M_PI - Azimut;
    }
    return Azimut;
}

int main()
{
    // Definimos todas las variables necesarias para nuestro programa //
    int horas, minutos, segundos;
    int anios, meses, dias;
    int N_Dias;
    float Longitud, Latitud;
    float Declinacion_Solar, EoT, TSV, Angulo_Horario, Altitud_Solar, Orientacion_Solar, Azimut, Hora_Solar, Minutos_Solar;
    /* Recuperamos la fecha y hora actual del sistema con ayuda de la libreria "time.h" y metemos cada uno de los datos en su respectiva
    variable*/
    time_t tiempoahora;
    time(&tiempoahora);
    struct tm *mitiempo = localtime(&tiempoahora);
    horas = mitiempo->tm_hour;
    minutos = mitiempo->tm_min;
    segundos = mitiempo->tm_sec;
    anios = mitiempo->tm_year;
    meses = mitiempo->tm_mon;
    dias = mitiempo->tm_mday;

    Latitud = -3.99313;
    Longitud = -79.20422;

    // Iniciamos el Programa llamando y asando los valores a cada una de las funciones antes definidas //

    N_Dias = Calcular_Numero_Dias(meses, dias, anios, N_Dias);

    Declinacion_Solar = Calcular_Angulo_Declinacion_Solar(N_Dias, Declinacion_Solar);

    EoT = Ecuacion_Del_Tiempo(N_Dias, EoT);
    // Calculamos el tiempo solar verdadero //
    TSV = Calcular_Tiempo_Solar_Verdadero(horas, minutos, Longitud, EoT, TSV);
    Hora_Solar = trunc(TSV);
    Minutos_Solar = round(60 * (TSV - Hora_Solar));

    // Calculamos el angulo horario //
    Angulo_Horario = Calcular_Angulo_Horario(TSV, Angulo_Horario);

    // Pasamos la Declinacion Solar y la Latitud de grados a radianes //

    Declinacion_Solar = Declinacion_Solar * (M_PI / 180);
    Latitud = Latitud * (M_PI / 180);

    // Calculamos la Altitud Solar en radianes //

    Altitud_Solar = Calcular_Altitud_Solar(Declinacion_Solar, Latitud, Angulo_Horario, Altitud_Solar);

    // Pasamos la Altitud Solar a grados //
    Altitud_Solar = Altitud_Solar * (180 / M_PI);

    // Calculamos la orientacion Solar o Azimut en radianes//
    Azimut = Calcular_Azimut(Declinacion_Solar, Altitud_Solar, Latitud, Angulo_Horario, Azimut);

    // Pasamos la orientacion solar a grados //
    Azimut = Azimut * (180 / M_PI);

    printf("La posición de la altitud del sol es de => %d grados", Altitud_Solar);
    printf("La posición de la orientación del sol es de => %d grados", Azimut);
    return 0;
}
