#include "logs.h"

#define MAGICAL_NUMBER 143                  // EEPROM address 1 should hold magical number which is '143'

Log::Log(byte* curCard, byte flag) {

    for (uint8_t i = 0; i < 4; i++) {

        card[i] = curCard[i];

    }

    actionFlag = flag;
}

Log::Log(uint16_t pos) {

    for (uint8_t i = 0; i < 4; i++) {

        card[i] = EEPROM.read(pos + i);

    }

    actionFlag = EEPROM.read(pos + 4);
}

void Log::printFlag() {
    switch (actionFlag) {

    case 0:
        Serial.print("Access granted");
        break;

    case 1:
        Serial.print("Access denied");
        break;

    case 2:
        Serial.print("Add card");
        break;

    case 3:
        Serial.print("Delete card");
        break;

    case 4:
        Serial.print("Entry program mode");
        break;

    case 5:
        Serial.print("Exiting program mode");
        break;

    case 6:
        Serial.print("Add mater card");
        break;
    }
}

void Log::outToSerialPort(){
    //Serial.print(curTime);
    //Serial.print(" ");
    for (uint8_t i = 0; i < 4; i++){
      char strbuf[2];
      sprintf(strbuf, "%02x", card[i]);
      //Serial.print(card[i]);
    }
    Serial.print(actionFlag);
    //printFlag();
    Serial.println();
}

void Log::writeLog(uint16_t pos) {

    for (int i = 0; i < 4; i++) {

        EEPROM.write(pos + i, card[i]);

    }

    EEPROM.write(pos + 4, actionFlag);
}

void Log::writeLog() {
    uint16_t i;
    for (i = logBegin; i < 1024 && EEPROM.read(i) != 143; i += 5);
    writeLog(i);
    if (i == 1017){
      EEPROM.write(logBegin, 143);
    }
    else{
      EEPROM.write(i + 5, 143);
    }
    return;
}

void printLogs(){
  Serial.println("Log is begin");
  bool flag = true;
  uint16_t i;
  if (EEPROM.read(logBegin) == 143){
    Serial.println("Log is out");
    return;
  }
  for (i = logBegin; i < 1024 && EEPROM.read(i) != 143; i += 5);
  if (EEPROM.read(i + 1) == 0){
    i = logBegin;
    flag = false;
  }
  for (i = i  + 5; i < 1000 && EEPROM.read(i) != 143; i += 5){
    Log logg(i);
    logg.outToSerialPort();
  }
  if (flag){
    for (uint16_t j = logBegin; EEPROM.read(j) != 143; j += 5){
      Log logg(j);
      logg.outToSerialPort(); 
    }
  }
  Serial.println("Log is out");
}

void cleanEEPROM(){
  for (uint16_t i = LOG_BEGIN; i < 1024; i++){
    EEPROM.write(i, 0);
  }
}

void request_ans(){
  if (Serial.available() > 0){
    byte request = Serial.read();
    if (request == '1') {
      printLogs();
    }
  }
}
