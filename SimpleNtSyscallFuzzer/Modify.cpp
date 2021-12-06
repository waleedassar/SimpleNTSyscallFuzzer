#include "stdafx.h"
#include "Header.h"


extern ulonglong AllKernelObject[0x5000];
extern ulong AllKernelObjectsUsed;
extern HANDLE AllFiles[0x1000];
extern HANDLE AllProcesses[0x1000];




extern ulong AllFilesUsed;
extern ulong AllProcessesUsed;



//hAdapter
extern HANDLE hLpc;
//hCallback
//Controller
extern HANDLE hDebugObject;
//hDevice
extern HANDLE hDirectory;
extern HANDLE hNameSpaceDirectory;
//hDriver
//hEtwConsumer
extern HANDLE hEtwRegistration;
extern HANDLE hEvent;
extern HANDLE hEventPair;
extern HANDLE hFile;
//hFilterCommunicationPort
//hFilterConnectionPort
extern HANDLE hIoCompletion;
extern HANDLE hIoCompletionReserve;
extern HANDLE hJob;
extern HANDLE hPartitionJob;
extern HKEY hKey;
extern HANDLE hKeyedEvent;
extern HANDLE hMutex;
//hPcwObject ==> IOCTL on pcw.sys
extern HANDLE hPowerRequest;
extern HANDLE hProcess;
extern HANDLE hProfile;
extern HANDLE hSection;
extern HANDLE hSemaphore;
extern HANDLE hSession;
extern HANDLE hSymLink;
extern HANDLE hThread;
extern HANDLE hTimer;
extern HANDLE hTmEn;
extern HANDLE hTmRm;
extern HANDLE hTmTm;
extern HANDLE hTmTx;
extern HANDLE hToken;
extern HANDLE hThreadToken;
extern HANDLE hToken2;
extern HANDLE hTpWorkerFactory;
//Type
extern HANDLE hUserApcReserve;
extern HANDLE hPartition;
extern HANDLE hPartition2;
//hWmiGuid
extern HANDLE hWaitCompletionPacket;
extern HANDLE hRegistryTransaction;




void DummyCallbackRoutine()
{
	return;
}




void w7NtMapUserPhysicalPagesScatter(unsigned long SysCall,
						 unsigned long long* Args,
						 void** pPool,
						 void** pSecondLevelPool)
{
	
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random


	R = Rand()%4;
	if(R==0) Args[1]=0;
	else if(R==1) Args[1] = Rand()%(0x200+0x1);
	else if(R==2) Args[1] = 0xFFFFFFFFFFFFF;
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random
}
	
void w7NtWaitForSingleObject(unsigned long SysCall,
						 unsigned long long* Args,
						 void** pPool,
						 void** pSecondLevelPool)
{
	ulonglong Handles[3]={0,-1,-2};
	if(Rand()%5 == 1) Args[0]=Handles[Rand()%3];
	else Args[0] = (ulonglong)(AllKernelObject[Rand()%AllKernelObjectsUsed]);


	if(Rand()%2 == 1) *(unsigned char*)(&Args[1])=0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random
}

void w7NtCallbackReturn(unsigned long SysCall,
						 unsigned long long* Args,
						 void** pPool,
						 void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[1]) = 0;
	else if(R==1) *(ulong*)(&Args[1]) = 0x1000; //Proper Size
	//else Random
	
	if( Rand()%3 == 1) *(ulong*)(&Args[2]) |= 0xC0000000;

}

void w7NtReadFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hFile;
	//Uncomment
	//if(Rand()%5==1) Args[0] = 0;

	Args[1] = (ulonglong)hEvent;
	if(Rand()%5==1) Args[1] = 0;

	if( Rand()%2 ==1) Args[2] = (ulonglong)(&DummyCallbackRoutine);
	else Args[2] = 0;



	//Partial Leak found in IO Status Block
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[6]) = 0;
	else if(R==1) *(ulong*)(&Args[6]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[7] = (ulonglong)(pPool[7]);
	}
	else if(R==1)
	{
		Args[7] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[8] = (ulonglong)(pPool[8]);
	}
	else if(R==1)
	{
		Args[8] = 0;
	}
	//else Random
}


void w7NtDeviceIoControlFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   	Args[0] = (ulonglong)hFile;
	//Uncomment
	//if(Rand()%15==1) Args[0] = 0;

	Args[1] = (ulonglong)hEvent;
	//Uncomment
	//if(Rand()%5==1) Args[1] = 0;

	if( Rand()%2 ==1) Args[2] = (ulonglong)(&DummyCallbackRoutine);
	else Args[2] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random

	// 5 ==> IOCTL



	R = Rand()%3;
	if(R==0)
	{
		Args[6] = (ulonglong)(pPool[6]);
	}
	else if(R==1)
	{
		Args[6] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[7]) = 0;
	else if(R==1) *(ulong*)(&Args[7]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[8] = (ulonglong)(pPool[8]);
	}
	else if(R==1)
	{
		Args[8] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[9]) = 0;
	else if(R==1) *(ulong*)(&Args[9]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random
}

void w7NtWriteFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hFile;
	//Uncomment
	//if(Rand()%5==1) Args[0] = 0;

	Args[1] = (ulonglong)hEvent;
	if(Rand()%5==1) Args[1] = 0;

	if( Rand()%2 ==1) Args[2] = (ulonglong)(&DummyCallbackRoutine);
	else Args[2] = 0;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[6]) = 0;
	else if(R==1) *(ulong*)(&Args[6]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[7] = (ulonglong)(pPool[7]);
	}
	else if(R==1)
	{
		Args[7] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[8] = (ulonglong)(pPool[8]);
	}
	else if(R==1)
	{
		Args[8] = 0;
	}
	//else Random
}

void w7NtRemoveIoCompletion
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hIoCompletion;
	if(Rand()%5==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random
}

void w7NtReleaseSemaphore
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hSemaphore;
	//Uncomment
	//if(Rand()%15==1) Args[0] = 0;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random
}

void w7NtReplyWaitReceivePort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hLpc;
	if(Rand()%5==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random
}

void w7NtReplyPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hLpc;
	if(Rand()%5==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtSetInformationThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hThread;
	//Uncomment
	//if(Rand()%5==1) Args[0] = 0;


	unsigned long long Classes[0x21]={0x2,0x3,0x4,0x5,0x7,0x9,0xa,0xd,0xe,0xf,0x11,0x12,0x13,0x16,0x18,0x19,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x24,0x26,0x27,0x2a,0x2b,0x2c,0x2e,0x2f,0x30,0x31,0x32};
	*(ulong*)(&Args[1]) = Classes[Rand()%0x21]; //classes

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random


	//printf("Class: %X\r\n",*(ulong*)(&Args[1]));
}

void w7NtSetEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hEvent;
	//Uncomment
	//if(Rand()%5==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtClose
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulonglong Handles[3]={0,-1,-2};
	if(Rand()%5 == 1) Args[0]=Handles[Rand()%3];
	else Args[0] = (ulonglong)(AllKernelObject[Rand()%AllKernelObjectsUsed]);
}

void w7NtQueryObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	//Uncomment
	/*
	ulonglong Handles[3]={0,-1,-2};
	if(Rand()%5 == 1) Args[0]=Handles[Rand()%3];
	else */Args[0] = (ulonglong)(AllKernelObject[Rand()%AllKernelObjectsUsed]);

	unsigned long long Classes[0x5]={0x0,0x1,0x2,0x3,0x4};
	if(Rand()%2 == 1) *(ulong*)(&Args[1]) = Classes[Rand()%0x5];

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random
}

void w7NtQueryInformationFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hFile;
	//Uncomment
	//if(Rand()%5==1) Args[0] = 0;



	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	unsigned long long Classes[0x2b]={0x4,0x5,0x6,0x7,0x8,0x9,0xe,0x10,0x11,0x12,0x15,0x16,0x17,0x18,0x19,0x1a,0x1c,0x1d,0x20,0x21,0x22,0x23,0x29,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x33,0x35,0x36,0x37,0x3a,0x3b,0x3e,0x43,0x44,0x46,0x47,0x4a,0x4b};
	*(ulong*)(&Args[4]) = Classes[Rand()%0x2b];
}

void w7NtOpenKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}

	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtEnumerateValueKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   	Args[0] = (ulonglong)hKey;
	if(Rand()%5==1) Args[0] = 0;

	if(Rand()%2 == 1) *(ulong*)(&Args[1]) = 0;

	if(Rand()%2 == 1) *(ulong*)(&Args[2]) = Rand()%3;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[4]) = 0;
	else if(R==1) *(ulong*)(&Args[4]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
}

void w7NtFindAtom
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
		GetRandomWideString((wchar_t*)(pPool[0]),RANDOM_PAGE_SIZE/2);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[1]) = 0;
	else if(R==1) *(ulong*)(&Args[1]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtQueryDefaultLocale
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   if(Rand()%2==1) *(unsigned char*)(&Args[0]) = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
}

void w7NtQueryKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   	Args[0] = (ulonglong)hKey;
	//Uncomment
	//if(Rand()%5==1) Args[0] = 0;


	if(Rand()%2 == 1) *(ulong*)(&Args[1]) = Rand()%0x10;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
}

void w7NtQueryValueKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   	Args[0] = (ulonglong)hKey;
	//Uncomment
	//if(Rand()%15==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
		FillRandomUnicodeString(pPool[1],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}

	if(Rand()%2 == 1) *(ulong*)(&Args[2]) = Rand()%0x10;

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[4]) = 0;
	else if(R==1) *(ulong*)(&Args[4]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
}

void w7NtAllocateVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   	Args[0] = (ulonglong)hProcess;
	//Uncomment
	//if(Rand()%5==1) Args[0] = 0;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}

}

void w7NtQueryInformationProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   	Args[0] = (ulonglong)hProcess;
	//UnComment
	//if(Rand()%15==1) Args[0] = 0;


	unsigned long long Classes[0x47]={0x0,0x1,0x2,0x3,0x4,0x7,0xa,0xc,0xe,0xf,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x24,0x25,0x26,0x27,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x31,0x32,0x33,0x34,0x36,0x37,0x3a,0x3b,0x3c,0x3d,0x40,0x41,0x42,0x43,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4f,0x51,0x52,0x54,0x55,0x57,0x58,0x59,0x5c,0x5e,0x60,0x61};
	if(Rand()%2 == 1) *(ulong*)(&Args[1]) = Classes[Rand()%0x47];

	if( (*(ulong*)(&Args[1])==0xF) ||
		(*(ulong*)(&Args[1])==0x2A) ) CancelSyscall(Args);

	//Leak found
	if( *(ulong*)(&Args[1]) == 0xF)	CancelSyscall(Args);


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
}


void w7NtWaitForMultipleObjects32
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   *(ulong*)(&Args[0]) = 1 + Rand()%0x40;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}

	*(ulong*)(&Args[2]) = Rand()%2;

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
}

void w7NtWriteFileGather
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hFile;
	//Uncomment
	//if(Rand()%15==1) Args[0] = 0;

	Args[1] = (ulonglong)hEvent;
	//Uncomment
	//if(Rand()%5==1) Args[1] = 0;

	if( Rand()%2 ==1) Args[2] = (ulonglong)(&DummyCallbackRoutine);
	else Args[2] = 0;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		//IOB
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[6]) = 0;
	else if(R==1) *(ulong*)(&Args[6]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[7] = (ulonglong)(pPool[7]);
	}
	else if(R==1)
	{
		Args[7] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[8] = (ulonglong)(pPool[8]);
	}
	else if(R==1)
	{
		Args[8] = 0;
	}
	//else Random
}

void w7NtSetInformationProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   	Args[0] = (ulonglong)hProcess;
	//Uncomment
	//if(Rand()%15==1) Args[0] = 0;


	unsigned long long Classes[0x42]={0x1,0x5,0x6,0x8,0x9,0xa,0xb,0xc,0xd,0xf,0x10,0x11,0x12,0x13,0x15,0x16,0x17,0x18,0x19,0x1d,0x1f,0x20,0x21,0x22,0x23,0x27,0x28,0x29,0x2a,0x2d,0x2e,0x30,0x31,0x34,0x35,0x36,0x38,0x39,0x3b,0x3e,0x3f,0x41,0x42,0x43,0x44,0x46,0x47,0x48,0x4a,0x4d,0x50,0x52,0x53,0x55,0x57,0x5a,0x5b,0x5d,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66};
	*(ulong*)(&Args[1]) = Classes[Rand()%0x42];


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random
}

void w7NtCreateKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}

	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		FillRandomUnicodeString(pPool[4],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}


	if( Rand()%2 == 1) *(ulong*)(&Args[5]) |= 0x100000F;


	R = Rand()%3;
	if(R==0)
	{
		Args[6] = (ulonglong)(pPool[6]);
	}
	else if(R==1)
	{
		Args[6] = 0;
	}
}

void w7NtFreeVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong) hProcess;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);

		if( Rand()%3 == 1) *(ulonglong*)(pPool[2]) = 0x1000;
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	if( Rand()%3 == 1) *(ulong*)(&Args[3]) ^= 0xFFFF3FFF;
}

void w7NtImpersonateClientOfPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong) hLpc;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
}

void w7NtReleaseMutant
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong) hMutex;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
}

void w7NtQueryInformationToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);

	unsigned long long Classes[0x29]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x25,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2f};
	*(ulong*)(&Args[1]) = Classes[Rand()%0x29];


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
}

void w7NtRequestWaitReplyPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hLpc;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtQueryVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	if(Rand()%3 == 1) Args[0] = -1;
	else Args[0] = (ulonglong)hProcess;

	ulong R = Rand()%3;
	if( R == 0)
	{
		Args[1] &= 0x7ffffffffff;
	}
	else if(R == 1)
	{
		Args[1] &= 0xFFFFFFFF;
	}
	//else Random

	unsigned long long Classes[0xa]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0xb};
	*(ulong*)(&Args[2]) = Classes[Rand()%0xA];


	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[4]) = 0;
	else if(R==1) *(ulong*)(&Args[4]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
}

void w7NtOpenThreadToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hThread;

	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	ulong R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
}

void w7NtQueryInformationThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	if( Rand()%3 == 1) Args[0] = -2;
	Args[0] = (ulonglong)hThread;

	unsigned long long Classes[0x22]={0x0,0x1,0x6,0x9,0xb,0xc,0xe,0x10,0x11,0x12,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d};
	*(ulong*)(&Args[1]) = Classes[Rand()%0x22];

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
}

void w7NtOpenProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}

	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
}

void w7NtSetInformationFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)hFile;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}


	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	unsigned long long Classes[0x1f]={0x4,0xa,0xb,0xd,0xe,0x10,0x13,0x14,0x17,0x19,0x1b,0x1d,0x1e,0x1f,0x20,0x24,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x3d,0x40,0x41,0x43,0x45,0x47,0x48,0x4a,0x4b};
	*(ulong*)(&Args[4]) = Classes[Rand()%0x1F];
}

void w7NtMapViewOfSection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hSection;
	
	Args[1] = (ulonglong)hProcess;
	//Uncomment
	//if(Rand()%0x10==1) Args[1]=0;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);

		ulong RR = Rand()%3;
		if(RR==0) *(ulonglong*)(pPool[2]) &= 0x7FFFFFFFFFF;
		else if(RR==1) *(ulonglong*)(pPool[2]) &= 0xFFFFFFFF;
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	if( Rand()%2 == 1) *(ulonglong*)(&Args[3]) = Rand()%0x36;

	//CommitSize

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[6] = (ulonglong)(pPool[6]);
	}
	else if(R==1)
	{
		Args[6] = 0;
	}

	if( Rand()%2 == 1) *(ulong*)(&Args[7]) = Rand()%2;

	if( Rand()%3==1) *(ulong*)(&Args[8]) ^= 0xDFAFDFFF;

	//Protect

}

void w7NtAccessCheckAndAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
		FillRandomUnicodeString( (void*)(pPool[0]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else random


	R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomUnicodeString( (void*)(pPool[2]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else random


	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
		FillRandomUnicodeString( (void*)(pPool[3]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else random


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else random


	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[5])=0;
	else if(R==1) *(ulong*)(&Args[5])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[5])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[6] = (ulonglong)(pPool[6]);
		//GENERIC_MAPPING
	}
	else if(R==1)
	{
		Args[6] = 0;
	}
	//else random

	R = Rand()%3;
	if(R==0)
	{
		Args[9] = (ulonglong)(pPool[9]);
	}
	else if(R==1)
	{
		Args[9] = 0;
	}
	//else random
}

void w7NtUnmapViewOfSection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hProcess;

	ulong R = Rand()%3;
	if(R==0) Args[1] &= 0x7FFFFFFFFFF;
	else if(R==1) Args[1] &= 0xFFFFFFFF;
	//else Random
}

void w7NtReplyWaitReceivePortEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hLpc;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else random



	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else random


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else random
}

void w7NtTerminateProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)hProcess;

   if(Rand()%0x10 == 1) *(ulong*)(&Args[1]) = 0;

}

void w7NtSetEventBoostPriority
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hEvent;
	
	//Uncomment
	//if(Rand()%0x10==1) Args[0]=0;
}

//Phase
void w7NtReadFileScatter
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
   Args[1] = (ulonglong)hEvent;
}

void w7NtOpenThreadTokenEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtOpenProcessTokenEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtQueryPerformanceCounter
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtEnumerateKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
}

void w7NtOpenFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDelayExecution
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	//Args[0] = GetClassRandomValue();

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else random
}

void w7NtQueryDirectoryFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;

   unsigned long long Classes[0xc]={0x1,0x2,0x3,0xc,0x1d,0x20,0x21,0x25,0x26,0x32,0x3c,0x3f};
   *(ulong*)(&Args[7]) = Classes[Rand()%0xc];
}

void w7NtQuerySystemInformation
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	unsigned long long Classes[0x9f]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x15,0x16,0x17,0x18,0x19,0x1c,0x1d,0x1f,0x21,0x23,0x24,0x25,0x2a,0x2b,0x2c,0x2d,0x32,0x33,0x35,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x48,0x49,0x4c,0x4d,0x4f,0x50,0x51,0x53,0x56,0x57,0x58,0x5a,0x5b,0x5c,0x5f,0x60,0x62,0x63,0x64,0x65,0x66,0x67,0x69,0x6c,0x6d,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x7a,0x7b,0x7c,0x7d,0x7e,0x80,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8f,0x90,0x91,0x93,0x94,0x95,0x96,0x97,0x99,0x9a,0x9c,0x9d,0x9e,0x9f,0xa0,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa9,0xab,0xac,0xad,0xae,0xaf,0xb2,0xb3,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbc,0xbd,0xbe,0xc0,0xc1,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xce,0xcf,0xd0,0xd1,0xd5,0xdd,0xe3};


	if(Rand()%5==1) *(ulong*)(&Args[0]) = GetClassicRandomValue();
	else *(ulong*)(&Args[0]) = Classes[Rand()%0x9F];

	/*
	if( (*(ulong*)(&Args[0])==0x0B) ||
		(*(ulong*)(&Args[0])==0x0C) ||
		(*(ulong*)(&Args[0])==0x10) ||
		(*(ulong*)(&Args[0])==0x12) ||
		(*(ulong*)(&Args[0])==0x39) ||
		(*(ulong*)(&Args[0])==0x40) ||
		(*(ulong*)(&Args[0])==0x42) ||
		(*(ulong*)(&Args[0])==0x4D) ) CancelSyscall(Args);
	 */

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[2]) = 0;
	else if(R==1) *(ulong*)(&Args[2]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random
}

void w7NtOpenSection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryTimer
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTimer;

   unsigned long long Classes[0x1]={0x0};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];

}

void w7NtFsControlFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[0] = (ulonglong)hFile;
   Args[1] = (ulonglong)hEvent;
}

void w7NtWriteVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtCloseObjectAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDuplicateObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = -1;
   if( Rand()%10==1) Args[0] = (ulonglong)hProcess;

   Args[1] = (ulonglong)(AllKernelObject[Rand()%AllKernelObjectsUsed]);

   Args[2] = (ulonglong)hProcess;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random

	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[4])=0;
	else if(R==1) *(ulong*)(&Args[4])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[4])=CalcX;
	}
	//else Random

	//Args[5] Random Handle Attributes
	ulong HandleAtt[11]={OBJ_PROTECT_CLOSE,OBJ_INHERIT,OBJ_AUDIT_OBJECT_CLOSE,OBJ_PERMANENT,
						OBJ_EXCLUSIVE,OBJ_CASE_INSENSITIVE,OBJ_OPENIF,OBJ_OPENLINK,
						OBJ_KERNEL_HANDLE,OBJ_FORCE_ACCESS_CHECK,OBJ_CREATOR_INFO_MINE};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[5])=0;
	else if(R==1) *(ulong*)(&Args[5])=HandleAtt[Rand()%11];
	else if(R==2)
	{
		ulong RR = Rand()%(11+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= HandleAtt[Rand()%11];
			RR--;
		}
		*(ulong*)(&Args[5])=CalcX;
	}
	//else Random

	//6 => Options
}

void w7NtQueryAttributesFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtClearEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEvent;
}

void w7NtReadVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtOpenEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAdjustPrivilegesToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);
}

void w7NtDuplicateToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);
}

void w7NtContinue
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryDefaultUILanguage
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueueApcThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtYieldExecution
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAddAtom
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryVolumeInformationFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;

   unsigned long long Classes[0xd]={0x1,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe};
   *(ulong*)(&Args[4]) = Classes[Rand()%0xd];
}

void w7NtCreateSection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtFlushBuffersFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtApphelpCacheControl
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateProcessEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{

   Args[0] = (ulonglong)(pPool[0]);

   if( Rand()%2 == 1) *(ulong*)(&Args[1]) = GetRandomDesiredAccess();

   Args[2] = (ulonglong)(pPool[2]);
   FillRandomObjectAttributes((void*)(pPool[2]),RANDOM_PAGE_SIZE);

   Args[3] = (ulonglong)hProcess;
   if( Rand()%3 == 1) Args[3] = 0;

   *(ulong*)(&Args[4]) &= (~0xFFF94040);

   Args[5] = (ulonglong)hSection;
   if( Rand()%3 == 1) Args[5] = 0;

   Args[6] = (ulonglong)hDebugObject;
   if( Rand()%3 == 1) Args[6] = 0;

   Args[7] = (ulonglong)hToken;
   if( Rand()%3 == 1) Args[7] = 0;
}

void w7NtCreateThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtIsProcessInJob
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;

   HANDLE hJobs[2] = {hJob,hPartitionJob};
   Args[1] = (ulonglong)(hJobs[Rand()%2]);
}

void w7NtProtectVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQuerySection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hSection;

   unsigned long long Classes[0x4]={0x0,0x1,0x2,0x3};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x4];
}

void w7NtResumeThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hThread;
}

void w7NtTerminateThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hThread;
	Args[1] = GetRandomNTStatusCode();
}

void w7NtReadRequestData
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[0] = (ulonglong)hLpc;
}

void w7NtCreateFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEvent;

   unsigned long long Classes[0x1]={0x0};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];

}

void w7NtWriteRequestData
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtOpenDirectoryObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAccessCheckByTypeAndAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQuerySystemTime
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtWaitForMultipleObjects
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetInformationObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = AllKernelObject[Rand()%AllKernelObjectsUsed];

   ulong Classes[3] = {4,5,6};
   if(Rand()%2 == 1) *(ulong*)(&Args[1]) = Classes[Rand()%3]; //classes
}

void w7NtCancelIoFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtTraceEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEtwRegistration;
}

void w7NtPowerInformation
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetValueKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
}

void w7NtCancelTimer
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTimer;
}

void w7NtSetTimer
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTimer;
}

void w7NtAcceptConnectPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAccessCheck
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAccessCheckByType
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAccessCheckByTypeResultList
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAccessCheckByTypeResultListAndAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAccessCheckByTypeResultListAndAuditAlarmByHandle
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAddBootEntry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAddDriverEntry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAdjustGroupsToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);
}

void w7NtAlertResumeThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtAlertThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtAllocateLocallyUniqueId
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAllocateReserveObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAllocateUserPhysicalPages
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtAllocateUuids
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAlpcAcceptConnectPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[1] = (ulonglong)hLpc;
}

void w7NtAlpcCancelMessage
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcConnectPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAlpcCreatePort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAlpcCreatePortSection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;

   Args[2] = (ulonglong)hSection;
   if( Rand()%5 == 1) Args[2] = 0;
}

void w7NtAlpcCreateResourceReserve
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcCreateSectionView
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAlpcCreateSecurityContext
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcDeletePortSection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;

   Args[2] = (ulonglong)hSection;
}

void w7NtAlpcDeleteResourceReserve
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcDeleteSectionView
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcDeleteSecurityContext
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcDisconnectPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcImpersonateClientOfPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcOpenSenderProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[1] = (ulonglong)hLpc;
}

void w7NtAlpcOpenSenderThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[1] = (ulonglong)hLpc;
}

void w7NtAlpcQueryInformation
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcQueryInformationMessage
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcRevokeSecurityContext
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcSendWaitReceivePort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtAlpcSetInformation
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;

   ulong Classes[8]={1,2,5,6,7,8,9,0xA};
   *(ulong*)(&Args[1]) = Classes[Rand()%8];
}

void w7NtAreMappedFilesTheSame
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtAssignProcessToJobObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   HANDLE hJobs[2] = {hJob,hPartitionJob};
   Args[0] = (ulonglong)(hJobs[Rand()%2]);

   Args[1] = (ulonglong)hProcess;
}

void w7NtCancelIoFileEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtCancelSynchronousIoFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtCommitComplete
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;
}

void w7NtCommitEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;
}

void w7NtCommitTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTx;
}

void w7NtCompactKeys
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCompareTokens
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);

	
  	Args[1] = (ulonglong)(Tokens[Rand()%3]);
}

void w7xHalGetInterruptTranslator
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCompressKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
}

void w7NtConnectPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtCreateDebugObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateDirectoryObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[2] = (ulonglong)hTmRm;
   Args[3] = (ulonglong)hTmTx;
}

void w7NtCreateEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateIoCompletion
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateJobObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateJobSet
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateKeyTransacted
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[6] = (ulonglong)hRegistryTransaction;
}

void w7NtCreateKeyedEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateMailslotFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateMutant
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateNamedPipeFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreatePagingFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreatePort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreatePrivateNamespace
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[3] = (ulonglong)hProcess;
   if( Rand()%3 == 1) Args[3] = 0;


   Args[5] = (ulonglong)hSection;
   if( Rand()%3 == 1) Args[5] = 0;

   Args[6] = (ulonglong)hDebugObject;
   if( Rand()%3 == 1) Args[6] = 0;

   Args[6] = (ulonglong)hLpc;
   if( Rand()%3 == 1) Args[6] = 0;
}

void w7NtCreateProfile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateProfileEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateResourceManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[2] = (ulonglong)hTmTm;
}

void w7NtCreateSemaphore
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateSymbolicLinkObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateThreadEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[3] = (ulonglong)hProcess;
}

void w7NtCreateTimer
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateUserProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateWaitablePort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtCreateWorkerFactory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[3] = (ulonglong)hIoCompletion;
   Args[4] = (ulonglong)hProcess;
}

void w7NtDebugActiveProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
   Args[1] = (ulonglong)hDebugObject;
}

void w7NtDebugContinue
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hDebugObject;
}

void w7NtDeleteAtom
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDeleteBootEntry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDeleteDriverEntry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDeleteFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDeleteKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
}

void w7NtDeleteObjectAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDeletePrivateNamespace
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hNameSpaceDirectory;
   //Uncomment
   //if( Rand()%15 == 1) Args[0] = (ulonglong)hDirectory;
}

void w7NtDeleteValueKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
}

void w7NtDisableLastKnownGood
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDisplayString
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtDrawText
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtEnableLastKnownGood
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtEnumerateBootEntries
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtEnumerateDriverEntries
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtEnumerateSystemEnvironmentValuesEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtEnumerateTransactionObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   ulong R = Rand()%3;
   if(R == 0) Args[0] = (ulonglong)hTmTm;
   else if(R == 1) Args[0] = (ulonglong)hTmRm;
   else
   {

   }
}

void w7NtExtendSection
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hSection;
}

void w7NtFilterToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);
}

void w7NtFlushInstallUILanguage
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtFlushInstructionCache
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtFlushKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
}

void w7NtFlushProcessWriteBuffers
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtFlushVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtFlushWriteBuffer
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtFreeUserPhysicalPages
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtFreezeRegistry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtFreezeTransactions
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtGetContextThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtGetCurrentProcessorNumber
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtGetDevicePowerState
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtGetMUIRegistryInfo
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtGetNextProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtGetNextThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
   Args[0] = (ulonglong)hThread;
}

void w7NtGetNlsSectionPtr
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtGetNotificationResourceManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmRm;
}

void w7NtGetPlugPlayEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEvent;
}

void w7NtGetWriteWatch
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtImpersonateAnonymousToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtImpersonateThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
   Args[1] = (ulonglong)hThread;
}

void w7NtInitializeNlsFiles
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtInitializeRegistry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtInitiatePowerAction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtIsSystemResumeAutomatic
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtIsUILanguageComitted
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtListenPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtLoadDriver
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtLoadKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtLoadKey2
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtLoadKeyEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtLockFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
   Args[0] = (ulonglong)hEvent;
}

void w7NtLockProductActivationKeys
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
		if(Rand()%3==1) *(ulong*)(pPool[0]) = 0x219B;
	}
	else if(R==1)
	{
		Args[0] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
}

void w7NtLockRegistryKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	Args[0] = (ulonglong)hKey;
}

void w7NtLockVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)hProcess;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	*(ulong*)(&Args[3]) |= (GetClassicRandomValue()% ((~0xFFFFFFFC)+1));
}

void w7NtMakePermanentObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)(AllKernelObject[Rand()%AllKernelObjectsUsed]);
}

void w7NtMakeTemporaryObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)(AllKernelObject[Rand()%AllKernelObjectsUsed]);
}

void w7NtMapCMFModule
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   *(ulong*)(&Args[0])  ^= 0xFFE0FE81;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%4;
	if(R == 0) Args[3]=0;
	else if(R==1) Args[3] = -1;
	else if(R==2) Args[3] = GetClassicRandomValue()%0x1001;
	//else random


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
}

void w7NtMapUserPhysicalPages
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtModifyBootEntry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
}

void w7NtModifyDriverEntry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
}


void w7NtNotifyChangeDirectoryFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)hFile;
   //Uncomment
   //if( Rand()%5==1) Args[0]=0;

   Args[1] = (ulonglong)hEvent;
   if( Rand()%5==1) Args[1]=0;


	Args[2] = (ulonglong)DummyAPCRoutine;
	//Args[3] APC Context


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		//IOB
	}
	else if(R==1)
	{
		Args[4] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[6]) = 0;
	else if(R==1) *(ulong*)(&Args[6]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	if( Rand()%2 == 1) *(ulong*)(&Args[7]) ^= 0xFFFFF000;

	if( Rand()% 2 == 1) *(unsigned char*)(&Args[8]) = 0;
}

void w7NtNotifyChangeKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	//Bug found, random APC
	Args[0] = (ulonglong)hKey;


	Args[1] = (ulonglong)hEvent;
	if(Rand()%5==1) Args[1]=0;


	Args[2] = (ulonglong)&DummyAPCRoutine;
	//Args[3] Apc Context

	


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		//IOB
	}
	else if(R==1)
	{
		Args[4] = 0;
	}

	*(ulong*)(&Args[5]) &= 0xF;

	if( Rand()% 2 == 1) *(unsigned char*)(&Args[6]) = 0;

	R = Rand()%3;
	if(R==0)
	{
		Args[7] = (ulonglong)(pPool[7]);
	}
	else if(R==1)
	{
		Args[7] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[8]) = 0;
	else if(R==1) *(ulong*)(&Args[8]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	if( Rand()% 2 == 1) *(unsigned char*)(&Args[9]) = 0;

	//PrintAllArgs(Args);
}

void w7NtNotifyChangeMultipleKeys
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hKey;

	if( Rand()%5 == 1) *(ulong*)(&Args[1]) = Rand()%2;


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	Args[3] = (ulonglong)hEvent;
	if(Rand()%5==1) Args[3]=0;

	Args[4] = (ulonglong)DummyAPCRoutine;
	//Args[5] Apc Context


	R = Rand()%3;
	if(R==0)
	{
		Args[6] = (ulonglong)(pPool[6]);
		//IOB
	}
	else if(R==1)
	{
		Args[6] = 0;
	}

	*(ulong*)(&Args[7]) &= 0xF;


	if( Rand()% 2 == 1) *(unsigned char*)(&Args[8]) = 0;

	R = Rand()%3;
	if(R==0)
	{
		Args[9] = (ulonglong)(pPool[9]);
	}
	else if(R==1)
	{
		Args[9] = 0;
	}

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[10]) = 0;
	else if(R==1) *(ulong*)(&Args[10]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	if( Rand()% 2 == 1) *(unsigned char*)(&Args[11]) = 0;

	//printf("Multi\r\n");
	//PrintAllArgs(Args);
}

void w7NtNotifyChangeSession
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hSession;

   
	if( Rand()%2==1) *(ulong*)(&Args[3]) = 1 + Rand()%2;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[6] = (ulonglong)(pPool[6]);
	}
	else if(R==1)
	{
		Args[6] = 0;
	}

	if( Rand()%2==1) *(ulong*)(&Args[7]) = 1 + Rand()%0x101;
}


void w7NtOpenEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	Args[2] = (ulonglong)hTmRm;
	//Uncomment
	//if( Rand()%5==1) Args[2]=0;

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
		//Guid
	}
	else if(R==1)
	{
		Args[3] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		FillRandomObjectAttributes(pPool[4],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
}

void w7NtOpenEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenIoCompletion
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenJobObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenKeyEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	if( Rand()%2==1) *(ulong*)(&Args[3]) |= 0xC;
}

void w7NtOpenKeyTransacted
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}


	Args[3] = (ulonglong)hRegistryTransaction;
	//Uncomment
	//if(Rand()%15==1) Args[3]=0;
}



void w7NtOpenKeyTransactedEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	if( Rand()%2==1) *(ulong*)(&Args[3]) |= 0xC;

	Args[4] = (ulonglong)hRegistryTransaction;
	//Uncomment
	//if(Rand()%15==1) Args[4]=0;
}

void w7NtOpenKeyedEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenMutant
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenObjectAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
		FillRandomUnicodeString( (void*)(pPool[0]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random



	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomUnicodeString( (void*)(pPool[2]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
		FillRandomUnicodeString( (void*)(pPool[3]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		//Security Descriptor
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random

	HANDLE Tokens[4] = {hToken,hThreadToken,hToken2,0};
  	Args[5] = (ulonglong)(Tokens[Rand()%4]);



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[6])=0;
	else if(R==1) *(ulong*)(&Args[6])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[6])=CalcX;
	}
	//else Random


	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[7])=0;
	else if(R==1) *(ulong*)(&Args[7])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[7])=CalcX;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[8] = (ulonglong)(pPool[8]);
		if(Rand()%5 == 1) *(ulong*)(pPool[8]) = Rand()%0x42;
	}
	else if(R==1)
	{
		Args[8] = 0;
	}
	//else Random

	if(Rand()%2 == 1) *(unsigned char*)(&Args[9]) = 0;
	if(Rand()%2 == 1) *(unsigned char*)(&Args[10]) = 0;

	R = Rand()%3;
	if(R==0)
	{
		Args[11] = (ulonglong)(pPool[11]);
	}
	else if(R==1)
	{
		Args[11] = 0;
	}
	//else Random
}

void w7NtOpenPrivateNamespace
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
}

void w7NtOpenProcessToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)hProcess;
   if( Rand()%5 == 1) Args[0] = -1;


	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	ulong R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenResourceManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}


	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random


	Args[2] = (ulonglong)hTmTm;


	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		FillRandomObjectAttributes(pPool[4],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}

}

void w7NtOpenSemaphore
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenSession
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenSymbolicLinkObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
		if( Rand()%3 == 1)
		{
			((_CLIENT_ID*)(pPool[3]))->UniqueProcess = GetClassicRandomValue()%0x10000;
			((_CLIENT_ID*)(pPool[3]))->UniqueThread = GetClassicRandomValue()%0x10000;
		}
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
}

void w7NtOpenTimer
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hTimer;
	if( Rand()%5==1) Args[0] = 0;



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	ulong R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
}

void w7NtOpenTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hTmTx;
	if( Rand()%5==1) Args[0] = 0;



	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	ulong R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
		//Random GUID
	}
	else if(R==1)
	{
		Args[3] = 0;
	}

	Args[4] = (ulonglong)hTmTm;
	if(Rand()%5 == 1) Args[4]=0;
}

void w7NtOpenTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hTmTm;


	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	ulong R = Rand()%4;
	if(R==0) *(ulong*)(&Args[1])=0;
	else if(R==1) *(ulong*)(&Args[1])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[1])=CalcX;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
		FillRandomObjectAttributes(pPool[2],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
		FillRandomUnicodeString(pPool[3],RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		//Random GUID
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
}

void w7NtPlugPlayControl
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{

	*(ulong*)(&Args[0]) = GetClassicRandomValue()%0x18;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[2]) = 0;
	else if(R==1) *(ulong*)(&Args[2]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random
}

void w7NtPrePrepareComplete
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hTmEn;
	//Uncomment
	//if( Rand()%15==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtPrePrepareEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hTmEn;
	//Uncomment
	//if( Rand()%15==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtPrepareComplete
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hTmEn;
	//Uncomment
	//if( Rand()%15==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtPrepareEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hTmEn;
	//Uncomment
	//if( Rand()%15==1) Args[0] = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtPrivilegeCheck
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random
}





void w7NtPrivilegeObjectAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
		FillRandomUnicodeString( (void*)(pPool[0]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random


	
	if( Rand()%5 == 1) Args[1] = (Rand()%3) - 2;

	HANDLE Tokens[4] = {hToken,hThreadToken,hToken2,0};
  	Args[2] = (ulonglong)(Tokens[Rand()%4]);

	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	R = Rand()%4;
	if(R==0) *(ulong*)(&Args[3])=0;
	else if(R==1) *(ulong*)(&Args[3])=Desireds[Rand()%4];
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		*(ulong*)(&Args[3])=CalcX;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
		if(Rand()%5 == 1) *(ulong*)(pPool[4]) = Rand()%0x42;
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random



	if(Rand()%2 == 1) *(unsigned char*)(&Args[5]) = 0;
}



void w7NtPrivilegedServiceAuditAlarm
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
		FillRandomUnicodeString( (void*)(pPool[0]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
		FillRandomUnicodeString( (void*)(pPool[1]),RANDOM_PAGE_SIZE);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random

	HANDLE Tokens[4] = {hToken,hThreadToken,hToken2,0};
  	Args[2] = (ulonglong)(Tokens[Rand()%4]);


	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
		if(Rand()%5 == 1) *(ulong*)(pPool[3]) = Rand()%0x42;
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random

	if(Rand()%2 == 1) *(unsigned char*)(&Args[4]) = 0;
}

void w7NtPropagationComplete
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)hTmRm;


	ulong R = Rand()%3;
	if(R==0) *(ulong*)(&Args[2]) = 0;
	else if(R==1) *(ulong*)(&Args[2]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random
}

void w7NtPropagationFailed
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0]=(ulonglong)hTmRm;
	//Uncomment
	//if( Rand()%15 == 1) Args[0] = 0;
}

void w7NtPulseEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hEvent;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random

}

void w7NtQueryBootEntryOrder
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtQueryBootOptions
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}

void w7NtQueryDebugFilterState
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   if( Rand()%2 == 1) *(ulong*)(&Args[0]) = GetClassicRandomValue()%0x94;

   if( Rand()%2 == 1) *(ulong*)(&Args[1]) = GetClassicRandomValue()%0x20;
}

void w7NtQueryDirectoryObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hDirectory;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[2]) = 0;
	else if(R==1) *(ulong*)(&Args[2]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	if(Rand()%2 == 1) *(unsigned char*)(&Args[3]) = 0;


	if(Rand()%2 == 1) *(unsigned char*)(&Args[4]) = 0;

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[6] = (ulonglong)(pPool[6]);
	}
	else if(R==1)
	{
		Args[6] = 0;
	}
	//else Random
}

void w7NtQueryDriverEntryOrder
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	ulong R = Rand()%3;
	if(R==0)
	{
		Args[0] = (ulonglong)(pPool[0]);
	}
	else if(R==1)
	{
		Args[0] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random
}




void w7NtQueryEaFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hFile;

	ulong R = Rand()%3;
	if(R==0)
	{
		Args[1] = (ulonglong)(pPool[1]);
	}
	else if(R==1)
	{
		Args[1] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[2] = (ulonglong)(pPool[2]);
	}
	else if(R==1)
	{
		Args[2] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[3]) = 0;
	else if(R==1) *(ulong*)(&Args[3]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	if(Rand()%2 == 1) *(unsigned char*)(&Args[4]) = 0;

	R = Rand()%3;
	if(R==0)
	{
		Args[5] = (ulonglong)(pPool[5]);
	}
	else if(R==1)
	{
		Args[5] = 0;
	}
	//else Random

	R = Rand()%3;
	if(R==0) *(ulong*)(&Args[6]) = 0;
	else if(R==1) *(ulong*)(&Args[6]) = GetClassicRandomValue()%(0x1000+0x1);
	//else Random

	R = Rand()%3;
	if(R==0)
	{
		Args[7] = (ulonglong)(pPool[7]);
	}
	else if(R==1)
	{
		Args[7] = 0;
	}
	//else Random

	if(Rand()%2 == 1) *(unsigned char*)(&Args[8]) = 0;

}

void w7NtQueryFullAttributesFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtQueryInformationAtom
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   unsigned long long Classes[0x2]={0x0,0x1};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x2];
}

void w7NtQueryInformationEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;

   unsigned long long Classes[0x3]={0x0,0x1,0x2};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x3];
}

void w7NtQueryInformationJobObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   HANDLE hJobs[2] = {hJob,hPartitionJob};
   Args[0] = (ulonglong)(hJobs[Rand()%2]);

	unsigned long long Classes[0x2c]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x1a,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f};
	*(ulong*)(&Args[1]) = Classes[Rand()%0x2C];
}

void w7NtQueryInformationPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtQueryInformationResourceManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmRm;

   unsigned long long Classes[0x1]={0x0};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];
}

void w7NtQueryInformationTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTx;

   unsigned long long Classes[0x5]={0x0,0x1,0x2,0x3,0x5};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x5];
}

void w7NtQueryInformationTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTm;

   unsigned long long Classes[0x5]={0x0,0x1,0x2,0x4,0x5};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x5];
}

void w7NtQueryInformationWorkerFactory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTpWorkerFactory;

   unsigned long long Classes[0x1]={0x7};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];
}

void w7NtQueryInstallUILanguage
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryIntervalProfile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryIoCompletion
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hIoCompletion;

   unsigned long long Classes[0x1]={0x0};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];
}

void w7NtQueryLicenseValue
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[3] = GetClassicRandomValue()%(0x800000 + 0x1);
}

void w7NtQueryMultipleValueKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;

   Args[2] = GetClassicRandomValue()%(0x10000 + 1);
}

void w7NtQueryMutant
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hMutex;

   unsigned long long Classes[0x2]={0x0,0x1};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x2];
}

void w7NtQueryOpenSubKeys
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryOpenSubKeysEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryPortInformationProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueryQuotaInformationFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtQuerySecurityAttributesToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);
}

void w7NtQuerySecurityObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = AllKernelObject[Rand()%AllKernelObjectsUsed];
}

void w7NtQuerySemaphore
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hSemaphore;

   unsigned long long Classes[0x1]={0x0};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];
}

void w7NtQuerySymbolicLinkObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hSymLink;
}

void w7NtQuerySystemEnvironmentValue
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQuerySystemEnvironmentValueEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQuerySystemInformationEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

	unsigned long long Classes[0x15]={0x8,0x17,0x2a,0x3d,0x48,0x49,0x53,0x64,0x6b,0x6c,0x79,0x8d,0xa0,0xa5,0xaf,0xb2,0xb4,0xb5,0xd1,0xd2,0xd3};
	*(ulong*)(&Args[0]) = Classes[Rand()%0x15];
}

void w7NtQueryTimerResolution
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtQueueApcThreadEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtRaiseException
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtRaiseHardError
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtReadOnlyEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;
}

void w7NtRecoverEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;
}

void w7NtRecoverResourceManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmRm;
}

void w7NtRecoverTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTm;
}

void w7NtRegisterProtocolAddressInformation
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmRm;
}

void w7NtRegisterThreadTerminatePort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtReleaseKeyedEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKeyedEvent;
}

void w7NtReleaseWorkerFactoryWorker
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTpWorkerFactory;
}

void w7NtRemoveIoCompletionEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hIoCompletion;
}

void w7NtRemoveProcessDebug
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
   Args[1] = (ulonglong)hDebugObject;
}

void w7NtRenameKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
}

void w7NtRenameTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   
   Args[0] = (ulonglong)(pPool[0]);
   FillRandomUnicodeString( pPool[0],RANDOM_PAGE_SIZE);
}

void w7NtReplaceKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[1] = (ulonglong)hKey;
}

void w7NtReplacePartitionUnit
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtReplyWaitReplyPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtRequestPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtResetEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEvent;
   //weird hard error in win10
   //printf("hEvent: %I64X\r\n",hEvent);
   //PrintAllArgs(Args);
}

void w7NtResetWriteWatch
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtRestoreKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
   Args[1] = (ulonglong)hFile;
}

void w7NtResumeProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtRollbackComplete
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;
}

void w7NtRollbackEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;
}

void w7NtRollbackTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTx;
}

void w7NtRollforwardTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTm;
}

void w7NtSaveKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[0] = (ulonglong)hKey;
   Args[1] = (ulonglong)hFile;
}

void w7NtSaveKeyEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[0] = (ulonglong)hKey;
   Args[1] = (ulonglong)hFile;
}

void w7NtSaveMergedKeys
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;
   Args[1] = (ulonglong)hKey;
   Args[2] = (ulonglong)hFile;
}

void w7NtSecureConnectPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSerializeBoot
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetBootEntryOrder
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetBootOptions
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetContextThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hThread;
}

void w7NtSetDebugFilterState
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetDefaultHardErrorPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w7NtSetDefaultLocale
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetDefaultUILanguage
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetDriverEntryOrder
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetEaFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtSetHighEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEventPair;
}

void w7NtSetHighWaitLowEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEventPair;
}

void w7NtSetInformationDebugObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[0] = (ulonglong)hDebugObject;

   unsigned long long Classes[0x1]={0x1};
   *(ulong*)(&Args[1]) = Classes[Rand()%1];
}

void w7NtSetInformationEnlistment
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;

   unsigned long long Classes[0x1]={0x1};
   *(ulong*)(&Args[1]) = Classes[Rand()%1];
}

void w7NtSetInformationJobObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   HANDLE hJobs[2] = {hJob,hPartitionJob};
   Args[0] = (ulonglong)(hJobs[Rand()%2]);

	unsigned long long Classes[0x2c]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xe,0xf,0x10,0x11,0x12,0x13,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x2C];
   if( Rand()%5 == 1) *(ulong*)(&Args[1]) |= 0x80000000;
}

void w7NtSetInformationKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKey;

   unsigned long long Classes[0x6]={0x0,0x1,0x2,0x3,0x4,0x5};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x6];
}

void w7NtSetInformationResourceManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmRm;

	unsigned long long Classes[0x1]={0x1};
	*(ulong*)(&Args[1]) = Classes[Rand()%0x1];
}

void w7NtSetInformationToken
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);

   unsigned long long Classes[0x10]={0x4,0x5,0x6,0xc,0xe,0x10,0x11,0x13,0x17,0x18,0x19,0x1a,0x1b,0x27,0x2a,0x2d};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x10];
}

void w7NtSetInformationTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTx;

   unsigned long long Classes[0x3]={0x1,0x4,0x5};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x3];
}

void w7NtSetInformationTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTm;

   unsigned long long Classes[0x1]={0x3};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];
}

void w7NtSetInformationWorkerFactory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[0] = (ulonglong)hTpWorkerFactory;

   unsigned long long Classes[0xd]={0x2,0x3,0x4,0x5,0x6,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf};
   *(ulong*)(&Args[1]) = Classes[Rand()%0xd];
}

void w7NtSetIntervalProfile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetIoCompletion
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hIoCompletion;
}

void w7NtSetIoCompletionEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hIoCompletion;
   Args[1] = (ulonglong)hIoCompletionReserve;
}

void w7xKdSetupPciDeviceForDebugging
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetLowEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEventPair;
}

void w7NtSetLowWaitHighEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEventPair;
}

void w7NtSetQuotaInformationFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtSetSecurityObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = AllKernelObject[Rand()%AllKernelObjectsUsed];
}

void w7NtSetSystemEnvironmentValue
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetSystemEnvironmentValueEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetSystemInformation
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   unsigned long long Classes[0x4d]={0x9,0x15,0x18,0x1a,0x1b,0x1c,0x1e,0x1f,0x22,0x25,0x26,0x27,0x28,0x29,0x2e,0x2f,0x30,0x33,0x34,0x36,0x38,0x3b,0x45,0x47,0x48,0x4a,0x4b,0x4f,0x50,0x51,0x52,0x56,0x57,0x59,0x5b,0x5c,0x5d,0x5e,0x5f,0x61,0x66,0x68,0x6d,0x6e,0x6f,0x71,0x7e,0x7f,0x81,0x82,0x83,0x84,0x86,0x8e,0x96,0x97,0x98,0x9b,0x9f,0xa1,0xa4,0xa8,0xaa,0xb0,0xb1,0xbb,0xbe,0xbf,0xc2,0xc7,0xcc,0xce,0xcf,0xd2,0xd4,0xe0,0xe1};
   *(ulong*)(&Args[0]) = Classes[Rand()%0x4d];

   //printf("Class: %X\r\n",*(ulong*)(&Args[0]));
}

void w7NtSetSystemPowerState
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetSystemTime
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetThreadExecutionState
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetTimerEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTimer;

   unsigned long long Classes[0x1]={0x0};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x1];
}

void w7NtSetTimerResolution
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetUuidSeed
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSetVolumeInformationFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;


   unsigned long long Classes[0x4]={0x2,0x6,0x8,0xa};
   *(ulong*)(&Args[4]) = Classes[Rand()%0x4];

}

void w7NtShutdownSystem
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtShutdownWorkerFactory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTpWorkerFactory;
}

void w7NtSignalAndWaitForSingleObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtSinglePhaseReject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtStartProfile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProfile;
}

void w7NtStopProfile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProfile;
}

void w7NtSuspendProcess
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hProcess;
}

void w7NtSuspendThread
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
	Args[0] = (ulonglong)hThread;
}

void w7NtSystemDebugControl
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtTerminateJobObject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   HANDLE hJobs[2] = {hJob,hPartitionJob};
   Args[0] = (ulonglong)(hJobs[Rand()%2]);
}

void w7NtTestAlert
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtThawRegistry
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtThawTransactions
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtTraceControl
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtTranslateFilePath
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtUmsThreadYield
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtUnloadDriver
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtUnloadKey
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtUnloadKey2
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtUnloadKeyEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[1] = (ulonglong)hEvent;
}

void w7NtUnlockFile
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w7NtUnlockVirtualMemory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w7NtVdmControl
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w7NtWaitForDebugEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hDebugObject;
}

void w7NtWaitForKeyedEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hKeyedEvent;
}

void w7NtWaitForWorkViaWorkerFactory
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTpWorkerFactory;
}

void w7NtWaitHighEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEventPair;
}

void w7NtWaitLowEventPair
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEventPair;
}

void w7NtWorkerFactoryWorkerReady
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTpWorkerFactory;
}

//--------------- win10 ----------------------
void w10xKdEnumerateDebuggingDevices 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10xHalAllocatePmcCounterSet 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtWaitForAlertByThreadId 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtUpdateWnfStateData 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtUnsubscribeWnfStateChange 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtUnmapViewOfSectionEx 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w10NtSubscribeWnfStateChange 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtSetWnfProcessNotificationEvent 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hEvent;
}

void w10NtSetTimer2 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTimer; 
}

void w10NtSetIRTimer 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTimer;
}

void w10NtSetInformationVirtualMemory 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   Args[0] = (ulonglong)hProcess;
   if(Rand()%5 == 1) Args[0] = -1;


   unsigned long long Classes[0x7]={0x0,0x1,0x2,0x3,0x4,0x5,0x6};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x7];


	ulong R = Rand()%3;
	if(R==0)
	{
		Args[3] = (ulonglong)(pPool[3]);
	}
	else if(R==1)
	{
		Args[3] = 0;
	}
	//else Random


	R = Rand()%3;
	if(R==0)
	{
		Args[4] = (ulonglong)(pPool[4]);
	}
	else if(R==1)
	{
		Args[4] = 0;
	}
	//else Random

	R = Rand()%3;

	if(R==0) *(ulong*)(&Args[5]) = 0x4;
	else if(R==1) *(ulong*)(&Args[5]) = 0x28;
	//else random
}



void w10NtSetInformationSymbolicLink 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hSymLink;


   unsigned long long Classes[0x2]={0x1,0x2};
   *(ulong*)(&Args[1]) = Classes[Rand()%0x2];
}

void w10NtSetCachedSigningLevel 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtRevertContainerImpersonation 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtRenameTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtRegisterProtocolAddressInformation
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmRm;
}

void w10NtQueryWnfStateNameInformation 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtQueryWnfStateData 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtOpenPartition 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtManagePartition 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   HANDLE hPartitions[5]={ (HANDLE)-1,(HANDLE)-2,hPartition,hPartition2,(HANDLE)0};
   Args[0] = (ulonglong)(hPartitions[Rand()%4]);

   Args[1] = (ulonglong)(hPartitions[Rand()%5]);

   ulong ClassX = Rand()%0x6;
   *(ulong*)(&Args[2]) = ClassX;


   Args[3] = (ulonglong) (pPool[3]);

   ulong Sizes[6]={0xF0,0x10,0x28,0x18,0x20,0x28};
   *(ulong*)(&Args[4]) = Sizes[ClassX];
   
}

void w10NtLoadEnclaveData 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtInitializeEnclave 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w10NtGetCurrentProcessorNumberEx 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtGetCompleteWnfStateSubscription 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtGetCachedSigningLevel 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w10NtFlushBuffersFileEx 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
}

void w10NtFilterBootOption 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtDeleteWnfStateName 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtDeleteWnfStateData 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtCreateWnfStateName 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtCreateWaitCompletionPacket 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtCreateTokenEx 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtCreateTimer2 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtCreatePartition 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   
   ulonglong Handles[3] = {0,-1,-2};
   Args[0] = (ulonglong)(Handles[Rand()%3]);
}

void w10NtCreateLowBoxToken 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[1] = (ulonglong)(Tokens[Rand()%3]);
}

void w10NtCreateIRTimer 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtCreateEnclave 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;


   ulonglong EnclaveTypes[4]={1,2,0x10,0x11};
   *(ulong*)(&Args[5]) = EnclaveTypes[Rand()%4];


   //Bypass a bug here, till Microsoft fixes it.
   //Type
   if( *(ulong*)(&Args[5]) == 1)
   {
	   //EnclaveInformationLength 
	   if( *(ulong*)(&Args[7]) == 0)
	   {
		   while(*(ulong*)(&Args[7])==0)
		   {
			   *(ulong*)(&Args[7]) = GetClassicRandomValue();
		   }
	   }
   }
}

void w10NtCreateDirectoryObjectEx 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);

   Args[3] = (ulonglong)hDirectory;
   if( Rand()% 5 == 1) Args[3] = AllKernelObject[Rand()%AllKernelObjectsUsed];
}

void w10NtCompareObjects 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = AllKernelObject[Rand()%AllKernelObjectsUsed];
   Args[1] = AllKernelObject[Rand()%AllKernelObjectsUsed];
}

void w10NtCancelWaitCompletionPacket 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hWaitCompletionPacket;
}

void w10NtCancelTimer2 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTimer;
}

void w10NtAssociateWaitCompletionPacket 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hWaitCompletionPacket;
   Args[1] = (ulonglong)hIoCompletion;
   Args[2] = AllKernelObject[Rand()%AllKernelObjectsUsed];
}

void w10NtAlpcImpersonateClientContainerOfPort 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}

void w10NtAlpcConnectPortEx 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtAlertThreadByThreadId 
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtNullSyscall
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}

void w10NtAcquireCrossVmMutant
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtAcquireProcessActivityReference
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtAdjustTokenClaimsAndDeviceGroups
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtAllocateUserPhysicalPagesEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}


void w10NtAllocateVirtualMemoryEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hProcess;
}

void w10NtCallEnclave
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtCommitRegistryTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hRegistryTransaction;
}


void w10NtCompareSigningLevels
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtCompleteConnectPort
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hLpc;
}


void w10NtContinueEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtConvertBetweenAuxiliaryCounterAndPerformanceCounter
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtCreateCrossVmEvent
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtCreateCrossVmMutant
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtCreateRegistryTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtCreateSectionEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtDirectGraphicsCall
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtFilterTokenEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
	HANDLE Tokens[3] = {hToken,hThreadToken,hToken2};
  	Args[0] = (ulonglong)(Tokens[Rand()%3]);
}



void w10NtManageHotPatch
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtMapViewOfSectionEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hSection;
   Args[1] = (ulonglong)hProcess;
}


void w10NtNotifyChangeDirectoryFileEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
   Args[1] = (ulonglong)hEvent;
}



void w10NtOpenRegistryTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtPssCaptureVaSpaceBulk
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
  
   Args[0] = (ulonglong)hProcess;
   if( Rand()%3 == 1) Args[0] = -1;


   //Base Address
   Args[1] = GetRandomValue()%(0x7FFFFFFEFFFF+0x1);

   //pInfo
   Args[2] = (ulonglong)(pPool[2]);
   ulong Flags[3]={1,2,3};
   *(ulong*)(pPool[2]) = Flags[Rand()%3];
   

   //InfoLength
   Args[3] = 0x10 + GetClassicRandomValue();

   //pResultLength
   Args[4] = (ulonglong)(pPool[4]);
   if( Rand()%3 == 1) Args[4] = 0;
}



void w10NtQueryAuxiliaryCounterFrequency
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtQueryDirectoryFileEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hFile;
   Args[1] = (ulonglong)hEvent;
}

void w10NtQueryInformationByName
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtQuerySecurityPolicy
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}










void w10NtRollbackRegistryTransaction
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hRegistryTransaction;
}


void w10NtRollforwardTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmTm;
}


void w10NtSetCachedSigningLevel2
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}



void w10NtSetInformationTransactionManager
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtSetLdtEntries
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtSinglePhaseReject
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
   Args[0] = (ulonglong)hTmEn;
}


void w10NtTerminateEnclave
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


void w10NtLoadKey3
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}





void w10NtAddAtomEx
(unsigned long SysCall,
unsigned long long* Args,
void** pPool,
void** pSecondLevelPool)
{
   DummyPre(SysCall,Args,pPool,pSecondLevelPool);
}


