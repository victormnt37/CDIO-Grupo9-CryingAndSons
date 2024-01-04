#ifndef SensorHumedad_h
#define SensorHumedad_h
#include <Arduino.h> 
#include <Adafruit_ADS1X15.h>

class SensorHumedad {
  private:
    int canalADS;
    Adafruit_ADS1115& ads1;
    static int valorSeco;
    static int valorMojado;

  public:
    SensorHumedad(int canal,Adafruit_ADS1115& ads);
    int lecturaHumedad();
    void calibrarSeco();
    void calibrarMojado();
};

#endif