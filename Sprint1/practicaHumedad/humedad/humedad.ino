#include <Adafruit_ADS1X15.h>

Adafruit_ADS1015 ads; 

int channelValue = 0;
int sensorValue = 0;
int HValue1 = 0;
int HValue2 = 0;
int HValue3 = 0;
int HValue4 = 0;
int HValue5 = 0;

int MinSensor = 950;//valor en seco
int MaxSensor = 440;//valor en mojado

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // Pedir al usuario que ajuste los valores de mapeo.
  Serial.println("¿Deseas ajustar los valores de mapeo? (Si/No)");
  while (!Serial.available()) {}  // Esperar hasta que el usuario escriba algo en el monitor serie.

  String input = Serial.readStringUntil('\n');
  if (input == "Si" || input == "si") {
    Serial.println("Ponga el sensor en SECO y pulse enter para registrar el nuevo valor...");
    while (Serial.read() != '\n') {}
    sensorValue = ads.readADC_SingleEnded(0); 
    MinSensor = sensorValue+10;
    Serial.println("Ponga el sensor en MOJADO y pulse enter para registrar el nuevo valor...");
    while (Serial.read() != '\n') {}
    sensorValue = ads.readADC_SingleEnded(0); 
    MaxSensor = sensorValue-10;
  }

  // Inicializar el ADC.
  if (!ads.begin()) {
    Serial.println("No se pudo inicializar el ADC.");
    while (1);
  }
}

void loop() {
  // Leer el valor del sensor
  sensorValue = ads.readADC_SingleEnded(0);
  // Convertir el valor leído del sensor a un valor de humedad en porcentaje (ajustar según las especificaciones del sensor).map(sensorValue,950, 440, 0, 100);
  HValue1 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  Serial.print(HValue1);
  delay(250);
  HValue2 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  Serial.print(HValue2);
  delay(250);
  HValue3 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  Serial.print(HValue3);
  delay(250);
  HValue4 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  Serial.print(HValue4);
  delay(250);
  HValue5 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  Serial.println(HValue5);
  int Media=(HValue1+HValue2+HValue3+HValue4+HValue5)/5;

  // Imprimir los valores en la comunicación serie.
  //Serial.print("Valor del sensor: ");
  //Serial.println(sensorValue);
  Serial.print("Humedad media: ");
  Serial.print(Media);
  Serial.println("%");

  // Esperar 1 segundo antes de realizar otra lectura.
  delay(1000);
}