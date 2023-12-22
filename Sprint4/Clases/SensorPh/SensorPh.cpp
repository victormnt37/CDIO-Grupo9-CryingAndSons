#include "SensorPh.h"

SensorPh::SensorPh(int canal, float offsetPh,Adafruit_ADS1115& ads): canalADS(canal), offset(offsetPh), ads1(ads) {
}

float SensorPh::lecturaPh() {
  float adc = ads1.readADC_SingleEnded(canalADS);
  float voltios = adc*4.096 / 32767.0;
  float phValue = (3.5 * voltios) + offset;
  return phValue;
}

