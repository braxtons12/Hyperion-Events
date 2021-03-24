#include <gtest/gtest.h>

auto main(int argc, char** argv) noexcept -> int {
	testing::InitGoogleTest(&argc, argv);
	testing::FLAGS_gtest_death_test_style = "fast";
	return RUN_ALL_TESTS();
}
