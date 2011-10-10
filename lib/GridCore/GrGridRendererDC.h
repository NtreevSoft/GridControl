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


#pragma once

#ifndef _WIN32_WINNT            // 필요한 최소 플랫폼을 Windows Vista로 지정합니다.
#define _WIN32_WINNT 0x0600     // 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#include "GrGridRenderer.h"
#include <windows.h>
#include <Uxtheme.h>
#include <vssym32.h>

class GrGridRendererDC : public GrGridRenderer
{
public:
	GrGridRendererDC(void* hWnd);
	virtual ~GrGridRendererDC();

	virtual bool			DoubleBuffered() const { return true; }

	virtual void			OnBeginRender(void* hdc);

	virtual void			DrawSplitterRow(const GrRect* pRenderRect);
	virtual void			DrawDropDown(const GrRect* pRenderRect, int nState);
	virtual void			DrawModal(const GrRect* pRenderRect, int nState);
	virtual void			DrawTreeGlyph(const GrRect* pRenderRect, bool bOpened = true);
	virtual void			DrawSortGlyph(const GrRect* pRenderRect, GrSort::Type sortType);

	virtual void			DrawHeader(GrFlag renderStyle, const GrRect* pRenderRect, GrColor Color = 0xffffffff, const GrRect* pClipping = NULL);
	virtual void			DrawCell(GrFlag renderStyle, GrColor color, const GrRect* pRenderRect, const GrRect* pClippingping = NULL);
#pragma push_macro("DrawText")
#undef DrawText
	virtual void			DrawText(GrFont* pFont, const wchar_t* strText, int nLen, GrColor Color, const GrRect* pRenderRect, const GrRect* pClipping = NULL);
#pragma pop_macro("DrawText")
	virtual void			DrawTextEllipsis(GrFont* pFont, const wchar_t* strText, int nLen, GrColor Color, const GrRect* pRenderRect, const GrRect* pClipping = NULL);
	virtual void			DrawTooltip(const GrRect* pRenderRect);

	virtual void			DrawSizingLine(int x1, int y1, int x2, int y2);
	virtual void			DrawSplitterMovingLine(int x1, int y1, int x2, int y2);
	virtual void			DrawCellLine(int x1, int y1, int x2, int y2);
	virtual void			DrawRectangle(const GrRect* pRenderRect, GrColor color, int nWidth);
	virtual void			FillRectangle(GrRect rect, GrColor color);

	virtual void			DrawLine(int x1, int y1, int x2, int y2, GrColor color);

	virtual void			OnEndRender();
	virtual void			SetTransform(GrPoint pt);
	virtual void*			GetDC() { return (void*)m_hdc; }
	virtual void			ReleaseDC(void* /*dc*/) {}

private:
	HDC						m_hdc;
	HWND					m_hWnd;
	PAINTSTRUCT				m_ps;

	HDC						m_hOverlayDC;
	HBITMAP					m_hOverlayBmp, m_hOverlayBmpOld;

	HDC						m_hColorDC;
	HBITMAP					m_hColorBmp, m_hColorBmpOld;

	HBRUSH					m_hMouseOverBrush;
	HBRUSH					m_hSelectedBrush;
	HBRUSH					m_hSelectedMouseOverBrush;
	HBRUSH					m_hWhiteBrush;
	HPEN					m_hFocusedPen;
	HPEN					m_hSelRectPen;
	HPEN					m_hSizingPen;
	HPEN					m_hSplitterMovingPen;

	HFONT					m_hFont;
	HGDIOBJ					m_hOldFont;

	HTHEME					m_hColumnTheme;
	HTHEME					m_hDropDownTheme;
	HTHEME					m_hModalTheme;
	HTHEME					m_hTreeTheme;
	HTHEME					m_hTooltip;

	DWORD					m_dwMajorVersion;

	GrPoint					m_ptTransform;
};

class GrFontDC : public GrFont
{
public:
	GrFontDC(void* hFont);
	virtual ~GrFontDC();

	virtual int		GetWidth(wchar_t w) const { return m_nCharWidth[w]; }
	virtual int		GetHeight() const { return m_nFontHeight; }

	virtual int		GetStringWidth(const wchar_t* str) const;

	virtual void*	GetFont() const { return m_hFont; }

#ifdef _MANAGED
	virtual System::Drawing::Font^	ToManaged() const;
#endif

private:



public:
	int		m_nCharWidth[0xffff];
	int		m_nFontHeight;

	HFONT	m_hFont;
};