/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jalling.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 */
/*
 DSTH01
 Library for communicationg with the DSTH01 temperature/
 humidity module over I2C.

 The DSTH01 module carries the Silabs Si7005 temperature/
 humidity sensor, or later versions designated  Si701x/2x.

 The DSTH01 library is based on the Si7005 library:
 created 9 Aug 2013
 by Jonas B. Jalling
 
 The library was modified by iggie to avoid delays by separating
 the setup and reading code into separate methods:
   startTemperature
   readTemperature
   startHumidity
   readHumidity
 This example is based on the Si7005 library example,
 Modified by iggie to work with the DSTH01 Library
*/

#include <Wire.h>
#include <DSTH01.h>
// These are the defaults for ESP8266 I2C, but any pin can be used.
// const int SDA_PIN = 4;
// const int SCL_PIN = 5;

// The global sensor object
DSTH01 Si;

void setup() {
//	Wire.begin (SDA_PIN, SCL_PIN);
	Wire.begin ();
//	Serial.begin(115200);
	Serial.begin(9600);
  
	if (Si.detectSensor()) {
		// The sensor is detected. Lets carry on!
		Serial.println("Device detected"); 
	} else {
		Serial.println("Device NOT detected");
		while (1) {;;} // The sensor is not detected. Let's just stay here forever.
	}
}

void loop() {
	float temperature, humidity;

	// Uncomment line below to enable faster measurements (lower resolution)
	// Si.enableFastMeasurements();

	// Uncomment line below to enable the internal heater (affects the temperature measurement)
	Si.enableHeater();

	// Do some measurements!
	Si.startTemperature();
	delay (50);
	temperature = Si.readTemperature();

	Si.startHumidity();
	delay (50);
	humidity = Si.readHumidity();

	// And output the data
	Serial.print("Temperature: ");
	Serial.print(temperature);
	Serial.print(" C, Humidity: ");
	Serial.print(humidity);
	Serial.println(" %RH");
	delay(1000);
}
