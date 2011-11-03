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
#include "GrGridRendererDC.h"
#include "GrGridCell.h"
#include <Shlobj.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "UxTheme.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "shell32.lib")

GrFont* GrFontCreator(void* pFontHandle)
{
	if(pFontHandle == NULL)
#ifdef _MANAGED
		return GrFontManager::FromManagedFont(System::Windows::Forms::Control::DefaultFont);
#else
		return new DeviceContext::GrFontDC((void*)GetStockObject(SYSTEM_FIXED_FONT));
#endif
	return new DeviceContext::GrFontDC(pFontHandle);
}

std::wstring GrFontHandleToKey(void* pFontHandle)
{
	LOGFONTW logfont;
	GetObjectW(pFontHandle, sizeof(logfont), &logfont);

	wchar_t strFileName[MAX_PATH];
	wsprintf(strFileName, L"%s%d%d%d%d%d%d%d%d%d%d%d%d%d.data", logfont.lfFaceName,
		logfont.lfCharSet, logfont.lfClipPrecision, logfont.lfEscapement, logfont.lfHeight,
		logfont.lfItalic, logfont.lfOrientation, logfont.lfOutPrecision, logfont.lfPitchAndFamily,
		logfont.lfQuality, logfont.lfStrikeOut, logfont.lfUnderline, logfont.lfWeight, logfont.lfWidth);

	return std::wstring(strFileName);
}


namespace DeviceContext
{
	struct RECT2 : RECT
	{
		RECT2() {}
		RECT2(const GrRect* rt) 
		{
			left   = rt->left;
			top    = rt->top;
			right  = rt->right;
			bottom = rt->bottom;
		}

		RECT2(const GrRect& rt) 
		{
			left   = rt.left;
			top    = rt.top;
			right  = rt.right;
			bottom = rt.bottom;
		}

		void	operator = (const GrRect& rt)
		{
			left   = rt.left;
			top    = rt.top;
			right  = rt.right;
			bottom = rt.bottom;
		}

		void	operator = (const GrRect* rt)
		{
			left   = rt->left;
			top    = rt->top;
			right  = rt->right;
			bottom = rt->bottom;
		}

		void	Offset(int x, int y)
		{
			left	+= x;
			top		+= y;
			right	+= x;
			bottom	+= y;
		}

		void	Offset(GrPoint pt)
		{
			Offset(pt.x, pt.y);
		}
	};

	GrGridRendererDC::GrGridRendererDC(void* hWnd) : GrGridRenderer(hWnd)
	{
		m_hWnd = (HWND)hWnd;
		m_hColumnTheme		= OpenThemeData(m_hWnd, L"HEADER");
		m_hDropDownTheme	= OpenThemeData(m_hWnd, L"COMBOBOX");
		m_hModalTheme		= OpenThemeData(m_hWnd, L"BUTTON");
		m_hTreeTheme		= OpenThemeData(m_hWnd, L"TREEVIEW");
		m_hTooltip			= OpenThemeData(m_hWnd, L"TOOLTIP");

		OSVERSIONINFOW osInfo;
		memset(&osInfo, 0, sizeof(OSVERSIONINFOW));
		osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
		GetVersionExW(&osInfo);

		m_dwMajorVersion = osInfo.dwMajorVersion;

		m_hOverlayBmp		= CreateBitmap(4, 4, 1, 32, NULL);
		m_hColorBmp			= CreateBitmap(4, 4, 1, 32, NULL);

		m_hMouseOverBrush	= CreateSolidBrush(RGB(240,240,240));
		m_hSelectedBrush	= CreateSolidBrush(RGB(47,106,197));
		m_hSelectedMouseOverBrush = CreateSolidBrush(RGB(71,130,221));

		m_hWhiteBrush		= (HBRUSH)GetStockObject(WHITE_BRUSH);
		m_hSelRectPen		= CreatePen(PS_SOLID, 1, RGB(208,215,229));

		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID; 
		lb.lbColor = RGB(125,125,125); 
		lb.lbHatch = 0; 
		DWORD Style[] = {1,1};
		m_hSizingPen		= ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_FLAT, 1, &lb, 2, Style);

		lb.lbColor = RGB(50,50,50);
		m_hFocusedPen		= ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_FLAT, 1, &lb, 2, Style);

		lb.lbStyle = BS_HATCHED; 
		lb.lbColor = RGB(0,0,0); 
		lb.lbHatch = HS_CROSS; 
		m_hSplitterMovingPen= ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_FLAT, DEF_SPLITTER_SIZE, &lb, 2, Style);
	}

	GrGridRendererDC::~GrGridRendererDC()
	{
		DeleteDC(m_hOverlayDC);
		DeleteObject(m_hSplitterMovingPen);
		DeleteObject(m_hSizingPen);
		DeleteObject(m_hSelRectPen);
		DeleteObject(m_hFocusedPen);
		DeleteObject(m_hSelectedMouseOverBrush);
		DeleteObject(m_hSelectedBrush);
		DeleteObject(m_hMouseOverBrush);

		//DestroyWindow(m_hTooltipWnd);
		DeleteObject(m_hOverlayBmp);
		CloseThemeData(m_hTooltip);
		CloseThemeData(m_hTreeTheme);
		CloseThemeData(m_hModalTheme);
		CloseThemeData(m_hDropDownTheme);
		CloseThemeData(m_hColumnTheme);
	}

	void GrGridRendererDC::OnBeginRender(void* _hdc)
	{
		m_hdc = (HDC)_hdc;

		m_hOverlayDC		= CreateCompatibleDC(m_hdc);
		m_hOverlayBmpOld	= (HBITMAP)SelectObject(m_hOverlayDC, (HGDIOBJ)m_hOverlayBmp);

		m_hFont		= NULL;
		m_hOldFont	= NULL;

		SetBkMode(m_hdc, TRANSPARENT);

		m_ptTransform = GrPoint(0,0);
	}

	void GrGridRendererDC::OnEndRender()
	{
		SelectObject(m_hdc, (HGDIOBJ)m_hOldFont);
		SelectObject(m_hOverlayDC, (HGDIOBJ)m_hOverlayBmpOld);

		m_hdc = NULL;
	}

	void GrGridRendererDC::DrawSplitterRow(const GrRect* pRenderRect)
	{
		RECT2 rt = pRenderRect;

		FillRect(m_hdc, &rt, m_hWhiteBrush);
		HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)m_hSelRectPen);

		MoveToEx(m_hdc, rt.left, rt.bottom-1, NULL);
		LineTo(m_hdc, rt.right, rt.bottom-1);
		SelectObject(m_hdc, hOldPen);
	}

	void GrGridRendererDC::DrawTreeGlyph(const GrRect* pRenderRect, bool bOpened)
	{
		RECT2 rt = pRenderRect;

		if(bOpened == true)
		{
			if(m_hTreeTheme != NULL && m_dwMajorVersion >= 6)
			{
				DrawThemeBackground(m_hTreeTheme, m_hdc, TVP_GLYPH, GLPS_OPENED, &rt, &rt);
			}
			else
			{
				GrPoint ptCenter = pRenderRect->GetCenter();

				rt.left   = ptCenter.x-4;
				rt.top    = ptCenter.y-4;
				rt.right  = ptCenter.x+5;
				rt.bottom = ptCenter.y+5;

				Rectangle(m_hdc, rt.left, rt.top, rt.right, rt.bottom);
				MoveToEx(m_hdc, rt.left+2, ptCenter.y, 0);
				LineTo(m_hdc, rt.right-2, ptCenter.y);
			}
		}
		else
		{
			if(m_hTreeTheme != NULL && m_dwMajorVersion >= 6)
			{
				DrawThemeBackground(m_hTreeTheme, m_hdc, TVP_GLYPH, GLPS_CLOSED, &rt, &rt);
			}
			else
			{
				GrPoint ptCenter = pRenderRect->GetCenter();

				rt.left   = ptCenter.x-4;
				rt.top    = ptCenter.y-4;
				rt.right  = ptCenter.x+5;
				rt.bottom = ptCenter.y+5;

				Rectangle(m_hdc, rt.left, rt.top, rt.right, rt.bottom);
				MoveToEx(m_hdc, rt.left+2, ptCenter.y, 0);
				LineTo(m_hdc, rt.right-2, ptCenter.y);

				MoveToEx(m_hdc, ptCenter.x, rt.top+2, 0);
				LineTo(m_hdc, ptCenter.x, rt.bottom-2);
			}
		}
	}

	void GrGridRendererDC::DrawCell(GrFlag renderStyle, GrColor color, const GrRect* pRenderRect, const GrRect* pClipping)
	{
		RECT2 rt = *pRenderRect;

		if(pClipping != NULL)
		{
			if(rt.right > pClipping->right)
				rt.right = pClipping->right;
			if(rt.bottom > pClipping->bottom)
				rt.bottom = pClipping->bottom;
		}

		HBRUSH hb = CreateSolidBrush(color.ToRGB());
		FillRect(m_hdc, &rt, hb);
		DeleteObject(hb);

		HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)m_hSelRectPen);

		if(renderStyle.Has(GRRS_NO_RIGHT_LINE) == false)
		{
			MoveToEx(m_hdc, rt.right-1, rt.top, NULL);
			LineTo(m_hdc, rt.right-1, rt.bottom-1);
		}

		if(renderStyle.Has(GRRS_NO_BOTTOM_LINE) == false)
		{
			MoveToEx(m_hdc, rt.left, rt.bottom-1, NULL);
			LineTo(m_hdc, rt.right, rt.bottom-1);
		}
		SelectObject(m_hdc, hOldPen);

		if(renderStyle.Has(GRRS_FOCUSED) == true)
		{
			HGDIOBJ hOld = SelectObject(m_hdc, (HGDIOBJ)m_hFocusedPen);
			HGDIOBJ hOb = SelectObject(m_hdc, GetStockObject(NULL_BRUSH));
			Rectangle(m_hdc, rt.left, rt.top, rt.right-1, rt.bottom-1);
			SelectObject(m_hdc, hOb);
			SelectObject(m_hdc, hOld);
		}
	}

#pragma push_macro("DrawText")
#undef DrawText
	void GrGridRendererDC::DrawText(GrFont* pFont, const wchar_t* strText, int nLen, GrColor Color, const GrRect* pRenderRect, const GrRect* pClipping)
#pragma pop_macro("DrawText")
	{
		RECT2 rtRender = pRenderRect;
		rtRender.Offset(m_ptTransform);
		HFONT hCurFont = (HFONT)pFont->GetFontHandle();

		if(m_hFont != hCurFont)
		{
			HFONT hOldFont = (HFONT)SelectObject(m_hdc, (HGDIOBJ)hCurFont);
			if(m_hOldFont == NULL)
				m_hOldFont = hOldFont;
			m_hFont = hCurFont;
		}

		SetTextColor(m_hdc, Color.ToRGB());
		if(pClipping)
		{
			RECT2 rtClip = pClipping;
			rtClip.Offset(m_ptTransform);
			ExtTextOutW(m_hdc, rtRender.left, rtRender.top, ETO_CLIPPED, &rtClip, strText, nLen, 0);
		}
		else
		{
			ExtTextOutW(m_hdc, rtRender.left, rtRender.top, 0, NULL, strText, nLen, 0);
		}
	}

	void GrGridRendererDC::DrawTextEllipsis(GrFont* pFont, const wchar_t* strText, int nLen, GrColor Color, const GrRect* pRenderRect, const GrRect* pClipping)
	{
		RECT2 rtRender = pRenderRect;
		rtRender.Offset(m_ptTransform);
		HFONT hCurFont = (HFONT)pFont->GetFontHandle();

		if(m_hFont != hCurFont)
		{
			HFONT hOldFont = (HFONT)SelectObject(m_hdc, (HGDIOBJ)hCurFont);
			if(m_hOldFont == NULL)
				m_hOldFont = hOldFont;
			m_hFont = hCurFont;
		}

		SetTextColor(m_hdc, Color.ToRGB());

		if(pClipping != NULL)
		{
			if(rtRender.right > pClipping->right)
				rtRender.right = pClipping->right;
			if(rtRender.bottom > pClipping->bottom)
				rtRender.bottom = pClipping->bottom;
		}

		DrawTextW(m_hdc, strText, nLen, &rtRender, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);
	}

	void GrGridRendererDC::DrawTooltip(const GrRect* pRenderRect)
	{
		RECT2 rtRender = pRenderRect;
		DrawThemeBackground(m_hTooltip, m_hdc, TTP_BALLOON, TTCS_NORMAL, &rtRender, &rtRender);
	}

	void GrGridRendererDC::DrawHeader(GrFlag renderStyle, const GrRect* pRenderRect, GrColor color, const GrRect* pClipping)
	{
		RECT2 rtRender(pRenderRect), rtClip(pRenderRect);
		rtRender.Offset(m_ptTransform);
		rtClip.Offset(m_ptTransform);
		rtRender.bottom++;

		BLENDFUNCTION blend;
		blend.BlendFlags			= 0;
		blend.AlphaFormat			= AC_SRC_ALPHA;
		blend.BlendOp				= AC_SRC_OVER;
		blend.SourceConstantAlpha   = 15;

		if(pClipping != NULL)
		{
			if(rtClip.right > pClipping->right)
				rtClip.right = pClipping->right;
			if(rtClip.bottom > pClipping->bottom)
				rtClip.bottom = pClipping->bottom;
		}

		if(m_hColumnTheme != NULL)
		{
			int nStateID = HIS_NORMAL;
			if(renderStyle & GRRS_MOUSEOVER)
				nStateID = HIS_HOT;
			if(renderStyle & GRRS_PRESSED)
				nStateID = HIS_PRESSED;

			DrawThemeBackground(m_hColumnTheme, m_hdc, HP_HEADERITEM, nStateID, &rtRender, &rtClip);
		}
		else
		{
			uint nStateID = 0;
			if(renderStyle & GRRS_MOUSEOVER)
				nStateID = DFCS_HOT;
			if(renderStyle & GRRS_PRESSED)
				nStateID = DFCS_PUSHED;
			DrawFrameControl(m_hdc, &rtRender, DFC_BUTTON, DFCS_BUTTONPUSH | nStateID);
		}

		HBRUSH hb = CreateSolidBrush(color.ToRGB());
		RECT2 rt;
		SetRect(&rt, 0, 0, 4, 4);

		color.A(255.0f);
		ulong bits[16];
		//c.A(columnAlpha/255.0f);
		for(int i=0 ; i<16 ; i++)
		{
			bits[i] = color.value;
		}

		HBITMAP hBitmap = CreateBitmap(4, 4, 1, 32, bits);
		HGDIOBJ hOldBitmap = SelectObject(m_hOverlayDC, hBitmap);
		//FillRect(m_hOverlayDC, &rt, hb);
		DeleteObject(hb);


		//if(dwRenderStyle & GRRS_SELECTED)
		{
			RECT2 rr;
			rr.left = 0;
			rr.top = 0;
			rr.right = rtRender.right-rtRender.left;
			rr.bottom = rtRender.bottom-rtRender.top;
			//DrawThemeBackground(m_hColumnTheme, hOverlayDC, HP_HEADERITEM, nStateID, &rr, &rr);
			AlphaBlend(m_hdc, rtRender.left, rtRender.top, rtRender.right-rtRender.left, rtRender.bottom-rtRender.top, 
				m_hOverlayDC, 0, 0, 4, 4, blend);
		}

		SelectObject(m_hOverlayDC, hOldBitmap);
		DeleteObject(hBitmap);

		if(renderStyle & GRRS_FOCUSED)
		{
			HGDIOBJ hOld = SelectObject(m_hdc, (HGDIOBJ)m_hFocusedPen);
			HGDIOBJ hOb = SelectObject(m_hdc, GetStockObject(NULL_BRUSH));
			Rectangle(m_hdc, rtRender.left+1, rtRender.top, rtRender.right, rtRender.bottom-1);
			SelectObject(m_hdc, hOb);
			SelectObject(m_hdc, hOld);
		}

		//SetBkMode(m_hdc, nOldBk);
	}

	void GrGridRendererDC::DrawSortGlyph(const GrRect* pRenderRect, GrSort sortType)
	{
		RECT2 rtRender = pRenderRect;

		switch(sortType)
		{
		case GrSort_Up:
			{
				if(m_hColumnTheme != NULL && m_dwMajorVersion >= 6)
				{
					DrawThemeBackground(m_hColumnTheme, m_hdc, HP_HEADERSORTARROW, HSAS_SORTEDUP, &rtRender, &rtRender);
				}
				else
				{
					GrPoint ptCenter = pRenderRect->GetCenter();

					MoveToEx(m_hdc, ptCenter.x, ptCenter.y-2 , 0);
					LineTo(m_hdc, ptCenter.x+1, ptCenter.y-2);

					MoveToEx(m_hdc, ptCenter.x-1, ptCenter.y-1, 0);
					LineTo(m_hdc, ptCenter.x+2, ptCenter.y-1);

					MoveToEx(m_hdc, ptCenter.x-2, ptCenter.y, 0);
					LineTo(m_hdc, ptCenter.x+3, ptCenter.y);

					MoveToEx(m_hdc, ptCenter.x-3, ptCenter.y+1, 0);
					LineTo(m_hdc, ptCenter.x+4, ptCenter.y+1);
				}
			}
			break;
		case GrSort_Down:
			{
				if(m_hColumnTheme != NULL && m_dwMajorVersion >= 6)
				{
					DrawThemeBackground(m_hColumnTheme, m_hdc, HP_HEADERSORTARROW, HSAS_SORTEDDOWN, &rtRender, &rtRender);
				}
				else
				{
					GrPoint ptCenter = pRenderRect->GetCenter();

					MoveToEx(m_hdc, ptCenter.x, ptCenter.y+1 , 0);
					LineTo(m_hdc, ptCenter.x+1, ptCenter.y+1);

					MoveToEx(m_hdc, ptCenter.x-1, ptCenter.y, 0);
					LineTo(m_hdc, ptCenter.x+2, ptCenter.y);

					MoveToEx(m_hdc, ptCenter.x-2, ptCenter.y-1, 0);
					LineTo(m_hdc, ptCenter.x+3, ptCenter.y-1);

					MoveToEx(m_hdc, ptCenter.x-3, ptCenter.y-2, 0);
					LineTo(m_hdc, ptCenter.x+4, ptCenter.y-2);
				}
			}
			break;
		case GrSort_None:
			break;
		}
	}

	void GrGridRendererDC::DrawCellLine(int x1, int y1, int x2, int y2)
	{
		HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)m_hSelRectPen);
		MoveToEx(m_hdc, x1, y1, NULL);
		LineTo(m_hdc, x2, y2);
		SelectObject(m_hdc, hOldPen);
	}

	void GrGridRendererDC::DrawRectangle(const GrRect* pRenderRect, GrColor color, int nWidth)
	{
		HPEN hPen		= CreatePen(PS_SOLID, nWidth, color.ToRGB());
		HPEN hOldPen	= (HPEN)SelectObject(m_hdc, hPen);
		// left
		MoveToEx(m_hdc,	pRenderRect->left,		pRenderRect->top, NULL);
		LineTo(m_hdc,	pRenderRect->left,		pRenderRect->bottom-1);
		// top
		MoveToEx(m_hdc,	pRenderRect->left,		pRenderRect->top, NULL);
		LineTo(m_hdc,	pRenderRect->right,		pRenderRect->top);
		// right
		MoveToEx(m_hdc,	pRenderRect->right-1,	pRenderRect->top, NULL);
		LineTo(m_hdc,	pRenderRect->right-1,	pRenderRect->bottom-1);
		// bottom
		MoveToEx(m_hdc,	pRenderRect->left,		pRenderRect->bottom-1, NULL);
		LineTo(m_hdc,	pRenderRect->right,		pRenderRect->bottom-1);
		SelectObject(m_hdc, hOldPen);
		DeleteObject(hPen);
	}

	void GrGridRendererDC::FillRectangle(GrRect rect, GrColor color)
	{
		RECT2 rt = rect;
		HBRUSH hBrush = CreateSolidBrush(color.ToRGB());
		FillRect(m_hdc, &rt, hBrush);
		DeleteObject(hBrush);
	}

	void GrGridRendererDC::DrawLine(int x1, int y1, int x2, int y2, GrColor color)
	{
		HBRUSH hBrush = CreateSolidBrush(color.ToRGB());
		MoveToEx(m_hdc, x1, y1, NULL);
		LineTo(m_hdc, x2, y2);
		DeleteObject(hBrush);
	}

	void GrGridRendererDC::DrawSizingLine(int x1, int y1, int x2, int y2)
	{
		//if(x1 < ps.rcPaint.left || x2 > ps.rcPaint.right)
		//	int wqer=0;
		HPEN hOldPen;
		hOldPen = (HPEN)SelectObject(m_hdc, (HGDIOBJ)m_hSizingPen);
		MoveToEx(m_hdc, x1, y1, NULL);
		LineTo(m_hdc, x2, y2);
		SelectObject(m_hdc, (HGDIOBJ)hOldPen);
	}

	void GrGridRendererDC::DrawSplitterMovingLine(int x1, int y1, int x2, int y2)
	{
		int n = SetROP2(m_hdc, R2_MERGEPEN);
		//HBRUSH hBrush = CreateHatchBrush(HS_CROSS, RGB(0,0,0));
		//HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)hBrush);


		DWORD b[4] = {0x00ffffff, 0, 0, 0x00ffffff};
		HBITMAP hBitmap = CreateBitmap(2, 2, 32, 1, b);
		HBRUSH hBrush = CreatePatternBrush(hBitmap);

		//MoveToEx(m_hdc, x1, y1, NULL);
		//LineTo(m_hdc, x2, y2);
		RECT2 rt;
		rt.left = x1;
		rt.top = y1;
		rt.right = x2;
		rt.bottom = y2;
		FillRect(m_hdc, &rt, hBrush);

		//SelectObject(m_hdc, hOldPen);
		DeleteObject(hBrush);
		SetROP2(m_hdc, n);
	}

	void GrGridRendererDC::DrawDropDown(const GrRect* pRenderRect, int nState)
	{
		RECT2 rtRender = pRenderRect;

		if(m_hDropDownTheme)
		{
			switch(nState)
			{
			case GrState_Normal:
				DrawThemeBackground(m_hDropDownTheme, m_hdc, CP_DROPDOWNBUTTON, CBXS_NORMAL, &rtRender, &rtRender);
				break;
			case GrState_MouseOver:
				DrawThemeBackground(m_hDropDownTheme, m_hdc, CP_DROPDOWNBUTTON, CBXS_HOT, &rtRender, &rtRender);
				break;
			case GrState_Pressed:
				DrawThemeBackground(m_hDropDownTheme, m_hdc, CP_DROPDOWNBUTTON, CBXS_PRESSED, &rtRender, &rtRender);
				break;
			}
		}
		else
		{
			uint flag = 0;
			switch(nState)
			{
			case GrState_Normal:
				break;
			case GrState_MouseOver:
				flag = DFCS_HOT;
				break;
			case GrState_Pressed:
				flag = DFCS_PUSHED;
				break;
			}

			DrawFrameControl(m_hdc, &rtRender, DFC_SCROLL, DFCS_SCROLLDOWN | flag);
		}
	}

	void GrGridRendererDC::DrawModal(const GrRect* pRenderRect, int nState)
	{
		RECT2 rtRender = pRenderRect;

		switch(nState)
		{
		case GrState_Normal:
			DrawThemeBackground(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_NORMAL, &rtRender, &rtRender);
			break;
		case GrState_MouseOver:
			DrawThemeBackground(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_HOT, &rtRender, &rtRender);
			break;
		case GrState_Pressed:
			DrawThemeBackground(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_PRESSED, &rtRender, &rtRender);
			break;
		}

		DrawThemeText(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_NORMAL, L"...", 3, DT_CENTER|DT_BOTTOM, 0, &rtRender);
	}

	void GrGridRendererDC::SetTransform(GrPoint pt)
	{
		m_ptTransform = pt;
	}

	GrFontDC::GrFontDC(void* pFontHandle) : GrFont(pFontHandle)
	{
		LOGFONTW logfont;
		GetObjectW(pFontHandle, sizeof(logfont), &logfont);

		m_strFontName = GrFontHandleToKey(pFontHandle);

		wchar_t strPath[MAX_PATH];
		SHGetSpecialFolderPathW(NULL, strPath, CSIDL_LOCAL_APPDATA, FALSE);
		wchar_t strTotalPath[MAX_PATH];
		wsprintf(strTotalPath, L"%s\\NtreevSoft", strPath);
		CreateDirectoryW(strTotalPath, NULL);
		wsprintf(strTotalPath, L"%s\\NtreevSoft\\GridControl", strPath);
		CreateDirectoryW(strTotalPath, NULL);
		wsprintf(strTotalPath, L"%s\\NtreevSoft\\GridControl\\%s", strPath, m_strFontName.c_str());

		FILE* pFile = NULL;
		_wfopen_s(&pFile, strTotalPath, L"rb");

		if(pFile == NULL)
		{
			wchar_t s[2] = {0,0};
			SIZE sz;
			HDC hdc = CreateCompatibleDC(NULL);
			HGDIOBJ hOldFont = SelectObject(hdc, (HGDIOBJ)pFontHandle);
			for(wchar_t i=0 ; i<0xffff ; i++)
			{
				s[0] = i;
				GetTextExtentPoint32W(hdc, s, 1, &sz);
				m_nCharWidth[i]	= sz.cx;
				if(i == L'a')
					m_nFontHeight = sz.cy;
			}
			SelectObject(hdc, hOldFont);
			DeleteDC(hdc);

			_wfopen_s(&pFile, strTotalPath, L"wb");
			fwrite(&m_nFontHeight, sizeof(int), 1, pFile);
			fwrite(m_nCharWidth, sizeof(int), 0xffff, pFile);
		}
		else
		{
			fread(&m_nFontHeight, sizeof(int), 1, pFile);
			fread(m_nCharWidth, sizeof(int), 0xffff, pFile);
		}

		m_hFont = (HFONT)pFontHandle;

		fclose(pFile);
	}

	GrFontDC::~GrFontDC()
	{

	}

#ifdef _MANAGED
	System::Drawing::Font^ GrFontDC::ToManaged() const
	{
		LOGFONTW logfont;
		GetObjectW(m_hFont, sizeof(logfont), &logfont);




		//return gcnew System::Drawing::Font(
		return nullptr;
	}
#endif

	int GrFontDC::GetStringWidth(const wchar_t* str) const 
	{
		int n=0;
		while(*str != 0)
		{
			n += GetCharacterWidth(*str);
			str++;
		}
		return n;
	}
} /*namespace DeviceContext*/