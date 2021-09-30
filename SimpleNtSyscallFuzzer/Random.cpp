#include "stdafx.h"
#include "Header.h"
#include "intrin.h"




unsigned long Rand()
{
	return (unsigned long)(rand());
}



wchar_t* GetRandomWideNewLines(wchar_t* pMem,ulong tLength)
{
	if(!pMem || !tLength) return 0;

	wchar_t* Printables = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	wchar_t* NewLines = L"\r\n";

	unsigned long i;
	for(i = 0; i < (tLength-1);i++)
	{
		if( Rand()%0x10 == 1) pMem[i] = NewLines[Rand()%2];
		else 				  pMem[i] = Printables[Rand()%95];
	}

	pMem[i]=0;

	return pMem;
}

wchar_t* GetRandomWideFormatString(wchar_t* pMem,ulong tLength)
{
	if(!pMem || !tLength) return 0;

	wchar_t* Printables = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	unsigned long i;
	for(i = 0; i < (tLength-1);i++)
	{
		if( Rand()%0x10 == 1) pMem[i] = L'%';
		else pMem[i] = Printables[Rand()%95];
	}
	pMem[i]=0;

	return pMem;
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

char* GetRandomAsciiNewLines(char* pMem,ulong tLength)
{
	if(!pMem || !tLength) return 0;

	char* Printables = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	char* NewLines = "\r\n";

	unsigned long i;
	for(i = 0; i < (tLength-1);i++)
	{
		if( Rand()%0x10 == 1) pMem[i] = NewLines[Rand()%2];
		else pMem[i] = Printables[Rand()%95];
	}

	pMem[i]=0;

	return pMem;
}


char* GetRandomAsciiFormatString(char* pMem,ulong tLength)
{
	if(!pMem || !tLength) return 0;

	char* Printables = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	unsigned long i;
	for(i = 0; i < (tLength-1);i++)
	{
		if( Rand()%0x10 == 1) pMem[i] = '%';
		else pMem[i] = Printables[Rand()%95];
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
			*pRunner = GetRandomValue();


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

void FillRandomData(void* pMem,ulong Size,bool bMode)
{
	if(pMem && Size)
	{
		ulong SizeX = Size/8;//Num of qwords

		ulong Rem = Size % 8;//fraction


		ulonglong* pRunner = (ulonglong*)pMem;
		while(SizeX)
		{
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






ulong GetRandomSectionAllocationAttribute(uchar PreviousMode)
{
	bool bInvalid = true;
	ulong Alloc = SEC_IMAGE;
	ulong Disallowed[15]={
		0x80,
		0x100,
		0x200,
		0x400,
		0x800,
		0x1000,
		0x2000,
		0x4000,
		0x8000,
		0x10000,
		0x20000,
		0x40000,
		0x80000,
		0x800000,
		0x20000000};


	while(bInvalid == true)
	{
		Alloc = GetClassicRandomValue();


		//Remove disallowed attributes
		for(ulong i=0;i<15;i++)
		{
			
			if(Alloc & Disallowed[i])
			{
				Alloc &= (~Disallowed[i]);
			}
		}

		if(PreviousMode == UserMode)
		{
			if((Alloc & SEC_UNK) || (Alloc & SEC_PROTECTED_IMAGE))
			{
				Alloc &= (~SEC_UNK);
				Alloc &= (~SEC_PROTECTED_IMAGE);
			}
		}






		//Remove disallowed combinations
		if( (Alloc & SEC_IMAGE) && (Alloc & SEC_PROTECTED_IMAGE) )
		{
			Alloc &= (SEC_PROTECTED_IMAGE);
		}

		if( (Alloc & SEC_IMAGE) && (Alloc & SEC_UNK) )
		{
			Alloc &= (SEC_UNK);
		}

		if( (Alloc & SEC_NEVER_RELOCATE) && (Alloc & SEC_UNK2))
		{
			if( Rand()%2 == 0)
			{
				Alloc &= (SEC_NEVER_RELOCATE);
			}
			else
			{
				Alloc &= (SEC_UNK2);
			}
		}


		if( (Alloc & SEC_UNK) ||
			(Alloc & SEC_IMAGE) ||
			(Alloc & SEC_PROTECTED_IMAGE) )
		{
			ulong disallowedX = 0xDC400000;
			if(Alloc & SEC_UNK)
			{
				disallowedX = 0xDC000000;
			}
	
			if(Alloc & disallowedX)
			{
				bInvalid = true;
				continue;
			}
		}

		//Add minimum
		if( (Alloc & SEC_UNK == 0) &&
			(Alloc & SEC_IMAGE == 0) &&
			(Alloc & SEC_PROTECTED_IMAGE == 0) &&
			(Alloc & SEC_UNK2 == 0) &&
			(Alloc & SEC_NEVER_RELOCATE == 0) )
		{
			Alloc |= SEC_IMAGE;
		}



		bInvalid = false;
	}
	return Alloc;
}


ulonglong GetRandomDesiredAccess()
{
	ulong Desireds[4]={GENERIC_READ,GENERIC_WRITE,GENERIC_EXECUTE,GENERIC_ALL};
	
	ulonglong Final = 0;

	ulong R = Rand()%3;
	if(R==0)
	{
		Final = 0;
	}
	else if(R==1)
	{
		Final =  Desireds[Rand()%4];
	}
	else if(R==2)
	{
		ulong RR = Rand()%(4+1);
		ulong CalcX = 0;
		while(RR)
		{
			CalcX |= Desireds[Rand()%4];
			RR--;
		}
		Final = CalcX;
	}
	return Final;
}





void FillRandomPortMessage(void* pInfo,ulong InfoSize,ulonglong ThreadId,ulonglong ProcessId)
{
	if(pInfo && InfoSize)
	{
		//FillRandomData(pInfo,InfoSize,0);
		FillClassicRandomData(pInfo,InfoSize,0);

		ulong UsableSize = InfoSize - sizeof(_PORT_MESSAGE);

		_PORT_MESSAGE* pPort = (_PORT_MESSAGE*)pInfo;

		pPort->Type = 1 + Rand()%10;

		ulong DataLength = (Rand()%(UsableSize+1));

		pPort->DataLength = DataLength;

		ulong TotalLength =  DataLength + sizeof(_PORT_MESSAGE);
		pPort->TotalLength = TotalLength;

		if(ThreadId)	pPort->ClientId.UniqueThread = ThreadId;
		if(ProcessId)   pPort->ClientId.UniqueProcess = ProcessId;


	}
}



void FillRandomPortMessage32(void* pInfo,ulong InfoSize,ulong ThreadId,ulong ProcessId)
{
	if(pInfo && InfoSize)
	{
		//FillRandomData(pInfo,InfoSize,0);
		FillClassicRandomData(pInfo,InfoSize,0);

		ulong UsableSize = InfoSize - sizeof(_PORT_MESSAGE32);

		_PORT_MESSAGE32* pPort = (_PORT_MESSAGE32*)pInfo;

		pPort->Type = (1 + Rand()%10) | 0x1000;

		ulong DataLength = (Rand()%(UsableSize+1));

		pPort->DataLength = DataLength;

		ulong TotalLength =  DataLength + sizeof(_PORT_MESSAGE);
		pPort->TotalLength = TotalLength;

		if(ThreadId)	pPort->ClientId.UniqueThread = ThreadId;
		if(ProcessId)   pPort->ClientId.UniqueProcess = ProcessId;
	}
}



ulonglong GetValidUserModeAllocatedAddress(HANDLE hProcess)
{

	ulong Rounds = GetClassicRandomValue()%0x100;

	ulonglong ChosenAddress = 0;

	ulonglong Min = 0;
	ulonglong Max = 0x7FFFFFFF0000;//win10

	ulonglong i = Min;

	while(i <= Max)
	{
			MEMORY_BASIC_INFORMATION MBI={0};

			if(VirtualQueryEx(hProcess,(void*)i,&MBI,sizeof(MBI)))
			{
				if( (MBI.State == MEM_RESERVE) || (MBI.State == MEM_COMMIT))
				{
					ChosenAddress = i;
					Rounds--;
					if(Rounds == 0) return ChosenAddress;

					i+= (MBI.RegionSize);
					continue;
				}
				else
				{
					i+= (MBI.RegionSize);
					continue;
				}
			}
			i+= 0x1000;//next Page
	}
	return ChosenAddress;
}

ulonglong GetValidUserModeReservedAddress(HANDLE hProcess)
{

	ulong Rounds = GetClassicRandomValue()%0x100;

	ulonglong ChosenAddress = 0;

	ulonglong Min = 0;
	ulonglong Max = 0x7FFFFFFF0000;//win10

	ulonglong i = Min;

	while(i <= Max)
	{
			MEMORY_BASIC_INFORMATION MBI={0};

			if(VirtualQueryEx(hProcess,(void*)i,&MBI,sizeof(MBI)))
			{
				if(MBI.State == MEM_RESERVE)
				{
					ChosenAddress = i;
					Rounds--;
					if(Rounds == 0) return ChosenAddress;


					i+= (MBI.RegionSize);
					continue;
				}
				else
				{
					i+= (MBI.RegionSize);
					continue;
				}
			}
			i+= 0x1000;//next Page
	}
	return ChosenAddress;
}


ulonglong GetValidUserModeFreeAddress(HANDLE hProcess)
{

	ulong Rounds = GetClassicRandomValue()%0x100;

	ulonglong ChosenAddress = 0;

	ulonglong Min = 0;
	ulonglong Max = 0x7FFFFFFF0000;//win10

	ulonglong i = Min;

	while(i <= Max)
	{
			MEMORY_BASIC_INFORMATION MBI={0};

			if(VirtualQueryEx(hProcess,(void*)i,&MBI,sizeof(MBI)))
			{
				if(MBI.State == MEM_FREE)
				{
					ChosenAddress = i;
					Rounds--;
					if(Rounds == 0) return ChosenAddress;


					i+= (MBI.RegionSize);
					continue;
				}
				else
				{
					i+= (MBI.RegionSize);
					continue;
				}
			}
			i+= 0x1000;//next Page
	}
	return ChosenAddress;
}


ulonglong GetValidUserModeAddress()
{
	return GetClassicRandomValue()%(0x7FFFFFFF0000 + 1);
}

ulonglong GetValidUserModeAddress32()
{
	return GetClassicRandomValue()%(0x7FFFFFFF+1);
}


ulonglong GetValidKernelModeAddress()
{
	return GetClassicRandomValue()|0xfffff00000000000;
}





void FillRandomSecurityDescriptor(void* pSecurityDescriptor,ulonglong InputSize)
{

	if( (!pSecurityDescriptor) || (!InputSize) ) return;

	FillClassicRandomData(pSecurityDescriptor,InputSize,0);

	ulonglong SecDescrX = (ulonglong)pSecurityDescriptor;
	ulonglong AfterSecDescrX = SecDescrX + InputSize;


		
		



	((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->Revision = 1;

	if(Rand()%2 == 1)
	{
		((_SECURITY_DESCRIPTOR*)SecDescrX)->Control |= 0x10;//Sacl Present
	}

	if(Rand()%2 == 1)
	{
		((_SECURITY_DESCRIPTOR*)SecDescrX)->Control |= 0x4;//Dacl Present
	}

	if( Rand()%3 == 1)
	{
		((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->Control |= 0x8000;

		ulong OffsetX = 0;

		if( Rand()%5 == 1) ((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetOwner = 0;
		else
		{
			OffsetX = (GetClassicRandomValue()%(InputSize+1)) & (~4);
			((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetOwner = OffsetX;
		}

		if( Rand()%5 == 1) ((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetGroup = 0;
		else
		{
			OffsetX = (GetClassicRandomValue()%(InputSize+1)) & (~4);
			((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetGroup = OffsetX;
		}


		_ACL* pACL = 0;

		if( Rand()%5 == 1) ((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetSacl = 0;
		else
		{
			OffsetX = (GetClassicRandomValue()%(InputSize+1)) & (~4);
			((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetSacl = OffsetX;

			pACL = (_ACL*)( ((ulonglong)SecDescrX) +OffsetX);

			//printf("OffsetX: %X\r\n",OffsetX);
			if( Rand()%0x10 == 1) pACL->AclSize = 0;
			else 
			{
				pACL->AclSize = 0x8 + (GetClassicRandomValue()%(InputSize-8+1));
			}
			pACL->AclRevision = 2 + Rand()%3;
		}


		if( Rand()%5 == 1) ((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetDacl = 0;
		else
		{
			OffsetX = (GetClassicRandomValue()%(InputSize+1)) & (~4);
			((_MY_SECURITY_DESCRIPTOR*)SecDescrX)->OffsetDacl = OffsetX;

			pACL = (_ACL*)( ((ulonglong)SecDescrX) +OffsetX);

			//printf("OffsetX: %X\r\n",OffsetX);
			if( Rand()%0x10 == 1) pACL->AclSize = 0;
			else 
			{
				pACL->AclSize = 0x8 + (GetClassicRandomValue()%(InputSize-8+1));
			}
			pACL->AclRevision = 2 + Rand()%3;
		}
	}
	else
	{
		((_SECURITY_DESCRIPTOR*)SecDescrX)->Control &= (~0x8000);

		if( Rand()%5 == 1)
		{
			((_SECURITY_DESCRIPTOR*)SecDescrX)->Owner = 0;
		}
		else
		{
			ulonglong pInXX2 = (SecDescrX + (Rand()%InputSize));

			((_SECURITY_DESCRIPTOR*)SecDescrX)->Owner = (_SID*)(pInXX2);


			if(Rand()%0x10 == 1) ((_SID*)pInXX2)->SubAuthorityCount = GetClassicRandomValue();
			else 				((_SID*)pInXX2)->SubAuthorityCount = GetClassicRandomValue()%(0x3FFFFFF7 + 1);
		}

		if( Rand()%5 == 1)
		{
			((_SECURITY_DESCRIPTOR*)SecDescrX)->Group = 0;
		}
		else
		{
			ulonglong pInXX3 = (SecDescrX + (Rand()%InputSize));
			((_SECURITY_DESCRIPTOR*)SecDescrX)->Group = (_SID*)pInXX3;


			if(Rand()%0x10 == 1) ((_SID*)pInXX3)->SubAuthorityCount = GetClassicRandomValue();
			else 				 ((_SID*)pInXX3)->SubAuthorityCount = GetClassicRandomValue()%(0x3FFFFFF7 + 1);
		}

		if( Rand()%5 == 1)
		{
			((_SECURITY_DESCRIPTOR*)SecDescrX)->Dacl  = 0;
		}
		else
		{
			ulonglong pInXX4 = (SecDescrX + (Rand()%InputSize));
			((_SECURITY_DESCRIPTOR*)SecDescrX)->Dacl  = (_ACL*)pInXX4;

			if( Rand()%0x10 == 1) ((_ACL*)pInXX4)->AclSize = 0;
			else ((_ACL*)pInXX4)->AclSize = 0x8 + (GetClassicRandomValue()%(InputSize+1));

			((_ACL*)pInXX4)->AclRevision = 2 + Rand()%3;
		}

		if( Rand()%5 == 1)
		{
			((_SECURITY_DESCRIPTOR*)SecDescrX)->Sacl = 0;
		}
		else
		{
			ulonglong pInXX5 = (SecDescrX + (Rand()%InputSize));
			((_SECURITY_DESCRIPTOR*)SecDescrX)->Sacl  = (_ACL*)pInXX5;

			if( Rand()%0x10 == 1) ((_ACL*)pInXX5)->AclSize = 0;
			else ((_ACL*)pInXX5)->AclSize = 0x8 + (GetClassicRandomValue()%(InputSize+1));

			((_ACL*)pInXX5)->AclRevision = 2 + Rand()%3;
		}
	}
	return;
}


void FillRandomSid(void* pSid,ulonglong InputSize)
{
	if( (!pSid) || (!InputSize) ) return;

	FillClassicRandomData(pSid,InputSize,0);

	_SID* pSidX = (_SID*)pSid;


	if(InputSize >= 2)
	{
		pSidX->Revision = 1;
		pSidX->SubAuthorityCount = GetClassicRandomValue()%(0xF + 0x1);
	}
}


ulong GetRandomNTStatusCode()
{
	ulong Bases[2] = {0xC0000000,0x80000000};

	return Bases[Rand()%2] | ( Rand()%0x10000);
}