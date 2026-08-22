/*
 * user.c
 *
 *  Created on: Aug 22, 2026
 *      Author: user
 */
#include "user.h"
#include <string.h>
#include <stdbool.h>

void user_init(User *user)
{
    strcpy(user->user_password, "4896");

    user->user_logged_in = false;
}

bool user_login(User *user, const char *password)
{
    if (strcmp(user->user_password, password) == 0)
    {
        user->user_logged_in = true;
        return true;
    }

    return false;
}

void user_logout(User *user)
{
    user->user_logged_in = false;
}

void user_change_password(User *user, const char *new_password)
{
    strcpy(user->user_password, new_password);
}
