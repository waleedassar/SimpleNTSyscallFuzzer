#pragma once
#include "windows.h"




#define RAND_MAX 0xFFFFFFFF
#define MAX_ARGS 20 // 18 == for ==> Windows 7/10, check KiSystemServiceCopyStart
#define RANDOM_PAGE_SIZE 0x1000
#define DEFAULT_ALIGNMENT 0x4


#define MAX_UNI_STRS 0x1000


//Can't be zero
#define MAX_BRUTE 0xFFFF




#define ulong unsigned long
#define ulonglong unsigned long long
#define longlong long long
#define ULONG unsigned long
#define ULONGLONG unsigned long long
#define ushort unsigned short
#define USHORT unsigned short
#define uchar unsigned char
#define UCHAR unsigned char




//Limited to 20 arguments max
typedef longlong(*MYFUNC)(ulonglong A1,
					ulonglong A2,
					ulonglong A3,
					ulonglong A4,
					ulonglong A5,
					ulonglong A6,
					ulonglong A7,
					ulonglong A8,
					ulonglong A9,
					ulonglong A10,
					ulonglong A11,
					ulonglong A12,
					ulonglong A13,
					ulonglong A14,
					ulonglong A15,
					ulonglong A16,
					ulonglong A17,
					ulonglong A18,
					ulonglong A19,
					ulonglong A20);


typedef void (*PREFUNC)(unsigned long SysCall,
						unsigned long long* Args,
						void** pPool,
						void** pSecondLevelPool);


typedef void (*POSTFUNC)(unsigned long SysCall,
						 unsigned long long* Args,
						 void** pPool,
						 void** pSecondLevelPool);

union _OBJECT_HANDLE_INFORMATION
{
	struct
	{
	   ulong HandleAttributes;
	   ulong GrantedAccess;
	}S;
	ulonglong All;
};

struct _MY_SECURITY_DESCRIPTOR
{
   uchar Revision;
   uchar Sbz1;
   ushort Control;
   ulong OffsetOwner; //at 4
   ulong OffsetGroup; //at 8
   ulong OffsetSacl;//at 0C
   ulong OffsetDacl;//at 10

};

#define OBJ_PROTECT_CLOSE 		0x00000001
#define OBJ_INHERIT             0x00000002
#define OBJ_AUDIT_OBJECT_CLOSE  0x00000004
#define OBJ_PERMANENT           0x00000010
#define OBJ_EXCLUSIVE           0x00000020
#define OBJ_CASE_INSENSITIVE    0x00000040
#define OBJ_OPENIF              0x00000080
#define OBJ_OPENLINK            0x00000100
#define OBJ_KERNEL_HANDLE       0x00000200
#define OBJ_FORCE_ACCESS_CHECK  0x00000400
#define OBJ_VALID_ATTRIBUTES    0x000007F2
#define OBJ_CREATOR_INFO_MINE   0x00010000

struct _UNICODE_STRING
{
	unsigned short Length;
	unsigned short MaxLength;
	unsigned long Pad;
	wchar_t* Buffer;
};


struct _OBJECT_ATTRIBUTES {
  ULONGLONG           Length;
  HANDLE          RootDirectory;
  _UNICODE_STRING* ObjectName;
  ULONGLONG           Attributes;
  PVOID           SecurityDescriptor;
  PVOID           SecurityQualityOfService;
};

/*
enum _SECURITY_IMPERSONATION_LEVEL
{
   SecurityAnonymous = 0,
   SecurityIdentification = 1,
   SecurityImpersonation = 2,
   SecurityDelegation =3
};
*/

//check sizeof, must be 0xC
/*
struct _SECURITY_QUALITY_OF_SERVICE
{
   ulong Length;
   _SECURITY_IMPERSONATION_LEVEL ImpersonationLevel ;
   unsigned char ContextTrackingMode;
   unsigned char EffectiveOnly;
};*/


#define OBJ_PROTECT_CLOSE 		0x00000001
#define OBJ_INHERIT             0x00000002
#define OBJ_AUDIT_OBJECT_CLOSE  0x00000004
#define OBJ_PERMANENT           0x00000010
#define OBJ_EXCLUSIVE           0x00000020
#define OBJ_CASE_INSENSITIVE    0x00000040
#define OBJ_OPENIF              0x00000080
#define OBJ_OPENLINK            0x00000100
#define OBJ_KERNEL_HANDLE       0x00000200
#define OBJ_FORCE_ACCESS_CHECK  0x00000400
#define OBJ_VALID_ATTRIBUTES    0x000007F2
#define OBJ_CREATOR_INFO_MINE   0x00010000

#define KernelMode 0
#define UserMode 1

#define SEC_IMAGE           0x1000000
#define SEC_NEVER_RELOCATE 0x8000000 //SEC_DATA
#define SEC_UNK 0x100000
#define SEC_PROTECTED_IMAGE 0x2000000
#define SEC_FOR_DRIVER 0x400000
#define SEC_UNK2 0x4000000
#define SEC_UNK3 0x10000000
#define SEC_UNK4 0x40000000
#define SEC_UNK5 0x80000000


#define 	LPC_REQUEST   1
#define 	LPC_REPLY   2
#define 	LPC_DATAGRAM   3
#define 	LPC_LOST_REPLY   4
#define 	LPC_PORT_CLOSED   5
#define 	LPC_CLIENT_DIED   6
#define 	LPC_EXCEPTION   7
#define 	LPC_DEBUG_EVENT   8
#define 	LPC_ERROR_EVENT   9
#define 	LPC_CONNECTION_REQUEST   10





struct _CLIENT_ID
{
	ulonglong UniqueProcess;
	ulonglong UniqueThread;
};

//Size 0x28
struct _PORT_MESSAGE
{
	ushort DataLength;
	ushort TotalLength;
	ushort Type;
	ushort DataInfoOffset;
	_CLIENT_ID ClientId;
	ulong MessageId;
	ulong Pad;
	ulonglong ClientViewSize;
};

struct _CLIENT_ID32
{
	ulong UniqueProcess;
	ulong UniqueThread;
};


struct _PORT_MESSAGE32
{
	ushort DataLength;
    ushort TotalLength;
	ushort Type;
	ushort DataInfoOffset;
	_CLIENT_ID32 ClientId;
	ulong MessageId;
	ulong ClientViewSize;
};

struct _ETW_CONSUMER_INPUT_OUTPUT
{
	ulong LoggerId;//at 0x0
	ulong NumberOfBlocks;//at 0x4, (0x1  becomes 0x80f8)
	void* pAddr0;//at 0x8
	void* pAddr1;//at 0x10
	HANDLE hEvent0;//at 0x18
	HANDLE hEvent1;//at 0x20
	void* pAddr2;//at 0x28
	void* pAddr3;//at 0x30
	HANDLE hEtwConsumer;//at 0x38
	ulonglong Pad0;//at 0x40
	ulonglong Pad1;//at 0x48
};

struct _ETW_REGISTRATION_INPUT_OUTPUT
{
	GUID guid;
	ulonglong Unk0;
	HANDLE hEtwReg;
	unsigned char Pad[0x80];
};

void FillRandomObjectAttributes(void* p,ulong Size);
wchar_t* GetRandomWideString(wchar_t* pMem,ulong tLength);
void PrintAllArgs(unsigned long long* Args);

struct APC_LOOP_THREAD
{
	ulonglong  ThreadId;
	HANDLE hEvent;
};

void Papcfunc(ULONG_PTR hEventX);
void ApcLoopThread(APC_LOOP_THREAD* pApcLoopThread);

struct _ALPC_PORT_ATTRIBUTES
{
   ulong Flags;
   _SECURITY_QUALITY_OF_SERVICE SecurityQos;
   ulonglong MaxMessageLength;
   ulonglong MemoryBandwidth;
   ulonglong MaxPoolUsage;
   ulonglong MaxSectionSize;
   ulonglong MaxViewSize;
   ulonglong MaxTotalSectionSize;
   ulong DupObjectTypes;
   ulong Reserved;
};



typedef struct pAddr
{
	ulong X; //At 0x0 Must be Zero
	ulong Flags;//At 0x04
	_UNICODE_STRING Uni; //At 0x08, sz 0x10
	ushort X0; //At 0x18 
	ushort X1; //At 0x1A
	ulong NumberOfUnicodeStrings; //at 0x1C
	void* pUnicodeStrings;//At 0x20
}PADDR;


struct _KAFFINITY_EX
{
	ushort Count;
	ushort Size;
	ulong Reserved;
	ulonglong Bitmap[4];
};

typedef int(*fNtOpenPartition)(HANDLE* phPartition,ulonglong DesiredAccess,_OBJECT_ATTRIBUTES* pObjAttr);
typedef int(*fNtCreatePartition)(HANDLE hPartition,ulonglong pOutHandle,ulonglong DesiredAccess,ulonglong pObjAttr);
typedef int(*fNtManagePartition)(HANDLE hPartition,HANDLE hSecondaryPartition,ulonglong InfoClass,ulonglong pInfo,ulonglong InfoLength);


typedef int(*fNtCreateRegistryTransaction)(HANDLE *pHandleOut, ulonglong DesiredAccess, _OBJECT_ATTRIBUTES *pObjAttr, ulonglong FLags);

typedef ulonglong (*fNtCreateEnclave)(
HANDLE hProcess, 
ulonglong BaseAddress, 
ulonglong ZeroBits, 
ulonglong Size, 
ulonglong InitialCommitment, 
ulonglong EnclaveType, 
ulonglong EnclaveInformation, 
ulonglong EnclaveInformationLength, 
ulonglong pErrorStatus);

typedef ulonglong (*fNtInitializeEnclave)(
HANDLE hProcess, 
ulonglong Address, 
ulonglong pData, 
ulonglong DataLength, 
ulonglong pOutValue);

bool Acquire_LockMemory();

extern "C"
{
	int ZwCreatePort (HANDLE* pPortHandle,
					_OBJECT_ATTRIBUTES* ObjectAttributes,
					ulonglong MaxConnectInfoLength,
					ulonglong MaxDataLength,
					void* Reserved);
	int ZwAlpcCreatePort(HANDLE* pPortHandle, _OBJECT_ATTRIBUTES* ObjectAttributes, _ALPC_PORT_ATTRIBUTES* PortAttributes);

	int ZwCallbackReturn(void* pResult,ulonglong ResultLength,ulonglong Status);

	int ZwCreateSection(HANDLE* SectionHandle,
						ACCESS_MASK DesiredAccess,
						_OBJECT_ATTRIBUTES* ObjectAttributes,
						_LARGE_INTEGER* MaximumSize,
						ulonglong SectionPageProtection,
						ulonglong AllocationAttributes,
						HANDLE FileHandle);


	int ZwTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus);

	int ZwResumeProcess(HANDLE hProcess);

	int RtlGetVersion(PRTL_OSVERSIONINFOW lpVersionInformation);

	int ZwCreateJobObject(HANDLE* JobHandle, 
						ACCESS_MASK DesiredAccess, 
						_OBJECT_ATTRIBUTES* ObjectAttributes);

	int ZwCreateDebugObject(HANDLE *DebugObjectHandle, 
		ACCESS_MASK DesiredAccess, 
		_OBJECT_ATTRIBUTES *ObjectAttributes, 
		ulonglong Flags);

	int ZwCreateDirectoryObject(HANDLE* DirectoryHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes);

	int ZwClose(HANDLE Handle);

	int ZwTraceControl(
			ulonglong FunctionCode, 
			void* InBuffer, 
			ulonglong InBufferLen, 
			void* OutBuffer, 
			ulonglong OutBufferLen, 
			ulonglong* ReturnLength);


	int ZwCreateEventPair(HANDLE* EventPairHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes);

	int ZwCreateIoCompletion(HANDLE* IoCompletionHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,__int64 Count);

	int ZwAllocateReserveObject(HANDLE* MemoryReserveHandle,_OBJECT_ATTRIBUTES* ObjectAttributes,ulong  ReserveType );

	int ZwCreateKeyedEvent( HANDLE* KeyedEventHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,ulonglong Flags );

	int ZwPowerInformation(ulonglong InformationLevel, 
						void* InputBuffer,
						ulonglong InputBufferLength,
						void* OutputBuffer,
						ulonglong OutputBufferLength) ;

	int ZwCreateProfile(HANDLE* phProfile,HANDLE hProcess,void* ProfileBase,ulonglong ProfileSize,ulonglong BucketSize,
					void* Buffer,ulonglong BufferSize,ulonglong ProfileSource,ulonglong Affinity );


	int ZwOpenSession(HANDLE* SessionHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes);

	int ZwCreateSymbolicLinkObject(HANDLE* LinkHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes, _UNICODE_STRING* LinkTarget);
	int ZwOpenSymbolicLinkObject(HANDLE* LinkHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes);

	int ZwSuspendProcess(HANDLE ProcessHandle);
	int ZwTerminateThread(HANDLE ThreadHandle, ulong ExitStatus);

	int ZwCreateEvent(HANDLE* EventHandle, 
				ACCESS_MASK DesiredAccess, 
				_OBJECT_ATTRIBUTES* ObjectAttributes, 
				ulonglong EventType, 
				ulonglong InitialState);
	int ZwCreateMutant(HANDLE* MutantHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes, bool InitialOwner);
	int ZwCreateTimer( HANDLE* TimerHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,ulonglong TimerType );
	int ZwCreateSemaphore(HANDLE* SemaphoreHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes, LONG InitialCount, LONG MaximumCount);


	int ZwCreateTransactionManager(HANDLE* TmHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,_UNICODE_STRING* LogFileName,ulonglong CreateOptions,ulonglong CommitStrength);

	int ZwCreateResourceManager(HANDLE* ResourceManagerHandle, ACCESS_MASK DesiredAccess, HANDLE TmHandle, void* RmGuid, _OBJECT_ATTRIBUTES* ObjectAttributes, ulonglong CreateOptions, _UNICODE_STRING* Description);

	int ZwCreateTransaction(HANDLE* TransactionHandle, 
		ACCESS_MASK DesiredAccess,
		_OBJECT_ATTRIBUTES* ObjectAttributes,
		LPGUID Uow, 
		HANDLE TmHandle,
		__int64 CreateOptions, 
		__int64 IsolationLevel, 
		__int64 IsolationFlags,
		_LARGE_INTEGER* Timeout, 
		_UNICODE_STRING* Description);

	int ZwCreateEnlistment(HANDLE* EnlistmentHandle,
		ACCESS_MASK DesiredAccess,
		HANDLE ResourceManagerHandle,
		HANDLE TransactionHandle,
		_OBJECT_ATTRIBUTES* ObjectAttributes,
		ulonglong CreateOptions,
		ulonglong NotificationMask,
		void* EnlistmentKey ) ;

	int ZwCreateWorkerFactory( HANDLE* WorkerFactoryHandleReturn,
				ACCESS_MASK DesiredAccess,
				_OBJECT_ATTRIBUTES* ObjectAttributes,
				HANDLE CompletionPortHandle,
				HANDLE WorkerProcessHandle,
				void* StartRoutine,
				void* StartParameter,
				ulonglong MaxThreadCount,
				ulonglong StackReserve,
				ulonglong StackCommit );

	int ZwDelayExecution(bool Alertable,ulonglong* pInterval);
	int ZwQueueApcThread(HANDLE ThreadHandle,void* ApcRoutine,void* ApcArgument1,void* ApcArgument2,void* ApcArgument3);
	int ZwSetEvent(HANDLE EventHandle,  ulong* PreviousState);
	int ZwResetEvent(HANDLE EventHandle, ulong* PreviousState);

	int ZwCreateWaitCompletionPacket(HANDLE* WaitCompletionPacketHandle,
		ACCESS_MASK DesiredAccess,
		_OBJECT_ATTRIBUTES* ObjectAttributes );

	int ZwCreateRegistryTransaction(HANDLE *pHandleOut, ulonglong DesiredAccess, _OBJECT_ATTRIBUTES *pObjAttr, ulonglong FLags);


	//HANDLE GetCurrentThreadToken();
	//HANDLE GetCurrentProcessToken();
	//HANDLE GetCurrentThreadEffectiveToken();

	//BOOL OpenProcessToken(HANDLE  ProcessHandle,ulonglong   DesiredAccess,HANDLE* TokenHandle);


	//Windows 7, prototype has changed in Windows 10
	int ZwQueueApcThread(HANDLE ThreadHandle,void* ApcRoutine,void* ApcArgument1,void* ApcArgument2,void* ApcArgument3);
}


void DummyAPCRoutine(ulonglong Dum);

int MigrateSyscalls(unsigned long SyscallCount);

void DumpHex_8(const void* data, size_t size);


void J_NtCallbackReturn(void* pXX,ulong SizeXX,ulonglong Index,void* pKernelCallbackTableX);

struct RANDOMIZATION_THREAD
{
	void** pPool;
	ulonglong Count;
	void** pSecondLevelPool;
	ulonglong SecondLevelCount;
};

ulong GetRandomNTStatusCode();


void InitOsInfo();

bool ShouldSkipLeakChecking(unsigned long SysCall);

void RandomizeOrderAndCounts(ulong* pOrder,ulong* pCounts,ulong Count);

//void* InitFunctionTable(unsigned long SyscallCount, unsigned long MinSyscallOrdinal,MYFUNC* pFuncTable,PREFUNC* pPreTable,POSTFUNC* pPostTable);
void* InitFunctionTable(unsigned long SyscallCount, unsigned long MinSyscallOrdinal,MYFUNC* pFuncTable,PREFUNC* pPreTable,POSTFUNC* pPostTable,ulong OSVer);
void  DestroyFunctionTable(void* pExecPages,unsigned long SyscallCount,MYFUNC* pFuncTable,PREFUNC* pPreTable,POSTFUNC* pPostTable);





//bool Scan(void* pMem,ulong Size);
bool Scan(void* pMem,ulong Size,ulong OSVer);

int InitKernelObjects(bool bPrint);

void ObjCreatorDestroyerThread();
void ObjDestroyerThread();

void FillRandomUnicodeString(void* p,ulong Size);

void RandomiztionThread(RANDOMIZATION_THREAD* pRandThr);

bool IsBlockingSyscall(unsigned long SysCall,bool bPrint);

longlong Func(ulonglong A, ulonglong B, ulonglong C);

MYFUNC CreateFunction(unsigned long SyscallNumber);




void** CreatePagesPool(unsigned long NumArgs);
void DestroyPagesPool(void** pPool,unsigned long NumArgs);

void ModifyFunction(MYFUNC pFunc,unsigned long SyscallNumber);
void DestroyFunction(MYFUNC pFunc);





void RandomizeTebStuff();

ulonglong GetClassicRandomValue();
void FillClassicRandomData(void* pMem,ulong Size,bool bMode);

bool ScanCanonical(void* pMem,ulonglong Size);


unsigned long GetSyscallCount(bool bWin32k);
unsigned long GetSyscallCount_i(MYFUNC pFunc,bool bWin32k);


wchar_t* GetRandomWideString(wchar_t* pMem,ulong tLength);
char* GetRandomAsciiString(char* pMem,ulong tLength);

ulong* GetRandomSyscallOrder_Swap(unsigned long Min,unsigned long Max);


ulonglong GetRandomValue();

void FillRandomData(void* pMem,ulong Size);



void* GetRandomPage();
void DestroyRandomPage(void* pNew);


void DummyPost(unsigned long SysCall,
			unsigned long long* Args,
			void** pPool,
			void** pSecondLevelPool);

void DummyPre(unsigned long SysCall,
						unsigned long long* Args,
						void** pPool,
						void** pSecondLevelPool);


int BruteDeferred(unsigned long Count,unsigned long* pSkippedList,ulong NumArgs, ulonglong* Args,void** pPool,void** pSecondLevelPool,ulong MaxIterationCount,bool bRandomTeb,char* pTeb,HANDLE hThread);

void* GetDummyExecutablePage();

void DestroyDummyExecutablePage(void* pNew);

unsigned long Rand();


void CancelSyscall(unsigned long long* Args);

void ModifyArguments0(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments1(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments2(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments3(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments4(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);

void ModifyArguments5(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments6(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments7(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments8(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void ModifyArguments9(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);



void CleanArguments(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
bool CallHasBugs(unsigned long SysCall,bool bPrint);
bool ShouldDeferSyscall(unsigned long SysCall,bool bPrint);
int LogOrderToDisk(ulong* pOrder,ulong Count);
//-------------------









ulong* GetRandomSyscallOrder(unsigned long Min,unsigned long Max);






void DummyThread();

bool CallHasReturnValueLeak(unsigned long SysCall,bool bPrint);
//bool ScanRegister(ulonglong Register,bool bMode);
bool ScanRegister(ulonglong Register,bool bMode,ulong OSVer);


void DumpHex(const void* data, size_t size);




struct _BOUNDARY_DESCRIPTOR_INPUT_TLV
{
	ulong Type;
	ulong Length;
	//uchar data[Length]
};



struct _BOUNDARY_DESCRIPTOR_INPUT_BASIC
{
	ulong Revision;//at 0x0, must be 1
	ulong NumberOfEntries; //at 0x4
	ulong Size; //At 0x8
	ulong UNKZ;//at 0x0C
	//Optional members here TLVs
};


extern "C"
{
	int ZwDeletePrivateNamespace(HANDLE NamespaceHandle); 
	int NtDeletePrivateNamespace(HANDLE NamespaceHandle); 

	int ZwCreatePrivateNamespace(HANDLE* NamespaceHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,void* BoundaryDescriptor);
	int NtCreatePrivateNamespace(HANDLE* NamespaceHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,void* BoundaryDescriptor);

	int NtOpenPrivateNamespace(HANDLE* NamespaceHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,void* BoundaryDescriptor);
	int ZwOpenPrivateNamespace(HANDLE* NamespaceHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,void* BoundaryDescriptor);
}


extern "C"
{
	int ZwOpenJobObject( HANDLE* JobHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes );
}



#define 	RTL_USER_PROC_PARAMS_NORMALIZED   0x00000001


struct _PS_CREATE_INFO
{
	ulonglong Size;
	ulonglong State;//at 0x8
	ulong InitFlags;//at 0x10
	ulong AdditionalFileAccess;//at 0x14
	ulonglong X0;
	ulonglong X1;
	ulonglong X2;
	ulonglong X3;
	ulonglong X4;
	ulonglong X5;
	ulonglong X6;
	ulonglong X7;
};


struct _PS_ATTRIBUTE
{
	ulonglong Attribute;
    ulonglong Size;
    ulonglong Value;
    ulonglong ReturnLength;
};


struct _PS_ATTRIBUTE_LIST
{
   ulonglong TotalLength;
   _PS_ATTRIBUTE Attributes[1];
};


extern "C"
{

	int ZwCreateProcessEx(HANDLE* ProcessHandle, 
						ACCESS_MASK DesiredAccess, 
						_OBJECT_ATTRIBUTES* ObjectAttributes, 
						HANDLE ParentProcess, 
						ulonglong Flags, 
						HANDLE SectionHandle, 
						HANDLE DebugPort, 
						HANDLE ExceptionPort, 
						ulonglong JobMemberLevel);
 

	void RtlInitUnicodeString(_UNICODE_STRING*         DestinationString, wchar_t* SourceString);

	int RtlCreateProcessParametersEx(
    void **pProcessParameters,
    _UNICODE_STRING* ImagePathName,
    _UNICODE_STRING* DllPath,
    _UNICODE_STRING* CurrentDirectory,
    _UNICODE_STRING* CommandLine,
    void* Environment,
    _UNICODE_STRING* WindowTitle,
    _UNICODE_STRING* DesktopInfo,
    _UNICODE_STRING* ShellInfo,
    _UNICODE_STRING* RuntimeData,
    ulonglong Flags);

	int ZwCreateUserProcess(HANDLE* ProcessHandle, 
							HANDLE* ThreadHandle,
							ACCESS_MASK ProcessDesiredAccess,
							ACCESS_MASK ThreadDesiredAccess,
							_OBJECT_ATTRIBUTES* ProcessObjectAttributes,
							_OBJECT_ATTRIBUTES* ThreadObjectAttributes,
							ulonglong ProcessFlags,
							ulonglong ThreadFlags,
							void* ProcessParameters, 
							void* CreateInfo,
							_PS_ATTRIBUTE_LIST* AttributeList);
}



HANDLE GetPartitionJob(bool bInherit,bool bPrint);


void ProcessCreatorDestroyer();
//----------------------------------

void w7NtMapUserPhysicalPagesScatter(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitForSingleObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCallbackReturn(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReadFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeviceIoControlFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWriteFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRemoveIoCompletion(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReleaseSemaphore(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReplyWaitReceivePort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReplyPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtClose(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtEnumerateValueKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFindAtom(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryDefaultLocale(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryValueKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAllocateVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitForMultipleObjects32(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWriteFileGather(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFreeVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtImpersonateClientOfPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReleaseMutant(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRequestWaitReplyPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenThreadToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtMapViewOfSection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAccessCheckAndAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUnmapViewOfSection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReplyWaitReceivePortEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtTerminateProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetEventBoostPriority(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReadFileScatter(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenThreadTokenEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenProcessTokenEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryPerformanceCounter(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtEnumerateKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDelayExecution(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryDirectoryFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySystemInformation(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenSection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryTimer(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFsControlFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWriteVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCloseObjectAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDuplicateObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryAttributesFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtClearEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReadVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAdjustPrivilegesToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDuplicateToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtContinue(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryDefaultUILanguage(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueueApcThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtYieldExecution(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAddAtom(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryVolumeInformationFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateSection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFlushBuffersFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtApphelpCacheControl(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateProcessEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtIsProcessInJob(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtProtectVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtResumeThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtTerminateThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReadRequestData(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWriteRequestData(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenDirectoryObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAccessCheckByTypeAndAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySystemTime(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitForMultipleObjects(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCancelIoFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtTraceEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPowerInformation(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetValueKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCancelTimer(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetTimer(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAcceptConnectPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAccessCheck(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAccessCheckByType(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAccessCheckByTypeResultList(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAccessCheckByTypeResultListAndAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAccessCheckByTypeResultListAndAuditAlarmByHandle(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAddBootEntry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAddDriverEntry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAdjustGroupsToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlertResumeThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlertThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAllocateLocallyUniqueId(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAllocateReserveObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAllocateUserPhysicalPages(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAllocateUuids(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcAcceptConnectPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcCancelMessage(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcConnectPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcCreatePort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcCreatePortSection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcCreateResourceReserve(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcCreateSectionView(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcCreateSecurityContext(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcDeletePortSection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcDeleteResourceReserve(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcDeleteSectionView(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcDeleteSecurityContext(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcDisconnectPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcImpersonateClientOfPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcOpenSenderProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcOpenSenderThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcQueryInformation(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcQueryInformationMessage(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcRevokeSecurityContext(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcSendWaitReceivePort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAlpcSetInformation(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAreMappedFilesTheSame(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtAssignProcessToJobObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCancelIoFileEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCancelSynchronousIoFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCommitComplete(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCommitEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCommitTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCompactKeys(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCompareTokens(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7xHalGetInterruptTranslator(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCompressKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtConnectPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateDebugObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateDirectoryObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateIoCompletion(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateJobObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateJobSet(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateKeyTransacted(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateKeyedEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateMailslotFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateMutant(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateNamedPipeFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreatePagingFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreatePort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreatePrivateNamespace(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateProfile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateProfileEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateResourceManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateSemaphore(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateSymbolicLinkObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateThreadEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateTimer(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateUserProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateWaitablePort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtCreateWorkerFactory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDebugActiveProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDebugContinue(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeleteAtom(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeleteBootEntry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeleteDriverEntry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeleteFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeleteKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeleteObjectAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeletePrivateNamespace(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDeleteValueKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDisableLastKnownGood(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDisplayString(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtDrawText(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtEnableLastKnownGood(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtEnumerateBootEntries(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtEnumerateDriverEntries(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtEnumerateSystemEnvironmentValuesEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtEnumerateTransactionObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtExtendSection(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFilterToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFlushInstallUILanguage(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFlushInstructionCache(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFlushKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFlushProcessWriteBuffers(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFlushVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFlushWriteBuffer(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFreeUserPhysicalPages(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFreezeRegistry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtFreezeTransactions(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetContextThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetCurrentProcessorNumber(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetDevicePowerState(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetMUIRegistryInfo(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetNextProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetNextThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetNlsSectionPtr(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetNotificationResourceManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetPlugPlayEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtGetWriteWatch(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtImpersonateAnonymousToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtImpersonateThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtInitializeNlsFiles(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtInitializeRegistry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtInitiatePowerAction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtIsSystemResumeAutomatic(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtIsUILanguageComitted(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtListenPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLoadDriver(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLoadKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLoadKey2(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLoadKeyEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLockFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLockProductActivationKeys(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLockRegistryKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtLockVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtMakePermanentObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtMakeTemporaryObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtMapCMFModule(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtMapUserPhysicalPages(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtModifyBootEntry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtModifyDriverEntry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtNotifyChangeDirectoryFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtNotifyChangeKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtNotifyChangeMultipleKeys(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtNotifyChangeSession(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenIoCompletion(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenJobObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenKeyEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenKeyTransacted(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenKeyTransactedEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenKeyedEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenMutant(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenObjectAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenPrivateNamespace(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenProcessToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenResourceManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenSemaphore(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenSession(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenSymbolicLinkObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenTimer(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtOpenTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPlugPlayControl(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPrePrepareComplete(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPrePrepareEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPrepareComplete(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPrepareEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPrivilegeCheck(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPrivilegeObjectAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPrivilegedServiceAuditAlarm(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPropagationComplete(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPropagationFailed(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtPulseEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryBootEntryOrder(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryBootOptions(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryDebugFilterState(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryDirectoryObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryDriverEntryOrder(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryEaFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryFullAttributesFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationAtom(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationJobObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationResourceManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInformationWorkerFactory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryInstallUILanguage(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryIntervalProfile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryIoCompletion(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryLicenseValue(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryMultipleValueKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryMutant(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryOpenSubKeys(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryOpenSubKeysEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryPortInformationProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryQuotaInformationFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySecurityAttributesToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySecurityObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySemaphore(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySymbolicLinkObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySystemEnvironmentValue(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySystemEnvironmentValueEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQuerySystemInformationEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueryTimerResolution(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtQueueApcThreadEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRaiseException(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRaiseHardError(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReadOnlyEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRecoverEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRecoverResourceManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRecoverTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRegisterProtocolAddressInformation(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRegisterThreadTerminatePort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReleaseKeyedEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReleaseWorkerFactoryWorker(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRemoveIoCompletionEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRemoveProcessDebug(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRenameKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRenameTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReplaceKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReplacePartitionUnit(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtReplyWaitReplyPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRequestPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtResetEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtResetWriteWatch(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRestoreKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtResumeProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRollbackComplete(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRollbackEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRollbackTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtRollforwardTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSaveKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSaveKeyEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSaveMergedKeys(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSecureConnectPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSerializeBoot(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetBootEntryOrder(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetBootOptions(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetContextThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetDebugFilterState(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetDefaultHardErrorPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetDefaultLocale(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetDefaultUILanguage(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetDriverEntryOrder(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetEaFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetHighEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetHighWaitLowEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationDebugObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationEnlistment(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationJobObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationResourceManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationToken(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetInformationWorkerFactory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetIntervalProfile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetIoCompletion(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetIoCompletionEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7xKdSetupPciDeviceForDebugging(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetLowEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetLowWaitHighEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetQuotaInformationFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetSecurityObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetSystemEnvironmentValue(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetSystemEnvironmentValueEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetSystemInformation(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetSystemPowerState(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetSystemTime(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetThreadExecutionState(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetTimerEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetTimerResolution(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetUuidSeed(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSetVolumeInformationFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtShutdownSystem(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtShutdownWorkerFactory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSignalAndWaitForSingleObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSinglePhaseReject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtStartProfile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtStopProfile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSuspendProcess(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSuspendThread(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtSystemDebugControl(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtTerminateJobObject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtTestAlert(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtThawRegistry(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtThawTransactions(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtTraceControl(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtTranslateFilePath(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUmsThreadYield(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUnloadDriver(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUnloadKey(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUnloadKey2(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUnloadKeyEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUnlockFile(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtUnlockVirtualMemory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtVdmControl(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitForDebugEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitForKeyedEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitForWorkViaWorkerFactory(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitHighEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWaitLowEventPair(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w7NtWorkerFactoryWorkerReady(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);


//----------------- win10

void w10xKdEnumerateDebuggingDevices (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10xHalAllocatePmcCounterSet (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtWaitForAlertByThreadId (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtUpdateWnfStateData (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtUnsubscribeWnfStateChange (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtUnmapViewOfSectionEx (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSubscribeWnfStateChange (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSinglePhaseReject_0 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetWnfProcessNotificationEvent (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetTimer2 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetIRTimer (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetInformationVirtualMemory (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetInformationTransactionManager_0 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetInformationSymbolicLink (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetCachedSigningLevel (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRollforwardTransactionManager_0 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRevertContainerImpersonation (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRenameTransactionManager_0 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRegisterProtocolAddressInformation_0 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtQueryWnfStateNameInformation (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtQueryWnfStateData (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtOpenPartition (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtManagePartition (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtLoadEnclaveData (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtInitializeEnclave (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtGetCurrentProcessorNumberEx (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtGetCompleteWnfStateSubscription (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtGetCachedSigningLevel (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtFlushBuffersFileEx (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtFilterBootOption (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtDeleteWnfStateName (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtDeleteWnfStateData (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateWnfStateName (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateWaitCompletionPacket (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateTokenEx (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateTimer2 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreatePartition (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateLowBoxToken (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateIRTimer (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateEnclave (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateDirectoryObjectEx (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCompareObjects (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCancelWaitCompletionPacket (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCancelTimer2 (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAssociateWaitCompletionPacket (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAlpcImpersonateClientContainerOfPort (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAlpcConnectPortEx (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAlertThreadByThreadId (unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAddAtomEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);

void w10NtNullSyscall(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAcquireCrossVmMutant(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAcquireProcessActivityReference(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAdjustTokenClaimsAndDeviceGroups(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAllocateUserPhysicalPagesEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtAllocateVirtualMemoryEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCallEnclave(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCommitRegistryTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCompareSigningLevels(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCompleteConnectPort(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtContinueEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtConvertBetweenAuxiliaryCounterAndPerformanceCounter(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateCrossVmEvent(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateCrossVmMutant(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateRegistryTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtCreateSectionEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtDirectGraphicsCall(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtFilterTokenEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtManageHotPatch(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtMapViewOfSectionEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtNotifyChangeDirectoryFileEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtOpenRegistryTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtPssCaptureVaSpaceBulk(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtQueryAuxiliaryCounterFrequency(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtQueryDirectoryFileEx(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtQueryInformationByName(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtQuerySecurityPolicy(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRegisterProtocolAddressInformation(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRenameTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRollbackRegistryTransaction(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtRollforwardTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetCachedSigningLevel2(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetInformationTransactionManager(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSetLdtEntries(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtSinglePhaseReject(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtTerminateEnclave(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);
void w10NtLoadKey3(unsigned long SysCall,unsigned long long* Args,void** pPool,void** pSecondLevelPool);


//--------
#define macr_NtAccessCheck 0x0
#define macr_NtWorkerFactoryWorkerReady 0x1
#define macr_NtAcceptConnectPort 0x2
#define macr_NtMapUserPhysicalPagesScatter 0x3
#define macr_NtWaitForSingleObject 0x4
#define macr_NtCallbackReturn 0x5
#define macr_NtReadFile 0x6
#define macr_NtDeviceIoControlFile 0x7
#define macr_NtWriteFile 0x8
#define macr_NtRemoveIoCompletion 0x9
#define macr_NtReleaseSemaphore 0xa
#define macr_NtReplyWaitReceivePort 0xb
#define macr_NtReplyPort 0xc
#define macr_NtSetInformationThread 0xd
#define macr_NtSetEvent 0xe
#define macr_NtClose 0xf
#define macr_NtQueryObject 0x10
#define macr_NtQueryInformationFile 0x11
#define macr_NtOpenKey 0x12
#define macr_NtEnumerateValueKey 0x13
#define macr_NtFindAtom 0x14
#define macr_NtQueryDefaultLocale 0x15
#define macr_NtQueryKey 0x16
#define macr_NtQueryValueKey 0x17
#define macr_NtAllocateVirtualMemory 0x18
#define macr_NtQueryInformationProcess 0x19
#define macr_NtWaitForMultipleObjects32 0x1a
#define macr_NtWriteFileGather 0x1b
#define macr_NtSetInformationProcess 0x1c
#define macr_NtCreateKey 0x1d
#define macr_NtFreeVirtualMemory 0x1e
#define macr_NtImpersonateClientOfPort 0x1f
#define macr_NtReleaseMutant 0x20
#define macr_NtQueryInformationToken 0x21
#define macr_NtRequestWaitReplyPort 0x22
#define macr_NtQueryVirtualMemory 0x23
#define macr_NtOpenThreadToken 0x24
#define macr_NtQueryInformationThread 0x25
#define macr_NtOpenProcess 0x26
#define macr_NtSetInformationFile 0x27
#define macr_NtMapViewOfSection 0x28
#define macr_NtAccessCheckAndAuditAlarm 0x29
#define macr_NtUnmapViewOfSection 0x2a
#define macr_NtReplyWaitReceivePortEx 0x2b
#define macr_NtTerminateProcess 0x2c
#define macr_NtSetEventBoostPriority 0x2d
#define macr_NtReadFileScatter 0x2e
#define macr_NtOpenThreadTokenEx 0x2f
#define macr_NtOpenProcessTokenEx 0x30
#define macr_NtQueryPerformanceCounter 0x31
#define macr_NtEnumerateKey 0x32
#define macr_NtOpenFile 0x33
#define macr_NtDelayExecution 0x34
#define macr_NtQueryDirectoryFile 0x35
#define macr_NtQuerySystemInformation 0x36
#define macr_NtOpenSection 0x37
#define macr_NtQueryTimer 0x38
#define macr_NtFsControlFile 0x39
#define macr_NtWriteVirtualMemory 0x3a
#define macr_NtCloseObjectAuditAlarm 0x3b
#define macr_NtDuplicateObject 0x3c
#define macr_NtQueryAttributesFile 0x3d
#define macr_NtClearEvent 0x3e
#define macr_NtReadVirtualMemory 0x3f
#define macr_NtOpenEvent 0x40
#define macr_NtAdjustPrivilegesToken 0x41
#define macr_NtDuplicateToken 0x42
#define macr_NtContinue 0x43
#define macr_NtQueryDefaultUILanguage 0x44
#define macr_NtQueueApcThread 0x45
#define macr_NtYieldExecution 0x46
#define macr_NtAddAtom 0x47
#define macr_NtCreateEvent 0x48
#define macr_NtQueryVolumeInformationFile 0x49
#define macr_NtCreateSection 0x4a
#define macr_NtFlushBuffersFile 0x4b
#define macr_NtApphelpCacheControl 0x4c
#define macr_NtCreateProcessEx 0x4d
#define macr_NtCreateThread 0x4e
#define macr_NtIsProcessInJob 0x4f
#define macr_NtProtectVirtualMemory 0x50
#define macr_NtQuerySection 0x51
#define macr_NtResumeThread 0x52
#define macr_NtTerminateThread 0x53
#define macr_NtReadRequestData 0x54
#define macr_NtCreateFile 0x55
#define macr_NtQueryEvent 0x56
#define macr_NtWriteRequestData 0x57
#define macr_NtOpenDirectoryObject 0x58
#define macr_NtAccessCheckByTypeAndAuditAlarm 0x59
#define macr_NtNullSyscall 0x5a
#define macr_NtWaitForMultipleObjects 0x5b
#define macr_NtSetInformationObject 0x5c
#define macr_NtCancelIoFile 0x5d
#define macr_NtTraceEvent 0x5e
#define macr_NtPowerInformation 0x5f
#define macr_NtSetValueKey 0x60
#define macr_NtCancelTimer 0x61
#define macr_NtSetTimer 0x62
#define macr_NtAccessCheckByType 0x63
#define macr_NtAccessCheckByTypeResultList 0x64
#define macr_NtAccessCheckByTypeResultListAndAuditAlarm 0x65
#define macr_NtAccessCheckByTypeResultListAndAuditAlarmByHandle 0x66
#define macr_NtAcquireCrossVmMutant 0x67
#define macr_NtAcquireProcessActivityReference 0x68
#define macr_NtAddAtomEx 0x69
#define macr_NtAddBootEntry 0x6a
#define macr_NtAddDriverEntry 0x6b
#define macr_NtAdjustGroupsToken 0x6c
#define macr_NtAdjustTokenClaimsAndDeviceGroups 0x6d
#define macr_NtAlertResumeThread 0x6e
#define macr_NtAlertThread 0x6f
#define macr_NtAlertThreadByThreadId 0x70
#define macr_NtAllocateLocallyUniqueId 0x71
#define macr_NtAllocateReserveObject 0x72
#define macr_NtAllocateUserPhysicalPages 0x73
#define macr_NtAllocateUserPhysicalPagesEx 0x74
#define macr_NtAllocateUuids 0x75
#define macr_NtAllocateVirtualMemoryEx 0x76
#define macr_NtAlpcAcceptConnectPort 0x77
#define macr_NtAlpcCancelMessage 0x78
#define macr_NtAlpcConnectPort 0x79
#define macr_NtAlpcConnectPortEx 0x7a
#define macr_NtAlpcCreatePort 0x7b
#define macr_NtAlpcCreatePortSection 0x7c
#define macr_NtAlpcCreateResourceReserve 0x7d
#define macr_NtAlpcCreateSectionView 0x7e
#define macr_NtAlpcCreateSecurityContext 0x7f
#define macr_NtAlpcDeletePortSection 0x80
#define macr_NtAlpcDeleteResourceReserve 0x81
#define macr_NtAlpcDeleteSectionView 0x82
#define macr_NtAlpcDeleteSecurityContext 0x83
#define macr_NtAlpcDisconnectPort 0x84
#define macr_NtAlpcImpersonateClientContainerOfPort 0x85
#define macr_NtAlpcImpersonateClientOfPort 0x86
#define macr_NtAlpcOpenSenderProcess 0x87
#define macr_NtAlpcOpenSenderThread 0x88
#define macr_NtAlpcQueryInformation 0x89
#define macr_NtAlpcQueryInformationMessage 0x8a
#define macr_NtAlpcRevokeSecurityContext 0x8b
#define macr_NtAlpcSendWaitReceivePort 0x8c
#define macr_NtAlpcSetInformation 0x8d
#define macr_NtAreMappedFilesTheSame 0x8e
#define macr_NtAssignProcessToJobObject 0x8f
#define macr_NtAssociateWaitCompletionPacket 0x90
#define macr_NtCallEnclave 0x91
#define macr_NtCancelIoFileEx 0x92
#define macr_NtCancelSynchronousIoFile 0x93
#define macr_NtCancelTimer2 0x94
#define macr_NtCancelWaitCompletionPacket 0x95
#define macr_NtCommitComplete 0x96
#define macr_NtCommitEnlistment 0x97
#define macr_NtCommitRegistryTransaction 0x98
#define macr_NtCommitTransaction 0x99
#define macr_NtCompactKeys 0x9a
#define macr_NtCompareObjects 0x9b
#define macr_NtCompareSigningLevels 0x9c
#define macr_NtCompareTokens 0x9d
#define macr_NtCompleteConnectPort 0x9e
#define macr_NtCompressKey 0x9f
#define macr_NtConnectPort 0xa0
#define macr_NtContinueEx 0xa1
#define macr_NtConvertBetweenAuxiliaryCounterAndPerformanceCounter 0xa2
#define macr_NtCreateCrossVmEvent 0xa3
#define macr_NtCreateCrossVmMutant 0xa4
#define macr_NtCreateDebugObject 0xa5
#define macr_NtCreateDirectoryObject 0xa6
#define macr_NtCreateDirectoryObjectEx 0xa7
#define macr_NtCreateEnclave 0xa8
#define macr_NtCreateEnlistment 0xa9
#define macr_NtCreateEventPair 0xaa
#define macr_NtCreateIRTimer 0xab
#define macr_NtCreateIoCompletion 0xac
#define macr_NtCreateJobObject 0xad
#define macr_NtCreateJobSet 0xae
#define macr_NtCreateKeyTransacted 0xaf
#define macr_NtCreateKeyedEvent 0xb0
#define macr_NtCreateLowBoxToken 0xb1
#define macr_NtCreateMailslotFile 0xb2
#define macr_NtCreateMutant 0xb3
#define macr_NtCreateNamedPipeFile 0xb4
#define macr_NtCreatePagingFile 0xb5
#define macr_NtCreatePartition 0xb6
#define macr_NtCreatePort 0xb7
#define macr_NtCreatePrivateNamespace 0xb8
#define macr_NtCreateProcess 0xb9
#define macr_NtCreateProfile 0xba
#define macr_NtCreateProfileEx 0xbb
#define macr_NtCreateRegistryTransaction 0xbc
#define macr_NtCreateResourceManager 0xbd
#define macr_NtCreateSectionEx 0xbe
#define macr_NtCreateSemaphore 0xbf
#define macr_NtCreateSymbolicLinkObject 0xc0
#define macr_NtCreateThreadEx 0xc1
#define macr_NtCreateTimer 0xc2
#define macr_NtCreateTimer2 0xc3
#define macr_NtCreateToken 0xc4
#define macr_NtCreateTokenEx 0xc5
#define macr_NtCreateTransaction 0xc6
#define macr_NtCreateTransactionManager 0xc7
#define macr_NtCreateUserProcess 0xc8
#define macr_NtCreateWaitCompletionPacket 0xc9
#define macr_NtCreateWaitablePort 0xca
#define macr_NtCreateWnfStateName 0xcb
#define macr_NtCreateWorkerFactory 0xcc
#define macr_NtDebugActiveProcess 0xcd
#define macr_NtDebugContinue 0xce
#define macr_NtDeleteAtom 0xcf
#define macr_NtDeleteBootEntry 0xd0
#define macr_NtDeleteDriverEntry 0xd1
#define macr_NtDeleteFile 0xd2
#define macr_NtDeleteKey 0xd3
#define macr_NtDeleteObjectAuditAlarm 0xd4
#define macr_NtDeletePrivateNamespace 0xd5
#define macr_NtDeleteValueKey 0xd6
#define macr_NtDeleteWnfStateData 0xd7
#define macr_NtDeleteWnfStateName 0xd8
#define macr_NtDirectGraphicsCall 0xd9
#define macr_NtDisableLastKnownGood 0xda
#define macr_NtDisplayString 0xdb
#define macr_NtDrawText 0xdc
#define macr_NtEnableLastKnownGood 0xdd
#define macr_NtEnumerateBootEntries 0xde
#define macr_NtEnumerateDriverEntries 0xdf
#define macr_NtEnumerateSystemEnvironmentValuesEx 0xe0
#define macr_NtEnumerateTransactionObject 0xe1
#define macr_NtExtendSection 0xe2
#define macr_NtFilterBootOption 0xe3
#define macr_NtFilterToken 0xe4
#define macr_NtFilterTokenEx 0xe5
#define macr_NtFlushBuffersFileEx 0xe6
#define macr_NtFlushInstallUILanguage 0xe7
#define macr_NtFlushInstructionCache 0xe8
#define macr_NtFlushKey 0xe9
#define macr_NtFlushProcessWriteBuffers 0xea
#define macr_NtFlushVirtualMemory 0xeb
#define macr_NtFlushWriteBuffer 0xec
#define macr_NtFreeUserPhysicalPages 0xed
#define macr_NtFreezeRegistry 0xee
#define macr_NtFreezeTransactions 0xef
#define macr_NtGetCachedSigningLevel 0xf0
#define macr_NtGetCompleteWnfStateSubscription 0xf1
#define macr_NtGetContextThread 0xf2
#define macr_NtGetCurrentProcessorNumber 0xf3
#define macr_NtGetCurrentProcessorNumberEx 0xf4
#define macr_NtGetDevicePowerState 0xf5
#define macr_NtGetMUIRegistryInfo 0xf6
#define macr_NtGetNextProcess 0xf7
#define macr_NtGetNextThread 0xf8
#define macr_NtGetNlsSectionPtr 0xf9
#define macr_NtGetNotificationResourceManager 0xfa
#define macr_NtGetWriteWatch 0xfb
#define macr_NtImpersonateAnonymousToken 0xfc
#define macr_NtImpersonateThread 0xfd
#define macr_NtInitializeEnclave 0xfe
#define macr_NtInitializeNlsFiles 0xff
#define macr_NtInitializeRegistry 0x100
#define macr_NtInitiatePowerAction 0x101
#define macr_NtIsSystemResumeAutomatic 0x102
#define macr_NtIsUILanguageComitted 0x103
#define macr_NtListenPort 0x104
#define macr_NtLoadDriver 0x105
#define macr_NtLoadEnclaveData 0x106
#define macr_NtLoadKey 0x107
#define macr_NtLoadKey2 0x108
#define macr_NtLoadKeyEx 0x109
#define macr_NtLockFile 0x10a
#define macr_NtLockProductActivationKeys 0x10b
#define macr_NtLockRegistryKey 0x10c
#define macr_NtLockVirtualMemory 0x10d
#define macr_NtMakePermanentObject 0x10e
#define macr_NtMakeTemporaryObject 0x10f
#define macr_NtManageHotPatch 0x110
#define macr_NtManagePartition 0x111
#define macr_NtMapCMFModule 0x112
#define macr_NtMapUserPhysicalPages 0x113
#define macr_NtMapViewOfSectionEx 0x114
#define macr_NtModifyBootEntry 0x115
#define macr_NtModifyDriverEntry 0x116
#define macr_NtNotifyChangeDirectoryFile 0x117
#define macr_NtNotifyChangeDirectoryFileEx 0x118
#define macr_NtNotifyChangeKey 0x119
#define macr_NtNotifyChangeMultipleKeys 0x11a
#define macr_NtNotifyChangeSession 0x11b
#define macr_NtOpenEnlistment 0x11c
#define macr_NtOpenEventPair 0x11d
#define macr_NtOpenIoCompletion 0x11e
#define macr_NtOpenJobObject 0x11f
#define macr_NtOpenKeyEx 0x120
#define macr_NtOpenKeyTransacted 0x121
#define macr_NtOpenKeyTransactedEx 0x122
#define macr_NtOpenKeyedEvent 0x123
#define macr_NtOpenMutant 0x124
#define macr_NtOpenObjectAuditAlarm 0x125
#define macr_NtOpenPartition 0x126
#define macr_NtOpenPrivateNamespace 0x127
#define macr_NtOpenProcessToken 0x128
#define macr_NtOpenRegistryTransaction 0x129
#define macr_NtOpenResourceManager 0x12a
#define macr_NtOpenSemaphore 0x12b
#define macr_NtOpenSession 0x12c
#define macr_NtOpenSymbolicLinkObject 0x12d
#define macr_NtOpenThread 0x12e
#define macr_NtOpenTimer 0x12f
#define macr_NtOpenTransaction 0x130
#define macr_NtOpenTransactionManager 0x131
#define macr_NtPlugPlayControl 0x132
#define macr_NtPrePrepareComplete 0x133
#define macr_NtPrePrepareEnlistment 0x134
#define macr_NtPrepareComplete 0x135
#define macr_NtPrepareEnlistment 0x136
#define macr_NtPrivilegeCheck 0x137
#define macr_NtPrivilegeObjectAuditAlarm 0x138
#define macr_NtPrivilegedServiceAuditAlarm 0x139
#define macr_NtPropagationComplete 0x13a
#define macr_NtPropagationFailed 0x13b
#define macr_NtPssCaptureVaSpaceBulk 0x13c
#define macr_NtPulseEvent 0x13d
#define macr_NtQueryAuxiliaryCounterFrequency 0x13e
#define macr_NtQueryBootEntryOrder 0x13f
#define macr_NtQueryBootOptions 0x140
#define macr_NtQueryDebugFilterState 0x141
#define macr_NtQueryDirectoryFileEx 0x142
#define macr_NtQueryDirectoryObject 0x143
#define macr_NtQueryDriverEntryOrder 0x144
#define macr_NtQueryEaFile 0x145
#define macr_NtQueryFullAttributesFile 0x146
#define macr_NtQueryInformationAtom 0x147
#define macr_NtQueryInformationByName 0x148
#define macr_NtQueryInformationEnlistment 0x149
#define macr_NtQueryInformationJobObject 0x14a
#define macr_NtQueryInformationPort 0x14b
#define macr_NtQueryInformationResourceManager 0x14c
#define macr_NtQueryInformationTransaction 0x14d
#define macr_NtQueryInformationTransactionManager 0x14e
#define macr_NtQueryInformationWorkerFactory 0x14f
#define macr_NtQueryInstallUILanguage 0x150
#define macr_NtQueryIntervalProfile 0x151
#define macr_NtQueryIoCompletion 0x152
#define macr_NtQueryLicenseValue 0x153
#define macr_NtQueryMultipleValueKey 0x154
#define macr_NtQueryMutant 0x155
#define macr_NtQueryOpenSubKeys 0x156
#define macr_NtQueryOpenSubKeysEx 0x157
#define macr_NtQueryPortInformationProcess 0x158
#define macr_NtQueryQuotaInformationFile 0x159
#define macr_NtQuerySecurityAttributesToken 0x15a
#define macr_NtQuerySecurityObject 0x15b
#define macr_NtQuerySecurityPolicy 0x15c
#define macr_NtQuerySemaphore 0x15d
#define macr_NtQuerySymbolicLinkObject 0x15e
#define macr_NtQuerySystemEnvironmentValue 0x15f
#define macr_NtQuerySystemEnvironmentValueEx 0x160
#define macr_NtQuerySystemInformationEx 0x161
#define macr_NtQueryTimerResolution 0x162
#define macr_NtQueryWnfStateData 0x163
#define macr_NtQueryWnfStateNameInformation 0x164
#define macr_NtQueueApcThreadEx 0x165
#define macr_NtRaiseException 0x166
#define macr_NtRaiseHardError 0x167
#define macr_NtReadOnlyEnlistment 0x168
#define macr_NtRecoverEnlistment 0x169
#define macr_NtRecoverResourceManager 0x16a
#define macr_NtRecoverTransactionManager 0x16b
#define macr_NtRegisterProtocolAddressInformation 0x16c
#define macr_NtRegisterThreadTerminatePort 0x16d
#define macr_NtReleaseKeyedEvent 0x16e
#define macr_NtReleaseWorkerFactoryWorker 0x16f
#define macr_NtRemoveIoCompletionEx 0x170
#define macr_NtRemoveProcessDebug 0x171
#define macr_NtRenameKey 0x172
#define macr_NtRenameTransactionManager 0x173
#define macr_NtReplaceKey 0x174
#define macr_NtReplacePartitionUnit 0x175
#define macr_NtReplyWaitReplyPort 0x176
#define macr_NtRequestPort 0x177
#define macr_NtResetEvent 0x178
#define macr_NtResetWriteWatch 0x179
#define macr_NtRestoreKey 0x17a
#define macr_NtResumeProcess 0x17b
#define macr_NtRevertContainerImpersonation 0x17c
#define macr_NtRollbackComplete 0x17d
#define macr_NtRollbackEnlistment 0x17e
#define macr_NtRollbackRegistryTransaction 0x17f
#define macr_NtRollbackTransaction 0x180
#define macr_NtRollforwardTransactionManager 0x181
#define macr_NtSaveKey 0x182
#define macr_NtSaveKeyEx 0x183
#define macr_NtSaveMergedKeys 0x184
#define macr_NtSecureConnectPort 0x185
#define macr_NtSerializeBoot 0x186
#define macr_NtSetBootEntryOrder 0x187
#define macr_NtSetBootOptions 0x188
#define macr_NtSetCachedSigningLevel 0x189
#define macr_NtSetCachedSigningLevel2 0x18a
#define macr_NtSetContextThread 0x18b
#define macr_NtSetDebugFilterState 0x18c
#define macr_NtSetDefaultHardErrorPort 0x18d
#define macr_NtSetDefaultLocale 0x18e
#define macr_NtSetDefaultUILanguage 0x18f
#define macr_NtSetDriverEntryOrder 0x190
#define macr_NtSetEaFile 0x191
#define macr_NtSetHighEventPair 0x192
#define macr_NtSetHighWaitLowEventPair 0x193
#define macr_NtSetIRTimer 0x194
#define macr_NtSetInformationDebugObject 0x195
#define macr_NtSetInformationEnlistment 0x196
#define macr_NtSetInformationJobObject 0x197
#define macr_NtSetInformationKey 0x198
#define macr_NtSetInformationResourceManager 0x199
#define macr_NtSetInformationSymbolicLink 0x19a
#define macr_NtSetInformationToken 0x19b
#define macr_NtSetInformationTransaction 0x19c
#define macr_NtSetInformationTransactionManager 0x19d
#define macr_NtSetInformationVirtualMemory 0x19e
#define macr_NtSetInformationWorkerFactory 0x19f
#define macr_NtSetIntervalProfile 0x1a0
#define macr_NtSetIoCompletion 0x1a1
#define macr_NtSetIoCompletionEx 0x1a2
#define macr_NtSetLdtEntries 0x1a3
#define macr_NtSetLowEventPair 0x1a4
#define macr_NtSetLowWaitHighEventPair 0x1a5
#define macr_NtSetQuotaInformationFile 0x1a6
#define macr_NtSetSecurityObject 0x1a7
#define macr_NtSetSystemEnvironmentValue 0x1a8
#define macr_NtSetSystemEnvironmentValueEx 0x1a9
#define macr_NtSetSystemInformation 0x1aa
#define macr_NtSetSystemPowerState 0x1ab
#define macr_NtSetSystemTime 0x1ac
#define macr_NtSetThreadExecutionState 0x1ad
#define macr_NtSetTimer2 0x1ae
#define macr_NtSetTimerEx 0x1af
#define macr_NtSetTimerResolution 0x1b0
#define macr_NtSetUuidSeed 0x1b1
#define macr_NtSetVolumeInformationFile 0x1b2
#define macr_NtSetWnfProcessNotificationEvent 0x1b3
#define macr_NtShutdownSystem 0x1b4
#define macr_NtShutdownWorkerFactory 0x1b5
#define macr_NtSignalAndWaitForSingleObject 0x1b6
#define macr_NtSinglePhaseReject 0x1b7
#define macr_NtStartProfile 0x1b8
#define macr_NtStopProfile 0x1b9
#define macr_NtSubscribeWnfStateChange 0x1ba
#define macr_NtSuspendProcess 0x1bb
#define macr_NtSuspendThread 0x1bc
#define macr_NtSystemDebugControl 0x1bd
#define macr_NtTerminateEnclave 0x1be
#define macr_NtTerminateJobObject 0x1bf
#define macr_NtTestAlert 0x1c0
#define macr_NtThawRegistry 0x1c1
#define macr_NtThawTransactions 0x1c2
#define macr_NtTraceControl 0x1c3
#define macr_NtTranslateFilePath 0x1c4
#define macr_NtUmsThreadYield 0x1c5
#define macr_NtUnloadDriver 0x1c6
#define macr_NtUnloadKey 0x1c7
#define macr_NtUnloadKey2 0x1c8
#define macr_NtUnloadKeyEx 0x1c9
#define macr_NtUnlockFile 0x1ca
#define macr_NtUnlockVirtualMemory 0x1cb
#define macr_NtUnmapViewOfSectionEx 0x1cc
#define macr_NtUnsubscribeWnfStateChange 0x1cd
#define macr_NtUpdateWnfStateData 0x1ce
#define macr_NtVdmControl 0x1cf
#define macr_NtWaitForAlertByThreadId 0x1d0
#define macr_NtWaitForDebugEvent 0x1d1
#define macr_NtWaitForKeyedEvent 0x1d2
#define macr_NtWaitForWorkViaWorkerFactory 0x1d3
#define macr_NtWaitHighEventPair 0x1d4
#define macr_NtWaitLowEventPair 0x1d5
#define macr_NtLoadKey3 0x1d6
