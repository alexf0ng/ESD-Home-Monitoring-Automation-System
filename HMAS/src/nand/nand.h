/*
 * nand.h
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#ifndef NAND_NAND_H_
#define NAND_NAND_H_

#include "fsmc_nand/fsmc_nand.h"
#include "usart/usart.h"
#include <string.h>
#include <stdio.h>

typedef struct {
	uint16_t temp;
	uint16_t hum;
	uint16_t ldr;
} SensorRecord;

#define RECORD_SIZE sizeof(SensorRecord)
#define RECORDS_PER_SAVE 5

void NAND_Log_Init(void);
void NAND_Log_AddRecord(uint16_t temp, uint16_t hum, uint16_t ldr);
void NAND_Log_ReadLast(SensorRecord *out, uint8_t count);

#endif /* NAND_NAND_H_ */
