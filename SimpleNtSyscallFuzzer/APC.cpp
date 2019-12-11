#include "stdafx.h"
#include "Header.h"



void DummyAPCRoutine(ulonglong Dum)
{
	return;
}


void Papcfunc(ULONG_PTR hEventX)
{
	HANDLE hEvent = (HANDLE)hEventX;

	//Assuming hEvent is already non-signaled
	
	
	//Do stuff


	if(hEvent)
	{
		if(!SetEvent(hEvent))
		{
			printf("Error SetEvent, APC routine: %X\r\n",GetLastError());
			ExitProcess(0);//remove later
		}
	}
}



void ApcLoopThread(APC_LOOP_THREAD* pApcLoopThread)
{
	if( (!pApcLoopThread) || (!pApcLoopThread->ThreadId) || (!pApcLoopThread->hEvent) || (pApcLoopThread->hEvent == INVALID_HANDLE_VALUE) )
	{
		printf("Error in APC Loop thread, plz fix\r\n");
		ExitProcess(0);
	}

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,pApcLoopThread->ThreadId);
	if(!hThread)
	{
		printf("Can't open target thread, plz fix\r\n");
		ExitProcess(0);
	}


	//Assuming hEvent is already non-signaled

	if(!QueueUserAPC(Papcfunc,hThread,(ULONG_PTR)(pApcLoopThread->hEvent)))
	{
		printf("Error, QueueUserAPC, :%X\r\n",GetLastError());
		ExitProcess(0);//remove later
	}

	while(1)
	{
		WaitForSingleObject(pApcLoopThread->hEvent,INFINITE);
		if(!QueueUserAPC(Papcfunc,hThread,(ULONG_PTR)(pApcLoopThread->hEvent)))
		{
			printf("Error, QueueUserAPC, :%X\r\n",GetLastError());
			ExitProcess(0);//remove later
		}

		ResetEvent(pApcLoopThread->hEvent);
	}
	return;
}