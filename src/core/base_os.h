#ifndef BASE_OS_H
#define BASE_OS_H

enum FlagType
{
	FT_Int,
	FT_Bool,
	FT_Float,
	FT_String,
	
	FT_IntPtr,
	FT_BoolPtr,
	FT_FloatPtr,
	FT_StringPtr,
};

struct Flag
{
	FlagType Type;
	String8 Name;
	String8 Usage;
	
	union
	{
		u64 IntValue;
		f64 FloatValue;
		String8 StringValue;
		b32 BoolValue;
		ptr_value PtrValue;
	};
};

// NOTE(afb) :: Memory Allocation
internal void* OS_Reserve(u64 size);
internal void  OS_Commit(void* base, u64 size);
internal void  OS_Release(void* base);

// NOTE(afb) :: Flag parsing
internal u64*	  OS_FlagInt(String8 name, u64 defaultValue, String8 usage);
internal void	  OS_FlagIntVal(u64* ptr, String8 name, u64 defaultValue, String8 usage);
internal f64*	  OS_FlagFloat(String8 name, f64 defaultValue, String8 usage);
internal void	  OS_FlagFloatVal(f64* ptr, String8 name, f64 defaultValue, String8 usage);
internal b32*	  OS_FlagBool(String8 name, b32 defaultValue, String8 usage);
internal void	  OS_FlagBoolVal(b32* ptr, String8 name, b32 defaultValue, String8 usage);
internal String8* OS_FlagString(String8 name, String8 defaultValue, String8 usage);
internal void	  OS_FlagIntVal(String8* ptr, String8 name, String8 defaultValue, String8 usage);
internal b8		  OS_FlagParse(u64 argc, char** argv);

struct OS_Handle
{
	ptr_value Handle;
};

enum OS_AccessFlags
{
	OS_AccessFlag_None		= 0,
	OS_AccessFlag_Read		= (1 << 0),
	OS_AccessFlag_Write		= (1 << 1),
	OS_AccessFlag_Shared	= (1 << 2),
	OS_AccessFlag_CreateNew = (1 << 3), // Creates new file no matter what
};

internal OS_Handle OS_FileOpen(String8 path, u64 flags);
internal String8   OS_FileRead(Arena* arena, OS_Handle handle);
internal String8   OS_FileReadAll(Arena* arena, String8 path);
internal b32       OS_FileWrite(OS_Handle file, String8 data, u64 offset);
internal void      OS_FileClose(OS_Handle file);
internal b32       OS_FileCopy(String8 dest, String8 src, b32 replace);
internal b32       OS_FileMove(String8 dest, String8 src, b32 replace);
internal b32       OS_FileDelete(String8 path);

enum OS_FileFlags
{
	OS_FileFlag_None      = (0 << 0),
	OS_FileFlag_Directory = (1 << 0),
};

struct OS_FileAttributes
{
	OS_FileFlags Flags;
};

struct OS_FileInfo
{
	String8 Name;
	u64 FileSize;	
	OS_FileFlags Flags;	
};

struct OS_FileIter
{
	OS_Handle Handle;
	OS_FileInfo Info;
};

internal b32         OS_DirectoryCreate(String8 path);
internal OS_FileIter OS_FileIterMake(Arena* arena, String8 path);
internal OS_FileIter OS_FileIterNext(Arena* arena, OS_FileIter iter);
internal void        OS_FileIterClose(OS_FileIter iter);
internal b32         OS_FileIterValid(OS_FileIter iter);

internal OS_FileInfo OS_GetFileInfo(String8 path);
internal u64	     OS_GetFileSize(String8 path);
internal b32	     OS_FileExists(String8 path);
internal b32	     OS_FileIsValid(OS_Handle file);

#endif // Header guard
