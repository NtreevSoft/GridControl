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
#include "CaptionRow.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    CaptionRow::CaptionRow(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrCaption* pCaption) 
        : CellBase(gridControl, pCaption), m_pCaption(pCaption)
    {
        this->Text = "제목";
    }

    System::String^ CaptionRow::Text::get()
    {
        return gcnew System::String(m_pCaption->GetText().c_str());
    }

    void CaptionRow::Text::set(System::String^ text)
    {
        m_pCaption->SetText(ToNativeString::Convert(text));
    }

    System::Drawing::StringAlignment CaptionRow::Alignment::get()
    {
        return (System::Drawing::StringAlignment)m_pCaption->GetTextHorzAlign();
    }

    void CaptionRow::Alignment::set(System::Drawing::StringAlignment value)
    {
        m_pCaption->SetTextHorzAlign((GrHorzAlign)value);
    }

    System::Drawing::StringAlignment CaptionRow::LineAlignment::get()
    {
        return (System::Drawing::StringAlignment)m_pCaption->GetTextVertAlign();
    }

    void CaptionRow::LineAlignment::set(System::Drawing::StringAlignment value)
    {
        m_pCaption->SetTextVertAlign((GrVertAlign)value);
    }

    bool CaptionRow::IsVisible::get()
    {
        return m_pCaption->GetVisible();
    }

    void CaptionRow::IsVisible::set(bool value)
    {
        m_pCaption->SetVisible(value);
    }

    CaptionRow^ CaptionRow::FromNative(GrCaption* pCaption)
    {
        System::Object^ ref = pCaption->ManagedRef;
        return safe_cast<CaptionRow^>(ref);
    }

    void CaptionRow::Height::set(int value)
    {
        if(value < 0)
            throw gcnew System::ArgumentOutOfRangeException("value");
        m_pCaption->SetHeight(value);
    }

    bool CaptionRow::ShouldSerializeHeight()
    {
        return m_pCaption->GetHeight() != GrRow::DefaultHeight;
    }

    bool CaptionRow::ShouldSerializeText()
    {
        return this->Text != "제목";
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/