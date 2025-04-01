#include "gmock/gmock.h"
#include <string>

using std::string;

TEST(SSDTestGroup, ValidCommandTest)
{
	SSD mySsd;
	int ret = mySsd.run("R 0");
	EXPECT_EQ(ret, true);

	ret = mySsd.run("W 0 0x12345678");
	EXPECT_EQ(ret, true);
}

TEST(SSDTestGroup, InvalidCommandTest)
{
	SSD mySsd;
	int ret = mySsd.run("D 0");
	EXPECT_EQ(ret, false);
}
