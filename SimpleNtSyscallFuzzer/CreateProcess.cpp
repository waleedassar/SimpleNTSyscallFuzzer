#include "stdafx.h"
#include "windows.h"
#include "Header.h"








int CreateProcessNative(wchar_t* ImagePath,HANDLE* phProcess,HANDLE* phThread)
{

	void* pProcessParameters = 0;
	_UNICODE_STRING UniExePath = {0};
	RtlInitUnicodeString(&UniExePath,ImagePath);

	int ret = RtlCreateProcessParametersEx(&pProcessParameters,
		&UniExePath,
		0 /* DllPath */,
		0 /* CurrentDirectory */,
		&UniExePath,
		0 /* Environment */,
		0 /* WindowTitle */,
		0 /* DesktopInfo */,
		0 /* ShellInfo */,
		0 /* RuntimeData */,
		RTL_USER_PROC_PARAMS_NORMALIZED /* Flags */);

	printf("RtlCreateProcessParametersEx, ret: %X\r\n",ret);
	if(ret >= 0)
	{
		HANDLE hProcess = 0;
		HANDLE hThread = 0;

		_PS_CREATE_INFO PsCreateInfo={0};
		PsCreateInfo.Size = sizeof(_PS_CREATE_INFO);
		PsCreateInfo.State = 0;
		PsCreateInfo.InitFlags = 3; // WriteOutputOnExit | DetectManifest

		ulong NumAttributes = 0x4;
		ulonglong TotalLength = sizeof(ulonglong) + (  (sizeof(_PS_ATTRIBUTE)) * NumAttributes);

		_PS_ATTRIBUTE_LIST* pAttributes = (_PS_ATTRIBUTE_LIST*)VirtualAlloc(0,TotalLength,MEM_COMMIT,PAGE_READWRITE);
		if(pAttributes == 0) ret = -20;



		pAttributes->TotalLength = TotalLength;

		//Process Full ImageName
		wchar_t FullPath[MAX_PATH]={0};
		wcscpy(FullPath,L"\\??\\");
		wcscat(FullPath,ImagePath);
		pAttributes->Attributes[0].Attribute = 0x020005;
		pAttributes->Attributes[0].Size = wcslen(FullPath) * 2;
		pAttributes->Attributes[0].Value = (ulonglong)FullPath;
		pAttributes->Attributes[0].ReturnLength = 0;

		//Client ID
		_CLIENT_ID CID={0};
		pAttributes->Attributes[1].Attribute = 0x010003;
		pAttributes->Attributes[1].Size = 0x10;//Sizeof(_CLIENT_ID)
		pAttributes->Attributes[1].Value = (ulonglong)(&CID);
		pAttributes->Attributes[1].ReturnLength = 0;

		//SectionImageInformation
		uchar SectionImageInformation[0x40]={0};
		pAttributes->Attributes[2].Attribute = 0x6;
		pAttributes->Attributes[2].Size = 0x40;
		pAttributes->Attributes[2].Value = (ulonglong)(&SectionImageInformation[0]);
		pAttributes->Attributes[2].ReturnLength = 0;

		//Unkown Attribute
		pAttributes->Attributes[3].Attribute = 0x6001A;
		pAttributes->Attributes[3].Size = 1;
		pAttributes->Attributes[3].Value = 1;
		pAttributes->Attributes[3].ReturnLength = 0;

		if(ret >= 0)
		{

			ret = ZwCreateUserProcess(&hProcess,
										&hThread,
										0x2000000 /* ProcessDesiredAccess */,
										0x2000000 /* ThreadDesiredAccess */,
										0 /* ProcessObjectAttributes */,
										0 /* ThreadObjectAttributes */,
										0 /* ProcessFlags */,
										0 /* ThreadFlags */,
										pProcessParameters,
										&PsCreateInfo,
										pAttributes);

			printf("ZwCreateUserProcess, ret: %X\r\n",ret);
			VirtualFree(pAttributes,0,MEM_RELEASE);

			if(phProcess) *phProcess = hProcess;
			if(phThread) *phThread = hThread;
		}
	}
	return ret;
}


int CreateJobProcessNative(wchar_t* ImagePath,HANDLE hJob,HANDLE* phProcess,HANDLE* phThread)
{

	void* pProcessParameters = 0;
	_UNICODE_STRING UniExePath = {0};
	RtlInitUnicodeString(&UniExePath,ImagePath);

	int ret = RtlCreateProcessParametersEx(&pProcessParameters,
		&UniExePath,
		0 /* DllPath */,
		0 /* CurrentDirectory */,
		&UniExePath,
		0 /* Environment */,
		0 /* WindowTitle */,
		0 /* DesktopInfo */,
		0 /* ShellInfo */,
		0 /* RuntimeData */,
		RTL_USER_PROC_PARAMS_NORMALIZED /* Flags */);

	printf("RtlCreateProcessParametersEx, ret: %X\r\n",ret);
	if(ret >= 0)
	{
		HANDLE hProcess = 0;
		HANDLE hThread = 0;

		_PS_CREATE_INFO PsCreateInfo={0};
		PsCreateInfo.Size = sizeof(_PS_CREATE_INFO);
		PsCreateInfo.State = 0;
		PsCreateInfo.InitFlags = 3; // WriteOutputOnExit | DetectManifest

		ulong NumAttributes = 0x5;
		ulonglong TotalLength = sizeof(ulonglong) + (  (sizeof(_PS_ATTRIBUTE)) * NumAttributes);

		_PS_ATTRIBUTE_LIST* pAttributes = (_PS_ATTRIBUTE_LIST*)VirtualAlloc(0,TotalLength,MEM_COMMIT,PAGE_READWRITE);
		if(pAttributes == 0) ret = -20;



		pAttributes->TotalLength = TotalLength;

		//Process Full ImageName
		wchar_t FullPath[MAX_PATH]={0};
		wcscpy(FullPath,L"\\??\\");
		wcscat(FullPath,ImagePath);
		pAttributes->Attributes[0].Attribute = 0x020005;
		pAttributes->Attributes[0].Size = wcslen(FullPath) * 2;
		pAttributes->Attributes[0].Value = (ulonglong)FullPath;
		pAttributes->Attributes[0].ReturnLength = 0;

		//Client ID
		_CLIENT_ID CID={0};
		pAttributes->Attributes[1].Attribute = 0x010003;
		pAttributes->Attributes[1].Size = 0x10;//Sizeof(_CLIENT_ID)
		pAttributes->Attributes[1].Value = (ulonglong)(&CID);
		pAttributes->Attributes[1].ReturnLength = 0;

		//SectionImageInformation
		uchar SectionImageInformation[0x40]={0};
		pAttributes->Attributes[2].Attribute = 0x6;
		pAttributes->Attributes[2].Size = 0x40;
		pAttributes->Attributes[2].Value = (ulonglong)(&SectionImageInformation[0]);
		pAttributes->Attributes[2].ReturnLength = 0;

		//Unkown Attribute
		pAttributes->Attributes[3].Attribute = 0x6001A;
		pAttributes->Attributes[3].Size = 1;
		pAttributes->Attributes[3].Value = 1;
		pAttributes->Attributes[3].ReturnLength = 0;

		//Jobs Attribute
		HANDLE hJobs[1] = {hJob};

		pAttributes->Attributes[4].Attribute = 0x20013;
		pAttributes->Attributes[4].Size = 0x8;
		pAttributes->Attributes[4].Value = (ulonglong)(hJobs);
		pAttributes->Attributes[4].ReturnLength = 0;

		if(ret >= 0)
		{

			ret = ZwCreateUserProcess(&hProcess,
										&hThread,
										0x2000000 /* ProcessDesiredAccess */,
										0x2000000 /* ThreadDesiredAccess */,
										0 /* ProcessObjectAttributes */,
										0 /* ThreadObjectAttributes */,
										0 /* ProcessFlags */,
										0 /* ThreadFlags */,
										pProcessParameters,
										&PsCreateInfo,
										pAttributes);

			printf("ZwCreateUserProcess, ret: %X\r\n",ret);
			VirtualFree(pAttributes,0,MEM_RELEASE);

			if(phProcess) *phProcess = hProcess;
			if(phThread) *phThread = hThread;
		}
	}
	return ret;
}


//with commandline
int CreateJobProcessNative_C(wchar_t* ImagePath,wchar_t* CommandLine, HANDLE hJob,HANDLE* phProcess,HANDLE* phThread)
{

	void* pProcessParameters = 0;
	_UNICODE_STRING UniExePath = {0};
	RtlInitUnicodeString(&UniExePath,ImagePath);


	_UNICODE_STRING CmdLine = {0};
	RtlInitUnicodeString(&CmdLine,CommandLine);

	int ret = RtlCreateProcessParametersEx(&pProcessParameters,
		&UniExePath,
		0 /* DllPath */,
		0 /* CurrentDirectory */,
		&CmdLine,
		0 /* Environment */,
		0 /* WindowTitle */,
		0 /* DesktopInfo */,
		0 /* ShellInfo */,
		0 /* RuntimeData */,
		RTL_USER_PROC_PARAMS_NORMALIZED /* Flags */);

	printf("RtlCreateProcessParametersEx, ret: %X\r\n",ret);
	if(ret >= 0)
	{
		HANDLE hProcess = 0;
		HANDLE hThread = 0;

		_PS_CREATE_INFO PsCreateInfo={0};
		PsCreateInfo.Size = sizeof(_PS_CREATE_INFO);
		PsCreateInfo.State = 0;
		PsCreateInfo.InitFlags = 3; // WriteOutputOnExit | DetectManifest

		ulong NumAttributes = 0x5;
		ulonglong TotalLength = sizeof(ulonglong) + (  (sizeof(_PS_ATTRIBUTE)) * NumAttributes);

		_PS_ATTRIBUTE_LIST* pAttributes = (_PS_ATTRIBUTE_LIST*)VirtualAlloc(0,TotalLength,MEM_COMMIT,PAGE_READWRITE);
		if(pAttributes == 0) ret = -20;



		pAttributes->TotalLength = TotalLength;

		//Process Full ImageName
		wchar_t FullPath[MAX_PATH]={0};
		wcscpy(FullPath,L"\\??\\");
		wcscat(FullPath,ImagePath);
		pAttributes->Attributes[0].Attribute = 0x020005;
		pAttributes->Attributes[0].Size = wcslen(FullPath) * 2;
		pAttributes->Attributes[0].Value = (ulonglong)FullPath;
		pAttributes->Attributes[0].ReturnLength = 0;

		//Client ID
		_CLIENT_ID CID={0};
		pAttributes->Attributes[1].Attribute = 0x010003;
		pAttributes->Attributes[1].Size = 0x10;//Sizeof(_CLIENT_ID)
		pAttributes->Attributes[1].Value = (ulonglong)(&CID);
		pAttributes->Attributes[1].ReturnLength = 0;

		//SectionImageInformation
		uchar SectionImageInformation[0x40]={0};
		pAttributes->Attributes[2].Attribute = 0x6;
		pAttributes->Attributes[2].Size = 0x40;
		pAttributes->Attributes[2].Value = (ulonglong)(&SectionImageInformation[0]);
		pAttributes->Attributes[2].ReturnLength = 0;

		//Unkown Attribute
		pAttributes->Attributes[3].Attribute = 0x6001A;
		pAttributes->Attributes[3].Size = 1;
		pAttributes->Attributes[3].Value = 1;
		pAttributes->Attributes[3].ReturnLength = 0;

		//Jobs Attribute
		HANDLE hJobs[1] = {hJob};

		pAttributes->Attributes[4].Attribute = 0x20013;
		pAttributes->Attributes[4].Size = 0x8;
		pAttributes->Attributes[4].Value = (ulonglong)(hJobs);
		pAttributes->Attributes[4].ReturnLength = 0;

		if(ret >= 0)
		{

			ret = ZwCreateUserProcess(&hProcess,
										&hThread,
										0x2000000 /* ProcessDesiredAccess */,
										0x2000000 /* ThreadDesiredAccess */,
										0 /* ProcessObjectAttributes */,
										0 /* ThreadObjectAttributes */,
										0 /* ProcessFlags */,
										0 /* ThreadFlags */,
										pProcessParameters,
										&PsCreateInfo,
										pAttributes);

			printf("ZwCreateUserProcess, ret: %X\r\n",ret);
			VirtualFree(pAttributes,0,MEM_RELEASE);

			if(phProcess) *phProcess = hProcess;
			if(phThread) *phThread = hThread;
		}
	}
	return ret;
}
