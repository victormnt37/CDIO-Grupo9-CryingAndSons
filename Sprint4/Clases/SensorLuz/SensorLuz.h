#ifndef SensorLuz_h
#define SensorLuz_h
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

class SensorLuz {
  private:
    int canalADS;
    Adafruit_ADS1115& ads1;
    
  public:
    SensorLuz(int canal,Adafruit_ADS1115& ads);
    float lecturaLuz();
};

#endif