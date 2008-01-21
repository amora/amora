#include <check.h>
#include <fcntl.h>
#include <stdio.h>
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

static int fail_read(int fd)
{
	(void) fd;

	return -1;
}

static int return_fd_read(int fd)
{
	return fd;
}

static int zero_read(int fd)
{
	static int i = 0;
	static char buffer;

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

static int random_read(int fd)
{
	static int i = 0;
	static char buffer;

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
	loop_add(0, return_fd_read);
	fail_unless(dispatch(0) == 0, "Wrong callback!");

	loop_add(666, return_fd_read);
	fail_unless(dispatch(666) == 666, "Wrong callback!");

	loop_add(1023, return_fd_read);
	fail_unless(dispatch(1023) == 1023, "Wrong callback!");
}
END_TEST

START_TEST (test_is_empty)
{
	fail_unless(is_empty(&loop_set.readfds), "fdset must be empty"
			" in the first call!");

	loop_add(zero_fd, zero_read);
	loop_add(random_fd, random_read);
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

	fail_if(FD_ISSET(zero_fd, &loop_set.readfds), "fd already set!");
	ret = loop_add(zero_fd, zero_read);
	fail_unless(FD_ISSET(zero_fd, &loop_set.readfds), "loop_add() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_if(FD_ISSET(random_fd, &loop_set.readfds), "fd already set!");
	ret = loop_add(random_fd, random_read);
	fail_unless(FD_ISSET(random_fd, &loop_set.readfds), "loop_add() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_unless(loop_add(random_fd, random_read) == -1, "Adding fd twice!");
}
END_TEST

START_TEST (test_loop_remove)
{
	int ret;

	loop_add(zero_fd, zero_read);
	loop_add(random_fd, random_read);

	fail_unless(FD_ISSET(zero_fd, &loop_set.readfds), "fd not set!");
	ret = loop_remove(zero_fd);
	fail_if(FD_ISSET(zero_fd, &loop_set.readfds), "loop_remove() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_unless(FD_ISSET(random_fd, &loop_set.readfds), "fd not set!");
	ret = loop_remove(random_fd);
	fail_if(FD_ISSET(random_fd, &loop_set.readfds), "loop_remove() not working!");
	fail_unless(ret == 0, "Wrong return value!");

	fail_unless(loop_remove(random_fd) == -1, "Removing fd twice!");
}
END_TEST

START_TEST (test_loop_loop)
{
	loop_add(zero_fd, zero_read);
	loop_add(random_fd, random_read);
	fail_if(loop(), "Internal loop() error!");

	loop_add(zero_fd, fail_read);
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
	tcase_add_test(tc, test_loop_loop);

	return tc;
}

