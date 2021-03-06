﻿//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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
#include "Tooltip.h"
#include <windows.h>
#include "GridControl.h"

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ToolTip::ToolTipItem::ToolTipItem()
    {
        m_created = false;
        InitCommonControls();
    }

    void ToolTip::ToolTipItem::Show(System::String^ text, System::IntPtr handle)
    {
        static TOOLINFO ti;
        if(m_created == false)
        {
            HWND hwndParent = (HWND)handle.ToPointer();
			HINSTANCE inst = (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE);

			HWND hTooltip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, nullptr,
                WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                hwndParent, nullptr, inst,
                nullptr);

            m_tooltip = (void*)hTooltip;
            static wchar_t strText[] = L"wow";

			HWND hwnd = (HWND)handle.ToPointer();
			HWND parent = GetParent(hwnd);
			while(parent != nullptr) 
			{
				hwnd = parent;
				parent = GetParent(hwnd);
			}

            ti.cbSize = sizeof(TOOLINFO) - sizeof(void*);
            ti.hwnd = parent;
            ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
            ti.uId = (UINT_PTR)hwndParent;
            ti.lpszText = strText;
			ti.hinst = inst;
            ::SendMessage((HWND)m_tooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
            m_created = true;
        }

		::SendMessage((HWND)m_tooltip, TTM_ACTIVATE, (WPARAM)TRUE, 0);

        wchar_t* strTooltip = new wchar_t[text->Length + 1];
        wcscpy_s(strTooltip, text->Length + 1, ToNativeString::Convert(text));
        ti.lpszText = strTooltip;
        ::SendMessage((HWND)m_tooltip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);
        
        delete [] strTooltip;
    }

    void ToolTip::ToolTipItem::Hide()
    {
        ::SendMessage((HWND)m_tooltip, TTM_ACTIVATE, (WPARAM)FALSE, 0);
    }

    ToolTip::ToolTipItemCollection::ToolTipItemCollection()
        : m_previousIndex(0), m_currentIndex(0)
    {

    }

    void ToolTip::ToolTipItemCollection::MoveNext()
    {
        if(Count == 0)
            throw gcnew System::NullReferenceException();
        m_previousIndex = m_currentIndex;
        m_currentIndex = (m_currentIndex + 1) % Count;
    }

    ToolTip::ToolTipItem^ ToolTip::ToolTipItemCollection::Current::get()
    {
        return default[m_currentIndex];
    }

    ToolTip::ToolTipItem^ ToolTip::ToolTipItemCollection::Previous::get()
    {
        return default[m_previousIndex];
    }

    ToolTip::ToolTip(_GridControl^ gridControl, int count)
        : GridObject(gridControl)
    {
        for(int i=0 ; i<count ; i++)
        {
            m_toolTips.Add(gcnew ToolTipItem());
        }
        m_showed = false;
    }

    void ToolTip::Hide()
    {
        if(m_showed == false)
            return;
        ToolTipItem^ previous = m_toolTips.Previous;
        previous->Hide();
        m_showed = false;
    }

    void ToolTip::Show(System::String^ text)
    {
        if(m_showed == true)
            return;
        ToolTipItem^ current = m_toolTips.Current;
        current->Show(text, this->GridControl->Handle);
        m_toolTips.MoveNext();
        m_showed = true;
        //Debug::WriteLine("Show tooltip : {0}", text);
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/