/*
 * record_page.c
 *
 *  Created on: Sep 8, 2026
 *      Author: User
 */
#include "record_page.h"

extern char buffer[50];
static uint32_t currentPage = 0;
static lv_obj_t *dateRows[RECORDS_PER_SAVE];
static lv_obj_t *dataRows[RECORDS_PER_SAVE];

static void record_page_refresh(void)
{
    uint32_t total = NAND_log_get_total_pages();

    if (total == 0){
        lv_label_set_text(ui_NextLabelR, "Page -");
        return;
    }

    SensorRecord recs[RECORDS_PER_SAVE];
    if (!NAND_log_read_page(currentPage, recs))
        return;

    for (int i = 0; i < RECORDS_PER_SAVE; i++){
        char dateStr[24];
        sensor_record_to_date_str(&recs[i], dateStr);
        lv_label_set_text(dateRows[i], dateStr);

        sprintf(buffer, "   %d °C   |   %d%% Hum   |   %d%% LDR", recs[i].temp, recs[i].hum, recs[i].ldr);
        lv_label_set_text(dataRows[i], buffer);
    }

    sprintf(buffer, "Page %d", (int)(currentPage + 1));
    lv_label_set_text(ui_NextLabelR, buffer);
}


void record_page_start(lv_event_t *e){
    dateRows[0] = ui_RecordDate1R;
    dataRows[0] = ui_RecordData1R;
    dateRows[1] = ui_RecordDate2R;
    dataRows[1] = ui_RecordData2R;
    dateRows[2] = ui_RecordDate3R;
    dataRows[2] = ui_RecordData3R;
    dateRows[3] = ui_RecordDate4R;
    dataRows[3] = ui_RecordData4R;
    dateRows[4] = ui_RecordDate5R;
    dataRows[4] = ui_RecordData5R;

    uint32_t total = NAND_log_get_total_pages();
    currentPage = (total > 0) ? total - 1 : 0;  // open on the newest saved batch

    record_page_refresh();
}

void record_page_prev_onc(lv_event_t *e){
    if (currentPage > 0){
        currentPage--;
        record_page_refresh();
    }
}

void record_page_next_onc(lv_event_t *e)
{
    uint32_t total = NAND_log_get_total_pages();
    if (total > 0 && currentPage + 1 < total){
        currentPage++;
        record_page_refresh();
    }
}

