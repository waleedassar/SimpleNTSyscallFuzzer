#include "stdafx.h"
#include "Header.h"



//Fisher--Yates
ulong* GetRandomSyscallOrder_Swap(unsigned long Min,unsigned long Max)
{
	if(Min > Max) return 0;

	unsigned long Count = 0;
	ulong* pOrder = 0;
	if(Min == Max)
	{
		pOrder = (ulong*)VirtualAlloc(0,0x4,MEM_COMMIT,PAGE_READWRITE);
		if(!pOrder)
		{
			printf("Error allocating Random Order memory\r\n");
			return 0;
		}
		*pOrder = Min;
	}
	else
	{
		Count = (Max-Min)+1;
		pOrder = (ulong*)VirtualAlloc(0,Count*4,MEM_COMMIT,PAGE_READWRITE);
		if(!pOrder)
		{
			printf("Error allocating Random Order memory\r\n");
			return 0;
		}


		unsigned long X = Max + 1;
		unsigned long i = 0;

		unsigned long ii = Min;


		for(i=0;i<Count;i++)
		{
			pOrder[i] = ii;
			ii++;

			//printf("==> %X\r\n",pOrder[i]);
		}


		unsigned long Trials = 0xFFFF;
		while(Trials)
		{
			unsigned long R0 = rand()%Count;
			unsigned long R1 = rand()%Count;
			if(R0==R1) continue;
			else
			{
				unsigned long Temp = pOrder[R0];
				
				pOrder[R0] = pOrder[R1];

				pOrder[R1] = Temp;
				Trials--;
			}
		}

	}
	return pOrder;
}


//Slow, takes so much time
ulong* GetRandomSyscallOrder(unsigned long Min,unsigned long Max)
{
	if(Min > Max) return 0;



	unsigned long Count = 0;
	ulong* pOrder = 0;
	if(Min == Max)
	{
		pOrder = (ulong*)LocalAlloc(LMEM_ZEROINIT,4);
		if(!pOrder)
		{
			printf("Error allocating Random Order memory\r\n");
			return 0;
		}
		*pOrder = Min;
	}
	else
	{
		Count = (Max-Min)+1;
		pOrder = (ulong*)LocalAlloc(LMEM_ZEROINIT,Count*4);
		if(!pOrder)
		{
			printf("Error allocating Random Order memory\r\n");
			return 0;
		}

		ulong* pTemp = (ulong*)LocalAlloc(LMEM_ZEROINIT,Count*4);
		if(!pTemp)
		{
			printf("Error allocating Random Order memory\r\n");
			return 0;
		}

		unsigned long X = Max + 1;


		unsigned long i = 0;
		unsigned long ii = Min;

		for(i=0;	i<Count,ii<X;	i++,ii++)
		{
			pTemp[i] = ii;
			//printf("==> %X\r\n",pTemp[i]);
		}

		

		i = 0;
		ii = 0;
		unsigned long T = -1;


		while(i<Count)
		{
			while(T == -1)
			{
				ii = rand()%Count;
				//printf("==xxxxx===> %X\r\n",ii);
				T = pTemp[ii];
				printf("==xxxxx===> %X\r\n",T);

				//if(IsAllFFs(pTemp,Count)) break;
			}

			pOrder[i] = T;
			//printf("=====> %X\r\n",pOrder[i]);

			pTemp[ii] = T = -1;

			//if(IsAllFFs(pTemp,Count)) break;

			i++;
		}

		LocalFree(pTemp);
	}
	return pOrder;
}



void RandomizeOrderAndCounts(ulong* pOrder,ulong* pCounts,ulong Count)
{
	if(!pOrder || !pCounts || !Count) return;


		unsigned long Trials = 0xFFFF;
		while(Trials)
		{
			unsigned long R0 = rand()%Count;
			unsigned long R1 = rand()%Count;
			if(R0==R1) continue;
			else
			{
				unsigned long Temp = pOrder[R0];
				unsigned long Temp2 = pCounts[R0];
				
				pOrder[R0] = pOrder[R1];
				pOrder[R1] = Temp;



				pCounts[R0] = pCounts[R1];
				pCounts[R1] = Temp2;
				Trials--;
			}
		}
		return;
}
