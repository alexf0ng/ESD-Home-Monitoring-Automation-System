#include "nand_storage.h"
#include <string.h>

static SensorRecord_t sensorBuffer[RECORDS_PER_BATCH];
static uint8_t recordCount = 0;
static NAND_ADDRESS nextWriteAddr = {0, 0, 0};

static void EraseBlockIfNeeded(NAND_ADDRESS addr)
{
    if (addr.Page == 0)
    {
        FMC_NAND_EraseBlock(addr);
    }
}

void NandStorage_Init(void)
{
    recordCount = 0;
    nextWriteAddr.Zone  = 0;
    nextWriteAddr.Block = 0;
    nextWriteAddr.Page  = 0;
}

static void WriteSensorBatchToNAND(void)
{
    static uint8_t pageBuf[NAND_PAGE_SIZE];

    memset(pageBuf, 0xFF, NAND_PAGE_SIZE);
 
    memcpy(pageBuf, sensorBuffer, sizeof(sensorBuffer));

    EraseBlockIfNeeded(nextWriteAddr);

    uint32_t status = FMC_NAND_WriteSmallPage(pageBuf, nextWriteAddr, 1);

    if (status == NAND_READY)
    {
       
        FMC_NAND_AddressIncrement(&nextWriteAddr);
    }
}

void NandStorage_LogReading(float temp, float humi, float lux)
{
    sensorBuffer[recordCount].temperature = temp;
    sensorBuffer[recordCount].humidity    = humi;
    sensorBuffer[recordCount].intensity   = lux;
    recordCount++;

    if (recordCount >= RECORDS_PER_BATCH)
    {
        WriteSensorBatchToNAND();
        recordCount = 0;
    }
}

uint8_t NandStorage_ReadBatch(uint32_t batchIndex, SensorRecord_t *outBuffer)
{
    static uint8_t pageBuf[NAND_PAGE_SIZE];

    /* Since we write one batch per page, and pages are numbered
     * 0..63 within a block before moving to the next block,
     * translate a flat batchIndex into Zone/Block/Page. */
    NAND_ADDRESS addr;
    addr.Zone  = 0;
    addr.Block = batchIndex / NAND_BLOCK_SIZE;
    addr.Page  = batchIndex % NAND_BLOCK_SIZE;

    uint32_t status = FMC_NAND_ReadSmallPage(pageBuf, addr, 1);

    if (status != NAND_READY)
    {
        return 0;
    }

    memcpy(outBuffer, pageBuf, sizeof(SensorRecord_t) * RECORDS_PER_BATCH);
    return 1;  
}
