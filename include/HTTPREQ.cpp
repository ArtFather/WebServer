#include "WebServ.h"
//#include "Bmp280.cpp"
#include "DHT.cpp"
#include "SHT21.cpp"
#include "NTP.cpp"
#include "TSL.cpp"
#include "Bmp180.cpp"

float R1 = 10000.00; // resistance of R1 (10K)
float R2 = 1000.00;  // resistance of R2 (1K)
float Vout = 0.00;
float Vin = 0.00;w
int val = 0;


void handle_ConfigJSON()
{
  String output = "";
  StaticJsonDocument<256> doc;

  doc["ssdpName"] = config.ssdpName;
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["ssidAP"] = config.ssidAP;
  doc["passwordAP"] = config.passwordAP;

  if (serializeJson(doc, output) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  Server.send(200, "text/json", output);
}

void handle_DateTime()
{
  String output = "";
  StaticJsonDocument<64> doc;
  
  doc["Date"] = Date1;
  doc["Time"] = Time1;

  if (serializeJson(doc, output) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  Server.send(200, "text/json", output);
}

  void handle_DataJSON()
  {
    String output = "";

    val = analogRead(0);           //reads the analog input
    Vout = (val * 3.30) / 1024.00; // formula for calculating voltage out i.e. V+, here 5.00
    Vin = Vout / (R2 / (R1 + R2)); // formula for calculating voltage in i.e. GND
    String adcValue = String(Vin);

    StaticJsonDocument<256> doc;
    doc["ADCValue"] = adcValue;
    doc["BMPTemp"]  = Bmp180Temp();
    doc["BMPPres"]  = Bmp180Press();
    doc["DHTTemp"]  = DHTTemp();
    doc["DHTHum"]   = DHTHum();
    doc["SHTTemp"]  = SHTTemp();
    doc["SHTHum"]   = SHTHum();
    doc["Lux"]      = Lux();

    if (serializeJson(doc, output) == 0)
    {
      Serial.println(F("Failed to write to file"));
    }
    Server.send(200, "text/json", output);
  }

  void handleADC()
  {
    val = analogRead(0);           //reads the analog input
    Vout = (val * 3.30) / 1024.00; // formula for calculating voltage out i.e. V+, here 5.00
    Vin = Vout / (R2 / (R1 + R2)); // formula for calculating voltage in i.e. GND
    String adcValue = String(Vin);
    Server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
  }

  void handle_Restart()
  {
    Serial.println("-----------RESTART-----------");
    String restart = Server.arg("device");
    if (restart == "ok")
    {
      Server.send(200, "text / plain", "Reset OK");
      ESP.restart();
    }
    else
    {
      Server.send(200, "text / plain", "No Reset");
    }
  }

  void handle_Set_Ssid()
  {
    config.ssid = Server.arg("ssid");           // Получаем значение ssid из запроса сохраняем в глобальной переменной
    config.password = Server.arg("password");   // Получаем значение password из запроса сохраняем в глобальной переменной
    saveConfiguration(filename, config);        // Функция сохранения данных во Flash пока пустая
    Server.send(200, "text/plain", "OK");       // отправляем ответ о выполнении
    Serial.println("-----------SSIDANDPASS CNAHGED-----------");
  }

  void handle_Set_SsidAP()
  {
    config.ssidAP = Server.arg("ssidAP");       // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
    config.passwordAP = Server.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
    saveConfiguration(filename, config);          // Функция сохранения данных во Flash пока пустая
    Server.send(200, "text/plain", "OK");       // отправляем ответ о выполнении
    Serial.println("-----------SSIDAPANDPASS CNAHGED---------");
  }

  void ReqInit(void)
  {
    Server.on("/restart",      handle_Restart);
    Server.on("/readADC",      handleADC);
    Server.on("/config.json",  handle_ConfigJSON);
    //Server.on("/data.json",    handle_DataJSON);
    Server.on("/data",         handle_DataJSON);
    Server.on("/DateTime.json",handle_DateTime);
    Server.on("/ssid",         handle_Set_Ssid);
    Server.on("/ssidAP",       handle_Set_SsidAP);

    Server.on(
        "/update", HTTP_POST, []() {
    Server.sendHeader("Connection", "close");
    Server.sendHeader("Access-Control-Allow-Origin", "*");
    //  Server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
    ESP.restart(); }, []() {
    HTTPUpload& upload = Server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.setDebugOutput(true);
      WiFiUDP::stopAll();
      // Serial.printf("Update: %s\n", upload.filename.c_str());
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        // Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        Serial.println("Update Success");
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    }
    yield(); });
}



