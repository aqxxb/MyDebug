#include "stdafx.h"
#include "MyDebug.h"
#include <windows.h>
#include <string>
#include <iostream>
using std::string;
using std::endl;
using std::cout;
using std::cin;


CMyDebug::CMyDebug()
{
	
}


CMyDebug::~CMyDebug()
{
}
/****************************************
/*函数名：DebugMain
/*函数功能：主函数，进行一些初始化操作
/*参数1：
/*参数2：
/*返回值：
******************************************/
void CMyDebug::DebugMain()
{
	
	//输入字符串
	string str;
	cout << "请输入要调试程序的路径：" << endl;
	cin >> str;
	//转换格式创建进程
	int mystringSize = (int)(str.length() + 1);
	WCHAR* mywstring = new WCHAR[mystringSize];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, mywstring, mystringSize);
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));//初始化si在内存块中的值
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	CreateProcess(mywstring, NULL, NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
		NULL, NULL, &si, &pi);
	//画图
	system("cls");
	Draw1.SetWindowsSize("调试器", 120, 80);
	Draw1.draw();

	//初始化调试事件结构体
	DEBUG_EVENT DbgEvent = { 0 };
	DWORD dwState = DBG_EXCEPTION_NOT_HANDLED;
	//等待目标Exe产生调试事件
	while (true)
	{
		//等待消息
		WaitForDebugEvent(&DbgEvent, INFINITE);
		// 分发消息
		dwState = DispatchDbgEvent(DbgEvent);
		// 处理完异常,继续运行被调试Exe
		ContinueDebugEvent(DbgEvent.dwProcessId, DbgEvent.dwThreadId, dwState);
	}
	delete[] mywstring;
}

/****************************************
/*函数名：DispatchDbgEvent
/*函数功能：分发消息
/*参数1：
/*参数2：
/*返回值：返回调试的类型
******************************************/
DWORD CMyDebug::DispatchDbgEvent(DEBUG_EVENT& de)
{
	//判断调试类型
	DWORD dwRet = DBG_EXCEPTION_NOT_HANDLED;
	switch (de.dwDebugEventCode)
	{
	case CREATE_PROCESS_DEBUG_EVENT:	//进程调试
		Draw1.UpdateMsg(de);
		dwRet = OnCreateProcess(de);
		break;
	case EXCEPTION_DEBUG_EVENT:			//异常调试	
		Draw1.UpdateMsg(de);
		dwRet = OnException(de); 
		break;
	case CREATE_THREAD_DEBUG_EVENT:		//线程调试
	case EXIT_THREAD_DEBUG_EVENT:		//退出线程
	case EXIT_PROCESS_DEBUG_EVENT:		//退出进程
	case LOAD_DLL_DEBUG_EVENT:			//加载DLL
	case UNLOAD_DLL_DEBUG_EVENT:		//卸载DLL
	case OUTPUT_DEBUG_STRING_EVENT:		//输出调试字符串
	case RIP_EVENT:						//RIP调试
		return dwRet;					//不处理
	}
	return dwRet;
}
/****************************************
/*函数名：OnCreateProcess
/*函数功能：进程调试，在创建进程以后，在OEP处设置软件断点
/*参数1：
/*参数2：
/*返回值：
******************************************/
DWORD CMyDebug::OnCreateProcess(DEBUG_EVENT& de)
{
	DWORD dwRet = DBG_CONTINUE;
	// 设置主模块OEP断点
	DWORD dwOep = (DWORD)de.u.CreateProcessInfo.lpStartAddress;
	BYTE bOld = 0;
	bi.bt = BP_CC;
	bi.dwAddress = dwOep;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, de.dwProcessId);
	if (BreakPoint.SetCcBreakPoint(hProcess, dwOep, bi.u.bCCOld))
	{
		// 保存断点信息
		m_vecBp.push_back(bi);
	}
	else
	{
		// 设置断点失败 
		dwRet = DBG_EXCEPTION_NOT_HANDLED;
	}
	CloseHandle(hProcess);
	return dwRet;
}
/****************************************
/*函数名：OnException
/*函数功能：异常处理函数
/*参数1：
/*参数2：
/*返回值：
******************************************/
DWORD CMyDebug::OnException(DEBUG_EVENT& de)
{
	// 根据异常类型分别处理
	DWORD dwRet = DBG_EXCEPTION_NOT_HANDLED;
	switch (de.u.Exception.ExceptionRecord.ExceptionCode)
	{
		//软件断点
	case EXCEPTION_BREAKPOINT:
	//	dwRet = OnExceptionCc(de);
		//printf("软件断点\n");
		break;

		//单步异常
	case EXCEPTION_SINGLE_STEP:
	//	dwRet = OnExceptionSingleStep(de);
	//	printf("单步异常\n");
		break;

		//内存访问异常
	case EXCEPTION_ACCESS_VIOLATION:
	//	dwRet = m_swbkpt.OnExceptionAccess(de);
	//	printf("内存访问异常\n");
		break;

	default:
		break;
	}
	return dwRet;
}