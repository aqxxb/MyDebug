#pragma once
#include <wtypes.h>
#include "MyDebug.h"
class CDraw
{
public:
	CDraw();
	~CDraw();
	bool SetWindowsSize(char*pszWindowTitle, int nX, int nY);
	void WriteChar(int High, int Wide, char*pszChar, WORD wArr);
	void draw();
	void UpdateMsg(DEBUG_EVENT & de);
};
