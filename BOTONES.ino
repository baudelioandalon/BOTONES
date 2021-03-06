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

const int HCoin = 13; //Pin entrada de Hopper
const int LCoin = 4;  //Pin entrada de Lector de monedas

String ultimoTextoRow = "";
int temporalPulsado = 20;

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

  ifClean(true);

  //            String    int     int       bool            bool          bool
  //Parametros   texto, columna, fila, limpiarPantalla, centrarColumna, centrarFila
  pantalla("COMPROBANDO",0,0,true,true,true);
  
  delay(1000);
  
  if(comprobarLongitudTexto()){
    pantalla("HAY UN ERROR",0,1,true,true,false);
    pantalla("REAJUSTA LOS NOMBRES",0,2,false,true,false);
 
    while(10>1){
      delay(1000);
    }
  }else{
    msj("---------------");
    msj("TODO CORRECTO");
    msj("---------------");
    pantalla("TODO CORRECTO",0,0,true,true,true);
    delay(1500);
    ifClean(true);
  }
  

}


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


void loop() {

if(digitalRead(BReset) == HIGH){
  pantalla("MODO CONFIGURACION",0,1,true,true,false);
  pantalla("ACTIVADO",0,2,false,true,false);
  delay(2000);
  pantalla("MODO CONFIGURACION",0,0,true,true,false);
  pantalla("SELECCIONA UN BOTON",0,1,false,true,false);
  delay(1000);
  
  while (10>1){

    int pulsado = 20;
    pulsado = (readBotones() - 1);
    
    if(pulsado != 19){
      String texto = "";
      texto = "BOTON PULSADO " + String(pulsado + 1);
      pantalla(texto,0,2,false,true,false);
      pantalla(titulos[pulsado],0,3,false,true,false);
      if(temporalPulsado != pulsado){
        temporalPulsado = pulsado;
      }
      else{
        pantalla("PRESIONA OTRA VEZ",0,0,true,true,false);
        pantalla("PARA RESETEAR",0,1,false,true,false);
        pantalla(titulos[pulsado],0,3,false,true,false);
        while(10>1){

          int cancelar = 20;
          cancelar = readConfirmarCancelar();

          if(cancelar == 11){
            pantalla("CANCELADO",0,0,true,true,true);
            temporalPulsado = 20;
            delay(2000);
            break;
          }else if(cancelar == (pulsado + 1)){
            pantalla(titulos[pulsado],0,0,true,true,false);
            pantalla("RESETEADO",0,1,false,true,false);
            temporalPulsado = 20;
            delay(2000);
            break;
          }
          
        }
        //este break es para salir del while si se cancela
        break;
      }
      
    }
    

    
  }
  
}else{
  while(10>1){
    readBotones();
  }

}

}//fin loop


int readBotones(){
 
  int botonPresionado = 0;
  
  if(boton.digitalRead(P0) == HIGH){
      //pantalla("boton 1",0,0,true,true,true);
      botonPresionado = 1;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P1) == HIGH){
      //pantalla("boton 2",0,0,true,true,true);
      botonPresionado = 2;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P2) == HIGH){
      //pantalla("boton 3",0,0,true,true,true);
      botonPresionado = 3;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P3) == HIGH){
      //pantalla("boton 4",0,0,true,true,true);
      botonPresionado = 4;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P4) == HIGH){
      //pantalla("boton 5",0,0,true,true,true);
      botonPresionado = 5;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P5) == HIGH){
      //pantalla("boton 6",0,0,true,true,true);
      botonPresionado = 6;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P6) == HIGH){
      //pantalla("boton 7",0,0,true,true,true);
      botonPresionado = 7;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P7) == HIGH){
      //pantalla("boton 8",0,0,true,true,true);
      botonPresionado = 8;
      delay(100);
      return botonPresionado;
  }else if(digitalRead(B9) == HIGH){
      //pantalla("boton 9",0,0,true,true,true);
      botonPresionado = 9;
      delay(100);
      return botonPresionado;
  }else if(digitalRead(B10) == HIGH){
      //pantalla("boton 10",0,0,true,true,true);
      botonPresionado = 10;
      delay(100);
      return botonPresionado;
  }

  return 20;
}


int readConfirmarCancelar(){
 
  int botonPresionado = 0;
  
  if(boton.digitalRead(P0) == HIGH){
      //pantalla("boton 1",0,0,true,true,true);
      botonPresionado = 1;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P1) == HIGH){
      //pantalla("boton 2",0,0,true,true,true);
      botonPresionado = 2;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P2) == HIGH){
      //pantalla("boton 3",0,0,true,true,true);
      botonPresionado = 3;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P3) == HIGH){
      //pantalla("boton 4",0,0,true,true,true);
      botonPresionado = 4;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P4) == HIGH){
      //pantalla("boton 5",0,0,true,true,true);
      botonPresionado = 5;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P5) == HIGH){
      //pantalla("boton 6",0,0,true,true,true);
      botonPresionado = 6;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P6) == HIGH){
      //pantalla("boton 7",0,0,true,true,true);
      botonPresionado = 7;
      delay(100);
      return botonPresionado;
  }else if(boton.digitalRead(P7) == HIGH){
      //pantalla("boton 8",0,0,true,true,true);
      botonPresionado = 8;
      delay(100);
      return botonPresionado;
  }else if(digitalRead(B9) == HIGH){
      //pantalla("boton 9",0,0,true,true,true);
      botonPresionado = 9;
      delay(100);
      return botonPresionado;
  }else if(digitalRead(B10) == HIGH){
      //pantalla("boton 10",0,0,true,true,true);
      botonPresionado = 10;
      delay(100);
      return botonPresionado;
  }else if(digitalRead(BCancelar) == HIGH){
      //pantalla("boton 10",0,0,true,true,true);
      botonPresionado = 11;
      delay(100);
      return botonPresionado;
  }

  return 20;
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
