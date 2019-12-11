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

struct _CLIENT_ID
{
	ulonglong UniqueProcess;
	ulonglong UniqueThread;
};


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

	int RtlGetVersion(PRTL_OSVERSIONINFOW lpVersionInformation);


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


	int ZwCreateTimer( HANDLE* TimerHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,ulonglong TimerType );

	int ZwCreateTransactionManager(HANDLE* TmHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,_UNICODE_STRING* LogFileName,ulonglong CreateOptions,ulonglong CommitStrength);

	int ZwCreateResourceManager(HANDLE* ResourceManagerHandle, ACCESS_MASK DesiredAccess, HANDLE TmHandle, void* RmGuid, _OBJECT_ATTRIBUTES* ObjectAttributes, ulonglong CreateOptions, _UNICODE_STRING* Description);

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

	//HANDLE GetCurrentThreadToken();
	//HANDLE GetCurrentProcessToken();
	//HANDLE GetCurrentThreadEffectiveToken();

	//BOOL OpenProcessToken(HANDLE  ProcessHandle,ulonglong   DesiredAccess,HANDLE* TokenHandle);
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


void InitOsInfo();

bool ShouldSkipLeakChecking(unsigned long SysCall);

void RandomizeOrderAndCounts(ulong* pOrder,ulong* pCounts,ulong Count);

//void* InitFunctionTable(unsigned long SyscallCount, unsigned long MinSyscallOrdinal,MYFUNC* pFuncTable,PREFUNC* pPreTable,POSTFUNC* pPostTable);
void* InitFunctionTable(unsigned long SyscallCount, unsigned long MinSyscallOrdinal,MYFUNC* pFuncTable,PREFUNC* pPreTable,POSTFUNC* pPostTable,ulong OSVer);
void  DestroyFunctionTable(void* pExecPages,unsigned long SyscallCount,MYFUNC* pFuncTable,PREFUNC* pPreTable,POSTFUNC* pPostTable);





//bool Scan(void* pMem,ulong Size);
bool Scan(void* pMem,ulong Size,ulong OSVer);

int InitKernelObjects();

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