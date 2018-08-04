# MyTransportPJON
PJON based transport for MySensors <a href="https://github.com/gioblu/PJON">https://github.com/gioblu/PJON</a>

Timer0 interrupt is used to send messages from queue in 1ms intervals.

## Requirements
PJON 3.0 library
MySensor libraries

## Example
DHT sensor node
```c++
#include "DHT.h"
#include <MySigningNone.h>
#include <MyHwATMega328.h>
#include <MySensor.h>
#include "MyTransportPJON.h"
#include <Agenda.h>

#define ADDRESS 5

Agenda scheduler;

//Define transport and set pin 12 for wire
MyTransportPJON transport(12);
// Select AtMega328 hardware profile
MyHwATMega328 hw;
// Construct MySensors library
MySensor gw(transport, hw);

DHT dht;

#define TEMP_ID 1
#define HUM_ID 2

MyMessage msgTemp(TEMP_ID, V_TEMP);
MyMessage msgHum(HUM_ID, V_HUM);

void setup()
{
  gw.begin(NULL, ADDRESS, false, 0);
  delay(1);
  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("TempHum", "1.1");
  delay(1); //added delay for send
  dht.setup(9); // data pin 2
  gw.present(TEMP_ID, S_TEMP, "temp1", false);
  gw.present(HUM_ID, S_HUM, "hum1", false);
  scheduler.insert(readDht, 5000000); //schedule readDht in 5s intervals
}

void readDht() {
  float h = dht.getHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.getTemperature();
  
  Serial.print("Readed: ");
  Serial.print(h);
  Serial.print("% ");
  Serial.print(t);
  Serial.println("*C");
  if (!isnan(h) && !isnan(t)) {
    gw.send(msgTemp.set(t, 2)); 
    gw.send(msgHum.set(h, 2));
  }
}

void loop() 
{
  //receive messages throught protocol
  transport.receive();
  gw.process();
  scheduler.update();
}
```
