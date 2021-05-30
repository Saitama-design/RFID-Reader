#ifndef LED
#define LED

#define LED_ON LOW
#define LED_OFF HIGH


/////////////////////////////////////////  Access Granted    ///////////////////////////////////
void granted(uint16_t setDelay) {
	digitalWrite(blueLed, LED_OFF);   // Turn off blue LED
	digitalWrite(redLed, LED_OFF);  // Turn off red LED
	digitalWrite(greenLed, LED_ON);   // Turn on green LED
	delay(1500);            // Hold green LED on for a second
}

///////////////////////////////////////// Access Denied  ///////////////////////////////////
void denied() {
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	digitalWrite(redLed, LED_ON);   // Turn on red LED
	delay(1000);
}

///////////////////////////////////////// Cycle Leds (Program Mode) ///////////////////////////////////
void cycleLeds() {
	digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
	digitalWrite(greenLed, LED_ON);   // Make sure green LED is on
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	delay(200);
	digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
	digitalWrite(blueLed, LED_ON);  // Make sure blue LED is on
	delay(200);
	digitalWrite(redLed, LED_ON);   // Make sure red LED is on
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	delay(200);
}

//////////////////////////////////////// Normal Mode Led  ///////////////////////////////////
void normalModeOn() {
	digitalWrite(blueLed, LED_ON);  // Blue LED ON and ready to read card
	digitalWrite(redLed, LED_OFF);  // Make sure Red LED is off
	digitalWrite(greenLed, LED_OFF);  // Make sure Green LED is off
	//digitalWrite(relay, HIGH);    // Make sure Door is Locked
}

///////////////////////////////////////// Write Success to EEPROM   ///////////////////////////////////
// Flashes the green LED 3 times to indicate a successful write to EEPROM
void successWrite() {
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is on
	delay(200);
	digitalWrite(greenLed, LED_ON);   // Make sure green LED is on
	delay(200);
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
	delay(200);
	digitalWrite(greenLed, LED_ON);   // Make sure green LED is on
	delay(200);
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
	delay(200);
	digitalWrite(greenLed, LED_ON);   // Make sure green LED is on
	delay(200);
}

///////////////////////////////////////// Write Failed to EEPROM   ///////////////////////////////////
// Flashes the red LED 3 times to indicate a failed write to EEPROM
void failedWrite() {
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
	delay(200);
	digitalWrite(redLed, LED_ON);   // Make sure red LED is on
	delay(200);
	digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
	delay(200);
	digitalWrite(redLed, LED_ON);   // Make sure red LED is on
	delay(200);
	digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
	delay(200);
	digitalWrite(redLed, LED_ON);   // Make sure red LED is on
	delay(200);
}

///////////////////////////////////////// Success Remove UID From EEPROM  ///////////////////////////////////
// Flashes the blue LED 3 times to indicate a success delete to EEPROM
void successDelete() {
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	digitalWrite(redLed, LED_OFF);  // Make sure red LED is off
	digitalWrite(greenLed, LED_OFF);  // Make sure green LED is off
	delay(200);
	digitalWrite(blueLed, LED_ON);  // Make sure blue LED is on
	delay(200);
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	delay(200);
	digitalWrite(blueLed, LED_ON);  // Make sure blue LED is on
	delay(200);
	digitalWrite(blueLed, LED_OFF);   // Make sure blue LED is off
	delay(200);
	digitalWrite(blueLed, LED_ON);  // Make sure blue LED is on
	delay(200);
}

#endif 
