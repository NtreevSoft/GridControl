//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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
#include "GridCaption.h"
#include "GridUtil.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	CaptionRow::CaptionRow(_GridControl^ gridControl, GrCaption* pCaption) 
		: CellBase(gridControl, pCaption) , m_pCaption(pCaption)
	{
		m_pCaption->ManagedRef = this;
	}

	string^ CaptionRow::Text::get()
	{
		return gcnew string(m_pCaption->GetText());
	}

	void CaptionRow::Text::set(string^ text)
	{
		m_pCaption->SetText(ToNativeString::Convert(text));
		Invalidate();
	}

	_StringAlignment CaptionRow::Alignment::get()
	{
		return (_StringAlignment)m_pCaption->GetTextHorzAlign();
	}
	
	void CaptionRow::Alignment::set(_StringAlignment value)
	{
		m_pCaption->SetTextHorzAlign((GrHorzAlign)value);
		Invalidate(this);
	}

	_StringAlignment CaptionRow::LineAlignment::get()
	{
		return (_StringAlignment)m_pCaption->GetTextVertAlign();
	}
	
	void CaptionRow::LineAlignment::set(_StringAlignment value)
	{
		m_pCaption->SetTextVertAlign((GrVertAlign)value);
		Invalidate(this);
	}

	bool CaptionRow::IsVisible::get()
	{
		return m_pCaption->GetVisible();
	}

	void CaptionRow::IsVisible::set(bool value)
	{
		m_pCaption->SetVisible(value);
		Invalidate();
	}

	CaptionRow^ CaptionRow::FromNative(GrCaption* pCaption)
	{
		object^ ref = pCaption->ManagedRef;
		return safe_cast<CaptionRow^>(ref);
	}

	int CaptionRow::Height::get()
	{	
		return m_pCaption->GetHeight();
	}

	void CaptionRow::Height::set(int value)
	{
		if(value < 0 || 
			value < m_pCaption->GetMinHeight() ||
			value > m_pCaption->GetMaxHeight() )
			throw gcnew System::ArgumentOutOfRangeException("value");
		m_pCaption->SetHeight(value);
		Invalidate();
	}

	int CaptionRow::MinHeight::get()
	{
		return m_pCaption->GetMinHeight();
	}

	void CaptionRow::MinHeight::set(int value)
	{
		if(value < 0 || 
			value > m_pCaption->GetMaxHeight() )
			throw gcnew System::ArgumentOutOfRangeException("value");
		m_pCaption->SetMinHeight(value);
		Invalidate();
	}

	int CaptionRow::MaxHeight::get()
	{
		return m_pCaption->GetMaxHeight();
	}

	void CaptionRow::MaxHeight::set(int value)
	{
		if(value < 0 || 
			value < m_pCaption->GetMinHeight())
			throw gcnew System::ArgumentOutOfRangeException("value");
		m_pCaption->SetMaxHeight(value);
		Invalidate();
	}

	bool CaptionRow::ShouldSerializeHeight()
	{
		return m_pCaption->GetHeight() != GrRow::DefaultHeight;
	}
	
	bool CaptionRow::ShouldSerializeMinHeight()
	{
		return m_pCaption->GetMinHeight() != GrRow::DefaultMinHeight;
	}
	
	bool CaptionRow::ShouldSerializeMaxHeight()
	{
		return m_pCaption->GetMaxHeight() != GrRow::DefaultMaxHeight;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/