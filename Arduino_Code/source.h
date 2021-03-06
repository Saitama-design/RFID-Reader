#ifndef source
#define source



uint8_t numberOfIDs() {
    return EEPROM.read(0);
}


//////////////////////////////////////// Read an ID from EEPROM //////////////////////////////
void readID(uint8_t number) {
    uint8_t start = (number * 4) + 2;    // Figure out starting position
    for (uint8_t i = 0; i < 4; i++) {     // Loop 4 times to get the 4 Bytes
        storedCard[i] = EEPROM.read(start + i);   // Assign values read from EEPROM to array
    }
}


///////////////////////////////////////// Check Bytes   ///////////////////////////////////
bool checkTwo(byte a[], byte b[]) {
    for (uint8_t k = 0; k < 4; k++) {   // Loop 4 times
        if (a[k] != b[k]) {     // IF a != b then false, because: one fails, all fail
            return false;
        }
    }
    return true;
}

///////////////////////////////////////// Find Slot   ///////////////////////////////////
uint8_t findIDSLOT(byte find[]) {
    uint8_t count = numberOfIDs();       // Read the first Byte of EEPROM that
    for (uint8_t i = 1; i <= count; i++) {    // Loop once for each EEPROM entry
        readID(i);                // Read an ID from EEPROM, it is stored in storedCard[4]
        if (checkTwo(find, storedCard)) {   // Check to see if the storedCard read from EEPROM
          // is the same as the find[] ID card passed
            return i;         // The slot number of the card
        }
    }
}

///////////////////////////////////////// Find ID From EEPROM   ///////////////////////////////////
bool findID(byte find[]) {
    uint8_t count = numberOfIDs();     // Read the first Byte of EEPROM that
    for (uint8_t i = 1; i < count; i++) {    // Loop once for each EEPROM entry
        readID(i);          // Read an ID from EEPROM, it is stored in storedCard[4]
        if (checkTwo(find, storedCard)) {   // Check to see if the storedCard read from EEPROM
            return true;
        }
        else {    // If not, return false
        }
    }
    return false;
}


////////////////////// Check readCard IF is masterCard   ///////////////////////////////////
// Check to see if the ID passed is the master programing card
bool isMaster(byte test[]) {
    return checkTwo(test, masterCard);
}

///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
uint8_t getID() {
    // Getting ready for Reading PICCs
    if (!mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return 0;
    }
    if (!mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
        return 0;
    }
    // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
    // I think we should assume every PICC as they have 4 byte UID
    // Until we support 7 byte PICCs
    Serial.println(F("Scanned PICC's UID:"));
    for (uint8_t i = 0; i < 4; i++) {  //
        readCard[i] = mfrc522.uid.uidByte[i];
        Serial.print(readCard[i], HEX);
    }
    Serial.println("");
    mfrc522.PICC_HaltA(); // Stop reading
    return 1;
}

void ShowReaderDetails() {
    // Get the MFRC522 software version
    byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    Serial.print(F("MFRC522 Software Version: 0x"));
    Serial.print(v, HEX);
    if (v == 0x91)
        Serial.print(F(" = v1.0"));
    else if (v == 0x92)
        Serial.print(F(" = v2.0"));
    else
        Serial.print(F(" (unknown),probably a chinese clone?"));
    Serial.println("");
    // When 0x00 or 0xFF is returned, communication probably failed
    if ((v == 0x00) || (v == 0xFF)) {
        Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
        Serial.println(F("SYSTEM HALTED: Check connections."));
        // Visualize system is halted
        digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
        digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
        digitalWrite(redLed, LED_ON);   // Turn on red LED
        while (true); // do not go further
    }
}


///////////////////////////////////////// Add ID to EEPROM   ///////////////////////////////////
void writeID(byte a[]) {
    if (!findID(a)) {     // Before we write to the EEPROM, check to see if we have seen this card before!
        uint8_t num = numberOfIDs();     // Get the numer of used spaces, position 0 stores the number of ID cards
        if (num >= 64) {
            failedWrite();
            Serial.println(F("Failed! Maximum records"));
        }
        uint8_t start = (num * 4) + 6;  // Figure out where the next slot starts
        num++;                // Increment the counter by one
        EEPROM.write(0, num);     // Write the new count to the counter
        for (uint8_t j = 0; j < 4; j++) {   // Loop 4 times
            EEPROM.write(start + j, a[j]);  // Write the array values to EEPROM in the right position
        }
        successWrite();
        Serial.println(F("Succesfully added ID record to EEPROM"));
    }
    else {
        failedWrite();
        Serial.println(F("Failed! There is something wrong with ID or bad EEPROM"));
    }
    delay(200);  //need for test
}

///////////////////////////////////////// Remove ID from EEPROM   ///////////////////////////////////
void deleteID(byte a[]) {
    if (!findID(a)) {     // Before we delete from the EEPROM, check to see if we have this card!
        failedWrite();      // If not
        Serial.println(F("Failed! There is something wrong with ID or bad EEPROM"));
    }
    else {
        uint8_t num = numberOfIDs();   // Get the numer of used spaces, position 0 stores the number of ID cards
        uint8_t slot;       // Figure out the slot number of the card
        uint8_t start;      // = ( num * 4 ) + 6; // Figure out where the next slot starts
        uint8_t looping;    // The number of times the loop repeats
        uint8_t j;
        uint8_t count = numberOfIDs(); // Read the first Byte of EEPROM that stores number of cards
        slot = findIDSLOT(a);   // Figure out the slot number of the card to delete
        start = (slot * 4) + 2;
        looping = ((num - slot) * 4);
        num--;      // Decrement the counter by one
        EEPROM.write(0, num);   // Write the new count to the counter
        for (j = 0; j < looping; j++) {         // Loop the card shift times
            EEPROM.write(start + j, EEPROM.read(start + 4 + j));   // Shift the array values to 4 places earlier in the EEPROM
        }
        for (uint8_t k = 0; k < 4; k++) {         // Shifting loop
            EEPROM.write(start + j + k, 0);
        }
        successDelete();
        Serial.println(F("Succesfully removed ID record from EEPROM"));
    }
    delay(200);
}

////////////////////////
void checkLogs() {
    if (EEPROM.read(LOG_BEGIN) == 0) {
        EEPROM.write(LOG_BEGIN, MAGICAL_NUMBER);
    }
}

void initialization() {
    //Arduino Pin Configuration
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    digitalWrite(redLed, LED_OFF);     // Make sure led is off
    digitalWrite(greenLed, LED_OFF);   // Make sure led is off
    digitalWrite(blueLed, LED_OFF);    // Make sure led is off

    //Protocol Configuration
    Serial.begin(9600);     // Initialize serial communications with PC
    SPI.begin();            // MFRC522 Hardware uses SPI protocol
    mfrc522.PCD_Init();     // Initialize MFRC522 Hardware

    //If you set Antenna Gain to Max it will increase reading distance
    mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

    Serial.println(F("Access Control Example v0.1"));   // For debugging purposes
    ShowReaderDetails();  // Show details of PCD - MFRC522 Card Reader details

    // Check if master card defined, if not let user choose a master card
    // This also useful to just redefine the Master Card
    // You can keep other EEPROM records just write other than 143 to EEPROM address 1
    // EEPROM address 1 should hold magical number which is '143'
    //cleanEEPROM()
    //EEPROM.write(LOG_BEGIN, MAGICAL_NUMBER);
    checkLogs();
}
void masterToSerial() {
    for (uint8_t i = 0; i < 4; i++) {                 // Read Master Card's UID from EEPROM
        masterCard[i] = EEPROM.read(2 + i);           // Write it to masterCard
        Serial.print(masterCard[i], HEX);
    }
}

bool isMasterDefined() {
    return EEPROM.read(1) == MAGICAL_NUMBER;
}

void defineMaster() {
    do {
        successRead = getID();                                          // sets successRead to 1 when we get read from reader otherwise 0
        digitalWrite(blueLed, LED_ON);                                  // Visualize Master Card need to be defined
        delay(200);
        digitalWrite(blueLed, LED_OFF);
        delay(200);
    } while (!successRead);                                             // Program will not go further while you not get a successful read
    for (uint8_t j = 0; j < 4; j++) {                               // Loop 4 times
        EEPROM.write(2 + j, readCard[j]);                             // Write scanned PICC's UID to EEPROM, start from address 3
    }
    EEPROM.write(1, MAGICAL_NUMBER);                                  // Write to EEPROM we defined Master Card.
    Serial.println(F("Master Card Defined"));
    Log log(masterCard, 6);
    log.writeLog();
}

#endif // !source
