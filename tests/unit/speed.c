#include <ztest.h>
#include "sensor.h"


ZTEST(speed_tests, test_speed_calc)
{
    zassert_equal(100, calculate_speed_kmh(1.0, 0.01), NULL);
    
    // edge cases
    zassert_equal(-1, calculate_speed_kmh(0, 1.0), NULL); // Zero distance
    zassert_equal(-1, calculate_speed_kmh(1.0, 0), NULL); // Zero time
    zassert_equal(-1, calculate_speed_kmh(-1.0, 1.0), NULL); // Negative distance
    zassert_equal(-1, calculate_speed_kmh(1.0, -1.0), NULL); // Negative time
}

ZTEST_SUITE(speed_tests, NULL, NULL, NULL, NULL, NULL);
