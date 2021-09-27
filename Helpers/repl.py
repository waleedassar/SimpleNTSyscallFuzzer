import os,sys,time





All="w10xKdEnumerateDebuggingDevices ,w10xHalAllocatePmcCounterSet ,w10NtWaitForAlertByThreadId ,w10NtUpdateWnfStateData ,w10NtUnsubscribeWnfStateChange ,w10NtUnmapViewOfSectionEx ,w10NtSubscribeWnfStateChange ,w10NtSinglePhaseReject_0 ,w10NtSetWnfProcessNotificationEvent ,w10NtSetTimer2 ,w10NtSetIRTimer ,w10NtSetInformationVirtualMemory ,w10NtSetInformationTransactionManager_0 ,w10NtSetInformationSymbolicLink ,w10NtSetCachedSigningLevel ,w10NtRollforwardTransactionManager_0 ,w10NtRevertContainerImpersonation ,w10NtRenameTransactionManager_0 ,w10NtRegisterProtocolAddressInformation_0 ,w10NtQueryWnfStateNameInformation ,w10NtQueryWnfStateData ,w10NtOpenPartition ,w10NtManagePartition ,w10NtLoadEnclaveData ,w10NtInitializeEnclave ,w10NtGetCurrentProcessorNumberEx ,w10NtGetCompleteWnfStateSubscription ,w10NtGetCachedSigningLevel ,w10NtFlushBuffersFileEx ,w10NtFilterBootOption ,w10NtDeleteWnfStateName ,w10NtDeleteWnfStateData ,w10NtCreateWnfStateName ,w10NtCreateWaitCompletionPacket ,w10NtCreateTokenEx ,w10NtCreateTimer2 ,w10NtCreatePartition ,w10NtCreateLowBoxToken ,w10NtCreateIRTimer ,w10NtCreateEnclave ,w10NtCreateDirectoryObjectEx ,w10NtCompareObjects ,w10NtCancelWaitCompletionPacket ,w10NtCancelTimer2 ,w10NtAssociateWaitCompletionPacket ,w10NtAlpcImpersonateClientContainerOfPort ,w10NtAlpcConnectPortEx ,w10NtAlertThreadByThreadId ,w10NtAddAtomEx"


strX = "void w7NtWaitForSingleObject\r\n(unsigned long SysCall,\r\nunsigned long long* Args,\r\nvoid** pPool,\r\nvoid** pSecondLevelPool)\r\n{\r\n}\r\n"

f = open("repl.txt","w")


AllX = All.split(",")

for X in AllX:
	Z =  strX.replace("w7NtWaitForSingleObject",X)
	f.write(Z)
	
f.close()
	