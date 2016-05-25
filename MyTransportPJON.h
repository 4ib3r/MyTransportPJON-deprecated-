
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

#ifndef MyTransportPJON_h
#define MyTransportPJON_h

#include <PJON.h>
#include <digitalWriteFast.h>
#include "MyConfig.h"
#include "MyTransport.h"
#include <stdint.h>

#define BROADCAST 255

#define INTERRUPT_TO_SEND 1

#define TO_ADDR(x) x

#define WRITE_PIPE ((uint8_t)0)
#define CURRENT_NODE_PIPE ((uint8_t)1)
#define BROADCAST_PIPE ((uint8_t)2)

class MyTransportPJON : public MyTransport
{ 
public:
  MyTransportPJON(uint8_t pin=12);
  bool init();
  void setAddress(uint8_t address);
  uint8_t getAddress();
  bool send(uint8_t to, const void* data, uint8_t len);
  bool available(uint8_t *to);
  uint8_t receive(void* data);
  void powerDown();
  void process();
  void update();
  void receive();
private:
  PJON<SoftwareBitBang> bus;
  uint8_t _address;
};

static MyTransportPJON *myInstance;
static uint8_t _buff[255];
static uint8_t _received = 0;
static void receiver_function(uint8_t _id, uint8_t *payload,uint8_t l) {
  Serial.print("preceived ");
  Serial.print(_id);
  Serial.print(": ");
  for (char i = 0; i < l; i++) {
    if (payload[i] < 15) {
      Serial.print('0');
    }
    Serial.print(payload[i], HEX);
  }
  Serial.println();
  if (_received == 0) {
    _received = l;
    memcpy(_buff, (const void *)payload, l); 
  }
};

#endif
