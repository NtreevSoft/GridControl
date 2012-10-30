//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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


#include "GrGridPainterDC.h"
#include "GrGridCell.h"
#include <Shlobj.h>
#include <map>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "UxTheme.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "shell32.lib")

#define _RGB(_color) (RGB(_color.r, _color.g, _color.b))



GrGridPainter* CreateGridPainterDC(void* windowHandle)
{
	return new GrGridPainterDC((HWND)windowHandle);
}

//void* GetGridFontHandle(GrFont* pFont)
//{
// return ((GrFontDC*)pFont)->GetFontHandle();
//}

struct RECT2 : RECT
{
	RECT2() {}
	RECT2(const GrRect* rt) 
	{
		left = rt->left;
		top = rt->top;
		right = rt->right;
		bottom = rt->bottom;
	}

	RECT2(const GrRect& rt) 
	{
		left = rt.left;
		top = rt.top;
		right = rt.right;
		bottom = rt.bottom;
	}

    RECT2(const GrRect& rt, const GrRect* pClip) 
	{
		left = rt.left;
		top = rt.top;
		right = rt.right;
		bottom = rt.bottom;

        if(pClip != nullptr)
        {
            if(right > pClip->right)
                right = pClip->right;
            if(bottom > pClip->bottom)
                bottom = pClip->bottom;
        }
	}

	void operator = (const GrRect& rt)
	{
		left = rt.left;
		top = rt.top;
		right = rt.right;
		bottom = rt.bottom;
	}

	void operator = (const GrRect* rt)
	{
		left = rt->left;
		top = rt->top;
		right = rt->right;
		bottom = rt->bottom;
	}

	void Offset(int x, int y)
	{
		left += x;
		top += y;
		right += x;
		bottom += y;
	}

	void Offset(GrPoint pt)
	{
		Offset(pt.x, pt.y);
	}
};

GrGridPainterDC::GrGridPainterDC(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hColumnTheme = OpenThemeData(m_hWnd, L"HEADER");
	m_hDropDownTheme = OpenThemeData(m_hWnd, L"COMBOBOX");
	m_hModalTheme = OpenThemeData(m_hWnd, L"BUTTON");
	m_hTreeTheme = OpenThemeData(m_hWnd, L"TREEVIEW");
	m_hTooltip = OpenThemeData(m_hWnd, L"TOOLTIP");

	OSVERSIONINFOW osInfo;
	memset(&osInfo, 0, sizeof(OSVERSIONINFOW));
	osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
	GetVersionExW(&osInfo);

	m_dwMajorVersion = osInfo.dwMajorVersion;

	m_hOverlayBmp = CreateBitmap(4, 4, 1, 32, nullptr);
	m_hColorBmp = CreateBitmap(4, 4, 1, 32, nullptr);

	m_hMouseOverBrush = CreateSolidBrush(RGB(240,240,240));
	m_hSelectedBrush = CreateSolidBrush(RGB(47,106,197));
	m_hSelectedMouseOverBrush = CreateSolidBrush(RGB(71,130,221));

	m_hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//m_hSelRectPen = CreatePen(PS_SOLID, 1, RGB(208,215,229));

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID; 
	lb.lbColor = RGB(125,125,125); 
	lb.lbHatch = 0; 
	DWORD Style[] = {1,1};
	m_hSizingPen = ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_FLAT, 1, &lb, 2, Style);

	lb.lbColor = RGB(50,50,50);
	m_hFocusedPen = ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_FLAT, 1, &lb, 2, Style);

	lb.lbStyle = BS_HATCHED; 
	lb.lbColor = RGB(0,0,0); 
	lb.lbHatch = HS_CROSS; 
	m_hSplitterMovingPen= ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_FLAT, GrColumnSplitter::DefaultSplitterWidth, &lb, 2, Style);
}

GrGridPainterDC::~GrGridPainterDC()
{
	DeleteDC(m_hOverlayDC);
	DeleteObject(m_hSplitterMovingPen);
	DeleteObject(m_hSizingPen);
	//DeleteObject(m_hSelRectPen);
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

void GrGridPainterDC::BeginPaint(void* _hdc)
{
	m_hdc = (HDC)_hdc;

	m_hOverlayDC = CreateCompatibleDC(m_hdc);
	m_hOverlayBmpOld = (HBITMAP)SelectObject(m_hOverlayDC, (HGDIOBJ)m_hOverlayBmp);

	m_hFont = nullptr;
	m_hOldFont = nullptr;

	SetBkMode(m_hdc, TRANSPARENT);

	m_ptTransform = GrPoint(0,0);
}

void GrGridPainterDC::EndPaint()
{
	SelectObject(m_hdc, (HGDIOBJ)m_hOldFont);
	SelectObject(m_hOverlayDC, (HGDIOBJ)m_hOverlayBmpOld);

	m_hdc = nullptr;
}

void GrGridPainterDC::DrawRowSplitter(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& backColor)
{
    DrawItem(paintStyle, paintRect, lineColor, backColor);
	//RECT2 rt = paintRect;

	//HBRUSH hb = CreateSolidBrush(_RGB(backColor));
	//FillRect(m_hdc, &rt, hb);
	//DeleteObject(hb);

	//HPEN hPen = CreatePen(PS_SOLID, 1, _RGB(lineColor));
	//HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)hPen);
	//MoveToEx(m_hdc, rt.left, rt.bottom-1, nullptr);
	//LineTo(m_hdc, rt.right, rt.bottom-1);
	//SelectObject(m_hdc, hOldPen);
	//DeleteObject(hPen);
}

void GrGridPainterDC::DrawExpander(GrFlag paintStyle, const GrRect& paintRect, GrControlState state, bool opened, const GrColor& foreColor, const GrColor& backColor)
{
	RECT2 rt = paintRect;

    HBRUSH br = CreateSolidBrush(_RGB(backColor));
	FillRect(m_hdc, &rt, br);
    DeleteObject(br);

	if(opened == true)
	{
		if(m_hTreeTheme != nullptr && m_dwMajorVersion >= 6)
		{
			DrawThemeBackground(m_hTreeTheme, m_hdc, TVP_GLYPH, GLPS_OPENED, &rt, &rt);
		}
		else
		{
			GrPoint ptCenter = paintRect.GetCenter();

			rt.left = ptCenter.x-4;
			rt.top = ptCenter.y-4;
			rt.right = ptCenter.x+5;
			rt.bottom = ptCenter.y+5;

			Rectangle(m_hdc, rt.left, rt.top, rt.right, rt.bottom);
			MoveToEx(m_hdc, rt.left+2, ptCenter.y, 0);
			LineTo(m_hdc, rt.right-2, ptCenter.y);
		}
	}
	else
	{
		if(m_hTreeTheme != nullptr && m_dwMajorVersion >= 6)
		{
			DrawThemeBackground(m_hTreeTheme, m_hdc, TVP_GLYPH, GLPS_CLOSED, &rt, &rt);
		}
		else
		{
			GrPoint ptCenter = paintRect.GetCenter();

			rt.left = ptCenter.x-4;
			rt.top = ptCenter.y-4;
			rt.right = ptCenter.x+5;
			rt.bottom = ptCenter.y+5;

			Rectangle(m_hdc, rt.left, rt.top, rt.right, rt.bottom);
			MoveToEx(m_hdc, rt.left+2, ptCenter.y, 0);
			LineTo(m_hdc, rt.right-2, ptCenter.y);

			MoveToEx(m_hdc, ptCenter.x, rt.top+2, 0);
			LineTo(m_hdc, ptCenter.x, rt.bottom-2);
		}
	}

    HPEN hPen = CreatePen(PS_SOLID, 1, _RGB(foreColor));
	HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)hPen);

    MoveToEx(m_hdc, rt.right-1, rt.top, nullptr);
    LineTo(m_hdc, rt.right-1, rt.bottom);

    if(paintStyle.Has(GrPaintStyle_BottomLine) == true)
	{
		MoveToEx(m_hdc, rt.left, rt.bottom-1, nullptr);
		LineTo(m_hdc, rt.right, rt.bottom-1);
	}
	
	SelectObject(m_hdc, hOldPen);
	DeleteObject(hPen);
}

void GrGridPainterDC::DrawItem(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& backColor, const GrRect* pClipRect)
{
	RECT2 rt(paintRect, pClipRect);

    if(paintStyle.Has(GrPaintStyle_Focused) == true)
    {
        GrColor backColor1 = backColor;
		GrColor backColor2 = RGBHSL::ModifyBrightness(backColor, 0.9f);
        TRIVERTEX vert[2];
        GRADIENT_RECT grect;

        vert[0].x = rt.left;
        vert[0].y = rt.top;

        vert[0].Red = backColor1.r << 8;
        vert[0].Green = backColor1.g << 8;
        vert[0].Blue = backColor1.b << 8;
        vert[0].Alpha = backColor1.a << 8;

        vert[1].x = rt.right;
        vert[1].y = rt.bottom;

        vert[1].Red = backColor2.r << 8;
        vert[1].Green = backColor2.g << 8;
        vert[1].Blue = backColor2.b << 8;
        vert[1].Alpha = backColor2.a << 8;

        grect.UpperLeft = 0;
        grect.LowerRight = 1;

        GradientFill(m_hdc, vert, 2, &grect, 1, GRADIENT_FILL_RECT_V);
    }
    else
    {
        HBRUSH hb = CreateSolidBrush(_RGB(backColor));
        FillRect(m_hdc, &rt, hb);
        DeleteObject(hb);
    }

	HPEN hPen = CreatePen(PS_SOLID, 1, _RGB(lineColor));
	HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)hPen);

	if(paintStyle.Has(GrPaintStyle_RightLine) == true)
	{
		MoveToEx(m_hdc, rt.right-1, rt.top, nullptr);
		LineTo(m_hdc, rt.right-1, rt.bottom-1);
	}

	if(paintStyle.Has(GrPaintStyle_BottomLine) == true)
	{
		MoveToEx(m_hdc, rt.left, rt.bottom-1, nullptr);
		LineTo(m_hdc, rt.right, rt.bottom-1);
	}
	SelectObject(m_hdc, hOldPen);
	DeleteObject(hPen);

	if(paintStyle.Has(GrPaintStyle_Focused) == true)
	{
		HGDIOBJ hOld = SelectObject(m_hdc, (HGDIOBJ)m_hFocusedPen);
		HGDIOBJ hOb = SelectObject(m_hdc, GetStockObject(NULL_BRUSH));
		Rectangle(m_hdc, rt.left, rt.top, rt.right-1, rt.bottom-1);
		SelectObject(m_hdc, hOb);
		SelectObject(m_hdc, hOld);
	}
}

void GrGridPainterDC::DrawGroupHeader(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& fillColor, const GrRect* pClipRect)
{
    DrawGradientHeader(paintStyle, paintRect, lineColor, fillColor, pClipRect, true);
}

#pragma push_macro("DrawText")
#undef DrawText
void GrGridPainterDC::DrawText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect)
#pragma pop_macro("DrawText")
{
	RECT2 rtPaint = paintRect;
	rtPaint.Offset(m_ptTransform);

	SetFont(pFont);
	SetTextColor(m_hdc, RGB(color.r,color.g,color.b));
	if(pClipRect)
	{
		RECT2 rtClip = pClipRect;
		rtClip.Offset(m_ptTransform);
		ExtTextOutW(m_hdc, rtPaint.left, rtPaint.top, ETO_CLIPPED, &rtClip, text, textLength, 0);
	}
	else
	{
		ExtTextOutW(m_hdc, rtPaint.left, rtPaint.top, 0, nullptr, text, textLength, 0);
	}
}

void GrGridPainterDC::DrawColumnText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect)
{
	RECT2 rtPaint(paintRect, pClipRect);
	rtPaint.Offset(m_ptTransform);

	SetFont(pFont);
	SetTextColor(m_hdc, RGB(color.r,color.g,color.b));

	if(pClipRect != nullptr)
	{
		if(rtPaint.right > pClipRect->right)
			rtPaint.right = pClipRect->right;
		if(rtPaint.bottom > pClipRect->bottom)
			rtPaint.bottom = pClipRect->bottom;
	}

	DrawTextW(m_hdc, text, textLength, &rtPaint, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);
}

void GrGridPainterDC::DrawTooltip(const GrRect& paintRect)
{
	RECT2 rtPaint = paintRect;
	DrawThemeBackground(m_hTooltip, m_hdc, TTP_BALLOON, TTCS_NORMAL, &rtPaint, &rtPaint);
}

void GrGridPainterDC::DrawColumn(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& backColor, const GrRect* pClipRect)
{
	//DrawThemeHeader(paintStyle, paintRect, lineColor, backColor, pClipRect);
	DrawGradientHeader(paintStyle, paintRect, lineColor, backColor, pClipRect);
}

void GrGridPainterDC::DrawRow(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& backColor, const GrRect* pClipRect)
{
	//DrawThemeHeader(paintStyle, paintRect, lineColor, backColor, pClipRect);
	DrawGradientHeader(paintStyle, paintRect, lineColor, backColor, pClipRect);
}

void GrGridPainterDC::DrawThemeHeader(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& backColor, const GrRect* pClipRect)
{
	RECT2 rtPaint(paintRect), rtClip(paintRect);
	rtPaint.Offset(m_ptTransform);
	rtClip.Offset(m_ptTransform);
	rtPaint.bottom++;
	GrColor paintColor = backColor;

	BLENDFUNCTION blend;
	blend.BlendFlags = 0;
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 15;

	if(pClipRect != nullptr)
	{
		rtClip.right = min(rtClip.right, pClipRect->right);
		rtClip.bottom = min(rtClip.bottom, pClipRect->bottom);
	}

	if(m_hColumnTheme != nullptr)
	{
		int nStateID = HIS_NORMAL;
		if(paintStyle.Has(GrPaintStyle_Mouseover) == true)
			nStateID = HIS_HOT;
		if(paintStyle.Has(GrPaintStyle_Pressed) == true)
			nStateID = HIS_PRESSED;

		DrawThemeBackground(m_hColumnTheme, m_hdc, HP_HEADERITEM, nStateID, &rtPaint, &rtClip);
	}
	else
	{
		uint nStateID = 0;
		if(paintStyle.Has(GrPaintStyle_Mouseover) == true)
			nStateID = DFCS_HOT;
		if(paintStyle.Has(GrPaintStyle_Pressed) == true)
			nStateID = DFCS_PUSHED;
		DrawFrameControl(m_hdc, &rtPaint, DFC_BUTTON, DFCS_BUTTONPUSH | nStateID);
	}

	HBRUSH hb = CreateSolidBrush(RGB(paintColor.r,paintColor.g,paintColor.b));
	RECT2 rt;
	SetRect(&rt, 0, 0, 4, 4);

	paintColor.A(255.0f);
	ulong bits[16];
	//c.A(columnAlpha/255.0f);
	for(int i=0 ; i<16 ; i++)
	{
		bits[i] = paintColor.ToARGB();
	}

	HBITMAP hBitmap = CreateBitmap(4, 4, 1, 32, bits);
	HGDIOBJ hOldBitmap = SelectObject(m_hOverlayDC, hBitmap);
	//FillRect(m_hOverlayDC, &rt, hb);
	DeleteObject(hb);


	//if(dwPaintStyle & GrPaintStyle_Selected)
	{
		RECT2 alphaRect;
		if(pClipRect == nullptr)
			alphaRect = rtPaint;
		else
			alphaRect = rtClip;
		AlphaBlend(m_hdc, alphaRect.left, alphaRect.top, alphaRect.right-alphaRect.left, alphaRect.bottom-alphaRect.top, 
			m_hOverlayDC, 0, 0, 4, 4, blend);
	}

	SelectObject(m_hOverlayDC, hOldBitmap);
	DeleteObject(hBitmap);

	if(paintStyle.Has(GrPaintStyle_Focused) == true)
	{
		HGDIOBJ hOld = SelectObject(m_hdc, (HGDIOBJ)m_hFocusedPen);
		HGDIOBJ hOb = SelectObject(m_hdc, GetStockObject(NULL_BRUSH));
		Rectangle(m_hdc, rtPaint.left, rtPaint.top, rtPaint.right, rtPaint.bottom-2);
		SelectObject(m_hdc, hOb);
		SelectObject(m_hdc, hOld);
	}

	//SetBkMode(m_hdc, nOldBk);
}

void GrGridPainterDC::DrawGradientHeader(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& backColor, const GrRect* pClipRect, bool focusedLine)
{
	GrColor backColor1, backColor2;

	if(paintStyle.Has(GrPaintStyle_Pressed) == true)
	{
		backColor1 = RGBHSL::ModifyBrightness(backColor, 0.9f);
		backColor2 = RGBHSL::ModifyBrightness(backColor1, 0.9f);
	}
	else if(paintStyle.Has(GrPaintStyle_Mouseover) == true)
	{
		backColor1 = backColor;
		backColor2 = RGBHSL::ModifyBrightness(backColor, 0.95f);
	}
	else if(paintStyle.Has(GrPaintStyle_Selected) == true)
	{
		backColor1 = RGBHSL::ModifyBrightness(backColor, 0.95f);
		backColor2 = RGBHSL::ModifyBrightness(backColor1, 0.9f);
	}
	else if(paintStyle.Has(GrPaintStyle_Focused) == true)
	{
		backColor1 = RGBHSL::ModifyBrightness(backColor, 0.925f);
		backColor2 = RGBHSL::ModifyBrightness(backColor1, 0.9f);
	}
	else
	{
		backColor1 = backColor;
		backColor2 = RGBHSL::ModifyBrightness(backColor, 0.9f);
	}

	RECT2 rt = paintRect;

	if(pClipRect != nullptr)
	{
		if(rt.right > pClipRect->right)
			rt.right = pClipRect->right;
		if(rt.bottom > pClipRect->bottom)
			rt.bottom = pClipRect->bottom;
	}

	TRIVERTEX vert[2];
	GRADIENT_RECT grect;

	vert[0].x = rt.left;
	vert[0].y = rt.top;

	vert[0].Red = backColor1.r << 8;
	vert[0].Green = backColor1.g << 8;
	vert[0].Blue = backColor1.b << 8;
	vert[0].Alpha = backColor1.a << 8;

	vert[1].x = rt.right;
	vert[1].y = rt.bottom;

	vert[1].Red = backColor2.r << 8;
	vert[1].Green = backColor2.g << 8;
	vert[1].Blue = backColor2.b << 8;
	vert[1].Alpha = backColor2.a << 8;

	grect.UpperLeft = 0;
	grect.LowerRight = 1;


	//HBRUSH hb = CreateSolidBrush(_RGB(backColor));
	GradientFill(m_hdc, vert, 2, &grect, 1, GRADIENT_FILL_RECT_V);
	//DeleteObject(hb);

	HPEN hPen = CreatePen(PS_SOLID, 1, _RGB(lineColor));
	HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)hPen);

	if(paintStyle.Has(GrPaintStyle_RightLine) == true)
	{
		MoveToEx(m_hdc, rt.right-1, rt.top, nullptr);
		LineTo(m_hdc, rt.right-1, rt.bottom-1);
	}

	if(paintStyle.Has(GrPaintStyle_BottomLine) == true)
	{
		MoveToEx(m_hdc, rt.left, rt.bottom-1, nullptr);
		LineTo(m_hdc, rt.right, rt.bottom-1);
	}
	SelectObject(m_hdc, hOldPen);
	DeleteObject(hPen);

	if(focusedLine == true && paintStyle.Has(GrPaintStyle_Focused) == true)
	{
	    HGDIOBJ hOld = SelectObject(m_hdc, (HGDIOBJ)m_hFocusedPen);
	    HGDIOBJ hOb = SelectObject(m_hdc, GetStockObject(NULL_BRUSH));
	    Rectangle(m_hdc, rt.left, rt.top, rt.right-1, rt.bottom-1);
	    SelectObject(m_hdc, hOb);
	    SelectObject(m_hdc, hOld);
	}
}

void GrGridPainterDC::DrawSortGlyph(const GrRect& paintRect, GrSort sortType)
{
	RECT2 rtPaint = paintRect;

	switch(sortType)
	{
	case GrSort_Up:
		{
			if(m_hColumnTheme != nullptr && m_dwMajorVersion >= 6)
			{
				DrawThemeBackground(m_hColumnTheme, m_hdc, HP_HEADERSORTARROW, HSAS_SORTEDUP, &rtPaint, &rtPaint);
			}
			else
			{
				GrPoint ptCenter = paintRect.GetCenter();

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
			if(m_hColumnTheme != nullptr && m_dwMajorVersion >= 6)
			{
				DrawThemeBackground(m_hColumnTheme, m_hdc, HP_HEADERSORTARROW, HSAS_SORTEDDOWN, &rtPaint, &rtPaint);
			}
			else
			{
				GrPoint ptCenter = paintRect.GetCenter();

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

//void GrGridPainterDC::DrawCellLine(int x1, int y1, int x2, int y2)
//{
//    HGDIOBJ hOldPen = SelectObject(m_hdc, (HGDIOBJ)m_hSelRectPen);
//    MoveToEx(m_hdc, x1, y1, nullptr);
//    LineTo(m_hdc, x2, y2);
//    SelectObject(m_hdc, hOldPen);
//}

void GrGridPainterDC::DrawRectangle(const GrRect& rect, const GrColor& color)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, _RGB(color));
	HPEN hOldPen = (HPEN)SelectObject(m_hdc, hPen);
	// left
	MoveToEx(m_hdc, rect.left, rect.top, nullptr);
	LineTo(m_hdc, rect.left, rect.bottom-1);
	// top
	MoveToEx(m_hdc, rect.left, rect.top, nullptr);
	LineTo(m_hdc, rect.right, rect.top);
	// right
	MoveToEx(m_hdc, rect.right-1, rect.top, nullptr);
	LineTo(m_hdc, rect.right-1, rect.bottom-1);
	// bottom
	MoveToEx(m_hdc, rect.left, rect.bottom-1, nullptr);
	LineTo(m_hdc, rect.right, rect.bottom-1);
	SelectObject(m_hdc, hOldPen);
	DeleteObject(hPen);
}

void GrGridPainterDC::FillRectangle(const GrRect& rect, const GrColor& color)
{
	RECT2 rt = rect;
	HBRUSH hBrush = CreateSolidBrush(_RGB(color));
	FillRect(m_hdc, &rt, hBrush);
	DeleteObject(hBrush);
}

void GrGridPainterDC::DrawLine(int x1, int y1, int x2, int y2, const GrColor& color)
{
	HBRUSH hBrush = CreateSolidBrush(_RGB(color));
	MoveToEx(m_hdc, x1, y1, nullptr);
	LineTo(m_hdc, x2, y2);
	DeleteObject(hBrush);
}

void GrGridPainterDC::DrawResizingLine(int x1, int y1, int x2, int y2)
{
	//if(x1 < ps.rcPaint.left || x2 > ps.rcPaint.right)
	// int wqer=0;
	HPEN hOldPen;
	hOldPen = (HPEN)SelectObject(m_hdc, (HGDIOBJ)m_hSizingPen);
	MoveToEx(m_hdc, x1, y1, nullptr);
	LineTo(m_hdc, x2, y2);
	SelectObject(m_hdc, (HGDIOBJ)hOldPen);
}

void GrGridPainterDC::DrawSplitterMovingLine(const GrRect& paintRect)
{
	int n = SetROP2(m_hdc, R2_MERGEPEN);

	DWORD b[4] = {0x00ffffff, 0, 0, 0x00ffffff};
	HBITMAP hBitmap = CreateBitmap(2, 2, 32, 1, b);
	HBRUSH hBrush = CreatePatternBrush(hBitmap);

	RECT2 rt = paintRect;
	FillRect(m_hdc, &rt, hBrush);

	DeleteObject(hBrush);
	SetROP2(m_hdc, n);
}

void GrGridPainterDC::DrawDropDown(const GrRect& paintRect, GrControlState state)
{
	RECT2 rtPaint = paintRect;

	if(m_hDropDownTheme)
	{
		switch(state)
		{
		case GrControlState_Normal:
			DrawThemeBackground(m_hDropDownTheme, m_hdc, CP_DROPDOWNBUTTON, CBXS_NORMAL, &rtPaint, &rtPaint);
			break;
		case GrControlState_Hot:
			DrawThemeBackground(m_hDropDownTheme, m_hdc, CP_DROPDOWNBUTTON, CBXS_HOT, &rtPaint, &rtPaint);
			break;
		case GrControlState_Pressed:
			DrawThemeBackground(m_hDropDownTheme, m_hdc, CP_DROPDOWNBUTTON, CBXS_PRESSED, &rtPaint, &rtPaint);
			break;
		}
	}
	else
	{
		uint flag = 0;
		switch(state)
		{
		case GrControlState_Normal:
			break;
		case GrControlState_Hot:
			flag = DFCS_HOT;
			break;
		case GrControlState_Pressed:
			flag = DFCS_PUSHED;
			break;
		}

		DrawFrameControl(m_hdc, &rtPaint, DFC_SCROLL, DFCS_SCROLLDOWN | flag);
	}
}

void GrGridPainterDC::DrawModal(const GrRect& paintRect, GrControlState state)
{
	RECT2 rtPaint = paintRect;

	switch(state)
	{
	case GrControlState_Normal:
		DrawThemeBackground(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_NORMAL, &rtPaint, &rtPaint);
		break;
	case GrControlState_Hot:
		DrawThemeBackground(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_HOT, &rtPaint, &rtPaint);
		break;
	case GrControlState_Pressed:
		DrawThemeBackground(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_PRESSED, &rtPaint, &rtPaint);
		break;
	}

	rtPaint.bottom = rtPaint.top + (int)((float)(rtPaint.bottom - rtPaint.top) * (4.5f/5.0f));
	DrawThemeText(m_hModalTheme, m_hdc, BP_PUSHBUTTON, PBS_NORMAL, L"...", 3, DT_CENTER|DT_BOTTOM|DT_SINGLELINE, 0, &rtPaint);
}

void GrGridPainterDC::SetFont(GrFont* pFont)
{
	GrFontDC* pFontDC = (GrFontDC*)pFont;

	HFONT hCurFont = (HFONT)pFontDC->GetFontHandle();

	if(m_hFont != hCurFont)
	{
		HFONT hOldFont = (HFONT)SelectObject(m_hdc, (HGDIOBJ)hCurFont);
		if(m_hOldFont == nullptr)
			m_hOldFont = hOldFont;
		m_hFont = hCurFont;
	}
}

GrFontDC::GrFontDC(void* fontHandle)
{
	m_hFont = (HFONT)fontHandle;
	m_hdc = CreateCompatibleDC(nullptr);

	SelectObject(m_hdc, m_hFont);

	TEXTMETRICW textMetric;
	GetTextMetricsW(m_hdc, &textMetric);
	//wchar_t s[2] = {L'a',0};
	//SIZE sz;
	//GetTextExtentPoint32W(m_hdc, s, 1, &sz);
	m_height = (int)textMetric.tmHeight;
	m_internalLeading = (int)textMetric.tmInternalLeading;
	m_externalLeading = (int)textMetric.tmExternalLeading;

	memset(m_characterWidth, -1, sizeof(int) * 0xffff);
}

GrFontDC::~GrFontDC()
{
	DeleteDC(m_hdc);
}

int GrFontDC::GetCharacterWidth(wchar_t w) const
{
	SIZE size;
	wchar_t s[2] = {w,0};
	size.cx = 0;
	size.cy = 0;

	if(m_characterWidth[w] == -1)
	{
		GetTextExtentPoint32W(m_hdc, s, 1, &size);
		m_characterWidth[w] = size.cx;
	}
	return m_characterWidth[w];
}

int GrFontDC::GetStringWidth(const std::wstring& text) const
{
	int width = 0;
	for(uint i=0 ; i<text.length() ; i++)
	{
		wchar_t w = text[i];
		width += GetCharacterWidth(w);
	}

	return width;
}

