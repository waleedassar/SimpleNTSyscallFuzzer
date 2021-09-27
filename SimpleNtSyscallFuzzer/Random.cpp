#include "stdafx.h"
#include "Header.h"

#include "intrin.h"

extern ulong bMode;//0=>TOCTOU, 1=> Memory Disclosure
//extern CRITICAL_SECTION CritSec;
extern longlong CS;


unsigned long Rand()
{
	return (unsigned long)(rand());
}





wchar_t* GetRandomWideString(wchar_t* pMem,ulong tLength)
{
	if(!pMem || !tLength) return 0;

	wchar_t* Printables = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	unsigned long i;
	for(i = 0; i < (tLength-1);i++)
	{
		pMem[i] = Printables[Rand()%95];
	}

	pMem[i]=0;

	return pMem;
}

char* GetRandomAsciiString(char* pMem,ulong tLength)
{
	if(!pMem || !tLength) return 0;

	char* Printables = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	unsigned long i;
	for(i = 0; i < (tLength-1);i++)
	{
		pMem[i] = Printables[Rand()%95];
	}

	pMem[i]=0;

	return pMem;
}



void FillRandomObjectAttributes(void* p,ulong Size)
{
	if(!p || !Size) return;

	if(Size < sizeof(_OBJECT_ATTRIBUTES)) return;

	((_OBJECT_ATTRIBUTES*)p)->Length = sizeof(_OBJECT_ATTRIBUTES);
	if(Rand()%5 == 1) ((_OBJECT_ATTRIBUTES*)p)->Length = GetRandomValue();



	ulong Atts[12] = {OBJ_PROTECT_CLOSE,OBJ_INHERIT,OBJ_AUDIT_OBJECT_CLOSE,OBJ_PERMANENT,OBJ_EXCLUSIVE,
		OBJ_CASE_INSENSITIVE,OBJ_OPENIF,OBJ_OPENLINK,OBJ_KERNEL_HANDLE,
		OBJ_FORCE_ACCESS_CHECK,OBJ_VALID_ATTRIBUTES,OBJ_CREATOR_INFO_MINE};

	if(Rand()%2 == 1)	((_OBJECT_ATTRIBUTES*)p)->Attributes = Atts[Rand()%12];
	else
	{
		ulong RR = Rand()%12;
		ulong Calc = 0;
		while(RR)
		{
			Calc |= Atts[Rand()%12];
			RR--;
		}
		((_OBJECT_ATTRIBUTES*)p)->Attributes = Calc;
	}

	((_OBJECT_ATTRIBUTES*)p)->ObjectName = (_UNICODE_STRING*)     (((unsigned char*)p)+sizeof(_OBJECT_ATTRIBUTES));

	FillRandomUnicodeString((((unsigned char*)p)+sizeof(_OBJECT_ATTRIBUTES)),Size-sizeof(_OBJECT_ATTRIBUTES));
}


void FillRandomUnicodeString(void* p,ulong Size)
{
	if(p)
	{
		if(Size >= sizeof(_UNICODE_STRING))
		{
			ulong bEmpty = Rand()%0x10;
			//printf("bEmpty: %X\r\n",bEmpty);

			ulong bCompletelyRandom = Rand()%0x10;
			//printf("bCompletelyRandom: %X\r\n",bCompletelyRandom);
			
			ulong bNonAlignedLengths = Rand()%5;
			//printf("bNonAlignedLengths: %X\r\n",bNonAlignedLengths);
			
			ulong bLenMaxAreSame = Rand()%5;
			//printf("bLenMaxAreSame: %X\r\n",bLenMaxAreSame);
			
			ulong bRandomPad = Rand()%2;
			//printf("bRandomPad: %X\r\n",bRandomPad);
			
			ulong NoBuffer = Rand()%0x10;
			//printf("NoBuffer: %X\r\n",NoBuffer);
			
			ulong bRandomBufferAddress = Rand()%5;
			//printf("bRandomBufferAddress: %X\r\n",bRandomBufferAddress);
			
			ulong bBufferAddressNotAligned = Rand()%3;
			//printf("bBufferAddressNotAligned: %X\r\n",bBufferAddressNotAligned);
			
			ulong bOnlyPrintables = Rand()%2;
			//printf("bOnlyPrintables: %X\r\n",bOnlyPrintables);
			
			ulong bNullTerm = Rand()%2;
			//printf("bNullTerm: %X\r\n",bNullTerm);
			//--------------------------------
			if(bEmpty == 1)//very less likely
			{
				memset(p,0,sizeof(_UNICODE_STRING));
				return;
			}
			
			if(bCompletelyRandom == 1) //very less likely
			{
				for(unsigned long c = 0; c < Size;c++)
				{
					((char*)p)[c]=Rand();
				}
				return;
			}
			//-------------------
			unsigned short RemSize = Size - sizeof(_UNICODE_STRING);
			if(RemSize == 0) return;
			if( RemSize % 2 ) RemSize -= 1;//Align
			//-------------------
			unsigned short ChosenSize = GetRandomValue() % RemSize;
			if( ChosenSize % 2 ) ChosenSize -= 1;//Align

			//printf("ChosenSize: %X\r\n",ChosenSize);
			//----------------------

			
			ulong Length;
			ulong MaxLength;
			if(bNonAlignedLengths == 1)
			{
				Length = (ChosenSize-2)+1;
				MaxLength = (ChosenSize)+1;
			}
			else
			{
				Length = (ChosenSize-2);
				MaxLength = (ChosenSize);
			}
			((_UNICODE_STRING*)p)->Length = Length;
			((_UNICODE_STRING*)p)->MaxLength = MaxLength;

			
			if(bLenMaxAreSame == 1)
			{
				((_UNICODE_STRING*)p)->MaxLength = ((_UNICODE_STRING*)p)->Length;
			}


			
			if(bRandomPad)
			{
				((_UNICODE_STRING*)p)->Pad = GetRandomValue();
			}
			else
			{
				((_UNICODE_STRING*)p)->Pad = 0;
			}

			if(NoBuffer == 1)
			{
				((_UNICODE_STRING*)p)->Buffer =0;
				return;
			}

			
			if(bRandomBufferAddress==1)//Less likely
			{
				((_UNICODE_STRING*)p)->Buffer = (wchar_t*)GetRandomValue();
			}
			else
			{
				wchar_t* wBuffer = (wchar_t*)(((unsigned char*)p)+sizeof(_UNICODE_STRING));

				((_UNICODE_STRING*)p)->Buffer = wBuffer;

				

				if(!bOnlyPrintables)
				{
					for(unsigned long ii = 0; ii < (ChosenSize/2);ii++)
					{
						wBuffer[ii] = Rand();
					}
				}
				else
				{
					wchar_t* Printables = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
					//wchar_t* Printables = L"ABCD";
					for(unsigned long i = 0; i < (ChosenSize/2);i++)
					{
						wBuffer[i] = Printables[Rand()%95];
						//wBuffer[i] = Printables[Rand()%4];
					}
				}

				if(bNullTerm)
				{
					wBuffer[ (ChosenSize-2)/2]=0;
				}
			}

			char* Buffer = (char*)(((_UNICODE_STRING*)p)->Buffer);


			
			if(bBufferAddressNotAligned==1)
			{
				Buffer += Rand()%0x8;
				((_UNICODE_STRING*)p)->Buffer  = (wchar_t*)Buffer;

			}


			
		}
	}
}

ulonglong GetRandomValue()
{
	ulonglong R = __rdtsc();
	R &= 0xFFFFFFFF;
	//wprintf(L"R: %X\r\n",R);

	
	ulong Y = rand() + __rdtsc();

	//ulong Z = rand();
	ulong Z = rand()%0x20;
	while(!Z) Z=rand()%0x20;

	ulong Count = __rdtsc() % Z;
	while(Count)
	{
		//Y += rand();

		Y += __rdtsc();
		
		Count--;
	}


	
	//wprintf(L"Y: %X\r\n",Y);
	

	ulonglong Final = Y;

	Final <<= 32;
	Final |= R;

	//wprintf(L"Final: %I64X\r\n",Final);
	return Final;
}


ulonglong GetClassicRandomValue()
{
	
	
	ulonglong RanX = GetRandomValue();
	ulong ClassX = Rand()%16;


	if(ClassX==0)	    RanX  = RanX%0x10;
	else if(ClassX==1)  RanX  = RanX%0x100;
	else if(ClassX==2)  RanX  = RanX%0x1000;
	else if(ClassX==3)  RanX  = RanX%0x10000;
	else if(ClassX==4)  RanX  = RanX%0x100000;
	else if(ClassX==5)  RanX  = RanX%0x1000000;
	else if(ClassX==6)  RanX  = RanX%0x10000000;
	else if(ClassX==7)  RanX  = RanX%0x100000000;
	else if(ClassX==8)  RanX  = RanX%0x1000000000;
	else if(ClassX==9)  RanX  = RanX%0x10000000000;
	else if(ClassX==10) RanX  = RanX%0x100000000000;
	else if(ClassX==11) RanX  = RanX%0x1000000000000;
	else if(ClassX==12) RanX  = RanX%0x10000000000000;
	else if(ClassX==13) RanX  = RanX%0x100000000000000;
	else if(ClassX==14) RanX  = RanX%0x1000000000000000;
	else if(ClassX==15)
	{
		ulong RZ = Rand()%3;
		if(RZ==0) RanX = 0;
		else if(RZ==1) RanX = -1;
		//else random
	}


	return RanX;
}

bool HasFF(ulonglong Data)
{
	ulonglong DataX = Data;
	unsigned char* pDataX = (unsigned char*)(&DataX);

	for(ulong i=0;i<8;i++)
	{
		if( *(pDataX+i) == 0xFF)
		{
			return true;
		}
	}


	return false;
}



void FillRandomData(void* pMem,ulong Size)
{
	if(pMem && Size)
	{
		ulong SizeX = Size/8;//Num of qwords

		ulong Rem = Size % 8;//fraction


		ulonglong* pRunner = (ulonglong*)pMem;
		while(SizeX)
		{
				//the following 5 lines are only for disclosure detection, remove later and uncomment next line
				ulonglong RX = GetRandomValue();
				while(HasFF(RX))
				{
					RX = GetRandomValue();
				}
				*pRunner = RX;

			//*pRunner = GetRandomValue();


			pRunner++;

			SizeX-= 1;
		}

		unsigned char* pRunnerX = (unsigned char*) pRunner;
		while(Rem)
		{
			*pRunnerX = (unsigned char)GetRandomValue();
			pRunnerX++;
			Rem--;
		}
	}
}


void FillClassicRandomData(void* pMem,ulong Size,bool bMode)
{
	if(pMem && Size)
	{
		ulong SizeX = Size/8;//Num of qwords

		ulong Rem = Size % 8;//fraction


		ulong ClassX = 0;
		ulonglong* pRunner = (ulonglong*)pMem;
		while(SizeX)
		{
			ulonglong RanX = GetRandomValue();
			ClassX = Rand()%18;
			if(ClassX==0)	    *pRunner  = RanX%0x10;
			else if(ClassX==1)  *pRunner  = RanX%0x100;
			else if(ClassX==2)  *pRunner  = RanX%0x1000;
			else if(ClassX==3)  *pRunner  = RanX%0x10000;
			else if(ClassX==4)  *pRunner  = RanX%0x100000;
			else if(ClassX==5)  *pRunner  = RanX%0x1000000;
			else if(ClassX==6)  *pRunner  = RanX%0x10000000;
			else if(ClassX==7)  *pRunner  = RanX%0x100000000;
			else if(ClassX==8)  *pRunner  = RanX%0x1000000000;
			else if(ClassX==9)  *pRunner  = RanX%0x10000000000;
			else if(ClassX==10) *pRunner  = RanX%0x100000000000;
			else if(ClassX==11) *pRunner  = RanX%0x1000000000000;
			else if(ClassX==12) *pRunner  = RanX%0x10000000000000;
			else if(ClassX==13) *pRunner  = RanX%0x100000000000000;
			else if(ClassX==14) *pRunner  = RanX%0x1000000000000000;
			else if(ClassX==15) *pRunner  = RanX;
			else if(ClassX==16) *pRunner  = (RanX | 0xffff000000000000);
			else if(ClassX==17)
			{
				ulong RZ = Rand()%3;
				if(RZ==0) *pRunner = 0;
				else if(RZ==1) *pRunner = -1;
				//else random
			}

			/*
			if(bMode == true)
			{
				//the following 5 lines are only for disclosure detection, remove later and uncomment next line
				ulonglong RX = GetRandomValue();
				while(HasFF(RX))
				{
					RX = GetRandomValue();
				}
				*pRunner = RX;
			}
			else
			{
				*pRunner = GetRandomValue();
			}
			*/

			pRunner++;

			SizeX-= 1;
		}

		unsigned char* pRunnerX = (unsigned char*) pRunner;
		while(Rem)
		{
			*pRunnerX = (unsigned char)GetRandomValue();
			pRunnerX++;
			Rem--;
		}
	}
}

bool IsAllFFs(ulong* pMem,unsigned long Count)
{
	unsigned long i = 0;
	while(i < Count)
	{
		if(pMem[i] != -1) return false;

		i++;
	}
	return true;
}

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

void* GetRandomPage()
{
		ulong SizeX = RANDOM_PAGE_SIZE;
		ulonglong* pNew = (ulonglong*)VirtualAlloc(0,SizeX,MEM_COMMIT,PAGE_READWRITE);
		if(!pNew)
		{
			printf("Insufficient resources\r\n");
			ExitProcess(-2);
		}
		FillClassicRandomData(pNew,SizeX,0);
		//*(((char*)pNew)+SizeX-0x8) = 0x8E9EAEBECEDEEEFE;//Magic Value

		return pNew;
}

void DestroyRandomPage(void* pNew)
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


//win32k
void RandomizeTebStuff()
{
	char* Teb = (char*)__readgsqword(0x30);
	if(!Teb) return;

	//FIx TEB GdiBatchCount here, fix it to zero or not
	//if ( Rand()%2 == 1) *(ulong*)(Teb+0x1740)=0;
	//else *(ulong*)(Teb+0x1740)=GetRandomValue();
					
	//_GDI_TEB_BATCH64* pGdiTebBatch = (_GDI_TEB_BATCH64*)(Teb+0x2F0);
	//if( Rand()% 2 == 1) FillRandomData(pGdiTebBatch,sizeof(_GDI_TEB_BATCH64));

	//if( Rand()%3 == 1) pGdiTebBatch->Offset=0;
	
	//if( Rand()%3 == 1) pGdiTebBatch->hDc = AllDCs[Rand()%AllDCsUsed];

	//if( Rand()%3 == 1) *(ulonglong*)( ((char*)Teb) + 0x78) = 0;//Win32ThreadInfo

	//char* Win32ClientInfo = Teb+0x800;
	//if( Rand()%2 == 1) FillRandomData(Win32ClientInfo,0x1F0);

	return;
}




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
				FillRandomData(pPool[i],RANDOM_PAGE_SIZE-0x8);//room for magic trail
			}
		}




		if(pPoolX && CountX)
		{
			for(i=0;i<CountX;i++)
			{
				if(pPoolX[i])
				{
					FillRandomData(pPoolX[i],RANDOM_PAGE_SIZE-0x8);//room for magic trail
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