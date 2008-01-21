#include <check.h>
#include <stdlib.h>

#include "utest_loop.h"

static Suite *core_suite(void)
{
	Suite *s = suite_create("core");

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
