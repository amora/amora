#include <check.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utest_loop.h"
#include "loop.c"


#define MAX_BYTES 100

static int zero_fd, random_fd, count;

static void setup(void)
{
	zero_fd = open("/dev/zero", O_RDONLY);
	fail_if(zero_fd < 0, "Can't open device for testing.");

	random_fd = open("/dev/urandom", O_RDONLY);
	fail_if(random_fd < 0, "Can't open device for testing.");
}

static void teardown(void)
{
	close(zero_fd);
	close(random_fd);
}

static int fail_read(int fd, void *data)
{
	(void) fd;
	(void) data;

	return -1;
}

static int return_fd_read(int fd, void *data)
{
	(void) data;

	return fd;
}

static int check_userdata(int fd, void *data)
{
	(void) fd;

	return strcmp("userdata", (char *) data);
}

static int zero_read(int fd, void *data)
{
	static int i = 0;
	static char buffer;

	(void) data;

	i = i + read(fd, &buffer, 1);
	fail_unless(buffer == 0, "Reading !0 from /dev/zero. Wrong fd?");

	/* Incrase the global readed bytes counter */
	count++;

	if (i == MAX_BYTES) {
		loop_remove(zero_fd);
		fail_if(count < MAX_BYTES + MAX_BYTES / 2, "Not alternating"
				" between fds");
	}

	return 0;
}

static int random_read(int fd, void *data)
{
	static int i = 0;
	static char buffer;

	(void) data;

	i = i + read(fd, &buffer, 1);

	/* Incrase the global readed bytes counter */
	count++;

	if (i == MAX_BYTES) {
		loop_remove(random_fd);
		fail_if(count < MAX_BYTES + MAX_BYTES / 2, "Not alternating"
				" between fds");
	}

	return 0;
}

START_TEST (test_dispatch)
{
	loop_add(0, return_fd_read, NULL);
	fail_unless(dispatch(0) == 0, "Wrong callback!");

	loop_add(1, check_userdata, "userdata");
	fail_unless(dispatch(1) == 0, "Wrong userdata!");

	loop_add(2, check_userdata, "faildata");
	fail_if(dispatch(2) == 0, "Wrong userdata!");

	loop_add(666, return_fd_read, NULL);
	fail_unless(dispatch(666) == 666, "Wrong callback!");

	loop_add(FD_SETSIZE - 1, return_fd_read, NULL);
	fail_unless(dispatch(FD_SETSIZE - 1) == FD_SETSIZE - 1, "Wrong callback!");

	fail_unless(dispatch(FD_SETSIZE) == -1, "Dispatching fd out of the range!");
	fail_unless(dispatch(-1) == -1, "Dispatching fd out of the range!");
}
END_TEST

START_TEST (test_is_empty)
{
	fail_unless(is_empty(NULL) == -1, "Not returning error on NULL parameter!");

	fail_unless(is_empty(&loop_set.readfds), "fdset must be empty"
			" in the first call!");

	loop_add(zero_fd, zero_read, NULL);
	loop_add(random_fd, random_read, NULL);
	fail_if(is_empty(&loop_set.readfds), "fdset is empty!");

	loop_remove(zero_fd);
	fail_if(is_empty(&loop_set.readfds), "fdset is empty!");

	loop_remove(random_fd);
	fail_unless(is_empty(&loop_set.readfds), "fdset isn't empty!");
}
END_TEST

START_TEST (test_loop_add)
{
	int ret;

	fail_unless(loop_add(FD_SETSIZE, fail_read, NULL) == -1, "Adding fd out of the range!");
	fail_unless(loop_add(-1, fail_read, NULL) == -1, "Adding fd out of the range!");
	fail_unless(loop_add(1, NULL, NULL) == -1, "Adding fd with NULL callback!");

	fail_if(FD_ISSET(zero_fd, &loop_set.readfds), "fd already set!");
	ret = loop_add(zero_fd, zero_read, NULL);
	fail_unless(FD_ISSET(zero_fd, &loop_set.readfds), "loop_add() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_if(FD_ISSET(random_fd, &loop_set.readfds), "fd already set!");
	ret = loop_add(random_fd, random_read, NULL);
	fail_unless(FD_ISSET(random_fd, &loop_set.readfds), "loop_add() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_unless(loop_add(random_fd, random_read, NULL) == -1, "Adding fd twice!");
}
END_TEST

START_TEST (test_loop_remove)
{
	int ret;

	loop_add(zero_fd, zero_read, "amora1");
	loop_add(random_fd, random_read, "amora2");

	fail_unless(loop_remove(FD_SETSIZE) == -1, "Removing fd out of the range!");
	fail_unless(loop_remove(-1) == -1, "Removing fd out of the range!");

	fail_unless(FD_ISSET(zero_fd, &loop_set.readfds), "fd not set!");
	fail_unless(strcmp(loop_set.data[zero_fd], "amora1") == 0, "Userdata not set!");
	ret = loop_remove(zero_fd);
	fail_if(FD_ISSET(zero_fd, &loop_set.readfds), "loop_remove() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_unless(FD_ISSET(random_fd, &loop_set.readfds), "fd not set!");
	fail_unless(strcmp(loop_set.data[random_fd], "amora2") == 0, "Userdata not set!");
	ret = loop_remove(random_fd);
	fail_if(FD_ISSET(random_fd, &loop_set.readfds), "loop_remove() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_unless(loop_remove(random_fd) == -1, "Removing fd twice!");
}
END_TEST

START_TEST (test_loop)
{
	loop_add(zero_fd, zero_read, NULL);
	loop_add(random_fd, random_read, NULL);
	fail_if(loop(), "Internal loop() error!");

	loop_add(zero_fd, fail_read, NULL);
	fail_unless(loop() == -1, "Not return error on fail!");
}
END_TEST

TCase *loop_tcase_create(void)
{
	TCase *tc = tcase_create("loop");
	tcase_add_checked_fixture(tc, setup, teardown);

	tcase_add_test(tc, test_dispatch);
	tcase_add_test(tc, test_is_empty);
	tcase_add_test(tc, test_loop_add);
	tcase_add_test(tc, test_loop_remove);
	tcase_add_test(tc, test_loop);

	return tc;
}

