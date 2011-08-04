#include "StdAfx.h"
#include "GridCollection.h"
#include "GridColumn.h"
#include "GridRow.h"
#include "GridCell.h"
#include "GridControl.h"
#include "GridColumnExtension.h"


namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	CellCollection::Enumerator::Enumerator(GrDataRow* pDataRow, GrColumnList* pColumnList)
		: m_pColumnList(pColumnList), m_pDataRow(pDataRow), m_index(0)
	{

	}

	CellCollection::Enumerator::~Enumerator()
	{

	}
	
	bool CellCollection::Enumerator::MoveNext()
	{
		m_index++;
		return m_index <= m_pColumnList->GetColumnCount();
	}
	
	void CellCollection::Enumerator::Reset()
	{
		m_index = 0;
	}

	Cell^ CellCollection::Enumerator::Current::get()
	{
		GrColumn* pColumn = m_pColumnList->GetColumn(m_index - 1);
		GrItem* pItem = m_pDataRow->GetItem(pColumn);
		object^ ref = pItem->ManagedRef;
		return safe_cast<Cell^>(ref);
	}

	System::Collections::Generic::IEnumerator<Cell^>^ CellCollection::GetEnumerator()
	{
		return gcnew Enumerator(m_row->NativeRef, m_pColumnList);
	}

	CellCollection::CellCollection(Row^ row)
		: m_row(row)
	{
		m_pColumnList = row->GridCore->GetColumnList();
	}

	string^ CellCollection::ToString()
	{
		string^ text = string::Empty;
		for each(_Cell^ item in this)
		{
			text += ", ";
			text += item->ToString();
		}
		return text;
	}

	Cell^ CellCollection::default::get(string^ columnName)
	{
		for each(Cell^ cell in this)
		{
			if(cell->Column->ColumnName == columnName)
				return cell;
		}
		return nullptr;
	}

	Cell^ CellCollection::default::get(_Column^ column)
	{
		if(column == nullptr)
			throw gcnew System::NullReferenceException();
		for each(Cell^ cell in this)
		{
			if(cell->Column == column)
				return cell;
		}
		return nullptr;
	}

	Cell^ CellCollection::default::get(int index)
	{
		GrColumn* pColumn = m_pColumnList->GetColumn(index);
		GrDataRow* pDataRow = m_row->NativeRef;
		GrItem* pItem = pDataRow->GetItem(pColumn);
		object^ ref = pItem->ManagedRef;
		return safe_cast<Cell^>(ref);
	}

	int CellCollection::Count::get()
	{
		return (int)m_pColumnList->GetColumnCount();
	}

	Cell^ CellCollection::default::get(GrColumn* pColumn)
	{
		GrDataRow* pDataRow = m_row->NativeRef;
		GrItem* pItem = pDataRow->GetItem(pColumn);
		object^ ref = pItem->ManagedRef;
		return safe_cast<Cell^>(ref);
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/