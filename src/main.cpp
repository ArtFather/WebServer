#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include "WifiSetup.cpp"
#include "OTA.cpp"
#include "Config.cpp"
#include "WebServ.cpp"
#include "HTTPREQ.cpp"
#include "i2cScanner.cpp"

void setup()
{
  Serial.begin(115200);
  i2cScanner();
  FileSystem_begin();
  loadConfiguration(filename, config);
  printFile(filename);
  WIFIinit();
  OTA_begin();
  ServerInit();
  ReqInit();
  //Bmp280_begin();
  bmp180_begin();
  //DHT_begin();
  SHT_begin();
  delay(200);
  //TSL_begin();

  timeClient.begin();
  timeClient.forceUpdate();
  getDatTime();
}

    void loop()
    {
      ArduinoOTA.handle();
      Server.handleClient();
      MDNS.update();
      TimeClock();
    }