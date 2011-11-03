//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include <windows.h>
#include "GridTooltip.h"
#include "GridColumn.h"
#include "GridControl.h"

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	ToolTip::ToolTip(_GridControl^ gridControl) : GridObject(gridControl)
	{
		m_created = false;
		InitCommonControls();
	}

	void ToolTip::Show(string^ text)
	{
		static TOOLINFO ti;
		if(m_created == false)
		{
			HWND hwndParent = (HWND)GridControl->Handle.ToPointer();
			HWND hTooltip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
				WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				hwndParent, NULL, NULL,
				NULL);

			SetWindowPos(hTooltip, HWND_TOPMOST,0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);


			m_tooltip = (void*)hTooltip;
			static wchar_t strText[] = L"wow";


			ti.cbSize = sizeof(TOOLINFO);
			ti.hwnd = hwndParent;
			ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
			ti.uId = (UINT_PTR)hwndParent;
			ti.lpszText = strText;
			::SendMessage((HWND)m_tooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
			m_created = true;
		}

		//ToNativeString nativeText(text);
		wchar_t* strTooltip = new wchar_t[text->Length + 1];
		wcscpy(strTooltip, ToNativeString::Convert(text));
		ti.lpszText = strTooltip;
		::SendMessage((HWND)m_tooltip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);

		::SendMessage((HWND)m_tooltip, TTM_ACTIVATE, (WPARAM)TRUE, 0);
		delete [] strTooltip;
	}

	void ToolTip::Hide()
	{
		::SendMessage((HWND)m_tooltip, TTM_ACTIVATE, (WPARAM)FALSE, 0);
	}

	GridTooltip::GridTooltip(_GridControl^ gridControl, int count) : Rotator<Private::ToolTip^>(gridControl)
	{
		for(int i=0 ; i<count ; i++)
		{
			Private::ToolTip^ toolTip		= gcnew Private::ToolTip(gridControl);
			//toolTip->IsBalloon		= true;
			//toolTip->UseFading		= false;
			//toolTip->AutomaticDelay	= 100;
			//toolTip->InitialDelay	= 0;
			//toolTip->ReshowDelay	= 0;
			Add(toolTip);
		}
		m_showed = false;
	}

	void GridTooltip::Hide()
	{
		if(m_showed == false)
			return;
		Private::ToolTip^ previous = Previous;
		previous->Hide();
		m_showed = false;
		//previous->RemoveAll();
		//previous->Active = false;
		//Debug::WriteLine("Hide tooltip : {0}", Previous->ToolTipTitle);
	}

	void GridTooltip::Show(string^ text)
	{
		if(m_showed == true)
			return;
		Private::ToolTip^ current = Current;
		current->Show(text);
		MoveNext();
		m_showed = true;
		//Debug::WriteLine("Show tooltip : {0}", text);
	}
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/