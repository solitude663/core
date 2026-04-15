#if OS_WINDOWS
#include "win32/base_win32_os.cpp"
#elif OS_LINUX
#include "linux/base_linux_os.cpp"
#else
#error OS not supported
#endif

#ifndef OS_FLAG_CAPACITY
#define OS_FLAG_CAPACITY 256
#endif

global Flag Flags[OS_FLAG_CAPACITY];
global u64 FlagCount;

internal u64* OS_FlagInt(String8 name, u64 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_Int;
	Flags[current].Name = name;
	Flags[current].IntValue = default_value;
	Flags[current].Usage = usage;
	return &Flags[current].IntValue;
}

internal void OS_FlagIntVar(u64* ptr, String8 name, u64 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_IntPtr;
	Flags[current].Name = name;
	Flags[current].PtrValue = (ptr_value)ptr;
	Flags[current].Usage = usage;
	*ptr = default_value;
}

internal f64* OS_FlagFloat(String8 name, f64 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_Float;
	Flags[current].Name = name;
	Flags[current].FloatValue = default_value;
	Flags[current].Usage = usage;
	return &Flags[current].FloatValue;
}

internal void OS_FlagFloatVar(f64* ptr, String8 name, f64 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_FloatPtr;
	Flags[current].Name = name;
	Flags[current].PtrValue = (ptr_value)ptr;
	Flags[current].Usage = usage;
	*ptr = default_value;
}

internal b32* OS_FlagBool(String8 name, b32 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_Bool;
	Flags[current].Name = name;
	Flags[current].BoolValue = default_value;
	Flags[current].Usage = usage;
	return &Flags[current].BoolValue;
}

internal void OS_FlagBoolVar(b32* ptr, String8 name, b32 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_BoolPtr;
	Flags[current].Name = name;
	Flags[current].PtrValue = (ptr_value)ptr;
	Flags[current].Usage = usage;
	*ptr = default_value;
}

internal String8* OS_FlagString(String8 name, String8 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_String;
	Flags[current].Name = name;
	Flags[current].StringValue = default_value;
	Flags[current].Usage = usage;
	return &Flags[current].StringValue;
}

internal void OS_FlagStringVar(String8* ptr, String8 name, String8 default_value, String8 usage)
{
	Assert((FlagCount + 1) < ArrayCount(Flags));
	u64 current = FlagCount++;

	Flags[current].Kind = Flag_StringPtr;
	Flags[current].Name = name;
	Flags[current].PtrValue = (ptr_value)ptr;
	Flags[current].Usage = usage;
	*ptr = default_value;
}

internal Flag* OS__GetFlagForArg(String8 arg)
{
	Flag* result = 0;

	for(u32 i = 0; i < FlagCount; i++)
	{
		if(Suffix8(arg, arg.Length - 2) == Flags[i].Name)
		{
			result = &Flags[i];
			break;
		}
	}
	
	return result;
}

internal u32 OS_FlagParse(u64 arg_count, char** argv)
{
	u64 arg_index = 1;
	for(; arg_index < arg_count; arg_index++)
	{
		Flag* flag = OS__GetFlagForArg(Str8C(argv[arg_index]));
		if(!flag) break;
		
		b32 has_next = (arg_index + 1) < arg_count;

		switch(flag->Kind)
		{
			case(Flag_Int):
			case(Flag_IntPtr):
			{
				if(!has_next) return 0;

				String8 arg_value = Str8C(argv[arg_index + 1]);
				u64 value = U64FromStr8(arg_value);
					
				if(flag->Kind == Flag_Int) flag->IntValue = value;
				else *(u64*)flag->PtrValue = value;
			}break;

			case(Flag_Float):
			case(Flag_FloatPtr):
			{
				if(!has_next) return 0;

				String8 arg_value = Str8C(argv[arg_index + 1]);
				f64 value = F64FromStr8(arg_value);
					
				if(flag->Kind == Flag_Float) flag->FloatValue = value;
				else *(f64*)flag->PtrValue = value;
			}break;

			case(Flag_String):
			case(Flag_StringPtr):
			{
				if(!has_next) return 0;
				String8 value = Str8C(argv[arg_index + 1]);

				if(flag->Kind == Flag_String) flag->StringValue = value;
				else *(String8*)flag->PtrValue = value;
			}break;

			case(Flag_Bool):
			case(Flag_BoolPtr):
			{
				b32 value = 0;
				if(has_next)
				{
					String8 arg_value = Str8C(argv[arg_index + 1]);						
					if(arg_value.Str[0] == '-')
					{
						value = 1;
					}
					else
					{
						if(Str8Match(arg_value, "true", MF_IgnoreCase))
							value = 1;
						else if(Str8Match(arg_value, "false", MF_IgnoreCase))
							value = 0;
						else
						{
							value = 1;							
						}
					}
				}
				else
				{
					value = 1;
				}
					
				if(flag->Kind == Flag_Bool) flag->BoolValue = value;
				else *(b32*)flag->PtrValue = value;
					
			}break;

			default:
			{
				Assert(0 && "Should not be possible\n");
			}
		}
	}
	
	return arg_index;
}

internal u32 OS_FlagParse2(u64 argc, char** argv)
{
	u32 result = 0;
	
	String8 program_name = Str8C(argv[0]);
	UnusedVariable(program_name);

	u64 arg_index = 1;
	for(u64 flag_id = 0; flag_id < FlagCount; flag_id++)
	{
		Flag* flag = &Flags[flag_id];
		for(; arg_index < argc; arg_index++)
		{
			if(!Str8Match(Prefix8(flag->Name, 2), "--", MF_None))
			{
				result = arg_index;
				return result;
			}
			
			if(!Str8Match(flag->Name, argv[arg_index], MF_None))
			{
				continue;			
			}

			b8 has_next_index = (arg_index + 1) < argc;
			switch(flag->Kind)
			{
				case(Flag_Int):
				case(Flag_IntPtr):
				{
					if(!has_next_index) return 0;
					String8 strValue = Str8C(argv[arg_index + 1]);
					u64 value = U64FromStr8(strValue);
					
					if(flag->Kind == Flag_Int) flag->IntValue = value;
					else *(u64*)flag->PtrValue = value;
				}break;

				case(Flag_Float):
				case(Flag_FloatPtr):
				{
					if(!has_next_index) return 0;
					String8 strValue = Str8C(argv[arg_index + 1]);
					f64 value = F64FromStr8(strValue);

					if(flag->Kind == Flag_Float) flag->FloatValue = value;
					else *(f64*)flag->PtrValue = value;
				}break;

				
				case(Flag_Bool):
				case(Flag_BoolPtr):
				{
					b32 value = 0;
					if(has_next_index)
					{
						String8 strValue = Str8C(argv[arg_index + 1]);						
						if(strValue.Str[0] == '-')
						{
							value = 1;
						}
						else
						{
							if(Str8Match(strValue, "true", MF_IgnoreCase))
								value = 1;
							else if(Str8Match(strValue, "false", MF_IgnoreCase))
								value = 0;
							else
								return 0;						
						}
					}
					else
					{
						value = 1;
					}
					
					if(flag->Kind == Flag_Bool) flag->BoolValue = value;
					else *(b32*)flag->PtrValue = value;
					
				}break;

				case(Flag_String):
				case(Flag_StringPtr):
				{
					if(!has_next_index) return 0;
					String8 strValue = Str8C(argv[arg_index + 1]);

					if(flag->Kind == Flag_String) flag->StringValue = strValue;
					else *(String8*)flag->PtrValue = strValue;
				}break;

				
				default:
				{
					Assert(0);
				}
			}
		}
	}

	result = arg_index;
	return result;
}

internal String8 OS_FileExt(String8 path)
{
	String8 result = {0};
	u64 index = Str8FindLast(path, ".");
	if(index != path.Length)
	{
		result = Substr8(path, index, path.Length - index);
	}
	return result;
}

internal String8 OS_Dir(String8 path)
{
	String8 result = path;
	u64 index = Str8FindLast(path, OS_PATH_SEPARATOR);
	if(index != path.Length)
	{
		result = Substr8(path, 0, index);
	}
	return result;
}


internal String8Array OS_DirFiles(M_Arena* arena, String8 path)
{
	TempArena temp = GetScratch(arena);
	String8List list = {};

	String8 search_path = OS_PathConcat(temp.Arena, path, "*");
	for(OS_FileIter iter = OS_FileIterMake(temp.Arena, search_path);
		OS_FileIterValid(iter);
		iter = OS_FileIterNext(temp.Arena,  iter))
	{
		Str8ListPush(temp.Arena, &list, iter.Info.Name);
	}

	String8Array result = Str8ListToArray(arena, list);	
	ReleaseScratch(temp);
	
	return result;
}

// NOTE(afb) :: Path
internal String8 OS_PathNormalize(M_Arena* arena, String8 file_path)
{	
	String8 result = {};
	TempArena temp = GetScratch(arena);

	String8 path_sep = OS_PATH_SEPARATOR;
	String8 bad_path_sep = OS_WRONG_PATH_SEPARATOR;

	String8 path = Str8Replace(temp.Arena, file_path, bad_path_sep, path_sep);
	String8Array path_parts = Str8ListToArray(temp.Arena,
											  Str8Split(temp.Arena, path, path_sep));

	u32* parts_to_save = PushArray(temp.Arena, u32, path_parts.Count);
	u64 current = 0;
	b32 last_is_back = false;
	
	for(u32 i = 0; i < path_parts.Count; i++)
	{
		if(Str8Match("..", path_parts.Values[i], MF_None))
		{
			if(current && !last_is_back) current--;
			else
			{
				parts_to_save[current++] = i;
			}
			last_is_back = true;
		}
		else if(Str8Match(".", path_parts.Values[i], MF_None))
		{
			last_is_back = false;
			if((i+1) >= path_parts.Count) parts_to_save[current++] = i;
		}
		else
		{
			parts_to_save[current++] = i;
			last_is_back = false;
		}
	}

	String8List sb = {};
	for(u32 i = 0; i < current; i++)
	{
		u32 index = parts_to_save[i];
		Str8ListPush(temp.Arena, &sb, path_parts.Values[index]);
	}

		// TODO(afb) :: Optimize for linux
#if OS_WINDOWS
	result = Str8Join(arena, sb, path_sep);
#else
	String8 temp_result = Str8Join(temp.Arena, sb, path_sep);
	result = Str8Concat(arena, "/", temp_result);
#endif
	
	ReleaseScratch(temp);
	return result;
}

internal String8 OS_PathConcat(M_Arena* arena, String8 a, String8 b) // TODO(afb) :: Varadic 
{
	TempArena temp = GetScratch(arena);
	if((a.Length && a.Str[a.Length-1] != OS_PATH_SEPARATOR[0]) &&
	   (b.Length && b.Str[0] != OS_PATH_SEPARATOR[0]))
		a = Str8Concat(temp.Arena, a, OS_PATH_SEPARATOR);

    String8 result = Str8Concat(arena, a, b);
	result = OS_PathNormalize(arena, result);
	ReleaseScratch(temp);
	return result;
}


internal b32 OS_PathExists(String8 path)
{
	OS_FileInfo info = OS_GetFileInfo(path);
	b32 result = IsDirectory(info.Flags);
	return result;
}

internal void OS__PathWalkHelper(String8 path, WalkFunc proc, void* obj)
{
	TempArena temp = GetScratch(0);

#if OS_WINDOWS
	String8 raw_path = Substr8(path, 0, path.Length-1);
#else
	String8 raw_path = path;
#endif
	
	for(OS_FileIter iter = OS_FileIterMake(temp.Arena, path);
		OS_FileIterValid(iter);
		iter = OS_FileIterNext(temp.Arena, iter))
	{
		OS_FileInfo info = iter.Info;
		String8 filename = OS_PathConcat(temp.Arena, raw_path, info.Name);

		if(Str8Match(info.Name, ".", MF_None) || Str8Match(info.Name, "..", MF_None))
			continue;
		
		proc(obj, filename, info);

		if(IsDirectory(info.Flags))
		{
			// String8 search_path = OS_PathConcat(temp.Arena, filename, "*");
			// OS_PathWalkHelper(search_path, proc, obj);
			// TODO(afb) :: Call the helper. Don't allocate new string
			OS_PathWalk(filename, proc, obj);
		}
		
	}

	ReleaseScratch(temp);
}

internal b32 OS_PathWalk(String8 path, WalkFunc proc, void* obj)
{
	if(!OS_PathExists(path)) return false;
	TempArena temp = GetScratch(0);

#if OS_WINDOWS	
	String8 search_path = OS_PathConcat(temp.Arena, path, "*");
#else
	String8 search_path = path;
#endif
	OS__PathWalkHelper(search_path, proc, obj);

	ReleaseScratch(temp);
	return true;
}
