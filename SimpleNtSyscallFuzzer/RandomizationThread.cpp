#include "stdafx.h"
#include "Header.h"

extern ulong bMode;//0=>TOCTOU, 1=> Memory Disclosure
//extern CRITICAL_SECTION CritSec;
extern longlong CS;

void RandomiztionThread(RANDOMIZATION_THREAD* pRandThr)
{
	if(!pRandThr) return;

	unsigned long Count = pRandThr->Count;
	void** pPool = pRandThr->pPool;

	if(!Count  || !pPool) return;

	unsigned long	CountX = pRandThr->SecondLevelCount;
	void** pPoolX = pRandThr->pSecondLevelPool;

	while(1)
	{
		if(bMode)
		{
			ulonglong retX;
			do
			{
				retX = InterlockedCompareExchange64(&CS,1,0);
			}while(retX);
		}

		//EnterCriticalSection(&CritSec);
		//WaitForSingleObject(hMutex,INFINITE);

		//ulonglong rdtscX = __rdtsc();

		
		unsigned long i=0;
		for(i=0;i<Count;i++)
		{
			if(pPool[i])
			{
				FillClassicRandomData(pPool[i],RANDOM_PAGE_SIZE-0x8,bMode);//room for magic trail
			}
		}




		if(pPoolX && CountX)
		{
			for(i=0;i<CountX;i++)
			{
				if(pPoolX[i])
				{
					FillClassicRandomData(pPoolX[i],RANDOM_PAGE_SIZE-0x8,bMode);//room for magic trail
				}
			}
		}

		
		//printf("Diff: %I64X\r\n",__rdtsc() - rdtscX);
		//getchar();

		//ReleaseMutex(hMutex);
		//LeaveCriticalSection(&CritSec);

		if(bMode)
		{
			InterlockedDecrement64(&CS);
		}

		//ZwDelayExecution(FALSE,0);//dummy syscall
		//Sleep(3);

		/*
		ulong n = 0x1000;
		while(n)
		{
			__nop();
			n--;
		}
		*/
		
	}

}