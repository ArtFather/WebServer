#include "WifiSetup.h"


IPAddress apIP(192, 168, 4, 1);

bool StartAPMode()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(config.ssidAP.c_str(), config.passwordAP.c_str());
    Serial.print("IP address: ");
    Serial.println(apIP);
    SignalStrength();
    return true;
}

void SignalStrength()
{
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

bool WIFIinit()
{
    WiFi.mode(WIFI_STA);
    byte tries = 11;
    WiFi.begin(config.ssid.c_str(), config.password.c_str());
    while (--tries && WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi up AP");
        StartAPMode();
        return (false);
    }
    else
    {
        Serial.println("");
        Serial.println("-----------WIFI OK-----------");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        SignalStrength();
        return (true);
    }
}