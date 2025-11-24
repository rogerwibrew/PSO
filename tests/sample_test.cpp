#include <gtest/gtest.h>

// Demonstrate that GoogleTest is working
TEST(SampleTest, BasicAssertions) {
    EXPECT_EQ(7 * 6, 42);
    EXPECT_TRUE(true);
}

TEST(SampleTest, FloatingPoint) {
    double a = 1.0;
    double b = 1.0;
    EXPECT_DOUBLE_EQ(a, b);
    EXPECT_NEAR(a, b, 1e-10);
}

TEST(SampleTest, StringOperations) {
    std::string str = "PSO";
    EXPECT_EQ(str.length(), 3);
    EXPECT_STREQ(str.c_str(), "PSO");
}
