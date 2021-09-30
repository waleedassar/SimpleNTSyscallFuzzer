#include "stdafx.h"
#include "Header.h"



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
