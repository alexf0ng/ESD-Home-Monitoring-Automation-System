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

void RTC_Config(void)
{
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

    // If calendar is still at hardware default, (re)apply our starting date/time
    if (d.RTC_Year == 0 && d.RTC_Month == 1 && d.RTC_Date == 1 &&
        t.RTC_Hours == 0 && t.RTC_Minutes == 0 && t.RTC_Seconds == 0)
    {
        RTC_SetDateTime(26, 9, 8, 12, 0, 0);
    }
}

void RTC_GetDateTimeStr(char *outStr)
{
    RTC_DateTypeDef d;
    RTC_TimeTypeDef t;
    RTC_GetCurrent(&d, &t);

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

void RTC_GetCurrent(RTC_DateTypeDef *outDate, RTC_TimeTypeDef *outTime)
{
    RTC_GetDate(RTC_Format_BIN, outDate);
    RTC_GetTime(RTC_Format_BIN, outTime);
}

void RTC_GetDateTimeStr(char *outStr)
{
    RTC_DateTypeDef d;
    RTC_TimeTypeDef t;

    RTC_GetCurrent(&d, &t);

    sprintf(outStr, "%02d %s %04d - %02d:%02d:%02d",
            d.RTC_Date, monthNames[d.RTC_Month - 1], 2000 + d.RTC_Year,
            t.RTC_Hours, t.RTC_Minutes, t.RTC_Seconds);
}


