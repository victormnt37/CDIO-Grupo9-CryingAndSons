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
String listaPines[] = { Pin0, Pin1, Pin2, Pin3, Pin4 };  //se autoactualiza?
float ListTemp[10] = { 8000 };
// int ListHum[10]={8000};
// float ListSal[10]={8000}; NO SALINIDAD por solo haber un unico lector Analogico
float ListPH[10] = { 8000 };

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
      Serial.println("¿Que pin de ADS quieres cambiar? (0-4)");
      String ChangePin = Serial.readStringUntil('\n');

      if (ChangePin >= 0 && ChangePin <= 4) {  //comprobar si pin es valido
        Serial.println("¿Que sensor hay?");
        for (int i = 0; i < sizeof(listaDeSensores) / sizeof(listaDeSensores[0]); ++i) {
          Serial.println(listaDeSensores[i]);
        }
        String NewSensor = Serial.readStringUntil('\n');
        bool sensorValido = false;

        for (int i = 0; i < sizeof(listaDeSensores) / sizeof(listaDeSensores[0]); ++i) {  //comprovar si el sensor existe
          if (NewSensor.equalsIgnoreCase(listaDeSensores[i])) {
            sensorValido = true;
            break;
          }
        }

        if (sensorValido) {
          //aplicar cambio, pin ChangePin, Sensor NewSensor
          if (PinChange == 0) {
            Pin0 = NewSensor;
          } else if (PinChange == 1) {
            Pin1 = NewSensor;
          } else if (PinChange == 2) {
            Pin2 = NewSensor;
          } else if (PinChange == 3) {
            Pin3 = NewSensor;
          } else if (PinChange == 4) {
            Pin4 = NewSensor;
          }
          infoSonda();
        } else {
          Serial.println("Opcion de Sensor no valida");
        }

      } else {
        Serial.println("Opcion de Pin no valida");
      }

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
      Serial.println("¿Qué sensor deseas calibrar? (Temperatura/Humedad/Salinidad)");
      String sensorInput = Serial.readStringUntil('\n');

      if (sensorInput.equalsIgnoreCase("Temperatura")) {
        // Calibración de temperatura
        Serial.println("Calibración de temperatura...");
        // Aquí puedes agregar la lógica específica de calibración para la temperatura
      } else if (sensorInput.equalsIgnoreCase("Humedad")) {
        // Calibración de humedad
        Serial.println("Calibración de humedad...");
        // Aquí puedes agregar la lógica específica de calibración para la humedad

        //CALIBRACION DE HUMEDAD SIMPLE
        // if (input == "Si" || input == "si") {
        //   Serial.println("Ponga el sensor en SECO y pulse enter para registrar el nuevo valor...");
        //   while (Serial.read() != '\n') {}
        //   sensorValue = ads.readADC_SingleEnded(0);
        //   MinSensor = sensorValue + 10;
        //   Serial.println("Ponga el sensor en MOJADO y pulse enter para registrar el nuevo valor...");
        //   while (Serial.read() != '\n') {}
        //   sensorValue = ads.readADC_SingleEnded(0);
        //   MaxSensor = sensorValue - 10;
      } else if (sensorInput.equalsIgnoreCase("Salinidad")) {
        // Calibración de salinidad
        Serial.println("Calibración de salinidad...");
        // Aquí puedes agregar la lógica específica de calibración para la salinidad
      } else {
        Serial.println("Sensor no válido para calibración.");
      }
    } else if (input == "N" || input == "n") {
      calibrar = false;
    } else {
      Serial.println("Opcion no valida");
    }
  }
}
void leerUltimaSonda() {
  Pin0 = listaDeSensores[0];
  Pin1 = listaDeSensores[0];
  Pin2 = listaDeSensores[0];
  Pin3 = listaDeSensores[0];
  Pin4 = listaDeSensores[0];
}
void infoSonda() {
  Serial.println("Sensores de la Sonda conectados a ADS:");
  Serial.println("Pin 0 ADS" + Pin0);
  Serial.println("Pin 1 ADS" + Pin1);
  Serial.println("Pin 2 ADS" + Pin2);
  Serial.println("Pin 3 ADS" + Pin3);
  Serial.println("Pin 4 ADS" + Pin4);
}

void loop() {
  for (int i = 0; i <= 4; ++i) {
    // Construir el nombre del pin
    String pinActual = listaPines[i];  //sonda del pin
    // Realizar acciones según el valor de la variable Pin
    if (pinActual == listaDeSensores[1]) {         //temperatura
      ListTemp[i] = tomarTemperatura(i);           //falta eliminar valores 8000
    } else if (pinActual == listaDeSensores[2]) {  //Humedad
      ListHum[i] = medirHumedad(i);                //falta eliminar valores 8000
    } else if (pinActual == listaDeSensores[3]) {  //Salinidad
      //no esta en el ads, pin analogico
      //ListSal[i]=medirSalinidad();//falta eliminar valores 8000
      float salinidad = medirSalinidad();
    } else if (pinActual == listaDeSensores[4]) {  //PH

    } else {
      //pin sin nada o sonda no contemplada
    }
  }
  //Imprimir Temperatura
  for (int i = 0; i < sizeof(ListTemp) / sizeof(ListTemp[0]); ++i) {
    // Verificar si el valor es diferente de 8000 antes de imprimir
    if (ListTemp[i] != 8000) {
      Serial.print("Temperatura: ");
      Serial.print(ListTemp[i]);
      Serial.println("Cº");
      //mandar add al string a enviar
    }
  }
  for (int i = 0; i < sizeof(ListHum) / sizeof(ListHum[0]); ++i) {
    // Verificar si el valor es diferente de 8000 antes de imprimir
    if (ListHum[i] != 8000) {
      Serial.print("Humedad: ");
      Serial.print(ListHum[i]);
      Serial.println("%");
      //mandar add al string a enviar
    }
  }
  Serial.println("Salinidad: " + salinidad);
  Serial.println("______________________________________________");

  delay(1000);
}

int medirHumedad(int Pin) {
  sensorValue = ads.readADC_SingleEnded(Pin);
  // Convertir el valor leído del sensor a un valor de humedad en porcentaje (ajustar según las especificaciones del sensor).map(sensorValue,950, 440, 0, 100);
  HValue1 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue2 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue3 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue4 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue5 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  Serial.println(sensorValue);
  int Media = (HValue1 + HValue2 + HValue3 + HValue4 + HValue5) / 5;
  return Media;
}

float tomarTemperatura(int Pin) {
  float rawValue = ads2.readADC_SingleEnded(Pin);
  float Volt = (rawValue / 32767) * 4.096;
  float T = ((Volt - B) / m) - 0.77;
  return T;
}

float medirSalinidad() {
  int16_t adc0;
  float salinidad;
  // Alimentamos la sonda con un tren de pulsos
  digitalWrite(power_pin, HIGH);
  // leemos cuando hay un nivel alto
  adc0 = analogRead(A0);
  digitalWrite(power_pin, LOW);
  // Realizamos el cálculo de la salinidad
  salinidad = map(adc0, 0, 1023, 0, 500);  // Ajustar rango de mapeo
  //Calibrado del sensor
  float salinidadReal = 288 + 33.067 * salinidad - 2.92 * pow(salinidad, 2) + 0.0853 * pow(salinidad, 3);
  return salinidadReal;
}