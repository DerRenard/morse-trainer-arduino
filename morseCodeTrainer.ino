/*****************************************
********Copyrighted by Yan Sivak**********
*************aka DerRenard****************
**************@ 8.26.2025*****************
*******https://github.com/DerRenard*******
******************************************/

// !Экран с русскмии символами!
// https://wiki.iarduino.ru/page/OLED_trema/
// https://iarduino.ru/file/341.html
#include <StringUtils.h>
#include <Wire.h>
#include <iarduino_OLED_txt.h>
// Морзянка в библиотеке лежит vvvvv
#include "codes.h"
#include "tools.h"

#define BUTTON_DOT 5
#define BUTTON_DASH 2
#define BUTTON_CONFIRM 3
#define BUTTON_CLEAR 4
#define BUTTON_LAYOUT 6
#define BUTTON_MODE 7
#define LED_OUTPUT 8
#define BUZZER_OUTPUT 9
// Analogue pin! vvvvvv
#define SCREEN_RESET 4

#define WIRE Wire

iarduino_OLED_txt display(0x3C);

String printedCode = "";
String sentanceInMorse = "";

String printedMorseCode = "";

String layout = "RU";  // "EN"

bool ledState = true;
bool buzzState = true;

int mode = 3;
String modes[4] = { "OFF", "BZ", "LD", "BZ_LD" };

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(BUTTON_DOT, INPUT_PULLUP);
  pinMode(BUTTON_DASH, INPUT_PULLUP);
  pinMode(BUTTON_CONFIRM, INPUT_PULLUP);
  pinMode(BUTTON_CLEAR, INPUT_PULLUP);
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_LAYOUT, INPUT_PULLUP);

  pinMode(LED_OUTPUT, OUTPUT);
  pinMode(BUZZER_OUTPUT, OUTPUT);

  digitalWrite(LED_OUTPUT, 0);

  // Сделать рабочий вариант с переключением режимов

  display.begin(&WIRE);
  display.clrScr();
  display.setFont(MediumFontRus);  //SmallFontRus
  display.setCoding(TXT_UTF8);
  display.print(modes[mode] + " " + layout, OLED_C, 1);
}


// ====== * Для раскладки * ======
bool lastLayoutButtonState = HIGH;
unsigned long lastLayoutDebounceTime = 0;
const unsigned long layoutDebounceDelay = 50;
bool layoutButtonPressed = false;
// ====== * Для раскладки * ======

void loop() {
  bool dotButton = !digitalRead(BUTTON_DOT);
  bool dashButton = !digitalRead(BUTTON_DASH);
  bool modeButton = !digitalRead(BUTTON_MODE);
  bool confirmButton = !digitalRead(BUTTON_CONFIRM);
  bool clearButton = !digitalRead(BUTTON_CLEAR);
  bool layoutButton = !digitalRead(BUTTON_LAYOUT);

  if (dotButton) {
    // ====== * Обработка точек * ======
    Serial.println(".");

    if (buzzState && ledState) {
      tone(BUZZER_OUTPUT, 5000);

      digitalWrite(LED_OUTPUT, 1);
      printedCode += "0";

      printedMorseCode += ".";
      display.print(processString(printedMorseCode), OLED_C, 3);

      delay(100);
      noTone(BUZZER_OUTPUT);

      digitalWrite(LED_OUTPUT, 0);
    } else if (buzzState) {
      tone(BUZZER_OUTPUT, 5000);

      printedCode += "0";

      printedMorseCode += ".";
      display.print(processString(printedMorseCode), OLED_C, 3);

      delay(100);
      noTone(BUZZER_OUTPUT);
    } else if (ledState) {
      digitalWrite(LED_OUTPUT, 1);
      printedCode += "0";

      printedMorseCode += ".";
      display.print(processString(printedMorseCode), OLED_C, 3);

      delay(100);
      digitalWrite(LED_OUTPUT, 0);
    } else {
      printedCode += "0";
      display.print(processString(printedMorseCode), OLED_C, 3);

      printedMorseCode += ".";
    }
    delay(100);
  }

  if (dashButton) {
    // ====== * Обработка Тире * ======
    Serial.println("_");

    if (buzzState && ledState) {
      tone(BUZZER_OUTPUT, 3500);

      digitalWrite(LED_OUTPUT, 1);
      printedCode += "1";

      printedMorseCode += "_";
      display.print(processString(printedMorseCode), OLED_C, 3);

      delay(300);
      noTone(BUZZER_OUTPUT);

      digitalWrite(LED_OUTPUT, 0);
    } else if (buzzState) {
      tone(BUZZER_OUTPUT, 3500);

      printedCode += "1";

      printedMorseCode += "_";
      display.print(processString(printedMorseCode), OLED_C, 3);

      delay(300);
      noTone(BUZZER_OUTPUT);
    } else if (ledState) {
      digitalWrite(LED_OUTPUT, 1);
      printedCode += "1";

      printedMorseCode += "_";
      display.print(processString(printedMorseCode), OLED_C, 3);

      delay(300);
      digitalWrite(LED_OUTPUT, 0);
    } else {
      printedCode += "1";

      printedMorseCode += "_";
      display.print(processString(printedMorseCode), OLED_C, 3);
    }
    delay(100);
  }

  if (confirmButton) {
    // ====== * Сбор в предложение и очистка буфера * ======
    Serial.println("confirmButton!");
    Serial.println(printedCode);
    if (printedCode == "") {
      sentanceInMorse += "/";
      printedMorseCode += " ";
    } else if (findSymbolByCode(printedCode, layout) == '?') {
      // Serial.println(findSymbolByCode(printedCode, layout));
      printedMorseCode += " ";
      Serial.println("Not in the sentance!");
    } else {
      sentanceInMorse += findSymbolByCode(printedCode, layout);
      printedMorseCode += " ";
      Serial.println("In the sentance!");
    }

    display.print(processString(sentanceInMorse, layout), OLED_C, 6);

    Serial.println(sentanceInMorse);

    printedCode = "";

    digitalWrite(LED_OUTPUT, 0);
    delay(1000);
  }

  if (clearButton) {
    // ====== * Очистка всего * ======
    Serial.println("cleared!");

    printedCode = "";
    sentanceInMorse = "";
    printedMorseCode = "";

    display.clrScr();
    display.print("Cleared!", OLED_C, 4);
    delay(1000);
    display.clrScr();
    display.print(modes[mode] + " " + layout, OLED_C, 1);

    digitalWrite(LED_OUTPUT, 0);
    delay(100);
  }

  // if (modeButton) {
  //   if (mode < 4) {
  //     mode++;
  //     modes[mode];
  //   } else {
  //     mode = 0;
  //     modes[mode];
  //   }
  // }

  // ====== * ОБРАБОТКА КНОПКИ РАСКЛАДКИ - ИСПРАВЛЕННАЯ * ======
  bool currentLayoutButton = !digitalRead(BUTTON_LAYOUT);

  // Обнаружение нажатия
  if (currentLayoutButton != lastLayoutButtonState) {
    lastLayoutDebounceTime = millis();
  }

  if ((millis() - lastLayoutDebounceTime) > layoutDebounceDelay) {
    // Если кнопка нажата и ранее не была обработана
    if (currentLayoutButton && !layoutButtonPressed) {
      layoutButtonPressed = true;

      // Меняем раскладку
      if (layout == "EN") {
        layout = "RU";
        printedMorseCode = "";
        sentanceInMorse = "";

        display.clrScr();

        display.print("Chng to " + layout, OLED_C, 4);
        delay(1000);

        display.clrScr();

        display.print(modes[mode] + " " + layout, OLED_C, 1);
        Serial.println("changed layout ru");
      } else {
        layout = "EN";
        printedMorseCode = "";
        sentanceInMorse = "";

        display.clrScr();

        display.print("Chng to " + layout, OLED_C, 4);
        delay(1000);

        display.clrScr();
        display.print(modes[mode] + " " + layout, OLED_C, 1);
        Serial.println("changed layout en");
      }
    }

    // Если кнопка отпущена - сбрасываем флаг
    if (!currentLayoutButton) {
      layoutButtonPressed = false;
    }
  }

  lastLayoutButtonState = currentLayoutButton;
  // ====== * КОНЕЦ ОБРАБОТКИ КНОПКИ * ======
}
