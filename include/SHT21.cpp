#include <HTU21D.h>
float SHThum = 0;

HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);

void SHT_begin()
{
    if (myHTU21D.begin() != true)
    {
        Serial.println(F("HTU21D error"));
    }
    else
    {
        Serial.println(F("HTU21D OK"));
    }
    
}

String SHTTemp()
{
    float SHTtemp = 0.00;
    SHTtemp = myHTU21D.readTemperature();
    return (String(SHTtemp));
}

String SHTHum()
{
    float SHTHum = 0.00;
    SHTHum = myHTU21D.readCompensatedHumidity();
    return (String(SHTHum));
}