#include <Wire.h>
#include <Adafruit_ADS1015.h>

// Definir el Canal del ADS1115 por el que leeremos la tension del sensor de PH
#define channelValue 0
#define Offset 0.00
#define samplingInterval 20
#define ArrayLength 40 // numero de muestras
int pHArray[ArrayLength]; // almacena las muestras
int pHArrayIndex = 0;

// Construimos el ADS1115
Adafruit_ADS1115 ads;

void setup() {
  // Inicializamos el monitor serie
  Serial.begin(9600);
  Serial.println("Inicializando el medidor de PH");

  // Inicializamos el ADS1115
  ads.begin();

  // Configuramos la ganancia del ADS1115
  ads.setGain(GAIN_ONE);
}

void loop() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float phValue;
  static float voltage;

  if (millis() - samplingTime > samplingInterval) {
    // realizar varias lecturas del ADS11115
    for (int i = 0; i < ArrayLength; i++) {
      pHArray[i] = ads.readADC_SingleEnded(channelValue);
      delay(2); // espera pequeña entre lecturas para estabilizar
    }
    // calcular la media de las muestras
    voltage = averageSample(ArrayLength, pHArray) * 5.0 / 32767.0;
    pHValue = 3.5 * voltage + Offset;
    samplingTime = millis();
  }

  if (millis() - printTime > printInterval) {
    // Cada printTime segundos se escribe un dato en pantalla
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    printTime = millis();
  }
}
