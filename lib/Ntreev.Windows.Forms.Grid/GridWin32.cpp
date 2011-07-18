#include "StdAfx.h"
#include "GridWin32.h"
#include <windows.h>
#include "GridColumnControl2.h"

#pragma comment(lib, "Imm32.lib")

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Win32
{
	System::IntPtr API::SendMessage(System::IntPtr handle, int message, System::IntPtr wParam, System::IntPtr lParam)
	{
		LRESULT result = ::SendMessage((HWND)handle.ToPointer(), message, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
		return System::IntPtr(result);
	}

#pragma push_macro("PostMessage")
#undef PostMessage
	System::IntPtr API::PostMessage(System::IntPtr handle, int message, System::IntPtr wParam, System::IntPtr lParam)
	{
#pragma pop_macro("PostMessage")
		LRESULT result = ::PostMessage((HWND)handle.ToPointer(), message, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
		return System::IntPtr(result);
	}

	System::Char API::ImmGetVirtualKey(System::IntPtr handle)
	{
		return (System::Char)::ImmGetVirtualKey((HWND)handle.ToPointer());
	}

	int API::HiWord(int l)
	{
		return HIWORD(l);
	}

	int API::LoWord(int l)
	{
		return LOWORD(l);
	}

	System::IntPtr API::MakeLParam(int l, int h)
	{
		return System::IntPtr((int)MAKELPARAM(l, h));
	}

	bool API::ScrollWindow(System::IntPtr handle, int XAmount, int YAmount, System::Drawing::Rectangle^ rect, System::Drawing::Rectangle^ clipRect)
	{
		RECT r1;
		SetRect(&r1, rect->Left, rect->Top, rect->Right, rect->Bottom);
		RECT r2;
		SetRect(&r2, clipRect->Left, clipRect->Top, clipRect->Right, clipRect->Bottom);
		return ::ScrollWindow((HWND)handle.ToPointer(), XAmount, YAmount, &r1, &r1) == TRUE ? true : false;
	}

	void API::InvokeLButtonDownEvent(System::Drawing::Point /*location*/)
	{
		//POINT pt;
		//::GetCursorPos(&pt);
		//int x = (DWORD)(65535.0 * pt.x / (double)GetSystemMetrics(SM_CXSCREEN));
		//int y = (DWORD)(65535.0 * pt.y / (double)GetSystemMetrics(SM_CYSCREEN));
		//::mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x, y, 0, 0);
		//::mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, 0, 0, 0, 0);
		::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		

		//::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		//::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}

	BitBlt::BitBlt()
	{
		m_width = 500;
		m_height = 500;
		m_dc = ::CreateCompatibleDC(NULL);
		HBITMAP hBitmap = ::CreateBitmap(m_width, m_height, 1, 32, NULL);
		SelectObject((HDC)m_dc, hBitmap);
	}

	void BitBlt::Do(System::IntPtr hdc, _Control^ control, _Rectangle% renderRect, object^ /*value*/)
	{
		HDC _hdc = (HDC)hdc.ToPointer();
		if(m_width < renderRect.Width || m_height < renderRect.Height)
		{
			m_width		= System::Math::Max(m_width, renderRect.Width);
			m_height	= System::Math::Max(m_height, renderRect.Height);
			HBITMAP hh	= CreateBitmap(m_width, m_height, 1, 32, NULL);
			HGDIOBJ hOldBitmap = SelectObject((HDC)m_dc, hh);
			DeleteObject(hOldBitmap);
		}

		HWND hControl = (HWND)control->Handle.ToPointer();
		::SendMessage(hControl, WM_PRINT, (WPARAM)m_dc, PRF_CLIENT|PRF_CHILDREN);

		//HWND hChild = ::GetWindow(hControl, GW_CHILD);
		//while(hChild != NULL)
		//{
		//	::SendMessage(hChild, WM_PRINT, (WPARAM)m_dc, PRF_CLIENT);
		//	hChild = ::GetWindow(hChild, GW_HWNDNEXT);
		//}
		::BitBlt(_hdc, renderRect.X, renderRect.Y, renderRect.Width, renderRect.Height, (HDC)m_dc, 0, 0, SRCCOPY);
	}
} /*namespace Win32*/} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/