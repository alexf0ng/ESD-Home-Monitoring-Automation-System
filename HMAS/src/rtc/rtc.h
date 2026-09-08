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

void RTC_Config(void);
void RTC_SetDateTime(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec);
void RTC_GetDateTimeStr(char *outStr);
void RTC_GetCurrent(RTC_DateTypeDef *outDate, RTC_TimeTypeDef *outTime);



#endif /* RTC_RTC_H_ */
