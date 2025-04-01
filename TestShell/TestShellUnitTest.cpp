#include "gmock/gmock.h"

TEST(TestShellTestGroup, TestShellParamTest)
{
	TestShell shell;
	int ret;
	ret = shell.run("read 3");
	EXPECT_EQ(ret, 0);
}