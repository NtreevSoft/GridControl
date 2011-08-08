#include "StdAfx.h"
#include "GridGroupingRow.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	GroupingRow::GroupingRow(_GridControl^ gridControl, GrGroupingList* pGroupingList) 
		: CellBase(gridControl, pGroupingList) , m_pGroupingList(pGroupingList)
	{
		
	}

	string^ GroupingRow::Text::get()
	{
		return gcnew string(m_pGroupingList->GetText());
	}

	void GroupingRow::Text::set(string^ text)
	{
		m_pGroupingList->SetText(ToNativeString::Convert(text));
		Invalidate();
	}

	bool GroupingRow::IsVisible::get()
	{
		return m_pGroupingList->GetVisible();
	}

	void GroupingRow::IsVisible::set(bool value)
	{
		m_pGroupingList->SetVisible(value);
		Invalidate();
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/