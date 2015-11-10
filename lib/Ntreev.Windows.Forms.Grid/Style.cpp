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


#include "StdAfx.h"
#include "Style.h"
#include "NativeClasses.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    System::Drawing::Font^ FontConverter::StoT(GrFont* pFont)
    {
        return GrFont::ToManaged(pFont);
    }

    GrFont* FontConverter::TtoS(System::Drawing::Font^ font)
    {
        return GrFont::FromManaged(font);
    }

    Style::Style()
    {
        m_pNativeStyle = new GrStyle();
        m_cellForeColors = gcnew StyleColorCollection(&m_pNativeStyle->ItemForeColors, GrStyle::Default.ForeColor);
        m_cellBackColors = gcnew StyleColorCollection(&m_pNativeStyle->ItemBackColors, GrStyle::Default.BackColor);
        m_cellLineColors = gcnew StyleColorCollection(&m_pNativeStyle->ItemLineColors, GrStyle::Default.LineColor);
        m_cellFonts = gcnew StyleFontCollection(&m_pNativeStyle->ItemFonts);

        m_groupForeColors = gcnew StyleColorCollection(&m_pNativeStyle->GroupForeColors, GrStyle::Default.ForeColor);
        m_groupBackColors = gcnew StyleColorCollection(&m_pNativeStyle->GroupBackColors, GrStyle::Default.BackColor);
        m_groupLineColors = gcnew StyleColorCollection(&m_pNativeStyle->GroupLineColors, GrStyle::Default.LineColor);
        m_groupFonts = gcnew StyleFontCollection(&m_pNativeStyle->GroupFonts);
    }

    Style::~Style()
    {
        Lock(this);

        this->GridControl = nullptr;

        if((m_site != nullptr) && (m_site->Container != nullptr))
        {
            m_site->Container->Remove(this);
        }

        this->Disposed(this, System::EventArgs::Empty);
    }

    void Style::ResetSelectedForeColor()
    {
        this->SelectedForeColor = GrStyle::Default.SelectedForeColor;
    }

    void Style::ResetSelectedBackColor()
    {
        this->SelectedBackColor = GrStyle::Default.SelectedBackColor;
    }

    void Style::ResetFocusedForeColor()
    {
        this->FocusedForeColor = GrStyle::Default.FocusedForeColor;
    }

    void Style::ResetFocusedBackColor()
    {
        this->FocusedBackColor = GrStyle::Default.FocusedBackColor;
    }

    void Style::ResetForeColor()
    {
        this->ForeColor = GrStyle::Default.ForeColor;
    }

    void Style::ResetBackColor()
    {
        this->BackColor = GrStyle::Default.BackColor;
    }

    void Style::ResetLineColor()
    {
        this->LineColor = GrStyle::Default.LineColor;
    }

    void Style::ResetFont()
    {
        this->Font = GrFont::ToManaged(GrStyle::Default.Font);
    }

    void Style::ResetColumnForeColor()
    {
        this->ColumnForeColor = System::Drawing::Color::Empty;
    }

    void Style::ResetColumnBackColor()
    {
        this->ColumnBackColor = System::Drawing::Color::Empty;
    }

    void Style::ResetColumnLineColor()
    {
        this->ColumnLineColor = System::Drawing::Color::Empty;
    }

    void Style::ResetColumnFont()
    {
        this->ColumnFont = nullptr;
    }

    void Style::ResetRowForeColor()
    {
        this->RowForeColor = System::Drawing::Color::Empty;
    }

    void Style::ResetRowBackColor()
    {
        this->RowBackColor = System::Drawing::Color::Empty;
    }

    void Style::ResetRowLineColor()
    {
        this->RowLineColor = System::Drawing::Color::Empty;
    }

    void Style::ResetRowFont()
    {
        this->RowFont = nullptr;
    }

    void Style::ResetCaptionForeColor()
    {
        this->CaptionForeColor = System::Drawing::Color::Empty;
    }

    void Style::ResetCaptionBackColor()
    {
        this->CaptionBackColor = System::Drawing::Color::Empty;
    }

    void Style::ResetCaptionLineColor()
    {
        this->CaptionLineColor = System::Drawing::Color::Empty;
    }

    void Style::ResetCaptionFont()
    {
        this->CaptionFont = nullptr;
    }

    void Style::ResetGroupPanelForeColor()
    {
        this->GroupPanelForeColor = System::Drawing::Color::Empty;
    }

    void Style::ResetGroupPanelBackColor()
    {
        this->GroupPanelBackColor = System::Drawing::Color::Empty;
    }

    void Style::ResetGroupPanelLineColor()
    {
        this->GroupPanelLineColor = System::Drawing::Color::Empty;
    }

    void Style::ResetGroupPanelFont()
    {
        this->GroupPanelFont = nullptr;
    }

    void Style::ResetRowHighlightLineColor()
    {
        this->RowHighlightLineColor = GrStyle::Default.RowHighlightLineColor;
    }

    void Style::ResetRowHighlightFillColor()
    {
        this->RowHighlightFillColor = GrStyle::Default.RowHighlightFillColor;
    }

    void Style::ResetCellForeColors()
    {
        m_cellForeColors->Clear();
    }

    void Style::ResetCellBackColors()
    {
        m_cellBackColors->Clear();
    }

    void Style::ResetCellLineColors()
    {
        m_cellLineColors->Clear();
    }

    void Style::ResetCellFonts()
    {
        m_cellFonts->Clear();
    }

    void Style::ResetGroupForeColors()
    {
        m_groupForeColors->Clear();
    }

    void Style::ResetGroupBackColors()
    {
        m_groupBackColors->Clear();
    }

    void Style::ResetGroupLineColors()
    {
        m_groupLineColors->Clear();
    }

    void Style::ResetGroupFonts()
    {
        m_groupFonts->Clear();
    }

    System::Drawing::Color Style::SelectedForeColor::get()
    {
        return m_pNativeStyle->SelectedForeColor;
    }

    void Style::SelectedForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->SelectedForeColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::SelectedBackColor::get()
    {
        return m_pNativeStyle->SelectedBackColor;
    }

    void Style::SelectedBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->SelectedBackColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::FocusedForeColor::get()
    {
        return m_pNativeStyle->FocusedForeColor;
    }

    void Style::FocusedForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->FocusedForeColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::FocusedBackColor::get()
    {
        return m_pNativeStyle->FocusedBackColor;
    }

    void Style::FocusedBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->FocusedBackColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::ForeColor::get()
    {
        return m_pNativeStyle->ForeColor;
    }

    void Style::ForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->ForeColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::BackColor::get()
    {
        return m_pNativeStyle->BackColor;
    }

    void Style::BackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->BackColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::LineColor::get()
    {
        return m_pNativeStyle->LineColor;
    }

    void Style::LineColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->LineColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Font^ Style::Font::get()
    {
        return GrFont::ToManaged(m_pNativeStyle->Font);
    }

    void Style::Font::set(System::Drawing::Font^ value)
    {
        m_pNativeStyle->Font = GrFont::FromManaged(value);
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::ColumnForeColor::get()
    {
        return m_pNativeStyle->GetColumnForeColor();
    }

    void Style::ColumnForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->ColumnForeColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::ColumnBackColor::get()
    {
        return m_pNativeStyle->GetColumnBackColor();
    }

    void Style::ColumnBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->ColumnBackColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::ColumnLineColor::get()
    {
        return m_pNativeStyle->GetColumnLineColor();
    }

    void Style::ColumnLineColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->ColumnLineColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Font^ Style::ColumnFont::get()
    {
        return GrFont::ToManaged(m_pNativeStyle->GetColumnFont());
    }

    void Style::ColumnFont::set(System::Drawing::Font^ value)
    {
        m_pNativeStyle->ColumnFont = GrFont::FromManaged(value);
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::RowForeColor::get()
    {
        return m_pNativeStyle->GetRowForeColor();
    }

    void Style::RowForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->RowForeColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::RowBackColor::get()
    {
        return m_pNativeStyle->GetRowBackColor();
    }

    void Style::RowBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->RowBackColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::RowLineColor::get()
    {
        return m_pNativeStyle->GetRowLineColor();
    }

    void Style::RowLineColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->RowLineColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Font^ Style::RowFont::get()
    {
        return GrFont::ToManaged(m_pNativeStyle->GetRowFont());
    }

    void Style::RowFont::set(System::Drawing::Font^ value)
    {
        m_pNativeStyle->RowFont = GrFont::FromManaged(value);
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::CaptionForeColor::get()
    {
        return m_pNativeStyle->GetCaptionForeColor();
    }

    void Style::CaptionForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->CaptionForeColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::CaptionBackColor::get()
    {
        return m_pNativeStyle->GetCaptionBackColor();
    }

    void Style::CaptionBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->CaptionBackColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::CaptionLineColor::get()
    {
        return m_pNativeStyle->GetCaptionLineColor();
    }

    void Style::CaptionLineColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->CaptionLineColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Font^ Style::CaptionFont::get()
    {
        return GrFont::ToManaged(m_pNativeStyle->GetCaptionFont());
    }

    void Style::CaptionFont::set(System::Drawing::Font^ value)
    {
        m_pNativeStyle->CaptionFont = GrFont::FromManaged(value);
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::GroupPanelForeColor::get()
    {
        return m_pNativeStyle->GetGroupPanelForeColor();
    }

    void Style::GroupPanelForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->GroupPanelForeColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::GroupPanelBackColor::get()
    {
        return m_pNativeStyle->GetGroupPanelBackColor();
    }

    void Style::GroupPanelBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->GroupPanelBackColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::GroupPanelLineColor::get()
    {
        return m_pNativeStyle->GetGroupPanelLineColor();
    }

    void Style::GroupPanelLineColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->GroupPanelLineColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Font^ Style::GroupPanelFont::get()
    {
        return GrFont::ToManaged(m_pNativeStyle->GetGroupPanelFont());
    }

    void Style::GroupPanelFont::set(System::Drawing::Font^ value)
    {
        m_pNativeStyle->GroupPanelFont = GrFont::FromManaged(value);
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    StyleColorCollection^ Style::CellBackColors::get()
    {
        return m_cellBackColors;
    }

    StyleColorCollection^ Style::CellForeColors::get()
    {
        return m_cellForeColors;
    }

    StyleColorCollection^ Style::CellLineColors::get()
    {
        return m_cellLineColors;
    }

    StyleFontCollection^ Style::CellFonts::get()
    {
        return m_cellFonts;
    }

    StyleColorCollection^ Style::GroupForeColors::get()
    {
        return m_groupForeColors;
    }

    StyleColorCollection^ Style::GroupBackColors::get()
    {
        return m_groupBackColors;
    }

    StyleColorCollection^ Style::GroupLineColors::get()
    {
        return m_groupLineColors;
    }

    StyleFontCollection^ Style::GroupFonts::get()
    {
        return m_groupFonts;
    }

    System::Drawing::Color Style::RowHighlightLineColor::get()
    {
        return m_pNativeStyle->RowHighlightLineColor; 
    }

    void Style::RowHighlightLineColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->RowHighlightLineColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    System::Drawing::Color Style::RowHighlightFillColor::get()
    {
        return m_pNativeStyle->RowHighlightFillColor; 
    }

    void Style::RowHighlightFillColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->RowHighlightFillColor = value;
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
    }

    GrStyle* Style::NativeStyle::get()
    {
        //m_pNativeStyle->ItemFonts.clear();

        //for each(System::Drawing::Color value in m_cellBackColors)
        //{
        //	// m_pNativeStyle->RowItemBackColors.push_back(value);
        //}

        //for each(System::Drawing::Color value in m_rowCellForeColors)
        //{
        // m_pNativeStyle->RowItemForeColors.push_back(value);
        //}

        //for each(System::Drawing::Font^ value in m_rowCellFonts)
        //{
        //	GrFont* pFont = GrFont::FromManaged(value);
        //	m_pNativeStyle->RowItemFonts.push_back(pFont);
        //}
        return m_pNativeStyle;
    }

    void Style::OnGridControlAttachedInternal()
    {
        GridObject::OnGridControlAttachedInternal();
        m_cellForeColors->GridControl = this->GridControl;
        m_cellBackColors->GridControl = this->GridControl;
        m_cellLineColors->GridControl = this->GridControl;
        m_cellFonts->GridControl = this->GridControl;

        m_groupForeColors->GridControl = this->GridControl;
        m_groupBackColors->GridControl = this->GridControl;
        m_groupLineColors->GridControl = this->GridControl;
        m_groupFonts->GridControl = this->GridControl;
    }

    void Style::OnGridControlDetachedInternal()
    {
        m_cellForeColors->GridControl = nullptr;
        m_cellBackColors->GridControl = nullptr;
        m_cellLineColors->GridControl = nullptr;
        m_cellFonts->GridControl = nullptr;

        m_groupForeColors->GridControl = nullptr;
        m_groupBackColors->GridControl = nullptr;
        m_groupLineColors->GridControl = nullptr;
        m_groupFonts->GridControl = nullptr;

        GridObject::OnGridControlDetachedInternal();
    }

    bool Style::ShouldSerializeForeColor()
    {
        return m_pNativeStyle->ForeColor != GrStyle::Default.ForeColor;
    }

    bool Style::ShouldSerializeBackColor()
    {
        return m_pNativeStyle->BackColor != GrStyle::Default.BackColor;
    }

    bool Style::ShouldSerializeLineColor()
    {
        return m_pNativeStyle->LineColor != GrStyle::Default.LineColor;
    }

    bool Style::ShouldSerializeFont()
    {
        return m_pNativeStyle->Font != GrStyle::Default.Font;
    }

    bool Style::ShouldSerializeSelectedForeColor()
    {
        return m_pNativeStyle->SelectedForeColor != GrStyle::Default.SelectedForeColor;
    }

    bool Style::ShouldSerializeSelectedBackColor()
    {
        return m_pNativeStyle->SelectedBackColor != GrStyle::Default.SelectedBackColor;
    }

    bool Style::ShouldSerializeFocusedForeColor()
    {
        return m_pNativeStyle->FocusedForeColor != GrStyle::Default.FocusedForeColor;
    }

    bool Style::ShouldSerializeFocusedBackColor()
    {
        return m_pNativeStyle->FocusedBackColor != GrStyle::Default.FocusedBackColor;
    }

    bool Style::ShouldSerializeColumnForeColor()
    {
        return m_pNativeStyle->ColumnForeColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeColumnBackColor()
    {
        return m_pNativeStyle->ColumnBackColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeColumnLineColor()
    {
        return m_pNativeStyle->ColumnLineColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeColumnFont()
    {
        return m_pNativeStyle->ColumnFont != nullptr;
    }

    bool Style::ShouldSerializeRowForeColor()
    {
        return m_pNativeStyle->RowForeColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeRowBackColor()
    {
        return m_pNativeStyle->RowBackColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeRowLineColor()
    {
        return m_pNativeStyle->RowLineColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeRowFont()
    {
        return m_pNativeStyle->RowFont != nullptr;
    }

    bool Style::ShouldSerializeCaptionForeColor()
    {
        return m_pNativeStyle->CaptionForeColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeCaptionBackColor()
    {
        return m_pNativeStyle->CaptionBackColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeCaptionLineColor()
    {
        return m_pNativeStyle->CaptionLineColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeCaptionFont()
    {
        return m_pNativeStyle->CaptionFont != nullptr;
    }

    bool Style::ShouldSerializeGroupPanelForeColor()
    {
        return m_pNativeStyle->GroupPanelForeColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeGroupPanelBackColor()
    {
        return m_pNativeStyle->GroupPanelBackColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeGroupPanelLineColor()
    {
        return m_pNativeStyle->GroupPanelLineColor != GrColor::Empty;
    }

    bool Style::ShouldSerializeGroupPanelFont()
    {
        return m_pNativeStyle->GroupPanelFont != nullptr;
    }

    bool Style::ShouldSerializeRowHighlightLineColor()
    {
        return m_pNativeStyle->RowHighlightLineColor != GrStyle::Default.RowHighlightLineColor;
    }

    bool Style::ShouldSerializeRowHighlightFillColor()
    {
        return m_pNativeStyle->RowHighlightFillColor != GrStyle::Default.RowHighlightFillColor;
    }

    bool Style::ShouldSerializeCellForeColors()
    {
        return m_cellForeColors->Count != 0;
    }

    bool Style::ShouldSerializeCellBackColors()
    {
        return m_cellBackColors->Count != 0;
    }

    bool Style::ShouldSerializeCellLineColors()
    {
        return m_cellBackColors->Count != 0;
    }

    bool Style::ShouldSerializeCellFonts()
    {
        return m_cellFonts->Count != 0;
    }

    bool Style::ShouldSerializeGroupForeColors()
    {
        return m_groupForeColors->Count != 0;
    }

    bool Style::ShouldSerializeGroupBackColors()
    {
        return m_groupBackColors->Count != 0;
    }

    bool Style::ShouldSerializeGroupLineColors()
    {
        return m_groupLineColors->Count != 0;
    }

    bool Style::ShouldSerializeGroupFonts()
    {
        return m_groupFonts->Count != 0;
    }

    System::Object^ Style::GetService(System::Type^ serviceType)
    {
        if(m_site != nullptr)
            return m_site->GetService(serviceType);
        return nullptr;
    }

    void Style::Site::set(System::ComponentModel::ISite^ value)
    {
        m_site = value;
    }

    System::ComponentModel::ISite^ Style::Site::get()
    {
        return m_site;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/