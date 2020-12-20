#include "Config.h"

void saveConfiguration(const char *filename, const Config &config)
{
    if (filesystem->exists(filename))
    {
        String string = "File Exist ";
        string += filename;
        Serial.println(string);
        Serial.println("-------FILE DELETED------");
        filesystem->remove(filename);
    }

    File file = filesystem->open(filename, "w");
    if (!file)
    {
        file.close();
        Serial.println("--FILE CREATING FAILED--");
    }
    else
    {
        Serial.println("------FILE CREATED------");
    }

    DynamicJsonDocument doc(256);

    doc["ssdpName"] = config.ssdpName;
    doc["ssid"] = config.ssid;
    doc["password"] = config.password;
    doc["ssidAP"] = config.ssidAP;
    doc["passwordAP"] = config.passwordAP;

    if (serializeJson(doc, file) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }

    file.close();
    Serial.println("-------FILE WRITED------");
    printFile(filename);
}

void loadConfiguration(const char *filename, Config &config)
{
    Serial.println("----Loading config-----");
    File file = filesystem->open(filename, "r");
    if (!file)
    {
        Serial.println("Failed to open config file");
        saveConfiguration(filename, config);
    }

    DynamicJsonDocument doc2(256);
    deserializeJson(doc2, file);
    config.ssdpName =   doc2["ssdpName"].as<char *>();
    config.ssid =       doc2["ssid"].as<char *>();
    config.password =   doc2["password"].as<char *>();
    config.ssidAP =     doc2["ssidAP"].as<char *>();
    config.passwordAP = doc2["passwordAP"].as<char *>();

    file.close();
}

void printFile(const char *filename)
{
    File file = filesystem->open(filename, "r");
    if (!file)
    {
        Serial.println("Failed to open config file");
    }

    while (file.available())
    {
        Serial.print((char)file.read());
    }
    Serial.println();
    file.close();
}