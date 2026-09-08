/*
 * rtc.h
 *
 *  Created on: Sep 8, 2026
 *      Author: User
 */

#ifndef RTC_RTC_H_
#define RTC_RTC_H_

#include "stm32f4xx.h"
#include <stdio.h>
typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} StartDateTime;

void RTC_Config_Init(StartDateTime startdatetime);
void RTC_set_date_time(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec);
void RTC_get_date_time_str(char *outStr);
void RTC_get_current(RTC_DateTypeDef *outDate, RTC_TimeTypeDef *outTime);



#endif /* RTC_RTC_H_ */
