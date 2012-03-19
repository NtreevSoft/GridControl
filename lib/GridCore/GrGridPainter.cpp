//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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


#include "GrGridPainter.h"

const GrStyle GrStyle::DefaultStyle;

GrStyle::GrStyle()
{
    CellForeColor = GrColor::Black;
    CellBackColor = GrColor::White;
    CellFont = 0;//GrFontManager::GetDefaultFont();

    SelectedForeColor = GrColor::White;
    SelectedBackColor = GrColor::CornflowerBlue;

    FocusedForeColor = GrColor::White;
    FocusedBackColor = GrColor::LightSkyBlue;

    RowHighlightForeColor = GrColor::Black;
    RowHighlightBackColor = GrColor::LightCyan;

    GroupBackColors.push_back(GrColor::Red);
    GroupBackColors.push_back(GrColor::Green);
    GroupBackColors.push_back(GrColor::Blue);
    GroupBackColors.push_back(GrColor::Yellow);
    GroupBackColors.push_back(GrColor::Purple);
    GroupBackColors.push_back(GrColor::SkyBlue);
}

GrColor GrStyle::GetCellForeColor() const
{
    if(CellForeColor == GrColor::Empty)
        return DefaultStyle.CellForeColor;
    return CellForeColor;
}

GrColor GrStyle::GetCellBackColor() const
{
    if(CellBackColor == GrColor::Empty)
        return DefaultStyle.CellBackColor;
    return CellBackColor;
}

GrFont* GrStyle::GetCellFont() const
{
    if(CellFont == NULL)
        return DefaultStyle.CellFont;
    return CellFont;
}

GrColor GrStyle::GetRowItemForeColor(uint index)
{
    if(RowItemForeColors.size() == 0)
        return CellForeColor;
    return RowItemForeColors[index % RowItemForeColors.size()];
}

GrColor GrStyle::GetRowItemBackColor(uint index)
{
    if(RowItemBackColors.size() == 0)
        return CellBackColor;

    return RowItemBackColors[index % RowItemBackColors.size()];
}

GrFont* GrStyle::GetRowItemFont(uint index)
{
    if(RowItemFonts.size() == 0)
        return NULL;
    return RowItemFonts[index % RowItemFonts.size()];
}

GrColor GrStyle::GetGroupForeColor(uint index)
{
    if(GroupForeColors.size() == 0)
        return CellForeColor;
    return GroupForeColors[index % GroupForeColors.size()];
}

GrColor GrStyle::GetGroupBackColor(uint index)
{
    if(GroupBackColors.size() == 0)
        return CellBackColor;

    return GroupBackColors[index % GroupBackColors.size()];
}

GrFont* GrStyle::GetGroupFont(uint index)
{
    if(GroupFonts.size() == 0)
        return NULL;
    return GroupFonts[index % GroupFonts.size()];
}