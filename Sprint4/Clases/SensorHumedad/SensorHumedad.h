#ifndef SensorHumedad_h
#define SensorHumedad_h
#include <Arduino.h> 
#include <Adafruit_ADS1X15.h>

class SensorHumedad {
  private:
    int canalADS;
    Adafruit_ADS1115& ads1;

  public:
    SensorHumedad(int canal,Adafruit_ADS1115& ads);
    int lecturaHumedad();
};

#endif