/*
 * password_page.c
 *
 *  Created on: Sep 8, 2026
 *      Author: User
 */
#include "password_page.h"

#define PC5_HOLD_MS           5000
#define PC5_CHECK_INTERVAL_MS 100

static char entered_pin[5] = "";
static uint8_t pin_index = 0;
static bool password_reset_mode = false;
static uint32_t pc5_hold_ms = 0;
static bool pc5_triggered = false;
extern User user;

void reset(void){
	pin_index = 0;
	entered_pin[0] = '\0';

	lv_obj_clear_state(ui_Pin0, LV_STATE_CHECKED);
	lv_obj_clear_state(ui_Pin1, LV_STATE_CHECKED);
	lv_obj_clear_state(ui_Pin2, LV_STATE_CHECKED);
	lv_obj_clear_state(ui_Pin3, LV_STATE_CHECKED);
}

static void led_test_done(lv_timer_t *timer){
    lv_timer_del(timer);
    GPIO_openObLED(false, false);
    dashboard();
}
void login_success_led_test(){
	lv_label_set_text(ui_InitializeMsg1, "Testing LEDs...");
	lv_label_set_text(ui_InitializeMsg2, "Testing...");
	// assert led here
	GPIO_openObLED(true, true);
	// end

	_ui_screen_change(&ui_InitializePage, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_InitializePage_screen_init);
	lv_timer_create(led_test_done, 2000, NULL);
}


void num_btn_onc(const char * num_str ){
	if(pin_index >= 4)
		return;

	entered_pin[pin_index] = num_str[0];
	entered_pin[pin_index + 1] = '\0';

	pin_index++;

//    sprintf(buffer, "PIN: %s\r\n", entered_pin);
//    USART1_send_string(buffer);

	if(pin_index == 1)
		lv_obj_add_state(ui_Pin0, LV_STATE_CHECKED);
	else if(pin_index == 2)
		lv_obj_add_state(ui_Pin1, LV_STATE_CHECKED);
	else if(pin_index == 3)
		lv_obj_add_state(ui_Pin2, LV_STATE_CHECKED);
	else if(pin_index == 4)
		lv_obj_add_state(ui_Pin3, LV_STATE_CHECKED);
}

void enter_btn_onc(){
	if(pin_index != 4)
		return;

	if(!password_reset_mode){
		if(user_login(&user, entered_pin)) {
			lv_label_set_text(ui_StatusLabel, "Success!");
			lv_obj_clear_state(ui_StatusLabel, LV_STATE_USER_1);
			reset();
			login_success_led_test();
		}else{
			lv_label_set_text(ui_StatusLabel, "Incorrect Password");
			lv_obj_add_state(ui_StatusLabel, LV_STATE_USER_1);
			reset();
		}
	}else{
		user_change_password(&user, entered_pin);

		lv_label_set_text(ui_StatusLabel, "Password Changed!");
		lv_obj_clear_state(ui_StatusLabel, LV_STATE_USER_1);
		password_reset_mode = false;
		reset();
		dashboard();
	}
}

void clr_btn_onc(){
	reset();
	lv_label_set_text(ui_StatusLabel, password_reset_mode ? "Enter New Password" : "Enter Password");
	lv_obj_clear_state(ui_StatusLabel, LV_STATE_USER_1);
}

void password_page_enter_reset_mode(void){
    password_reset_mode = true;
    lv_label_set_text(ui_StatusLabel, "Enter New Password");
    lv_obj_clear_state(ui_StatusLabel, LV_STATE_USER_1);
    reset();
    _ui_screen_change(&ui_PasswordPage, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_PasswordPage_screen_init);
}

static void pc5_hold_check(lv_timer_t *timer){
	if (lv_scr_act() == ui_PasswordPage || lv_scr_act() == ui_InitializePage){
		pc5_hold_ms   = 0;
		pc5_triggered = false;
		return;
	}
	// also skip if we're already in reset mode - no need to re-trigger
	if (password_reset_mode){
		pc5_hold_ms   = 0;
		pc5_triggered = false;
		return;
	}
    bool pressed = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == SET);
    if (pressed){
        pc5_hold_ms += PC5_CHECK_INTERVAL_MS;

        if (pc5_hold_ms >= PC5_HOLD_MS && !pc5_triggered){
            pc5_triggered = true;
            password_page_enter_reset_mode();
        }
    }
    else{
        pc5_hold_ms   = 0;
        pc5_triggered = false;
    }
}


void password_page_reset_listen(void){
    lv_timer_create(pc5_hold_check, PC5_CHECK_INTERVAL_MS, NULL);
}

