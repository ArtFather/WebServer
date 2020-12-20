#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp180;

void bmp180_begin(){
    if (!bmp180.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }
        Serial.println("BMP180 OK");
        float bmp180_temp = bmp180.readTemperature();
        Serial.print("BMP180 Temperature = ");
        Serial.print(bmp180_temp);
        Serial.println(" *C");
        float bmp_pressure = bmp180.readPressure();
        Serial.print("BMP180 Pressure = ");
        Serial.print(bmp_pressure);
        Serial.println(" Pa");
}


String Bmp180Temp()
{
    float bmp180_temp = bmp180.readTemperature();
  //  Serial.print("BMP180 Temperature = ");
    //Serial.print(bmp180_temp);
    //Serial.println(" *C");
    return (String(bmp180_temp));
}

String Bmp180Press()
{
    float bmp_pressure = bmp180.readPressure();
   // Serial.print("BMP180 Pressure = ");
    //Serial.print(bmp_pressure);
    //Serial.println(" Pa");
    return (String(bmp_pressure));
}