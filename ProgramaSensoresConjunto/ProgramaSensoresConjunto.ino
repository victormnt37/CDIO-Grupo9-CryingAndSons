#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1015 ads;  // Instancia de Adafruit_ADS1015

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
int MinSensor = 1300;  // valor en seco
int MaxSensor = 650;   // valor en mojado

// Inicialización variables salinidad

// Iniciacion de variables de pines
// Num en Pin = Tipo de sensor
//
String listaDeSensores[] = { "Nada Conectado", "Termometro", "Sensor de Humedad", "Sensor de Salinidad", "Sensor de PH" };
String Pin0, Pin1, Pin2, Pin3, Pin4;


#define power_pin 5  // Pin para alimentar el sensor de salinidad

void setup() {
  Serial.begin(115200);
  Serial.println("Programa de lectura de NTC con ESP8266 y ADS1115");

  if (!ads.begin()) {  // Inicializar el ADC para la humedad
    Serial.println("No se pudo inicializar el ADC del primer programa.");
    while (1)
      ;
  }
  ads.setGain(GAIN_ONE);  // Inicializar el ADC para la temperatura
  ads.begin();
  pinMode(power_pin, OUTPUT);  // Inicialixar el pin mode para salinidad

  //Menu de inicializado
  delay(1000);
  leerUltimaSonda();
  infoSonda();
  bool configurar = true;
  while (configurar) {
    Serial.println("¿Deseas cambiar la configuracion de la sonda? (S/N)");
    String input = Serial.readStringUntil('\n');
    if (input == "S" || input == "s") {
      //Declarar que pin es cada cosa
    } else if (input == "N" || input == "n") {
      configurar = false;
    } else {
      Serial.println("Opcion no valida");
    }
  }
  bool calibrar = true;
  while (calibrar) {
    Serial.println("¿Deseas calibrar algun sensor de la sonda? (S/N)");
    String input = Serial.readStringUntil('\n');
    if (input == "S" || input == "s") {
      //Calibracion de sensores
    } else if (input == "N" || input == "n") {
      calibrar = false;
    } else {
      Serial.println("Opcion no valida");
    }
  }


  // Pedir al usuario que ajuste los valores de mapeo para la humedad
  // Serial.println("¿Deseas ajustar los valores de mapeo para la humedad? (Si/No)");
  // while (!Serial.available()) {}
  // String input = Serial.readStringUntil('\n');
  // if (input == "Si" || input == "si") {
  //   Serial.println("Ponga el sensor en SECO y pulse enter para registrar el nuevo valor...");
  //   while (Serial.read() != '\n') {}
  //   sensorValue = ads.readADC_SingleEnded(0);
  //   MinSensor = sensorValue + 10;
  //   Serial.println("Ponga el sensor en MOJADO y pulse enter para registrar el nuevo valor...");
  //   while (Serial.read() != '\n') {}
  //   sensorValue = ads.readADC_SingleEnded(0);
  //   MaxSensor = sensorValue - 10;
  // }
}
void leerUltimaSonda() {
  Pin0 = listaDeSensores[0];
  Pin1 = listaDeSensores[0];
  Pin2 = listaDeSensores[0];
  Pin3 = listaDeSensores[0];
  Pin4 = listaDeSensores[0];
}
void infoSonda() {
  Serial.println("Sensores de la Sonda:");
  Serial.println("Pin 0 ADS" + Pin0);
  Serial.println("Pin 1 ADS" + Pin1);
  Serial.println("Pin 2 ADS" + Pin2);
  Serial.println("Pin 3 ADS" + Pin3);
  Serial.println("Pin 4 ADS" + Pin4);
}

void loop() {
  // Llamar a las funciones de los dos programas
  int humedad = medirHumedad(0);
  float temperatura = tomarTemperatura(1);
  float salinidad = medirSalinidad();

  Serial.println("Humedad: " + humedad + "%");
  Serial.println("Temperatura: " + temperatura + "Cº");
  Serial.println("Salinidad: " + salinidad);
  Serial.println("______________________________________________");

  delay(1000);
}

int medirHumedad(int Pin) {
  sensorValue = ads.readADC_SingleEnded(Pin);
  // Convertir el valor leído del sensor a un valor de humedad en porcentaje (ajustar según las especificaciones del sensor).map(sensorValue,950, 440, 0, 100);
  HValue1 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue2 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue3 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue4 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  delay(250);
  HValue5 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  Serial.println(sensorValue);
  int Media = (HValue1 + HValue2 + HValue3 + HValue4 + HValue5) / 5;
  return Media;
  //   Serial.print("Medición de humedad: ");
  // Serial.print(Media);
  // Serial.println("%");
  // delay(1000);
}

float tomarTemperatura(int Pin) {
  float rawValue = ads2.readADC_SingleEnded(Pin);
  float Volt = (rawValue / 32767) * 4.096;
  float T = ((Volt - B) / m) - 0.77;
  return T;
  // Serial.print("Lectura de temperatura: ");
  // Serial.print(rawValue);
  // Serial.print(" Volt: ");
  // Serial.print(Volt);
  // Serial.print(" Temp: ");
  // Serial.println(T);
}

float medirSalinidad() {
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
  salinidad = map(adc0, 0, 1023, 0, 500);  // Ajustar rango de mapeo

  // // presentamos lectura
  // Serial.print("Lectura digital de salinidad: ");
  // Serial.println(adc0, DEC);

  //Calibrado del sensor
  float salinidadReal = 288 + 33.067 * salinidad - 2.92 * pow(salinidad, 2) + 0.0853 * pow(salinidad, 3);
  return salinidadReal;

  // Serial.print("Salinidad = ");
  // Serial.println(salinidadReal, 2); // Ajusta el número de decimales según tus necesidades
  // delay(1000); // Puedes ajustar el tiempo de espera entre lecturas
}