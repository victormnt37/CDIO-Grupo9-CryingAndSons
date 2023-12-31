#include "SensorTemperatura.h"

SensorTemperatura::SensorTemperatura(int canal, Adafruit_ADS1115& ads, float calibT) : canalADS(canal),ads1(ads),calibT(calibT){
}

float SensorTemperatura::lecturaTemperatura() {
  float rawValue =ads1.readADC_SingleEnded(canalADS);
  float Volt=(rawValue/32767)*4.096;
  //valores de temperaura
  float refResistance = 2160.0;  // Resistencia de referencia de la NTC
  float m=0.034;
  float B=0.784;
  float T= (Volt-B)/m;
  float Tf= T+calibT; 
  return Tf;
}
void SensorTemperatura::setCalibration(float calibration) {
  calibT = calibration;
}