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


#include "GrGridPainter.h"

const GrStyle GrStyle::DefaultStyle;
GrFont* const GrFont::Empty = NULL;

GrStyle::GrStyle()
{
    ForeColor = GrColor::Black;
    BackColor = GrColor::White;
	LineColor = GrColor(208,215,229);
    Font = GrFont::GetDefaultFont();

    SelectedForeColor = GrColor::White;
    SelectedBackColor = GrColor::CornflowerBlue;

    FocusedForeColor = GrColor::White;
    FocusedBackColor = GrColor::LightSkyBlue;

	ColumnForeColor = GrColor::Black;
	ColumnBackColor = GrColor::White;
	ColumnLineColor = GrColor(208,215,229);
	ColumnFont = GrFont::GetDefaultFont();

	RowForeColor = GrColor::Black;
	RowBackColor = GrColor::White;
	RowLineColor = GrColor(208,215,229);
	RowFont = GrFont::GetDefaultFont();

    RowHighlightForeColor = GrColor::Black;
    RowHighlightBackColor = GrColor::LightCyan;

    GroupBackColors.push_back(GrColor::Red);
    GroupBackColors.push_back(GrColor::Green);
    GroupBackColors.push_back(GrColor::Blue);
    GroupBackColors.push_back(GrColor::Yellow);
    GroupBackColors.push_back(GrColor::Purple);
    GroupBackColors.push_back(GrColor::SkyBlue);
}

GrColor GrStyle::GetItemForeColor(uint index) const
{
    return ItemForeColors[index % ItemForeColors.size()];
}

GrColor GrStyle::GetItemBackColor(uint index) const
{
    return ItemBackColors[index % ItemBackColors.size()];
}

GrColor GrStyle::GetItemLineColor(uint index) const
{
    return ItemLineColors[index % ItemLineColors.size()];
}

GrFont* GrStyle::GetItemFont(uint index) const
{
    return ItemFonts[index % ItemFonts.size()];
}

GrColor GrStyle::GetGroupForeColor(uint index) const
{
    return GroupForeColors[index % GroupForeColors.size()];
}

GrColor GrStyle::GetGroupBackColor(uint index) const
{
    return GroupBackColors[index % GroupBackColors.size()];
}

GrFont* GrStyle::GetGroupFont(uint index) const
{
    return GroupFonts[index % GroupFonts.size()];
}