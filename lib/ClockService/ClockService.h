#ifndef CLOCKSERVICE_H
#define CLOCKSERVICE_H


#include <ctime>
#include <iomanip>
#include <sstream>

#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "../../config/config.h"


class ClockService
{
    private:
        WiFiUDP ntpUDP;
        NTPClient timeClient = NTPClient(ntpUDP, NTP_SERVER, NTP_OFFSET);

        unsigned long lastSync = 0;
        unsigned long syncInterval = 60000;
    public:
        ClockService();

        void setup();
        void syncTime();
        void getDateTime(char* datetime);
        String getDateTime();
};

#endif