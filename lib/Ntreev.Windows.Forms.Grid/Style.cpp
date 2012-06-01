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
		m_cellForeColors = gcnew StyleColorCollection(&m_pNativeStyle->ItemForeColors);
		m_cellBackColors = gcnew StyleColorCollection(&m_pNativeStyle->ItemBackColors);
		m_cellLineColors = gcnew StyleColorCollection(&m_pNativeStyle->ItemLineColors);
		m_cellFonts = gcnew StyleFontCollection();

		m_groupForeColors = gcnew StyleColorCollection(&m_pNativeStyle->GroupForeColors);
		m_groupBackColors = gcnew StyleColorCollection(&m_pNativeStyle->GroupForeColors);
		m_groupFonts = gcnew StyleFontCollection();
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

	void Style::ResetSelectedForeColor()
	{
		m_pNativeStyle->SelectedForeColor = GrStyle::DefaultStyle.SelectedForeColor;
	}

	void Style::ResetSelectedBackColor()
	{
		m_pNativeStyle->SelectedBackColor = GrStyle::DefaultStyle.SelectedBackColor;
	}

	void Style::ResetFocusedForeColor()
	{
		m_pNativeStyle->FocusedForeColor = GrStyle::DefaultStyle.FocusedForeColor;
	}

	void Style::ResetFocusedBackColor()
	{
		m_pNativeStyle->FocusedBackColor = GrStyle::DefaultStyle.FocusedBackColor;
	}

	void Style::ResetForeColor()
	{
		this->ForeColor = GrStyle::DefaultStyle.ForeColor;
	}

	void Style::ResetBackColor()
	{
		this->BackColor = GrStyle::DefaultStyle.BackColor;
	}

	void Style::ResetLineColor()
	{
		this->LineColor = GrStyle::DefaultStyle.LineColor;
	}

	void Style::ResetFont()
	{
		this->Font = GrFont::ToManaged(GrStyle::DefaultStyle.Font);
	}

	void Style::ResetColumnForeColor()
	{
		m_pNativeStyle->ColumnForeColor = GrStyle::DefaultStyle.ColumnForeColor;
	}

	void Style::ResetColumnBackColor()
	{
		m_pNativeStyle->ColumnBackColor = GrStyle::DefaultStyle.ColumnBackColor;
	}

	void Style::ResetColumnLineColor()
	{
		m_pNativeStyle->ColumnLineColor = GrStyle::DefaultStyle.ColumnLineColor;
	}

	void Style::ResetColumnFont()
	{
		m_pNativeStyle->ColumnFont = GrStyle::DefaultStyle.ColumnFont;
	}

	void Style::ResetRowForeColor()
	{
		m_pNativeStyle->RowForeColor = GrStyle::DefaultStyle.RowForeColor;
	}

	void Style::ResetRowBackColor()
	{
		m_pNativeStyle->RowBackColor = GrStyle::DefaultStyle.RowBackColor;
	}

	void Style::ResetRowLineColor()
	{
		m_pNativeStyle->RowLineColor = GrStyle::DefaultStyle.RowLineColor;
	}

	void Style::ResetRowFont()
	{
		m_pNativeStyle->RowFont = GrStyle::DefaultStyle.RowFont;
	}

	void Style::ResetRowHighlightForeColor()
	{
		m_pNativeStyle->RowHighlightForeColor = GrStyle::DefaultStyle.RowHighlightForeColor;
	}

	void Style::ResetRowHighlightBackColor()
	{
		m_pNativeStyle->RowHighlightBackColor = GrStyle::DefaultStyle.RowHighlightBackColor;
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
		return m_pNativeStyle->ColumnForeColor;
	}

	void Style::ColumnForeColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->ColumnForeColor = value;
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Color Style::ColumnBackColor::get()
	{
		return m_pNativeStyle->ColumnBackColor;
	}

	void Style::ColumnBackColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->ColumnBackColor = value;
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Color Style::ColumnLineColor::get()
	{
		return m_pNativeStyle->ColumnLineColor;
	}

	void Style::ColumnLineColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->ColumnLineColor = value;
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Font^ Style::ColumnFont::get()
	{
		return GrFont::ToManaged(m_pNativeStyle->ColumnFont);
	}

	void Style::ColumnFont::set(System::Drawing::Font^ value)
	{
		m_pNativeStyle->ColumnFont = GrFont::FromManaged(value);
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Color Style::RowForeColor::get()
	{
		return m_pNativeStyle->RowForeColor;
	}

	void Style::RowForeColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->RowForeColor = value;
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Color Style::RowBackColor::get()
	{
		return m_pNativeStyle->RowBackColor;
	}

	void Style::RowBackColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->RowBackColor = value;
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Color Style::RowLineColor::get()
	{
		return m_pNativeStyle->RowLineColor;
	}

	void Style::RowLineColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->RowLineColor = value;
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Font^ Style::RowFont::get()
	{
		return GrFont::ToManaged(m_pNativeStyle->RowFont);
	}

	void Style::RowFont::set(System::Drawing::Font^ value)
	{
		m_pNativeStyle->RowFont = GrFont::FromManaged(value);
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
		return m_cellForeColors;
	}

	StyleFontCollection^ Style::CellFonts::get()
	{
		return m_cellFonts;
	}

	StyleColorCollection^ Style::GroupBackColors::get()
	{
		return m_groupBackColors;
	}

	StyleColorCollection^ Style::GroupForeColors::get()
	{
		return m_groupForeColors;
	}

	StyleFontCollection^ Style::GroupFonts::get()
	{
		return m_groupFonts;
	}

	System::Drawing::Color Style::RowHighlightForeColor::get()
	{
		return m_pNativeStyle->RowHighlightForeColor; 
	}

	void Style::RowHighlightForeColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->RowHighlightForeColor = value;
		if(this->GridControl != nullptr)
			this->GridControl->Invalidate();
	}

	System::Drawing::Color Style::RowHighlightBackColor::get()
	{
		return m_pNativeStyle->RowHighlightBackColor; 
	}

	void Style::RowHighlightBackColor::set(System::Drawing::Color value)
	{
		m_pNativeStyle->RowHighlightBackColor = value;
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
		m_groupFonts->GridControl = nullptr;

		GridObject::OnGridControlDetachedInternal();
	}

	bool Style::ShouldSerializeForeColor()
	{
		return m_pNativeStyle->ForeColor != GrStyle::DefaultStyle.ForeColor;
	}

	bool Style::ShouldSerializeBackColor()
	{
		return m_pNativeStyle->BackColor != GrStyle::DefaultStyle.BackColor;
	}

	bool Style::ShouldSerializeLineColor()
	{
		return m_pNativeStyle->LineColor != GrStyle::DefaultStyle.LineColor;
	}

	bool Style::ShouldSerializeFont()
	{
		return m_pNativeStyle->Font != GrStyle::DefaultStyle.Font;
	}

	bool Style::ShouldSerializeSelectedForeColor()
	{
		return m_pNativeStyle->SelectedForeColor != GrStyle::DefaultStyle.SelectedForeColor;
	}

	bool Style::ShouldSerializeSelectedBackColor()
	{
		return m_pNativeStyle->SelectedBackColor != GrStyle::DefaultStyle.SelectedBackColor;
	}

	bool Style::ShouldSerializeFocusedForeColor()
	{
		return m_pNativeStyle->FocusedForeColor != GrStyle::DefaultStyle.FocusedForeColor;
	}

	bool Style::ShouldSerializeFocusedBackColor()
	{
		return m_pNativeStyle->FocusedBackColor != GrStyle::DefaultStyle.FocusedBackColor;
	}

	bool Style::ShouldSerializeColumnForeColor()
	{
		return m_pNativeStyle->ColumnForeColor != GrStyle::DefaultStyle.ColumnForeColor;
	}

	bool Style::ShouldSerializeColumnBackColor()
	{
		return m_pNativeStyle->ColumnBackColor != GrStyle::DefaultStyle.ColumnBackColor;
	}

	bool Style::ShouldSerializeColumnLineColor()
	{
		return m_pNativeStyle->ColumnLineColor != GrStyle::DefaultStyle.ColumnLineColor;
	}

	bool Style::ShouldSerializeColumnFont()
	{
		return m_pNativeStyle->ColumnFont != GrStyle::DefaultStyle.ColumnFont;
	}

	bool Style::ShouldSerializeRowForeColor()
	{
		return m_pNativeStyle->RowForeColor != GrStyle::DefaultStyle.RowForeColor;
	}

	bool Style::ShouldSerializeRowBackColor()
	{
		return m_pNativeStyle->RowBackColor != GrStyle::DefaultStyle.RowBackColor;
	}

	bool Style::ShouldSerializeRowLineColor()
	{
		return m_pNativeStyle->RowLineColor != GrStyle::DefaultStyle.RowLineColor;
	}

	bool Style::ShouldSerializeRowFont()
	{
		return m_pNativeStyle->RowFont != GrStyle::DefaultStyle.RowFont;
	}

	bool Style::ShouldSerializeRowHighlightForeColor()
	{
		return m_pNativeStyle->RowHighlightForeColor != GrStyle::DefaultStyle.RowHighlightForeColor;
	}

	bool Style::ShouldSerializeRowHighlightBackColor()
	{
		return m_pNativeStyle->RowHighlightBackColor != GrStyle::DefaultStyle.RowHighlightBackColor;
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