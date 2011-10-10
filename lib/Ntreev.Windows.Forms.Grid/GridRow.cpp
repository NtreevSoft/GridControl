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
#include "GridRow.h"
#include "GridControl.h"
#include "GridCell.h"
#include "GridColumn.h"
#include "GridCollection.h"
#include "GridColumnCollection.h"

#include "GrGridCell.h"
#include "GrGridCore.h"
#include "GrGridRenderer.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	Row::Row(_GridControl^ gridControl) : m_pDataRow(new GrDataRow()), RowBase(gridControl, m_pDataRow)
	{
		m_pDataRow->ManagedRef = this;
		m_cellCollection = gcnew CellCollection(this);
		m_componentIndex = -1;
	}

	Row::Row(_GridControl^ gridControl, GrDataRow* pDataRow) : m_pDataRow(pDataRow), RowBase(gridControl, m_pDataRow)
	{
		m_pDataRow->ManagedRef = this;
		m_cellCollection = gcnew CellCollection(this);
	}

	void Row::Component::set(object^ value)
	{
		m_component = value;
	}

	GrDataRow* Row::NativeRef::get()
	{
		return m_pDataRow;
	}

	void Row::RefreshCells()
	{
		for each(_Column^ item in GridControl->Columns)
		{
			NewCell(item);
		}
	}

	void Row::NewCell(Column^ column)
	{
		GrItem* pItem = m_pDataRow->GetItem(column->NativeRef);
		Cell^ cell = Cell::FromNative(pItem);
		if(cell == nullptr)
			cell = gcnew Cell(GridControl, pItem);
	}

	string^ Row::ToString()
	{
		return this->Index.ToString();
		//string^ text = gcnew string(m_pDataRow->GetText());
		//text += " : ";
		//text += this->Cells->ToString();
		//return text;
	}

	CellCollection^ Row::Cells::get()
	{
		return m_cellCollection;
	}

	int Row::CellCount::get()
	{
		return m_cellCollection->Count;
	}

	uint Row::RowID::get()
	{
		return m_pDataRow->GetDataRowID();	
	}

	Cell^ Row::default::get(int index)
	{
		return m_cellCollection[index];
	}

	Cell^ Row::default::get(string^ columnName)
	{
		return m_cellCollection[columnName];
	}

	Cell^ Row::default::get(Column^ column)
	{
		return m_cellCollection[column];
	}

	Cell^ Row::default::get(GrColumn* pColumn)
	{
		return m_cellCollection[pColumn];
	}

	bool Row::IsVisible::get()
	{
		return m_pDataRow->GetVisible();
	}

	void Row::IsVisible::set(bool value)
	{
		m_pDataRow->SetVisible(value);
		Invalidate();
	}

	bool Row::IsReadOnly::get()
	{
		return m_pDataRow->GetReadOnly();
	}

	void Row::IsReadOnly::set(bool value)
	{
		m_pDataRow->SetReadOnly(value);
	}
	
	bool Row::IsSelected::get()
	{
		return m_pDataRow->GetSelected();
	}

	void Row::IsSelected::set(bool value)
	{
		m_pDataRow->SetSelected(value);
	}

	bool Row::IsFullSelected::get()
	{
		return m_pDataRow->GetFullSelected();
	}

	void Row::AddEditedCell()
	{
		m_editedCount++;
	}

	void Row::RemoveEditedCell()
	{
		m_editedCount--;
		if(m_editedCount < 0)
			throw gcnew System::Exception();
	}

	Row^ Row::FromNative(const GrDataRow* pDataRow)
	{
		object^ ref = pDataRow->ManagedRef;
		return safe_cast<Row^>(ref);
	}

	bool Row::IsBeingEdited::get()
	{
		return m_editing;
	}

	bool Row::IsEdited::get()
	{
		return m_editedCount > 0 ? true : false;
	}

	int Row::Index::get()
	{
		return (int)m_pDataRow->GetDataRowIndex();
	}

	void Row::BeginEdit()
	{
		m_editing = true;
	}

	void Row::CancelEdit()
	{
		try
		{
			if(m_editedCount == 0)
				return;

			for each(Cell^ cell in m_cellCollection)
			{
				cell->CancelEdit();
			}

			if(m_editedCount < 0)
				throw gcnew System::Exception("먼가 수상합니다.");
		}
		finally
		{
			m_editedCount = 0;
			m_editing = false;
		}
	}

	void Row::ApplyEdit()
	{
		try
		{
			if(m_editedCount == 0)
				return;

			for each(Cell^ cell in m_cellCollection)
			{
				cell->ApplyEdit();
			}

			if(m_editedCount < 0)
				throw gcnew System::Exception("먼가 수상합니다.");
		}
		finally
		{
			m_editedCount = 0;
			m_editing = false;
		}
	}
	
	void Row::EnsureVisible()
	{
		if(m_pDataRow->GetDisplayable() == true)
			return;
		GridControl->EnsureVisible(this);
	}

	void Row::Select(_SelectionType selectionType)
	{
		Selector->SelectItems(m_pDataRow, (GrSelectionType)selectionType);
	}

	void Row::Focus()
	{
		Focuser->Set(m_pDataRow);
	}

	void Row::ResetCellBackColor()
	{
		CellBackColor = _Color::Empty;
	}

	void Row::ResetCellForeColor()
	{
		CellForeColor = _Color::Empty;
	}

	_Color Row::CellBackColor::get()
	{
		return m_pDataRow->GetItemBackColor();
	}

	void Row::CellBackColor::set(_Color value)
	{
		m_pDataRow->SetItemBackColor(value);
		Invalidate();
	}

	_Color Row::CellForeColor::get()
	{
		return m_pDataRow->GetItemForeColor();
	}

	void Row::CellForeColor::set(_Color value)
	{
		m_pDataRow->SetItemForeColor(value);
		Invalidate();
	}

	bool Row::ShouldSerializeCellForeColor()
	{
		return m_pDataRow->GetItemForeColor() != GrColor::Empty;
	}

	bool Row::ShouldSerializeCellBackColor()
	{
		return m_pDataRow->GetItemBackColor() != GrColor::Empty;
	}
	
	bool Row::ShouldSerializeCellFont()
	{
		return m_pDataRow->GetItemFont() != nullptr;
	}

	InsertionRow::InsertionRow(_GridControl^ gridControl, GrInsertionRow* pInsertionRow)
		: Row(gridControl, pInsertionRow)
	{
		
	}

	void InsertionRow::SetDefaultValue()
	{
		for each(InsertionCell^ cell in Cells)
		{
			cell->SetDefaultValue();
		}
		ApplyEdit();
	}

	void InsertionRow::NewCell(Column^ column)
	{
		GrItem* pItem = NativeRef->GetItem(column->NativeRef);
		Cell^ cell = Cell::FromNative(pItem);
		if(cell == nullptr)
		{
			gcnew InsertionCell(GridControl, pItem, column->DefaultValue);
		}
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/