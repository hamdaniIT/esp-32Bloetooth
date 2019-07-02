#include <Arduino.h>
#include <EEPROM.h>
#include "BluetoothSerial.h"
#define EEPROM_SIZE 512 //max512 /4kb

// seting device BYTE
#define TYPE 0
#define SN 1
#define SSID 2
#define PASSWORD 3
#define MQTT_BROKER 4
#define MQTT_PORT 5
#define MQTT_USER 6
#define MQTT_PASSWORD 7

// end seting device

BluetoothSerial SerialBT;
char data[30];

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void writeString(char add, String data)
{
  int _size = data.length();
  int i;
  for (i = 0; i < _size; i++)
  {
    EEPROM.write(add + i, data[i]);
  }
  EEPROM.write(add + _size, '\0'); //Add termination null character for String Data
  EEPROM.commit();
}

String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500) //Read until null character
  {
    k = EEPROM.read(add + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  return String(data);
}
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE); // max 512 byte

  if (!SerialBT.begin("ESP32"))
  {
    Serial.println("An error occurred initializing Bluetooth");
  }
}
void erase_data()
{
  for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}
void printData()
{
  Serial.println("==========Print data============");
  String setDev = read_String(0);
  String type = getValue(setDev, ';', TYPE);
  String sn = getValue(setDev, ';', SN);
  String ssid = getValue(setDev, ';', SSID);
  String password = getValue(setDev, ';', PASSWORD);
  String mqttBroker = getValue(setDev, ';', MQTT_BROKER);
  int mqttPort = getValue(setDev, ';', MQTT_PORT).toInt();
  String mqttUser = getValue(setDev, ';', MQTT_USER);
  String mqttPassword = getValue(setDev, ';', MQTT_PASSWORD);
  String str = "Type : " + type + "\n" + "SN : " + sn + "\n" + "SSID : " + ssid + "\n" + "PASSWORD : " + password + "\n" +
               "MQTT_BROKER : " + mqttBroker + "\n" + "MQTT_PORT : " + mqttPort + "\n" + "MQTT_USER : " + mqttUser + "\n" + "mqttPassword : " +
               mqttPassword + "\n";
  Serial.print(str);
  Serial.println("==========Print data end============");
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
    erase_data();
    writeString(0,dataIn);
    printData();
  }
}