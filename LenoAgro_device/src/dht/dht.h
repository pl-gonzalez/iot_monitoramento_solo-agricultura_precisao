#ifndef DHT_H
#define DHT_H
#include "DHTesp.h"

#define I2C_ADDR 0x27
#define DHT_PIN 15

extern DHTesp dhtSensor;

TempAndHumidity temp_umidade();

#endif