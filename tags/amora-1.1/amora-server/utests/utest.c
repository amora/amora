#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>
#include <stdlib.h>

#include "utest_dbus.h"
#include "utest_log.h"
#include "utest_loop.h"


static Suite *core_suite(void)
{
	Suite *s = suite_create("core");

#ifdef HAVE_DBUS
	suite_add_tcase(s, dbus_tcase_create());
#endif
	suite_add_tcase(s, log_tcase_create());
	suite_add_tcase(s, loop_tcase_create());

	return s;
}

int main(void)
{
	int number_failed;
	Suite *s = core_suite();
	SRunner *sr = srunner_create(s);

	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);

	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
