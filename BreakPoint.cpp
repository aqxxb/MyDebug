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
		DWORD TmpLen = 0;		//返回实际读取,写入字节
		UCHAR tmp = 0;			//读取写入结果
		BYTE Int3 = 0xcc;		//cc断点
		//1. 更改原内存页的保护属性，确保能写进去
		DWORD dwProtect = 0;
		VirtualProtectEx(hProcess, (LPVOID)dwAddress, 1, PAGE_READWRITE, &dwProtect);
		//2. 读取保存当前1字节数据
		if (!ReadProcessMemory(hProcess, (LPVOID)(dwAddress), &oldByte, 1, &TmpLen))
			bRet = FALSE;
		//3. 写入cc
		if (!WriteProcessMemory(hProcess, (LPVOID)(dwAddress), &Int3, 1, &TmpLen))
			bRet = FALSE;
		//4. 把内存页的保护属性写回去
		VirtualProtectEx(hProcess, (LPVOID)dwAddress, 1, dwProtect, &dwProtect);
		return bRet;
}