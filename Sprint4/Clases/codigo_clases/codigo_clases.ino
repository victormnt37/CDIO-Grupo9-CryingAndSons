#include <Adafruit_ADS1X15.h>
#include <SensorTemperatura.h>
#include <SensorPh.h>
#include <SensorHumedad.h>
#include <SensorLuz.h>
Adafruit_ADS1115 ads1115;

//Valores de salinidad
#define power_pin 5

//variables para pH
float Offset = 0.35;

//variables para luminosidad
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

int tempA;
int T = 0;
float calibT = 2.65;

// Declaración de los objetos tipo sensor *primer valor es el canal del ADS al que estan conectados
SensorTemperatura sensorTemp(1, ads1115, calibT);
SensorPh sensorPh(2, Offset, ads1115);
SensorHumedad sensorHum(0, ads1115);
SensorLuz sensorLuz(3, ads1115);


//--------------------NUEVA ESCALA DE LUZ--------------------------------
//                        OSCURO == 0
//                     LUZ NATURAL == 1
//                   LUZ ARTIFICIAL == 10

//creo que hay un error en vuetro algoritmo, chequear la clase y corregir si hace falta
// float voltage5 = (adc10*4.096)/32767; // linea innecesaria nunca llegais a usar los resultados de voltage5 solo de adc10



void setup() {
#ifdef PRINT_DEBUG_MESSAGES
  Serial.begin(9600);
#endif
  Serial.println("Programa de lectura de NTC con ESP8266 y ADS1115");

  ads1115.begin(0x48);
  ads1115.setGain(GAIN_ONE);

  if (!ads1115.begin()) {  // Inicializar el ADC para la humedad
    Serial.println("No se pudo inicializar el ADC del primer programa.");
    while (1)
      ;
  }
  pinMode(power_pin, OUTPUT);  // Inicialixar el pin mode para salinidad

  Serial.println("¿Desea ejecutar el menú? (y/n)");

  while (!Serial.available()) {
    // Esperar a que el usuario ingrese datos
  }

  sensorTemp.setCalibration(calibT);

  char respuesta = Serial.read();

  if (respuesta == 'y' || respuesta == 'Y') {
    // Ejecutar el menú si la respuesta es 'y' o 'Y'
    int seleccion;
    bool salir = false;

    while (true) {

      if (salir) {
        break;
      }
      // Mostrar las opciones del menú
      Serial.println("Seleccione una opción:");
      Serial.println("1. Calibrar sensor Humedad");
      Serial.println("2. Calibrar sensor Temperatura");
      Serial.println("3. Calibrar sensor PH");
      Serial.println("4. Calibrar sensor Luz");
      Serial.println("5. Salir del menú");

      // Leer la selección del usuario
      while (!Serial.available()) {
        // Esperar a que el usuario ingrese datos
      }
      seleccion = Serial.parseInt();

      int numpin = -1;
      // Realizar la acción según la selección
      switch (seleccion) {
        case 1:
          Serial.println("Pin 0: Humedad");
          //calibrar Humedad
          Serial.println("Coloque el sensor en condiciones de seco y presione Enter");
          while (!Serial.available()) {
          }
          while (Serial.available()) { // Limpiar el buffer de entrada para evitar problemas con futuras lecturas
            Serial.read();
          }
          sensorHum.calibrarSeco();// Lanzar método de calibrar en seco

          Serial.println("Coloque el sensor en condiciones de mojado y presione Enter");// Mensaje de que el sensor tiene que estar en mojado
          while (!Serial.available()) {
          }
          while (Serial.available()) {
            Serial.read();
          }
          sensorHum.calibrarMojado();// Lanzar método de calibrar en mojado

          Serial.println("Calibración completada.");
          break;

        case 2:
          Serial.println("Pin 1: Temperatura");

          while (true) {
            Serial.println("Ingrese el valor de temperatura ambiente:");

            while (!Serial.available()) {
              // Espera a que el usuario ingrese un valor
            }
            String inputString = Serial.readStringUntil('\n');  // Leer la entrada del usuario hasta que presiona Enter
            inputString.trim();                                 // Eliminar espacios en blanco al principio y al final
            // Validar la entrada para asegurarse de que sea un número
            if (inputString.length() > 0 && inputString.toInt() != 0) {
              tempA = inputString.toInt();
              break;  // Salir del bucle si la entrada es válida
            } else {
              Serial.println("Entrada no válida. Por favor, ingrese un número.");
            }
          }
          T = sensorTemp.lecturaTemperatura();
          calibT = tempA - T;
          //mandar calibT a la clase temperatura
          sensorTemp.setCalibration(calibT);

          Serial.println("Calibración completada.");
          break;
        case 3:
          Serial.println("Pin 2: PH");
          //calibrar PH
          break;
        case 4:
          Serial.println("Pin 3: Luz");
          //calibrar Luz
          break;
        case 5:
          Serial.println("Salir");
          salir = true;
          break;  // Salir del bucle y, por lo tanto, del menú
        default:
          Serial.println("Opción no válida. Inténtelo de nuevo.");
          break;
      }
    }
  }
  //Menu de inicializado
  delay(1000);
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
float medirSalinidad() {
  int16_t adc0;
  float salinidad;

  // Alimentamos la sonda con un tren de pulsos
  digitalWrite(power_pin, HIGH);
  delay(100);
  adc0 = analogRead(A0);
  digitalWrite(power_pin, LOW);
  delay(100);
  salinidad = map(adc0, 0, 1023, 0, 500);
  return salinidad;
}



void loop() {
  Serial.println("________________");
  //delay(1000);
  // Serial.println("\033[2J\033[H");  //borrar monitor serie

  String data[NUM_FIELDS_TO_SEND + 1];  // Podemos enviar hasta 8 datos

  data[1] = String(sensorTemp.lecturaTemperatura());  //debuelve un float //Escribimos el dato 1. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Temperatura: ");
  Serial.println(data[1]);
#endif

  data[2] = String(medirSalinidad());  //Escribimos el dato 2. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Salinidad: ");
  Serial.println(data[2]);
#endif

  data[3] = String(sensorHum.lecturaHumedad());  //debuelve un int //Escribimos el dato 3. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Humedad: ");
  Serial.println(data[3]);
#endif

  data[4] = String(sensorPh.lecturaPh());  // debuelve un float //Escribimos el dato 2. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("PH: ");
  Serial.println(data[4]);
#endif

  data[5] = String(sensorLuz.lecturaLuz());  // debuelve un float //Escribimos el dato 2. Recuerda actualizar numFields
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Luz: ");
  Serial.println(data[5]);
#endif

  //Selecciona si quieres enviar con GET(ThingSpeak o Dweet) o con POST(ThingSpeak)
  //HTTPPost( data, NUM_FIELDS_TO_SEND );
  HTTPGet(data, NUM_FIELDS_TO_SEND);

  //Selecciona si quieres un retardo de 15seg para hacer pruebas o dormir el SparkFun
  delay(10000);
  // Serial.flush();
  //Serial.print( "Goodnight" );
  //ESP.deepSleep( sleepTimeSeconds * 1000000 );
}