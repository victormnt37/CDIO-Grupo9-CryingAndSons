#define power_pin 5 // Pin para alimentar el sensor de salinidad

void setup() {
  pinMode(power_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
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

  // Presentamos lectura
  Serial.print("Lectura digital = ");
  Serial.println(adc0, DEC);
  Serial.print("Salinidad de lectura = ");
  Serial.println(salinidad, 2);

  // Calibrado del sensor
  float salinidadReal = 288 + 33.067*salinidad - 2.92*pow(salinidad, 2) + 0.0853*pow(salinidad, 3);

  Serial.print("Salinidad de cálculo = ");
  Serial.println(salinidadReal, 2);

  delay(1000); // Puedes ajustar el tiempo de espera entre lecturas
}
