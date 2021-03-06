#ifndef __MINIFLT_H__
#define __MINIFLT_H__

//  Enabled warnings

#pragma warning(error:4100)     //  Enable-Unreferenced formal parameter
#pragma warning(error:4101)     //  Enable-Unreferenced local variable
#pragma warning(error:4061)     //  Eenable-missing enumeration in switch statement
#pragma warning(error:4505)     //  Enable-identify dead functions

#define MAX_KPATH 264 
#define MAX_NAME 64
#define MAX_USER_NAME 48
#define MAX_DATA_BUF 4096
#define BUF_UNIT 32768

#define IOCTL_LMR_DISABLE_LOCAL_BUFFERING 0x140390
#define IOCTL_QUERY_REMOTE_SERVER_NAME 0x140410
#define IOCTL_COPYCHUNK 0x140412

typedef unsigned int UINT;
typedef char CHAR, * PCHAR;
typedef unsigned char UCHAR, * PUCHAR;
typedef unsigned short USHORT;
typedef short SHORT, WORD;
typedef unsigned long ULONG, * PULONG, DWORD;
typedef unsigned char BYTE, * PBYTE;
typedef long BOOL, * PBOOL;

#include "CommFunc.h"

#define TAG_CONTEXT       'FMTC'
#define MIN_SECTOR_SIZE 0x200

#define PROCESS_TERMINATE 0x1
#define ACTIVE_PROCESS_LINKS 0x2e8

#define MINI_PORT_NAME     L"\\MiniFltPort"
#define MINI_LOG_PORT_NAME L"\\MiniFltLogPort"

#define RECORD_SIZE 4096
#define MAX_RECORDS_ALLOCATE 500

extern NPAGED_LOOKASIDE_LIST g_MiniFltLookaside;

typedef enum _MINIFLT_COMMAND {

	GET_LOG,
} MINIFLT_COMMAND;

typedef struct _COMMAND_MESSAGE {
	MINIFLT_COMMAND Command;
	ULONG Reserved;  // Alignment on IA64
	UCHAR Data[];
} COMMAND_MESSAGE, *PCOMMAND_MESSAGE;

typedef struct _MINI_GLOBAL_DATA {
	PFLT_FILTER hFilter;
	PDRIVER_OBJECT DriverObject;
	UNICODE_STRING RegistryPath;

	PFLT_PORT ServerPort;
	PFLT_PORT ClientPort;

	PFLT_PORT ServerLogPort;
	PFLT_PORT ClientLogPort;

	LONG MaxLogBufCnt;
	__volatile LONG LogBufCnt;

	__volatile LONG StaticBufInUse;
	PVOID OutOfMemoryBuf[RECORD_SIZE / sizeof(PVOID)];

	__volatile LONG LogSequenceNumber;

	LIST_ENTRY LogBufList;

	NPAGED_LOOKASIDE_LIST LogBufLookaside;

	BOOL bFirstInitLoad;
	BOOL bFirstLoadReg;

	KSPIN_LOCK SpinLock;

} MINI_GLOBAL_DATA, *PMINI_GLOBAL_DATA;

extern MINI_GLOBAL_DATA g_MiniData;

typedef enum {
	DRIVE_FIXED = 0,
	DRIVE_NETWORK,
	DRIVE_REMOVABLE,
	DRIVE_UNKNOWN
} DRIVE_TYPE, *PDRIVE_TYPE;

typedef struct _MINIFLT_INFO MINIFLT_INFO, *PMINIFLT_INFO;
struct _MINIFLT_INFO {
	LARGE_INTEGER Time;
	CHAR UserName[MAX_NAME];
	CHAR GroupName[MAX_NAME];
	CHAR FileName[MAX_KPATH];
	WCHAR FileNameW[MAX_KPATH];

	CHAR ProcName[MAX_KPATH];
	WCHAR ProcNameW[MAX_KPATH];

	CHAR RegPath[MAX_KPATH];
	WCHAR RegPathW[MAX_KPATH];

	BOOL bLocal;
	DRIVE_TYPE DrvType;
	FLT_FILESYSTEM_TYPE FSType;
	PCFLT_RELATED_OBJECTS FltObjects;
};

typedef struct _VOLUME_CONTEXT {
	UNICODE_STRING VolumeName;
	ULONG SectorSize;

	FLT_FILESYSTEM_TYPE FileSystemType;
	DRIVE_TYPE DriveType;
} VOLUME_CONTEXT, *PVOLUME_CONTEXT;

typedef struct _MYMINIFLT_DATA {
	BOOL bInitialize;
} MYMINIFLT_DATA, *PMYMINIFLT_DATA;

typedef union _FILE_REFERENCE {
	struct {
		ULONGLONG Value;
		ULONGLONG UpperZeroes;
	} FileId64;

	FILE_ID_128 FileId128;
} FILE_REFERENCE, * PFILE_REFERENCE;

DRIVER_INITIALIZE DriverEntry;
NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
);

NTSTATUS MiniFltUnload(
	_In_ FLT_FILTER_UNLOAD_FLAGS Flags
);

NTSTATUS MiniFltInstanceSetup(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_SETUP_FLAGS Flags,
	_In_ DEVICE_TYPE VolumeDeviceType,
	_In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType
);

NTSTATUS MiniFltInstanceQueryTeardown(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreCreate(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS MiniFltPostCreate(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Inout_opt_ PVOID CbdContext,
	_In_ FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreRead(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS MiniFltPostRead(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ PVOID CompletionContext,
	_In_ FLT_POST_OPERATION_FLAGS Flags
);

FLT_POSTOP_CALLBACK_STATUS MiniFltPostReadWhenSafe(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ PVOID CompletionContext,
	_In_ FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreWrite(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS MiniFltPostWrite(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ PVOID CompletionContext,
	_In_ FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreCleanup(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreSetSecurity(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreSetInformation(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreFileMapping(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreFsControl(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreDeviceControl(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_PREOP_CALLBACK_STATUS MiniFltPreMDLReadBuffers(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

ULONGLONG GetFileId(
	_In_ PCFLT_RELATED_OBJECTS FltObjects
);

PMINIFLT_INFO GetMiniFltInfo(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ PCHAR CallFuncName
);

PCHAR MakeFileName(
	_In_ PVOLUME_CONTEXT pVolContext,
	_In_ PFLT_FILE_NAME_INFORMATION NameInfo
);

PCHAR MakeFileNameByFileObj(
	_In_ PVOLUME_CONTEXT	pVolContext,
	_In_ PFILE_OBJECT FileObject
);

PMINIFLT_INFO GetNewMiniFltInfo(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects
);



#endif __MINIFLT_H__

