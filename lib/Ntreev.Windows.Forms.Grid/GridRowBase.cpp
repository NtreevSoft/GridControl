#include "StdAfx.h"
#include "GridRowBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	RowBase::RowBase(_GridControl^ gridControl, IDataRow* pDataRow)
		: CellBase(gridControl, pDataRow), m_pDataRow(pDataRow)
	{

	}

	int RowBase::Height::get()
	{	
		return m_pDataRow->GetHeight();
	}

	void RowBase::Height::set(int value)
	{
		if(value < 0 || 
			value < m_pDataRow->GetMinHeight() ||
			value > m_pDataRow->GetMaxHeight() )
			throw gcnew System::ArgumentOutOfRangeException("value");
		m_pDataRow->SetHeight(value);
		Invalidate();
	}

	int RowBase::MinHeight::get()
	{
		return m_pDataRow->GetMinHeight();
	}

	void RowBase::MinHeight::set(int value)
	{
		if(value < 0 || 
			value > m_pDataRow->GetMaxHeight() )
			throw gcnew System::ArgumentOutOfRangeException("value");
		m_pDataRow->SetMinHeight(value);
		Invalidate();
	}

	int RowBase::MaxHeight::get()
	{
		return m_pDataRow->GetMaxHeight();
	}

	void RowBase::MaxHeight::set(int value)
	{
		if(value < 0 || 
			value < m_pDataRow->GetMinHeight())
			throw gcnew System::ArgumentOutOfRangeException("value");
		m_pDataRow->SetMaxHeight(value);
		Invalidate();
	}

	bool RowBase::IsVisible::get()
	{
		return m_pDataRow->GetVisible();
	}

	uint RowBase::DisplayIndex::get()
	{
		return m_pDataRow->GetDisplayIndex();
	}

	uint RowBase::VisibleIndex::get()
	{
		return m_pDataRow->GetVisibleIndex();
	}

	bool RowBase::ShouldSerializeHeight()
	{
		return m_pDataRow->GetHeight() != GrRow::DefaultHeight;
	}
	
	bool RowBase::ShouldSerializeMinHeight()
	{
		return m_pDataRow->GetMinHeight() != GrRow::DefaultMinHeight;
	}
	
	bool RowBase::ShouldSerializeMaxHeight()
	{
		return m_pDataRow->GetMaxHeight() != GrRow::DefaultMaxHeight;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/