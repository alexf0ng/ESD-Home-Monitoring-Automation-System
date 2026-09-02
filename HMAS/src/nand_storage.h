#ifndef __NAND_STORAGE_H
#define __NAND_STORAGE_H

#include "stm32f4xx.h"
#include "fmsc_nand.h"   

#define RECORDS_PER_BATCH   5


typedef struct {
    float temperature;
    float humidity;
    float intensity;
} 
SensorRecord_t;

/* Call once at startup, after FMC_NAND_Init() */
void NandStorage_Init(void);

/* Call every time you take a new reading (e.g. every 10s).
 * Once 5 readings have accumulated, this automatically writes
 * the batch to NAND flash and resets the counter. */
void NandStorage_LogReading(float temp, float humi, float lux);

/* Reads back one previously-written batch of 5 records.
 * batchIndex = 0 for the first batch ever written, 1 for the
 * second, etc. (each batch occupies one NAND page). */
uint8_t NandStorage_ReadBatch(uint32_t batchIndex, SensorRecord_t *outBuffer);

#endif 
