#include <Adafruit_ADS1015.h>

// Definir el Canal del ADS1115 por el que leeremos la tension del sensor de PH
#define channelValue 0
#define Offset 0.00
#define samplingInterval 20
#define ArrayLength 40 // numero de muestras
int pHArray[ArrayLength]; // almacena las muestras
int pHArrayIndex = 0;

// Construimos el ADS1115
// TO-DO

void setup() {
  // Inicializamos  el monitor serie
  Serial.begin(9600);
  Serial.println("Inicializando el medidor de PH");

  // Inicializamos el ADS1115
  // TO-DO
  // Configuramos la ganancia del ADS1115
  // TO-DO
}

void loop() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float phValue;
  static float voltage;

  if (millis() - samplingTime > samplingInterval) {
    // realizar una lectura del ADS11115
    pHArray[pHArrayIndex++] = //TO-DO;
    if (pHArrayIndex == ArrayLenght) pHArrayIndex = 0;
    // convertir la tectura en tensión
    voltage = //TO-DO;
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
