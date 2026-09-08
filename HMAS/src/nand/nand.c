/*
 * nand.c
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */
// nand_log.c
#include "nand.h"

extern char buffer[50];

static SensorRecord recordBuf[RECORDS_PER_SAVE];
static uint8_t recordIndex = 0;
static NAND_ADDRESS currentAddr = {0, 0, 0};
static uint8_t nandReady = 0;

void NAND_Log_Init(void){
    FMC_NAND_Init();

    NAND_IDTypeDef id;
    FMC_NAND_ReadID(&id);
    sprintf(buffer, "NAND ID: %02X %02X %02X %02X\r\n",
    id.Maker_ID, id.Device_ID, id.Third_ID, id.Fourth_ID);
    USART1_send_string(buffer);

    // Erase the very first block so we start clean
    FMC_NAND_EraseBlock(currentAddr);

    recordIndex = 0;
    nandReady = 1;
}

static void FlushToNand(void){
    static uint8_t pageBuf[NAND_PAGE_SIZE];

    memset(pageBuf, 0xFF, NAND_PAGE_SIZE);
    memcpy(pageBuf, recordBuf, RECORDS_PER_SAVE * RECORD_SIZE);

    uint32_t status = FMC_NAND_WriteSmallPage(pageBuf, currentAddr, 1);

    if ((status & NAND_READY) == 0)
    {
        sprintf(buffer, "NAND write FAIL: 0x%lX\r\n", status);
        USART1_send_string(buffer);
        return;
    }

    sprintf(buffer, "NAND write OK (Zone%d Blk%d Pg%d)\r\n",
    currentAddr.Zone, currentAddr.Block, currentAddr.Page);
    USART1_send_string(buffer);

    uint32_t incStatus = FMC_NAND_AddressIncrement(&currentAddr);

    if (currentAddr.Page == 0 && incStatus == NAND_VALID_ADDRESS){
        FMC_NAND_EraseBlock(currentAddr);  // erase next block before first use
    }
}

void NAND_Log_AddRecord(uint16_t temp, uint16_t hum, uint16_t ldr){
    if (!nandReady) return;

    recordBuf[recordIndex].temp = temp;
    recordBuf[recordIndex].hum  = hum;
    recordBuf[recordIndex].ldr  = ldr;
    recordIndex++;

    if (recordIndex >= RECORDS_PER_SAVE){
        FlushToNand();
        recordIndex = 0;
    }
}

void NAND_Log_ReadLast(SensorRecord *out, uint8_t count){
    static uint8_t pageBuf[NAND_PAGE_SIZE];
    NAND_ADDRESS readAddr = {0, 0, 0}; // adjust if you want a specific page

    FMC_NAND_ReadSmallPage(pageBuf, readAddr, 1);
    memcpy(out, pageBuf, count * RECORD_SIZE);
}


