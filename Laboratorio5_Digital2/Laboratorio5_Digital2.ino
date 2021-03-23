/*
  Listfiles
 
 This example shows how print out the files in a 
 directory on a SD card 
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 2 Feb 2014
 by Scott Fitzgerald
 
 This example code is in the public domain.

 */
#include <SPI.h>
#include <SD.h>

File root;
File myFile;
int enable;
int enable1;
int var1;
int toggle;
//******************prototipos de funcinoes*********************************
void archivos (void);
void printDirectory(File dir, int numTabs);
void printimages(void);
void  printselect(int selector);
void checkserial (void);
//************************************************************************
void setup(){
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SPI.setModule(0);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);
  if (!SD.begin(32)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  enable = 1;
  enable1 = 0;
}

void loop(){
  checkserial();
  if (enable == 1){
    archivos();
  }
  if (enable1 == 1){
    printimages();
  }
  delay(100);
  enable1 = 0;
  var1 = 0;
}

void archivos (void){
    enable = 0;
    root = SD.open("/");

    printDirectory(root, 0);
}
void printDirectory(File dir, int numTabs) {
   Serial.println("Seleccione el emoji que desea imprimir");
   dir.rewindDirectory();
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
    for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
    switch(toggle){
        case 0:
          entry.name();
          toggle = 1;
          break;
        case 1:
          Serial.print(toggle);
          Serial.print(".");
          Serial.print(entry.name());
          toggle = 2;
          break;
        case 2:
          Serial.print(toggle);
          Serial.print(".");
          Serial.print(entry.name());
          toggle = 3;
          break;
        case 3:
          Serial.print(toggle);
          Serial.print(".");
          Serial.print(entry.name());
          toggle = 0;
          break;
       }
     
     if (entry.isDirectory()) {
       //Serial.println("/");
       //printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
void printimages(void){
  enable = 1;
  enable1 = 0;
  printselect(var1);
  if (myFile) {
    Serial.println("el archivo seleccionado fue");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
void printselect(int selector){
    switch(selector){
      case 1:
       myFile = SD.open("carafe~1.txt");
        break;
      case 2:
        myFile = SD.open("caraco~1.txt");
        break;
      case 3:
        myFile = SD.open("carall~1.txt");
        break;
      default:
        myFile = SD.open("nada.txt");
        break;
    }
}
void checkserial (void){
    int check = Serial.read();
    if (check == 49){
      enable1 = 1;
      var1 = 1;
    }else if (check == 50){
      enable1 = 1;
      var1 = 2;
    }else if (check == 51){
      enable1 = 1;
      var1 = 3;
    }
    
    
}
