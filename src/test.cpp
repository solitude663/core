#include "./core/base_inc.h"
#include "./core/base_inc.cpp"


internal void MainEntry(i32 argc, char** argv)
{
	LogInfo(0, "Hello World");
	LogError(0, "My name is Anthony");
	// LogPanic(0, "What is your name?");
}

int main(void)
{
	BaseMainThreadEntry(MainEntry, 0, 0);
	return 0;
}
