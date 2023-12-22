#ifndef SensorPh_h
#define SensorPh_h
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

class SensorPh {
  private:
    int canalADS;
    float offset;
    Adafruit_ADS1115& ads1;

  public:
    SensorPh(int canal, float offsetPh,Adafruit_ADS1115& ads);
    float lecturaPh();
};

#endif