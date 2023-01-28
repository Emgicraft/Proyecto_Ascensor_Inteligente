/*Programa para compilar al ESP32 Dev Module

  Código escrito y modificado por: MAGH

  Conexiones para Display LCD:
  PIN 21 = SDA
  PIN 22 = SCL

*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void resetear_LCD();
int piso_destino(); int piso_actual();
void display_LCD(int actual, int destino);
void puerta();

const int LCD_reset = 2;
const int motor_baja = 27; const int motor_sube = 14;
const int puerta_abre = 26; const int puerta_cierra = 25;
const int fc_puerta_estado = 4;int fc_abierto = 0; int abrir = 0;int fc_cerrado = 0; int cerrar = 0;
const int senso_piso = 32; int p = 0;
int cnx_establecida = 0;
int rst_LCD; int sLCD = 0;
int puertaactual=0;
int pisoactual = 0; int pisodestino = 0; int piso = 0; int subiendo = 0; int bajando = 0;
uint8_t downarrow[8] = {0x0A, 0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04};
uint8_t uparrow[8] = {0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x04, 0x0A};
byte dato; int seguro= 0;
int temporizador_1 = 0; int temporizador_2 = 0; int temporizador_3 = 0; int temporizador_4 = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);
BluetoothSerial ESP_BT;

void setup()
{
  Serial.begin(115200); //Inicia el monitor serial
  ESP_BT.begin("ESP32_MAGH"); //Nombre para el dispositivo Bluetooth y lo inicia
  lcd.begin();  //Inicia el LCD
  lcd.backlight();
  lcd.createChar(0, downarrow); lcd.createChar(1, uparrow); delay(10);
  lcd.home();
  Serial.println("Dispositivo iniciado, listo para vicular.");
  pinMode(motor_sube, OUTPUT); pinMode(motor_baja, OUTPUT);
  pinMode(puerta_abre, OUTPUT); pinMode(puerta_cierra, OUTPUT); delay(10);
  pinMode(LCD_reset, INPUT);
  pinMode(senso_piso, INPUT);
  pinMode(fc_puerta_estado, INPUT); //pinMode(fc_puerta_cerrada, INPUT);
  digitalWrite(motor_sube, HIGH); digitalWrite(motor_baja, HIGH);
  digitalWrite(puerta_abre, HIGH); digitalWrite(puerta_cierra, HIGH);
  temporizador_1 = millis(); temporizador_2 = millis();
  temporizador_3 = millis(); temporizador_4 = millis();
  //La cantidad de temporizadores es para evitar ciertas complicaciones en la práctica, como un excesivo refresh del display LCD.
}

void loop()
{
  //Resetea el LCD
  resetear_LCD();
  //Detecta si recibio alguna señal por Bluetooth y si es así, hace su lógica.
  if (millis() >= temporizador_1 + 5000) //Cada 5 segundo lo tomará como una desconexión automática por inactividad y mostrará de nuevo los mensajes.
  {
    temporizador_1 = millis();
    cnx_establecida = 0;
  }
  while (!ESP_BT.available() && cnx_establecida == 0 && rst_LCD == 1)
  {
    rst_LCD = digitalRead(LCD_reset);
    lcd.setCursor(1, 1);
    lcd.print("Esperando conexion");
    lcd.setCursor(1, 2);
    lcd.print("Bluetooth...");
    if (millis() >= temporizador_2 + 5000)
    {
      temporizador_2 = millis();
      lcd.clear();
    }
  }
  if (ESP_BT.available())
  {
    if (cnx_establecida == 0)
    {
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("Conexion");
      lcd.setCursor(5, 2);
      lcd.print("establecida...");
      delay(500);
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("Dispositivo");
      lcd.setCursor(7, 2);
      lcd.print("conectado");
      delay(1000);
      lcd.clear();
      cnx_establecida = 1;
    }
    delay(10);
    piso_destino(); piso_actual();
    piso = pisoactual - pisodestino;
    if (piso <= 0)
    {
      piso = -piso; delay(10); Serial.print("Piso = "); Serial.println(piso);
      puerta(); subiendo = 1;
      while (piso > 0)
      {
        digitalWrite(motor_sube, LOW);
        if (millis() >= temporizador_3 + 400)
        {
          temporizador_3 = millis();
          piso_actual(); piso = pisodestino - pisoactual;
        }
        if (millis() >= temporizador_4 + 400)
        {
          temporizador_4 = millis();
          display_LCD(pisoactual, pisodestino);
          Serial.print("Sube - Piso actual = "); Serial.println(pisoactual);
        }
      }
      digitalWrite(motor_sube, HIGH); delay(10);
      subiendo = 0; puerta();
    }
    if (piso > 0)
    {
      delay(10);
      puerta(); bajando = 1;
      while (piso > 0)
      {
        digitalWrite(motor_baja, LOW);
        if (millis() >= temporizador_3 + 400)
        {
          temporizador_3 = millis();
          piso_actual(); piso = pisoactual - pisodestino;
        }
        if (millis() >= temporizador_4 + 1000)
        {
          temporizador_4 = millis();
          display_LCD(pisoactual, pisodestino);
          Serial.print("Baja - Piso actual = "); Serial.println(pisoactual);
        }
      }
      digitalWrite(motor_baja, HIGH); delay(10);
      bajando = 0; puerta();
    }
  }
  display_LCD(pisoactual, pisodestino); delay(1000);
}

void resetear_LCD()
{
  //Resetea el LCD
  rst_LCD = digitalRead(LCD_reset);
  if (rst_LCD == 0 && sLCD == 1)
  {
    lcd.clear();
    sLCD = 0;
  }
  if (rst_LCD == 1 && sLCD == 0)
  {
    lcd.begin(); lcd.backlight();
    lcd.setCursor(1, 1); lcd.print("SENATI-EST-uC-Deza");
    lcd.setCursor(0, 2); lcd.print("Ascensor Inteligente");
    sLCD = 1;
    delay(2000);
    lcd.clear();
  }
}

int piso_destino()
{
  dato = ESP_BT.read(); Serial.print("dato = "); Serial.println(dato);
  switch (dato)
  {
    case 1: pisodestino = 1;
      break;
    case 2: pisodestino = 2;
      break;
    case 3: pisodestino = 3;
      break;
    case 4: pisodestino = 4;
      break;
  }
  return pisodestino;
}

int piso_actual()
{
  p = digitalRead(senso_piso); Serial.print("senso_piso = "); Serial.println(p);
  if (p == 0 && subiendo == 1 && seguro == 0)
  {
    pisoactual = pisoactual + 1; Serial.print("Piso actual + = "); Serial.println(pisoactual);
    seguro = 1;
  }
  if (p == 0 && bajando == 1 && seguro == 0)
  {
    pisoactual = pisoactual - 1; Serial.print("Piso actual - = "); Serial.println(pisoactual);
    seguro = 1;
  }
  if (p == 1 && seguro == 1)
  {
    seguro = 0;
  }
  return pisoactual;
}

void display_LCD(int actual, int destino)
{
  lcd.clear(); lcd.setCursor(0, 0);
  lcd.print("Ascensor Inteligente");
  if (subiendo == 1)
  {
    lcd.setCursor(4, 1); delay(5);
    lcd.write(0);
    lcd.print(" Subiendo "); delay(5);
    lcd.write(0);
  }
  if (bajando == 1)
  {
    lcd.setCursor(4, 1); delay(5);
    lcd.write(1);
    lcd.print(" Bajando "); delay(5);
    lcd.write(1);
  }
  lcd.setCursor(7, 3);
  lcd.print("Piso ");
  lcd.print(actual); Serial.print("dLCD - Piso actual = "); Serial.println(actual);
}

void puerta()
{

  fc_abierto = digitalRead(fc_puerta_estado);
  
  
  Serial.print("fc_puerta_abierta = "); Serial.println(fc_abierto);
  fc_cerrado = digitalRead(fc_puerta_cerrada);
  Serial.print("fc_puerta_cerrada = "); Serial.println(fc_cerrado); delay(10);
  
  if (fc_cerrado == 0 && (subiendo == 0 || bajando == 0))
  {
    lcd.clear(); lcd.setCursor(2, 1); lcd.print("Abriendo");
    lcd.setCursor(12, 1); lcd.print("puerta");
    while (fc_abierto = 1)
    {
      digitalWrite(puerta_abre, LOW); fc_abierto = digitalRead(fc_puerta_abierta);
    }
    digitalWrite(puerta_abre, HIGH);
    delay(10); lcd.print("Puerta abierta"); delay(1000);
  }
  if (fc_abierto == 0 && (subiendo == 0 || bajando == 0))
  {
    lcd.clear(); lcd.setCursor(2, 1); lcd.print("Cerrando");
    lcd.setCursor(12, 2); lcd.print("puerta");
    while (fc_cerrado = 1)
    {
      digitalWrite(puerta_cierra, LOW); fc_cerrado = digitalRead(fc_puerta_cerrada);
    }
    digitalWrite(puerta_cierra, HIGH);
    delay(10); lcd.print("Puerta cerrada"); delay(1000);
  }
}
