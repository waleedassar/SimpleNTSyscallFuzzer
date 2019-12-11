#include "stdafx.h"
#include "Header.h"


extern char* NtosCalls[0x1000];
extern bool bWin7;
extern bool bWin10;


bool ShouldSkipLeakChecking_10(unsigned long SysCall)
{
	/*
	char* SyscallName = NtosCalls[SysCall];
	if(SysCall == 0x108F) //NtUserGetTitleBarInfo
	{
		printf("Ignoring %X (%s), found Memory Disclosure/Leak\r\n",SysCall,SyscallName);
		return true;
	}
	else if(SysCall== 0x1017) //NtUserBeginPaint
	{
		printf("Ignoring %X (%s), found Memory Disclosure/Leak\r\n",SysCall,SyscallName);
		return true;
	}
	*/
	return false;
}

bool ShouldSkipLeakChecking_7(unsigned long SysCall)
{

	char* SyscallName = NtosCalls[SysCall];
	if(SysCall == 0x3) //NtReadFile
	{
		//printf("Ignoring %X (%s), found Memory Disclosure/Leak\r\n",SysCall,SyscallName);
		return true;
	}
	else if(SysCall== 0x5) //NtWriteFile
	{
		//printf("Ignoring %X (%s), found Memory Disclosure/Leak\r\n",SysCall,SyscallName);
		return true;
	}
	else if(SysCall== 0x188) //NtUnlockFile
	{
		//printf("Ignoring %X (%s), found Memory Disclosure/Leak\r\n",SysCall,SyscallName);
		return true;
	}
	else if(SysCall== 0xE0) //NtLockFile
	{
		//printf("Ignoring %X (%s), found Memory Disclosure/Leak\r\n",SysCall,SyscallName);
		return true;
	}
	else if(SysCall== 0xCD) //NtGetMUIRegistryInfo
	{
		//printf("Ignoring %X (%s), found Memory Disclosure/Leak\r\n",SysCall,SyscallName);
		return true;
	}
	return false;
}

bool ShouldSkipLeakChecking(unsigned long SysCall)
{
	if(bWin7) return ShouldSkipLeakChecking_7(SysCall);
	else if(bWin10) ShouldSkipLeakChecking_10(SysCall);
	return false;
}


bool CallHasReturnValueLeak_10(unsigned long SysCall,bool bPrint)
{
	/*
		char* SyscallName = NtosCalls[SysCall];
		if(SysCall== 0) //NtGdiDdDDICheckExclusiveOwnership
		{
			if(bPrint)  printf("Ignoring %X (%s), found leak in return value\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x10E8) //NtUserSetWindowWord
		{
			if(bPrint)  printf("Ignoring %X (%s), found leak in return value\r\n",SysCall,SyscallName);
			return true;
		}
	*/
		return false;
}

bool CallHasReturnValueLeak_7(unsigned long SysCall,bool bPrint)
{
	/*
		char* SyscallName = NtosCalls[SysCall];
		if(SysCall== 0) //NtGdiDdDDICheckExclusiveOwnership
		{
			if(bPrint)  printf("Ignoring %X (%s), found leak in return value\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x10E8) //NtUserSetWindowWord
		{
			if(bPrint)  printf("Ignoring %X (%s), found leak in return value\r\n",SysCall,SyscallName);
			return true;
		}
	*/
		return false;
}

bool CallHasReturnValueLeak(unsigned long SysCall,bool bPrint)
{
	if(bWin7) return CallHasReturnValueLeak_7(SysCall,bPrint);
	else if(bWin10) return CallHasReturnValueLeak_10(SysCall,bPrint);
	return false;
}

bool CallHasBugs_10(unsigned long SysCall,bool bPrint)
{
		char* SyscallName = NtosCalls[SysCall];

		if(SysCall== 0x157) //NtReleaseKeyedEvent
		{
			if(bPrint)  printf("Ignoring Syscall: %X NtReleaseKeyedEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x4) //NtWaitForSingleObject
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForSingleObject (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0xF) //NtClose
		{
			if(bPrint) printf("Ignoring Syscall: %X NtClose (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x1B8) //NtWaitForKeyedEvent
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForKeyedEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x43) //NtContinue
		{
			if(bPrint) printf("Ignoring Syscall: %X NtContinue (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x2C) //NtTerminateProcess
		{
			if(bPrint) printf("Ignoring Syscall: %X NtTerminateProcess (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x34) //NtDelayExecution
		{
			if(bPrint) printf("Ignoring Syscall: %X NtDelayExecution (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x53) //NtTerminateThread
		{
			if(bPrint) printf("Ignoring Syscall: %X NtTerminateThread (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x1A2) //NtSuspendProcess
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSuspendProcess (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x1A3) //NtSuspendThread
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSuspendThread (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x172) //NtSetContextThread
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSetContextThread (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x1A6) //NtTestAlert
		{
			if(bPrint) printf("Ignoring Syscall: %X NtTestAlert (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x19D) //NtSignalAndWaitForSingleObject
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSignalAndWaitForSingleObject (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0xF6) //NtListenPort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtListenPort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x150) //NtRaiseHardError
		{
			if(bPrint) printf("Ignoring Syscall: %X NtRaiseHardError (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x88) //NtAlpcSendWaitReceivePort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtAlpcSendWaitReceivePort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0xB) //NtReplyWaitReceivePort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReceivePort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x2B) //NtReplyWaitReceivePortEx
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReceivePortEx (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x1B6) //NtWaitForAlertByThreadId
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForAlertByThreadId (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x1B5) //NtWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x1BA) //NtWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x18B) //NtWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x168) //NtRollbackTransaction
		{
			if(bPrint) printf("Ignoring Syscall: %X NtRollbackTransaction (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x9) //NtRemoveIoCompletion
		{
			if(bPrint) printf("Ignoring Syscall: %X NtRemoveIoCompletion (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x1B0) //NtUnlockFile
		{
			if(bPrint) printf("Ignoring Syscall: %X NtUnlockFile (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0xFF) //NtLockVirtualMemory
		{
			if(bPrint) printf("Ignoring Syscall: %X NtLockVirtualMemory (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x1B9) //NtWaitForWorkViaWorkerFactory
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForWorkViaWorkerFactory (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x161) //NtResetEvent
		{
			if(bPrint) printf("Ignoring Syscall: %X NtResetEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == 0x15F) //NtReplyWaitReplyPort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReplyPort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		return false;
}

bool CallHasBugs_7(unsigned long SysCall,bool bPrint)
{
		char* SyscallName = NtosCalls[SysCall];

		if(SysCall== 0x137) //NtReleaseKeyedEvent
		{
			if(bPrint)  printf("Ignoring Syscall: %X NtReleaseKeyedEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x1) //NtWaitForSingleObject
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForSingleObject (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0xC) //NtClose
		{
			if(bPrint) printf("Ignoring Syscall: %X NtClose (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x18C) //NtWaitForKeyedEvent
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForKeyedEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x40) //NtContinue
		{
			if(bPrint) printf("Ignoring Syscall: %X NtContinue (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x29) //NtTerminateProcess
		{
			if(bPrint) printf("Ignoring Syscall: %X NtTerminateProcess (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x31) //NtDelayExecution
		{
			if(bPrint) printf("Ignoring Syscall: %X NtDelayExecution (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x50) //NtTerminateThread
		{
			if(bPrint) printf("Ignoring Syscall: %X NtTerminateThread (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x17A) //NtSuspendProcess
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSuspendProcess (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x17B) //NtSuspendThread
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSuspendThread (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x150) //NtSetContextThread
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSetContextThread (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x17E) //NtTestAlert
		{
			if(bPrint) printf("Ignoring Syscall: %X NtTestAlert (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x176) //NtSignalAndWaitForSingleObject
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSignalAndWaitForSingleObject (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0xDB) //NtListenPort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtListenPort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x130) //NtRaiseHardError
		{
			if(bPrint) printf("Ignoring Syscall: %X NtRaiseHardError (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x82) //NtAlpcSendWaitReceivePort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtAlpcSendWaitReceivePort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x8) //NtReplyWaitReceivePort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReceivePort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x28) //NtReplyWaitReceivePortEx
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReceivePortEx (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x158) //NtSetHighWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSetHighWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x167) //NtSetLowWaitHighEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSetLowWaitHighEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x18E) //NtWaitHighEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitHighEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x18F) //NtWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x18B) //NtWaitForDebugEvent
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForDebugEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x130) //NtRaiseHardError
		{
			if(bPrint) printf("Ignoring Syscall: %X NtRaiseHardError (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0xEB) //NtNotifyChangeKey
		{
			if(bPrint) printf("Ignoring Syscall: %X NtNotifyChangeKey (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0xEC) //NtNotifyChangeMultipleKeys
		{
			if(bPrint) printf("Ignoring Syscall: %X NtNotifyChangeMultipleKeys (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		return false;
}

bool CallHasBugs(unsigned long SysCall,bool bPrint)
{
	if(bWin7) return CallHasBugs_7(SysCall,bPrint);
	else if(bWin10) return CallHasBugs_10(SysCall,bPrint);
	return false;
}








bool ShouldDeferSyscall_10(unsigned long SysCall,bool bPrint)
{
	/*
		char* SyscallName = NtosCalls[SysCall];

		if(SysCall== 0x2) //NtAcceptConnectPort
		{
			if(bPrint) printf("Deferring Syscall: %X NtAcceptConnectPort, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x3) //NtMapUserPhysicalPagesScatter
		{
			if(bPrint) printf("Deferring Syscall: %X NtMapUserPhysicalPagesScatter, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
	*/
		return false;
}

bool ShouldDeferSyscall_7(unsigned long SysCall,bool bPrint)
{

		char* SyscallName = NtosCalls[SysCall];

		if(SysCall== 0x17D) //NtTerminateJobObject
		{
			if(bPrint) printf("Deferring Syscall: %X NtTerminateJobObject, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== 0x6) //NtRemoveIoCompletion
		{
			if(bPrint) printf("Deferring Syscall: %X NtRemoveIoCompletion, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		return false;
}

bool ShouldDeferSyscall(unsigned long SysCall,bool bPrint)
{
	if(bWin7) return ShouldDeferSyscall_7(SysCall,bPrint);
	else if(bWin10) return ShouldDeferSyscall_10(SysCall,bPrint);

	return false;
}






/*
bool IsBlockingSyscall(unsigned long SysCall,bool bPrint)
{
	if(SysCall== 0x129A) //NtUserDragObject
	{
		if(bPrint) printf("Ignoring %X NtUserDragObject\r\n",SysCall);
		return true;
	}
	else if(SysCall== 0x1299) //NtUserDragDetect
	{
		if(bPrint) printf("Ignoring %X NtUserDragDetect\r\n",SysCall);
		return true;
	}
	else if(SysCall == 0x1006) //NtUserGetMessage
	{
		if(bPrint) printf("Ignoring %X NtUserGetMessage\r\n",SysCall);
		return true;
	}
	else if(SysCall == 0x100C) //NtUserWaitMessage
	{
		if(bPrint) printf("Ignoring %X NtUserWaitMessage\r\n",SysCall);
		return true;
	}
	else if(SysCall== 0x12A3) //NtUserEvent
	{
		if(bPrint) printf("Ignoring %X NtUserEvent\r\n",SysCall);
		return true;
	}
	return false;
}
*/

/*
bool IsCsrssSyscall(unsigned long SysCall,bool bPrint)
{
	if(SysCall == 0x12D3) //NtUserInitialize
	{
		if(bPrint) printf("Ignoring %X NtUserInitialize, requires csrss.exe\r\n",SysCall);
		return true;
	}
	return false;
}
*/