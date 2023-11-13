#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;  // Crear una instancia de la clase Adafruit_ADS1115


const int NTC_PIN = A0;  // Pin analógico al que está conectada la NTC
float refResistance = 2160.0;  // Resistencia de referencia de la NTC
float m=0.034;
float B=0.784;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Programa de lectura de NTC con ESP8266 y ADS1115");
  ads.setGain(GAIN_ONE);  
  ads.begin();  // Inicializar el objeto ads

}

void loop(void) {
   // Leer el valor analógico desde el pin A0
  float rawValue =ads.readADC_SingleEnded(0);
  // Calcular el voltaje
  float Volt=(rawValue/32767)*4.096;
  //Calcular temperatura
  float T= (Volt-B)/m;
  //float Tf= (T-23)+T;
  // Mostrar el valor del voltaje por pantalla
  Serial.print("Lectura: ");
  Serial.print(rawValue);
  Serial.print(" Volt: ");
  Serial.print(Volt);
  Serial.print(" Temp: ");
  Serial.println(T);
  
  delay(1000);
}
