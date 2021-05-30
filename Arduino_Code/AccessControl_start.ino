
#include <SPI.h>
#include <MFRC522.h> 
#include "AccessControl.h"


///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  initialization();
  if (!isMasterDefined()) {
    Serial.println(F("No Master Card Defined"));
    Serial.println(F("Scan A PICC to Define as Master Card"));
    defineMaster();
  }
  Serial.println(F("-------------------"));
  Serial.println(F("Master Card's UID"));
  masterToSerial();
  
  Serial.println("");
  Serial.println(F("-------------------"));
  Serial.println(F("Everything is ready"));
  Serial.println(F("Waiting PICCs to be scanned"));
  cycleLeds();                                                  // Everything ready lets give user some feedback by cycling leds
}

///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  do {
    request_ans();
    successRead = getID();                                       // sets successRead to 1 when we get read from reader otherwise 0
    if (programMode) {
      cycleLeds();                                               // Program Mode cycles through Red Green Blue waiting to read a new card
    }
    else {
      normalModeOn();                                            // Normal mode, blue Power LED is on, all others are off
    }
  }
  while (!successRead);                                          //the program will not go further while you are not getting a successful read
  if (programMode) {
    if ( isMaster(readCard) ) {                                  //When in program mode check First If master card scanned again to exit program mode
      Serial.println(F("Master Card Scanned"));
      Serial.println(F("Exiting Program Mode"));
      Serial.println(F("-----------------------------"));
      programMode = false;
      Log log(masterCard, 5);
      log.writeLog();
      return;
    }
    else {
      if ( findID(readCard) ) {                                 // If scanned card is known delete it
        Serial.println(F("I know this PICC, removing..."));
        deleteID(readCard);
        Serial.println("-----------------------------");
        Serial.println(F("Scan a PICC to ADD or REMOVE to EEPROM"));
        Log log(readCard, 3);
        log.writeLog();
      }
      else {                    // If scanned card is not known add it
        Serial.println(F("I do not know this PICC, adding..."));
        writeID(readCard);
        Serial.println(F("-----------------------------"));
        Serial.println(F("Scan a PICC to ADD or REMOVE to EEPROM"));
        Log log(readCard, 2);
        log.writeLog();
      }
    }
  }
  else {
    if ( isMaster(readCard)) {    // If scanned card's ID matches Master Card's ID - enter program mode
      programMode = true;
      Log log(masterCard, 4);
      log.writeLog();
      Serial.println(F("Hello Master - Entered Program Mode"));
      uint8_t count = numberOfIDs();   // Read the first Byte of EEPROM that
      Serial.print(F("I have "));     // stores the number of ID's in EEPROM
      Serial.print(count);
      Serial.println(F(" record(s) on EEPROM"));
      Serial.println(F("Scan a PICC to ADD or REMOVE to EEPROM"));
      Serial.println(F("Scan Master Card again to Exit Program Mode"));
      Serial.println(F("-----------------------------"));
    }
    else {
      if ( findID(readCard) ) { // If not, see if the card is in the EEPROM
        Serial.println(F("Welcome, You shall pass"));
        granted(300);         // Open the door lock for 300 ms
        Log log(readCard, 0);
        log.writeLog();
      }
      else {      // If not, show that the ID was not valid
        Serial.println(F("You shall not pass"));
        denied();
        Log log(readCard, 1);
        log.writeLog();
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////
