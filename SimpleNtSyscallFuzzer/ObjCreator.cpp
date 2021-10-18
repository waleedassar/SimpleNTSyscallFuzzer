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
HANDLE hNameSpaceDirectory;
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
HANDLE hPartitionJob;
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
HANDLE hToken2;
HANDLE hTmEn;
HANDLE hTpWorkerFactory;
//Type
HANDLE hUserApcReserve;
HANDLE hPartition;
HANDLE hPartition2;
//hWmiGuid
HANDLE hWaitCompletionPacket;
HANDLE hRegistryTransaction;



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

extern fNtOpenPartition   NtOpenPartition;
extern fNtCreatePartition NtCreatePartition;
extern fNtManagePartition NtManagePartition;
extern fNtCreateRegistryTransaction NtCreateRegistryTransaction;


wchar_t* wPartName = L"\\RPC Control\\walied";
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
PSID GetCurrentUserSID(ulong* pSidSize)
{


	SetLastError(0);

	if(pSidSize) *pSidSize = 0;

	DWORD cchDomainName = 0x100;

	WCHAR * wszDomainName = (wchar_t*)LocalAlloc(LMEM_ZEROINIT,cchDomainName);
	if(!wszDomainName) return 0;
	


	ulong SidSize = 0x10000;
	_SID* pSid = (_SID*)VirtualAlloc(0,SidSize,MEM_COMMIT,PAGE_READWRITE);
	if(pSid)
	{
		SID_NAME_USE UseX;

		if( LookupAccountName(0,L"walied",pSid,&SidSize,wszDomainName,&cchDomainName,&UseX) )
		{
			LocalFree(wszDomainName);
			if(pSidSize) *pSidSize = SidSize;
			return pSid;
		}
		else
		{
			printf("Error getting current user's sid.\r\n");
			VirtualFree(pSid,0,MEM_RELEASE);
		}
	}
	LocalFree(wszDomainName);
	return 0;
}

uchar* FillBasic(_BOUNDARY_DESCRIPTOR_INPUT_BASIC* pBoundBasic)
{
	if(!pBoundBasic) return 0;

	pBoundBasic->Revision=1;
	pBoundBasic->NumberOfEntries=0;
	pBoundBasic->Size= sizeof(_BOUNDARY_DESCRIPTOR_INPUT_BASIC); //to be updated
	pBoundBasic->UNKZ = 0;

	return (((uchar*)pBoundBasic) + sizeof(_BOUNDARY_DESCRIPTOR_INPUT_BASIC));
}

uchar* AddNameTLV(_BOUNDARY_DESCRIPTOR_INPUT_BASIC* pBoundBasic,
				_BOUNDARY_DESCRIPTOR_INPUT_TLV* pTLV,
				wchar_t* NameSpaceName)
{
	if(!pBoundBasic) return 0;
	if(!pTLV) return 0;
	if(!NameSpaceName) return 0;
	//----------------
	ulonglong DiffX = 0;
	if( ((ulonglong)pTLV) & 0x7)
	{
		//printf("Realigning\r\n");
		
		ulonglong xTLV = 
		((((ulonglong)pTLV) + 7) & 0xFFFFFFFFFFFFFFF8);
		
		DiffX = xTLV - ((ulonglong)pTLV);

		pTLV = (_BOUNDARY_DESCRIPTOR_INPUT_TLV*)xTLV;
	}

	pTLV->Type = 1;

	ulong NameLen = wcslen(NameSpaceName) * 2;

	pTLV->Length = sizeof(_BOUNDARY_DESCRIPTOR_INPUT_TLV) + NameLen;

	uchar* pTLV_data = ((uchar*)pTLV) + sizeof(_BOUNDARY_DESCRIPTOR_INPUT_TLV);

	memcpy(pTLV_data,NameSpaceName,NameLen);

	pBoundBasic->NumberOfEntries++;

	pBoundBasic->Size += pTLV->Length;
	pBoundBasic->Size += DiffX;

	return pTLV_data+NameLen;
}

uchar* AddSidTLV(_BOUNDARY_DESCRIPTOR_INPUT_BASIC* pBoundBasic,
				_BOUNDARY_DESCRIPTOR_INPUT_TLV* pTLV,
				PSID pSid)
{
	if(!pBoundBasic) return 0;
	if(!pTLV) return 0;
	if(!pSid) return 0;
	//----------------
	ulonglong DiffX = 0;
	if( ((ulonglong)pTLV) & 0x7)
	{
		//printf("Realigning\r\n");
		
		ulonglong xTLV = 
		((((ulonglong)pTLV) + 7) & 0xFFFFFFFFFFFFFFF8);
		
		DiffX = xTLV - ((ulonglong)pTLV);

		pTLV = (_BOUNDARY_DESCRIPTOR_INPUT_TLV*)xTLV;
	}


	pTLV->Type = 2;



	ulong SidSize = GetLengthSid(pSid);
	if(!SidSize) return 0;

	pTLV->Length = sizeof(_BOUNDARY_DESCRIPTOR_INPUT_TLV) + SidSize;

	uchar* pTLV_data = ((uchar*)pTLV) + sizeof(_BOUNDARY_DESCRIPTOR_INPUT_TLV);

	memcpy(pTLV_data,pSid,SidSize);

	pBoundBasic->NumberOfEntries++;

	pBoundBasic->Size += pTLV->Length;
	pBoundBasic->Size += DiffX;

	return pTLV_data+SidSize;
}

uchar* AddObjectIdTLV(_BOUNDARY_DESCRIPTOR_INPUT_BASIC* pBoundBasic,
				_BOUNDARY_DESCRIPTOR_INPUT_TLV* pTLV,
				PSID pSid)
{
	if(!pBoundBasic) return 0;
	if(!pTLV) return 0;
	if(!pSid) return 0;
	//----------------
	ulonglong DiffX = 0;
	if( ((ulonglong)pTLV) & 0x7)
	{
		//printf("Realigning\r\n");
		
		ulonglong xTLV = 
		((((ulonglong)pTLV) + 7) & 0xFFFFFFFFFFFFFFF8);
		
		DiffX = xTLV - ((ulonglong)pTLV);

		pTLV = (_BOUNDARY_DESCRIPTOR_INPUT_TLV*)xTLV;
	}

	pTLV->Type = 3;



	ulong SidSize = GetLengthSid(pSid);
	if(!SidSize) return 0;

	pTLV->Length = sizeof(_BOUNDARY_DESCRIPTOR_INPUT_TLV) + SidSize;

	uchar* pTLV_data = ((uchar*)pTLV) + sizeof(_BOUNDARY_DESCRIPTOR_INPUT_TLV);

	memcpy(pTLV_data,pSid,SidSize);

	pBoundBasic->NumberOfEntries++;

	pBoundBasic->Size += pTLV->Length;
	pBoundBasic->Size += DiffX;

	return pTLV_data+SidSize;
}

//------------------
HANDLE mCreateLpcPort(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttr.Attributes |= OBJ_INHERIT;
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

HANDLE mCreateDebug(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrDebug.Attributes |= OBJ_INHERIT;
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

HANDLE mCreateObjDir(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrDir.Attributes |= OBJ_INHERIT;
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


HANDLE mCreatePrivateNameSpaceDirectory(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
{
	//Object Directories for namespaces can't be named objects.
	_OBJECT_ATTRIBUTES ObjAttrX = {0};
	ObjAttrX.Length = sizeof(_OBJECT_ATTRIBUTES);
	if(bInherit)	ObjAttrX.Attributes |= OBJ_INHERIT;


	_BOUNDARY_DESCRIPTOR_INPUT_BASIC* inBoundaryDescriptor = 
	(_BOUNDARY_DESCRIPTOR_INPUT_BASIC*)VirtualAlloc(0,0x10000,MEM_COMMIT,PAGE_READWRITE);



	uchar* pRunner = FillBasic(inBoundaryDescriptor);
	if(!pRunner)
	{
		VirtualFree(inBoundaryDescriptor,0,MEM_RELEASE);
		return 0;
	}

	//Insert name
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	if(bPrint) wprintf(L"NameSpace: %s\r\n",ObjectName_All);



	pRunner = AddNameTLV(inBoundaryDescriptor,
		(_BOUNDARY_DESCRIPTOR_INPUT_TLV*)pRunner,
		ObjectName_All);

	if(!pRunner) 
	{
		VirtualFree(inBoundaryDescriptor,0,MEM_RELEASE);
		return 0;
	}

	//Insert _SID_INFO
	ulong SidSize = 0;
	PSID pSid = GetCurrentUserSID(&SidSize);

	if(!pSid)
	{
		VirtualFree(inBoundaryDescriptor,0,MEM_RELEASE);
		return 0;
	}

	if(!SidSize)
	{
		if(pSid) VirtualFree(pSid,0,MEM_RELEASE);
		VirtualFree(inBoundaryDescriptor,0,MEM_RELEASE);
		return 0;
	}


	pRunner = AddSidTLV(inBoundaryDescriptor,
		(_BOUNDARY_DESCRIPTOR_INPUT_TLV*)pRunner,
		pSid);

	if(!pRunner)
	{
		if(pSid) VirtualFree(pSid,0,MEM_RELEASE);
		VirtualFree(inBoundaryDescriptor,0,MEM_RELEASE);
		return 0;
	}


	//Insert ObjectId
	pRunner = AddObjectIdTLV(inBoundaryDescriptor,
		(_BOUNDARY_DESCRIPTOR_INPUT_TLV*)pRunner,
		pSid);

	if(!pRunner)
	{
		if(pSid) VirtualFree(pSid,0,MEM_RELEASE);
		VirtualFree(inBoundaryDescriptor,0,MEM_RELEASE);
		return 0;
	}
	//-------- END ----------
	
	HANDLE hName = 0;
	int ret = NtCreatePrivateNamespace(&hName,GENERIC_ALL,
									&ObjAttrX,inBoundaryDescriptor);
	if(bPrint) wprintf(L"NtCreatePrivateNamespace, ret: %X, hName: %X\r\n",ret,hName);


	if(ret < 0)
	{
		hName = 0;
	}
	else
	{
		if(bInherit)	ObjAttrX.Attributes |= OBJ_INHERIT;
		HANDLE hNameZ = 0;
		int retValue = NtOpenPrivateNamespace(&hNameZ,
												GENERIC_ALL,
												&ObjAttrX,
												inBoundaryDescriptor);

		if(bPrint) wprintf(L"NtOpenPrivateNamespace, ret: %X, hName: %X\r\n",retValue,hNameZ);

		if(retValue >= 0)
		{
			ZwClose(hNameZ);
		}
	}

	if(pSid) VirtualFree(pSid,0,MEM_RELEASE);
	VirtualFree(inBoundaryDescriptor,0,MEM_RELEASE);
	return hName;
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


HANDLE mCreateEvent(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrEV.Attributes |= OBJ_INHERIT;
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



HANDLE mCreateIOCompletion(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrIOCOMP.Attributes |= OBJ_INHERIT;
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


HANDLE mCreateJob(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrZZZ.Attributes |= OBJ_INHERIT;
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

HANDLE GetPartitionJob(bool bInherit,bool bPrint)
{
	wchar_t* JobName = L"\\RPC Control\\waliedj";
	_UNICODE_STRING UniStr={0};
	UniStr.Length = wcslen(JobName) * 2;
	UniStr.MaxLength = UniStr.Length + 2;
	UniStr.Buffer = JobName;

	_OBJECT_ATTRIBUTES ObjAttr = {0};
	ObjAttr.Length = sizeof(ObjAttr);
	ObjAttr.Attributes = OBJ_CASE_INSENSITIVE;
	if(bInherit)	ObjAttr.Attributes |= OBJ_INHERIT;
	ObjAttr.ObjectName = &UniStr;

	HANDLE hJob = 0;
	int ret = ZwOpenJobObject(&hJob,0x1 /* Desired */,&ObjAttr);
	if(bPrint) printf("ZwOpenJobObject, ret: %X\r\n",ret);

	if(ret >= 0)
	{
		return hJob;
	}

	return 0;
}


HKEY mCreateKey(wchar_t* BaseName,ulong Rx,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,L"software\\AppDataLow\\");
	wcscat(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	if(bPrint) wprintf(L"Key: %s\r\n",ObjectName_All);

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

HANDLE mCreateKeyedEvent(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttr_keyed.Attributes |= OBJ_INHERIT;
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

HANDLE mCreateMutex(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	ObjAttr_mx.Attributes = OBJ_CASE_INSENSITIVE;
	if(bInherit)	ObjAttr_mx.Attributes |= OBJ_INHERIT;
	ObjAttr_mx.ObjectName = &UniSS_mx;


	HANDLE hMutex_l = 0;
	
	
	int retValue = ZwCreateMutant(&hMutex_l,GENERIC_ALL,&ObjAttr_mx,0);
	if(bPrint)	printf("ZwCreateMutant, ret: %X\r\n",retValue);
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
	wcscat(Path_All,L"\\charmap.exe");

	PROCESS_INFORMATION PI = {0};
	STARTUPINFO SI = {sizeof(SI)};

	
	if(!CreateProcess(Path_All,0,0,0,TRUE /* bInheritHandles */,CREATE_SUSPENDED,0,0,&SI,&PI))
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



HANDLE mCreateSemaphore(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttr_smph.Attributes |= OBJ_INHERIT;
	ObjAttr_smph.ObjectName = &UniSS_smph;


	HANDLE hSemaphore_l = 0;
	
	
	int retValue = ZwCreateSemaphore(&hSemaphore_l,GENERIC_ALL,&ObjAttr_smph,0x1,0x1000);
	if(bPrint)	printf("ZwCreateSemaphore, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel semaphore object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hSemaphore: %I64X\r\n",hSemaphore_l);
	return hSemaphore_l;
}

HANDLE mCreateSymLink(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttr_sl.Attributes |= OBJ_INHERIT;
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
									GENERIC_ALL|2,
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

HANDLE mCreateTimer(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrTi.Attributes |= OBJ_INHERIT;
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



HANDLE mCreateTm(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
{
	wchar_t TmLogName[MAX_PATH]={0};
	wcscpy(TmLogName,L"\\??\\");
	GetCurrentDirectory(MAX_PATH,&TmLogName[4]);
	wcscat(TmLogName,L"\\");
	wcscat(TmLogName,BaseName);

	ulong LenX = wcslen(TmLogName);
	_ultow(Rx,&TmLogName[LenX],0x10);
	wcscat(TmLogName,L".log");

	if(bPrint) wprintf(L"TmTm: %s\r\n",TmLogName);
	
	_UNICODE_STRING UniTmLog = {0};
	UniTmLog.Length = wcslen(TmLogName)*2;
	UniTmLog.MaxLength = UniTmLog.Length + 2;
	UniTmLog.Buffer = TmLogName;

	_OBJECT_ATTRIBUTES ObjAttrTmTm = {0};
	ObjAttrTmTm.Length = sizeof(ObjAttrTmTm);
	if(bInherit)	ObjAttrTmTm.Attributes |= OBJ_INHERIT;

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

HANDLE mCreateRm(wchar_t* BaseName,ulong Rx,HANDLE hTmTmX,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrRm.Attributes |= OBJ_INHERIT;
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

HANDLE mCreateEn(wchar_t* BaseName,ulong Rx,HANDLE hTmRmX,HANDLE hTmTxX,bool bInherit,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);

	_UNICODE_STRING uniEnName = {0};
	uniEnName.Length = wcslen(ObjectName_All)*2;
	uniEnName.MaxLength = uniEnName.Length + 2;
	uniEnName.Buffer = ObjectName_All;

	if(bPrint)	wprintf(L"TmEn: %s\r\n",ObjectName_All);

	_OBJECT_ATTRIBUTES ObjAttrEn = {0};
	ObjAttrEn.Length = sizeof(ObjAttrEn);
	ObjAttrEn.Attributes = OBJ_CASE_INSENSITIVE;
	if(bInherit)	ObjAttrEn.Attributes |= OBJ_INHERIT;
	ObjAttrEn.ObjectName = &uniEnName;

	HANDLE hTmEn_l = 0;
	int retValue = ZwCreateEnlistment(&hTmEn_l,GENERIC_ALL,hTmRmX,hTmTxX,
							&ObjAttrEn,1 /* CreateOptions 0 or 1 */,0x8 /* Notification Mask*/,0);
	if(bPrint)	printf("ZwCreateEnlistment: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating TmEn Enlistment\r\n");
		return 0;
	}
	if(bPrint)	printf("hTmEn: %I64X\r\n",hTmEn_l);
	return hTmEn_l;
}


HANDLE mCreateTx(wchar_t* BaseName,ulong Rx,HANDLE hTmTmX,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrTx.Attributes |= OBJ_INHERIT;
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
	if(bPrint)	printf("ZwCreateTransaction, ret: %X\r\n",retValue);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating TmTx Transaction\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hTmTx: %I64X\r\n",hTmTx_l);
	return hTmTx_l;
}



HANDLE mOpenTokenProcess(HANDLE hProcessX,bool bPrint)
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


HANDLE mCreateTokenLogon(wchar_t* UserName,wchar_t* Pass,bool bPrint)
{
	HANDLE hToken_l = 0;
	if(!LogonUser(UserName,0,Pass,LOGON32_LOGON_INTERACTIVE,LOGON32_PROVIDER_DEFAULT,&hToken_l))
	{
		if(bPrint) printf("LogonUser failed, err: %X\r\n",GetLastError());
		return 0;
	}
	if(bPrint) printf("hToken (LogonUser): %I64X\r\n",hToken_l);
	return hToken_l;
}

HANDLE mCreateTP(wchar_t* BaseName,ulong Rx,HANDLE hIoCompletionX,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrTP.Attributes |= OBJ_INHERIT;
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
HANDLE mCreatePartition(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttr.Attributes |= OBJ_INHERIT;
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
	else
	{
		ZwSuspendProcess(GetCurrentProcess());
	}
	return 0;
}


//Opens 
HANDLE mOpenPartition(wchar_t* PartName,bool bInherit,bool bPrint)
{
	if(bPrint)	wprintf(L"Opening Partition: %s\r\n",PartName);
	_UNICODE_STRING uniPart = {0};
	uniPart.Length = wcslen(PartName) * 2;
	uniPart.MaxLength = uniPart.Length + 2;
	uniPart.Buffer = PartName;

	_OBJECT_ATTRIBUTES ObjAttr = {0};
	ObjAttr.Length = sizeof(ObjAttr);
	ObjAttr.Attributes = OBJ_CASE_INSENSITIVE;
	if(bInherit)	ObjAttr.Attributes |= OBJ_INHERIT;
	ObjAttr.ObjectName = &uniPart;

	HANDLE hPart_l = 0;
	int ret = NtOpenPartition(&hPart_l,GENERIC_ALL,&ObjAttr);
	if(bPrint) printf("NtOpenPartition, ret: %X, hPart_l: %I64X\r\n",ret,hPart_l);
	if(ret >= 0)
	{
		return hPart_l;
	}
	else
	{
		ZwSuspendProcess(GetCurrentProcess());
	}
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



HANDLE mCreateEventPair(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
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
	if(bInherit)	ObjAttrEP.Attributes |= OBJ_INHERIT;
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




HANDLE mOpenSession(bool bInherit,bool bPrint)
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
		if(bInherit)	ObjAttrSession.Attributes |= OBJ_INHERIT;
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


HANDLE mCreateWaitCompletionPacket(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	
	if(bPrint)	wprintf(L"WaitCompletionPacket: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_wcp = {0};
	UniSS_wcp.Length = wcslen(ObjectName_All)*2;
	UniSS_wcp.MaxLength = UniSS_wcp.Length + 2;
	UniSS_wcp.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrWCP={0};
	ObjAttrWCP.Length = sizeof(ObjAttrWCP);
	ObjAttrWCP.Attributes= OBJ_CASE_INSENSITIVE;
	if(bInherit)	ObjAttrWCP.Attributes |= OBJ_INHERIT;
	ObjAttrWCP.ObjectName = & UniSS_wcp;

	HANDLE hWaitCompletionPacket_l = 0;
	int retValue = ZwCreateWaitCompletionPacket(&hWaitCompletionPacket_l,GENERIC_ALL,&ObjAttrWCP);
	if(bPrint) printf("ZwCreateWaitCompletionPacket, ret: %X, hWaitCompletionPacket_l: %I64X\r\n",retValue,hWaitCompletionPacket_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel WaitCompletionPacket\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hWaitCompletionPacket: %X\r\n",hWaitCompletionPacket_l);
	return hWaitCompletionPacket_l;
}


HANDLE mCreateRegistryTransaction(wchar_t* BaseName,ulong Rx,bool bInherit,bool bPrint)
{
	wchar_t ObjectName_All[MAX_PATH+1]={0};
	wcscpy(ObjectName_All,BaseName);
	ulong LenX = wcslen(ObjectName_All);
	_ultow(Rx,&ObjectName_All[LenX],0x10);
	LenX = wcslen(ObjectName_All);
	ulong Rxx = GetRandomValue();
	_ultow(Rxx,&ObjectName_All[LenX],0x10);
	
	if(bPrint)	wprintf(L"RegistryTransaction: %s\r\n",ObjectName_All);

	_UNICODE_STRING UniSS_RT = {0};
	UniSS_RT.Length = wcslen(ObjectName_All)*2;
	UniSS_RT.MaxLength = UniSS_RT.Length + 2;
	UniSS_RT.Buffer = ObjectName_All;


	_OBJECT_ATTRIBUTES ObjAttrRT={0};
	ObjAttrRT.Length = sizeof(ObjAttrRT);
	ObjAttrRT.Attributes= OBJ_CASE_INSENSITIVE;
	if(bInherit)	ObjAttrRT.Attributes |= OBJ_INHERIT;
	ObjAttrRT.ObjectName = & UniSS_RT;

	HANDLE hRegistryTransaction_l = 0;
	int retValue = NtCreateRegistryTransaction(&hRegistryTransaction_l,GENERIC_ALL,&ObjAttrRT,0);
	if(bPrint) printf("NtCreateRegistryTransaction, ret: %X, hRegistryTransaction_l: %I64X\r\n",retValue,hRegistryTransaction_l);
	if(retValue < 0)
	{
		if(bPrint)	printf("Error creating kernel RegistryTransaction object\r\n");
		ZwSuspendProcess(GetCurrentProcess());
		return 0;
	}
	if(bPrint)	printf("hRegistryTransaction: %X\r\n",hRegistryTransaction_l);
	return hRegistryTransaction_l;
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

	bool bInherit = Rand()%2;
	//---------------- ALPC ----------------------------------------------
	HANDLE hLpc_l= mCreateLpcPort(FullName,Rx,bInherit,bPrint);
	//if(bPrint) printf("LPC: %X\r\n",hLpc_l);
	hLpc = hLpc_l;

	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hLpc_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//--------------------------- Debug -----------------------------------
	HANDLE hDebug_l = mCreateDebug(FullName,Rx,bInherit,bPrint);
	//if(bPrint) printf("hDebugObject: %X\r\n",hDebug_l);
	hDebugObject = hDebug_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hDebug_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	
	//------------------------ Directory ---------------------------------
	HANDLE hDir_l = mCreateObjDir(FullName,Rx,bInherit,bPrint);
	//if(bPrint)	printf("hDirectory: %X\r\n",hDir_l);
	hDirectory = hDir_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hDir_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//------------- Private NameSpace Directory --------

	HANDLE hNameSpaceDirectory_l = mCreatePrivateNameSpaceDirectory(Name,Rx,bInherit,bPrint);
	if(bPrint)	printf("hNameSpaceDirectory: %X\r\n",hNameSpaceDirectory_l);

	hNameSpaceDirectory = hNameSpaceDirectory_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hNameSpaceDirectory_l;
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
	HANDLE hEvent_l = mCreateEvent(FullName,Rx,bInherit,bPrint);
	if(bPrint)	printf("Event: %X\r\n",hEvent_l);
	hEvent = hEvent_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hEvent_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//-------------------- Event Pair -------------------------------------
	if(OSVer == 0) //Event pair not implemented in windows 10
	{
		HANDLE hEventPair_l = mCreateEventPair(FullName,Rx,bInherit,bPrint);
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
	HANDLE hIoCompletion_l = mCreateIOCompletion(FullName,Rx,bInherit,bPrint);
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
	HANDLE hJob_l = mCreateJob(FullName,Rx,bInherit,bPrint);
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
	//---------------------- ----------
	HANDLE hPartitionJob_l = GetPartitionJob(bInherit,bPrint);
	if(!hPartitionJob_l)
	{
		if(bPrint)	printf("Error creating job object, Err: %X\r\n",GetLastError());
		if(bPrint) printf("Please launch PartitionCreator.exe first\r\n");
		return -5;
	}
	if(bPrint)	printf("PartitionJob: %X\r\n",hPartitionJob_l);
	hPartitionJob = hPartitionJob_l;
	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hPartitionJob_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//---------------------- Key ---------------------------------------------
	HKEY hKey_l = mCreateKey(Name,Rx,bPrint);
	if(bPrint)	printf("hKey: %X\r\n",hKey_l);


	hKey = hKey_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hKey_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//-------------------------- Keyed Event --------------------------------
	HANDLE hKeyedEvent_l = mCreateKeyedEvent(FullName,Rx,bInherit,bPrint);
	if(bPrint)	printf("hKeyedEvent: %X\r\n",hKeyedEvent_l);

	hKeyedEvent = hKeyedEvent_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hKeyedEvent_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//---------------- Mutex / Mutant ---------------------------
	HANDLE hMutex_l = mCreateMutex(FullName,Rx,bInherit,bPrint);
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
	HANDLE hSemaphore_l = mCreateSemaphore(FullName,Rx,bInherit,bPrint);
	if(bPrint)	printf("hSemaphore: %I64X\r\n",hSemaphore_l);
	
	hSemaphore = hSemaphore_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hSemaphore_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//---------------------- Session -----------Requires priv, try NtCreateUserProcess
	HANDLE hSession_l = mOpenSession(bInherit,bPrint);
	if(bPrint)	printf("hSession: %I64X\r\n",hSession_l);
	
	hSession = hSession_l;
	//AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hSession_l;
	//AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//-----------------------  SymLink ---------------
	HANDLE hSymLink_l = mCreateSymLink(L"\\RPC Control\\walied",Rx,bInherit,bPrint);

	if(bPrint)	printf("hSymLink: %I64X\r\n",hSymLink_l);
	hSymLink = hSymLink_l;

	
	
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hSymLink_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//--------------------- Timer ----------------------
	HANDLE hTimer_l = mCreateTimer(FullName,Rx,bInherit,bPrint);
	if(bPrint)	printf("hTimer: %I64X\r\n",hTimer_l);

	hTimer = hTimer_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTimer_l;

	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//------------------------------- TmTm Transaction Manager ------------------------
	HANDLE hTmTm_l = mCreateTm(Name,Rx,bInherit,bPrint);
	if(bPrint)	printf("hTmTm: %I64X\r\n",hTmTm_l);

	hTmTm = hTmTm_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTmTm_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//-------------------------- TmRm  Resource Manager ------------------------------
	HANDLE hTmRm_l = mCreateRm(FullName,Rx,hTmTm_l,bInherit,bPrint);
	if(bPrint)	printf("hTmRm: %I64X\r\n",hTmRm_l);

	hTmRm = hTmRm_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTmRm_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//----------------------- TmTx ------------------------
	HANDLE hTmTx_l = mCreateTx(FullName,Rx,hTmTm_l,bInherit,bPrint);
	if(bPrint)	printf("hTmTx: %I64X\r\n",hTmTx_l);
	hTmTx = hTmTx_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTmTx_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//---------------------- TmEn Enlistment --------------------------
	HANDLE hTmEn_l = mCreateEn(FullName,Rx,hTmRm_l,hTmTx_l,bInherit,bPrint);
	if(bPrint)	printf("hTmEn: %I64X\r\n",hTmEn_l);
	hTmEn = hTmEn_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hTmEn_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//------------- Token from Logon --------------------------
	HANDLE hToken2_l = mCreateTokenLogon(L"fuzz",L"Fuzz123",bPrint);
	if(bPrint)	printf("hToken (LogonUser): %I64X\r\n",hToken2_l);
	
	hToken2 = hToken2_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hToken2_l;
	AllKernelObjectsUsed++;

	ObjectName_All[0]=0;
	//---------------------- Thread Pool ----------------------
	HANDLE hTpWorkerFactory_l = mCreateTP(FullName,Rx,hIoCompletion_l,bInherit,bPrint);
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
		hPartition_l = mCreatePartition(FullName,Rx,bInherit,bPrint);
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
	//------------------------------
	HANDLE hPartition2_l = mOpenPartition(wPartName,bInherit,bPrint);
	if(!hPartition2_l)
	{
			if( Rand()%2 == 1)
			{
				hPartition2_l = (HANDLE)-1;//Current Partition
			}
			else
			{
				hPartition2_l = (HANDLE) -2;//System Partition
			}
	}

	if(bPrint) printf("hPartition2: %X\r\n",hPartition2_l);


	hPartition2 = hPartition2_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hPartition2_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//--------------- Wait Completion Packet -------
	HANDLE hWaitCompletionPacket_l = mCreateWaitCompletionPacket(FullName,Rx,bInherit,bPrint);
	if(bPrint)	printf("hWaitCompletionPacket: %X\r\n",hWaitCompletionPacket_l);
	hWaitCompletionPacket = hWaitCompletionPacket_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hWaitCompletionPacket_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//------------- Registry Transaction -------
	HANDLE hRegistryTransaction_l = mCreateRegistryTransaction(FullName,Rx,bInherit,bPrint);
	if(bPrint)	printf("hRegistryTransaction: %X\r\n",hRegistryTransaction_l);
	hRegistryTransaction = hRegistryTransaction_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hRegistryTransaction_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0] = 0;
	//------------------------- Process ---------------------------------
	HANDLE hThread_P = 0;
	HANDLE hProcess_l = mCreateProcess(&hThread_P,bPrint);
	if(bPrint)	printf("hProcess: %I64X\r\n",hProcess_l);
	hProcess = hProcess_l;

	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hProcess_l;
	AllKernelObjectsUsed++;
	ObjectName_All[0]=0;
	//----------------------- Thread --------------------------
	HANDLE hThread_l = hThread_P;
	if(bPrint)	printf("hThread: %I64X\r\n",hThread_l);

	hThread = hThread_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hThread_l;
	AllKernelObjectsUsed++;
	//---------------------- Token ------------------------------------
	HANDLE hToken_l = mOpenTokenProcess(hProcess_l,bPrint);
	if(bPrint)	printf("hToken: %I64X\r\n",hToken_l);
	
	hToken = hToken_l;
	AllKernelObject[AllKernelObjectsUsed] = (ulonglong)hToken_l;
	AllKernelObjectsUsed++;

	ObjectName_All[0]=0;
	//--------------
	AllKernelObject[0] = -1; //GetCurrentProcess();
	AllKernelObject[0] = AllKernelObject[Rand()%AllKernelObjectsUsed];
	return 0;
}


void ObjCreatorDestroyerThread()
{
	srand(time(NULL));
	while(1)
	{
		Sleep(10 * 60 * 1000);
		if(hProcess)
		{
			/*
			int retX = ZwTerminateThread(hThread,GetRandomNTStatusCode());
			if(retX < 0)
			{
				printf("ZwTerminateThread, ret: %X (hThread: %I64X)\r\n",retX,hThread);
				ExitProcess(0);
			}
			*/
			int retX = ZwTerminateProcess(hProcess,GetRandomNTStatusCode());
			if(retX < 0)
			{
				//printf("ZwTerminateProcess, ret: %X (hProcess: %I64X)\r\n",retX,hProcess);

				if(retX != 0xC000010A)
				{
					ExitProcess(0);
				}
				else
				{
					HANDLE hThread_P = 0;
					HANDLE hProcess_l = mCreateProcess(&hThread_P,false);
					hProcess = hProcess_l;
					hThread = hThread_P;
				}
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
		/*
		if(InitKernelObjects(false))
		{
			printf("Error creating dummy Kernel objects\r\n");
			ExitProcess(0);
		}
		*/
		cached_AllKernelObjectsUsed = AllKernelObjectsUsed;
		cached_AllFilesUsed = AllFilesUsed;
		cached_AllProcessesUsed = AllProcessesUsed;
		//--------------------------

	}
}


void ProcessCreatorDestroyer()
{
	srand(time(NULL));
	while(1)
	{
		Sleep(1000);
		if(hProcess)
		{
			int retX = ZwResumeProcess(hProcess);
			if(retX < 0)
			{
				//0xC000010A
				printf("ZwResumeProcess, ret: %X (hProcess: %I64X)\r\n",retX,hProcess);
				

				HANDLE hThread_P = 0;
				HANDLE hProcess_l = mCreateProcess(&hThread_P,false);
				hProcess = hProcess_l;
				hThread = hThread_P;
			}
		}
	}
}
