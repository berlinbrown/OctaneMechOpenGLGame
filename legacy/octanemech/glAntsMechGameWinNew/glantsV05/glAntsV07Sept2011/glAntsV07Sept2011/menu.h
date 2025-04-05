//
// menu.h
//
#ifndef _MENU_H_
#define _MENU_H_

// menu mode
#define MENU_TITLE_MODE			1
#define MENU_RUN_MODE			2
#define MENU_HELP_MODE			3

// These are used with _menu_state
#define MENU_DEAD_MODE			4
#define FIRST_TIME_TRUE			1
#define FIRST_TIME_FALSE		2

void Toggle_MenuItems(int dir);
bool Set_MenuMode(void);
void Reset_DeadText(void);

#endif