#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1015 ads;  // Instancia de Adafruit_ADS1015 para Humedad
Adafruit_ADS1115 ads2; // Instancia de Adafruit_ADS1115 para Temperatura

// Inicialización variables Temperatura

float refResistance = 2160.0;  // Resistencia de referencia de la NTC
float m = 0.034;
float B = 0.784;

// Inicialización variables Humedad

int channelValue = 0;
int sensorValue = 0;
int HValue1 = 0;
int HValue2 = 0;
int HValue3 = 0;
int HValue4 = 0;
int HValue5 = 0;
int MinSensor = 1300; // valor en seco
int MaxSensor = 650; // valor en mojado

// Inicialización variables salinidad

#define power_pin 5 // Pin para alimentar el sensor de salinidad

void setup() {
  Serial.begin(115200);
  Serial.println("Programa de lectura de NTC con ESP8266 y ADS1115");

  // Pedir al usuario que ajuste los valores de mapeo para la humedad
  Serial.println("¿Deseas ajustar los valores de mapeo para la humedad? (Si/No)");
  while (!Serial.available()) {}
  String input = Serial.readStringUntil('\n');
  if (input == "Si" || input == "si") {
    Serial.println("Ponga el sensor en SECO y pulse enter para registrar el nuevo valor...");
    while (Serial.read() != '\n') {}
    sensorValue = ads.readADC_SingleEnded(0);
    MinSensor = sensorValue + 10;
    Serial.println("Ponga el sensor en MOJADO y pulse enter para registrar el nuevo valor...");
    while (Serial.read() != '\n') {}
    sensorValue = ads.readADC_SingleEnded(0);
    MaxSensor = sensorValue - 10;
  }

  // Inicializar el ADC para la humedad
  if (!ads.begin()) {
    Serial.println("No se pudo inicializar el ADC del primer programa.");
    while (1);
  }

  // Inicializar el ADC para la temperatura
  ads2.setGain(GAIN_ONE);
  ads2.begin();

  // Inicialixar el pin mode para salinidad
  pinMode(power_pin, OUTPUT);
}

void loop() {
  // Llamar a las funciones de los dos programas
  medirHumedad();
  tomarTemperatura();
  medirSalinidad();
  Serial.println("______________________________________________");
}

void medirHumedad() {
  sensorValue = ads.readADC_SingleEnded(0);
  // Convertir el valor leído del sensor a un valor de humedad en porcentaje (ajustar según las especificaciones del sensor).map(sensorValue,950, 440, 0, 100);
  HValue1 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue2 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue3 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue4 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue5 = map(sensorValue,MinSensor, MaxSensor, 0, 100);
  Serial.println(sensorValue);
  int Media=(HValue1+HValue2+HValue3+HValue4+HValue5)/5;

  Serial.print("Medición de humedad: ");
  Serial.print(Media);
  Serial.println("%");
  delay(1000);
}

void tomarTemperatura() {
  float rawValue = ads2.readADC_SingleEnded(1);
  float Volt = (rawValue / 32767) * 4.096;
  float T = ((Volt - B) / m)-0.77;

  Serial.print("Lectura de temperatura: ");
  Serial.print(rawValue);
  Serial.print(" Volt: ");
  Serial.print(Volt);
  Serial.print(" Temp: ");
  Serial.println(T);

  delay(1000);
}

void medirSalinidad() {
  int16_t adc0;
  float salinidad;
    
  // Alimentamos la sonda con un tren de pulsos
  digitalWrite(power_pin, HIGH);
  delay(100);

  // leemos cuando hay un nivel alto
  adc0 = analogRead(A0);
  digitalWrite(power_pin, LOW);
  delay(100);

  // Realizamos el cálculo de la salinidad
  salinidad = map(adc0, 0, 1023, 0, 500); // Ajustar rango de mapeo

  // presentamos lectura
  Serial.print("Lectura digital de salinidad: ");
  Serial.println(adc0, DEC);

  //Calibrado del sensor
  float salinidadReal = 288 + 33.067*salinidad - 2.92*pow(salinidad, 2) + 0.0853*pow(salinidad, 3);

  Serial.print("Salinidad = ");
  Serial.println(salinidadReal, 2); // Ajusta el número de decimales según tus necesidades
  delay(1000); // Puedes ajustar el tiempo de espera entre lecturas
}