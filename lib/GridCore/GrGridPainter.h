//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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

#include "GrGridType.h"

#ifdef _MANAGED
#include <vcclr.h>
#endif

enum GrPaintStyle
{
    GrPaintStyle_Focused = 0x00000001,
    GrPaintStyle_Mouseover = 0x00000002,
    GrPaintStyle_Selected = 0x00000004,
    GrPaintStyle_Pressed = 0x00000008,
    GrPaintStyle_NoBottomLine = 0x00010000,
    GrPaintStyle_NoRightLine = 0x00020000,
    GrPaintStyle_TopLine = 0x00040000,
    GrPaintStyle_LeftLine = 0x00080000,
};

class GrFont;

typedef std::vector<GrColor> GrColors;
typedef std::vector<GrFont*> GrFonts;

class GrStyle
{
public:
    GrStyle();

public:
    GrColor CellForeColor;
    GrColor CellBackColor;
    GrFont* CellFont;

    GrColor SelectedForeColor;
    GrColor SelectedBackColor;

    GrColor FocusedForeColor;
    GrColor FocusedBackColor;

    GrColor RowHighlightForeColor;
    GrColor RowHighlightBackColor;

    GrColors RowItemBackColors;
    GrColors RowItemForeColors;
    GrFonts RowItemFonts;

    GrColors GroupBackColors;
    GrColors GroupForeColors;
    GrFonts GroupFonts;

    GrColor GetCellForeColor() const;
    GrColor GetCellBackColor() const;
    GrFont* GetCellFont() const;

    GrColor GetRowItemForeColor(uint index);
    GrColor GetRowItemBackColor(uint index);
    GrFont* GetRowItemFont(uint index);

    GrColor GetGroupForeColor(uint index);
    GrColor GetGroupBackColor(uint index);
    GrFont* GetGroupFont(uint index);

    static const GrStyle DefaultStyle;
};

class GrFont
{
public:
    virtual int GetCharacterWidth(wchar_t w) const = 0;
    virtual int GetHeight() const = 0;
    virtual int GetInternalLeading() const = 0;
    virtual int GetExternalLeading() const = 0;
    virtual int GetStringWidth(const std::wstring& text) const = 0;

#ifdef _MANAGED
    gcroot<System::Drawing::Font^> ManagedRef;
#endif
};

class GrGridPainter
{
public:
    virtual void BeginPaint(void* painterDevice) = 0;

    virtual void DrawRowSplitter(const GrRect& paintRect) = 0;
    virtual void DrawDropDown(const GrRect& paintRect, GrControlState state) = 0;
    virtual void DrawModal(const GrRect& paintRect, GrControlState state) = 0;
    virtual void DrawTreeGlyph(const GrRect& paintRect, bool opened) = 0;
    virtual void DrawSortGlyph(const GrRect& paintRect, GrSort sortType) = 0;

    virtual void DrawColumn(GrFlag paintStyle, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect = NULL) = 0;
    virtual void DrawRow(GrFlag paintStyle, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect = NULL) = 0;
    virtual void DrawItem(GrFlag paintStyle, const GrRect& paintRect, const GrColor& color, const GrRect* pClipRect = NULL) = 0;

    virtual void DrawText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color , const GrRect* pClipRect = NULL) = 0;
    virtual void DrawColumnText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color , const GrRect* pClipRect = NULL) = 0;

    virtual void DrawResizingLine(int x1, int y1, int x2, int y2) = 0;
    virtual void DrawSplitterMovingLine(const GrRect& paintRect) = 0;

    virtual void DrawRectangle(const GrRect& rect, const GrColor& color) = 0;
    virtual void DrawLine(int x1, int y1, int x2, int y2, const GrColor& color) = 0;

    virtual void FillRectangle(const GrRect& rect, const GrColor& color) = 0;

    virtual void EndPaint() = 0;
    virtual void SetFont(GrFont* pFont) = 0;

    virtual void* GetDevice() = 0;
    virtual void ReleaseDevice(void* painterDevice) = 0;
};