
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
  checkserial(); //revisar lo que ingreso al puerto serial para habilitar que se muestre el archivo seleccionado
  if (enable == 1){ //condicional para que los archivos dentro de la SD solo se muestren al inicio y luego de que el usuario muestre un archivo 
    archivos(); //subrutina que lee el contenido en la SD y lo imprime en pantalla 
  }
  if (enable1 == 1){//condicional que habilita que se imprima el archivo seleccionado unicamente cuando se seleccione 
    printimages();
  }
  delay(100);
  enable1 = 0; //se limpian las variables 
  var1 = 0;
}

void archivos (void){ //subrutina que lee el contenido en la SD y lo imprime en pantalla 
    enable = 0;
    root = SD.open("/");

    printDirectory(root, 0); 
}
void printDirectory(File dir, int numTabs) {
   Serial.println("Seleccione el emoji que desea imprimir");
   dir.rewindDirectory();
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) { //se revisa que hayan archivos para abrir 
       // no more files
       break;
     }
    for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
    switch(toggle){ //por cada archivo entra, se omite el primero y para los demas se le coloca la numeracion al apar del nombre del archivo 
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
     
     if (entry.isDirectory()) { //se revisa si es un directorio o un archivo 
       //Serial.println("/"); //para este laboratorio no es necesario mostrar los directorios 
       //printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       
       Serial.print("\t\t"); //si es un archivo colocar su tamaño 
       Serial.println(entry.size(), DEC);
     }
     entry.close();//cerrar el arcivho 
   }
}
void printimages(void){
  enable = 1;
  enable1 = 0;
  printselect(var1); //toggle que abre el archivo actode con lo que se ingreso en el puerto serial 
  if (myFile) { //si se logra abrir el archivo y tiene algo dentro 
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
void printselect(int selector){ //el selector de la rutina acorde con lo ingresado en el puerto serial 
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
void checkserial (void){ //condicionales para colocar el valor de la variable de toggle de los archivos, acorde con el valor equivalente en ASCII
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
