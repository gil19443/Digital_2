 #define RXD2 16
 #define TXD2 17
 HardwareSerial MySerial(2);
 void setup() {
   Serial.begin(115200);
   Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
   //delay(1000);
   Serial.println("Loopback program started");
 }
 void loop() {
    Serial.println("Loopback program started");
   //if(Serial2.available()>0){
     Serial.println("Loopback program started");
     Serial.print(Serial2.readString());  
   }
