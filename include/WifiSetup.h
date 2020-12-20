#ifndef __WIFISETUP_H_
#define __WIFISETUP_H_ 1

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "Pass.h"


    bool WIFIinit();
    bool StartAPMode();
    void SignalStrength();


#endif