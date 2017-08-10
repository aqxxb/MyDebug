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
/*��������DebugMain
/*�������ܣ�������������һЩ��ʼ������
/*����1��
/*����2��
/*����ֵ��
******************************************/
void CMyDebug::DebugMain()
{
	
	//�����ַ���
	string str;
	cout << "������Ҫ���Գ����·����" << endl;
	cin >> str;
	//ת����ʽ��������
	int mystringSize = (int)(str.length() + 1);
	WCHAR* mywstring = new WCHAR[mystringSize];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, mywstring, mystringSize);
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));//��ʼ��si���ڴ���е�ֵ
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	CreateProcess(mywstring, NULL, NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
		NULL, NULL, &si, &pi);
	//��ͼ
	system("cls");
	Draw1.SetWindowsSize("������", 120, 80);
	Draw1.draw();

	//��ʼ�������¼��ṹ��
	DEBUG_EVENT DbgEvent = { 0 };
	DWORD dwState = DBG_EXCEPTION_NOT_HANDLED;
	//�ȴ�Ŀ��Exe���������¼�
	while (true)
	{
		//�ȴ���Ϣ
		WaitForDebugEvent(&DbgEvent, INFINITE);
		// �ַ���Ϣ
		dwState = DispatchDbgEvent(DbgEvent);
		// �������쳣,�������б�����Exe
		ContinueDebugEvent(DbgEvent.dwProcessId, DbgEvent.dwThreadId, dwState);
	}
	delete[] mywstring;
}

/****************************************
/*��������DispatchDbgEvent
/*�������ܣ��ַ���Ϣ
/*����1��
/*����2��
/*����ֵ�����ص��Ե�����
******************************************/
DWORD CMyDebug::DispatchDbgEvent(DEBUG_EVENT& de)
{
	//�жϵ�������
	DWORD dwRet = DBG_EXCEPTION_NOT_HANDLED;
	switch (de.dwDebugEventCode)
	{
	case CREATE_PROCESS_DEBUG_EVENT:	//���̵���
		Draw1.UpdateMsg(de);
		dwRet = OnCreateProcess(de);
		break;
	case EXCEPTION_DEBUG_EVENT:			//�쳣����	
		Draw1.UpdateMsg(de);
		dwRet = OnException(de); 
		break;
	case CREATE_THREAD_DEBUG_EVENT:		//�̵߳���
	case EXIT_THREAD_DEBUG_EVENT:		//�˳��߳�
	case EXIT_PROCESS_DEBUG_EVENT:		//�˳�����
	case LOAD_DLL_DEBUG_EVENT:			//����DLL
	case UNLOAD_DLL_DEBUG_EVENT:		//ж��DLL
	case OUTPUT_DEBUG_STRING_EVENT:		//��������ַ���
	case RIP_EVENT:						//RIP����
		return dwRet;					//������
	}
	return dwRet;
}
/****************************************
/*��������OnCreateProcess
/*�������ܣ����̵��ԣ��ڴ��������Ժ���OEP����������ϵ�
/*����1��
/*����2��
/*����ֵ��
******************************************/
DWORD CMyDebug::OnCreateProcess(DEBUG_EVENT& de)
{
	DWORD dwRet = DBG_CONTINUE;
	// ������ģ��OEP�ϵ�
	DWORD dwOep = (DWORD)de.u.CreateProcessInfo.lpStartAddress;
	BYTE bOld = 0;
	bi.bt = BP_CC;
	bi.dwAddress = dwOep;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, de.dwProcessId);
	if (BreakPoint.SetCcBreakPoint(hProcess, dwOep, bi.u.bCCOld))
	{
		// ����ϵ���Ϣ
		m_vecBp.push_back(bi);
	}
	else
	{
		// ���öϵ�ʧ�� 
		dwRet = DBG_EXCEPTION_NOT_HANDLED;
	}
	CloseHandle(hProcess);
	return dwRet;
}
/****************************************
/*��������OnException
/*�������ܣ��쳣������
/*����1��
/*����2��
/*����ֵ��
******************************************/
DWORD CMyDebug::OnException(DEBUG_EVENT& de)
{
	// �����쳣���ͷֱ���
	DWORD dwRet = DBG_EXCEPTION_NOT_HANDLED;
	switch (de.u.Exception.ExceptionRecord.ExceptionCode)
	{
		//����ϵ�
	case EXCEPTION_BREAKPOINT:
	//	dwRet = OnExceptionCc(de);
		//printf("����ϵ�\n");
		break;

		//�����쳣
	case EXCEPTION_SINGLE_STEP:
	//	dwRet = OnExceptionSingleStep(de);
	//	printf("�����쳣\n");
		break;

		//�ڴ�����쳣
	case EXCEPTION_ACCESS_VIOLATION:
	//	dwRet = m_swbkpt.OnExceptionAccess(de);
	//	printf("�ڴ�����쳣\n");
		break;

	default:
		break;
	}
	return dwRet;
}