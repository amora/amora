#include <stdio.h>
#include <unistd.h>
#include "x11_event.h"

void foward_test(Display *active_display, int times);
void backward_test(Display *active_display, int times);

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
		usleep(500000);
		send_event(KeyRelease, x_key_code[RIGHT], active_display);
		sleep(2);
	}
}

void backward_test(Display *active_display, int times)
{
	printf("\t\tdoing backward test...\n");
	int i;
	for (i = 0; i < times; ++i) {
		send_event(KeyPress, x_key_code[LEFT], active_display);
		usleep(500000);
		send_event(KeyRelease, x_key_code[LEFT], active_display);
		sleep(2);
	}

}
