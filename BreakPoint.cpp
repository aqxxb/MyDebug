#include "stdafx.h"
#include "BreakPoint.h"
#include <windows.h>


CBreakPoint::CBreakPoint()
{
}


CBreakPoint::~CBreakPoint()
{
}

bool CBreakPoint::SetCcBreakPoint(HANDLE hProcess, DWORD dwAddress, BYTE& oldByte)
{
		BOOL bRet = TRUE;
		DWORD TmpLen = 0;		//����ʵ�ʶ�ȡ,д���ֽ�
		UCHAR tmp = 0;			//��ȡд����
		BYTE Int3 = 0xcc;		//cc�ϵ�
		//1. ����ԭ�ڴ�ҳ�ı������ԣ�ȷ����д��ȥ
		DWORD dwProtect = 0;
		VirtualProtectEx(hProcess, (LPVOID)dwAddress, 1, PAGE_READWRITE, &dwProtect);
		//2. ��ȡ���浱ǰ1�ֽ�����
		if (!ReadProcessMemory(hProcess, (LPVOID)(dwAddress), &oldByte, 1, &TmpLen))
			bRet = FALSE;
		//3. д��cc
		if (!WriteProcessMemory(hProcess, (LPVOID)(dwAddress), &Int3, 1, &TmpLen))
			bRet = FALSE;
		//4. ���ڴ�ҳ�ı�������д��ȥ
		VirtualProtectEx(hProcess, (LPVOID)dwAddress, 1, dwProtect, &dwProtect);
		return bRet;
}