/*
 * page.c
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#include "page.h"

void dashboard(void) {
	_ui_screen_change(
		&ui_DashboardPage,
		LV_SCR_LOAD_ANIM_NONE,
		50,
		0,
		&ui_DashboardPage_screen_init
	);

}

void sensor(void) {
	_ui_screen_change(
		&ui_SensorPage,
		LV_SCR_LOAD_ANIM_NONE,
		50,
		0,
		&ui_SensorPage_screen_init
	);
}
void record(void) {
	_ui_screen_change(
		&ui_RecordPage,
		LV_SCR_LOAD_ANIM_FADE_ON,
		500,
		0,
		&ui_RecordPage_screen_init
	);
}
void setting(void) {
	_ui_screen_change(
		&ui_SettingPage,
		LV_SCR_LOAD_ANIM_NONE,
		50,
		0,
		&ui_SettingPage_screen_init
	);


}
void password(void) {
	_ui_screen_change(
		&ui_PasswordPage,
		LV_SCR_LOAD_ANIM_NONE,
		50,
		0,
		&ui_PasswordPage_screen_init
	);

}

