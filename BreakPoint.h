#pragma once
#include <wtypes.h>
class CBreakPoint
{
public:
	CBreakPoint();
	~CBreakPoint();
	bool SetCcBreakPoint(HANDLE hProcess, DWORD dwAddress, BYTE& oldByte);
};

