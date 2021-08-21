#pragma once

#include <gtest/gtest>

class TestCase : public ::testing::Test
{
public:
	static void SetUpTestCase();
	void SetUp() override;
	void TearDown() override;
};
