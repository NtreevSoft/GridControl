//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
		System::Object^ ref = pItem->ManagedRef;
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

	System::String^ CellCollection::ToString()
	{
		System::String^ text = System::String::Empty;
		for each(Cell^ item in this)
		{
			text += ", ";
			text += item->ToString();
		}
		return text;
	}

	Cell^ CellCollection::default::get(System::String^ columnName)
	{
		for each(Cell^ cell in this)
		{
			if(cell->Column->ColumnName == columnName)
				return cell;
		}
        throw gcnew System::ArgumentException();
	}

	Cell^ CellCollection::default::get(Column^ column)
	{
		if(column == nullptr)
			throw gcnew System::NullReferenceException();
		for each(Cell^ cell in this)
		{
			if(cell->Column == column)
				return cell;
		}
		throw gcnew System::ArgumentException();
	}

	Cell^ CellCollection::default::get(int index)
	{
		GrColumn* pColumn = m_pColumnList->GetColumn(index);
		GrDataRow* pDataRow = m_row->NativeRef;
		GrItem* pItem = pDataRow->GetItem(pColumn);
		System::Object^ ref = pItem->ManagedRef;
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
		System::Object^ ref = pItem->ManagedRef;
		return safe_cast<Cell^>(ref);
	}

    CellTagCollection::Enumerator::Enumerator(GrDataRow* pDataRow, GrColumnList* pColumnList)
		: m_pColumnList(pColumnList), m_pDataRow(pDataRow), m_index(0)
	{

	}

	CellTagCollection::Enumerator::~Enumerator()
	{

	}

	bool CellTagCollection::Enumerator::MoveNext()
	{
		m_index++;
		return m_index <= m_pColumnList->GetColumnCount();
	}

	void CellTagCollection::Enumerator::Reset()
	{
		m_index = 0;
	}

	System::Object^ CellTagCollection::Enumerator::Current::get()
	{
		GrColumn* pColumn = m_pColumnList->GetColumn(m_index - 1);
		GrItem* pItem = m_pDataRow->GetItem(pColumn);
		System::Object^ ref = pItem->ManagedRef;
		Cell^ cell = safe_cast<Cell^>(ref);
        return cell->Tag;
	}

	System::Collections::IEnumerator^ CellTagCollection::GetEnumerator()
	{
		return gcnew Enumerator(m_row->NativeRef, m_pColumnList);
	}

	CellTagCollection::CellTagCollection(Row^ row)
		: m_row(row)
	{
		m_pColumnList = row->GridCore->GetColumnList();
	}

	System::Object^ CellTagCollection::default::get(System::String^ columnName)
	{
        return m_row->Cells[columnName]->Tag;
	}

    void CellTagCollection::default::set(System::String^ columnName, System::Object^ value)
    {
        m_row->Cells[columnName]->Tag = value;
    }

	System::Object^ CellTagCollection::default::get(Column^ column)
	{
		return m_row->Cells[column]->Tag;
	}

    void CellTagCollection::default::set(Column^ column, System::Object^ value)
    {
        m_row->Cells[column]->Tag = value;
    }

	System::Object^ CellTagCollection::default::get(int index)
	{
		return m_row->Cells[index]->Tag;
	}

    void CellTagCollection::default::set(int index, System::Object^ value)
	{
		m_row->Cells[index]->Tag = value;
	}

	int CellTagCollection::Count::get()
	{
		return m_row->Cells->Count;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/