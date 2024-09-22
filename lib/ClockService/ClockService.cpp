#include "ClockService.h"


ClockService::ClockService() : timeClient(ntpUDP, NTP_SERVER, NTP_OFFSET)
{
}

void ClockService::setup()
{
    timeClient.begin();
    timeClient.setTimeOffset(NTP_OFFSET);

    this->syncTime();
}

void ClockService::syncTime()
{
    timeClient.update();
}

void ClockService::getDateTime(char *datetime)
{
    time_t now = timeClient.getEpochTime();
    struct tm *timeinfo;
    timeinfo = localtime(&now);
    strftime(datetime, 128, "%Y-%m-%d %H:%M:%S", timeinfo);
}
