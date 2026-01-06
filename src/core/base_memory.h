#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

#define ARENA_ALLOCATION_GRANULARITY MB(64)
#define ARENA_COMMIT_GRANULARITY KB(4)
#define ARENA_DEFAULT_ALIGN 8

struct M_Arena
{
	u8* Base;
	u64 Size;
	u64 Used;
	u64 SizeCommited;
	u64 Align;

	// M_Arena* Next;
	// M_Arena* Next;
	// Realloc
	// u64 LastOffset;
	// u64 LastAllocSize;	
};

struct TempArena
{
	M_Arena* Arena;
	u64 Position;
};

internal M_Arena*	 ArenaAlloc(u64 size, u64 align);
internal M_Arena*	 ArenaAllocDefault();
internal void		 ArenaFree(M_Arena* arena);
internal void*		 ArenaPushNoZero(M_Arena* arena, u64 size);
internal void*		 ArenaPush(M_Arena* arena, u64 size);
// TODO internal void*	 ArenaRealloc(M_Arena* arena, u8* ptr, u64 size);
internal void		 ArenaClear(M_Arena* arena);

internal TempArena TempArenaBegin(M_Arena* arena);
internal void      TempArenaEnd(TempArena temp);
	
#endif // Header guard
