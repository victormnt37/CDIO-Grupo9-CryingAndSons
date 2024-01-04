#include "SensorPh.h"

SensorPh::SensorPh(int canal, float offsetPh,Adafruit_ADS1115& ads): canalADS(canal), offset(offsetPh), ads1(ads) {
}

float SensorPh::lecturaPh() {
  float adc = ads1.readADC_SingleEnded(canalADS);
  float voltios = adc*4.096 / 32767.0;
  float phValue = (3.5 * voltios) + offset;
  return phValue;
}
void SensorPh::calibrarOffsetAutomatico() {
  // Calibrar automáticamente midiendo pH en una solución de pH 7 con offset 0
  float ph7Puro = 7.0;
  float lecturaActual = lecturaPh();
  float nuevoOffset = ph7Puro - lecturaActual;
  offset = nuevoOffset;
}

