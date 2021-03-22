#include <stdint.h>
#include <TM4C123GH6PM.h>
#define PORTB GPIO_PORTB_DATA_R
#define PORTD GPIO_PORTD_DATA_R
//************** Variables *****************************
uint8_t check;
uint8_t conteo1;
uint8_t conteo2;
uint8_t jugadores1;
uint8_t jugadores2;
uint8_t enable;
uint8_t enable1;
int PORTD_1[]= {PE_4,PE_5,PD_2,PD_3,PE_1, PE_2, PD_6, PD_7};
//************ Prototipos de funciones*******************
void luces (void);
void habilitar_semaforo (void);
void juego1(void);
void juego2(uint8_t cont);
void jugador1(void);
void jugador2(void);
//**********************setup****************************
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PE_3, INPUT_PULLUP);
  pinMode(PE_4, OUTPUT);
  pinMode(PE_5, OUTPUT);
  pinMode(PD_2, OUTPUT);
  pinMode(PD_3, OUTPUT);
  pinMode(PE_1, OUTPUT);
  pinMode(PE_2, OUTPUT);
  pinMode(PD_6, OUTPUT);
  pinMode(PD_7, OUTPUT);
  pinMode(PB_0, OUTPUT);
  pinMode(PB_1, OUTPUT);
  pinMode(PB_2, OUTPUT);
  pinMode(PB_3, OUTPUT);
  pinMode(PB_4, OUTPUT);
  pinMode(PB_5, OUTPUT);
  pinMode(PB_6, OUTPUT);
  pinMode(PB_7, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  jugadores1 = 0;
  jugadores2 = 0;
  check = 0;
  enable = 0;
  enable1 = 0;
}
//***************main loop********************************
void loop() {
  // put your main code here, to run repeatedly:
  habilitar_semaforo();//funcion que revisa el boton de inicio de juego, coloca check en 1 y hace el semaforo 
  if (check == 1) { //si ya se presiono el boton de activar juego y nadie ha ganado pasa lo siguente
    jugador1(); //antirebores para el boton 1
    jugador2(); //antirebores para el boton 2
    juego1(); //rutina que enciende los LEDS del jugador 1
    juego2(jugadores2); //rutina que enciende los LEDS del jugador 2
    delay(100);
    Serial.print("J1: "); //impresiones en el puerto serial para llevar el control de las variables 
    Serial.print(jugadores1);
    Serial.print(" J2: ");
    Serial.println(jugadores2);
    Serial.print("PORTD : ");
    Serial.print(PORTD);
    Serial.print(" PORTB : ");
    Serial.println(PORTB);
    
  }
  delay(100);
}
void luces (void) { //rutina que enciende y apaga las lueces RGB de la tiva como un semaforo
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(500);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED,HIGH);
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}
void habilitar_semaforo (void) {
  if (digitalRead(PE_3) == LOW) { //si ya se presiono el boton enable pasa la condicional 
    digitalWrite(BLUE_LED, LOW); //se apagan los leds de los ganadores 
    digitalWrite(GREEN_LED, LOW);
    Serial.println("inicio semaforo"); //se imprime en el puerto serial 
    luces(); //se llama la rutian de las luces 
    check = 1; //habilita el check del inicio del juego y se limpian las variables para empezar desde 0
    PORTD = 0;
    PORTB = 0;
    conteo1 = 0;
    conteo2 = 0;
    jugadores1 = 0;
    jugadores2 = 0;

  }
}
void jugador1 (void ) { //antirrebotes del boton 1
  if (digitalRead(PUSH1) == LOW) { //si se presiona se encuende una bandera 
    conteo1 = 1;
  }
  if (digitalRead(PUSH1) == HIGH && conteo1 == 1) { //si el boton se encuentra sin presionar y fue presionado ejecutar la accion 
    conteo1 = 0;
    enable = 1;
    jugadores1++;
  }
}
void jugador2 (void) { //antirrebotes del boton 2
  if (digitalRead(PUSH2) == LOW) { //si se presiona se encuende una bandera 
    conteo2 = 1;
  }
  if (digitalRead(PUSH2) == HIGH && conteo2 == 1) { //si el boton se encuentra sin presionar y fue presionado ejecutar la accion 
    enable1 = 1;
    conteo2 = 0;
    jugadores2++;
  }
}
void juego1(void) {
    if (jugadores1==1){ //si el contador del jugador 1 esta en 1, encender el primer bits del arreglo 
    digitalWrite(PE_4,HIGH);
    }else if (jugadores1>1 && jugadores1<=8){ //si esta entre 2 y 8 encender el bit del arreglo que toca y apagar el anterior 
    digitalWrite(PORTD_1[jugadores1-1],HIGH);
    digitalWrite(PORTD_1[jugadores1-2],LOW);
  }else if (jugadores1 > 8){ //cuando se llega al final, encender el indicador de ganador, limpiar las variables y bloquear el inicio de juego 
      digitalWrite(GREEN_LED, HIGH);
      check = 0;
      PORTD = 0;
      jugadores2 = 0;
      jugadores1 = 0;
  }
}
void juego2(uint8_t cont) {
  switch (cont) {
    case 1: //si el contador del jugador 2 esta en 1, encender el primer bit del puerto B
      digitalWrite(PB_0, HIGH);
      break;
    case 9:
      digitalWrite(BLUE_LED, HIGH); //cuando se llega al final, encender el indicador de ganador, limpiar las variables y bloquear el inicio de juego 
      check = 0;
      PORTB = 0;
      jugadores2 = 0;
      jugadores1 = 0;
      break;
    default: //si esta entre 2 y 8 encender el bit del arreglo que toca y apagar el anterior 
    if (enable1 == 1){ 
      enable1 = 0;
      PORTB = PORTB << 1;
    }
      break;

  }
}
