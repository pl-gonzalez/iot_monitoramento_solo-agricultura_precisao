#include <Arduino.h>
#include "ph.h"

/**
 * Mede entrada analogica PH_SOLO e converte para a escala de pH
 * 
 */

float medir_ph_solo(){
  int sensorPh = analogRead(PH_SOLO);
  float ph_solo = map(sensorPh, 0, 4095, 0.0, 14.0);

  return ph_solo;
}

void init_ph(){
  pinMode(PH_SOLO, INPUT);
}