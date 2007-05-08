/**
 * @file   main.c
 * @author Adenilson Cavalcanti
 * @date   Fri May  4 14:25:09 2007
 *
 * @brief  Main server side presenter app.
 *
 * This app is reponsible to receive events from cellphone (in
 * near future...) and send them to X Server.
 * \todo
 * - communicate with cellphone using BlueZ.
 * - screenshot from active window.
 * - autotools buildsystem.
 * - logging feature.
 */

#include <stdio.h>
#include <unistd.h>
#include "x11_event.h"
#include "protocol.h"
#include "bluecode.h"

/** Tests for forward key event.
 *
 *
 * @param active_display Pointer to active display.
 * @param times How many times to advance (send event right arrow).
 */
void foward_test(Display *active_display, int times);

/** Tests for backward key event.
 *
 *
 * @param active_display Pointer to active display.
 * @param times How many times to advance (send event left arrow).
 */
void backward_test(Display *active_display, int times);


/** Main app function.
 *
 *
 * @param argc Number of arguments.
 * @param argv Arguments values.
 *
 * @return 0 on sucess, -1 on error.
 */
int main(int argc, char* argv[])
{
	Display *own_display = NULL;
	int res = 0, times = 5;
	printf("testing... put your slide show on!\n");
	sleep(10);

	own_display = construct_display(NULL);
	if (!own_display) {
		printf("Error creating display object! Aborting...\n");
		return -1;
	}

	foward_test(own_display, times);
	backward_test(own_display, times);

	res = destroy_display(own_display);
	printf("Done, we are closing now.\n");
	return 0;
}

void foward_test(Display *active_display, int times)
{
	printf("\t\tdoing forward test...\n");
	int i;
	for (i = 0; i < times; ++i) {
		send_event(KeyPress, x_key_code[RIGHT], active_display);
		sleep(2);
	}
}

void backward_test(Display *active_display, int times)
{
	printf("\t\tdoing backward test...\n");
	int i;
	for (i = 0; i < times; ++i) {
		send_event(KeyPress, x_key_code[LEFT], active_display);
		sleep(2);
	}

}
