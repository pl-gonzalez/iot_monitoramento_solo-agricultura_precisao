#include <Arduino.h>
#include "dht.h"


TempAndHumidity temp_umidade(){
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    return data;
}

#include "dht.h"