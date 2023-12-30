#ifndef SensorTemperatura_h
#define SensorTemperatura_h
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

class SensorTemperatura {
  private:
    int canalADS;
    Adafruit_ADS1115& ads1;
    float calibT;

  public:
    SensorTemperatura(int canal, Adafruit_ADS1115& ads, float calibT);
    float lecturaTemperatura();
    void setCalibration(float calibration);
};

#endif