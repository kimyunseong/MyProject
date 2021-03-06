#include "AccessControl.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <queue>

using namespace std;

template <typename Out>
void mySplit(const string& s, char delim, Out result) {
	istringstream iss(s);
	string item;
	while (getline(iss, item, delim)) {
		*result++ = item;
	}
}

vector<string> split(const string& s, char delim) {
	vector<string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

enum class TYPE_SUBJECT {
	SUBJECT_USER = 0,
	SUBJECT_GROUP,
	SUBJECT_PROC,
	SUBJECT_UNKNOWN
};

const string SUBJECT_USER_STR = "user";
const string SUBJECT_GROUP_STR = "group";
const string SUBJECT_PROC_STR = "proc";


TYPE_SUBJECT AccessControl::StrToSubType(
	string SubTypeStr
)
{
	if (!SubTypeStr.compare(SUBJECT_USER_STR)) return TYPE_SUBJECT::SUBJECT_USER;
	else if (!SubTypeStr.compare(SUBJECT_GROUP_STR)) return TYPE_SUBJECT::SUBJECT_GROUP;
	else if (!SubTypeStr.compare(SUBJECT_PROC_STR)) return TYPE_SUBJECT::SUBJECT_PROC;

	return TYPE_SUBJECT::SUBJECT_UNKNOWN;
}

string AccessControl::SubTypeToStr(
	TYPE_SUBJECT SubType
)
{
	if (SubType == TYPE_SUBJECT::SUBJECT_USER) return SUBJECT_USER_STR;
	if (SubType == TYPE_SUBJECT::SUBJECT_GROUP) return SUBJECT_GROUP_STR;
	else if (SubType == TYPE_SUBJECT::SUBJECT_PROC) return SUBJECT_PROC_STR;

	return "";
}

enum class TYPE_OBJECT {
	OBJECT_FILE = 0,
	OBJECT_DIR,
	OBJECT_UNKNOWN
};

const string OBJECT_FILE_STR = "file";
const string OBJECT_DIR_STR = "dir";

TYPE_OBJECT AccessControl::StrToObjType(
	string ObjTypeStr
)
{
	if (!ObjTypeStr.compare(OBJECT_FILE_STR)) return TYPE_OBJECT::OBJECT_FILE;
	else if (!ObjTypeStr.compare(OBJECT_DIR_STR)) return TYPE_OBJECT::OBJECT_DIR;

	return TYPE_OBJECT::OBJECT_UNKNOWN;
}

string AccessControl::ObjTypeToStr(
	_In_ TYPE_OBJECT ObjType
)
{
	if (ObjType == TYPE_OBJECT::OBJECT_FILE) return OBJECT_FILE_STR;
	else if (ObjType == TYPE_OBJECT::OBJECT_DIR) return OBJECT_DIR_STR;

	return "";
}

enum class EFFECT_MODE {
	EFT_DENY = 0,
	EFT_ALLOW,
	EFT_UNKNOWN
};

const string EFT_ALLOW_STR = "allow";
const string EFT_DENY_STR = "deny";
const string EFT_UNKNOWN_STR = "unknown";

EFFECT_MODE AccessControl::StrToEffectMode(
	_In_ string EffectModeStr
)
{
	if (EffectModeStr.compare(EFT_ALLOW_STR)) return EFFECT_MODE::EFT_ALLOW;
	else if (EffectModeStr.compare(EFT_DENY_STR)) return EFFECT_MODE::EFT_DENY;

	return EFFECT_MODE::EFT_UNKNOWN;
}

string AccessControl::EffectModeToStr(
	_In_ EFFECT_MODE EffectMode
)
{
	if (EffectMode == EFFECT_MODE::EFT_ALLOW) return EFT_ALLOW_STR;
	else if (EffectMode == EFFECT_MODE::EFT_DENY) return EFT_DENY_STR;

	return EFT_UNKNOWN_STR;
}

enum class ACL_ACTION {
	ACTION_UNKNOWN = 0,
	ACTION_READ = 0x00000001,
	ACTION_WRITE = 0x00000002,
	ACTION_TRAVERSE = 0x0000000,
	ACTION_DELETE = 0x00000008,
	ACTION_CREATE = 0x00000010,
	ACTION_EXECUTE = 0x00000020,
	ACTION_RENAME = 0x00000040,
	ACTION_KEY_CREATE = 0x00000100,
	ACTION_KEY_DELETE = 0x00000200,
	ACTION_VALUE_WRITE = 0x00000400,
	ACTION_VALUE_DELETE = 0x00000800,
	ACTION_ALL = 0xFFFFFFFF
};

const string ACTION_READ_STR = "read";
const string ACTION_WRITE_STR = "write";
const string ACTION_TRAVERSE_STR = "traverse";
const string ACTION_DELETE_STR = "delete";
const string ACTION_CREATE_STR = "create";
const string ACTION_EXECUTE_STR = "execute";
const string ACTION_RENAME_STR = "rename";
const string ACTION_KEY_CREATE_STR = "keycreate";
const string ACTION_KEY_DELETE_STR = "keydel";
const string ACTION_VALUE_WRITE_STR = "valwrite";
const string ACTION_VALUE_DELETE_STR = "valdel";
const string ACTION_ALL_STR = "all";

ACL_ACTION AccessControl::StrToAction(
	_In_ string ActionStr
)
{
	ULONG Action = 0;

	if (!ActionStr.empty()) return ACL_ACTION::ACTION_UNKNOWN;

	if (ActionStr.compare(ACTION_ALL_STR)) return ACL_ACTION::ACTION_ALL;

	while (NextStr) {
		Value = MyStrTok(NextStr, SEP_COMMA, &NextStr, FALSE);
		if (!Value) break;

		if (Value(Value, ACTION_READ_STR)) Action |= (ULONG)ACL_ACTION::ACTION_READ;
		else if (!_stricmp(Value, ACTION_WRITE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_WRITE;
		else if (!_stricmp(Value, ACTION_TRAVERSE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_TRAVERSE;
		else if (!_stricmp(Value, ACTION_EXECUTE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_EXECUTE;
		else if (!_stricmp(Value, ACTION_DELETE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_DELETE;
		else if (!_stricmp(Value, ACTION_CREATE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_CREATE;
		else if (!_stricmp(Value, ACTION_RENAME_STR)) Action |= (ULONG)ACL_ACTION::ACTION_RENAME;
		else if (!_stricmp(Value, ACTION_KEY_CREATE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_KEY_CREATE;
		else if (!_stricmp(Value, ACTION_KEY_DELETE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_KEY_DELETE;
		else if (!_stricmp(Value, ACTION_VALUE_WRITE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_VALUE_WRITE;
		else if (!_stricmp(Value, ACTION_VALUE_DELETE_STR)) Action |= (ULONG)ACL_ACTION::ACTION_VALUE_DELETE;
	}

	return Action;
}

string AccessControl::ActionToStr(
	_In_ ULONG Action,
	_Out_ PCHAR ActionStrBuf,
	_In_ ULONG MaxLen
)
{
	ULONG OffSet = 0;

	if (Action == (ULONG)ACL_ACTION::ACTION_ALL) MyStrNCopy(ActionStrBuf, ACTION_ALL_STR, MaxLen);
	else {
		if (Action & (ULONG)ACL_ACTION::ACTION_READ) OffSet = MySNPrintf(ActionStrBuf, MaxLen, "%s,", ACTION_READ_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_WRITE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_WRITE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_TRAVERSE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_TRAVERSE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_EXECUTE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_EXECUTE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_DELETE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_DELETE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_CREATE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_CREATE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_RENAME) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_RENAME_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_KEY_CREATE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_KEY_CREATE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_KEY_DELETE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_KEY_DELETE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_VALUE_WRITE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_VALUE_WRITE_STR);
		if (Action & (ULONG)ACL_ACTION::ACTION_VALUE_DELETE) OffSet += MySNPrintf(ActionStrBuf + OffSet, MaxLen - OffSet, "%s,", ACTION_VALUE_DELETE_STR);

		if (OffSet == 0) *ActionStrBuf = 0;
		else ActionStrBuf[OffSet - 1] = 0;
	}

	return ActionStrBuf;
}

ACL_SUBJECT AccessControl::SearchSubject(string subjectName)
{
	ACL_SUBJECT aclSub = NULL;

	foreach(var sub in aclSubject)
	{
		if (sub.subjectName == subjectName)
		{
			aclSub = sub;
			break;
		}
	}

	return aclSub;
}
void AccessControl::ACLSubjectAdd(string subjectName, INT32 permissions)
{
	ACL_SUBJECT aclSub;

	aclSub = SearchSubject(subjectName);
	if (aclSub != null) return;
	else aclSub = new ACL_Subject();

	aclSub.subjectName = subjectName;
	aclSub.permissions = permissions;

	aclSubject.Add(aclSub);
}

void AccessControl::ACLSubjectRemove(string subjectName)
{
	INT32 index = -1;
	ACL_SUBJECT aclSub;

	foreach(var aclSub in aclSubject)
	{
		if (aclSub.subjectName == subjectName)
		{
			index = aclSubject.IndexOf(aclSub);
			break;
		}
	}

	if (index != -1) aclSubject.RemoveAt(index);
}

void AccessControl::ACLSubjectList()
{
	foreach(var aclSub in aclSubject)
	{
		Console.WriteLine("SubjectName={0}, permissions={1}", aclSub.subjectName, aclSub.permissions);
	}
}

ACL_OBJECT AccessControl::SearchObject(string objectName)
{
	ACL_Object aclObj = null;

	foreach(var obj in aclObject)
	{
		if (obj.objectName == objectName)
		{
			aclObj = obj;
			break;
		}
	}

	return aclObj;
}

void AccessControl::ACLObjectAdd(string objectName, UINT64 permissions)
{
	ACL_Object aclObj;

	aclObj = SearchObject(objectName);
	if (aclObj != null) return;
	else aclObj = new ACL_Object();

	aclObj.objectName = objectName;
	aclObj.permissions = permissions;

	aclObject.Add(aclObj);
}

void AccessControl::ACLObjectRemove(string objectName)
{
	Int32 index = -1;

	foreach(var aclObj in aclObject)
	{
		if (aclObj.objectName == objectName)
		{
			index = aclObject.IndexOf(aclObj);
			break;
		}
	}

	if (index != -1) aclObject.RemoveAt(index);
}

void AccessControl::ACLObjectList()
{
	foreach(var aclObj in aclObject)
	{
		Console.WriteLine("ObjectName={0}, permissions={1}", aclObj.objectName, aclObj.permissions);
	}
}