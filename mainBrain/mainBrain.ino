#include <DS_raw.h>
#include <microDS18B20.h>
#include <microOneWire.h>

#include <TDA7448.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EncButton.h>


TDA7448 tda; //ТДА
const String buttonPin = "A0";     // Номер куда подключена кнопка
const int ledPin =  13;      // Номер выхода куда подключено питание
const int transistorBaklightPin =  10;      // Номер выхода куда подключено питание
const int rs = 12, en = 11, d4 = 8, d5 = 7, d6 = 6, d7 = 5; // номера куда подключен дисплей
int buttonState = 0;         // variable for reading the pushbutton status
bool powerUp = false;          //Включение питания
int volume = -60;           //Громкость по-умолчанию в dB
MicroDS18B20<2> sensor1;

EncButton<EB_TICK, A3, A2, A1> enc; //sw, DT, CLK
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Wire.begin();
  pinMode(ledPin, OUTPUT); //Включение питания
  pinMode(transistorBaklightPin, OUTPUT); //Транзистор подсветки LCD
  pinMode(A0, INPUT); //Кнопка включения
  enc.counter = 100;      // изменение счётчика
  
  lcd.begin(24, 2);
}


uint32_t btnTimer = 0;
void loop() {
  enc.tick();
  buttonState = digitalRead(A0);

  //Убавить громкость
  if (enc.isLeft()) {
    if(volume != -79){
      volume = volume - 1;  

      tda.setAtt(1, volume);
      tda.setAtt(2, volume);
      tda.setAtt(3, volume);
      tda.setAtt(4, volume);
      tda.setAtt(5, volume);
      tda.setAtt(6, volume);
    }
    lcd.clear();
    lcd.setCursor(14, 0);
    lcd.print(String(volume, DEC) + " dB ");
  }


  //Добавить громкость
  if (enc.isRight()) {
    if(volume != 0){
      volume = volume + 1; 
       
      tda.setAtt(1, volume);
      tda.setAtt(2, volume);
      tda.setAtt(3, volume);
      tda.setAtt(4, volume);
      tda.setAtt(5, volume);
      tda.setAtt(6, volume);
    }
    lcd.clear();
    lcd.setCursor(14, 0);
    lcd.print(String(volume, DEC) + " dB ");
  }

  //Нажатие кнопки питания для выключения
  if (buttonState == HIGH && powerUp && millis() - btnTimer > 1000){
    btnTimer = millis();
    powerUp = false;
    lcd.setCursor(5, 1);
    lcd.print("Offline");
    lcd.clear();
  }

  //Нажатие кнопки питания для включения
  if (buttonState == HIGH && !powerUp && millis() - btnTimer > 1000){
    btnTimer = millis();
    powerUp = true;
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("Online - 666");

  }

  sensor1.requestTemp();
    lcd.setCursor(2, 1);
  lcd.print(sensor1.getTemp());
  

  //Включение схемы
  if (powerUp) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(transistorBaklightPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(transistorBaklightPin, LOW);
  }
}
