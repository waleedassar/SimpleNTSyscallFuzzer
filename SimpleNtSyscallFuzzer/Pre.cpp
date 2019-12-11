#include "stdafx.h"
#include "Header.h"

void Pre_NtUserGetThreadState(unsigned long long* Args,void** pPool,void** pSecondLevelPool)
{
		//0x1000
		if( Rand()%2 == 1) *(ulong*)(&Args[0]) = Rand()%0x10;
		//else Random
}