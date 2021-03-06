#define RXD2 16
#define TXD2 17
#include <AdafruitIO.h>
#include "config.h"
int toggle = 0;
char lectura[8];
char leds1;
char leds2;
int EN;
AdafruitIO_Feed *horas = io.feed("horas"); //feed que muestra la hora 
AdafruitIO_Feed *LED1 = io.feed("LED1"); //feed que escribe en una de las luces piloto 
AdafruitIO_Feed *LED2 = io.feed("LED2"); //feed que escribe en otra de las luces piloto 
void setup() {
  pinMode(2, OUTPUT); //LED interno del ESP32 como salida para que sirva como indicador 
  Serial.begin(9600); //se inicializa el Serial entre el ESP32 y la computadora
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //se inicializa el Serial2 para comunicar el ESP32 con el PIC16F887
  while (! Serial); //esperar el serial 

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();
  LED1->onMessage(handleMessage); //rutinas que reciben y traducen los valores que se mandan de la plataforma 
  LED2->onMessage(handleMessage1);
  // wait for a connection
  while (io.status() < AIO_CONNECTED) { //espera a que se concecte con Adafruit 
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
  digitalWrite(2, HIGH); //Encender indicador LED como indicador de comunicacion con Adafruit 
  Serial.print("sending -> ");
  Serial.println(lectura); //imprimir el valor que se recibe del PIC en el puerto serial 
  if (EN == 1){ //verificacion de datos 
    horas->save(lectura); // si los datos estan en el orden correcto, enviar el valor recibido al feed horas
  }
  delay(2200); //esperar 2.2ms ya que por la version gratis de adafruit solo se puede enviar un dato cada 2seg
  digitalWrite(2, LOW); //apagar indicador LED de comuniacion con Adafruit 
  if (Serial2.available() > 0) { //secion de envio y recepcion UART con el PIC 
    Serial2.write(leds1); //se escribe una cadena de 4 bytes en el orden mencionado 
    Serial2.write(leds2);
    Serial2.write(1); //dato que le dice al PIC que el ESP32 esta dispuesto a escuchar los datos del puerto serial 
    Serial2.write(10); //caracter para verificar el orden en el PIC
    Serial2.readBytesUntil(10, lectura, 8); //rutina que lee 8 bytes hasta el caracter "n" y lo guarda en el arreglo llamado lectura 
    correccion(); //rutina que verifica el orden de los datos para verificar el ENABLE 
  }
        
}
void handleMessage(AdafruitIO_Data *data) {
  if (data->toString() == "1"){ //verifica el dato recibido del feed LED1 para asignar un valor a una variable que se envia el PIC
    leds1 = 53;
  }else{
    leds1 = 0;
  }
  Serial.print("received <- ");
  Serial.println(data->value());

}
void handleMessage1(AdafruitIO_Data *data) {
  if (data->toString() == "1"){ //verifica el dato recibido del feed LED1 para asignar un valor a una variable que se envia el PIC
      leds2 = 53;
  }else{
      leds2 = 0;
  }
  Serial.print("received <- ");
  Serial.println(data->value());

}
void correccion(void){ //rutina de correcion 
  if (lectura[2] == ':' && lectura[5] == ':'){ //por el formato de hora se sabe que se envio de la forma HH:MM:SS de modo deben haber 
      EN = 1; //: en las posiciones 2 y 5 del buffer que se leyo 
  }else{
      EN = 0;   
  }
}
