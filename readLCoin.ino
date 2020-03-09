#include "PCF8574.h"
#include <Wire.h>    // Required for I2C communication
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);  

PCF8574 boton(0x24);  //ARRIBA
PCF8574 expander2(0x20);  //ABAJO

/***    NO EXCEDER LOS 20 CARACTERES    ***/
/***    SE COMPROBARAN ANTES DE INICIAR EL SISTEMA    ***/

String titulos[10] = {
"PINOL",
"CLORO",
"ARIEL LIQUIDO",
"FABULOSO",
"SUAVITEL AZUL",
"SUAVITEL AMARILLO",
"SUAVITEL ECONOMICO",
"AXION",
"DESENGRASANTE",
"DETERGENTE LIQUIDO"};


#define B1  P0   //Boton 1
#define B2  P1   //Boton 2
#define B3  P2   //Boton 3
#define B4  P3   //Boton 4
#define B5  P4   //Boton 5
#define B6  P5   //Boton 6
#define B7  P6   //Boton 7
#define B8  P7   //Boton 8
#define B9 39    //Boton 9
#define B10 36   //Boton 10


#define M1  0  //Motor 1
#define M2  1  //Motor 2
#define M3  2  //Motor 3
#define M4  3  //Motor 4
#define M5  4  //Motor 5
#define M6  5  //Motor 6
#define M7  6  //Motor 7
#define M8  7  //Motor 8
#define M9  32  //Motor 10
#define M10 33  //Motor 10
#define BReset 18  //Boton de reset
#define BCancelar 35 //Boton de cancelar

const int EncendidoLCoin = 16;  //Funciona para encender el Lector de monedas
const int EncendidoHCoin = 17;  //Funciona para encender el Hopper

const int SDA_PIN =  21;    //Pin de comunicacion SDA
const int SCL_PIN =  22;    //Pin de comunicacion SCL

const int HCoin = 34; //antes pin 13 Pin entrada de Hopper
const int LCoin = 13;  //Pin entrada de Lector de monedas

String ultimoTextoRow = "";
int temporalPulsado = 20;

int countMonedas = 0;

bool activado = true;

void setup() {  

  Wire.begin(SDA_PIN,SCL_PIN);
  Serial.begin(115200);
  
  lcd.init();                 
  lcd.backlight();
  

  //Habilitar boton Reset
  pinMode(BReset, INPUT_PULLUP);
  //Habilitar boton Cancelar
  pinMode(BCancelar, INPUT_PULLUP);
  //Habilitar readHopper
  pinMode(HCoin, INPUT_PULLUP);
  pinMode(LCoin, INPUT_PULLUP);
 /*  pinMode(HCoin, INPUT_PULLUP);

  digitalWrite(HCoin,HIGH); */
//  const int ledAChannel = 0;
//  ledcSetup(ledAChannel, 1000, 8);
//  ledcAttachPin(HCoin, ledAChannel);



  //Habilitar readLector
  //pinMode(LCoin, INPUT_PULLUP);

  

  //Habilitar entrada de botones
  boton.pinMode(B1, INPUT);
  boton.pinMode(B2, INPUT);
  boton.pinMode(B3, INPUT);
  boton.pinMode(B4, INPUT);
  boton.pinMode(B5, INPUT);
  boton.pinMode(B6, INPUT);
  boton.pinMode(B7, INPUT);
  boton.pinMode(B8, INPUT);
  boton.begin();
  pinMode(B9,INPUT_PULLUP);
  pinMode(B10,INPUT_PULLUP);

  //Habilitar salida a motores (Relevadores)
  expander2.pinMode(M1, OUTPUT);
  expander2.pinMode(M2, OUTPUT);
  expander2.pinMode(M3, OUTPUT);
  expander2.pinMode(M4, OUTPUT);
  expander2.pinMode(M5, OUTPUT);
  expander2.pinMode(M6, OUTPUT);
  expander2.pinMode(M7, OUTPUT);
  expander2.pinMode(M8, OUTPUT);
  expander2.begin();
  pinMode(M9,OUTPUT);
  pinMode(M10,OUTPUT);
  Serial.println("todo correcto");
  delay(1000);
  //ifClean(true);
   //pantalla("COMENZAMOS",0,0,true,true,true);

}


void loop() {

  while(10>1){
    //EL ESTADO NORMAL ES HIGH (ANALOGICO ES 4095)
    if(digitalRead(LCoin) == HIGH){

    }else{
      countMonedas++;
      Serial.println(countMonedas);
      while(10>1){
        if(digitalRead(LCoin) == HIGH){
          break;
        }
      }
    }

  }


}//fin loop





void pantalla(String texto, int columna, int fila, bool limpiarPantalla, bool centeredColumna, bool centeredFila){

  msj(texto);
  if(texto.length() > 20){
    
    ifClean(true);
    lcd.setCursor(ifCenteredColumn("NOMBRE MUY LARGO", true, 0), 1);
    msj("NOMBRE MUY LARGO");
    msj(texto);
    lcd.print("NOMBRE MUY LARGO");
    msj(texto);
    while(10>1){
      delay(1000);
    }
    
  }else if(texto.length() == 0){
    delay(100);
    ifClean(true);
    lcd.setCursor(ifCenteredColumn("NOMBRE VACIO", true, 2), ifCenteredRow(texto, centeredFila, fila));
    lcd.print(texto);
    msj("NOMBRE VACIO");
    msj(texto);

    //Habilitar este while si quieres que no continue si dejas un nombre vacio
    /* while(10>1){
      delay(1000);
    } */
  }else{

    ifClean(limpiarPantalla);
    lcd.setCursor(ifCenteredColumn(texto, centeredColumna, columna), ifCenteredRow(texto,centeredFila, fila));
    lcd.print(texto);
    ultimoTextoRow = texto;
    
  }
    

}//fin pantalla



/* Centrar fila */
int ifCenteredRow(String texto, bool centered, int fila){

int filaNueva = 0;

if(centered){
  filaNueva = 1;
}else{
  if(ultimoTextoRow.length() > texto.length()){
    lcd.setCursor(0,fila);
    lcd.print("                    ");
  }
  filaNueva = fila;
}

return filaNueva;

}//fin ifCenteredRow

/* Centrar columna */
int ifCenteredColumn(String texto, bool centered, int columna){
  int nuevaColumnaInicial = 0;

 if(centered){
  /******   Comprobar tamaño para calcular acomodo de letras  ******/
  if(texto.length() == 20){
    nuevaColumnaInicial  = 0;
  }else{
    nuevaColumnaInicial  = (20 - texto.length())/2;
/*     String spa = "espacio: " + String(nuevaColumnaInicial);
    String tex = "texto: " + String(texto);
    String tam = "tamaño: " + String(texto.length());
    msj(spa);
    msj(tex);
    msj(tam); */
  }
}else{
  nuevaColumnaInicial = columna;
}
    
  return nuevaColumnaInicial;

}//fin ifCenteredColumn


/* Limpiar pantalla */
void ifClean(bool ifCleanValue){

if(ifCleanValue){
    lcd.clear();
  }
}




void msj(String mensaje){
  Serial.println(mensaje);
}


bool comprobarLongitudTexto(){
  
  for(int x = 0; x < 10; x++){

    if(titulos[x].length() > 20){
      msj("--------------------");
      msj("AQUI ESTA EL ERROR");
      msj("--------------------");
      msj(titulos[x]);
      String texto = "POSICION: " + String(x + 1);
      msj(texto);
      msj("--------------------");
      return true;
      break;
    }else{
      
    }
    
  }

  return false;
}//fin comprobarLongitudTexto
