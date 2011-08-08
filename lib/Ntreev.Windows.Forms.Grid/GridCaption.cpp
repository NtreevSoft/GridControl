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
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/