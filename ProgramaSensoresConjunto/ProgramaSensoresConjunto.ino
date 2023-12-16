#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1015 ads;  // Instancia de Adafruit_ADS1015

// Inicialización variables Temperatura

float refResistance = 2160.0;  // Resistencia de referencia de la NTC
float m = 0.034;
float B = 0.784;

// Inicialización variables Humedad
int sensorValue = 0;
int HValue1 = 0;
int HValue2 = 0;
int HValue3 = 0;
int HValue4 = 0;
int HValue5 = 0;
int MinSensor = 1300;  // valor en seco
int MaxSensor = 650;   // valor en mojado

// Inicialización variables PH
#define Offset 0.00
#define samplingInterval 20
#define ArrayLength 40 // numero de muestras
int pHArray[ArrayLength]; // almacena las muestras
int pHArrayIndex = 0;

// Iniciacion de variables de pines
// Num en Pin = Tipo de sensor
//
String listaDeSensores[] = { "Nada Conectado", "Termometro", "Sensor de Humedad", "Sensor de Luz", "Sensor de PH" };
String Pin0, Pin1, Pin2, Pin3;
String listaPines[] = { Pin0, Pin1, Pin2, Pin3 };  //se autoactualiza?

#define power_pin 5  // Pin para alimentar el sensor de salinidad

// ****************************** Definición de funciones ******************************

void leerUltimaSonda() {
  Pin0 = listaDeSensores[2];
  Pin1 = listaDeSensores[1];
  Pin2 = listaDeSensores[4];
  Pin3 = listaDeSensores[3];
}
void infoSonda() {
  Serial.println("Sensores de la Sonda conectados a ADS:");
  Serial.println("Pin 0 ADS" + Pin0);
  Serial.println("Pin 1 ADS" + Pin1);
  Serial.println("Pin 2 ADS" + Pin2);
  Serial.println("Pin 3 ADS" + Pin3);
}

int medirHumedad(int Pin) {
  sensorValue = ads.readADC_SingleEnded(Pin);
  // Convertir el valor leído del sensor a un valor de humedad en porcentaje (ajustar según las especificaciones del sensor).map(sensorValue,950, 440, 0, 100);
  HValue1 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue2 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue3 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue4 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  HValue5 = map(sensorValue, MinSensor, MaxSensor, 0, 100);
  int Media = (HValue1 + HValue2 + HValue3 + HValue4 + HValue5) / 5;
  return Media;
}

float tomarTemperatura(int Pin) {
  float rawValue = ads.readADC_SingleEnded(Pin);
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
float averageSample(int numSamples, int* samples) {
  float sum = 0.0;
  for (int i = 0; i < numSamples; i++) {
    sum += samples[i];
  }
  return sum / numSamples;
}

float medirPH(int pin) {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue;
  static float voltage;

  if (millis() - samplingTime > samplingInterval) {
    // realizar varias lecturas del ADS11115
    for (int i = 0; i < ArrayLength; i++) {
      pHArray[i] = ads.readADC_SingleEnded(pin);
      delay(2); // espera pequeña entre lecturas para estabilizar
    }
    // calcular la media de las muestras
    voltage = averageSample(ArrayLength, pHArray) * 5.0 / 32767.0;
    pHValue = 3.5 * voltage + Offset;
    samplingTime = millis();
  }
  return pHValue;
}
int buscarPosicion(String sensor) {
  bool encontrado = false;
  // Variable para almacenar la posición del string (si se encuentra)
  int posicion = -1;
  // Buscar el string en la lista
  for (int i = 0; i < sizeof(listaDeSensores) / sizeof(listaDeSensores[0]); ++i) {
    if (listaDeSensores[i] == sensor) {
      encontrado = true;
      posicion = i;
      break;  // Salir del bucle cuando se encuentra el string
    }
  }
  //if si no encuentra (encontrado = false)
  return posicion;
}
void calibrar(String sensor, int pin) {

  int listnum = buscarPosicion(sensor);  //termometro
  if (listnum == 0) {
    // Nada Conectado
    Serial.println("Nada conectado");
  } else if (listnum == 1) {
    // Calibrar Termometro
  } else if (listnum == 2) {
    // Calibrar Sensor de Humedad
  } else if (listnum == 3) {
    // Calibrar Sensor de Luz
  } else if (listnum == 4) {
    // Calibrar Sensor de PH
  } else {
    Serial.println("Sensor no reconocido/no listado");
  }
}
void exeMenuCalibracion() {
  int seleccion;
  int valorSensor;

  while (true) {
    // Mostrar las opciones del menú
    Serial.println("Seleccione una opción:");
    Serial.println("1. Calibrar sensor pin 1");
    Serial.println("2. Calibrar sensor pin 2");
    Serial.println("3. Calibrar sensor pin 3");
    Serial.println("4. Calibrar sensor pin 4");
    Serial.println("5. Salir del menú");

    void calibrar(int pin) {
    Serial.println("Ingrese el valor de temperatura ambiente:");
    while (!Serial.available()) {
        // Espera a que el usuario ingrese un valor
    }
    int temperaturaAmbiente = Serial.parseInt();
    Serial.print("Calibrando sensor en el pin ");
    Serial.print(pin);
    Serial.print(" con valor de temperatura ambiente ");
    Serial.println(temperaturaAmbiente);
    // Aquí puedes realizar la calibración con el valor de temperatura ambiente proporcionado
}
    // Leer la selección del usuario
    while (!Serial.available()) {
      // Esperar a que el usuario ingrese datos
    }
    seleccion = Serial.parseInt();

    int numpin = -1;
    // Realizar la acción según la selección
    switch (seleccion) {
      case 1:
        Serial.println("Pin 0:");
        Serial.println(Pin0);
        numpin = 0;
        calibrar(Pin0, numpin);
        break;

      case 2:
        Serial.println("Pin 1");
        Serial.println(Pin1);
        numpin = 1;
        calibrar(Pin1, numpin);
        break;
      case 3:
        Serial.println("Pin 2");
        Serial.println(Pin2);
        numpin = 2;
        calibrar(Pin2, numpin);
        break;
      case 4:
        Serial.println("Pin 3");
        Serial.println(Pin3);
        numpin = 3;
        calibrar(Pin3, numpin);
        break;
      case 5:
        Serial.println("Salir");
        return;  // Salir del bucle y, por lo tanto, del menú
      default:
        Serial.println("Opción no válida. Inténtelo de nuevo.");
        break;
    }
  }
}
// ****************************** SETUP ******************************

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
  
  // Preguntar al usuario si desea ejecutar el menú
  Serial.println("¿Desea ejecutar el menú? (y/n)");

  while (!Serial.available()) {
    // Esperar a que el usuario ingrese datos
  }

  char respuesta = Serial.read();

  if (respuesta == 'y' || respuesta == 'Y') {
    // Ejecutar el menú si la respuesta es 'y' o 'Y'
    exeMenuCalibracion();
  } else {
    Serial.println("Iniciando.");
  }

}

// ****************************** Loop ******************************

void loop() {
  //medir Temperatura y imprimir
  Serial.print("Temperatura: ");
  Serial.println(tomarTemperatura(1));//1=num pin de termometro, debuelve un float
  //medir Salinidad y imprimir
  float salinidad = medirSalinidad();
  Serial.print("Salinidad: ");
  Serial.println(salinidad);
  //medir humedad y imprimir
  Serial.print("Humedad: ");
  Serial.println(medirHumedad(0));//0=num pin de termometro, debuelve un int
  //medir ph y imprimir
  Serial.print("PH: ");
  Serial.println(medirPH(2));//2=num pin de termometro, debuelve un float
  //medir luz y imprimir
  Serial.print("LUM: ");
  //Serial.println(medirLuz(3));//3=num pin de termometro, debuelve un float
  Serial.println("______________________________________________");  delay(1000);
  Serial.println("\033[2J\033[H");//borrar monitor serie
}
