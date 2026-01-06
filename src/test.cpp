#include "./core/base_inc.h"
#include "./core/base_inc.cpp"

internal void PrintName(void* obj, String8 path, OS_FileInfo info)
{
	UnusedVariable(obj);
	UnusedVariable(info);
	printf("%.*s\n", Str8Print(path));
}

internal void MainEntry(i32 argc, char** argv)
{
	UnusedVariable(argc);
	UnusedVariable(argv);
#if 0
	M_Arena* arena = ArenaAllocDefault();
	String8 root_path = "/home/tonii";
	for(OS_FileIter iter = OS_FileIterMake(arena, root_path);
		OS_FileIterValid(iter);
		iter = OS_FileIterNext(arena, iter))
	{
		printf("Name: %.*s/%.*sSize: %llu\n",
			   Str8Print(root_path),
			   Str8Print(iter.Info.Name),
			   iter.Info.FileSize);
	}
#endif

	OS_PathWalk("/home/tonii", PrintName, 0);
}

int main(void)
{
	BaseMainThreadEntry(MainEntry, 0, 0);
	return 0;
}
