#ifndef BASE_OS_H
#define BASE_OS_H

#if OS_WINDOWS
#define OS_PATH_SEPARATOR "\\"
#define OS_WRONG_PATH_SEPARATOR "/"
#endif

#if OS_LINUX || OS_MACOS
#define OS_PATH_SEPARATOR "/"
#define OS_WRONG_PATH_SEPARATOR "\\"
#endif

enum FlagKind
{
	Flag_Int,
	Flag_Bool,
	Flag_Float,
	Flag_String,	

	Flag_IntPtr,
	Flag_BoolPtr,
	Flag_FloatPtr,
	Flag_StringPtr,
};

struct Flag
{
	FlagKind Kind;
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
// TODO(afb) :: internal void  OS_Decommit(void* base, u64 size); 
internal void* OS_Reserve(u64 size);
internal void  OS_Commit(void* base, u64 size);
internal void  OS_Release(void* base, u64 size = 0);

// NOTE(afb) :: Flag parsing
internal u64*	  OS_FlagInt(String8 name, u64 default_value, String8 usage);
internal f64*	  OS_FlagFloat(String8 name, f64 default_value, String8 usage);
internal b32*	  OS_FlagBool(String8 name, b32 default_value, String8 usage);
internal String8* OS_FlagString(String8 name, String8 default_value, String8 usage);

internal void	  OS_FlagIntVar(u64* ptr, String8 name, u64 default_value, String8 usage);
internal void	  OS_FlagFloatVar(f64* ptr, String8 name, f64 default_value, String8 usage);
internal void	  OS_FlagBoolVar(b32* ptr, String8 name, b32 default_value, String8 usage);
internal void	  OS_FlagStringVar(String8* ptr, String8 name, String8 default_value, String8 usage);

internal u32	  OS_FlagParse(u64 argc, char** argv);

// NOTE(afb) :: File and directory manipulation
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
internal void      OS_FileClose(OS_Handle file);
internal b32       OS_FileWrite(OS_Handle file, String8 data, u64 offset = 0);
internal b32       OS_FileWrite(OS_Handle file, String8List data, u64 offset);
internal String8   OS_FileRead(M_Arena* arena, OS_Handle handle, u64 start, u64 count);
internal String8   OS_FileReadAll(M_Arena* arena, String8 path);
internal b32       OS_FileCopy(String8 dest, String8 src, b32 replace);
internal b32       OS_FileMove(String8 dest, String8 src, b32 replace);
internal b32       OS_FileDelete(String8 path);
internal String8   OS_FileExt(String8 path);

#define IsDirectory(flags) (((flags) & OS_FileFlag_Directory) == OS_FileFlag_Directory)
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


internal String8	  OS_Dir(String8 path);
internal b32		  OS_DirCreate(String8 path);
internal String8Array OS_DirFiles(M_Arena* arena, String8 path);

internal String8 GetWorkingDirectory(M_Arena* arena);


internal OS_FileIter OS_FileIterMake(M_Arena* arena, String8 path);
internal OS_FileIter OS_FileIterNext(M_Arena* arena, OS_FileIter iter);
internal void        OS_FileIterClose(OS_FileIter iter);
internal b32         OS_FileIterValid(OS_FileIter iter);

// internal OS_FileInfo OS_GetFileInfo(String8 path);
internal u64	     OS_GetFileSize(String8 path);
internal b32	     OS_FileExists(String8 path);
internal b32	     OS_FileIsValid(OS_Handle file);

// NOTE(afb) :: Process Manipulation
// TODO(afb) :: Add arguments
#if OS_WINDOWS
internal String8 OS_RunCommand(M_Arena* arena, String8 commandToExecute); 
#endif

// NOTE(afb) :: Path
internal String8 OS_PathNormalize(M_Arena* arena, String8 file_path);
internal String8 OS_PathConcat(M_Arena* arena, String8 a, String8 b);
internal b32     OS_PathExists(String8 a);

typedef void (*WalkFunc)(void* obj, String8 path, OS_FileInfo info);
internal b32 OS_PathWalk(String8 path, WalkFunc proc, void* obj);

#endif // Header guard
