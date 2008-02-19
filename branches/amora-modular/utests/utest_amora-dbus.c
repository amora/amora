#include <check.h>

#include "utest_amora-dbus.h"
#include "amora-dbus.c"


static void setup(void)
{
	return;
}


static void teardown(void)
{
	return;
}


START_TEST (test_dbus)
{
	return;
}
END_TEST


TCase *dbus_tcase_create(void)
{
	TCase *tc = tcase_create("dbus");
	tcase_add_checked_fixture(tc, setup, teardown);

	tcase_add_test(tc, test_dbus);

	return tc;
}

