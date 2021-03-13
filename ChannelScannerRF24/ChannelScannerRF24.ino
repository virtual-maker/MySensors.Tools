/*
   ChannelScannerRF24 - A nRF24L01+ based WiFi Channel Scanner
   optimised for MySensors RF24 radios

   Developed for the MySensors library https://www.mysensors.org/

   Created by Immo Wache <virtual.mkr@gmail.com>
   Copyright (C) 2021 by Immo Wache

   Code based on the Poor Man's Wireless 2.4GHz Scanner
   created March 2011 by Rolf Henkel
   https://forum.arduino.cc/index.php?topic=54795.0

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 3 as published by the Free Software Foundation.

 *******************************

   REVISION HISTORY
   Version 1.0 - Immo Wache

   DESCRIPTION
   Uses an nRF24L01+ connected to an Arduino AVR Uno/Nano/Mini

   Cables are:
	   SS       -> 10
	   MOSI     -> 11
	   MISO     -> 12
	   SCK      -> 13

   and CE       ->  9 - do not change CE to other pin, it is hard coded

*/

#include <SPI.h>

// Do not change CE to other pin, it is hard coded
#define CE (9)

// Array to hold Channel data for all available RF24 channels (1 - 126)
#define CHANNELS  126
int channel[CHANNELS];

int  line;

// greyscale mapping
const char* grayRamp = " .:-=+*aRW";
const size_t rampLength = strlen(grayRamp);

// nRF24L01P registers we need
#define _NRF24_CONFIG      0x00
#define _NRF24_EN_AA       0x01
#define _NRF24_RF_CH       0x05
#define _NRF24_RF_SETUP    0x06
#define _NRF24_RPD         0x09

// get the value of a nRF24L01p register
byte getRegister(byte r)
{
	byte c;

	PORTB &= ~_BV(2);
	c = SPI.transfer(r & 0x1F);
	c = SPI.transfer(0);
	PORTB |= _BV(2);

	return (c);
}

// set the value of a nRF24L01p register
void setRegister(byte r, byte v)
{
	PORTB &= ~_BV(2);
	SPI.transfer((r & 0x1F) | 0x20);
	SPI.transfer(v);
	PORTB |= _BV(2);
}

// power up the nRF24L01p chip
void powerUp(void)
{
	setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) | 0x02);
	delayMicroseconds(130);
}

// switch nRF24L01p off
void powerDown(void)
{
	setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) & ~0x02);
}

// enable RX
void enable(void)
{
	PORTB |= _BV(1);
}

// disable RX
void disable(void)
{
	PORTB &= ~_BV(1);
}

// Setup RX-Mode of nRF24L01p
void setRX(void)
{
	setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) | 0x01);
	enable();
	// this is slightly shorter than
	// the recommended delay of 130 microseconds
	// - but it works for me and speeds things up a little...
	delayMicroseconds(100);
}

// scanning all channels in the 2.4GHz band
void scanChannels(void)
{
	disable();
	for (int j = 0; j < 200; j++) {
		for (int i = 0; i < CHANNELS; i++) {
			// select a new channel
			setRegister(_NRF24_RF_CH, i);

			// switch on RX
			setRX();

			// wait enough for RX-things to settle
			delayMicroseconds(40);

			// this is actually the point where the RPD-flag
			// is set, when CE goes low
			disable();

			// read out RPD flag; set to 1 if
			// received power > -64dBm
			if (getRegister(_NRF24_RPD) > 0) {
				channel[i]++;
			}
		}
	}
}

// outputs channel data as a simple grey map
void outputChannels(void)
{
	int norm = 0;

	// find the maximal count in channel array
	for (int i = 0; i < CHANNELS; i++) {
		if (channel[i] > norm) {
			norm = channel[i];
		}
	}

	// now output the data
	Serial.print('|');
	for (int i = 0; i < CHANNELS; i++) {
		int pos;
		int rampMax = rampLength - 1;

		// calculate grey value position
		if (norm != 0) {
			pos = (channel[i] * rampMax) / norm;
		} else {
			pos = 0;
		}
		// boost low values
		if (pos == 0 && channel[i] > 0) {
			pos++;
		}
		// clamp large values
		if (pos > rampMax) {
			pos = rampMax;
		}
		// print it out
		Serial.print(grayRamp[pos]);
		channel[i] = 0;
	}

	// indicate overall power
	Serial.print("| ");
	Serial.println(norm);
}

// Give a visual reference between RF24 channels and displayed data
void printChannels(void)
{
	// Channel 1 - 126 Layout (0 ... 125) 2.400 - 2.525 MHz
	Serial.println(
	    F("|0         1         2         3         4         5         6         7         8         9         10        11        12    |"));
	Serial.println(
	    F("|012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345| max."));
}

// Give a visual reference between WiFi channels and displayed data
void printWiFiChannels(void)
{
	/* Output approximate positions of WLAN-channels; see: https://en.wikipedia.org/wiki/List_of_WLAN_channels#2.4_GHz_(802.11b/g/n/ax)
	Serial.println(F("|0         1         2         3         4         5         6         7         8         9         10        11        12    |"));
	Serial.println(F("|012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345|"));*/
	Serial.println(
	    F("|WiFi        1    2    3    4    5    6    7    8    9    10   11   12   13          14                                        |"));
}

void setup()
{
	Serial.begin(115200);

	Serial.println(F("Starting RF24 Wireless 2.4GHz Channel Scanner ..."));
	Serial.println();


	Serial.println(F("' ' = 0%; '.' > 1%; ':' > 20%; '-' > 30%; '=' > 40%; '+' > 50%;"
		" '*' > 60%; 'a' > 70%; 'R' > 80%; 'W' > 90% of max."));
	Serial.println(F("Channel 1 - 126 Layout (0 ... 125) ; 2.400 - 2.525 MHz"));
	Serial.println();
	printChannels();
	printWiFiChannels();

	// Setup SPI
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setBitOrder(MSBFIRST);

	// Activate Chip Enable
	pinMode(CE, OUTPUT);
	disable();

	// now start receiver
	powerUp();

	// switch off Enhanced ShockBurst
	setRegister(_NRF24_EN_AA, 0x0);

	// make sure RF-section is set properly
	// - just write default value...
	setRegister(_NRF24_RF_SETUP, 0x0F);

	// reset line counter
	line = 0;
}

void loop()
{
	// do the scan
	scanChannels();

	// output the result
	outputChannels();

	// output WLAN-channel reference every 12th line
	if (line++ > 12) {
		printChannels();
		line = 0;
	}
}
