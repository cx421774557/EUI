
#pragma once

#include "native/NativeEditBox.h"

#ifdef PLATFORM_WIN

class EUIEditBox;

class WinEditBox : public NativeEditBox
{
public:

	static int timerID; 
	int cur_timerID;
	float time2callback = -1.0f;

	WinEditBox(EUIWidget* owner);
	virtual ~WinEditBox();

	EUIEditBox* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);
	virtual void SetText(const char* txt);
	virtual const char* GetText();
};
#endif
