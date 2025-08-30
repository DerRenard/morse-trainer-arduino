#pragma once;

// ====== * Преобразование кода в символы * ======
const int morseCodesCount = sizeof(morseCodesLatin) / sizeof(morseCodesLatin[0]);
const int morseCodesCyrillicCount = sizeof(morseCodesCyrillic) / sizeof(morseCodesCyrillic[0]);

String findSymbolByCode(String code, String layout = "RU") {
  if (layout == "EN") {
    for (int i = 0; i < morseCodesCount; i++) {
      if (code.equals(morseCodesLatin[i].code)) {
        // Для латинских символов - просто возвращаем символ
        return String((char)morseCodesLatin[i].byte1);
      }
    }
  } else if (layout == "RU") {
    for (int i = 0; i < morseCodesCyrillicCount; i++) {
      if (code.equals(morseCodesCyrillic[i].code)) {
        // Для кириллицы - собираем UTF-8 символ из двух байт
        char utf8Symbol[3] = {(char)morseCodesCyrillic[i].byte1, 
                              (char)morseCodesCyrillic[i].byte2, 
                              '\0'};
        return String(utf8Symbol);
      }
    }
  }
  return "?";
}
// ====== * Преобразование кода в символы * ======

// ====== * Обрезание строки до 10зн * ======
String processString(String newString, String layout = "EN") {
     String thisString = newString;
    if (layout == "RU"){
        if (thisString.length() >= 20) {
          thisString = thisString.substring(thisString.length() - 10);
          return thisString;
      } else {
        return thisString;
      }
    } else if (layout == "EN"){
      if (thisString.length() >= 10) {
        thisString = thisString.substring(thisString.length() - 10);
        return thisString;
      } else {
        return thisString;
      }
    } 
}
// ====== * Обрезание строки до 10зн * ======