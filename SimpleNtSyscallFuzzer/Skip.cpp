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

//------------------
//Calls where bugs had been found
//disruptive and blocking calls
bool CallHasBugs_10(unsigned long SysCall,bool bPrint)
{
		char* SyscallName = NtosCalls[SysCall];

		//Blocking calls
		if(SysCall== macr_NtWaitForSingleObject) //NtWaitForSingleObject
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForSingleObject (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtWaitForKeyedEvent) //NtWaitForKeyedEvent
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForKeyedEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtDelayExecution) //NtDelayExecution
		{
			if(bPrint) printf("Ignoring Syscall: %X NtDelayExecution (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtTestAlert) //NtTestAlert
		{
			if(bPrint) printf("Ignoring Syscall: %X NtTestAlert (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtSignalAndWaitForSingleObject) //NtSignalAndWaitForSingleObject
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSignalAndWaitForSingleObject (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtListenPort) //NtListenPort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtListenPort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtAlpcSendWaitReceivePort) //NtAlpcSendWaitReceivePort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtAlpcSendWaitReceivePort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtReplyWaitReceivePort) //NtReplyWaitReceivePort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReceivePort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtReplyWaitReceivePortEx) //NtReplyWaitReceivePortEx
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReceivePortEx (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtWaitForAlertByThreadId) //NtWaitForAlertByThreadId
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForAlertByThreadId (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtWaitLowEventPair) //NtWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtWaitLowEventPair) //NtWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtWaitLowEventPair) //NtWaitLowEventPair
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitLowEventPair (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtWaitForWorkViaWorkerFactory) //NtWaitForWorkViaWorkerFactory
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForWorkViaWorkerFactory (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtWaitForDebugEvent) //NtWaitForDebugEvent
		{
			if(bPrint) printf("Ignoring Syscall: %X NtWaitForDebugEvent (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtReplyWaitReplyPort) //NtReplyWaitReplyPort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtReplyWaitReplyPort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtAlpcSendWaitReceivePort) //NtAlpcSendWaitReceivePort
		{
			if(bPrint) printf("Ignoring Syscall: %X NtAlpcSendWaitReceivePort (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtRemoveIoCompletion) //NtRemoveIoCompletion
		{
			if(bPrint) printf("Ignoring Syscall: %X NtRemoveIoCompletion (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		//disruptive calls
		else if(SysCall== macr_NtContinue) //NtContinue
		{
			if(bPrint) printf("Ignoring Syscall: %X NtContinue (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtContinueEx) //NtContinueEx
		{
			if(bPrint) printf("Ignoring Syscall: %X NtContinueEx (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtNotifyChangeKey) //NtNotifyChangeKey
		{
			if(bPrint) printf("Ignoring Syscall: %X NtNotifyChangeKey (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtNotifyChangeMultipleKeys) //NtNotifyChangeMultipleKeys
		{
			if(bPrint) printf("Ignoring Syscall: %X NtNotifyChangeMultipleKeys (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtSetContextThread) //NtSetContextThread
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSetContextThread (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtRaiseHardError) //NtRaiseHardError
		{
			if(bPrint) printf("Ignoring Syscall: %X NtRaiseHardError (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtQueueApcThreadEx) //NtQueueApcThreadEx
		{
			if(bPrint) printf("Ignoring Syscall: %X NtQueueApcThreadEx (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall == macr_NtSetDefaultLocale) //NtSetDefaultLocale
		{
			if(bPrint) printf("Ignoring Syscall: %X NtSetDefaultLocale (%s)\r\n",SysCall,SyscallName);
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
		char* SyscallName = NtosCalls[SysCall];

		if(SysCall== macr_NtClose) //NtClose
		{
			if(bPrint) printf("Deferring Syscall: %X NtClose, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtTerminateProcess) //NtTerminateProcess
		{
			if(bPrint) printf("Deferring Syscall: %X NtTerminateProcess, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtTerminateThread) //NtTerminateThread
		{
			if(bPrint) printf("Deferring Syscall: %X NtTerminateThread, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtTerminateJobObject) //NtTerminateJobObject
		{
			if(bPrint) printf("Deferring Syscall: %X NtTerminateJobObject, (%s)\r\n",SysCall,SyscallName);
			return true;
		}
		else if(SysCall== macr_NtRemoveIoCompletionEx) //NtRemoveIoCompletionEx
 		{
			if(bPrint) printf("Deferring Syscall: %X NtRemoveIoCompletionEx, (%s)\r\n",SysCall,SyscallName);
 			return true;
 		}
		else if(SysCall== macr_NtReleaseKeyedEvent) //NtReleaseKeyedEvent
 		{
			if(bPrint) printf("Deferring Syscall: %X NtReleaseKeyedEvent, (%s)\r\n",SysCall,SyscallName);
 			return true;
 		}
		else if(SysCall== macr_NtResetEvent) //NtResetEvent
 		{
			if(bPrint) printf("Deferring Syscall: %X NtResetEvent, (%s)\r\n",SysCall,SyscallName);
 			return true;
 		}
		else if(SysCall== macr_NtRollbackTransaction) //NtRollbackTransaction
 		{
			if(bPrint) printf("Deferring Syscall: %X NtRollbackTransaction, (%s)\r\n",SysCall,SyscallName);
 			return true;
 		}
		else if(SysCall== macr_NtUnlockFile) //NtUnlockFile
 		{
			if(bPrint) printf("Deferring Syscall: %X NtUnlockFile, (%s)\r\n",SysCall,SyscallName);
 			return true;
 		}
		else if(SysCall== macr_NtLockVirtualMemory) //NtLockVirtualMemory
 		{
			if(bPrint) printf("Deferring Syscall: %X NtLockVirtualMemory, (%s)\r\n",SysCall,SyscallName);
 			return true;
 		}
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