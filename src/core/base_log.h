#ifndef BASE_LOG_H
#define BASE_LOG_H

typedef enum LogMsgKind
{
	LogMsgKind_Info,
	LogMsgKind_Warning,
	LogMsgKind_Error,
	LogMsgKind_Panic,
	LogMsgKind_Count,
} LogMsgKind;

struct Logger
{
	Arena* Mem;

	String8List Messages[LogMsgKind_Count];
};

internal void LogMsg(Logger* logger, LogMsgKind kind, String8 msg);
internal void LogMsgF(Logger* logger, LogMsgKind kind, String8 format, ...);

#define LogInfo		LogMsg(l, LogMsgKind_Info, msg)
#define LogInfoF	LogMsgF(l, LogMsgKind_Info, format, __VA_ARGS__)

#define LogWarning	LogMsg(l, LogMsgKind_Warning, msg)
#define LogWarningF LogMsgF(l, LogMsgKind_Warning, format, __VA_ARGS__)

#define LogError	LogMsg(l, LogMsgKind_Error, msg)
#define LogErrorF	LogMsgF(l, LogMsgKind_Error, format, __VA_ARGS__)

#define LogPanic	LogMsg(l, LogMsgKind_Panic, msg)
#define LogPanicF	LogMsgF(l, LogMsgKind_Panic, format, __VA_ARGS__)

#endif // Header guard
