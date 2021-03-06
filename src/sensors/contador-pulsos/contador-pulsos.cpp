#include <Arduino.h>

const int PIN_CONTADOR = 33;

int leida = 0;
double ultimoCaudal = 0.00;
long t1 = millis();
long t2 = t1;
long intervalo = 0;
long intervaloRestar = intervalo;

const int EVENTO_CAUDAL = 4;

void setupContador()
{
    pinMode(PIN_CONTADOR, INPUT);
}

void loopContador(long &lectura, float &caudal, int &event, int &eventCaudal)
{
    t1 = millis();
    int lecturaPIN35 = digitalRead(PIN_CONTADOR);

    if (lecturaPIN35 == HIGH)
    {
        if (!leida)
        {
            lectura++;
            leida = 1;
            event = 1;
            // Caudal
            intervalo = (t1 - t2) / 1000;
            if (intervalo != 0)
            {
                caudal = round(1000 / intervalo);
            }
            if (caudal < 0)
                caudal = 0;
            t2 = t1;
        }
    }
    else
    {
        leida = 0;
    }

    intervaloRestar = (t1 - t2) / 1000;
    if (intervaloRestar != 0 && intervaloRestar > intervalo)
    {
        caudal = round(1000 / intervaloRestar);
    }

    if (caudal < 5)
    {
        caudal = 0;
    }

    if (ultimoCaudal > 0 && caudal == 0)
    {
        eventCaudal = 1;
    }

    ultimoCaudal = caudal;
}