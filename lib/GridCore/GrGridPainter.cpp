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

const GrStyle GrStyle::DefaultStyle(true);
GrFont* const GrFont::Empty = nullptr;

GrStyle::GrStyle()
{
    ForeColor = GrColor::Black;
    BackColor = GrColor::White;
	LineColor = GrColor::DefaultLineColor;
    Font = GrFont::GetDefaultFont();

    SelectedForeColor = GrColor::White;
    SelectedBackColor = GrColor::CornflowerBlue;

    FocusedForeColor = GrColor::White;
    FocusedBackColor = GrColor::LightSkyBlue;

	ColumnForeColor = GrColor::Empty;
	ColumnBackColor = GrColor::Empty;
	ColumnLineColor = GrColor::Empty;
	ColumnFont = nullptr;

	RowForeColor = GrColor::Empty;
	RowBackColor = GrColor::Empty;
	RowLineColor = GrColor::Empty;
	RowFont = nullptr;

    CaptionForeColor = GrColor::Empty;
	CaptionBackColor = GrColor::Empty;
	CaptionLineColor = GrColor::Empty;
	CaptionFont = nullptr;

    GroupPanelForeColor = GrColor::Empty;
	GroupPanelBackColor = GrColor::Empty;
	GroupPanelLineColor = GrColor::Empty;
	GroupPanelFont = nullptr;

    RowHighlightLineColor = GrColor::Black;
    RowHighlightFillColor = GrColor::LightCyan;

    //GroupBackColors.push_back(GrColor::LightSalmon);
    //GroupBackColors.push_back(GrColor::PaleGreen);
    //GroupBackColors.push_back(GrColor::SkyBlue);
    //GroupBackColors.push_back(GrColor::LemonChiffon);
    //GroupBackColors.push_back(GrColor::Plum);
    //GroupBackColors.push_back(GrColor::Bisque);
    //GroupBackColors.push_back(GrColor::Honeydew);
}

GrStyle::GrStyle(bool /*defaultStyle*/)
{
    ForeColor = GrColor::Black;
    BackColor = GrColor::White;
	LineColor = GrColor::DefaultLineColor;
    Font = GrFont::GetDefaultFont();

    SelectedForeColor = GrColor::White;
    SelectedBackColor = GrColor::CornflowerBlue;

    FocusedForeColor = GrColor::White;
    FocusedBackColor = GrColor::LightSkyBlue;

	ColumnForeColor = GrColor::Black;
	ColumnBackColor = GrColor::White;
	ColumnLineColor = GrColor::DefaultLineColor;
	ColumnFont = GrFont::GetDefaultFont();

	RowForeColor = GrColor::Black;
	RowBackColor = GrColor::White;
	RowLineColor = GrColor::DefaultLineColor;
	RowFont = GrFont::GetDefaultFont();

    CaptionForeColor = GrColor::Black;
	CaptionBackColor = GrColor::White;
	CaptionLineColor = GrColor::DefaultLineColor;
	CaptionFont = GrFont::GetDefaultFont();

    GroupPanelForeColor = GrColor::Black;
	GroupPanelBackColor = GrColor::White;
	GroupPanelLineColor = GrColor::DefaultLineColor;
	GroupPanelFont = GrFont::GetDefaultFont();

    RowHighlightLineColor = GrColor::Black;
    RowHighlightFillColor = GrColor::LightCyan;

    GroupForeColors.push_back(GrColor::Black);

    GroupBackColors.push_back(GrColor::LightSalmon);
    GroupBackColors.push_back(GrColor::PaleGreen);
    GroupBackColors.push_back(GrColor::SkyBlue);
    GroupBackColors.push_back(GrColor::LemonChiffon);
    GroupBackColors.push_back(GrColor::Plum);
    GroupBackColors.push_back(GrColor::Bisque);
    GroupBackColors.push_back(GrColor::Honeydew);

    GroupLineColors.push_back(GrColor::DefaultLineColor);
    GroupFonts.push_back(GrFont::GetDefaultFont());
}

GrColor GrStyle::GetColumnForeColor() const
{
    if(this->ColumnForeColor == GrColor::Empty)
        return this->ForeColor;
    return this->ColumnForeColor;
}

GrColor GrStyle::GetColumnBackColor() const
{
    if(this->ColumnBackColor == GrColor::Empty)
        return this->BackColor;
    return this->ColumnBackColor;
}

GrColor GrStyle::GetColumnLineColor() const
{
    if(this->ColumnLineColor == GrColor::Empty)
        return this->LineColor;
    return this->ColumnLineColor;
}

GrFont* GrStyle::GetColumnFont() const
{
    if(this->ColumnFont == nullptr)
        return this->Font;
    return this->ColumnFont;
}

GrColor GrStyle::GetRowForeColor() const
{
    if(this->RowForeColor == GrColor::Empty)
        return this->ForeColor;
    return this->RowForeColor;
}

GrColor GrStyle::GetRowBackColor() const
{
    if(this->RowBackColor == GrColor::Empty)
        return this->BackColor;
    return this->RowBackColor;
}

GrColor GrStyle::GetRowLineColor() const
{
    if(this->RowLineColor == GrColor::Empty)
        return this->LineColor;
    return this->RowLineColor;
}

GrFont* GrStyle::GetRowFont() const
{
    if(this->RowFont == nullptr)
        return this->Font;
    return this->RowFont;
}

GrColor GrStyle::GetCaptionForeColor() const
{
    if(this->CaptionForeColor == GrColor::Empty)
        return this->ForeColor;
    return this->CaptionForeColor;
}

GrColor GrStyle::GetCaptionBackColor() const
{
    if(this->CaptionBackColor == GrColor::Empty)
        return this->BackColor;
    return this->CaptionBackColor;
}

GrColor GrStyle::GetCaptionLineColor() const
{
    if(this->CaptionLineColor == GrColor::Empty)
        return this->LineColor;
    return this->CaptionLineColor;
}

GrFont* GrStyle::GetCaptionFont() const
{
    if(this->CaptionFont == nullptr)
        return this->Font;
    return this->CaptionFont;
}

GrColor GrStyle::GetGroupPanelForeColor() const
{
    if(this->GroupPanelForeColor == GrColor::Empty)
        return this->ForeColor;
    return this->GroupPanelForeColor;
}

GrColor GrStyle::GetGroupPanelBackColor() const
{
    if(this->GroupPanelBackColor == GrColor::Empty)
        return this->BackColor;
    return this->GroupPanelBackColor;
}

GrColor GrStyle::GetGroupPanelLineColor() const
{
    if(this->GroupPanelLineColor == GrColor::Empty)
        return this->LineColor;
    return this->GroupPanelLineColor;
}

GrFont* GrStyle::GetGroupPanelFont() const
{
    if(this->GroupPanelFont == nullptr)
        return this->Font;
    return this->GroupPanelFont;
}

GrColor GrStyle::GetItemForeColor(uint index) const
{
    return this->ItemForeColors[index % ItemForeColors.size()];
}

GrColor GrStyle::GetItemBackColor(uint index) const
{
    return this->ItemBackColors[index % ItemBackColors.size()];
}

GrColor GrStyle::GetItemLineColor(uint index) const
{
    return this->ItemLineColors[index % ItemLineColors.size()];
}

GrFont* GrStyle::GetItemFont(uint index) const
{
    return this->ItemFonts[index % ItemFonts.size()];
}

GrColor GrStyle::GetGroupForeColor(uint index) const
{
    const GrColors& colors = 
        this->GroupForeColors.size() == 0 ? 
        GrStyle::DefaultStyle.GroupForeColors : this->GroupForeColors;

    return colors[index % colors.size()];
}

GrColor GrStyle::GetGroupBackColor(uint index) const
{
    const GrColors& colors = 
        this->GroupBackColors.size() == 0 ? 
        GrStyle::DefaultStyle.GroupBackColors : this->GroupBackColors;

    return colors[index % colors.size()];
}

GrColor GrStyle::GetGroupLineColor(uint index) const
{
   const GrColors& colors = 
        this->GroupLineColors.size() == 0 ? 
        GrStyle::DefaultStyle.GroupLineColors : this->GroupLineColors;

    return colors[index % colors.size()];
}

GrFont* GrStyle::GetGroupFont(uint index) const
{
    const GrFonts& fonts = 
        this->GroupFonts.size() == 0 ? 
        GrStyle::DefaultStyle.GroupFonts : this->GroupFonts;

    return fonts[index % fonts.size()];
}