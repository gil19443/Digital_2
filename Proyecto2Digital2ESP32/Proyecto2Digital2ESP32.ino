#define RXD2 16
#define TXD2 17
#include <AdafruitIO.h>
#include "config.h"
int toggle = 0;
char lectura[18];
char *segundo;
char *minuto;
char *hora1;
char *checkdate;
char *hora;
char *i;
char leds1;
char leds2;
AdafruitIO_Feed *segundos = io.feed("segundos");
AdafruitIO_Feed *minutos = io.feed("minutos");
AdafruitIO_Feed *horas = io.feed("horas");
AdafruitIO_Feed *fecha = io.feed("fecha");
AdafruitIO_Feed *LED1 = io.feed("LED1");
AdafruitIO_Feed *LED2 = io.feed("LED2");
void concat_fecha (void);
void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // start the serial connection
  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();
  LED1->onMessage(handleMessage);
  LED2->onMessage(handleMessage1);
  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  LED1->get();
  LED2->get();
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}
void loop() {
  
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  switch(toggle){
    case 0:
        digitalWrite(2, HIGH);
        Serial.print("sending -> ");
        Serial.println(String(segundo));
        write_adafruit (segundos, segundo);
        Serial.print("sending -> ");
        Serial.println(String(minuto));
        write_adafruit (minutos, minuto);
        toggle++;
        break;
    case 1:
        digitalWrite(2, HIGH);
        Serial.print("sending -> ");
        Serial.println(10);
        write_adafruit (segundos, segundo);
        Serial.print("sending -> ");
        Serial.println(String(hora));
        write_adafruit (horas, hora);
        toggle++;
        break;
    case 2:
        digitalWrite(2, HIGH);
        Serial.print("sending -> ");
        Serial.println(10);
        write_adafruit (segundos, segundo);
        Serial.print("sending -> ");
        Serial.println(String(checkdate));
        write_adafruit (fecha, checkdate);
        toggle = 0;
        break;
  }
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(4000);
  TX_PIC();
  concat_fecha();
}
void concat_fecha (void) {
  hora = strtok(lectura, ":");
  minuto = strtok(NULL, ":");
  segundo = strtok(NULL, ",");
  checkdate = strtok(NULL, ",");
  }
void handleMessage(AdafruitIO_Data *data) {
  String led1 = data->toString();
  if (led1 == "1"){
    leds1 = 53;
  }else{
    leds1 = 0;
  }
  Serial.print("received <- ");
  Serial.println(data->value());

}
void handleMessage1(AdafruitIO_Data *data) {
  String led2 = data->toString();
  if (led2 == "1"){
      leds2 = 53;
  }else{
      leds2 = 0;
  }
  Serial.print("received <- ");
  Serial.println(data->value());

}
void TX_PIC (void){
    if (Serial2.available() > 0) {
    Serial2.write(leds1);
    Serial2.write(leds2);
    Serial2.write(51);
    Serial2.write(10);
    Serial2.readBytesUntil(44, lectura, 19);
    Serial.println(lectura);
    Serial.println(hora);
    digitalWrite(2, LOW);
  }
}
void write_adafruit (AdafruitIO_Feed *port, char *port1){
  if (port1 !=NULL){
      port->save(String(port1));
  }

}
