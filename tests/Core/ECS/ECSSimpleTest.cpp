#include <gtest/gtest.h>

#include "ECS/EntityManager.hpp"

int Factorial(int num) 
{
    int result = 1;
    for (int i = 1; i <= num; i++)
    {
        result *= i;
    }
    
    return result;
}

// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(Factorial(0), 1);
}

// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(2), 2);
  EXPECT_EQ(Factorial(3), 6);
  EXPECT_EQ(Factorial(8), 40320);
}
