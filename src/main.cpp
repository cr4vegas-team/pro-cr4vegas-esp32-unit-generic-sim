/**************************************************************
 *  Proyecto: cr4vegas-node-dual-core
 *  Autor: Rubén Francisco Gazquez Rosales
 *  Description: Nodo final para las unidades de control genéricas.
 *    Su uso es exclusivo para GPRS (NO WIFI).
 *  Enlaces de las librerías utilizadas:
 *    - https://github.com/knolleary/pubsubclient
 *    - http://librarymanager/all#PubSubClient
 *    - https://tiny.cc/tinygsm-readme
 *  Placas: ESP32
 * 
 *  CONFIGURACIÓN:
 *  Tópico de subscripción: "server/test" --> "s/u/g/1"
 *  Tópico de publicación:  "client/test" --> "n/u/g/1"
 *  Server MQTT:            "mqtt.rubenfgr.com"
 *  APN Things-Mobile:      "TM"
 * 
 *  MQTT
 *  p = propiedad
 *  o = orden
 *  Arrays de publicación soportados:
 *    - 0 --> pub[0]                      (publicar inicio)
 *    - 1 --> pub[1]                      (publicar comunicación)
 *    - 2 --> pub[2,p1,p2,p3,p4,p5]       (publicar datos de sensores)
 *    - 3 --> pub[3,operador,señal,ip]    (publicar datos de la sim)
 *    - 8 --> pub[8,ss]                  (publicar velocidad de envio de datos)
 *  Arrays de subscripción soportados:
 *    - 1 --> sub[1]                -->   pub(1)  (subcripción de comunicación)
 *    - 2 --> sub[2]                -->   pub(2)  (subcripción de datos de sensores)
 *    - 3 --> sub[3]                -->   pub(8)  (subcripción de datos de la sim)
 *    - 5 --> sub[5,o1,o2,o3,o4]    -->   pub(2)  (subcripción de ordenes a actuadores)
 *    - 8 --> sub[7,ss]             -->   pub(7)  (subcripción de configuración de velocidad de envio de datos)
 *    - 9 --> sub[9,p1,p2,p3,p4,p5] -->   pub(2)  (subcripción de configuración)
 * 
 *  PINES:
 *    ENTRADAS (inputs):
 *      - D18   = SIM900L RX
 *      - D19   = SIM900L TX
 *      - D21   = SIM900 RESET
 * 
 *      - D13  = orden1
 *      - D12  = orden2
 *      - D14  = orden3
 *      - D27  = orden4
 * 
 *    SALIDAS (outputs):
 *      - D34   = sensor1
 *      - D35   = sensor2
 *      - D32   = sensor3
 *      - D33   = sensor4
 *      - D25   = sensor5
 **************************************************************/
#include <Arduino.h>
#include "sim/sim.h"
#include "save/save.h"
#include "sensors/sensors.h"

#define SerialMon Serial

TaskHandle_t taskSensors;
TaskHandle_t taskSIM;

long timeToSave = 0;
long TIME_TO_SAVE = 600000;

void runTaskSensors(void *pvParameters);
void runTaskSIM(void *pvParameters);

void setup()
{
    SerialMon.begin(115200);
    xTaskCreatePinnedToCore(runTaskSensors, "Task Sensors", 5120, NULL, 5, &taskSensors, 0);
    delay(1000);
    xTaskCreatePinnedToCore(runTaskSIM, "Task Sim", 10240, NULL, 6, &taskSIM, 1);
    delay(1000);
}

void runTaskSIM(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    SerialMon.println("runTaskSIM running on core: " + (String)xPortGetCoreID());

    setLectura(readLectura()); // Cargo la lectura desde la flash

    setupSIM(xLastWakeTime);

    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    for (;;)
    {
        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        // Perform action here.
        loopSIM();

        if (getEvent() == 1)
        {
            saveLectura(getLectura());
        }
    }
}

void runTaskSensors(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    SerialMon.println("runTaskSensors running on core: " + (String)xPortGetCoreID());

    setupSensors();

    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    for (;;)
    {
        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        // Perform action here.
        loopSensors();
    }
}

void loop()
{
}