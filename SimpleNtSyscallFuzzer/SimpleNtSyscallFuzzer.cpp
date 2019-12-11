// SimpleNtSyscallFuzzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "time.h"

#include "Header.h"





ulong OSVer;
ulong LastSyscall;
ulong bMode; //0 => TOCTOU 1=> Memory Disclosure


//CRITICAL_SECTION CritSec;
//CRITICAL_SECTION CritSec2;
longlong CS = 0;
//----------------------------------------

void ScanContextAndStack()
{
	//CONTEXT_CONTROL 
	//CONTEXT_INTEGER
	//CONTEXT_FLOATING_POINT
	//CONTEXT_SEGMENTS
	//CONTEXT_EXTENDED_REGISTERS
	//CONTEXT_ALL_FLOATING
	//CONTEXT_XSTATE 
	//CONTEXT_FULL 
	//CONTEXT_ALL
	CONTEXT Ctx = {0};
	Ctx.ContextFlags = 
	GetThreadContext(GetCurrentThread(),&Ctx);
}


void PatchUserEntrypoints()
{
	ulong OldP;

	HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
	ulonglong pZwTerminateThread = (ulonglong) GetProcAddress(hNtdll,"ZwTerminateThread");
	void* pRtlUserThreadStart = GetProcAddress(hNtdll,"RtlUserThreadStart");

	//printf("RtlUserThreadStart: %I64X\r\n",pRtlUserThreadStart);

	unsigned char pShell[23] = "\x48\xC7\xC2\xFE\xFF\xFF\xFF\x48\x8B\xCA\x48\xBE\x90\x90\x90\x90\x90\x90\x90\x90\xFF\xD6";
	*(ulonglong*)(pShell+12) = pZwTerminateThread;


	if(VirtualProtect(pRtlUserThreadStart,23,PAGE_EXECUTE_READWRITE,&OldP))
	{
		memcpy(pRtlUserThreadStart,pShell,23);
		VirtualProtect(pRtlUserThreadStart,23,OldP,&OldP);
	}

}


//----------------------------------------
void DummyThread()
{
	Sleep(-1);
}


longlong Func(ulonglong A, ulonglong B, ulonglong C)
{
	wprintf(L"A: %I64X, B: %I64X, C: %I64X\r\n",A,B,C);
	return 0;
}




void Replace_Invalid_Chars(char* pStr)
{
	if(pStr)
	{
		ulong Length = strlen(pStr);
		if(Length)
		{
			int i=0;
			while(i<Length)
			{
				if(pStr[i] == ':') pStr[i] = '-';
				else if(pStr[i]==0xA) pStr[i]=0;
				else if(pStr[i]==0xD) pStr[i]=0;
				i++;
			}
		}
	}
}


int LogOrderToDisk(ulong* pOrder,ulong Count)
{
	if(!pOrder || !Count) return -1;


	time_t Now;
	time(&Now);

	char* pTime = ctime(&Now);
	ulong Length = strlen(pTime);

	//char* pLogFileName = (char*)LocalAlloc(LMEM_ZEROINIT,Length+5);
	char* pLogFileName = (char*)VirtualAlloc(0,Length+5,MEM_COMMIT,PAGE_READWRITE);
	strcpy(pLogFileName,pTime);
	Replace_Invalid_Chars(pLogFileName);
	strcat(pLogFileName,".txt");

	

	printf("%s\r\n",pLogFileName);

	HANDLE hLogFile = CreateFileA(pLogFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,0,0);
	if(hLogFile == INVALID_HANDLE_VALUE)
	{
		printf("Shit: Can't create log file\r\n");
		//LocalFree(pLogFileName);
		VirtualFree(pLogFileName,0,MEM_RELEASE);
		return -2;
	}
	//LocalFree(pLogFileName);
	VirtualFree(pLogFileName,0,MEM_RELEASE);

	unsigned long c = 0;
	//ulong* pSkipped = (ulong*)LocalAlloc(LMEM_ZEROINIT,Count*8);
	ulong* pSkipped = (ulong*)VirtualAlloc(0,Count*8,MEM_COMMIT,PAGE_READWRITE);
	if(!pSkipped)
	{
		printf("Error while allocating memory for syscall order logging\r\n");
		return -3;
	}
	



	unsigned long i = 0;
	for(i=0;i<Count;i++)
	{
		unsigned long EleX = pOrder[i];
		if( ( CallHasBugs(EleX,false) == false) )
		{
			if( (ShouldDeferSyscall(EleX,false) == true))
			{
				pSkipped[c] = EleX;
				c++;
			}
			else
			{
				char Buffer[0x100]={0};
				strcpy(Buffer,"Syscall: ");
				_ultoa(EleX,&Buffer[9],0x10);
				strcat(Buffer,"\x0D\x0A");

				unsigned long BytesWrit;
				if(! WriteFile(hLogFile,Buffer,strlen(Buffer),&BytesWrit,0) )
				{
					printf("Error writing to log file\r\n");
					//LocalFree(pSkipped);
					VirtualFree(pSkipped,0,MEM_RELEASE);
					FlushFileBuffers(hLogFile);
					CloseHandle(hLogFile);
					return -4;
				}
			}
		}
	}

	unsigned long BytesWrit;
	char* Def = "Deferred syscalls===>\r\n";

	if(! WriteFile(hLogFile,Def,strlen(Def),&BytesWrit,0) )
	{
		printf("Error writing to log file\r\n");
		//LocalFree(pSkipped);
		VirtualFree(pSkipped,0,MEM_RELEASE);
		FlushFileBuffers(hLogFile);
		CloseHandle(hLogFile);
		return -4;
	}

	for(i=0;i<c;i++)
	{
		//Log Skipped
		unsigned long EleXX = pSkipped[i];
		char Bufferr[0x100]={0};
		strcpy(Bufferr,"Syscall: ");
		_ultoa(EleXX,&Bufferr[9],0x10);
		strcat(Bufferr,"\x0D\x0A");

		unsigned long BytesWrit;
		if(! WriteFile(hLogFile,Bufferr,strlen(Bufferr),&BytesWrit,0) )
		{
			printf("Error writing to log file\r\n");
			//LocalFree(pSkipped);
			VirtualFree(pSkipped,0,MEM_RELEASE);
			CloseHandle(hLogFile);
			return -4;
		}
	}

	FlushFileBuffers(hLogFile);
	//LocalFree(pSkipped);
	VirtualFree(pSkipped,0,MEM_RELEASE);
	CloseHandle(hLogFile);
	return 0;
}





void* GetDummyExecutablePage()
{
		ulong SizeX = RANDOM_PAGE_SIZE;
		ulonglong* pNew = (ulonglong*)VirtualAlloc(0,SizeX,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		if(!pNew)
		{
			printf("Insufficient resources\r\n");
			ExitProcess(-2);
		}
		*(unsigned char*)pNew = 0xC3;//retn
		*(((char*)pNew)+SizeX-0x8) = 0xC0DEC0DEC0DEC0DE;//Magic Value
		return pNew;
}

void DestroyDummyExecutablePage(void* pNew)
{
	ulong SizeX = RANDOM_PAGE_SIZE;
	if(pNew)
	{
		//if(!VirtualFree(pNew,SizeX,MEM_DECOMMIT))
		if(!VirtualFree(pNew,0,MEM_RELEASE))
		{
			printf("Error freeing random page, plz fix\r\n");
			ExitProcess(-3);
		}
	}
}

bool Returns_64Bit_Handle(unsigned long Syscall)
{
	if( ( Syscall == 0x100A)
		||( Syscall == 0x1055)
		||( Syscall == 0x1064)
		)
	{
		return true;
	}
	return false;
}


void** CreatePagesPool(unsigned long NumArgs)
{
	unsigned long SizeToAlloc = NumArgs*8;

	//void** pPool = (void**)LocalAlloc(LMEM_ZEROINIT,SizeToAlloc);
	void** pPool = (void**)VirtualAlloc(0,0x1000,MEM_COMMIT,PAGE_READWRITE);//assuming SizeToAlloc is less than one page
	if(!pPool)
	{
		printf("Error allocating memory for random pages pool\r\n");
		return 0;
	}

	for(int ii=0;ii<NumArgs;ii++)
	{
		pPool[ii]=GetRandomPage();
		if(!pPool[ii])
		{
			printf("Error allocating memory for random pages pool\r\n");
			VirtualFree(pPool,0,MEM_RELEASE);
			//LocalFree(pPool);
			//code  is incomplete free the ones allocated if any
			return 0;
		}
	}
	return pPool;
}




void DestroyPagesPool(void** pPool,unsigned long NumArgs)
{
	for(int ii=0;ii<NumArgs;ii++)
	{
		DestroyRandomPage(pPool[ii]);
	}
	//LocalFree(pPool);
	VirtualFree(pPool,0,MEM_RELEASE);
}

bool bWin7 = false;
bool bWin10 = false;


extern unsigned long Magic1;
extern unsigned long Magic2;

extern char* NtosCalls[0x1000];
extern char* NtosCalls_7[0x1000];
extern char* NtosCalls_10[0x1000];
extern MYFUNC FuncTable[0x1000];
extern PREFUNC ModifyTable[0x1000];
extern POSTFUNC CleanTable[0x1000];


extern ulonglong AllKernelObject[0x5000];
extern ulong AllKernelObjectsUsed;

extern ulong AllFilesUsed;
extern ulong AllProcessesUsed;


extern ulong cached_AllKernelObjectsUsed;
extern ulong cached_AllFilesUsed;
extern ulong cached_AllProcessesUsed;

int BruteDeferred(unsigned long Count,unsigned long* pSkippedList,ulong NumArgs, ulonglong* Args,void** pPool,void** pSecondLevelPool,ulong MaxIterationCount,bool bRandomTeb,char* Teb,HANDLE hThread)
{
	if(!Count || !pSkippedList  || !Args || !pPool || !pSecondLevelPool || !MaxIterationCount)
	{
		return -1;
	}


	char* Peb = (char*)__readgsqword(0x60);
	printf("PEB at: %I64X\r\n",Peb);


	longlong retValue = -1;

	ulong SizeToAlloc = NumArgs*8;

	/*
	for(unsigned long cc = 0;cc< Count;cc++)
	{
			unsigned long SyscallX = pSkippedList[cc];
			//if( CallHasBugs(Syscall)X) continue;

			printf("Now fuzzing deferred syscall: %X\r\n",SyscallX);
	}
	ExitProcess(0);
	*/

	for(unsigned long c = 0;c< Count;c++)
	{

			
			unsigned long Syscall = pSkippedList[c];
			char* SyscallName = NtosCalls[Syscall];
			printf("Now  fuzzing deferred syscall: %X (%s)\r\n",Syscall,SyscallName);

			if( CallHasBugs(Syscall,false)) continue;

			
			
			retValue = -1;
			
			//ModifyFunction(pFunc,Syscall);

			ulong CountXX = (MaxIterationCount);

			//printf("Def COunt: %X\r\n",CountXX);


			while(CountXX)
			{


					memset(Args,0,SizeToAlloc);
					int i;
					for(i =0; i < NumArgs;i++)
					{
						//ulonglong R = GetRandomValue();
						//wprintf(L"Random value: %I64X\r\n",R);
						//Args[i]=R;
						Args[i] = GetRandomValue();
					}

					if(bMode)
					{
						ulonglong retX;
						do
						{
							retX = InterlockedCompareExchange64(&CS,1,0);
						}while(retX);
					}

					//SuspendThread(hThread);
					//EnterCriticalSection(&CritSec);

					ModifyTable[Syscall](Syscall,Args,pPool,pSecondLevelPool);

					//ModifyArguments0(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments1(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments2(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments3(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments4(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments5(Syscall,Args,pPool,pSecondLevelPool);

					//ModifyArguments6(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments7(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments8(Syscall,Args,pPool,pSecondLevelPool);
					//ModifyArguments9(Syscall,Args,pPool,pSecondLevelPool);


					//for(i =0; i < NumArgs;i++)
					//{
					//	wprintf(L"Random value: %I64X\r\n",Args[i]);
					//}

					

					SetLastError(0);

					unsigned char PEB_r[0x1000];
					unsigned char TEB_r[0x2000];
					unsigned char PEB[0x1000];
					unsigned char TEB[0x2000];

					if(bRandomTeb)
					{
						//Backup Peb/TEB
						FillRandomData(PEB_r,0x1000);
						FillRandomData(TEB_r,0x2000);


						memcpy(PEB,Peb,0x1000);
						memcpy(TEB,Teb,0x2000);


						memcpy(Peb,PEB_r,0x1000);
						memcpy(Teb,TEB_r,0x2000);

					}


					/*retValue = 
					pFunc(Args[0],
						Args[1],
						Args[2],
						Args[3],
						Args[4],
						Args[5],
						Args[6],
						Args[7],
						Args[8],
						Args[9],
						Args[10],
						Args[11],
						Args[12],
						Args[13],
						Args[14],
						Args[15],
						Args[16],
						Args[17],
						Args[18],
						Args[19]);*/

					LastSyscall = Syscall;

					retValue = 
					FuncTable[Syscall](Args[0],
						Args[1],
						Args[2],
						Args[3],
						Args[4],
						Args[5],
						Args[6],
						Args[7],
						Args[8],
						Args[9],
						Args[10],
						Args[11],
						Args[12],
						Args[13],
						Args[14],
						Args[15],
						Args[16],
						Args[17],
						Args[18],
						Args[19]);

					if( CallHasReturnValueLeak(Syscall,true) == false)
					{
						if( ScanRegister((ulonglong)retValue,true,OSVer))
						{
							DumpHex_8(Args,MAX_ARGS*0x8);
							PrintAllArgs(Args);
							printf("Leak: %X, %I64X\r\n",Syscall,(ulonglong)retValue);
							getchar();
						}
					}


					if(bRandomTeb)
					{
						//Restore Peb/Teb
						memcpy(Peb,PEB,0x1000);
						memcpy(Teb,TEB,0x2000);
					}

					ulong LastError = GetLastError();



					//wprintf(L"Syscall: %X, return value: %I64X\r\n",Syscall,(ulonglong)retValue);

					/*
					//Memory leak checker
					if( retValue < 0 )
					{
						//0x1003==> partially leaks _KTHREAD of current thread.
						//0x1012==> leaks full _KTHREAD address of current thread.
						//0x1028==> partially leaks _KTHREAD of current thread.
						//0x102D ==> leak 
						//0x1044 ==> Leak
						//0x1307==>ZwUserSetImeInfoEx Vulnerability, null ptr deref
						//0x10B0 ==> Leak, NtUserRegisterClassExWOW
						if((c != 0x1003) 
							&& (c != 0x1012)
							&& (c != 0x1028)
							&& (c != 0x102D)
							&& (c != 0x1044)
							&& (c != 0x1307)
							&& ( c!= 0x10B0)
							&& (Returns_64Bit_Handle(c)==false))
						{
							printf("Memory leak found\r\n");
							int d = 0;
							scanf_s("%d",&d);
						}

					}
					*/

					CleanTable[Syscall](Syscall,Args,pPool,pSecondLevelPool);
					//CleanArguments(Syscall,Args,pPool,pSecondLevelPool);
					//LeaveCriticalSection(&CritSec);
					//ResumeThread(hThread);

					if(bMode)
					{
						InterlockedDecrement64(&CS);
					}

					CountXX--;
			}
	}
	return 0;
}


//instrusive
//Cons ==> Blocking System Calls (Later use APC to unblock)
unsigned long GetSyscallCount_i(MYFUNC pFunc,bool bWin32k)
{
	MYFUNC pFuncX = 0;
	bool bAllocated = false;
	if(!pFunc)
	{
		pFuncX = CreateFunction(0);
		if(!pFuncX)
		{
			printf("Error allocating memory for dynamic function\r\n");
			return 0;
		}
		bAllocated = true;
	}
	else
	{
		pFuncX = pFunc;
	}


	ulong FinalCount = 0;

	unsigned long MinCall = 0x0;
	unsigned long MaxCall = 0x1000;

	if(bWin32k)
	{
		MinCall += 0x1000;
		MaxCall += 0x1000;
	}



	unsigned long long* Args = (ulonglong*)LocalAlloc(LMEM_ZEROINIT,MAX_ARGS*8);
	if(!Args)
	{
		printf("Error enumerating system calls\r\n");
		return 0;
	}




	for(unsigned long c = MinCall; c<= MaxCall;c++)
	{

		printf("Syscall: %X\r\n",c);

		ModifyFunction(pFuncX,c);

		

		//Call with all params set to 0
		int retValue = 
					pFuncX(Args[0],
						Args[1],
						Args[2],
						Args[3],
						Args[4],
						Args[5],
						Args[6],
						Args[7],
						Args[8],
						Args[9],
						Args[10],
						Args[11],
						Args[12],
						Args[13],
						Args[14],
						Args[15],
						Args[16],
						Args[17],
						Args[18],
						Args[19]);


		if(retValue == 0xC000001C)
		{
			FinalCount = c;
			break;
		}
	}

	LocalFree(Args);
	if(bAllocated) DestroyFunction(pFuncX);

	return FinalCount;
}

//static
unsigned long GetSyscallCount(bool bWin32k)
{

	bool bWin7 = false;
	bool bWin10 = false;


	OSVERSIONINFOW Ver;
	Ver.dwOSVersionInfoSize = sizeof(Ver);
	RtlGetVersion(&Ver);

	ulong Major = Ver.dwMajorVersion;
	ulong Minor = Ver.dwMinorVersion;


	if(Major == 6 && Minor == 1)
	{
		bWin7 = true;
	}
	else if(Major == 10 && Minor == 0)
	{
		bWin10 = true;
	}
	else
	{
		return 0;
	}

	//inspect nt!KiServiceLimit & win32k!W32pServiceLimit
	if(bWin7)
	{
		if(bWin32k) return 0x33B;
		else return 0x191;
	}

	if(bWin10)
	{
		if(bWin32k) return 0x4CA;
		else return 0x1BC;
	}

	return 0;
}


int MigrateSyscalls(unsigned long SyscallCount)
{
	if(!SyscallCount) return -1;

	bool bWin7 = false;
	bool bWin10 = false;


	OSVERSIONINFOW Ver;
	Ver.dwOSVersionInfoSize = sizeof(Ver);
	RtlGetVersion(&Ver);

	ulong Major = Ver.dwMajorVersion;
	ulong Minor = Ver.dwMinorVersion;


	if(Major == 6 && Minor == 1)
	{
		bWin7 = true;
	}
	else if(Major == 10 && Minor == 0)
	{
		bWin10 = true;
	}
	else
	{
		return -2;//Not implemented yet
	}

	if(bWin7)
	{
		memcpy(&NtosCalls[0],&NtosCalls_7[0],SyscallCount*0x8);
	}
	else if(bWin10)
	{
		memcpy(&NtosCalls[0],&NtosCalls_10[0],SyscallCount*0x8);
	}
	return 0;//success
}



void InitOsInfo()
{
	OSVERSIONINFOW Ver;
	Ver.dwOSVersionInfoSize = sizeof(Ver);
	RtlGetVersion(&Ver);

	ulong Major = Ver.dwMajorVersion;
	ulong Minor = Ver.dwMinorVersion;


	if(Major == 6 && Minor == 1)
	{
		bWin7 = true;
		OSVer = 0;
	}
	else if(Major == 10 && Minor == 0)
	{
		bWin10 = true;
		OSVer = 1;
	}
	else
	{
		OSVer = -1;
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	//PatchUserEntrypoints();
	//return 0;
	printf("Hello....Magic: %X, %X\r\n",Magic1,Magic2); //remove later

	bool bSetAffinity = true;
	bool bShared = false;
	ulong bVerbose = 0;
	ulong CountZZ = 0;
	bool bDefer = true;

	//noaf
	//shared
	//verbose
	//nodefer
	//maxcount X




	wchar_t* Cmdline_X =0;
	if(argc > 1)
	{
		for(unsigned long cc=1;cc<argc;cc++)
		{
			Cmdline_X = argv[cc];
			if( wcsstr(Cmdline_X,L"/noaf") )
			{
				bSetAffinity = false;
				//printf("Yes\r\n");
			}
			else if( wcsstr(Cmdline_X,L"/shared") )
			{
				bShared = true;
				//printf("Yes\r\n");
			}
			else if( wcsstr(Cmdline_X,L"/verbose") )
			{
				bVerbose = 1;
				//printf("Yes\r\n");
			}
			else if( wcsstr(Cmdline_X,L"/nodefer") )
			{
				bDefer = false;
				//printf("Yes\r\n");
			}
			else if( wcsstr(Cmdline_X,L"/maxcount") )
			{
				if(cc+1 < argc)
				{
					wchar_t* sCount = argv[cc+1];
					CountZZ = _wtoi(sCount);
				}
				//printf("Yes\r\n");
			}

		}
	}

	printf("bSetAffinity: %X\r\n",bSetAffinity);//remove later
	printf("bShared: %X\r\n",bShared);//remove later
	printf("bVerbose: %X\r\n",bVerbose);//remove later
	printf("CountZZ: %X\r\n",CountZZ);//remove later
	printf("bDefer: %X\r\n",bDefer);//remove later


	
	//----------------------------------------------------------
	
	if(bSetAffinity)
	{
		//-----------------Specific to my machine-------------------
		if(!SetThreadAffinityMask(GetCurrentThread(),0x1))
		{
			printf("Error setting affinity mask\r\n");
			return -1;
		}
		if(SetThreadIdealProcessor(GetCurrentThread(),0) == -1)
		{
			printf("Error setting ideal processor, Err: %X\r\n", GetLastError());
			return -2;
		}
		//-----------------End-------------------------------------
	}
	//--------------------------------------------------------------------------------
	InitOsInfo();

	if( (bWin7 == false && bWin10 == false) || (OSVer == -1) )
	{
		printf("OS Not supported\r\n");
		return -50;
	}



	srand(time(NULL)); 

	//--------------------------------------------------------------------------------
	ulong SyscallCount_t = GetSyscallCount(false);



	int retMi = MigrateSyscalls(SyscallCount_t);
	if(retMi < 0)
	{
		if(retMi == -1)
		{
			printf("OS not supported\r\n");
			return -3;
		}
		else
		{
			printf("Error initializing Syscall Table\r\n");
			return -4;
		}
	}

	

	void* pExecPages = InitFunctionTable(SyscallCount_t,0x0,FuncTable,ModifyTable,CleanTable,OSVer);
	if(!pExecPages)
	{
		printf("Can't initialize Syscall Tables\r\n");
		return -5;
	}



	//--------------------------------------------------------------------------------
	if( bShared == false) 
	{
		if( AllKernelObjectsUsed == 1 ) //First Instance
		{
			if(InitKernelObjects())
			{
				printf("Error creating dummy Kernel objects\r\n");
				DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
				return -6;
			}
		}
		//else Use existing
	}
	else
	{
		if( AllKernelObjectsUsed == 1 ) //Shared requires a prior instance
		{
			printf("Error: no prior instances found\r\n");
			DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
			return -7;
		}
		//else Use existing
	}




	cached_AllKernelObjectsUsed = AllKernelObjectsUsed;

	cached_AllFilesUsed = AllFilesUsed;
	cached_AllProcessesUsed = AllProcessesUsed;


	//-------------------------------------------------------------------------------
	//---- Intrusive-------
	char* Teb = (char*)__readgsqword(0x30);
	char* Peb = (char*)__readgsqword(0x60);
	printf("TEB at: %I64X\r\n",Teb);
	printf("PEB at: %I64X\r\n",Peb);

	
	
	ulonglong Old_KernelCallbackTable = *(ulonglong*)(Peb+0x58);
	printf("Original KernelCallbackTable at: %I64X\r\n",Old_KernelCallbackTable);

	if(Old_KernelCallbackTable == 0)
	{
		printf("Info: Non-GUI thread\r\n");
	}
	else
	{
		printf("Info: GUI thread\r\n");
		/*
		ulonglong* pCallbacks = (ulonglong*)Old_KernelCallbackTable;
		unsigned long CallbackCountX = 0;
	
		while(*pCallbacks)
		{
			CallbackCountX++;
			pCallbacks++;
		}

		if(CallbackCountX == 0)
		{
			printf("Error, Non-GUI thread, plz fix\r\n");
			return -7;
		}
		else
		{
			//pCallbacks = (ulonglong*)LocalAlloc(LMEM_ZEROINIT,(CallbackCountX+1)*8);
			pCallbacks = (ulonglong*)VirtualAlloc(0,(CallbackCountX+1)*8,MEM_COMMIT,PAGE_READWRITE);
			if(!pCallbacks)
			{
				printf("Error while replacing KernelCallbackTable\r\n");
				return -8;
			}
		}

		void* pDummyExec =  GetDummyExecutablePage();
		if(!pDummyExec)
		{
				printf("Error while replacing KernelCallbackTable\r\n");
				//LocalFree(pCallbacks);
				VirtualFree(pCallbacks,0,MEM_RELEASE);
				return -9;
		}


		char* sc = "\x33\xc0\xc3";//xor eax,eax  retn
		memcpy(pDummyExec,sc,3);
		int y = 0;
		while(y < CallbackCountX)
		{
			//pCallbacks[y] = (ulonglong)pDummyExec;
			pCallbacks[y] = (ulonglong)&J_NtCallbackReturn;

			y++;
		}
		pCallbacks[y] = 0;

		*(ulonglong*)(Peb+0x58) = (ulonglong)pCallbacks;//replace it later
		*/
	}
	//--------------------------------------------------------
	unsigned long NumArgs = MAX_ARGS;//Depending upon os version/SP/Build Number/Arch
									//later will be dynamic instead of defined constant
	ulong SizeToAlloc = NumArgs*8;
	ulonglong* Args = (ulonglong*)VirtualAlloc(0,0x1000,MEM_COMMIT,PAGE_READWRITE);
	if(!Args)
	{
		printf("Error allocating memory for dynamic arguments\r\n");
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -8;
	}
	//----------------------------------------------------------
	void** pPool = CreatePagesPool(NumArgs);
	if(!pPool)
	{
		printf("Error allocating memory for 1st level pages pool\r\n");
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -9;
	}

	void** pSecondLevelPool = CreatePagesPool(NumArgs);
	if(!pSecondLevelPool)
	{
		printf("Error allocating memory for 2nd level pages pool\r\n");
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -10;
	}
	//-------------------------------------------------------------
	//InitializeCriticalSection(&CritSec);

	unsigned long r_ThreadId = 0;
	RANDOMIZATION_THREAD RT={0};

	RT.Count = NumArgs;
	RT.pPool=pPool;

	RT.SecondLevelCount=NumArgs;
	RT.pSecondLevelPool = pSecondLevelPool;

	HANDLE hThread_r = CreateThread(0,0x1000,(LPTHREAD_START_ROUTINE)RandomiztionThread,&RT,CREATE_SUSPENDED,&r_ThreadId);
	if(!hThread_r)
	{
		printf("Error creating randomization thread\r\n");

		//DeleteCriticalSection(&CritSec);

		DestroyPagesPool(pSecondLevelPool,NumArgs);
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -11;
	}
	else
	{
		if(bSetAffinity)
		{
			//-----------------Specific to my machine-------------------
			if(!SetThreadAffinityMask(hThread_r,0x2))
			{
				printf("Warning: problem while setting affinity mask\r\n");
			}
			if(SetThreadIdealProcessor(hThread_r,1) == -1)
			{
				printf("Wanring: problem while setting ideal processor, Err: %X\r\n", GetLastError());
			}
			//-----------------End-------------------------------------
		}
	}
	//--------------------------------
	//--------------------------------

	//Syscall order randomization should go here
	unsigned long TotalCallCount = SyscallCount_t;

	printf("Total ntoskrnl Syscall count: %X\r\n",TotalCallCount);	
	
	
	unsigned long MinX = 0x0;
	unsigned long MaxX = (MinX + TotalCallCount)-1;
	printf("First Syscall ordinal: %X\r\n",MinX);
	printf("Last Syscall ordinal: %X\r\n",MaxX);


	



	ulong* pOrder = GetRandomSyscallOrder_Swap(MinX,MaxX);
	if(!pOrder)
	{
		printf("Error randomizing syscalls orders\r\n");


		//Thread is already suspended
		//EnterCriticalSection(&CritSec);
		TerminateThread(hThread_r,-12);
		//LeaveCriticalSection(&CritSec);

		//DeleteCriticalSection(&CritSec);

		DestroyPagesPool(pSecondLevelPool,NumArgs);
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -12;
	}



	//later add another param to receive file handle
	//or move it to beginning of main
	/*
	if(LogOrderToDisk(pOrder,TotalCallCount)< 0)
	{
		printf("Can't create log file\r\n");
		VirtualFree(pOrder,0,MEM_RELEASE);

		EnterCriticalSection(&CritSec);
		TerminateThread(hThread_r,-16);
		LeaveCriticalSection(&CritSec);

		DeleteCriticalSection(&CritSec);

		DestroyPagesPool(pSecondLevelPool,NumArgs);
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -13;
	}
	*/
	//----------------------
	//Ignoring vulnerable and disruptive syscalls
	//Deferring destructive syscalls
	ulong SkippedCalls = 0;

	ulong BuggyCalls = 0;


	ulong* pSkipped = 0;




	//Get total number of syscalls to defer and number of syscalls to Ignore
	ulong TotalCallCount_X = TotalCallCount;
	
	ulong* pOrder_temp = (ulong*)VirtualAlloc(0,TotalCallCount*0x4,MEM_COMMIT,PAGE_READWRITE);
	pSkipped =			 (ulong*)VirtualAlloc(0,TotalCallCount*0x4,MEM_COMMIT,PAGE_READWRITE);

	if((!pOrder_temp) || (!pSkipped))
	{
				printf("Error allocating memory for skipped list\r\n");

				if(pSkipped) VirtualFree(pSkipped,0,MEM_RELEASE);
				if(pOrder_temp) VirtualFree(pOrder_temp,0,MEM_RELEASE);


				VirtualFree(pOrder,0,MEM_RELEASE);

				//EnterCriticalSection(&CritSec);
				TerminateThread(hThread_r,-14);
				//LeaveCriticalSection(&CritSec);

				//DeleteCriticalSection(&CritSec);

				DestroyPagesPool(pSecondLevelPool,NumArgs);
				DestroyPagesPool(pPool,NumArgs);
				VirtualFree(Args,0,MEM_RELEASE);
				DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
				return -14;
	}


	unsigned long xyz = 0;
	unsigned long syscal = 0;
	for(unsigned long ccc = 0;ccc<TotalCallCount;ccc++)
	{
			unsigned long syscal  = pOrder[ccc];
			if(CallHasBugs(syscal,true))
			{
					BuggyCalls++;
					
					TotalCallCount_X--;
			}
			else if( (ShouldDeferSyscall(syscal,true)) && bDefer == true)
			{
				    pSkipped[SkippedCalls] = syscal;

					SkippedCalls++;
					
					TotalCallCount_X--;
			}

			else
			{
				pOrder_temp[xyz] = syscal;
				xyz++;
			}
	}

	//xyz must be equal to TotalCallCount_X

	VirtualFree(pOrder,0,MEM_RELEASE);
	pOrder = pOrder_temp;
	pOrder_temp = 0;


	if(SkippedCalls == 0)
	{
		printf("N.B. No syscalls to defer, destructive!!!!\r\n");
		VirtualFree(pSkipped,0,MEM_RELEASE);
		pSkipped=0;
	}


	ulong CountXX = (MAX_BRUTE);
	if(CountZZ != 0) CountXX = (CountZZ);

	


	ulong* pCounts = (ulong*)VirtualAlloc(0,TotalCallCount_X*0x4,MEM_COMMIT,PAGE_READWRITE);
	if(!pCounts)
	{
		printf("Error allocating memory for Counts array\r\n");

		if(pSkipped)
		{
			VirtualFree(pSkipped,0,MEM_RELEASE);
			pSkipped = 0;
		}

		VirtualFree(pOrder,0,MEM_RELEASE);

		//EnterCriticalSection(&CritSec);
		TerminateThread(hThread_r,-15);
		//LeaveCriticalSection(&CritSec);

		//DeleteCriticalSection(&CritSec);

		DestroyPagesPool(pSecondLevelPool,NumArgs);
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);

		return -15;
	}


	for(ulong g=0;g<TotalCallCount_X;g++)
	{
		pCounts[g] = CountXX;
	}


	
	
	//-----------------------
	//Start Message Waiting Thread/APC



	HANDLE hEvent = CreateEvent(0,TRUE,FALSE,L"APC");
	if(hEvent == INVALID_HANDLE_VALUE)
	{
		printf("Error creating kernel event for APC\r\n");
		
		VirtualFree(pCounts,0,MEM_RELEASE);

		if(pSkipped)
		{
			VirtualFree(pSkipped,0,MEM_RELEASE);
			pSkipped = 0;
		}

		VirtualFree(pOrder,0,MEM_RELEASE);

		//EnterCriticalSection(&CritSec);
		TerminateThread(hThread_r,-16);
		//LeaveCriticalSection(&CritSec);

		//DeleteCriticalSection(&CritSec);

		DestroyPagesPool(pSecondLevelPool,NumArgs);
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -16;
	}

	APC_LOOP_THREAD* pApcLoopThread = (APC_LOOP_THREAD*)VirtualAlloc(0,sizeof(APC_LOOP_THREAD),MEM_COMMIT,PAGE_READWRITE);
	if(!pApcLoopThread)
	{
		printf("Error Allocating memory for APC structure\r\n");
		
		CloseHandle(hEvent);


		VirtualFree(pCounts,0,MEM_RELEASE);


		if(pSkipped)
		{
			VirtualFree(pSkipped,0,MEM_RELEASE);
			pSkipped = 0;
		}


		VirtualFree(pOrder,0,MEM_RELEASE);

		//EnterCriticalSection(&CritSec);
		TerminateThread(hThread_r,-17);
		//LeaveCriticalSection(&CritSec);

		//DeleteCriticalSection(&CritSec);

		DestroyPagesPool(pSecondLevelPool,NumArgs);
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -17;
	}


	pApcLoopThread->ThreadId = GetCurrentThreadId();
	pApcLoopThread->hEvent = hEvent;

	unsigned long Tid_ApcThread = 0;
	HANDLE hThread_Apc = CreateThread(0,0x1000,(LPTHREAD_START_ROUTINE)&ApcLoopThread,pApcLoopThread,CREATE_SUSPENDED,&Tid_ApcThread);
	if(!hThread_Apc)
	{
		printf("Error creating APC Loop Thread\r\n");


		VirtualFree(pApcLoopThread,0,MEM_RELEASE);

		CloseHandle(hEvent);

		VirtualFree(pCounts,0,MEM_RELEASE);


		if(pSkipped)
		{
			VirtualFree(pSkipped,0,MEM_RELEASE);
			pSkipped = 0;
		}



		VirtualFree(pOrder,0,MEM_RELEASE);

		//EnterCriticalSection(&CritSec);
		TerminateThread(hThread_r,-18);
		//LeaveCriticalSection(&CritSec);

		//DeleteCriticalSection(&CritSec);

		DestroyPagesPool(pSecondLevelPool,NumArgs);
		DestroyPagesPool(pPool,NumArgs);
		VirtualFree(Args,0,MEM_RELEASE);
		DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
		return -18;
	}
	else
	{
		if(bSetAffinity)
		{
			//-----------------Specific to my machine-------------------
			if(!SetThreadAffinityMask(hThread_Apc,0x4))
			{
				printf("Warning problem while setting affinity mask\r\n");
			}
			if(SetThreadIdealProcessor(hThread_Apc,2)==-1)
			{
				printf("Warning problem while setting ideal processor, Err: %X\r\n", GetLastError());
			}
			//------------------------------------------------------------
		}
	}


	//-----------------------
	int x=0;
	longlong retValue = -1;
	ulong CountXXX;
	ulong bRandomTeb = 0; // =  Rand()%2;
	


	

	ResumeThread(hThread_r);
	ResumeThread(hThread_Apc);
	Sleep(10);

	PatchUserEntrypoints();


	


	while(pCounts[0])
	{
		printf("Fuzzing Round #: %X\r\n",x);
		x++;

		for(unsigned long cc = 0;cc<TotalCallCount_X;cc++)
		{
				unsigned long c = pOrder[cc];
			


				if(bVerbose) printf("Now fuzzing syscall: %X (%s)\r\n",c,NtosCalls[c]);

				retValue = -1;
				unsigned long Syscall = c;
				//ModifyFunction(pFunc,Syscall);

				CountXXX = pCounts[cc];

				if(CountXXX)
				{
						memset(Args,0,SizeToAlloc);
						int i;
						for(i =0; i < NumArgs;i++)
						{
							Args[i] = GetRandomValue();
						}

						if(bMode)
						{
							ulonglong retX;
							do
							{
								retX = InterlockedCompareExchange64(&CS,1,0);
							}while(retX);
						}


						//SuspendThread(hThread_r);
						//EnterCriticalSection(&CritSec);
						//WaitForSingleObject(hMutex,INFINITE);



						ModifyTable[Syscall](c,Args,pPool,pSecondLevelPool);


						//ModifyArguments0(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments1(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments2(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments3(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments4(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments5(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments6(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments7(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments8(c,Args,pPool,pSecondLevelPool);
						//ModifyArguments9(c,Args,pPool,pSecondLevelPool);


						/*for(i =0; i < NumArgs;i++)
						{
							wprintf(L"Random value: %I64X\r\n",Args[i]);
						}*/




					
						SetLastError(0);

						unsigned char PEB_r[0x1000];
						unsigned char TEB_r[0x2000];
						unsigned char PEB[0x1000];
						unsigned char TEB[0x2000];

						if(bRandomTeb)
						{
							//Backup Peb/TEB
							FillRandomData(PEB_r,0x1000);
							FillRandomData(TEB_r,0x2000);


							memcpy(PEB,Peb,0x1000);
							memcpy(TEB,Teb,0x2000);


							memcpy(Peb,PEB_r,0x1000);
							memcpy(Teb,TEB_r,0x2000);

						}
						






						LastSyscall = Syscall;


							retValue = 
							FuncTable[Syscall](Args[0],
							Args[1],
							Args[2],
							Args[3],
							Args[4],
							Args[5],
							Args[6],
							Args[7],
							Args[8],
							Args[9],
							Args[10],
							Args[11],
							Args[12],
							Args[13],
							Args[14],
							Args[15],
							Args[16],
							Args[17],
							Args[18],
							Args[19]);




						if( CallHasReturnValueLeak(Syscall,true) == false)
						{
							if( ScanRegister((ulonglong)retValue,true,OSVer))
							{
								DumpHex_8(Args,MAX_ARGS*0x8);
								PrintAllArgs(Args);
								printf("Leak: %X, %I64X\r\n",Syscall,(ulonglong)retValue);
								getchar();
							}
						}


						if(bRandomTeb)
						{
							//Restore Peb/Teb
							memcpy(Peb,PEB,0x1000);
							memcpy(Teb,TEB,0x2000);
						}

						ulong LastError = GetLastError();


					




						CleanTable[Syscall](c,Args,pPool,pSecondLevelPool);
						//CleanArguments(c,Args,pPool,pSecondLevelPool);

						//printf("Exit\r\n");

						//LeaveCriticalSection(&CritSec);
						//ReleaseMutex(hMutex);
						//ResumeThread(hThread_r);
						if(bMode)
						{
							InterlockedDecrement64(&CS);
						}


						//CountXXX--;
						pCounts[cc]--;

						if(retValue == 0xC000001C)
						{


								if(pSkipped)
								{
									if(bShared == false) BruteDeferred(SkippedCalls,pSkipped,NumArgs,Args,pPool,pSecondLevelPool,CountXX,bRandomTeb,Teb,hThread_r);
									//LocalFree(pSkipped);
									VirtualFree(pSkipped,0,MEM_RELEASE);
									pSkipped = 0;
								}

								printf("=============== Almost DONE =======\r\n");

								
								TerminateThread(hThread_Apc,0);
								VirtualFree(pApcLoopThread,0,MEM_RELEASE);
								CloseHandle(hEvent);
								
								VirtualFree(pCounts,0,MEM_RELEASE);

								if(pSkipped) 
								{
									VirtualFree(pSkipped,0,MEM_RELEASE);
									pSkipped = 0;
								}

								VirtualFree(pOrder,0,MEM_RELEASE);


								//EnterCriticalSection(&CritSec);
								TerminateThread(hThread_r,0);
								//LeaveCriticalSection(&CritSec);
								//DeleteCriticalSection(&CritSec);

								DestroyPagesPool(pSecondLevelPool,NumArgs);
								DestroyPagesPool(pPool,NumArgs);
								VirtualFree(Args,0,MEM_RELEASE);
								DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
								printf("=============== DONE =======\r\n");
								AllKernelObjectsUsed = 1;
								if(bShared == false) ZwTerminateProcess(GetCurrentProcess(),0);
								//ExitProcess(0);//till i fix heap
								return 0;
						}
				}

			
		}
		RandomizeOrderAndCounts(pOrder,pCounts,TotalCallCount_X);
	}

	if(pSkipped)
	{
		if(bShared == false) BruteDeferred(SkippedCalls,pSkipped,NumArgs,Args,pPool,pSecondLevelPool,CountXX,bRandomTeb,Teb,hThread_r);
		VirtualFree(pSkipped,0,MEM_RELEASE);
		pSkipped = 0;
	}
	printf("=============== Almost DONE =======\r\n");
	TerminateThread(hThread_Apc,0);
	VirtualFree(pApcLoopThread,0,MEM_RELEASE);
	CloseHandle(hEvent);
	VirtualFree(pCounts,0,MEM_RELEASE);

	if(pSkipped)
	{
		VirtualFree(pSkipped,0,MEM_RELEASE);
		pSkipped = 0;
	}


	VirtualFree(pOrder,0,MEM_RELEASE);


	//EnterCriticalSection(&CritSec);
	TerminateThread(hThread_r,0);
	//LeaveCriticalSection(&CritSec);
	//DeleteCriticalSection(&CritSec);

	DestroyPagesPool(pSecondLevelPool,NumArgs);
	DestroyPagesPool(pPool,NumArgs);
	VirtualFree(Args,0,MEM_RELEASE);
	DestroyFunctionTable(pExecPages,SyscallCount_t,FuncTable,ModifyTable,CleanTable);
	printf("=============== DONE =======\r\n");
	if(bShared == false) AllKernelObjectsUsed = 1;
	ZwTerminateProcess(GetCurrentProcess(),0);
	//ExitProcess(0);//till i fix heap
	return 0;
}
