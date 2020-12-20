#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.ca.pool.ntp.org", -18000);
String Date1 = "", Time1 = "";
String formattedDate;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

int startingHour = timeClient.getHours(); // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int seconds      = timeClient.getSeconds();
int minutes      = timeClient.getMinutes();
int hours        = startingHour;

int oldHour = 0, oldSec = 0;

void getDatTime()
{
    Date1 = "";
    timeClient.update();

    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    String formattedTime = timeClient.getFormattedTime();

    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon + 1;
    int currentYear = ptm->tm_year + 1900;

    String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    Serial.print("Current date: ");
    Serial.print(currentDate);
    Serial.print("  Current time: ");
    Serial.println(formattedTime);
    Date1 += currentDate;
}

void TimeClock()
{
    timeNow = millis() / 1000;    // the number of milliseconds that have passed since boot
    seconds = timeNow - timeLast; //the number of seconds that have passed since the last time 60 seconds was reached.

    if (seconds == 60)
    {
        seconds = 0;
        timeLast = timeNow;
        minutes = minutes + 1;
    }

    //if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

    if (minutes == 60)
    {
        minutes = 0;
        hours = hours + 1;
    }

    // if one hour has passed, start counting minutes from zero and add one hour to the clock

    if (hours == 24)
    {
        hours = 0;
    }

    if (hours != oldHour)
    {
        Serial.printf("Updating Time");
        getDatTime();
        hours = timeClient.getHours();
        timeLast = timeClient.getSeconds();
        minutes = timeClient.getMinutes();
        oldHour = hours;
    }
    
    if (seconds != oldSec)
    {
        Time1 = "";
        Time1 += hours;
        Time1 += ':';
        Time1 += minutes;
        Time1 += ':';
        Time1 += seconds;
        /*    Serial.print("The Timer time is: ");
        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(":");
        Serial.println(seconds);*/
        oldSec = seconds;
    }
}