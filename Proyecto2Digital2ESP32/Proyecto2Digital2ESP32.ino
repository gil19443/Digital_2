#define RXD2 16
#define TXD2 17
#include <AdafruitIO.h>
#include "config.h"
int toggle = 0;
char lectura[16] = "";
char date[7] = "";
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
  concat_fecha();
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println((int(lectura[6]) * 10) + int(lectura[7]));
  segundos->save((int(lectura[6]) * 10) + int(lectura[7]));
  Serial.print("sending -> ");
  Serial.println((int(lectura[3]) * 10) + int(lectura[4]));
  minutos->save((int(lectura[3]) * 10) + int(lectura[4]));
  Serial.print("sending -> ");
  Serial.println((int(lectura[0]) * 10) + int(lectura[1]));
  horas->save((int(lectura[0]) * 10) + int(lectura[1]));
  Serial.print("sending -> ");
  Serial.println(date);
  fecha->save(date);
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(8000);
  digitalWrite(2, HIGH);
  if (Serial2.available() > 0) {
    Serial2.write(leds1);
    Serial2.write(leds2);
    Serial2.write(51);
    Serial2.write(10);
    Serial2.readBytesUntil(10, lectura, 17);
    Serial.print("los segundos son");
    Serial.println(lectura[7]);

  }
}
void concat_fecha (void) {
  date[0] = lectura[9];
  date[1] = lectura[10];
  date[2] = lectura[11];
  date[3] = lectura[12];
  date[4] = lectura[13];
  date[5] = lectura[14];
  date[6] = lectura[15];
  date[7] = lectura[16];
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
