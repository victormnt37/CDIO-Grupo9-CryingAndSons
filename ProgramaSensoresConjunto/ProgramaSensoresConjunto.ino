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
#define ArrayLength 40     // numero de muestras
int pHArray[ArrayLength];  // almacena las muestras
int pHArrayIndex = 0;

// Iniciacion de variables de pines
// Num en Pin = Tipo de sensor
//
String listaDeSensores[] = { "Nada Conectado", "Termometro", "Sensor de Humedad", "Sensor de Luz", "Sensor de PH" };
String Pin0, Pin1, Pin2, Pin3;
String listaPines[] = { Pin0, Pin1, Pin2, Pin3 };  //se autoactualiza?

#define power_pin 5  // Pin para alimentar el sensor de salinidad

#include <ESP8266WiFi.h>

// Comentar/Descomentar para ver mensajes de depuracion en monitor serie y/o respuesta del HTTP server
#define PRINT_DEBUG_MESSAGES
#define PRINT_HTTP_RESPONSE

// Comentar/Descomentar para conexion Fuera/Dentro de UPV
//#define WiFi_CONNECTION_UPV

// Selecciona que servidor REST quieres utilizar entre ThingSpeak y Dweet
//#define REST_SERVER_THINGSPEAK //Selecciona tu canal para ver los datos en la web (https://thingspeak.com/channels/360979)
#define REST_SERVER_DWEET  //Selecciona tu canal para ver los datos en la web (http://dweet.io/follow/PruebaGTI)

///////////////////////////////////////////////////////
/////////////// WiFi Definitions /////////////////////
//////////////////////////////////////////////////////

#ifdef WiFi_CONNECTION_UPV  //Conexion UPV
const char WiFiSSID[] = "GTI1";
const char WiFiPSK[] = "1PV.arduino.Toledo";
#else  //Conexion fuera de la UPV
const char WiFiSSID[] = "aaaa";
const char WiFiPSK[] = "12345678";
#endif



///////////////////////////////////////////////////////
/////////////// SERVER Definitions /////////////////////
//////////////////////////////////////////////////////

#if defined(WiFi_CONNECTION_UPV)  //Conexion UPV
const char Server_Host[] = "proxy.upv.es";
const int Server_HttpPort = 8080;
#elif defined(REST_SERVER_THINGSPEAK)  //Conexion fuera de la UPV
const char Server_Host[] = "api.thingspeak.com";
const int Server_HttpPort = 80;
#else
const char Server_Host[] = "dweet.io";
const int Server_HttpPort = 80;
#endif

WiFiClient client;

///////////////////////////////////////////////////////
/////////////// HTTP REST Connection ////////////////
//////////////////////////////////////////////////////

#ifdef REST_SERVER_THINGSPEAK
const char Rest_Host[] = "api.thingspeak.com";
String MyWriteAPIKey = "15CUR2E6XRSC3G24";  // Escribe la clave de tu canal ThingSpeak
#else
const char Rest_Host[] = "dweet.io";
String MyWriteAPIKey = "cdiocurso2018g09";  // Escribe la clave de tu canal Dweet
#endif

#define NUM_FIELDS_TO_SEND 5  //Numero de medidas a enviar al servidor REST (Entre 1 y 8)

/////////////////////////////////////////////////////
/////////////// Pin Definitions ////////////////
//////////////////////////////////////////////////////

const int LED_PIN = 5;  // Thing's onboard, green LED

/////////////////////////////////////////////////////
/////////////// WiFi Connection ////////////////
//////////////////////////////////////////////////////

void connectWiFi() {
  byte ledStatus = LOW;

#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
#endif

  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED) {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus);  // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println(".");
#endif
    delay(500);
  }
#ifdef PRINT_DEBUG_MESSAGES
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());  // Print the IP address
#endif
}

/////////////////////////////////////////////////////
/////////////// HTTP POST  ThingSpeak////////////////
//////////////////////////////////////////////////////
void HTTPPost(String fieldData[], int numFields) {
  if (client.connect(Server_Host, Server_HttpPort)) {

    // Construimos el string de datos. Si tienes multiples campos asegurate de no pasarte de 1440 caracteres

    String PostData = "api_key=" + MyWriteAPIKey;
    for (int field = 1; field < (numFields + 1); field++) {
      PostData += "&field" + String(field) + "=" + fieldData[field];
    }

// POST data via HTTP
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println("Connecting to ThingSpeak for update...");
#endif
    client.println("POST http://" + String(Rest_Host) + "/update HTTP/1.1");
    client.println("Host: " + String(Rest_Host));
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(PostData.length()));
    client.println();
    client.println(PostData);
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println(PostData);
    Serial.println();
//Para ver la respuesta del servidor
#ifdef PRINT_HTTP_RESPONSE
    delay(500);
    Serial.println();
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println();
#endif
#endif
  }
}

////////////////////////////////////////////////////
/////////////// HTTP GET  ////////////////
//////////////////////////////////////////////////////
void HTTPGet(String fieldData[], int numFields) {
  if (client.connect(Server_Host, Server_HttpPort)) {
#ifdef REST_SERVER_THINGSPEAK
    String PostData = "GET https://api.thingspeak.com/update?api_key=";
    PostData = PostData + MyWriteAPIKey;
#else
    String PostData = "GET http://dweet.io/dweet/for/";
    PostData = PostData + MyWriteAPIKey + "?";
#endif

    for (int field = 1; field < (numFields + 1); field++) {
      PostData += "&field" + String(field) + "=" + fieldData[field];
    }


#ifdef PRINT_DEBUG_MESSAGES
    Serial.println("Connecting to Server for update...");
#endif
    client.print(PostData);
    client.println(" HTTP/1.1");
    client.println("Host: " + String(Rest_Host));
    client.println("Connection: close");
    client.println();
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println(PostData);
    Serial.println();
//Para ver la respuesta del servidor
#ifdef PRINT_HTTP_RESPONSE
    delay(500);
    Serial.println();
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println();
#endif
#endif
  }
}



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
      delay(2);  // espera pequeña entre lecturas para estabilizar
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

  // Serial.begin(115200);

#ifdef PRINT_DEBUG_MESSAGES
  Serial.begin(9600);
#endif
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
    connectWiFi();
    digitalWrite(LED_PIN, HIGH);

#ifdef PRINT_DEBUG_MESSAGES
    Serial.print("Server_Host: ");
    Serial.println(Server_Host);
    Serial.print("Port: ");
    Serial.println(String(Server_HttpPort));
    Serial.print("Server_Rest: ");
    Serial.println(Rest_Host);
#endif
  }
}

// ****************************** Loop ******************************

void loop() {
  Serial.println("______________________________________________");
  //delay(1000);
  // Serial.println("\033[2J\033[H");  //borrar monitor serie

  String data[NUM_FIELDS_TO_SEND + 1];  // Podemos enviar hasta 8 datos

  data[1] = String(tomarTemperatura(1));  //1=num pin de termometro, debuelve un float //Escribimos el dato 1. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Temperatura: ");
  Serial.println(data[1]);
#endif

  data[2] = String(medirSalinidad());  //Escribimos el dato 2. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Salinidad: ");
  Serial.println(data[2]);
#endif

  data[3] = String(medirHumedad(0));  //0=num pin de termometro, debuelve un int //Escribimos el dato 3. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Humedad: ");
  Serial.println(data[3]);
#endif

  data[4] = String(medirPH(2));  //2=num pin de termometro, debuelve un float //Escribimos el dato 2. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("PH: ");
  Serial.println(data[4]);
#endif

  //data[ 5 ] = String( medirLuz(3) ); //3=num pin de termometro, debuelve un float //Escribimos el dato 2. Recuerda actualizar numFields
  data[5] = String(0);  //quitar cuando haya sensor de luz
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Luz: ");
  Serial.println(data[5]);
#endif

  //Selecciona si quieres enviar con GET(ThingSpeak o Dweet) o con POST(ThingSpeak)
  //HTTPPost( data, NUM_FIELDS_TO_SEND );
  HTTPGet(data, NUM_FIELDS_TO_SEND);

  //Selecciona si quieres un retardo de 15seg para hacer pruebas o dormir el SparkFun
  delay(15000);
  //Serial.print( "Goodnight" );
  //ESP.deepSleep( sleepTimeSeconds * 1000000 );
}