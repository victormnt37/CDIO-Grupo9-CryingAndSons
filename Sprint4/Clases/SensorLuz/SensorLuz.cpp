#include "SensorLuz.h"

SensorLuz::SensorLuz(int canal, Adafruit_ADS1115& ads) : canalADS(canal), ads1(ads){
}

float SensorLuz::lecturaLuz() {
  int16_t adc10 = ads1.readADC_SingleEnded(canalADS);
  float voltage5 = (adc10*4.096)/32767;
  if (adc10<=130) {
    return 0; // oscuro
  } else {
    if (adc10>=1200) {
      return 10; // luz artificial
    } else {
    return 1; // luz natural
    }
  }
}

