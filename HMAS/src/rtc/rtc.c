/*
 * rtc.c
 *
 *  Created on: Sep 8, 2026
 *      Author: User
 */
#include "rtc.h"
#include <string.h>

static const char *monthNames[] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static void pad2(char *dst, uint8_t val){
    dst[0] = '0' + (val / 10);
    dst[1] = '0' + (val % 10);
    dst[2] = '\0';
}

void RTC_Config_Init(StartDateTime startdatetime){
    RTC_InitTypeDef RTC_InitStructure;
    RTC_DateTypeDef d;
    RTC_TimeTypeDef t;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    RCC_LSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);   // no-op if already set - safe to call every boot
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();

    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7D;
    RTC_InitStructure.RTC_SynchPrediv  = 0xF9;
    RTC_Init(&RTC_InitStructure);

    RTC_GetDate(RTC_Format_BIN, &d);
    RTC_GetTime(RTC_Format_BIN, &t);

    if (d.RTC_Year == 0 && d.RTC_Month == 1 && d.RTC_Date == 1 &&
        t.RTC_Hours == 0 && t.RTC_Minutes == 0 && t.RTC_Seconds == 0)
    {
        RTC_set_date_time(startdatetime.year, startdatetime.month, startdatetime.day, startdatetime.hour, startdatetime.minute, startdatetime.second);
    }
}

void RTC_set_date_time(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec){
    RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeTypeDef RTC_TimeStructure;

    RTC_DateStructure.RTC_Year    = year;
    RTC_DateStructure.RTC_Month   = month;
    RTC_DateStructure.RTC_Date    = date;
    RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

    RTC_TimeStructure.RTC_Hours   = hour;
    RTC_TimeStructure.RTC_Minutes = min;
    RTC_TimeStructure.RTC_Seconds = sec;
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
}

void RTC_get_current(RTC_DateTypeDef *outDate, RTC_TimeTypeDef *outTime){
    RTC_GetDate(RTC_Format_BIN, outDate);
    RTC_GetTime(RTC_Format_BIN, outTime);
}

void RTC_get_date_time_str(char *outStr){
    RTC_DateTypeDef d;
    RTC_TimeTypeDef t;
    RTC_get_current(&d, &t);

    char dateBuf[3], hourBuf[3], minBuf[3], secBuf[3];
    pad2(dateBuf, d.RTC_Date);
    pad2(hourBuf, t.RTC_Hours);
    pad2(minBuf,  t.RTC_Minutes);
    pad2(secBuf,  t.RTC_Seconds);

    const char *mon = monthNames[d.RTC_Month - 1];

    outStr[0] = '\0';
    strcat(outStr, dateBuf);
    strcat(outStr, " ");
    strcat(outStr, mon);
    strcat(outStr, " 20");

    // year is only ever 0-99, safe with plain %d (no width/padding)
    char yearBuf[8];
    sprintf(yearBuf, "%d", d.RTC_Year);
    strcat(outStr, yearBuf);

    strcat(outStr, " - ");
    strcat(outStr, hourBuf);
    strcat(outStr, ":");
    strcat(outStr, minBuf);
    strcat(outStr, ":");
    strcat(outStr, secBuf);
}
