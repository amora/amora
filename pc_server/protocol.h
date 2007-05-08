/**
 * @file   protocol.h
 * @author Adenilson Cavalcanti
 * @date   Tue May  8 09:08:59 2007
 *
 * @brief  Protocol handling functions/codes goes here.
 *
 * I'm still working on a protocol for handling events and data
 * exchange between the cellphone and PC.
 *
 */

#ifndef __PROTOCOL_PC__
#define __PROTOCOL_PC__

#include <string.h>

/** Command event codes */
enum { UP, DOWN, LEFT, RIGHT, NONE } codes;

/** Command event codes, what we expect to receive from
 * cell phone.
 */
char *cell_key_code[] = { "UP",
			  "DOWN",
			  "LEFT",
			  "RIGHT"
};

/** Special character to describe end of command */
const char CMD_BREAK = '\n';


/** Convert a cell event to correspondent X window event code.
 *
 *
 * @param event String with event/command.
 * @param length Length of string command.
 *
 * @return A code representing the event or NONE otherwise.
 * \todo: turn this code to a vector with loop... several if/elses are
 * not beauty.
 *
 */
int ecell_convert_ewindow(char *event, int length)
{
	int res = NONE;
	if (!strncasecmp(cell_key_code[UP], event, strlen(cell_key_code[UP])))
		res = UP;
	else if (!strncasecmp(cell_key_code[DOWN], event, strlen(cell_key_code[DOWN])))
		res = DOWN;
	else if (!strncasecmp(cell_key_code[LEFT], event, strlen(cell_key_code[LEFT])))
		res = LEFT;
	else if (!strncasecmp(cell_key_code[RIGHT], event, strlen(cell_key_code[RIGHT])))
		res = RIGHT;

	return res;
}

#endif
