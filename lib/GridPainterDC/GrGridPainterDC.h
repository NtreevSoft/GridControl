//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4475.19551
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


#pragma once

#include "GrGridPainter.h"

#ifndef _WIN32_WINNT // 필요한 최소 플랫폼을 Windows Vista로 지정합니다.
#define _WIN32_WINNT 0x0600 // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#include <windows.h>
#include <Uxtheme.h>
#include <vssym32.h>


class GrGridPainterDC : public GrGridPainter
{
public:
    GrGridPainterDC(HWND hWnd);
    virtual ~GrGridPainterDC();

    virtual void BeginPaint(void* hdc);

    virtual void DrawRowSplitter(const GrRect& paintRect);
    virtual void DrawDropDown(const GrRect& paintRect, GrControlState state);
    virtual void DrawModal(const GrRect& paintRect, GrControlState state);
    virtual void DrawTreeGlyph(const GrRect& paintRect, bool opened);
    virtual void DrawSortGlyph(const GrRect& paintRect, GrSort sortType);

    virtual void DrawColumn(GrFlag paintStyle, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect);
    virtual void DrawRow(GrFlag paintStyle, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect);
    virtual void DrawItem(GrFlag paintStyle, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRectping = NULL);
#pragma push_macro("DrawText")
#undef DrawText
    virtual void DrawText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect = NULL);
#pragma pop_macro("DrawText")
    virtual void DrawColumnText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect = NULL);
    virtual void DrawTooltip(const GrRect& paintRect);

    virtual void DrawResizingLine(int x1, int y1, int x2, int y2);
    virtual void DrawSplitterMovingLine(const GrRect& paintRect);
    virtual void DrawCellLine(int x1, int y1, int x2, int y2);
    virtual void DrawRectangle(const GrRect& rect, const GrColor& color);
    virtual void DrawLine(int x1, int y1, int x2, int y2, const GrColor& color);

    virtual void FillRectangle(const GrRect& rect, const GrColor& color);

    virtual void EndPaint();
    virtual void SetFont(GrFont* pFont);
    virtual void* GetDevice() { return (void*)m_hdc; }
    virtual void ReleaseDevice(void* /*dc*/) {}

private:
    void DrawHeader(GrFlag paintStyle, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect);

private:
    HDC m_hdc;
    HWND m_hWnd;
    PAINTSTRUCT m_ps;

    HDC m_hOverlayDC;
    HBITMAP m_hOverlayBmp, m_hOverlayBmpOld;

    HDC m_hColorDC;
    HBITMAP m_hColorBmp, m_hColorBmpOld;

    HBRUSH m_hMouseOverBrush;
    HBRUSH m_hSelectedBrush;
    HBRUSH m_hSelectedMouseOverBrush;
    HBRUSH m_hWhiteBrush;
    HPEN m_hFocusedPen;
    HPEN m_hSelRectPen;
    HPEN m_hSizingPen;
    HPEN m_hSplitterMovingPen;

    HFONT m_hFont;
    HGDIOBJ m_hOldFont;

    HTHEME m_hColumnTheme;
    HTHEME m_hDropDownTheme;
    HTHEME m_hModalTheme;
    HTHEME m_hTreeTheme;
    HTHEME m_hTooltip;

    DWORD m_dwMajorVersion;

    GrPoint m_ptTransform;
};

class GrFontDC : public GrFont
{
public:
    GrFontDC(void* fontHandle);
    virtual ~GrFontDC();

    virtual int GetCharacterWidth(wchar_t w) const;
    virtual int GetHeight() const { return m_height; }
    virtual int GetInternalLeading() const { return m_internalLeading; }
    virtual int GetExternalLeading() const { return m_externalLeading; }
    virtual int GetStringWidth(const std::wstring& text) const;

    HFONT GetFontHandle() const { return m_hFont; }

private:
    int m_height;
    int m_internalLeading;
    int m_externalLeading;

    HFONT m_hFont;
    HDC m_hdc;
    mutable int m_characterWidth[0xffff];
};