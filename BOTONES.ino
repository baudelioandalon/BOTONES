#include "PCF8574.h"
#include <Wire.h>    // Required for I2C communication
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);  

PCF8574 boton(0x24);  //ARRIBA
PCF8574 expander2(0x20);  //ABAJO

/***    NO EXCEDER LOS 18 CARACTERES    ***/
/***    SE COMPROBARAN ANTES DE INICIAR EL SISTEMA    ***/

String titulos[] = {
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


//
//String elementoTexto1 = "PINOL"; //5 Caracteres
//String elementoTexto2 = "CLORO"; //5 Caracteres 
//String elementoTexto3 = "ARIEL LIQUIDO"; //13 Caracteres 
//String elementoTexto4 = "FABULOSO"; //8 Caracteres 
//String elementoTexto5 = "SUAVITEL AZUL"; //13 Caracteres 
//String elementoTexto6 = "SUAVITEL AMARILLO"; //17 Caracteres 
//String elementoTexto7 = "SUAVITEL ECONOMICO"; //18 Caracteres 
//String elementoTexto8 = "AXION"; //5 Caracteres 
//String elementoTexto9 = "DESENGRASANTE"; //13 Caracteres 
//String elementoTexto10 = ; //18 Caracteres 

#define B1  P0   //Boton 1
#define B2  P1   //Boton 2
#define B3  P2   //Boton 3
#define B4  P3   //Boton 4
#define B5  P4   //Boton 5
#define B6  P5   //Boton 6
#define B7  P6   //Boton 7
#define B8  P7   //Boton 8
#define B9 36    //Boton 9
#define B10 39   //Boton 10


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
#define BReset 34  //Boton de reset
#define BCancelar 35 //Boton de cancelar

const int EncendidoLCoin = 16;  //Funciona para encender el Lector de monedas
const int EncendidoHCoin = 17;  //Funciona para encender el Hopper

const int SDA_PIN =  21;    //Pin de comunicacion SDA
const int SCL_PIN =  22;    //Pin de comunicacion SCL

const int HCoin = 13; //Pin entrada de Hopper
const int LCoin = 4;  //Pin entrada de Lector de monedas


void setup() {  
  Wire.begin(SDA_PIN,SCL_PIN);
  Serial.begin(115200);
  
  lcd.init();                 
  lcd.backlight();

  //Habilitar boton Reset
  pinMode(BReset, INPUT_PULLUP);
  //Habilitar boton Cancelar
  pinMode(BCancelar, INPUT_PULLUP);

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


  //            String    int     int       boolean       boolean       boolean
  //Parametros   texto, columna, fila, limpiarPantalla, centrarColumna, centrarFila
  pantalla("COMPROBANDO",0,0,true,true,true);
  ifClean(true);
  

}


void pantalla(String texto, int columna, int fila, boolean limpiarPantalla, boolean centeredColumna, boolean centeredFila){

  if(sizeof(texto) > 20){
    delay(100);
    ifClean(true);
    lcd.setCursor(ifCenteredColumn("NOMBRE MUY LARGO", true, 0), 1);
    msj("NOMBRE MUY LARGO");
    msj(texto);
    lcd.print(texto);
    return;
  }else if(sizeof(texto) == 0){
    delay(100);
    ifClean(true);
    lcd.setCursor(ifCenteredColumn("NOMBRE VACIO", true, 2), ifCenteredRow(centeredFila, fila));
    lcd.print(texto);
    msj("NOMBRE VACIO");
    msj(texto);
    return;
  }else{
    ifClean(limpiarPantalla);
    lcd.setCursor(ifCenteredColumn(texto, centeredColumna, columna), ifCenteredRow(centeredFila, fila));
    lcd.print(texto);
  }
    

}//fin pantalla



/* Centrar fila */
int ifCenteredRow(boolean centered, int fila){

int filaNueva = 0;

if(centered){
  filaNueva = 1;
}else{
  filaNueva = fila;
}

return filaNueva;

}//fin ifCenteredRow

/* Centrar columna */
int ifCenteredColumn(String texto, boolean centered, int columna){
  int nuevaColumnaInicial = 0;

if(centered){
  /******   Comprobar tamaño para calcular acomodo de letras  ******/
  if(sizeof(texto) == 20){
    nuevaColumnaInicial  = 0;
  }else{
    nuevaColumnaInicial  = (20 - sizeof(texto))/2;
  }
}else{
  nuevaColumnaInicial = columna;
}
    
  return nuevaColumnaInicial;

}//fin ifCenteredColumn


/* Limpiar pantalla */
void ifClean(boolean ifCleanValue){

if(ifCleanValue){
    lcd.clear();
  }
}


void loop() {

if(BReset == HIGH){
  
  while (10>1)
  {
    if(readBotones() == 1){

    }
  }
  

}else{

}

}


int readBotones(){
 
  int botonPresionado = 0;
  
  if(boton.digitalRead(P0) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 1");
      botonPresionado = 1;
    delay(50);
  }else if(boton.digitalRead(P1) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 2");
      botonPresionado = 2;
    delay(50);
  }else if(boton.digitalRead(P2) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 3");
      botonPresionado = 3;
    delay(50);
    
  }else if(boton.digitalRead(P3) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 4");
      botonPresionado = 4;
    delay(50);
  }else if(boton.digitalRead(P4) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 5");
      botonPresionado = 5;
    delay(50);
  }else if(boton.digitalRead(P5) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 6");
      botonPresionado = 6;
    delay(50);
  }else if(boton.digitalRead(P6) == HIGH){
      pantalla("boton 7",0,0,true,true,true);
      botonPresionado = 7;
    delay(50);
    
  }else if(boton.digitalRead(P7) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 8");
      botonPresionado = 8;
    delay(50);
  }else if(digitalRead(B9) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 9");
      botonPresionado = 9;
    delay(50);
  }else if(digitalRead(B10) == HIGH){
      lcd.clear(); //con esto limpias pantalla
      lcd.setCursor(0, 0);    //Columna, Fila
      lcd.print("boton 10");
      botonPresionado = 10;
    delay(50);
  }

  return botonPresionado;
}


void msj(String mensaje){
  Serial.println(mensaje);
}


boolean comprobarLongitudTexto(){
  boolean correcto = false;

  for(int x = 0; x < sizeof(titulos); x++){
    if(sizeof(titulos[x])){
      correcto = true;
    }
  }

return correcto;
}
