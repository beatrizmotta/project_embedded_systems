#include <ztest.h>
#include "camera_capture.h"

bool is_valid_plate(const char *plate);

ZTEST(radar_unit_tests, test_valid_plates)
{
    zassert_true(is_valid_plate("BRA4E23"), "Valid plate marked as invalid");
    zassert_true(is_valid_plate("ARG3G11"), "Valid plate marked as invalid");
    zassert_true(is_valid_plate("PRY8K76"), "Valid plate marked as invalid");
    zassert_true(is_valid_plate("PRY8K76"), "Valid plate marked as invalid");
}

ZTEST(radar_unit_tests, test_invalid_plates)
{
    zassert_false(is_valid_plate("1234567"), "Invalid plate marked as valid");
    zassert_false(is_valid_plate("BR1234"), "Invalid plate marked as valid");
    zassert_false(is_valid_plate("ZZZXXXX"), "Invalid plate marked as valid");
}

ZTEST_SUITE(radar_unit_tests, NULL, NULL, NULL, NULL, NULL);
