//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
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
        return Native::WinFormFontManager::ToManagedFont(pFont);
    }

    GrFont* FontConverter::TtoS(System::Drawing::Font^ font)
    {
        return Native::WinFormFontManager::FromManagedFont(font);
    }

    Style::Style()
    {
        m_pNativeStyle = new GrStyle();
        m_rowCellForeColors = gcnew Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>(&m_pNativeStyle->RowItemForeColors);
        m_rowCellBackColors = gcnew Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>(&m_pNativeStyle->RowItemBackColors);
        m_rowCellFonts = gcnew  Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Font^, GrFont*, FontConverter>();


        m_groupingForeColors = gcnew Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>(&m_pNativeStyle->GroupForeColors);
        m_groupingBackColors = gcnew Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>(&m_pNativeStyle->GroupForeColors);
        m_groupingFonts = gcnew  Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Font^, GrFont*, FontConverter>();
    }

    void Style::ResetRowCellForeColors()
    {
        m_rowCellForeColors->Clear();
        m_pNativeStyle->RowItemForeColors.clear();
    }

    void Style::ResetRowCellBackColors()
    {
        m_rowCellBackColors->Clear();
        m_pNativeStyle->RowItemBackColors.clear();
    }

    void Style::ResetRowCellFonts()
    {
        m_rowCellFonts->Clear();
        m_pNativeStyle->RowItemFonts.clear();
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

    void Style::ResetCellForeColor()
    {
        m_pNativeStyle->CellForeColor = GrStyle::DefaultStyle.CellForeColor;
    }

    void Style::ResetCellBackColor()
    {
        m_pNativeStyle->CellBackColor = GrStyle::DefaultStyle.CellBackColor;
    }

    void Style::ResetCellFont()
    {
        m_pNativeStyle->CellFont = GrStyle::DefaultStyle.CellFont;
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
    }

    System::Drawing::Color Style::SelectedBackColor::get()
    {
        return m_pNativeStyle->SelectedBackColor;
    }

    void Style::SelectedBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->SelectedBackColor = value;
    }

    System::Drawing::Color Style::FocusedForeColor::get()
    {
        return m_pNativeStyle->FocusedForeColor;
    }

    void Style::FocusedForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->FocusedForeColor = value;
    }

    System::Drawing::Color Style::FocusedBackColor::get()
    {
        return m_pNativeStyle->FocusedBackColor;
    }

    void Style::FocusedBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->FocusedBackColor = value;
    }

    System::Drawing::Color Style::CellForeColor::get()
    {
        return m_pNativeStyle->GetCellForeColor();
    }

    void Style::CellForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->CellForeColor = value;
    }

    System::Drawing::Color Style::CellBackColor::get()
    {
        return m_pNativeStyle->GetCellBackColor();
    }

    void Style::CellBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->CellBackColor = value;
    }

    System::Drawing::Font^ Style::CellFont::get()
    {
        return Native::WinFormFontManager::ToManagedFont(m_pNativeStyle->GetCellFont());
    }

    void Style::CellFont::set(System::Drawing::Font^ value)
    {
        m_pNativeStyle->CellFont = Native::WinFormFontManager::FromManagedFont(value);
    }

    System::Collections::Generic::ICollection<System::Drawing::Color>^ Style::RowCellBackColors::get()
    {
        return m_rowCellBackColors;
    }

    System::Collections::Generic::ICollection<System::Drawing::Color>^ Style::RowCellForeColors::get()
    {
        return m_rowCellForeColors;
    }

    System::Collections::Generic::ICollection<System::Drawing::Font^>^ Style::RowCellFonts::get()
    {
        return m_rowCellFonts;
    }

    System::Collections::Generic::ICollection<System::Drawing::Color>^ Style::GroupingBackColors::get()
    {
        return m_groupingBackColors;
    }

    System::Collections::Generic::ICollection<System::Drawing::Color>^ Style::GroupingForeColors::get()
    {
        return m_groupingForeColors;
    }

    System::Collections::Generic::ICollection<System::Drawing::Font^>^ Style::GroupingFonts::get()
    {
        return m_groupingFonts;
    }

    System::Drawing::Color Style::RowHighlightForeColor::get()
    {
        return m_pNativeStyle->RowHighlightForeColor; 
    }

    void Style::RowHighlightForeColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->RowHighlightForeColor = value;
    }

    System::Drawing::Color Style::RowHighlightBackColor::get()
    {
        return m_pNativeStyle->RowHighlightBackColor; 
    }

    void Style::RowHighlightBackColor::set(System::Drawing::Color value)
    {
        m_pNativeStyle->RowHighlightBackColor = value;
    }

    GrStyle* Style::NativeStyle::get()
    {
        m_pNativeStyle->RowItemFonts.clear();

        for each(System::Drawing::Color value in m_rowCellBackColors)
        {
            // m_pNativeStyle->RowItemBackColors.push_back(value);
        }

        //for each(System::Drawing::Color value in m_rowCellForeColors)
        //{
        // m_pNativeStyle->RowItemForeColors.push_back(value);
        //}

        for each(System::Drawing::Font^ value in m_rowCellFonts)
        {
            GrFont* pFont = Native::WinFormFontManager::FromManagedFont(value);
            m_pNativeStyle->RowItemFonts.push_back(pFont);
        }
        return m_pNativeStyle;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/