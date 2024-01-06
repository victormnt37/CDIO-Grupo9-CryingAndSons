// Incluir la biblioteca SoftwareSerial
#include <SoftwareSerial.h>

// Definir los pines del GPS
#define RX_PIN 12
#define TX_PIN 13

// Crear un objeto SoftwareSerial para comunicarse con el GPS
SoftwareSerial gps(RX_PIN, TX_PIN);

// Crear una variable para almacenar la trama GPGGA
String gpgga;

// Crear una variable para almacenar la posición del asterisco
int pos;

void setup()
{
  // Iniciar la comunicación serie con el ordenador
  Serial.begin(115200);            
  // Iniciar la comunicación serie con el GPS
  gps.begin(9600); 
  // Imprimir un mensaje de inicio
  Serial.println("Inicializando el GPS...");
  // Esperar 5 segundos
  delay(5000);
  // Imprimir un mensaje de espera
  Serial.println("Esperando datos");
}

void loop()
{
  // Si hay datos disponibles del GPS
  if(gps.available())
  {
    // Leer una línea del GPS
    gpgga = gps.readStringUntil('\n');
    // Si la línea empieza por $GPGGA
    if(gpgga.startsWith("$GPGGA"))
    {
      // Encontrar la posición del asterisco
      pos = gpgga.indexOf('*');
      // Extraer la parte de la cadena entre el $ y el *
      gpgga = gpgga.substring(1, pos);
      // Imprimir la trama GPGGA por pantalla
      Serial.println(gpgga);
    }
  }
}