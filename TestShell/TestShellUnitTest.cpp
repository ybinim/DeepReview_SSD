#include "gmock/gmock.h"
#include "TestShell.cpp"

TEST(TestShellTestGroup, TestShellParamTest)
{
	TestShell shell;
	int ret;

	ret = shell.run("write 3 0xAAAAAAAA");
	EXPECT_EQ(ret, 0);

	ret = shell.run("read 3");
	EXPECT_EQ(ret, 0);

	ret = shell.run("help");
	EXPECT_EQ(ret, 0);

	ret = shell.run("fullwrite 0xBBBBBBBB");
	EXPECT_EQ(ret, 0);

	ret = shell.run("fullread");
	EXPECT_EQ(ret, 0);

	ret = shell.run("exit");
	EXPECT_EQ(ret, 1);
}