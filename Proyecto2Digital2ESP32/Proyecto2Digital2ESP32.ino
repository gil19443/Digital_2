#define RXD2 16
#define TXD2 17
#include <AdafruitIO.h>
#include "config.h"
int toggle = 0;
int i = 0;
char lectura[16]="";
AdafruitIO_Feed *segundos = io.feed("segundos");
void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    // start the serial connection
  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

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

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println((int(lectura[7])*10)+int(lectura[8]));
  segundos->save((int(lectura[7])*10)+int(lectura[8]));
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);
  digitalWrite(2, HIGH);
  if (Serial2.available() > 0) {
    
    Serial2.write(50);
    Serial2.write(51);
    Serial2.write(10);
    Serial2.readBytesUntil(10,lectura,17);
    Serial.print("los segundos son");
    Serial.println(lectura[7]);
    }
  }
