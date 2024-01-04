#include "SensorHumedad.h"


  int SensorHumedad::valorSeco = 30166;
  int SensorHumedad::valorMojado = 17300;

SensorHumedad::SensorHumedad(int canal, Adafruit_ADS1115& ads): canalADS(canal), ads1(ads) {
}

int SensorHumedad::lecturaHumedad() {
  int sensorValue = ads1.readADC_SingleEnded(canalADS);
  int humedad = map(sensorValue, valorSeco, valorMojado,0,100);
  return humedad;
}
void SensorHumedad::calibrarSeco() {
  valorSeco = ads1.readADC_SingleEnded(canalADS);
}

void SensorHumedad::calibrarMojado() {
  valorMojado = ads1.readADC_SingleEnded(canalADS);
}

