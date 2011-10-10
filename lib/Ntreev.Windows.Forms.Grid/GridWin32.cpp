//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


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

	ControlPainter::ControlPainter()
		: m_colorKey(RGB(0,128,128))
	{
		m_width = 500;
		m_height = 500;
		m_dc = ::CreateCompatibleDC(NULL);
		HBITMAP hBitmap = ::CreateBitmap(m_width, m_height, 1, 32, NULL);
		SelectObject((HDC)m_dc, hBitmap);
		SetBkMode((HDC)m_dc, TRANSPARENT);

		m_colorKeyBrush = CreateSolidBrush(m_colorKey);
	}

	void ControlPainter::TransparentBlt(System::IntPtr hdc, _Control^ control, _Rectangle% renderRect, object^ /*value*/)
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
		
		RECT r;
		SetRect(&r, 0, 0, m_width, m_height);
		FillRect((HDC)m_dc, &r, (HBRUSH)m_colorKeyBrush);

		SendMessage(hControl, WM_PRINT, (WPARAM)m_dc, PRF_CLIENT|PRF_CHILDREN);

		int destWidth = control->Width - (renderRect.Left - control->Left);
		destWidth = System::Math::Min(destWidth, renderRect.Width);

		int destHeight = control->Height - (renderRect.Top - control->Top);
		destHeight = System::Math::Min(destHeight, renderRect.Height);

		::TransparentBlt(_hdc, renderRect.X, renderRect.Y, destWidth, destHeight, (HDC)m_dc,
			renderRect.Left - control->Left, renderRect.Top - control->Top, destWidth, destHeight, m_colorKey);
	}

	void ControlPainter::BitBlt(System::IntPtr hdc, _Control^ control, _Rectangle% renderRect, object^ /*value*/)
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

		SendMessage(hControl, WM_PRINT, (WPARAM)m_dc, PRF_CLIENT|PRF_CHILDREN);

		int destWidth = control->Width - (renderRect.Left - control->Left);
		destWidth = System::Math::Min(destWidth, renderRect.Width);

		int destHeight = control->Height - (renderRect.Top - control->Top);
		destHeight = System::Math::Min(destHeight, renderRect.Height);

		::BitBlt(_hdc, renderRect.X, renderRect.Y, destWidth, destHeight, (HDC)m_dc,
			renderRect.Left - control->Left, renderRect.Top - control->Top, SRCCOPY);
	}
} /*namespace Win32*/} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/