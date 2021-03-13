/*
 * TrafficDetector24 - A nRF24L01+ based WiFi Traffic Detector 
 * for logic analysers 
 * 
 * Developed for the MySensors library https://www.mysensors.org/
 * 
 * Created by Immo Wache <virtual.mkr@gmail.com>
 * Copyright (C) 2021 by Immo Wache
 * 
 * Code based on the Poor Man's Wireless 2.4GHz Scanner
 * created March 2011 by Rolf Henkel
 * https://forum.arduino.cc/index.php?topic=54795.0
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 * 
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Immo Wache
 *
 * DESCRIPTION
 * Uses an nRF24L01+ connected to an Arduino AVR Uno/Nano/Mini
 *
 * Cables are:
 *     SS       -> 10
 *     MOSI     -> 11
 *     MISO     -> 12
 *     SCK      -> 13
 *
 * and CE       ->  9 - do not change CE to other pin, it is hard coded 
 *
 * Define MY_CHANNEL to detect radio traffic on it
 * and DEBUG_PIN to connect with a LED or better with a logic analyser
 * 
 */

// Default channel for MySensors RF24 is 76 (Channel 77)
#define MY_CHANNEL 76

#define DEBUG_PIN A0

#include <SPI.h>

// Do not change CE to other pin, it is hard coded 
#define CE  9

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

// setup RX-Mode of nRF24L01p
void setRX(void)
{
  setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) | 0x01);
  enable();
  // Recommended delay of 130 usec
  delayMicroseconds(130);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting Wireless 2.4GHz Channel Traffic Detector");
  Serial.print("Detect traffic on channel: ");
  Serial.println(MY_CHANNEL);

  // Setup SPI
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);

  // Activate debug pins
  pinMode(DEBUG_PIN, OUTPUT);

  // Activate Chip Enable
  pinMode(CE, OUTPUT);
  disable();

  // Now start receiver
  powerUp();

  // Switch off Shockburst
  setRegister(_NRF24_EN_AA, 0x0);

  // Make sure RF-section is set properly
  // - Just write default value...
  setRegister(_NRF24_RF_SETUP, 0x0F);

  // Set the channel to scan
  setRegister(_NRF24_RF_CH, MY_CHANNEL);

  // Switch on RX and disable CE
  setRX();
  disable();

  Serial.println("Start traffic detection ...");
}

void loop()
{
  enable();

  // Wait enough for RX-things to settle
  // This is slightly shorter than
  // the recommended delay of 130 + 40 usec
  // but it works for me and speeds things up a little
  delayMicroseconds(140);

  // This is actually the point where the RPD-flag
  // is set, when CE goes low
  disable();

  // Read out RPD flag
  // Set to 1 if received power > -64dBm
  bool active = getRegister(_NRF24_RPD) > 0;

  // Output active state to debug pin
  digitalWrite(DEBUG_PIN, active ? HIGH : LOW);
}
