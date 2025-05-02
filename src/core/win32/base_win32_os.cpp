#include <windows.h>

internal void* OS_Reserve(u64 size)
{
	void* result = VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
	return result;
}

internal void OS_Commit(void* base, u64 size)
{
	VirtualAlloc(base, size, MEM_COMMIT, PAGE_READWRITE);
}

internal void OS_Release(void* base)
{
	VirtualFree(base, 0, MEM_RELEASE);
}
