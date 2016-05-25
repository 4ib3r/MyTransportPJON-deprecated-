/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include "MyTransportPJON.h"

void (*debug)(const char *fmt, ... );
MyTransportPJON::MyTransportPJON(uint8_t pin)
  :
  MyTransport(),
  bus()
{
  #if INTERRUPT_TO_SEND == 1
  myInstance = this;
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  #endif
  bus.set_pin(pin);
}
bool MyTransportPJON::init() {
  bus.set_receiver(&receiver_function);
  bus.begin();
  return true;
}
#if INTERRUPT_TO_SEND == 1
SIGNAL(TIMER0_COMPA_vect) 
{
  myInstance->update();
}
#endif


void MyTransportPJON::setAddress(uint8_t address) {
  /*Serial.print("address: 0x");
    if (address < 15) Serial.print('0');
    Serial.println(address, HEX);*/
  _address = address;
  bus.set_id(address);
}

uint8_t MyTransportPJON::getAddress() {
  return _address;
}

bool MyTransportPJON::send(uint8_t to, const void* data, uint8_t len) {
  /*Serial.print("psend: 0x");
  if (to < 15) Serial.print('0');
  Serial.print(to, HEX);
  Serial.print(" len: ");
  Serial.print(len);*/
  uint16_t r = bus.send(to, (char *)data, len);
  /*Serial.print(" res: ");
  Serial.println(r);*/
  return (r != FAIL);
}


void MyTransportPJON::update() {
  bus.update();
}
void MyTransportPJON::receive() {
  bus.receive(500);
}

void MyTransportPJON::process() {
  bus.update();
  bus.receive(500);
}

bool MyTransportPJON::available(uint8_t *to) {
  if (_received) {
    *to = _address;
  }
  return _received;
}

uint8_t MyTransportPJON::receive(void* data) {
  uint8_t l = _received;
  memcpy(data, (const void *)_buff, l);
  _received = 0;
  return l;
}

void MyTransportPJON::powerDown() {
  //not used
}
