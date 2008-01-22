/* Fake replacements of standard C library functions */

#include <time.h>

struct tm *localtime(const time_t *timep) {
	static struct tm fake_time;

	(void) timep;

	if(!timep)
		return NULL;

	return &fake_time;
}

