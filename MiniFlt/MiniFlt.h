#ifndef __MINIFLT_H__
#define __MINIFLT_H__


//  Enabled warnings

#pragma warning(error:4100)     //  Enable-Unreferenced formal parameter
#pragma warning(error:4101)     //  Enable-Unreferenced local variable
#pragma warning(error:4061)     //  Eenable-missing enumeration in switch statement
#pragma warning(error:4505)     //  Enable-identify dead functions

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

#include "CommFunc.h"

#define TAG_CONTEXT       'FMTC'
#define MIN_SECTOR_SIZE 0x200

#define PROCESS_TERMINATE 0x1
#define ACTIVE_PROCESS_LINKS 0x2e8

typedef enum {
	DRIVE_FIXED = 0,
	DRIVE_NETWORK,
	DRIVE_REMOVABLE,
	DRIVE_UNKNOWN
} DRIVE_TYPE, *PDRIVE_TYPE;

typedef struct _VOLUME_CONTEXT {
	UNICODE_STRING VolumeName;
	ULONG SectorSize;

	FLT_FILESYSTEM_TYPE FileSystemType;
	DRIVE_TYPE DriveType;
} VOLUME_CONTEXT, *PVOLUME_CONTEXT;

typedef struct _MYMINIFLT_DATA {
	BOOL bInitialize;
} MYMINIFLT_DATA, *PMYMINIFLT_DATA;


#endif __MINIFLT_H__

