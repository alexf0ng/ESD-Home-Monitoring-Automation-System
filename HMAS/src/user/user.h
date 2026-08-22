/*
 * user.h
 *
 *  Created on: Aug 22, 2026
 *      Author: user
 *
 *      Description: handle user authentication
 */

#ifndef USER_H_
#define USER_H_

#include <stdbool.h>

typedef struct {
	char user_password[5];
	bool user_logged_in;
} User;

void user_init(User *user);
bool user_login(User *user, const char *password);
void user_logout(User *user);
void user_change_password(User *user, const char *new_password);



#endif /* USER_H_ */
