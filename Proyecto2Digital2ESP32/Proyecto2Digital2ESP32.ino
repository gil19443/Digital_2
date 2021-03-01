#define RXD2 16
#define TXD2 17
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(1000);
}
void loop() {
  if (Serial2.available() > 0) {
    char lectura;
    lectura = Serial2.read();
    Serial.print(lectura);
  }
}
