#include "stdafx.h"
#include "windows.h"
#include "Header.h"




//Later add functions for special pool session/global


ulong DefaultAlignment = DEFAULT_ALIGNMENT;

//Indicators: nt!MiSessionDynamicVaStart, Size: 0x2000000000, function: nt!MI_VA_IN_SESSION_SPECIAL_POOL
bool ScanSessionPool(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic_S = 0xFFFFF900;
	ulong Magic_E = 0xfffff920;

	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if( (( *(ulong*)( ((unsigned char*)pMem)+i) >= Magic_S) ) && (( *(ulong*)( ((unsigned char*)pMem)+i) <= Magic_E) ) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )		pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}

bool ScanSessionPool_10(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic_S = 0xfffff900;//not so sure
	ulong Magic_E = 0xfffff910;//not so sure

	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if( (( *(ulong*)( ((unsigned char*)pMem)+i) >= Magic_S) ) && (( *(ulong*)( ((unsigned char*)pMem)+i) < Magic_E) ) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )		pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}


//------------------------------------------------------------

//global
//Indicators: nt!MmPagedPoolEnd, nt!MmSizeOfPagedPoolInBytes
bool ScanPagedPoolRanges_g(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic_S = 0xfffff8a0;
	ulong Magic_E = 0xfffff8bf;

	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if( (( *(ulong*)( ((unsigned char*)pMem)+i) >= Magic_S) ) && (( *(ulong*)( ((unsigned char*)pMem)+i) <= Magic_E) ) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )		pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}


bool ScanPagedPoolRanges_g_10(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic_S = 0xFFFFC000;
	ulong Magic_E = 0xffffcf80;

	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if( (( *(ulong*)( ((unsigned char*)pMem)+i) >= Magic_S) ) && (( *(ulong*)( ((unsigned char*)pMem)+i) < Magic_E) ) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )		pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}





//Indicators: nt!MmNonPagedPoolStart, nt!MmSizeOfNonPagedPoolInBytes
bool ScanNonPagedPoolRanges_g(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic = 0xfffffa80;


	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if(( *(ulong*)( ((unsigned char*)pMem)+i) == Magic) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )	pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}

bool ScanNonPagedPoolRanges_g_10(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic = 0xffffe000;


	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if(( *(ulong*)( ((unsigned char*)pMem)+i) == Magic) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )	pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}

bool ScanStackRanges(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic =  0xFFFFF680; 
	ulong MagicX = 0xfffff880;


	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if( (( *(ulong*)( ((unsigned char*)pMem)+i) == Magic) ) || (( *(ulong*)( ((unsigned char*)pMem)+i) == MagicX) ) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )	pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}

bool ScanStackRanges_10(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic = 0xffffaa8f;


	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if(( *(ulong*)( ((unsigned char*)pMem)+i) == Magic) )
		{
			pOffsets[*pLastIndex] = i;

			if( i >= 4 )	pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			else					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}

//Indicators: PsLoadedModuleList
bool ScanModuleRanges(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic_S = 0xfffff880;
	ulong Magic_E = 0xfffff960;//or more

	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if( (( *(ulong*)( ((unsigned char*)pMem)+i) >= Magic_S) ) && (( *(ulong*)( ((unsigned char*)pMem)+i) <= Magic_E) ) )
		{

			pOffsets[*pLastIndex] = i;

			//if( i - 4 >= 0 )
			if( i >= 4 )
			{
					pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			}
			else
			{
					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);
			}


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}


//both global and session
bool ScanModuleRanges_10(void* pMem,ulong Size,ulong* pOffsets,ulonglong* pValues,ulong* pLastIndex)
{
	ulong Magic_S = 0xFFFFF800;
	ulong Magic_E = 0xfffff880;//or more

	ulong Magic_S_S = 0xFFFFF960;
	ulong Magic_S_E = 0xfffff980;

	if(!pOffsets || !pValues || !pLastIndex) return false;

	if((!pMem) || (Size < 4) )	return false;


	bool bFound = false;

	for(ulong i = 0; i <= (Size-4); i+=DefaultAlignment)
	{
		if(  ( (( *(ulong*)( ((unsigned char*)pMem)+i) >= Magic_S) ) && (( *(ulong*)( ((unsigned char*)pMem)+i) < Magic_E) ) ) ||
			( (( *(ulong*)( ((unsigned char*)pMem)+i) >= Magic_S_S) ) && (( *(ulong*)( ((unsigned char*)pMem)+i) < Magic_S_E) ) ) )
		{

			pOffsets[*pLastIndex] = i;

			//if( i - 4 >= 0 )
			if( i >= 4 )
			{
					pValues[*pLastIndex] = *(ulonglong*)( ((unsigned char*)pMem)+i-4);
			}
			else
			{
					pValues[*pLastIndex] = *(ulong*)( ((unsigned char*)pMem)+i);
			}


			(*pLastIndex)++;

			bFound = true;
		}
	}
	if(bFound) return true;
	return false;
}





//OSVer 0 Windows 7
//OSVer 1 Windows 10
bool Scan(void* pMem,ulong Size,ulong OSVer)
{
	if(OSVer!=0 && OSVer!=1) return false;


	ulong LastIndex = 0;


	ulong MaxSizeX = Size-4;
	ulong* pOffsets = (ulong*)VirtualAlloc(0,MaxSizeX,MEM_COMMIT,PAGE_READWRITE);
	if(!pOffsets)
	{
		printf("Error allocating %X memory for offsets of leaked addresses\r\n",MaxSizeX);
		return false;
	}

	ulong MaxSizeY = Size-8;
	ulonglong* pValues = (ulonglong*)VirtualAlloc(0,MaxSizeY,MEM_COMMIT,PAGE_READWRITE);
	if(!pValues)
	{
		printf("Error allocating %X memory for leaked addresses\r\n",MaxSizeY);
		return false;
	}



	bool bRet0;
	if(OSVer == 0 )	bRet0 = ScanModuleRanges(pMem,Size,pOffsets,pValues,&LastIndex);
	else if(OSVer == 1) bRet0 = ScanModuleRanges_10(pMem,Size,pOffsets,pValues,&LastIndex);


	bool bRet1;
	if(OSVer == 0 )	bRet1 = ScanNonPagedPoolRanges_g(pMem,Size,pOffsets,pValues,&LastIndex);
	else if(OSVer == 1)	bRet1 = ScanNonPagedPoolRanges_g_10(pMem,Size,pOffsets,pValues,&LastIndex);

	bool bRet2;
	if(OSVer == 0 )	bRet2 = ScanPagedPoolRanges_g(pMem,Size,pOffsets,pValues,&LastIndex);
	else if(OSVer == 1)	bRet2 = ScanPagedPoolRanges_g_10(pMem,Size,pOffsets,pValues,&LastIndex);

	bool bRet3;
	if(OSVer == 0 )	bRet3 = ScanSessionPool(pMem,Size,pOffsets,pValues,&LastIndex);
	else if(OSVer == 1)	bRet3 = ScanSessionPool_10(pMem,Size,pOffsets,pValues,&LastIndex);
	
	bool bRet4;
	if(OSVer == 0 )	bRet4 = ScanStackRanges(pMem,Size,pOffsets,pValues,&LastIndex);
	else if(OSVer == 1)	bRet4 = ScanStackRanges_10(pMem,Size,pOffsets,pValues,&LastIndex);

	
	bool bSuccess = false;
	if( bRet0 || bRet1 || bRet2 || bRet3 || bRet4)
	{

		if(LastIndex)
		{
			bSuccess = true;
			printf("Found %X leaks\r\n",LastIndex);

			for(ulong i =0;i<LastIndex;i++)
			{
				printf("Leak: %I64X at offset: %X\r\n",pValues[i],pOffsets[i]);
			}
		}
		return true;
	}

	VirtualFree(pValues,0,MEM_RELEASE);
	VirtualFree(pOffsets,0,MEM_RELEASE);

	if(bSuccess) return true;

	return false;
}


//bMode ==> 0 High and Low Dwords
//bMode ==> 1 only High Dword
bool ScanRegister_7(ulonglong Register,bool bMode)
{
	ulong Low = Register & 0xFFFFFFFF;
	ulong High = (Register >> 0x20) & 0xFFFFFFFF;

	//Modules
	ulong Magic_S = 0xfffff880;
	ulong Magic_E = 0xfffff960;//or more

	////NonPagedPool - Global
	ulong Magic_npg =  0xfffffa80;

	//PagedPool - Global 
	ulong Magic_S_PPG = 0xfffff8a0;
	ulong Magic_E_PPG = 0xfffff8bf;

	//Session
	ulong Magic_S_S = 0xFFFFF900;
	ulong Magic_E_S = 0xfffff920;

	if(bMode == true)
	{
		//Modules
		if( (	(High  >= Magic_S) ) && (	( High <= Magic_E) ) )
		{
			printf("Leak, Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}
		//NonPagedPool - Global
		if( High == Magic_npg)
		{
			printf("Leak, Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}

		//PagedPool - Global
		if( (	(High  >= Magic_S_PPG) ) && (	( High <= Magic_E_PPG) ) )
		{
			printf("Leak, Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}

		//Session
		if( (	(High  >= Magic_S_S) ) && (	( High <= Magic_E_S) ) )
		{
			printf("Leak, Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}
	}


	return false;
}

bool ScanRegister_10(ulonglong Register,bool bMode)
{
	ulong Low = Register & 0xFFFFFFFF;
	ulong High = (Register >> 0x20) & 0xFFFFFFFF;

	//Modules
	ulong Magic_S = 0xFFFFF800;
	ulong Magic_E = 0xfffff880;//or more

	ulong Magic_S_S = 0xFFFFF960;
	ulong Magic_S_E = 0xfffff980;



	////NonPagedPool - Global
	ulong Magic_npg = 0xffffe000;

	//PagedPool - Global 
	ulong Magic_S_PPG = 0xFFFFC000;
	ulong Magic_E_PPG = 0xffffcf80;

	//Session
	ulong _Magic_S_S = 0xfffff900;
	ulong _Magic_E_S = 0xfffff910;


	if(bMode == true)
	{
		//Modules
		if( (	(High  >= Magic_S) ) && (	( High <= Magic_E) ) )
		{
			printf("Leak (Module), Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}
		if( (	(High  >= Magic_S_S) ) && (	( High <= Magic_S_E) ) )
		{
			printf("Leak (Session Module), Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}
		//NonPagedPool - Global
		if( High == Magic_npg)
		{
			printf("Leak (Global NonPagedPool), Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}

		//PagedPool - Global
		if( (	(High  >= Magic_S_PPG) ) && (	( High < Magic_E_PPG) ) )
		{
			printf("Leak, Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}

		//Session
		if( (	(High  >= _Magic_S_S) ) && (	( High < _Magic_E_S) ) )
		{
			printf("Leak, Return Value: %I64X\r\n",Register);
			//getchar();
			return true;
		}
	}


	return false;
}




bool ScanRegister(ulonglong Register,bool bMode,ulong OSVer)
{
	if(OSVer==0) return ScanRegister_7(Register,bMode);
	else if(OSVer==1) return ScanRegister_10(Register,bMode);

	return false;
}