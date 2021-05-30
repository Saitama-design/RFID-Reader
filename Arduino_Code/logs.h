#ifndef LOGS_H

#define LOGS_H
#define LOG_BEGIN 262
#define LOG_SIZE 5
#include<Arduino.h>
#include <EEPROM.h>


struct Log {
    byte card[4];
    byte actionFlag = 0;

    Log(byte* curCard, byte flag);

    Log(uint16_t pos);

    void printFlag();

    void outToSerialPort();

    void writeLog(uint16_t pos);

    void writeLog();

} typedef Log;

void printLogs();

void cleanEEPROM();

void request_ans();

#endif 
