/*
 * nand.h
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#ifndef NAND_NAND_H_
#define NAND_NAND_H_
#include "rtc/rtc.h"
#include "usart/usart.h"
#include "fsmc_nand/fsmc_nand.h"
#include <string.h>
#include <stdio.h>
#pragma pack(push, 1)
typedef struct {
    uint8_t  year;
    uint8_t  month;
    uint8_t  date;
    uint8_t  hour;
    uint8_t  min;
    uint8_t  sec;
    int16_t  temp;
    uint8_t  hum;
    uint16_t ldr;
} SensorRecord;
#pragma pack(pop)

#define RECORD_SIZE       sizeof(SensorRecord)   // 10 bytes
#define RECORDS_PER_SAVE  5

void     NAND_Init(void);
void     NAND_log_add_record(int16_t temp, uint8_t hum, uint16_t ldr);
uint32_t NAND_log_get_total_pages(void);

uint8_t  NAND_log_read_page(uint32_t pageIndex, SensorRecord *outArr);
void     sensor_record_to_date_str(const SensorRecord *rec, char *outStr);

#endif /* NAND_NAND_H_ */
