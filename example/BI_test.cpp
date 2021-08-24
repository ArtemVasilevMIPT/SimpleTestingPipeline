#include "TestCase.h"
#include "biginteger.h"

TEST_F(TestCase, SimpleTest)
{
	BigInteger a(1), b(2), c(3);
	EXPECT_EQ(a + b, c);
}
