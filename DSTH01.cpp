/* DSTH01 Temperature/Humidity sensor Arduino Library
 * Based on Si7005 library by Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jalling.dk> wrote this file.	As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.		Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 */
#include "Arduino.h"

#include "DSTH01.h"
#include <Wire.h>

// the CS pin is optional
DSTH01::DSTH01( int pin ) {
	_cs_pin = pin;
	if (_cs_pin > -1) {
		pinMode( _cs_pin, OUTPUT );
		digitalWrite( _cs_pin, HIGH );
	}
	_last_temperature = 25.0;
	_config_reg = 0;
}

bool DSTH01::detectSensor( ) {
	byte deviceID;

	if (_cs_pin > -1) {
		pinMode( _cs_pin, OUTPUT );     // Enable the sensor
		digitalWrite( _cs_pin, LOW );
		delay( WAKE_UP_TIME );	        // Wait for it to wake up
	}

	Wire.beginTransmission( DSTH01_ADR );
	Wire.write( REG_ID );            // Select the ID register
	Wire.endTransmission( false );   // We don't want to release the bus - this is important!

	Wire.requestFrom( DSTH01_ADR, 1 );
	deviceID	= Wire.read( );      // Read the ID from the sensor

	if (_cs_pin > -1) {
		digitalWrite( _cs_pin, HIGH );   // Disable the sensor
	}

	if ( ( deviceID & ID_SAMPLE ) == ID_DSTH01 ) {
		return true;
	} else {
		return false;
	}
}

void DSTH01::_startMeasurement( byte configValue ) {	
	if (_cs_pin > -1) {
		pinMode( _cs_pin, OUTPUT );     // Enable the sensor
		digitalWrite( _cs_pin, LOW );
		delay( WAKE_UP_TIME );	        // Wait for it to wake up
	}

	Wire.beginTransmission( DSTH01_ADR );
	Wire.write(REG_CONFIG);                                 // Select the CONFIG register
	Wire.write( CONFIG_START | configValue | _config_reg ); // Start measurement of the selected type (Temperature / humidity)
	Wire.endTransmission( );
}

int DSTH01::_readMeasurement() {
	int rawData;
	byte measurementStatus;

	measurementStatus = STATUS_NOT_READY;                   // Wait for the measurement to finish
	while ( measurementStatus & STATUS_NOT_READY ) {
		Wire.beginTransmission( DSTH01_ADR );
		Wire.write( REG_STATUS );
		Wire.endTransmission( false );                      // We don't want to release the bus - this is important!
		Wire.requestFrom( DSTH01_ADR, 1 );
		measurementStatus = Wire.read( );
	}

	Wire.beginTransmission( DSTH01_ADR );
	Wire.write( REG_DATA );                                 // Select the DATA register
	Wire.endTransmission( false );

	Wire.requestFrom( DSTH01_ADR, 2 );                      // Read 2 bytes from the sensor
	while ( Wire.available( ) < 2 )                         // Wait for data
	{;;}
	
	rawData	 = ( Wire.read() << 8 );                        // MSB
	rawData |= Wire.read( );                                // LSB
	
	if (_cs_pin > -1) {
		digitalWrite( _cs_pin, HIGH );                          // Disable the sensor
	}

	return rawData;
}

void DSTH01::startTemperature( ) {
	_startMeasurement (CONFIG_TEMPERATURE);
}
float DSTH01::readTemperature( ) {
	float rawTemperature;

	rawTemperature = _readMeasurement() >> 2;
	_last_temperature = ( rawTemperature / TEMPERATURE_SLOPE ) - TEMPERATURE_OFFSET;

	return _last_temperature;
}

void DSTH01::startHumidity( ) {
	_startMeasurement (CONFIG_HUMIDITY);
}
float DSTH01::readHumidity( ) {
	float rawHumidity, curve, linearHumidity;

	rawHumidity = _readMeasurement() >> 4;
	curve = ( rawHumidity / HUMIDITY_SLOPE ) - HUMIDITY_OFFSET;
	linearHumidity = curve - ( (curve * curve) * a2 + curve * a1 + a0);
	linearHumidity = linearHumidity + ( _last_temperature - 30 ) * ( linearHumidity * q1 + q0 );

	return linearHumidity;
}

void DSTH01::enableHeater( ) {
	_config_reg |= CONFIG_HEAT;
}

void DSTH01::disableHeater( ) {
	_config_reg ^= CONFIG_HEAT;
}

void DSTH01::enableFastMeasurements( ) {
	_config_reg |= CONFIG_FAST;
}

void DSTH01::disableFastMeasurements( ) {
	_config_reg ^= CONFIG_FAST;
}