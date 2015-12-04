//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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
    GrPaintStyle_BottomLine = 0x00010000,
    GrPaintStyle_RightLine = 0x00020000,
    GrPaintStyle_TopLine = 0x00040000,
    GrPaintStyle_LeftLine = 0x00080000,

    GrPaintStyle_Default = GrPaintStyle_RightLine | GrPaintStyle_BottomLine,
};

class GrFont
{
public:
    virtual int GetCharacterWidth(wchar_t w) const = 0;
    virtual int GetHeight() const = 0;
    virtual int GetInternalLeading() const = 0;
    virtual int GetExternalLeading() const = 0;
    virtual int GetStringWidth(const std::wstring& text) const = 0;

    int GetStringWidth(const wchar_t* w, size_t length) const
    {
        int width = 0;
        for(size_t i=0 ; i<length ; i++)
        {
            width += GetCharacterWidth(w[i]);
        }
        return width;
    }

#ifdef _MANAGED
    gcroot<System::Drawing::Font^> ManagedRef;
	static System::Drawing::Font^ ToManaged(GrFont* pFont);
	static GrFont* FromManaged(System::Drawing::Font^ font);
#endif

	static GrFont* GetDefaultFont();

	static GrFont* const Empty;
};

typedef std::vector<GrColor> GrColors;
typedef std::vector<GrFont*> GrFonts;

class GrStyle
{
private:
    GrStyle(bool defaultStyle);
public:
    GrStyle();

public:
    GrColor ForeColor;
    GrColor BackColor;
	GrColor LineColor;
	GrPadding Padding;
    GrFont* Font;

    GrColor SelectedForeColor;
    GrColor SelectedBackColor;

    GrColor FocusedForeColor;
    GrColor FocusedBackColor;

	GrColor ColumnForeColor;
	GrColor ColumnBackColor;
	GrColor ColumnLineColor;
	GrFont* ColumnFont;

	GrColor RowForeColor;
	GrColor RowBackColor;
	GrColor RowLineColor;
	GrFont* RowFont;

    GrColor CaptionForeColor;
	GrColor CaptionBackColor;
	GrColor CaptionLineColor;
	GrFont* CaptionFont;

    GrColor GroupPanelForeColor;
	GrColor GroupPanelBackColor;
	GrColor GroupPanelLineColor;
	GrFont* GroupPanelFont;
	
    GrColor RowHighlightLineColor;
    GrColor RowHighlightFillColor;

    GrColors ItemBackColors;
    GrColors ItemForeColors;
	GrColors ItemLineColors;
    GrFonts ItemFonts;

    GrColors GroupBackColors;
    GrColors GroupForeColors;
    GrColors GroupLineColors;
    GrFonts GroupFonts;

    GrColor GetColumnForeColor() const;
    GrColor GetColumnBackColor() const;
    GrColor GetColumnLineColor() const;
    GrFont* GetColumnFont() const;

    GrColor GetRowForeColor() const;
    GrColor GetRowBackColor() const;
    GrColor GetRowLineColor() const;
    GrFont* GetRowFont() const;

    GrColor GetCaptionForeColor() const;
    GrColor GetCaptionBackColor() const;
    GrColor GetCaptionLineColor() const;
    GrFont* GetCaptionFont() const;

    GrColor GetGroupPanelForeColor() const;
    GrColor GetGroupPanelBackColor() const;
    GrColor GetGroupPanelLineColor() const;
    GrFont* GetGroupPanelFont() const;

    GrColor GetItemForeColor(uint index) const;
    GrColor GetItemBackColor(uint index)const;
	GrColor GetItemLineColor(uint index)const;
    GrFont* GetItemFont(uint index)const;

    GrColor GetGroupForeColor(uint index)const;
    GrColor GetGroupBackColor(uint index)const;
    GrColor GetGroupLineColor(uint index)const;
    GrFont* GetGroupFont(uint index)const;

    static const GrStyle Default;
};



//GrFont* GetDefaultGridCoreFont();

class GrGridPainter
{
public:
    virtual void BeginPaint(void* painterDevice) = 0;

    virtual void DrawRowSplitter(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& fillColor) = 0;
    virtual void DrawDropDown(const GrRect& paintRect, GrControlState state, const GrRect* pClipRect = nullptr) = 0;
    virtual void DrawModal(const GrRect& paintRect, GrControlState state, const GrRect* pClipRect = nullptr) = 0;
    virtual void DrawExpander(GrFlag paintStyle, const GrRect& paintRect, GrControlState state, bool opened, const GrColor& foreColor, const GrColor& backColor) = 0;
    virtual void DrawSortGlyph(const GrRect& paintRect, GrSort sortType) = 0;

    virtual void DrawColumn(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& fillColor, const GrRect* pClipRect = nullptr) = 0;
    virtual void DrawRow(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& fillColor, const GrRect* pClipRect = nullptr) = 0;
    virtual void DrawItem(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& fillColor, const GrRect* pClipRect = nullptr) = 0;
    virtual void DrawGroupHeader(GrFlag paintStyle, const GrRect& paintRect, const GrColor& lineColor, const GrColor& fillColor, const GrRect* pClipRect = nullptr) = 0;

    virtual void DrawText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color , const GrRect* pClipRect = nullptr) = 0;
    virtual void DrawColumnText(GrFont* pFont, const wchar_t* text, int textLength, const GrRect& paintRect, const GrColor& color , const GrRect* pClipRect = nullptr) = 0;

    virtual void DrawResizingLine(int x1, int y1, int x2, int y2) = 0;
    virtual void DrawSplitterMovingLine(const GrRect& paintRect) = 0;

    virtual void DrawRectangle(const GrRect& rect, const GrColor& color) = 0;
    virtual void DrawLine(int x1, int y1, int x2, int y2, const GrColor& color) = 0;

    virtual void FillRectangle(const GrRect& rect, const GrColor& color, const GrRect* pClipRect = nullptr) = 0;

    virtual void EndPaint() = 0;
    virtual void SetFont(GrFont* pFont) = 0;

    virtual void* GetDevice() = 0;
    virtual void ReleaseDevice(void* painterDevice) = 0;
};