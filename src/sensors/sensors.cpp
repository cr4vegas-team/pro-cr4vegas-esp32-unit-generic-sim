#include "./contador-pulsos/contador-pulsos.h"

// ==================================================
//  Sensores
// ==================================================
long lectura = 0;
float caudal = 0;
int event = 0;
int eventCaudal = 0;

void setupSensors()
{
    setupContador();
}

void loopSensors()
{
    loopContador(lectura, caudal, event, eventCaudal);
}

uint32_t getLectura()
{
    return lectura;
}

void setLectura(uint32_t lecturaP)
{
    lectura = lecturaP;
    event = 1;
}

double getCaudal()
{
    return caudal;
}

int getEvent()
{
    return event;
}

int getEventCaudal()
{
    return eventCaudal;
}

void setEvent(int eventP)
{
    event = eventP;
}

void setEventCaudal(int eventcaudalP)
{
    eventCaudal = eventcaudalP;
}