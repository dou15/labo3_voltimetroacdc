/***************************************************************************
            Importar bibliotecas
***************************************************************************/
#include <PCD8544.h>

/***************************************************************************
            definiciones - variables
***************************************************************************/
//Puerto activar comunicación serial
//Swhitch selector ac/dc medicion
#define SW_serial 2
int SW_serial_READ;

//Define signo tensión en DC
#define SIGNO_DC 8
int SIGNO_DC_READ;

//Swhitch selector ac/dc medicion
#define SW_ACDC 9
int SW_ACDC_READ;

//Salidas utilizadas como selector de mux, para determinar si la tensión es 
//positiva o negativa
#define SELECT_CHSIGNO4 A4
#define SELECT_CHSIGNO5 A5

//Establece leds warning tension > 20V
#define WARNING_CH0 10
#define WARNING_CH1 11
#define WARNING_CH2 12
#define WARNING_CH3 13

//Lectura puertos analogicos canales CH0 CH1 CH2 CH3
int readCH0;
int readCH1;
int readCH2;
int readCH3;
// calculo tensiones canales
float voltajeCH0;
float voltajeCH1;
float voltajeCH2;
float voltajeCH3;

/***************************************************************************
            Display PCD8544 definiciones - variables
***************************************************************************/

//#define switchDisplay 8 // Switch encendido apagado pantalla
PCD8544 lcd;
// The dimensions of the LCD (in pixels)...
static const byte LCD_WIDTH = 84;
static const byte LCD_HEIGHT = 48;
// A custom "degrees" symbol...
static const byte DEGREES_CHAR = 1;
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

/***************************************************************************
            Display PCD8544 funciones
***************************************************************************/

// Despliega datos en pantalla
void displaylcd(){ 
    lcd.clear();
      lcd.setPower(1);  
      // Imprime tensión canal 0
      lcd.setCursor(0, 0);
      lcd.print("CH0 ");
      lcd.print(voltajeCH0);
      // Imprime tensión canal 1
      lcd.setCursor(0, 1);
      lcd.print("CH1 ");
      lcd.print(voltajeCH1);
      // Imprime tensión canal 2
      lcd.setCursor(0, 2);
      lcd.print("CH2 ");
      lcd.print(voltajeCH2);
      // Imprime tensión canal 3
      lcd.setCursor(0, 3);
      lcd.print("CH3 ");
      lcd.print(voltajeCH3);        
    //delay(1000);
} 

/***************************************************************************
            Calculo tensión 
***************************************************************************/

//Toma las  lecturas de los canales voltage y activa los leds de prevención
void tension(){
  //lee si es DC o AC
  SW_ACDC_READ = digitalRead(SW_ACDC);
  //Configuración DC
  if(SW_ACDC_READ==HIGH){
    //lee tensiones en los canales
    readCH0 = analogRead(A0);
    readCH1 = analogRead(A1);
    readCH2 = analogRead(A2);
    readCH3 = analogRead(A3);

    //determina signo canal 0  
    digitalWrite(SELECT_CHSIGNO4, LOW);
    digitalWrite(SELECT_CHSIGNO5, LOW);
    //delay(500);
    SIGNO_DC_READ = digitalRead(SIGNO_DC);
    //tension canal 0    
    voltajeCH0 = readCH0 * (5.0 / 1023.0);
    voltajeCH0 = voltajeCH0 * 100 + 0.7;
    digitalWrite(WARNING_CH0, LOW); 
    if(voltajeCH0>20){
      digitalWrite(WARNING_CH0, HIGH);
    }
    if(SIGNO_DC_READ>0){
    voltajeCH0 = -voltajeCH0;
    }

    //determina signo canal 1  
    digitalWrite(SELECT_CHSIGNO4, HIGH);
    digitalWrite(SELECT_CHSIGNO5, LOW);
    //delay(500);
    SIGNO_DC_READ = digitalRead(SIGNO_DC);
    //tension canal 1    
    voltajeCH1 = readCH1 * (5.0 / 1023.0);
    voltajeCH1 = voltajeCH1 * 100 + 0.7;
    digitalWrite(WARNING_CH1, LOW); 
    if(voltajeCH1>20){
      digitalWrite(WARNING_CH1, HIGH);
    }
    if(SIGNO_DC_READ>0){
    voltajeCH1 = -voltajeCH1;
    }

    //determina signo canal 2  
    digitalWrite(SELECT_CHSIGNO4, LOW);
    digitalWrite(SELECT_CHSIGNO5, HIGH);
    //delay(500);
    SIGNO_DC_READ = digitalRead(SIGNO_DC);
    //tension canal 2    
    voltajeCH2 = readCH2 * (5.0 / 1023.0);
    voltajeCH2 = voltajeCH2 * 100 + 0.7;
    digitalWrite(WARNING_CH2, LOW); 
    if(voltajeCH2>20){
      digitalWrite(WARNING_CH2, HIGH);
    }
    if(SIGNO_DC_READ>0){
    voltajeCH2 = -voltajeCH2;
    }

    //determina signo canal 3  
    digitalWrite(SELECT_CHSIGNO4, HIGH);
    digitalWrite(SELECT_CHSIGNO5, HIGH);
    //delay(500);
    SIGNO_DC_READ = digitalRead(SIGNO_DC);
    //tension canal 3    
    voltajeCH3 = readCH3 * (5.0 / 1023.0);
    voltajeCH3 = voltajeCH3 * 100 + 0.7;
    digitalWrite(WARNING_CH3, LOW); 
    if(voltajeCH3>20){
      digitalWrite(WARNING_CH3, HIGH);
    }
    if(SIGNO_DC_READ>0){
    voltajeCH3 = -voltajeCH3;
    }

  }
  //Caso AC
  else{
    //lee tensiones en los canales
    readCH0 = analogRead(A0);
    readCH1 = analogRead(A1);
    readCH2 = analogRead(A2);
    readCH3 = analogRead(A3);

    //tension canal 0    
    voltajeCH0 = readCH0 * (5.0 / 1023.0);
    voltajeCH0 = (voltajeCH0 + 0.07) * 100;
    voltajeCH0 = voltajeCH0 / sqrt(2);
    digitalWrite(WARNING_CH0, LOW); 
    if(voltajeCH0>20){
      digitalWrite(WARNING_CH0, HIGH);
    }

    //tension canal 1    
    voltajeCH1 = readCH1 * (5.0 / 1023.0);
    voltajeCH1 = (voltajeCH1 + 0.07) * 100;
    voltajeCH1 = voltajeCH1 / sqrt(2);
    digitalWrite(WARNING_CH1, LOW); 
    if(voltajeCH1>20){
      digitalWrite(WARNING_CH1, HIGH);
    }

    //tension canal 2    
    voltajeCH2 = readCH2 * (5.0 / 1023.0);
    voltajeCH2 = (voltajeCH2 + 0.07) * 100;
    voltajeCH2 = voltajeCH2 / sqrt(2);
    digitalWrite(WARNING_CH2, LOW); 
    if(voltajeCH2>20){
      digitalWrite(WARNING_CH2, HIGH);
    }

    //tension canal 3    
    voltajeCH3 = readCH3 * (5.0 / 1023.0);
    voltajeCH3 = (voltajeCH3 + 0.07) * 100;
    voltajeCH3 = voltajeCH3 / sqrt(2);
    digitalWrite(WARNING_CH3, LOW); 
    if(voltajeCH3>20){
      digitalWrite(WARNING_CH3, HIGH);
    }

  }
}

void comunicacionSerial(){
  SW_serial_READ = digitalRead(SW_serial);
  if(SW_serial==LOW){

    /*Serial.print("CH0: ");
    Serial.print(voltajeCH0); 
    Serial.print("-");
    Serial.print("CH1: ");
    Serial.print(voltajeCH1); 
    Serial.print("-"); 
    Serial.print("CH2: ");
    Serial.print(voltajeCH2); 
    Serial.print("-"); 
    Serial.print("CH3: ");
    Serial.println(voltajeCH3);*/

    Serial.println("CH0: " + String(voltajeCH0) + " ,CH1: " + String(voltajeCH1) + " ,CH2: " + String(voltajeCH2) + " ,CH3: " + String(voltajeCH3)); 
  }
  delay(500);
}

/***************************************************************************
            SETUP
***************************************************************************/
void setup(void){
  Serial.begin(9600);
  // Configura pantalla   
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);  
  // Register the custom symbol...
  lcd.createChar(DEGREES_CHAR, degrees_glyph);
  lcd.clear();

  pinMode(SW_serial, INPUT);

  pinMode(SELECT_CHSIGNO4, OUTPUT);
  pinMode(SELECT_CHSIGNO5, OUTPUT);  
  pinMode(SIGNO_DC, INPUT);
  pinMode(SW_ACDC, INPUT);
  
  pinMode(WARNING_CH0, OUTPUT);
  pinMode(WARNING_CH1, OUTPUT);
  pinMode(WARNING_CH2, OUTPUT);
  pinMode(WARNING_CH3, OUTPUT); 

}

/***************************************************************************
            LOOP
***************************************************************************/
void loop(void){

  tension();

  displaylcd();
 
  comunicacionSerial();
  
  delay(1000);  // 1 segundo en milisegundos   
}
