
#include <Arduino.h>

void setupSensors();
void loopSensors();

uint32_t getLectura();
void setLectura(uint32_t lecturaP);
double getCaudal();

int getEvent();
void setEvent(int eventP);
int getSaving();
