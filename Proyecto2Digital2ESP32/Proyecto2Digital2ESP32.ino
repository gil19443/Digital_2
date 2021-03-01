#define RXD2 16
#define TXD2 17
int toggle = 0;
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  //delay(1000);
}
void loop() {
  if (Serial2.available() > 0) {
    char lectura;
    lectura = Serial2.read();
    Serial.print(lectura);
   if (toggle == 1){
        toggle = 0;
        Serial2.write(49);
        Serial2.write(10);
    }else{
        toggle = 1;
        Serial2.write(50);
        Serial2.write(10);
    }

  }
}
