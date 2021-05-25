//****************************** Librerías*********************************************
#include <WiFi.h>
#include <WebServer.h>
//*******************************macros************************************************
#define RXD2 16
#define TXD2 17
//******************************variables**********************************************
uint8_t var1 = 0;
char lectura[5];

const char* ssid = "UNE_F45A";  // Enter your SSID here
const char* password = "00002501207273";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)
//******************************prototipo de funciones*********************************
void tabla(uint8_t g, uint8_t f, uint8_t e, uint8_t d, uint8_t c, uint8_t b, uint8_t a);
void conteo (uint8_t var);
//*************************************************************************************
void setup() {
  pinMode(33, OUTPUT);//Pines de salida para el display
  pinMode(32, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(22, OUTPUT);
  Serial.begin(115200);//inicializacion del UART0
  //Configuracion para el servidor 
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  WiFi.begin(ssid, password);
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  //estando en la direccion ip, si no sucede nada, llamar a la funcion handle_OnConnect
  //esto es util en este caso, ya que la interfaz no tiene botones
  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8

  //si el servidor no funciona, ir a la funcion handle_NotFound
  server.onNotFound(handle_NotFound);
  //iniciar el servidor 
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
  //enlazar al cliente 
  server.handleClient();
  if (Serial2.available() > 0) { //secion de envio y recepcion UART con la tiva  
    Serial2.readBytesUntil(10, lectura, 4); //rutina que lee 8 bytes hasta el caracter "\n" y lo guarda en el arreglo llamado lectura 
    //Serial.println(lectura); //se verfica que los datos guardados en lectura sean correctos 
  }
  var1 = 0;//se limpia variable temporal que almacena el numero de parqueos ocupados 
  for (int i = 0; i<4;i++){//ciclo que revisa el numero de parqueos ocupados en el arreglo llamado lecutra 
    if (lectura[i] =='1'){//cada ver que encuentra uno, incrementa la variable temporal 
      var1++;
    }
    }
    conteo(4-var1);//funcion que muestra el valor de parqueos disponibles en el display 
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() { //funcion que se llama cuandno no sucede nada a la interfaz, envia el string SendHTML
  server.send(200, "text/html", SendHTML(lectura[0],lectura[1],lectura[2],lectura[3]));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(char p1, char p2, char p3, char p4) {//esta funcion contiene la programacion de la interfaz
  String ptr = "<!DOCTYPE html>\n"; //recibe cada uno de los caracteres recibidos para determinar los graficos que se muestran
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "<script>\n";
  ptr += "<!--\n";
  ptr += "function timedRefresh(timeoutPeriod) {\n";
  ptr += "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "window.onload = timedRefresh(5000);\n";
  ptr += "\n";
  ptr += "//   -->\n";
  ptr += "</script>";
  ptr += "<style>\n";
  ptr += "header {\n";
  ptr += "background-color: #255;\n";
  ptr += "padding: 20px;\n";
  ptr += "text-align: center;\n";
  ptr += "  font-size: 35px;\n";
  ptr +="  color: white;\n";
  ptr +="}footer {\n";
  ptr +="  background-color: #255;\n";
  ptr +="  padding: 10px;\n";
  ptr +="  text-align: center;\n";
  ptr +="  color: white;\n";
  ptr +="}table, th, td {\n";
  ptr +="  border: 5px solid black;\n";
  ptr +="}th, td {\n";
  ptr +="  padding: 10px;\n";
  ptr +="}th, td {\n";
  ptr +="  text-align: center;\n";
  ptr +="}td {\n";
  ptr +="  font-size:48px;\n";
  ptr +="}table{\n";
  ptr +="  width:100%;\n";
  ptr +="}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="  <header>\n";
  ptr +="  <h2>PARQUE - O - MATIC</h2>\n";
  ptr +="  </header>\n";
  ptr +="<table>\n";
  ptr +="  <tr>\n";
  ptr +="    <th>Parqueo 1</th>\n";
  ptr +="    <th>Parqueo 2</th>\n";
  ptr +="    <th>Parqueo 3</th>\n";
  ptr +="    <th>Parqueo 4</th>\n";
  ptr +="  </tr>\n";
  ptr +="  <tr>\n";
  if (lectura[0]=='0'){ //acrode con el estado de cada parqueo, se escoge si mostrar le corazon verde o el circuilo rojo 
    ptr +="    <td>&#x1f49a;</td>\n";
  }else{
    ptr +="    <td>&#x1f534;</td>\n";
  }
  if (lectura[1]=='0'){
    ptr +="    <td>&#x1f49a;</td>\n";
  }else{
    ptr +="    <td>&#x1f534;</td>\n";
  }
  if (lectura[2]=='0'){
    ptr +="    <td>&#x1f49a;</td>\n";
  }else{
    ptr +="    <td>&#x1f534;</td>\n";
  }
  if (lectura[3]=='0'){
    ptr +="    <td>&#x1f49a;</td>\n";
  }else{
    ptr +="    <td>&#x1f534;</td>\n";
  }  
  ptr +="  </tr>\n";
  ptr +="  <tr>\n";
  if (lectura[0] == '1'){//acrode con el estado de cada parqueo, se escoge si mostrar el carro o la mano que apunta 
    ptr +="    <td>&#x1f698;</td>\n";
  }else{
    ptr +="    <td>&#x1f446;</td>\n";
  }
  if (lectura[1] == '1'){
    ptr +="    <td>&#x1f698;</td>\n";
  }else{
    ptr +="    <td>&#x1f446;</td>\n";
  }
  if (lectura[2] == '1'){
    ptr +="    <td>&#x1f698;</td>\n";
  }else{
    ptr +="    <td>&#x1f446;</td>\n";
  }
  if (lectura[3] == '1'){
    ptr +="    <td>&#x1f698;</td>\n";
  }else{
    ptr +="    <td>&#x1f446;</td>\n";
  }
  ptr +="  </tr>\n";
  ptr +="</table>\n";
  ptr +="<footer>\n";
  if (var1 == 0){ //se revisa la misma variable que ingresa al display de 7 segmentos 
    ptr +="<h2>Parqueos disponibles: 4</h2>\n"; //para colocar el numero de parqueos disponibles 
  }else if (var1 == 1){
    ptr +="<h2>Parqueos disponibles: 3</h2>\n";
  }else if (var1 == 2){
    ptr +="<h2>Parqueos disponibles: 2</h2>\n";
  }else if (var1 == 3){
    ptr +="<h2>Parqueos disponibles: 1</h2>\n";
  }else if (var1 == 4){
    ptr +="<h2>Parqueos disponibles: 0</h2>\n";
  }
  ptr +="</footer>\n";
  ptr +="</body>\n";
  ptr +="</html>";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() { //funcion para cuando no funciona el servidor
  server.send(404, "text/plain", "Not found");
}
void conteo (uint8_t var){ //funcion que recibe un valor en decimal para colocar le mismo valor en el display de 7 segmentos 
    switch(var){
    case 0:
      tabla(1,0,0,0,0,0,0); //funcion que codifica los pines del display 
      break;
    case 1:
      tabla(1,1,1,1,0,0,1);
      break;
    case 2:
      tabla(0,1,0,0,1,0,0);
      break;
    case 3:
      tabla(0,1,1,0,0,0,0);
      break;
    case 4:
      tabla(0,0,1,1,0,0,1);
      break;
  }
}
void tabla(uint8_t g, uint8_t f, uint8_t e, uint8_t d, uint8_t c, uint8_t b, uint8_t a){//funcion para encender los pines del display 
  if (g == 1){
    digitalWrite(33, HIGH);
  }else{
    digitalWrite(33, LOW);
    
  }
  if (f == 1){
    digitalWrite(32, HIGH);
  }else{
    digitalWrite(32, LOW);
    
  }
  if (e == 1){
    digitalWrite(21, HIGH);
  }else{
    digitalWrite(21, LOW);
    
  }
  if (d == 1){
    digitalWrite(19, HIGH);
  }else{
    digitalWrite(19, LOW);
    
  }
  if (c == 1){
    digitalWrite(22, HIGH);
  }else{
    digitalWrite(22, LOW);
    
  }
  if (b == 1){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
    
  }
  if (a == 1){
    digitalWrite(27, HIGH);
  }else{
    digitalWrite(27, LOW);
    
  }
}
