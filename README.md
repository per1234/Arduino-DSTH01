What ?
======
This is a Arduino library for the Silabs Si7005 temperature/humidity sensor.
The library makes it easy to use the sensor and all of its features.

This was slightly modified and renamed to DSTH01, which is a more common name for
These modules as of this writing.

Other modifications include:

  * separated "get~" methods into "start~" and "read~" methods to avoid delays
  * Made setting of CS pin optional


Why ?
=====
Because I think it's an interesting sensor, and I wanted to test it. I couldn't find
any library or sample code on the internet, so i wrote my own.


Getting started
===============
Copy the directory DSTH01 to you Arduino library folder.

Load the Arduino IDE and open the example program from the file menu.

The humidity measurement is dependant on the temperature measurement, and uses the last
measured temperature.
If no temperature measurement is done at all, a default value of 25.0C is used.


Who ?
=====
* Written by Jonas Bo Jalling, <jonas@jalling.dk>
* Modified by igg2, https://github.com/igg2


License ?
=========

	/*
	 * ----------------------------------------------------------------------------
	 * "THE BEER-WARE LICENSE" (Revision 42):
	 * <jonas@jalling.dk> wrote this file.  As long as you retain this notice you
	 * can do whatever you want with this stuff. If we meet some day, and you think
	 * this stuff is worth it, you can buy me a beer in return.   Jonas Bo Jalling
	 * ----------------------------------------------------------------------------
	 */
