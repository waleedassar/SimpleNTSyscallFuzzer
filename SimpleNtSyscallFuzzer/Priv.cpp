#include "stdafx.h"
#include "windows.h"


BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    LUID luid;
    BOOL bRet=FALSE;

    if (LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
    {
        TOKEN_PRIVILEGES tp;

        tp.PrivilegeCount=1;
        tp.Privileges[0].Luid=luid;
        tp.Privileges[0].Attributes=(bEnablePrivilege) ? SE_PRIVILEGE_ENABLED: 0;
        //
        //  Enable the privilege or disable all privileges.
        //
        if (AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
        {
            //
            //  Check to see if you have proper access.
            //  You may get "ERROR_NOT_ALL_ASSIGNED".
            //
            bRet=(GetLastError() == ERROR_SUCCESS);
        }
    }
    return bRet;
}

bool Acquire_Volume()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_MANAGE_VOLUME_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Acquire_Shutdown()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_SHUTDOWN_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}

bool Acquire_LockMemory()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_LOCK_MEMORY_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}



bool Acquire_CreatePageFile()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_CREATE_PAGEFILE_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}




bool Acquire_LoadDriver()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_LOAD_DRIVER_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}




bool Acquire_Tcb()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_TCB_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Acquire_Debug()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Acquire_INC_BASE()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_INC_BASE_PRIORITY_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}

bool Acquire_INC_QUOTA()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_INCREASE_QUOTA_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}

bool Acquire_ASSIGN_PrimaryToken()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_ASSIGNPRIMARYTOKEN_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}



//-------------

bool Revoke_Volume()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_MANAGE_VOLUME_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Revoke_Shutdown()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_SHUTDOWN_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}

bool Revoke_LockMemory()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_LOCK_MEMORY_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}



bool Revoke_CreatePageFile()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_CREATE_PAGEFILE_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}




bool Revoke_LoadDriver()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_LOAD_DRIVER_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}




bool Revoke_Tcb()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_TCB_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Revoke_Debug()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_DEBUG_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Revoke_INC_BASE()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_INC_BASE_PRIORITY_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}

bool Revoke_INC_QUOTA()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_INCREASE_QUOTA_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Revoke_ASSIGN_PrimaryToken()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_ASSIGNPRIMARYTOKEN_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Acquire_Environment()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_SYSTEM_ENVIRONMENT_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Revoke_Environment()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_SYSTEM_ENVIRONMENT_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}



bool Acquire_Restore()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_RESTORE_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Revoke_Restore()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_RESTORE_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}



bool Acquire_SystemProfile()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_SYSTEM_PROFILE_NAME, TRUE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}


bool Revoke_SystemProfile()
{
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		BOOL b = SetPrivilege(hToken, SE_SYSTEM_PROFILE_NAME, FALSE);
		CloseHandle(hToken);
		return b;
	}
	return false;
}