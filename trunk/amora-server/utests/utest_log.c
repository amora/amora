#define _GNU_SOURCE

#include <check.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "utest_log.h"
#include "log.c"


#define LOG_FILE "utest_logfile.log"

static struct log_resource *lres;

static void setup(void)
{
	/* This will make logging to stdout redirect to file */
	close(STDOUT_FILENO);

	lres = log_build_resources(LOG_FILE);
	fail_if(lres == NULL, "Can't create log resource!");
}

static void teardown(void)
{
	log_clean_resources(lres);
	fail_if(remove(LOG_FILE), "Can't remove logfile!");
}

START_TEST (test_log_build_resources)
{
	struct log_resource *p;
	struct stat buf;

	p = log_build_resources(NULL);
	fail_unless(p == NULL, "Creating log resource with NULL filename!");

	p = log_build_resources("/non-existent/file.i_hope");
	fail_unless(p == NULL, "Creating log resource with ghost file!");

	p = log_build_resources("/root/permission_denied");
	fail_unless(p == NULL, "Creating log resource with non-authorized file!");

	/* A real log file was created on setup, let's check if it is true */
	fail_if(stat(LOG_FILE, &buf), "Can't stat logfile!");
	fail_unless(buf.st_mode & S_IRUSR, "Can't open file for read!");
	fail_unless(buf.st_mode & S_IWUSR, "Can't open file for write!");
}
END_TEST

START_TEST (test_log_clean_resources)
{
	/* No return value, let's see if crashes with NULL parameter */
	log_clean_resources(NULL);
}
END_TEST

START_TEST (test_log_message)
{
	int ret;
	size_t len = 0;
	char buffer[100], *line = NULL, *foo = "Unit test";
	FILE *file;

	ret = log_message(FIL, lres, "%s log message to file!", foo);
	fail_unless(ret == 0, "Error while logging to file!");

	ret = log_message(OUT, lres, "%s log message to stdout!", foo);
	fail_unless(ret == 0, "Error while logging to stdout!");

	ret = log_message(666, lres, "%s log message to nowhere!", foo);
	fail_unless(ret == -1, "Logging to unknown output!");

	ret = log_message(OUT, NULL, "%s log message without resource!", foo);
	fail_unless(ret == -1, "Logging without resources!");

	ret = log_message(OUT, lres, NULL);
	fail_unless(ret == -1, "Logging without message!");

	file = fopen(LOG_FILE, "r");
	fail_if(file == NULL, "Can't open logfile for reading!");

	ret = getline(&line, &len, file);
	fail_if(ret == -1, "Can't read line on logfile!");
	snprintf(buffer, sizeof(buffer), "[Jan 00 00:00:00]: %s "
			"log message to file!\n", foo);
	fail_unless(strcmp(line, buffer) == 0, "Wrong message logged!");

	ret = getline(&line, &len, file);
	fail_if(ret == -1, "Can't read line on logfile!");
	snprintf(buffer, sizeof(buffer), "%s log message to stdout!\n", foo);
	fail_unless(strcmp(line, buffer) == 0, "Wrong message logged!");

	ret = getline(&line, &len, file);
	fail_unless(ret == -1, "Reading more lines than logged!");

	if (line)
		free(line);
}
END_TEST

START_TEST (test_get_timestamp)
{
	char buffer[100];
	char *expected = "Jan 00 00:00:00";
	char size = strlen(expected) + 1;

	memset(buffer, 0, sizeof(buffer));

	fail_unless(get_timestamp(NULL, size)   == -1, "Getting time with"
			" NULL buffer!");

	fail_unless(get_timestamp(NULL,   0)    == -1, "Getting time with"
			" NULL parameters!");

	fail_unless(get_timestamp(buffer, -1)   == -1, "Getting time with"
			" negative buffer size!");

	fail_unless(get_timestamp(buffer, 0)    == -1, "Getting time with"
			" 0 buffer size!");

	fail_unless(get_timestamp(buffer, 15)   == -1, "Getting time with"
			" buffer size smaller then timestamp!");

	fail_unless(get_timestamp(buffer, size) ==  0, "Returning error with"
			" sane parameters!");

	memset(buffer, 0, sizeof(buffer));

	/* We can predict the timestamp: see utest_fake.c for more details */
	get_timestamp(buffer, sizeof(buffer));
	fail_unless(strcmp("Jan 00 00:00:00", buffer) == 0, "Wrong timestamp!");
}
END_TEST

TCase *log_tcase_create(void)
{
	TCase *tc = tcase_create("log");
	tcase_add_checked_fixture(tc, setup, teardown);

	tcase_add_test(tc, test_log_build_resources);
	tcase_add_test(tc, test_log_clean_resources);
	tcase_add_test(tc, test_log_message);
	tcase_add_test(tc, test_get_timestamp);

	return tc;
}

