/*
 * initialize_page.c
 *
 *  Created on: Sep 8, 2026
 *      Author: User
 */
#include "initialize_page.h"

/* LCD resolution in Landscape */
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
static lv_color_t buf1[LCD_WIDTH * 20];

TM_STMPE811_TouchData touchData;

void my_touchpad_read(lv_indev_t * indev, lv_indev_data_t * data){
    if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed){
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchData.y;
        data->point.y = LCD_HEIGHT - touchData.x;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map){
    TM_ILI9341_DrawBuffer(
		area->x1,
		area->y1,
		area->x2,
		area->y2,
		(uint16_t *)px_map
	);

    lv_display_flush_ready(display);
}
void screen_init() {
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
	lv_display_t *display = lv_display_create(LCD_WIDTH, LCD_HEIGHT);
	lv_display_set_buffers(
		display,
		buf1,
		NULL,
		sizeof(buf1),
		LV_DISPLAY_RENDER_MODE_PARTIAL
	);
	lv_display_set_flush_cb(
		display,
		my_flush_cb
	);
	/* Create LVGL touch input device */
	lv_indev_t *indev = lv_indev_create();
	lv_indev_set_type(
		indev,
		LV_INDEV_TYPE_POINTER
	);
	lv_indev_set_read_cb(
		indev,
		my_touchpad_read
	);
}
static void switch_to_password(lv_timer_t *timer){
    password();
    lv_timer_del(timer);
}

void splash_and_jump(void){
	lv_timer_create(switch_to_password, 5000, NULL);
}


