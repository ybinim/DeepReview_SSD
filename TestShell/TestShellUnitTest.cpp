#include "gmock/gmock.h"
#include "TestShell.cpp"

TEST(TestShellTestGroup, TestShellParamTest)
{
	TestShell shell;
	int ret;
	ret = shell.run("read 3");
	EXPECT_EQ(ret, 0);
}