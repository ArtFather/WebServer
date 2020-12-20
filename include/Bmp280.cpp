#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void Bmp280_begin(){

    if (!bmp.begin(0x76))
    {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
       
    }

    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    bmp_temp->printSensorDetails();
}

String Bmp280Temp()
{
    sensors_event_t temp_event;
    bmp_temp->getEvent(&temp_event);
    return (String(temp_event.temperature));
}

String Bmp280Press()
{
    sensors_event_t pressure_event;
    bmp_pressure->getEvent(&pressure_event);
    return (String(pressure_event.pressure));
}