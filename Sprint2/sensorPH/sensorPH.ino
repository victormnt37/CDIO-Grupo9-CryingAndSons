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
Adafruit_ADS1115 ads; // TO-DO: Instanciar el objeto de la clase Adafruit_ADS1115

void setup() {
  // Inicializamos el monitor serie
  Serial.begin(9600);
  Serial.println("Inicializando el medidor de PH");

  // Inicializamos el ADS1115
  ads.begin(); // TO-DO: Inicializar el objeto ADS1115

  // Configuramos la ganancia del ADS1115
  ads.setGain(GAIN_ONE); // TO-DO: Configurar la ganancia del ADS1115
}

void loop() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float phValue;
  static float voltage;

  if (millis() - samplingTime > samplingInterval) {
    // realizar una lectura del ADS11115
    pHArray[pHArrayIndex++] = ads.readADC_SingleEnded(channelValue); // TO-DO: Realizar la lectura del canal del ADS1115
    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    // convertir la tectura en tensión
    voltage = (float)pHArray[pHArrayIndex] * 5.0 / 32767.0; // TO-DO: Convertir la lectura a tensión
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
