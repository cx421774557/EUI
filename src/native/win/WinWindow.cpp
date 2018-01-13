
#include "EUIWindow.h"
#include "EUI.h"
#include "WinWindow.h"

bool WinWindow::wndClassReg = false;

LRESULT CALLBACK WinWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN64
	WinWindow* wnd = (WinWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#else
	WinWindow* wnd = (WinWindow*)GetWindowLong(hwnd, GWL_USERDATA);
#endif

	if (wnd)
	{
		return wnd->Proc(hwnd, msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

WinWindow::WinWindow(EUIWidget* owner, bool popup, bool adjust) : NativeWindow(owner)
{
	EUI::wnds.push_back(Owner());

	need_strict_size = false;

	active = false;

	const char* class_name = "ENgineWindow";

	if (!wndClassReg)
	{
		WNDCLASS	winClass;
	
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hbrBackground = ( HBRUSH )GetStockObject( GRAY_BRUSH );
		winClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		winClass.hIcon = 0;
		winClass.hInstance = NULL;
		winClass.lpfnWndProc = &WndProc;

		winClass.lpszClassName = class_name;
		winClass.lpszMenuName = NULL;
		winClass.style = CS_CLASSDC | CS_DBLCLKS;

		RegisterClass(&winClass);

		wndClassReg = true;
	}

	int flag = 0;

	if (!popup)
	{
		flag = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
	}
	else
	{
		flag = WS_DLGFRAME | WS_BORDER | WS_CAPTION | WS_SIZEBOX;
	}

	RECT rect;

	rect.left = (LONG)Owner()->x;
	rect.top = (LONG)Owner()->y;

	rect.right = (LONG)(Owner()->x + Owner()->width);
	rect.bottom = (LONG)(Owner()->y + Owner()->height);

	if (adjust)
	{
		AdjustWindowRectEx(&rect, flag, false, NULL);
	}

	hack_wnd_width = rect.right - rect.left;
	hack_wnd_height = rect.bottom - rect.top;

	handle = CreateWindow( class_name, Owner()->text.c_str(), flag,
							rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
							NULL, NULL, NULL, NULL );

#ifdef _WIN64
	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)this);
#else
	SetWindowLong(handle, GWL_USERDATA, (LONG)this);
#endif

	if (!adjust)
	{
		RECT rect;
		
		GetClientRect(handle, &rect);
		Owner()->width = (float)rect.right;
		Owner()->height = (float)rect.bottom;
	}

	is_minimazed = false;
}

WinWindow::~WinWindow()
{
}

EUIWindow* WinWindow::Owner()
{
	return (EUIWindow*)owner;
}

LRESULT WinWindow::Proc( HWND hwnd, long msg, WPARAM wParam, LPARAM lParam )
{
	NativeWindow::ProcessWidget(msg, wParam, lParam);

	switch( msg )
	{
		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xFFF0) == SC_MINIMIZE)
			{
				Owner()->is_minimazed = true;
			}

			if ((wParam & 0xFFF0) == SC_RESTORE)
			{
				Owner()->is_minimazed = false;
			}
		}
		break;
		case WM_SIZE:
		{
			if (wParam != 1)
			{
				Owner()->width = (float)GET_X_LPARAM(lParam);
				Owner()->height = (float)GET_Y_LPARAM(lParam);

				Resize();
			}
		};
		break;
		case WM_CLOSE:
		{
			if (owner->listener)
			{
				owner->listener->OnWinClose(owner);
			}
		}
		break;
		case WM_SHOWWINDOW:
		{
			if (owner->listener)
			{
				owner->listener->OnWinShow(owner);
			}
		}
		break;
		case WM_ACTIVATE:
		{
			active = (LOWORD(wParam) > 0);
		}
		break;
		case WM_DESTROY:
		{
			for (int i = 0; i < (int)EUI::wnds.size(); i++)
			{
				if (EUI::wnds[i] == owner)
				{
					if (i == 0)
					{
						for (int i = 1; i < (int)EUI::wnds.size(); i++)
						{
							EUI::wnds[i]->Close();
						}
					}

					EUI::wnds.erase(EUI::wnds.begin() + i);
					break;
				}
			}
		}
		break;
		case WM_GETMINMAXINFO:
		{
			if (need_strict_size)
			{
				MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
			
				pmmi->ptMaxSize.x = hack_wnd_width;
				pmmi->ptMaxSize.y = hack_wnd_height;

				pmmi->ptMinTrackSize.x = hack_wnd_width;
				pmmi->ptMinTrackSize.y = hack_wnd_height;

				pmmi->ptMaxTrackSize.x = hack_wnd_width;
				pmmi->ptMaxTrackSize.y = hack_wnd_height;
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}

void WinWindow::Maximaze()
{
	ShowWindow(handle, SW_MAXIMIZE);
}

void WinWindow::SetAtScreenCenter()
{
	HWND desktop = GetDesktopWindow();

	RECT desktopRect;
	GetWindowRect(desktop, &desktopRect);
	
	RECT rect;
	GetWindowRect(handle, &rect);

	SetPos((desktopRect.right- rect.right) * 0.5f,
		   (desktopRect.bottom - rect.bottom) * 0.5f);

}

void WinWindow::Close()
{
	DestroyWindow(handle);
}