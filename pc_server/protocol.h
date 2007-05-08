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

/** Command event codes */
enum { UP, DOWN, LEFT, RIGHT } codes;

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



#endif
