#include "SensorHumedad.h"

SensorHumedad::SensorHumedad(int canal, Adafruit_ADS1115& ads): canalADS(canal), ads1(ads) {
}

int SensorHumedad::lecturaHumedad() {
  int sensorValue = ads1.readADC_SingleEnded(canalADS);
  //valores de humedad
  int valorSeco = 30166;
  int valorMojado = 17300;
  int humedad = map(sensorValue, valorSeco, valorMojado,0,100);
  return humedad;
}

