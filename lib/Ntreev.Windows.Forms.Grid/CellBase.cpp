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
#include "CellBase.h"
#include "GridControl.h"
#include "NativeClasses.h"

#include <vcclr.h>

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    CellBase::CellBase(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrCell* pCell)
        : GridObject(gridControl), m_pCell(pCell)
    {
        m_pCell->ManagedRef = this;
    }

    CellBase::CellBase(GrCell* pCell)
        : GridObject(pCell), m_pCell(pCell)
    {
        m_pCell->ManagedRef = this;
    }

    System::Drawing::Color CellBase::BackColor::get()
    {
        return m_pCell->GetBackColor();
    }

    void CellBase::BackColor::set(System::Drawing::Color color)
    {
        m_pCell->SetBackColor(color);
    }

    System::Drawing::Color CellBase::ForeColor::get()
    {
        return m_pCell->GetForeColor();
    }

    void CellBase::ForeColor::set(System::Drawing::Color color)
    {
        m_pCell->SetForeColor(color);
    }

    System::Drawing::Font^ CellBase::Font::get()
    {
        GrFont* pFont = (GrFont*)m_pCell->GetPaintingFont();
        return Native::WinFormFontManager::ToManagedFont(pFont);
    }

    void CellBase::Font::set(System::Drawing::Font^ font)
    {
        GrFont* pFont = Native::WinFormFontManager::FromManagedFont(font);
        m_pCell->SetFont(pFont);
    }

    bool CellBase::IsDisplayable::get()
    {
        return m_pCell->GetDisplayable();
    }

    void CellBase::ResetBackColor()
    {
        m_pCell->SetBackColor(GrColor::Empty);
    }

    void CellBase::ResetForeColor()
    {
        m_pCell->SetForeColor(GrColor::Empty);
    }

    bool CellBase::ShouldSerializeForeColor()
    {
        return m_pCell->GetForeColor(false) != GrColor::Empty;
    }

    bool CellBase::ShouldSerializeBackColor()
    {
        return m_pCell->GetBackColor(false) != GrColor::Empty;
    }

    bool CellBase::ShouldSerializeFont()
    {
        GrFont* pFont = m_pCell->GetFont(false);
        if(pFont == nullptr)
            return false;
        return pFont != this->GridCore->GetGridWindow()->GetDefaultFont();
    }

    bool CellBase::ShouldSerializePadding()
    {
        return m_pCell->GetPadding(false) != GrPadding::Default;
    }

    void CellBase::ResetPadding()
    {
        m_pCell->SetPadding(GrPadding::Default);
    }

    void CellBase::ResetFont()
    {
        m_pCell->SetFont(NULL);
    }

    System::Windows::Forms::Padding CellBase::Padding::get()
    {
        return m_pCell->GetPadding();
    }

    void CellBase::Padding::set(System::Windows::Forms::Padding value)
    {
        m_pCell->SetPadding(value); 
    }

    void CellBase::OnBackColorChanged(System::EventArgs^ e)
    {
        BackColorChanged(this, e);
    }

    void CellBase::OnForeColorChanged(System::EventArgs^ e)
    {
        ForeColorChanged(this, e);
    }

    void CellBase::OnFontChanged(System::EventArgs^ e)
    {
        FontChanged(this, e);
    }

    System::Drawing::Color CellBase::DisplayBackColor::get()
    {
        return m_pCell->GetPaintingBackColor();
    }

    System::Drawing::Color CellBase::DisplayForeColor::get()
    {
        return m_pCell->GetPaintingForeColor();
    }

    System::Drawing::Rectangle CellBase::ClientRectangle::get()
    {
        return m_pCell->GetClientRect();
    }

    System::Drawing::Rectangle CellBase::Bounds::get()
    {
        return m_pCell->GetRect();
    }

    int CellBase::X::get()
    {
        return m_pCell->GetX();
    }

    int CellBase::Y::get()
    {
        return m_pCell->GetY();
    }

    int CellBase::Width::get()
    {
        return m_pCell->GetWidth();
    }

    int CellBase::Height::get()
    {
        return m_pCell->GetHeight();
    }

    System::Drawing::Point CellBase::Location::get()
    {
        return m_pCell->GetLocation();
    }

    System::Drawing::Size CellBase::Size::get()
    {
        return m_pCell->GetSize();
    }

    int CellBase::Left::get()
    {
        return m_pCell->GetX();
    }

    int CellBase::Top::get()
    {
        return m_pCell->GetY();
    }

    int CellBase::Right::get()
    {
        return m_pCell->GetRight();
    }

    int CellBase::Bottom::get()
    {
        return m_pCell->GetBottom();
    }

    void CellBase::ForeColorChanged::add(System::EventHandler^ p)
    {
        m_eventForeColorChanged += p; 
    }

    void CellBase::ForeColorChanged::remove(System::EventHandler^ p)
    {
        m_eventForeColorChanged -= p; 
    }

    void CellBase::ForeColorChanged::raise(System::Object^ sender, System::EventArgs^ e)
    {
        if(m_eventForeColorChanged != nullptr) 
        {
            m_eventForeColorChanged->Invoke(sender, e); 
        } 
    }

    void CellBase::BackColorChanged::add(System::EventHandler^ p)
    {
        m_eventBackColorChanged += p; 
    }

    void CellBase::BackColorChanged::remove(System::EventHandler^ p)
    {
        m_eventBackColorChanged -= p; 
    }

    void CellBase::BackColorChanged::raise(System::Object^ sender, System::EventArgs^ e)
    {
        if(m_eventBackColorChanged != nullptr) 
        {
            m_eventBackColorChanged->Invoke(sender, e); 
        } 
    }

    void CellBase::FontChanged::add(System::EventHandler^ p)
    {
        m_eventFontChanged += p; 
    }

    void CellBase::FontChanged::remove(System::EventHandler^ p)
    {
        m_eventFontChanged -= p; 
    }

    void CellBase::FontChanged::raise(System::Object^ sender, System::EventArgs^ e)
    {
        if(m_eventFontChanged != nullptr) 
        {
            m_eventFontChanged->Invoke(sender, e); 
        } 
    }

    System::Drawing::Color CellBase::PaintingForeColor_ICellBase::get()
    {
        return m_pCell->GetPaintingForeColor();
    }

    System::Drawing::Color CellBase::PaintingBackColor_ICellBase::get()
    {
        return m_pCell->GetPaintingBackColor();
    }

    bool CellBase::Multiline_ICellBase::get()
    {
        return m_pCell->GetTextMulitiline();
    }

    bool CellBase::WordWrap_ICellBase::get()
    {
        return m_pCell->GetTextWordWrap();
    }

    System::Drawing::StringAlignment CellBase::Alignment_ICellBase::get()
    {
        return (System::Drawing::StringAlignment)m_pCell->GetTextHorzAlign();
    }

    System::Drawing::StringAlignment CellBase::LineAlignment_ICellBase::get()
    {
        return (System::Drawing::StringAlignment)m_pCell->GetTextVertAlign();
    }

    CellState CellBase::State_ICellBase::get()
    {
        GrFlag flag = m_pCell->ToPaintStyle();
        return (CellState)flag.Get((ulong)CellState::All);
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/