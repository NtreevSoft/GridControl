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
#include "GridWin32.h"
#include <windows.h>
#include "GridColumnControl2.h"

#pragma comment(lib, "Imm32.lib")

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Win32
{
    System::IntPtr API::SendMessage(System::IntPtr handle, WM message, System::IntPtr wParam, System::IntPtr lParam)
    {
        LRESULT result = ::SendMessage((HWND)handle.ToPointer(), (uint)message, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
        return System::IntPtr(result);
    }

    System::IntPtr API::SendMessage(System::Windows::Forms::Message msg)
    {
        //System::Diagnostics::Trace::WriteLine(System::String::Format("SendMessage : {0}", (WM)msg.Msg));
        return SendMessage(msg.HWnd, (WM)msg.Msg, msg.WParam, msg.LParam);
    }

#pragma push_macro("PostMessage")
#undef PostMessage
    System::IntPtr API::PostMessage(System::IntPtr handle, WM message, System::IntPtr wParam, System::IntPtr lParam)
    {
#pragma pop_macro("PostMessage")
        LRESULT result = ::PostMessage((HWND)handle.ToPointer(), (uint)message, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
        return System::IntPtr(result);
    }

#pragma push_macro("PostMessage")
#undef PostMessage
    System::IntPtr API::PostMessage(System::Windows::Forms::Message msg)
    {
        return PostMessage(msg.HWnd, (WM)msg.Msg, msg.WParam, msg.LParam);
#pragma pop_macro("PostMessage")
    }

    System::Char API::ImmGetVirtualKey(System::IntPtr handle)
    {
        return (System::Char)::ImmGetVirtualKey((HWND)handle.ToPointer());
    }

    int API::HiWord(System::IntPtr param)
    {
        return HIWORD(param.ToInt32());
    }

    int API::LoWord(System::IntPtr param)
    {
        return LOWORD(param.ToInt32());
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

    bool API::GetScrollTrackPosition(System::IntPtr handle, int scrollType, int* pValue)
    {
        SCROLLINFO si;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_TRACKPOS;
        if(GetScrollInfo((HWND)handle.ToPointer(), scrollType, &si) == false)
            return false;
        *pValue = si.nTrackPos;
        return true;
    }

    void API::InvokeLButtonDownEvent(System::Drawing::Point /*location*/)
    {
        ;
        //::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        ::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
    }

    void API::InvokeLButtonUpEvent(System::Drawing::Point /*location*/)
    {
        ::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    void API::TranslateAndDispatchMessage(System::Windows::Forms::Message% msg)
    {
        MSG message;
        message.hwnd = (HWND)msg.HWnd.ToPointer();
        message.wParam = (WPARAM)msg.WParam.ToPointer();
        message.lParam = (WPARAM)msg.LParam.ToPointer();
        message.message = msg.Msg;
        ::TranslateMessage(&message);
        ::DispatchMessageW(&message);
    }

    void API::DoEventsModal(System::Windows::Forms::Form^ form)
    {
        using namespace System::Windows::Forms;
        HWND hWnd = (HWND)form->Handle.ToPointer();
        do
        {
            MSG msg;

            if(PeekMessageW(&msg, 0, 0, 0, PM_REMOVE) == TRUE)
            {
                if(msg.message == WM_QUIT)
                {
                    form->DialogResult = DialogResult::Cancel;
                    ::PostMessage(NULL, WM_QUIT, 0, 0);
                    break;
                }

                Message message;
                message.HWnd = System::IntPtr(msg.hwnd);
                message.Msg = (int)msg.message;
                message.WParam = System::IntPtr((int)msg.wParam);
                message.LParam = System::IntPtr((int)msg.lParam);

                //WM wm = (WM)message.Msg;
                //Control^ ctl = Control::FromHandle(message.HWnd);
                //System::String^ name = System::String::Empty;
                //if(ctl != nullptr)
                //    name = ctl->Name;

                //System::Diagnostics::Trace::WriteLine(System::String::Format("{0}, wParam : {1:X}, lParam : {2:X}, name : {3}", wm, (int)message.WParam, (int)message.LParam, name));

                //#pragma push_macro("WM_NCLBUTTONDOWN")
                //#undef WM_NCLBUTTONDOWN
                //                if(wm == WM::WM_NCLBUTTONDOWN)
                //                    int eqw=0;
                //#pragma pop_macro("WM_NCLBUTTONDOWN")

                if(Application::FilterMessage(message) == true)
                    continue;

                if((msg.message >= 0x100 ) && (msg.message <= 0x108))
                {
                    Control^ target = Control::FromChildHandle(message.HWnd);

                    if(target != nullptr)
                    {
                        if(target->PreProcessControlMessage(message) == PreProcessControlState::MessageProcessed)
                            continue;
                    }
                    else
                    {
                        if(IsDialogMessageW(hWnd, &msg) == FALSE)
                            continue;
                    }
                }

                msg.wParam = (WPARAM)message.WParam.ToInt32();
                msg.lParam = (WPARAM)message.LParam.ToInt32();

                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            else
            {
                WaitMessage();
            }
        }
        while(form->DialogResult == DialogResult::None);
    }

    void API::DoEvents()
    {
        using namespace System::Windows::Forms;
        MSG message;
        while(::PeekMessageW(&message, 0, 0, 0, PM_REMOVE) == TRUE)
        {
            System::Windows::Forms::Message msg;
            msg.Msg = (int)message.message;
            msg.HWnd = System::IntPtr(message.hwnd);
            msg.WParam = System::IntPtr((int)message.wParam);
            msg.LParam = System::IntPtr((int)message.lParam);

            if(Application::FilterMessage(msg) == false)
            {
                Control^ control = Control::FromHandle(msg.HWnd);
                if(control == nullptr || control->PreProcessControlMessage(msg) != PreProcessControlState::MessageProcessed)
                {
                    message.wParam = (WPARAM)msg.WParam.ToPointer();
                    message.lParam = (WPARAM)msg.LParam.ToPointer();
                    ::TranslateMessage(&message);
                    ::DispatchMessageW(&message);
                }
            }
        }
        ::MsgWaitForMultipleObjectsEx(0, 0, 250, 0xff, 4);
    }

    int API::OemKeyScan(System::Char word)
    {
        return (int)::OemKeyScan((WORD)word);
    }

#pragma push_macro("VkKeyScan")
#undef VkKeyScan
    int API::VkKeyScan(System::Char word)
#pragma pop_macro("VkKeyScan")
    {
        return LOWORD((int)::VkKeyScanW(word));
    }

    void API::keybd_event(int key)
    {
        ::keybd_event((BYTE)key, 0, 0, 0);
    }

    bool API::ShowWindow(System::IntPtr handle, int flag)
    {
        return ::ShowWindow((HWND)handle.ToPointer(), flag) == TRUE;
    }

    int API::GetFontInternalLeading(System::Drawing::Font^ font)
    {
        HFONT hFont = (HFONT)font->ToHfont().ToPointer();
        HDC hdc = CreateCompatibleDC(NULL);

        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        TEXTMETRICW textMetric;
        GetTextMetricsW(hdc, &textMetric);

        int internalLeading = (int)textMetric.tmInternalLeading;

        SelectObject(hdc, hOldFont);
        DeleteDC(hdc);
        return internalLeading;
    }

    System::IntPtr API::WindowFromPoint(System::Drawing::Point location)
    {
        POINT pt;
        pt.x = location.X;
        pt.y = location.Y;
        HWND hWnd = ::WindowFromPoint(pt);
        return System::IntPtr((void*)hWnd);
    }

    System::Drawing::Rectangle API::GetWindowRect(System::IntPtr handle)
    {
        RECT rt;
        if(::GetWindowRect((HWND)handle.ToPointer(), &rt) == FALSE)
            return System::Drawing::Rectangle::Empty;

        return System::Drawing::Rectangle::FromLTRB(rt.left, rt.top, rt.right, rt.bottom);
    }

    int API::GetAsyncKeyState(int vKey)
    {
        return (int)::GetAsyncKeyState(VK_LBUTTON);
    }

    void API::SetLButtonState(bool b)
    {
        static BYTE bytes[0xff];

        if(b == false)
        {
            BYTE bbb[0xff];
            ::GetKeyboardState(bbb);
            memcpy(bytes, bbb, sizeof(BYTE) * 0xff);
            bbb[VK_LBUTTON] = 0;
            ::SetKeyboardState(bbb);
            SHORT ddd = GetKeyState(1);
        }
        else
        {
            ::SetKeyboardState(bytes);
        }
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

        m_colorKeyBrush = CreateSolidBrush(RGB(240,240,240));
    }

    void ControlPainter::TransparentBlt(System::IntPtr hdc, System::Windows::Forms::Control^ control, System::Drawing::Rectangle paintRect, System::Object^ /*value*/)
    {
        HDC _hdc = (HDC)hdc.ToPointer();
        if(m_width < paintRect.Width || m_height < paintRect.Height)
        {
            m_width    = System::Math::Max(m_width, paintRect.Width);
            m_height   = System::Math::Max(m_height, paintRect.Height);
            HBITMAP hh = CreateBitmap(m_width, m_height, 1, 32, NULL);
            HGDIOBJ hOldBitmap = SelectObject((HDC)m_dc, hh);
            DeleteObject(hOldBitmap);
        }

        HWND hControl = (HWND)control->Handle.ToPointer();

        RECT r;
        SetRect(&r, 0, 0, m_width, m_height);
        HBRUSH hBrush = CreateSolidBrush(RGB(240,240,240));
        FillRect((HDC)m_dc, &r, (HBRUSH)hBrush);
        DeleteObject(hBrush);

        LRESULT re = SendMessage(hControl, WM_PRINT, (WPARAM)m_dc, PRF_CLIENT|PRF_CHILDREN);

        HRGN hRgn = CreateRectRgn(paintRect.X, paintRect.Y, paintRect.Right, paintRect.Bottom);
        HRGN restoreRegion = CreateRectRgn( 0, 0, 0, 0 );
        if (GetClipRgn(_hdc, restoreRegion ) != 1)
        {
            ::DeleteObject(restoreRegion);
            restoreRegion = NULL;
        }

        ::SelectClipRgn(_hdc, hRgn);

        ::TransparentBlt(_hdc, paintRect.X + control->Left, paintRect.Y + control->Top, control->Width, control->Height, (HDC)m_dc,
            0, 0, control->Width, control->Height, m_colorKey);

        ::SelectClipRgn(_hdc, restoreRegion);
        ::DeleteObject(hRgn);
        ::DeleteObject(restoreRegion);
    }

    void ControlPainter::BitBlt(System::IntPtr hdc, System::Windows::Forms::Control^ control, System::Drawing::Rectangle paintRect, System::Object^ /*value*/)
    {
        HDC _hdc = (HDC)hdc.ToPointer();
        if(m_width < paintRect.Width || m_height < paintRect.Height)
        {
            m_width    = System::Math::Max(m_width, paintRect.Width);
            m_height   = System::Math::Max(m_height, paintRect.Height);
            HBITMAP hh = CreateBitmap(m_width, m_height, 1, 32, NULL);
            HGDIOBJ hOldBitmap = SelectObject((HDC)m_dc, hh);
            DeleteObject(hOldBitmap);
        }

        HWND hControl = (HWND)control->Handle.ToPointer();

        RECT r;
        SetRect(&r, 0, 0, m_width, m_height);
        FillRect((HDC)m_dc, &r, (HBRUSH)m_colorKeyBrush);

        LRESULT re = SendMessage(hControl, WM_PRINT, (WPARAM)m_dc, PRF_CLIENT|PRF_CHILDREN);

        HRGN hRgn = CreateRectRgn(paintRect.X, paintRect.Y, paintRect.Right, paintRect.Bottom);
        HRGN restoreRegion = CreateRectRgn( 0, 0, 0, 0 );
        if (GetClipRgn(_hdc, restoreRegion ) != 1)
        {
            DeleteObject(restoreRegion);
            restoreRegion = NULL;
        }

        ::SelectClipRgn(_hdc, hRgn);

        ::BitBlt(_hdc, paintRect.X + control->Left, paintRect.Y + control->Top, control->Width, control->Height, (HDC)m_dc,
            0, 0, SRCCOPY);

        ::SelectClipRgn(_hdc, restoreRegion);
        ::DeleteObject(hRgn);
        ::DeleteObject(restoreRegion);
    }
} /*namespace Win32*/} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/