#include "nand.h"

extern char buffer[50];

static const char *monthNames[] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

#define TOTAL_PAGES ((uint32_t)NAND_MAX_ZONE * NAND_ZONE_SIZE * NAND_BLOCK_SIZE)

static SensorRecord recordBuf[RECORDS_PER_SAVE];
static uint8_t  recordIndex = 0;
static uint32_t currentWriteAbs;    // absolute page index of the NEXT slot to write (counts down)
static uint32_t totalPagesWritten;  // how many record-pages saved so far
static uint8_t  nandReady = 0;

static void pad2(char *dst, uint8_t val){
    dst[0] = '0' + (val / 10);
    dst[1] = '0' + (val % 10);
    dst[2] = '\0';
}

static NAND_ADDRESS absolute_to_addr(uint32_t absPage){
    NAND_ADDRESS addr;
    addr.Page  = absPage % NAND_BLOCK_SIZE;
    uint32_t bz = absPage / NAND_BLOCK_SIZE;
    addr.Block = bz % NAND_ZONE_SIZE;
    addr.Zone  = bz / NAND_ZONE_SIZE;
    return addr;
}

void NAND_Init(void){
    FMC_NAND_Init();

    NAND_IDTypeDef id;
    FMC_NAND_ReadID(&id);
    sprintf(buffer, "NAND ID: %d %d %d %d\r\n", id.Maker_ID, id.Device_ID, id.Third_ID, id.Fourth_ID);
    USART1_send_string(buffer);

    currentWriteAbs = TOTAL_PAGES - 1;
    totalPagesWritten = 0;
    recordIndex = 0;

    FMC_NAND_EraseBlock(absolute_to_addr(currentWriteAbs));

    nandReady = 1;
}

static void flush_to_NAND(void){
    static uint8_t pageBuf[NAND_PAGE_SIZE];
    memset(pageBuf, 0xFF, NAND_PAGE_SIZE);
    memcpy(pageBuf, recordBuf, RECORDS_PER_SAVE * RECORD_SIZE);

    NAND_ADDRESS addr = absolute_to_addr(currentWriteAbs);
    uint32_t status = FMC_NAND_WriteSmallPage(pageBuf, addr, 1);

    if ((status & NAND_READY) == 0){
        sprintf(buffer, "NAND write FAIL: %d\r\n", (int)status);
        USART1_send_string(buffer);
        return;
    }

    sprintf(buffer, "Saved record-page %d\r\n", (int)totalPagesWritten);
    USART1_send_string(buffer);

    totalPagesWritten++;

    if (currentWriteAbs == 0){
        USART1_send_string("NAND full - wrapping to end\r\n");
        currentWriteAbs = TOTAL_PAGES - 1;
        FMC_NAND_EraseBlock(absolute_to_addr(currentWriteAbs));
        return;
    }

    currentWriteAbs--;

    NAND_ADDRESS nextAddr = absolute_to_addr(currentWriteAbs);
    if (nextAddr.Page == NAND_BLOCK_SIZE - 1){
        FMC_NAND_EraseBlock(nextAddr);
    }
}

void NAND_log_add_record(int16_t temp, uint8_t hum, uint16_t ldr)
{
    if (!nandReady) return;

    RTC_DateTypeDef d;
    RTC_TimeTypeDef t;
    RTC_get_current(&d, &t);

    recordBuf[recordIndex].year = d.RTC_Year;
    recordBuf[recordIndex].month = d.RTC_Month;
    recordBuf[recordIndex].date = d.RTC_Date;
    recordBuf[recordIndex].hour = t.RTC_Hours;
    recordBuf[recordIndex].min = t.RTC_Minutes;
    recordBuf[recordIndex].sec = t.RTC_Seconds;
    recordBuf[recordIndex].temp = temp;
    recordBuf[recordIndex].hum = hum;
    recordBuf[recordIndex].ldr = ldr;
    recordIndex++;

    if (recordIndex >= RECORDS_PER_SAVE){
        flush_to_NAND();
        recordIndex = 0;
    }
}

uint32_t NAND_log_get_total_pages(void){
    return totalPagesWritten;
}

uint8_t NAND_log_read_page(uint32_t pageIndex, SensorRecord *outArr){
    if (pageIndex >= totalPagesWritten) return 0;

    static uint8_t pageBuf[NAND_PAGE_SIZE];
    uint32_t absPage = (TOTAL_PAGES - 1) - pageIndex;
    NAND_ADDRESS addr = absolute_to_addr(absPage);

    FMC_NAND_ReadSmallPage(pageBuf, addr, 1);
    memcpy(outArr, pageBuf, RECORDS_PER_SAVE * RECORD_SIZE);
    return 1;
}

void sensor_record_to_date_str(const SensorRecord *rec, char *outStr){
    char dateBuf[3], hourBuf[3], minBuf[3], secBuf[3];
    pad2(dateBuf, rec->date);
    pad2(hourBuf, rec->hour);
    pad2(minBuf, rec->min);
    pad2(secBuf, rec->sec);

    const char *mon = monthNames[rec->month - 1];

    outStr[0] = '\0';
    strcat(outStr, dateBuf);
    strcat(outStr, " ");
    strcat(outStr, mon);
    strcat(outStr, " 20");

    char yearBuf[8];
    sprintf(yearBuf, "%d", rec->year);
    strcat(outStr, yearBuf);

    strcat(outStr, " - ");
    strcat(outStr, hourBuf);
    strcat(outStr, ":");
    strcat(outStr, minBuf);
    strcat(outStr, ":");
    strcat(outStr, secBuf);
}
