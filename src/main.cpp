#include <Arduino.h>
#include <EEPROM.h>
#include "BluetoothSerial.h"
#define EEPROM_SIZE 1 //max512 /4kb

BluetoothSerial SerialBT;
char data[30];

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE); // max 512 byte

  if (!SerialBT.begin("ESP32"))
  {
    Serial.println("An error occurred initializing Bluetooth");
  }
}

void loop()
{
  int i = 0;
  String dataIn = "";
  while (SerialBT.available())
  {
    char a = SerialBT.read();
    dataIn += a;
    i++;
  }
  delay(50);
  if ((i - 1) >= 0)
  {
    Serial.println("==========Print data============");
    Serial.println(dataIn);
    Serial.println(dataIn.length());
  }
  // ======== To read eeprom ===================
  // digitalRead(buttonPin)
  // ======== To wreite eeprom =====================
  // EEPROM.write(0, ledState);
  // EEPROM.commit();
  // ======================
}