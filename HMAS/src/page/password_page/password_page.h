/*
 * password_page.h
 *
 *  Created on: Sep 8, 2026
 *      Author: User
 */

#ifndef PAGE_PASSWORD_PAGE_PASSWORD_PAGE_H_
#define PAGE_PASSWORD_PAGE_PASSWORD_PAGE_H_

#include "ui.h"
#include "usart/usart.h"
#include <stdio.h>
#include <stdbool.h>

void num_btn_onc(const char * num_str );
void enter_btn_onc(void);
void clr_btn_onc(void);
void password_page_reset_listen(void);

#endif /* PAGE_PASSWORD_PAGE_PASSWORD_PAGE_H_ */
