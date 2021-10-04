#include "stdafx.h"
#include "time.h"
#include "Header.h"
#include "Evntrace.h"
#include "ktmw32.h"


extern ulong OSVer;

//------------Global GDI Objects------------

//#pragma comment(linker,"/section:ntos,rws")
//#pragma data_seg("ntos")

unsigned long Magic1 = 0xCCCCDDDD;

ulonglong InitProcessId;
ulonglong InitThreadId;
//hAdapter
HANDLE hLpc;
//hCallback
//Controller
HANDLE hDebugObject;
//hDevice
HANDLE hDirectory;
//hDriver
//hEtwConsumer
HANDLE hEtwRegistration;
HANDLE hEvent;
HANDLE hEventPair;
HANDLE hFile;
//hFilterCommunicationPort
//hFilterConnectionPort
HANDLE hIoCompletion;
HANDLE hIoCompletionReserve;
HANDLE hJob;
HKEY hKey;
HANDLE hKeyedEvent;
HANDLE hMutex;
//hPcwObject ==> IOCTL on pcw.sys
HANDLE hPowerRequest;
HANDLE hProcess;
HANDLE hProfile;
HANDLE hSection;
HANDLE hSemaphore;
HANDLE hSession;
HANDLE hSymLink;
HANDLE hThread;
HANDLE hTimer;

HANDLE hTmRm;
HANDLE hTmTm;
HANDLE hTmTx;
HANDLE hToken;
HANDLE hTmEn;
HANDLE hTpWorkerFactory;
//Type
HANDLE hUserApcReserve;
HANDLE hPartition;
//hWmiGuid





HANDLE AllFiles[0x1000]={0};
HANDLE AllProcesses[0x1000]={0};


ulonglong AllKernelObject[0x5000]={0};
ulong AllKernelObjectsUsed = 1;	//fix for div by zero, == side effec==> first object is always zero
ulong AllFilesUsed = 1;			//fix for div by zero, == side effec==> first object is always zero
ulong AllProcessesUsed = 1;		//fix for div by zero, == side effec==> first object is always zero



unsigned long Magic2 = 0xCCCCCCCC;
//#pragma data_seg()
//-------------------------------



ulong cached_AllKernelObjectsUsed = 0;
ulong cached_AllFilesUsed = 0;
ulong cached_AllProcessesUsed = 0;


extern fNtCreatePartition NtCreatePartition;
extern fNtManagePartition NtManagePartition;
//------------------------------------------





//---------------------------
ULONG WINAPI ControlCallback(
  _In_ WMIDPREQUESTCODE RequestCode,
  _In_ PVOID            Context,
  _In_ ULONG            *Reserved,
  _In_ PVOID            Buffer
)
{
	return ERROR_SUCCESS;
}


void TpRoutine(void* pContext)
{
	return;
}


//------------------
HANDLE mCreateLpcPort(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};

	wcscpy(ObjectName_All,BaseName);
	unsigned long  LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);

	if(bPrint)	wprintf(L"LPC: %s\r\n",ObjectName_All);
	
	_UNICODE_STRING UniS = {0};
	UniS.Length = wcslen(ObjectName_All)*2;
	UniS.MaxLength = (UniS.Length)+2;
	UniS.Buffer = ObjectName_All;

	_OBJECT_ATTRIBUTES ObjAttr = {sizeof(ObjAttr)};
	ObjAttr.Attributes=OBJ_CASE_INSENSITIVE;
	ObjAttr.ObjectName=&UniS;
	
	HANDLE hLpc_l = 0;
	int retValue = ZwCreatePort(&hLpc_l,&ObjAttr,0x0,0x200,0);
	if(retValue < 0)
	{
		if(bPrint)	printf ("Error creating ALPC port, ret: %X\r\n",retValue);
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint) printf("LPC: %X\r\n",hLpc_l);
	return hLpc_l;
}

HANDLE mCreateDebug(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};

	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	if(bPrint)	wprintf(L"DebugObject: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSec = {0};
	UniSec.Length = wcslen(ObjectName_All)*2;
	UniSec.MaxLength = UniSec.Length + 2;
	UniSec.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrDebug={0};
	ObjAttrDebug.Length = sizeof(ObjAttrDebug);
	ObjAttrDebug.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrDebug.ObjectName = & UniSec;

	

	HANDLE hDebug_l = 0;
	int retValue = ZwCreateDebugObject(&hDebug_l,GENERIC_ALL,&ObjAttrDebug,0);
	if(retValue < 0)
	{
		if(bPrint)	printf ("Error creating Debug Object ret: %X\r\n",retValue);
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint) printf("hDebugObject: %X\r\n",hDebug_l);
	return hDebug_l;
}

HANDLE mCreateObjDir(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	if(bPrint) wprintf(L"Directory: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSD = {0};
	UniSD.Length = wcslen(ObjectName_All)*2;
	UniSD.MaxLength = UniSD.Length + 2;
	UniSD.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrDir={0};
	ObjAttrDir.Length = sizeof(ObjAttrDir);
	ObjAttrDir.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrDir.ObjectName = & UniSD;

	HANDLE hDir_l = 0;
	int retValue = ZwCreateDirectoryObject(&hDir_l,GENERIC_ALL,&ObjAttrDir);
	if(retValue < 0)
	{
		if(bPrint)	printf ("Error creating Directory Object ret: %X\r\n",retValue);
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hDirectory: %X\r\n",hDir_l);
	return hDir_l;
}

HANDLE mCreateEtwRegistration(bool bPrint)
{
	//printf("sizeof: %X\r\n",sizeof(_ETW_REGISTRATION_INPUT_OUTPUT));
	unsigned char In[0x11]="\xE8\x2A\xA9\xBD\x11\x9F\x49\x4D\xBA\x1D\xA4\xC2\xAB\xCA\x69\x2E";//copied as is


	_ETW_REGISTRATION_INPUT_OUTPUT EtwReg = {0};
	memcpy(&EtwReg.guid,In,0x10);

	ulonglong ResultLength = 0;
	int retValue = 
		ZwTraceControl(0xF,
		&EtwReg,
		sizeof(_ETW_REGISTRATION_INPUT_OUTPUT),
		&EtwReg,
		sizeof(_ETW_REGISTRATION_INPUT_OUTPUT),
		&ResultLength);

	if(bPrint) printf("ZwTraceControl, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Can't create EtwRegistration kernel object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hEtwRegistration: %I64X\r\n",EtwReg.hEtwReg);
	return EtwReg.hEtwReg;
}


HANDLE mCreateEvent(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	
	if(bPrint)	wprintf(L"Event: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_ev = {0};
	UniSS_ev.Length = wcslen(ObjectName_All)*2;
	UniSS_ev.MaxLength = UniSS_ev.Length + 2;
	UniSS_ev.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrEV={0};
	ObjAttrEV.Length = sizeof(ObjAttrEV);
	ObjAttrEV.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrEV.ObjectName = & UniSS_ev;

	HANDLE hEvent_l = 0;
	int retValue = ZwCreateEvent(&hEvent_l,GENERIC_ALL,&ObjAttrEV,0,0);
	if(bPrint) printf("ZwCreateEvent, ret: %X, hEvent: %I64X\r\n",retValue,hEvent_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel event pair\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hEvent: %X\r\n",hEvent_l);
	return hEvent_l;
}


HANDLE mCreateFile(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	GetCurrentDirectory(MAX_PATH,ObjectName_All);
	wcscat(ObjectName_All,L"\\");
	wcscat(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);

	if(bPrint)	wprintf(L"File: %s\r\n",ObjectName_All);
	
	HANDLE hFile_l = CreateFile(ObjectName_All,GENERIC_READ|GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_FLAG_DELETE_ON_CLOSE,0);
	if(hFile_l == INVALID_HANDLE_VALUE)
	{
		if(bPrint)	printf("Error creating kernel file object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("File: %X\r\n",hFile_l);
	return hFile_l;
}



HANDLE mCreateIOCompletion(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	if(bPrint)	wprintf(L"IOCompletion: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_iocomp = {0};
	UniSS_iocomp.Length = wcslen(ObjectName_All)*2;
	UniSS_iocomp.MaxLength = UniSS_iocomp.Length + 2;
	UniSS_iocomp.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrIOCOMP={0};
	ObjAttrIOCOMP.Length = sizeof(ObjAttrIOCOMP);
	ObjAttrIOCOMP.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrIOCOMP.ObjectName = & UniSS_iocomp;


	HANDLE hIoCompletion_l = 0;
	int retValue = ZwCreateIoCompletion(&hIoCompletion_l,GENERIC_ALL,&ObjAttrIOCOMP,2);
	if(bPrint)	printf("ZwCreateIoCompletion, ret: %X, hIoCompletion_l: %I64X\r\n",retValue,hIoCompletion_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel IOCompletion\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hIOCompletion: %X\r\n",hIoCompletion_l);
	return hIoCompletion_l;
}

HANDLE mCreateIOCompletionReserve(bool bPrint)
{
	HANDLE hIoCompletionReserve_l = 0;
	
	int retValue = ZwAllocateReserveObject(&hIoCompletionReserve_l,0,1);
	if(bPrint)	printf("ZwAllocateReserveObject, ret: %X, hIoCompletionReserve_l: %I64X\r\n",retValue,hIoCompletionReserve_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel IOCompletionReserve\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hIoCompletionReserve: %X\r\n",hIoCompletionReserve_l);
	return hIoCompletionReserve_l;
}


HANDLE mCreateJob(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};

	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	if(bPrint)	wprintf(L"Job: %s\r\n",ObjectName_All);

	_UNICODE_STRING uniZZZ = {0};
	uniZZZ.Length = wcslen(ObjectName_All) * 2;
	uniZZZ.MaxLength = uniZZZ.Length + 2;
	uniZZZ.Buffer = ObjectName_All;

	_OBJECT_ATTRIBUTES ObjAttrZZZ={0};
	ObjAttrZZZ.Length = sizeof(ObjAttrZZZ);
	ObjAttrZZZ.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrZZZ.ObjectName = & uniZZZ;

	HANDLE hJob_l = 0;

	int retValue = ZwCreateJobObject(&hJob_l,GENERIC_ALL,&ObjAttrZZZ);
	if(bPrint) printf("ZwCreateJobObject, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating job object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("Job: %X\r\n",hJob_l);
	return hJob_l;
}

HKEY mCreateKey(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};

	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	HKEY hKey_l = 0;
	if( RegCreateKey(HKEY_CURRENT_USER,ObjectName_All,&hKey_l) != ERROR_SUCCESS)
	{
		if(bPrint)	printf("Error creating key object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hKey: %X\r\n",hKey_l);
	return hKey_l;
}

HANDLE mCreateKeyedEvent(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);

	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);


	if(bPrint)	wprintf(L"hKeyedEvent: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_keyed = {0};
	UniSS_keyed.Length = wcslen(ObjectName_All)*2;
	UniSS_keyed.MaxLength = UniSS_keyed.Length + 2;
	UniSS_keyed.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttr_keyed={0};
	ObjAttr_keyed.Length = sizeof(ObjAttr_keyed);
	ObjAttr_keyed.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttr_keyed.ObjectName = & UniSS_keyed;

	HANDLE hKeyedEvent_l = 0;
	
	int retValue = ZwCreateKeyedEvent(&hKeyedEvent_l,GENERIC_ALL,&ObjAttr_keyed,0);
	if(bPrint)	printf("ZwCreateKeyedEvent, ret: %X, hKeyedEvent_l: %I64X\r\n",retValue,hKeyedEvent_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel KeyedEvent\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hKeyedEvent: %X\r\n",hKeyedEvent_l);
	return hKeyedEvent_l;
}

HANDLE mCreateMutex(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	if(bPrint)	wprintf(L"Mutex: %s\r\n",ObjectName_All);


	_UNICODE_STRING UniSS_mx = {0};
	UniSS_mx.Length = wcslen(ObjectName_All)*2;
	UniSS_mx.MaxLength = UniSS_mx.Length + 2;
	UniSS_mx.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttr_mx={0};
	ObjAttr_mx.Length = sizeof(ObjAttr_mx);
	ObjAttr_mx.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttr_mx.ObjectName = &UniSS_mx;


	HANDLE hMutex_l = 0;
	
	
	int retValue = ZwCreateMutant(&hMutex_l,GENERIC_ALL,&ObjAttr_mx,0);
	printf("ZwCreateMutant, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel mutex object, Err: %X\r\n",GetLastError());
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hMutext: %I64X\r\n",hMutex_l);
	return hMutex_l;
}


HANDLE mCreatePowerRequest(bool bPrint)
{
	pAddr Addr = {0};
	Addr.X = 0;
	Addr.Flags = 0x1;


	HANDLE hPowerRequest_l = 0;
	int retValue = ZwPowerInformation(0x2B,&Addr,0x28,&hPowerRequest_l,0x8);
	if(bPrint)	printf("ZwPowerInformation: ret: %X, hPowerRequest: %I64X\r\n",retValue,hPowerRequest_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel power request\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hPowerRequest: %I64X\r\n",hPowerRequest_l);
	return hPowerRequest_l;
}


HANDLE mCreateProcess(HANDLE* phThread,bool bPrint)
{
	if(phThread) *phThread = 0;

	wchar_t Path_All[MAX_PATH+1] = {0};
	GetSystemDirectory(Path_All,MAX_PATH);
	wcscat(Path_All,L"\\calc.exe");

	PROCESS_INFORMATION PI = {0};
	STARTUPINFO SI = {sizeof(SI)};

	if(!CreateProcess(Path_All,0,0,0,FALSE,CREATE_SUSPENDED,0,0,&SI,&PI))
	{
		if(bPrint)	printf("Error creating process\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	HANDLE hProcess_l = PI.hProcess;
	HANDLE hThread_l = PI.hThread;


	if(bPrint)	printf("hProcess: %I64X, hThread: %I64X\r\n",hProcess_l,hThread_l);

	if(phThread) *phThread = hThread_l;
	return hProcess_l;
}



HANDLE mCreateProfile(bool bPrint)
{
	unsigned char BufferProf[0x8]={0};


	HANDLE hProfile_l = 0;

	int retValue = ZwCreateProfile(&hProfile_l,
							GetCurrentProcess(),
							(void*)GetModuleHandle(0),
							0x8 /* ProfileSize */,
							0x8 /* BucketSize */,
							BufferProf /* Buffer */, 8 /* BufferSize */,
							2 /* Source */,
							-1 /* Affinity*/);

	if(bPrint)	printf("ZwCreateProfile, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel profile object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hProfile: %I64X\r\n",hProfile_l);
	return hProfile_l;
}



HANDLE mCreateSection(bool bPrint)
{
	HANDLE hSection_l = 0;
	ulonglong MaxSize=0x400000;


	int retValue = ZwCreateSection(&hSection_l,
							SECTION_MAP_READ|SECTION_MAP_WRITE,
							0,
							(_LARGE_INTEGER*)(&MaxSize),
							PAGE_READWRITE,
							SEC_COMMIT,
							0);
	if(bPrint)	printf("ZwCreateSection, ret: %X\r\n",retValue);
	if( retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel section Object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hSection: %I64X\r\n",hSection_l);
	return hSection_l;
}



HANDLE mCreateSemaphore(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	if(bPrint)	wprintf(L"Semaphore: %s\r\n",ObjectName_All);


	_UNICODE_STRING UniSS_smph = {0};
	UniSS_smph.Length = wcslen(ObjectName_All)*2;
	UniSS_smph.MaxLength = UniSS_smph.Length + 2;
	UniSS_smph.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttr_smph={0};
	ObjAttr_smph.Length = sizeof(ObjAttr_smph);
	ObjAttr_smph.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttr_smph.ObjectName = &UniSS_smph;


	HANDLE hSemaphore_l = 0;
	
	
	int retValue = ZwCreateSemaphore(&hSemaphore_l,GENERIC_ALL,&ObjAttr_smph,0x1,0x1000);
	printf("ZwCreateSemaphore, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel semaphore object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hSemaphore: %I64X\r\n",hSemaphore_l);
	return hSemaphore_l;
}

HANDLE mCreateSymLink(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};

	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);



	_UNICODE_STRING UniSS_sl = {0};
	UniSS_sl.Length = wcslen(ObjectName_All)*2;
	UniSS_sl.MaxLength = UniSS_sl.Length + 2;
	UniSS_sl.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttr_sl={0};
	ObjAttr_sl.Length = sizeof(ObjAttr_sl);
	ObjAttr_sl.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttr_sl.ObjectName = & UniSS_sl;


	wchar_t* TargetName = L"\\BaseNamedObjects";
	_UNICODE_STRING uniTarget = {0};
	uniTarget.Length = wcslen(TargetName)*2;
	uniTarget.MaxLength = uniTarget.Length + 2;
	uniTarget.Buffer = TargetName;

	if(bPrint) wprintf(L"SymLink: %s\r\n",ObjectName_All);
	if(bPrint) wprintf(L"SymLink Target: %s\r\n",TargetName);

	HANDLE hSymLink_l = 0;
	int retValue = ZwCreateSymbolicLinkObject(&hSymLink_l,
									GENERIC_ALL,
									&ObjAttr_sl,
									&uniTarget);
	if(bPrint)	printf("ZwCreateSymbolicLinkObject, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating symbolic link\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hSymLink: %I64X\r\n",hSymLink_l);
	return hSymLink_l;
}

HANDLE mCreateTimer(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	
	if(bPrint)	wprintf(L"Timer: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_ti = {0};
	UniSS_ti.Length = wcslen(ObjectName_All)*2;
	UniSS_ti.MaxLength = UniSS_ti.Length + 2;
	UniSS_ti.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrTi={0};
	ObjAttrTi.Length = sizeof(ObjAttrTi);
	ObjAttrTi.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrTi.ObjectName = & UniSS_ti;

	HANDLE hTimer_l = 0;
	int retValue = ZwCreateTimer(&hTimer_l,GENERIC_ALL,&ObjAttrTi,1);
	if(bPrint)	printf("ZwCreateTimer, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel timer\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hTimer: %I64X\r\n",hTimer_l);
	return hTimer_l;
}



HANDLE mCreateTm(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t TmLogName[MAX_PATH]={0};
	wcscpy(TmLogName,L"\\??\\");
	GetCurrentDirectory(MAX_PATH,&TmLogName[4]);
	wcscat(TmLogName,L"\\");
	wcscat(TmLogName,BaseName);

	ulong LenX = wcslen(TmLogName);
	_ultow(Rx,&TmLogName[LenX],0x10);
	wcscat(TmLogName,L".log");

	wprintf(L"TmTm: %s\r\n",TmLogName);
	
	_UNICODE_STRING UniTmLog = {0};
	UniTmLog.Length = wcslen(TmLogName)*2;
	UniTmLog.MaxLength = UniTmLog.Length + 2;
	UniTmLog.Buffer = TmLogName;

	_OBJECT_ATTRIBUTES ObjAttrTmTm = {0};
	ObjAttrTmTm.Length = sizeof(ObjAttrTmTm);

	HANDLE hTmTm_l = 0;
	int retValue = ZwCreateTransactionManager(	&hTmTm_l,
												0xf003f,
												&ObjAttrTmTm,
												&UniTmLog,
												0,
												0);


	
	if(bPrint)	printf("ZwCreateTransactionManager, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating TmTm Transaction Manager\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hTmTm: %I64X\r\n",hTmTm_l);
	return hTmTm_l;
}

HANDLE mCreateRm(wchar_t* BaseName,ulong Rx,HANDLE hTmTmX,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	_UNICODE_STRING uniRmName = {0};
	uniRmName.Length = wcslen(ObjectName_All)*2;
	uniRmName.MaxLength = uniRmName.Length + 2;
	uniRmName.Buffer = ObjectName_All;

	if(bPrint)	wprintf(L"TmRm: %s\r\n",ObjectName_All);
	//------------------
	HANDLE hTmRm_l = 0;

	GUID Guid;
	FillRandomData(&Guid,sizeof(GUID));

	

	_OBJECT_ATTRIBUTES ObjAttrRm = {0};
	ObjAttrRm.Length = sizeof(ObjAttrRm);
	ObjAttrRm.Attributes = OBJ_CASE_INSENSITIVE;
	ObjAttrRm.ObjectName = &uniRmName;


	wchar_t* Descri = L"walied assar desc";
	_UNICODE_STRING uniDesc = {0};
	uniDesc.Length = wcslen(Descri);
	uniDesc.MaxLength = uniDesc.Length + 2;
	uniDesc.Buffer = Descri;
	
	int retValue = ZwCreateResourceManager(&hTmRm_l,GENERIC_ALL,hTmTmX,&Guid,&ObjAttrRm,0,&uniDesc);
	if(bPrint)	printf("ZwCreateResourceManager, ret: %X\r\n",retValue);

	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating TmRm Resource Manager\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hTmRm: %I64X\r\n",hTmRm_l);
	return hTmRm_l;
}

HANDLE mCreateTx(wchar_t* BaseName,ulong Rx,HANDLE hTmTmX,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	_UNICODE_STRING uniTxName = {0};
	uniTxName.Length = wcslen(ObjectName_All)*2;
	uniTxName.MaxLength = uniTxName.Length + 2;
	uniTxName.Buffer = ObjectName_All;

	if(bPrint)	wprintf(L"TmTx: %s\r\n",ObjectName_All);

	_OBJECT_ATTRIBUTES ObjAttrTx = {0};
	ObjAttrTx.Length = sizeof(ObjAttrTx);
	ObjAttrTx.Attributes = OBJ_CASE_INSENSITIVE;
	ObjAttrTx.ObjectName = &uniTxName;

	wchar_t* Descri = L"walied assar desc";
	_UNICODE_STRING uniDesc = {0};
	uniDesc.Length = wcslen(Descri);
	uniDesc.MaxLength = uniDesc.Length + 2;
	uniDesc.Buffer = Descri;


	HANDLE hTmTx_l = 0;
	int retValue = ZwCreateTransaction(&hTmTx_l,
							GENERIC_ALL,
							&ObjAttrTx,
							0,
							0 /* Tm */,
							0,
							0,
							0,
							0,
							&uniDesc);
	printf("ZwCreateTransaction, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating TmTx Transaction\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hTmTx: %I64X\r\n",hTmTx_l);
	return hTmTx_l;
}



HANDLE mCreateToken(HANDLE hProcessX,bool bPrint)
{
	HANDLE hToken_l = 0;
	if(!	OpenProcessToken(hProcessX,TOKEN_ALL_ACCESS,&hToken_l) )
	{
		if(bPrint)	printf("Error OpenProcessToken, %X\r\n",GetLastError());
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hToken: %I64X\r\n",hToken_l);
	return hToken_l;
}

HANDLE mCreateTP(wchar_t* BaseName,ulong Rx,HANDLE hIoCompletionX,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	

	
	_UNICODE_STRING UniSS_tp = {0};
	UniSS_tp.Length = wcslen(ObjectName_All)*2;
	UniSS_tp.MaxLength = UniSS_tp.Length + 2;
	UniSS_tp.Buffer = ObjectName_All;

	if(bPrint)	wprintf(L"ThreadPool: %s\r\n",ObjectName_All);

	_OBJECT_ATTRIBUTES ObjAttrTP={0};
	ObjAttrTP.Length = sizeof(ObjAttrTP);
	ObjAttrTP.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrTP.ObjectName = & UniSS_tp;




	HANDLE hTpWorkerFactory_l = 0;

	int retValue = ZwCreateWorkerFactory(&hTpWorkerFactory_l,
									GENERIC_ALL,
									&ObjAttrTP,
									hIoCompletionX,
									GetCurrentProcess(),
									TpRoutine,0,1,0x10000,0x1000);
	if(bPrint)	printf("ZwCreateWorkerFactory, ret: %X, hTpWorkerFactory_l: %I64X\r\n",retValue,hTpWorkerFactory_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel TP Worker Factory\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hTpWorkerFactory: %X\r\n",hTpWorkerFactory_l);
	return hTpWorkerFactory_l;
}

HANDLE mCreateUserApcReserve(bool bPrint)
{
	HANDLE hUserApcReserve_l = 0;
	
	int retValue = ZwAllocateReserveObject(&hUserApcReserve_l,0,0);
	if(bPrint)	printf("ZwAllocateReserveObject, ret: %X, hUserApcReserve_l: %I64X\r\n",retValue,hUserApcReserve_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel UserApcReserve\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hUserApcReserve: %X\r\n",hUserApcReserve_l);
	return hUserApcReserve_l;
}


//requires SeLockMemoryPrivilege
HANDLE mCreatePartition(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	if(bPrint)	wprintf(L"Partition: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniStr={0};
	UniStr.Length = wcslen(ObjectName_All) * 2;
	UniStr.MaxLength = UniStr.MaxLength + 2;
	UniStr.Buffer = ObjectName_All;



	_OBJECT_ATTRIBUTES ObjAttr={0};
	ObjAttr.Length = sizeof(ObjAttr);
	ObjAttr.Attributes = OBJ_CASE_INSENSITIVE;
	ObjAttr.ObjectName = &UniStr;


	HANDLE hParentPartition = (HANDLE)0; //-1 Current Partition, -2 System Partition
	HANDLE hNewPartition = 0; 
	ulonglong DesiredAccess = 3;//0,1,2,3 ( [+0x01c] ValidAccessMask  : 0x1f0003 )

	
	

	int ret = NtCreatePartition(hParentPartition,(ulonglong)(&hNewPartition),DesiredAccess,(ulonglong)(&ObjAttr));
	if(bPrint) printf("NtCreatePartition, ret: %X, hNewPartition: %I64X\r\n",ret,hNewPartition);
	if(ret >= 0)
	{
		if(bPrint) wprintf(L"%s created successfully\r\n",ObjectName_All);
		return hNewPartition;
	}
	ZwSuspendProcess(GetCurrentProcess());
	return 0;
}




//Major Code change in latest Win10, please update
//Requires an admin
HANDLE mCreateEtwConsumer(bool bPrint)
{
	//printf("sizeof: %X\r\n",sizeof(_ETW_CONSUMER_INPUT_OUTPUT));
	ulonglong ResultLength = 0;
	
	
	_ETW_CONSUMER_INPUT_OUTPUT EtwConsumer = {0};
	EtwConsumer.LoggerId = 2;
	EtwConsumer.NumberOfBlocks = 4;

	ulong X0 = 0;
	EtwConsumer.pAddr0 = &X0;


	EtwConsumer.pAddr1 = VirtualAlloc(0,(0x4 * 0x80f8),MEM_COMMIT,PAGE_READWRITE);
	printf("Addr1: %I64X\r\n",EtwConsumer.pAddr1 );

	ulong X3 = 0;
	EtwConsumer.pAddr3 = &X3;

	ulonglong X2 = 0;
	EtwConsumer.pAddr2 = &X2;




	EtwConsumer.hEvent0 = CreateEvent(0,TRUE,TRUE,0);
	printf("hEvent0: %I64X\r\n",EtwConsumer.hEvent0);

	EtwConsumer.hEvent1 = CreateEvent(0,TRUE,TRUE,0);
	printf("hEvent1: %I64X\r\n",EtwConsumer.hEvent1);


	int retValue = ZwTraceControl(0xB,
								&EtwConsumer,
								sizeof(_ETW_CONSUMER_INPUT_OUTPUT),
								&EtwConsumer,
								sizeof(_ETW_CONSUMER_INPUT_OUTPUT),
								&ResultLength);
	printf("ZwTraceControl, ret: %X, ResultLength: %X\r\n",retValue,ResultLength);
	if(retValue < 0)
	{
		VirtualFree(EtwConsumer.pAddr1,0,MEM_RELEASE);
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}

	printf("hEtwConsumer: %I64X\r\n",EtwConsumer.hEtwConsumer);

	VirtualFree(EtwConsumer.pAddr1,0,MEM_RELEASE);
	return EtwConsumer.hEtwConsumer;
}



HANDLE mCreateEventPair(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	
	if(bPrint)	wprintf(L"EventPair: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_ep = {0};
	UniSS_ep.Length = wcslen(ObjectName_All)*2;
	UniSS_ep.MaxLength = UniSS_ep.Length + 2;
	UniSS_ep.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrEP={0};
	ObjAttrEP.Length = sizeof(ObjAttrEP);
	ObjAttrEP.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrEP.ObjectName = & UniSS_ep;

	HANDLE hEventPair_l = 0;
	int retValue = ZwCreateEventPair(&hEventPair_l,GENERIC_ALL,&ObjAttrEP);
	if(bPrint) printf("ZwCreateEventPair, ret: %X, hEventPair_l: %I64X\r\n",retValue,hEventPair_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel event pair\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hEventPair: %X\r\n",hEventPair_l);
	return hEventPair_l;
}









HANDLE mOpenSession(bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,L"\\KernelObjects\\session");
	ulong LenX = wcslen(ObjectName_All);
	for(unsigned long ss = 0; ss < 10;ss++)
	{
		_ultow(ss,&ObjectName_All[LenX],0x10);
		if(bPrint) wprintf(L"Trying Session: %s\r\n",ObjectName_All);

		_UNICODE_STRING UniSS_session = {0};
		UniSS_session.Length = wcslen(ObjectName_All)*2;
		UniSS_session.MaxLength = UniSS_session.Length + 2;
		UniSS_session.Buffer = ObjectName_All;

		_OBJECT_ATTRIBUTES ObjAttrSession={0};
		ObjAttrSession.Length = sizeof(ObjAttrSession);
		ObjAttrSession.Attributes= OBJ_CASE_INSENSITIVE;
		ObjAttrSession.ObjectName = & UniSS_session;


		HANDLE hSession_l = 0;
		int retValue = ZwOpenSession(&hSession_l,GENERIC_ALL,&ObjAttrSession);
		if(bPrint)	printf("ZwOpenSession, ret: %X\r\n",retValue);
		if(retValue >= 0)
		{
			if(bPrint)	printf("hSession: %I64X\r\n",hSession_l);
			return hSession_l;
		}
	}
	return 0;
}



int InitKernelObjects(bool bPrint)
{
	AllKernelObjectsUsed = 1;
	AllFilesUsed = 1;
	AllProcessesUsed = 1;

	InitProcessId = GetCurrentProcessId();
	InitThreadId = GetCurrentThreadId();

	wchar_t ObjectName_All[MAX_PATH+1]={0};



	//------------ Static Name Parts -------------------------------------
	wchar_t* Name = L"walied";
	wchar_t* FullName = L"\\BaseNamedObjects\\Local\\walied";
	//-------------- Random Name Parts ------------------------------------
	unsigned long Rx = GetRandomValue();
	unsigned long Rxx;
	//---------------- ALPC ----------------------------------------------
	HANDLE hLpc_l= mCreateLpcPort(FullName,Rx,bPrint);
	//if(bPrint) printf("LPC: %X\r\n",hLpc_l);
	hLpc = hLpc_l;

	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hLpc_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//--------------------------- Debug -----------------------------------
	HANDLE hDebug_l = mCreateDebug(FullName,Rx,bPrint);
	//if(bPrint) printf("hDebugObject: %X\r\n",hDebug_l);
	hDebugObject = hDebug_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hDebug_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	
	//------------------------ Directory ---------------------------------
	HANDLE hDir_l = mCreateObjDir(FullName,Rx,bPrint);
	//if(bPrint)	printf("hDirectory: %X\r\n",hDir_l);
	hDirectory = hDir_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hDir_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	
	//------------------------ ETW --------------------------------------------------
	//----------------------EtwConsumer -------------- Requires admin----------------
	//mCreateEtwConsumer(bPrint);
	//-------------------------- hEtwRegistration ------------------------
	HANDLE hEtwRegistration_l = mCreateEtwRegistration(bPrint);
	//if(bPrint)	printf("hEtwRegistration: %I64X\r\n",hEtwRegistration_l);
	hEtwRegistration = hEtwRegistration_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hEtwRegistration_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//ExitProcess(0);
	//-------------------------Event--------------------------------------------
	HANDLE hEvent_l = mCreateEvent(FullName,Rx,bPrint);
	if(bPrint)	printf("Event: %X\r\n",hEvent_l);
	hEvent = hEvent_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hEvent_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//-------------------- Event Pair -------------------------------------
	if(OSVer == 0) //Event pair not implemented in windows 10
	{
		HANDLE hEventPair_l = mCreateEventPair(FullName,Rx,bPrint);
		if(bPrint)	printf("hEventPair: %X\r\n",hEventPair_l);
		hEventPair = hEventPair_l;
		AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hEventPair_l;
		AllKernelObjectsUsed++;
		ObjectName_All[0]=0;
	}
	//------------------- File ---------------------------
	HANDLE hFile_l = mCreateFile(Name,Rx,bPrint);
	if(bPrint)	printf("File: %X\r\n",hFile_l);
	hFile = hFile_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hFile_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//----------------------- IO Completion ------------------------------
	HANDLE hIoCompletion_l = mCreateIOCompletion(FullName,Rx,bPrint);
	if(bPrint)	printf("hIOCompletion: %X\r\n",hIoCompletion_l);
	hIoCompletion = hIoCompletion_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hIoCompletion_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//------------------------ IO Completion Reserve ----------------------

	/*
	wcscpy(ObjectName_All,FullName);
	LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	Rxx = Rand();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);


	if(bPrint)	wprintf(L"IOCompletionReserve: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_iocomprsv = {0};
	UniSS_iocomprsv.Length = wcslen(ObjectName_All)*2;
	UniSS_iocomprsv.MaxLength = UniSS_iocomprsv.Length + 2;
	UniSS_iocomprsv.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrIOCOMPrsv={0};
	ObjAttrIOCOMPrsv.Length = sizeof(ObjAttrIOCOMPrsv);
	ObjAttrIOCOMPrsv.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttrIOCOMPrsv.ObjectName = & UniSS_iocomprsv;
	*/
	HANDLE hIoCompletionReserve_l = mCreateIOCompletionReserve(bPrint);
	if(bPrint)	printf("hIoCompletionReserve: %X\r\n",hIoCompletionReserve_l);
	hIoCompletionReserve = hIoCompletionReserve_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hIoCompletionReserve_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//---------------------- Job ------------------------------------------
	HANDLE hJob_l = mCreateJob(FullName,Rx,bPrint);
	if(!hJob_l)
	{
		if(bPrint)	printf("Error creating job object, Err: %X\r\n",GetLastError());
		return -5;
	}
	if(bPrint)	printf("Job: %X\r\n",hJob_l);
	hJob = hJob_l;
	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hJob_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//Visual studio has its own job while testing, uncomment later
	//if(!AssignProcessToJobObject(hJob,GetCurrentProcess()))
	//{
	//	if(bPrint)	printf("Error assigning to  job object, process already a job object\r\n");
	//	return -5;
	//}
	//---------------------- Key ---------------------------------------------
	HKEY hKey_l = mCreateKey(Name,Rx,bPrint);
	if(bPrint)	printf("hKey: %X\r\n",hKey_l);


	hKey = hKey_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hKey_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//-------------------------- Keyed Event --------------------------------
	HANDLE hKeyedEvent_l = mCreateKeyedEvent(FullName,Rx,bPrint);
	if(bPrint)	printf("hKeyedEvent: %X\r\n",hKeyedEvent_l);

	hKeyedEvent = hKeyedEvent_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hKeyedEvent_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//---------------- Mutex / Mutant ---------------------------
	HANDLE hMutex_l = mCreateMutex(FullName,Rx,bPrint);
	if(bPrint)	printf("hMutext: %I64X\r\n",hMutex_l);
	hMutex = hMutex_l;
	ObjectName_All[0]=0;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hMutex_l;
	AllKernelObjectsUsed++;
	
	//--------------------- Power Request ---------------------------
	HANDLE hPowerRequest_l = mCreatePowerRequest(bPrint);
	
	if(bPrint)	printf("hPowerRequest: %I64X\r\n",hPowerRequest_l);
	hPowerRequest = hPowerRequest_l;

	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hPowerRequest_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	
	//------------------------- Process ---------------------------------
	HANDLE hThread_P = 0;
	HANDLE hProcess_l = mCreateProcess(&hThread_P,bPrint);
	if(bPrint)	printf("hProcess: %I64X\r\n",hProcess_l);
	hProcess = hProcess_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hProcess_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//------------------- Profile ----------------------------
	HANDLE hProfile_l = mCreateProfile(bPrint);
	if(bPrint)	printf("hProfile: %I64X\r\n",hProfile_l);


	hProfile = hProfile_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hProfile_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//-------------------- Section ------------------ Implement Named Section Later ---
	//wcscpy(ObjectName_All,FullName);
	//LenX = wcslen(ObjectName_All);
	//_ultow(Rx,&ObjectName_All[LenX],0x10);
	//LenX = wcslen(ObjectName_All);
	//Rxx = Rand();
	//_ultow(Rxx,&ObjectName_All[LenX],0x10);
	


	//wprintf(L"Section: %s\r\n",ObjectName_All);

	//_UNICODE_STRING UniSS = {0};
	//UniSS.Length = wcslen(ObjectName_All)*2;
	//UniSS.MaxLength = UniSS.Length + 2;
	//UniSS.Buffer = ObjectName_All;


	//_OBJECT_ATTRIBUTES ObjAttrSection={0};
	//ObjAttrSection.Length = sizeof(ObjAttrSection);
	//ObjAttrSection.Attributes= OBJ_CASE_INSENSITIVE;
	//ObjAttrSection.ObjectName = & UniSS;

	HANDLE hSection_l = mCreateSection(bPrint);
	
	if(bPrint)	printf("hSection: %I64X\r\n",hSection_l);
	hSection = hSection_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hSection_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;

	//------------------------- Semaphore ---------------------
	HANDLE hSemaphore_l = mCreateSemaphore(FullName,Rx,bPrint);
	if(bPrint)	printf("hSemaphore: %I64X\r\n",hSemaphore_l);
	
	hSemaphore = hSemaphore_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hSemaphore_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//---------------------- Session -----------Requires priv, try NtCreateUserProcess
	HANDLE hSession_l = mOpenSession(bPrint);
	if(bPrint)	printf("hSession: %I64X\r\n",hSession_l);
	
	hSession = hSession_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hSession_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//-----------------------  SymLink ---------------
	HANDLE hSymLink_l = mCreateSymLink(L"\\RPC Control\\walied",Rx,bPrint);

	if(bPrint)	printf("hSymLink: %I64X\r\n",hSymLink_l);
	hSymLink = hSymLink_l;

	
	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hSymLink_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	
	//----------------------- Thread --------------------------
	HANDLE hThread_l = hThread_P;
	if(bPrint)	printf("hThread: %I64X\r\n",hThread_l);

	hThread = hThread_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hThread_l;
	AllKernelObjectsUsed++;
	//--------------------- Timer ----------------------
	HANDLE hTimer_l = mCreateTimer(FullName,Rx,bPrint);
	if(bPrint)	printf("hTimer: %I64X\r\n",hTimer_l);

	hTimer = hTimer_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTimer_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//------------------------------- TmTm Transaction Manager ------------------------
	/*
	_UNICODE_STRING UniTmLog = {0};
	UniTmLog.Length = wcslen(pTmLog)*2;
	UniTmLog.MaxLength = UniTmLog.Length + 2;
	UniTmLog.Buffer = pTmLog;

	retValue = ZwCreateTransactionManager(&hTmTm_l,GENERIC_ALL,0,0,1,0);

	
	if(bPrint)	printf("ZwCreateTransactionManager, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating TmTm Transaction Manager\r\n");
		return -17;
	}
	*/
	HANDLE hTmTm_l = mCreateTm(Name,Rx,bPrint);
	if(bPrint)	printf("hTmTm: %I64X\r\n",hTmTm_l);

	hTmTm = hTmTm_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTmTm_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//-------------------------- TmRm  Resource Manager ------------------------------
	HANDLE hTmRm_l = mCreateRm(FullName,Rx,hTmTm_l,bPrint);
	if(bPrint)	printf("hTmRm: %I64X\r\n",hTmRm_l);

	hTmRm = hTmRm_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTmRm_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//----------------------- TmTx ------------------------
	HANDLE hTmTx_l = mCreateTx(FullName,Rx,hTmTm_l,bPrint);
	if(bPrint)	printf("hTmTx: %I64X\r\n",hTmTx_l);
	hTmTx = hTmTx_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTmTx_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//---------------------- TmEn Enlistment --------------------------
	
	//unsigned char Buff[0x200]={0};
	//FillRandomData(Buff,0x200);
	//HANDLE hTmEn_l = CreateEnlistment(0,hTmRm_l,hTmTx_l,0,0,Buff);
	//printf("hTmEn: %I64X\r\n",hTmEn_l);
	
	//hTmEn_l = 0;
	//int retValue = ZwCreateEnlistment(&hTmEn_l,GENERIC_ALL,hTmRm_l,hTmTx_l,0,1 /* CreateOptions 0 or 1 */,0 /* Notification Mask*/,0);
	//printf("ZwCreateEnlistment: %X\r\n",retValue);
	//printf("hTmEn: %I64X\r\n",hTmEn_l);
	//---------------------- Token ------------------------------------
	HANDLE hToken_l = mCreateToken(hProcess_l,bPrint);
	if(bPrint)	printf("hToken: %I64X\r\n",hToken_l);
	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hToken_l;
	AllKernelObjectsUsed++;

	ObjectName_All[0]=0;
	
	//---------------------- Thread Pool ----------------------
	HANDLE hTpWorkerFactory_l = mCreateTP(FullName,Rx,hIoCompletion_l,bPrint);
	if(bPrint)	printf("hTpWorkerFactory: %X\r\n",hTpWorkerFactory_l);
	hTpWorkerFactory = hTpWorkerFactory_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTpWorkerFactory_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	
	//---------------------- UserApcReserve --------------------
	HANDLE hUserApcReserve_l = mCreateUserApcReserve(bPrint);
	if(bPrint)	printf("hUserApcReserve: %X\r\n",hUserApcReserve_l);
	hUserApcReserve = hUserApcReserve_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hUserApcReserve_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//----------- Partition ---------
	HANDLE hPartition_l = 0;
	BOOL bLock = Acquire_LockMemory();
	if(!bLock)
	{
		if(bPrint)
		{
			printf("SeLockMemoryPrivilege required for creating partition objects\r\n");
		}

		if( Rand()%2 == 1)
		{
			hPartition_l = (HANDLE)-1;//Current Partition
		}
		else
		{
			hPartition_l = (HANDLE) -2;//System Partition
		}
	}
	else
	{
		hPartition_l = mCreatePartition(FullName,Rx,bPrint);
		if(!hPartition_l)
		{
			if( Rand()%2 == 1)
			{
				hPartition_l = (HANDLE)-1;//Current Partition
			}
			else
			{
				hPartition_l = (HANDLE) -2;//System Partition
			}
		}
	}

	if(bPrint) printf("hPartition: %X\r\n",hPartition_l);


	hPartition = hPartition_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hPartition_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	return 0;
}


void ObjCreatorDestroyerThread()
{
	srand(time(NULL));
	while(1)
	{
		Sleep(10000);
		if(hProcess)
		{
			int retX = ZwTerminateProcess(hProcess,GetRandomNTStatusCode());
			if(retX < 0)
			{
				//printf("ZwTerminateProcess, ret: %X (hProcess: %I64X)\r\n",retX,hProcess);
				//ExitProcess(0);
			}
		}

		ulong var_AllKernelObjectsUsed = AllKernelObjectsUsed;
		ulong i = 0;
		while(i < var_AllKernelObjectsUsed)
		{
			ZwClose( (HANDLE) (AllKernelObject[i]) );
			i++;
		}
		//----------------
		InitKernelObjects(false);
		cached_AllKernelObjectsUsed = AllKernelObjectsUsed;
		cached_AllFilesUsed = AllFilesUsed;
		cached_AllProcessesUsed = AllProcessesUsed;
		//--------------------------

	}
}

void ObjDestroyerThread()
{
	srand(time(NULL));
	while(1)
	{
		Sleep(2000);

		if(hThread)
		{
			int retX = ZwTerminateThread(hThread,GetRandomNTStatusCode());
			if(retX < 0)
			{
				//printf("ZwTerminateThread, ret: %X (hThread: %I64X)\r\n",retX,hThread);
				//ExitProcess(0);
			}
		}

		if(hProcess)
		{
			int retX = ZwTerminateProcess(hProcess,GetRandomNTStatusCode());
			if(retX < 0)
			{
				//printf("ZwTerminateProcess, ret: %X (hProcess: %I64X)\r\n",retX,hProcess);
				//ExitProcess(0);
			}
		}

		ulong var_AllKernelObjectsUsed = AllKernelObjectsUsed;
		ulong i = 0;
		while(i < var_AllKernelObjectsUsed)
		{
			ZwClose( (HANDLE) (AllKernelObject[i]) );
			i++;
		}
	}
}