//=====================================================================================================================
// Ntreev Grid for .Net 1.0
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
#include "GridCellIterator.h"
#include "GridControl.h"
#include "GridCell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	CellIterator::CellIterator(_GridControl^ gridControl) : GridObject(gridControl)
	{
		GridControl->FocusedCellChanged += gcnew CellEventHandler(this, &CellIterator::gridControl_OnFocusedCellChanged);

		m_pDataRowList = GridCore->GetDataRowList();
	}

	void CellIterator::FirstCell(_SelectionRange range)
	{
		GrItem* pFocused = Focuser->GetItem();
		if(pFocused == nullptr)
			return;

		GrColumn*	pColumn = pFocused->GetColumn();
		GrDataRow*	pDataRow= pFocused->GetDataRow();

		GrColumnList* pColumnList = pColumn->GetColumnList();

		if(pColumn->GetFrozen() == true)
		{
			uint nIndex = pColumn->GetFrozenIndex();
			if(nIndex == 0)
				return;

			nIndex = nIndex - 1;
			pColumn = pColumnList->GetFrozenColumn(0);
		}
		else
		{
			uint nFrozenColumnCount		= pColumnList->GetFrozenColumnCount();

			uint nIndex = pColumn->GetScrollableIndex();
			if(nIndex == 0 && nFrozenColumnCount > 0)
			{
				pColumn = pColumnList->GetFrozenColumn(0);
			}
			else
			{
				pColumn = pColumnList->GetUnfrozenColumn(0);
			}
		}

		pFocused = pDataRow->GetItem(pColumn);

		if(range == _SelectionRange::One)
		{
			Selector->SetColumnAnchor(pColumn);
			Selector->SelectItem(pFocused, GridControl->NativeSelectionType);
		}
		else
		{
			Selector->SelectItems(
				Selector->GetColumnSelections(pFocused), 
				Selector->GetRowSelections(pFocused),
				GridControl->NativeSelectionType);
		}
		Focuser->Set(pFocused);

		HorizontalScroll->DoScroll(_ScrollEventType::First);
		Invalidate();
	}

	void CellIterator::LastCell(_SelectionRange range)
	{
		GrItem* pFocused = Focuser->GetItem();
		if(pFocused == nullptr)
			return;

		GrColumn*	pColumn = pFocused->GetColumn();
		GrDataRow*		pDataRow	= pFocused->GetDataRow();

		GrColumnList* pColumnList = pColumn->GetColumnList();

		uint nVisibleColumnCount = pColumnList->GetVisibleColumnCount();
		uint nIndex = nVisibleColumnCount - 1;

		if(nIndex == pColumn->GetVisibleIndex())
			return;

		pColumn = pColumnList->GetVisibleColumn(nIndex);
		pFocused = pDataRow->GetItem(pColumn);
		
		if(range == _SelectionRange::One)
		{
			Selector->SelectItem(pFocused, GridControl->NativeSelectionType);
			Selector->SetAnchor(pFocused);
		}
		else
		{
			Selector->SelectItems(
				Selector->GetColumnSelections(pFocused), 
				Selector->GetRowSelections(pFocused),
				GridControl->NativeSelectionType);
		}
		Focuser->Set(pFocused);

		HorizontalScroll->DoScroll(_ScrollEventType::Last);
		Invalidate();
	}

	void CellIterator::PageUp(_SelectionRange range)
	{
		IFocusable* pFocused = Focuser->Get();
		if(pFocused == NULL)
			return;
		if(m_pDataRowList->GetVisibleRowCount() == 0)
			return;

		IDataRow* pDataRow = pFocused->GetIDataRow();
		uint nIndex = pDataRow->GetVisibleIndex();
		uint nNewIndex;

		if(nIndex == INSERTION_ROW)
			return;

		if(nIndex == 0)
			nNewIndex = INSERTION_ROW;
		else
			nNewIndex = m_pDataRowList->ClipTo(nIndex);

		if(nNewIndex == nIndex)
		{
			if(nNewIndex != 0)
				nNewIndex--;
		}

		IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(nNewIndex);

		if(range == _SelectionRange::One)
			SelectOne(pNewDataRow);
		else
			SelectMulti(pDataRow, pNewDataRow);

		if(pDataRow->GetDisplayable() == true)
			VerticalScroll->DoScroll(_ScrollEventType::LargeDecrement);
		else
			VerticalScroll->EnsureVisible(pNewDataRow);
	}

	void CellIterator::PageDown(_SelectionRange range)
	{
		IFocusable* pFocused = Focuser->Get();
		if(pFocused == NULL)
			return;
		if(m_pDataRowList->GetVisibleRowCount() == 0)
			return;

		IDataRow* pDataRow = pFocused->GetIDataRow();
		uint nIndex = pDataRow->GetVisibleIndex();
		uint nNewIndex;

		if(nIndex == INSERTION_ROW)
			nNewIndex = 0;
		else
			nNewIndex = m_pDataRowList->ClipFrom(nIndex);

		if(nNewIndex == nIndex)
		{
			if(nNewIndex + 1 != m_pDataRowList->GetVisibleRowCount())
				nNewIndex++;
		}

		IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(nNewIndex);

		if(range == _SelectionRange::One)
			SelectOne(pNewDataRow);
		else
			SelectMulti(pDataRow, pNewDataRow);

		if(pDataRow != GridCore->GetInsertionRow() && pDataRow->GetDisplayable() == true)
			VerticalScroll->DoScroll(_ScrollEventType::LargeIncrement);
		else
			VerticalScroll->EnsureVisible(pNewDataRow);
	}

	void CellIterator::FirstRow(_SelectionRange range)
	{
		IFocusable* pFocused = Focuser->Get();
		if(pFocused == NULL)
			return;
		if(m_pDataRowList->GetVisibleRowCount() == 0)
			return;

		IDataRow* pDataRow = pFocused->GetIDataRow();
		uint nNewIndex = 0;

		IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(nNewIndex);

		if(range == _SelectionRange::One)
			SelectOne(pNewDataRow);
		else
			SelectMulti(pDataRow, pNewDataRow);

		VerticalScroll->DoScroll(_ScrollEventType::First);
	}

	void CellIterator::LastRow(_SelectionRange range)
	{
		IFocusable* pFocused = Focuser->Get();
		if(pFocused == NULL)
			return;
		if(m_pDataRowList->GetVisibleRowCount() == 0)
			return;

		IDataRow* pDataRow = pFocused->GetIDataRow();
		uint nNewIndex = m_pDataRowList->GetVisibleRowCount() - 1;

		IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(nNewIndex);

		if(range == _SelectionRange::One)
			SelectOne(pNewDataRow);
		else
			SelectMulti(pDataRow, pNewDataRow);

		VerticalScroll->DoScroll(_ScrollEventType::Last);
	}

	void CellIterator::MoveUp(_SelectionRange range)
	{
		IFocusable* pFocused = Focuser->Get();
		if(pFocused == IFocusable::Null)
			return;
		if(m_pDataRowList->GetVisibleRowCount() == 0)
			return;
		
		IDataRow* pDataRow = pFocused->GetIDataRow();
		uint nIndex = pDataRow->GetVisibleIndex();

		switch(nIndex)
		{
		case 0:
			if(GridCore->GetInsertionRowVisible() == true)
				nIndex = INSERTION_ROW;
			else
				return;
			break;
		case INSERTION_ROW:
			return;
		default:
			nIndex = nIndex - 1;
			break;
		}

		IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(nIndex);

		if(range == _SelectionRange::One)
			SelectOne(pNewDataRow);
		else
			SelectMulti(pDataRow, pNewDataRow);

		VerticalScroll->EnsureVisible(pNewDataRow);
	}

	void CellIterator::MoveDown(_SelectionRange range)
	{
		IFocusable* pFocused = Focuser->Get();
		if(pFocused == IFocusable::Null)
			return;

		if(m_pDataRowList->GetVisibleRowCount() == 0)
			return;
		
		IDataRow* pDataRow = pFocused->GetIDataRow();
		uint nIndex = pDataRow->GetVisibleIndex();
		uint nLastIndex = m_pDataRowList->GetVisibleRowCount() - 1;

		if(nIndex == nLastIndex)
			return;
		else if(nIndex == INSERTION_ROW)
			nIndex = 0;
		else
			nIndex = nIndex + 1;

		IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(nIndex);

		if(range == _SelectionRange::One)
			SelectOne(pNewDataRow);
		else
			SelectMulti(pDataRow, pNewDataRow);

		VerticalScroll->EnsureVisible(pNewDataRow);
	}

	void CellIterator::MoveLeft(_SelectionRange range)
	{
		GrItem* pFocused = Focuser->GetItem();
		if(pFocused == NULL)
			return;

		GrColumn*	pColumn = pFocused->GetColumn();
		GrDataRow*		pDataRow	= pFocused->GetDataRow();

		GrColumnList* pColumnList = pColumn->GetColumnList();

		uint nIndex = pColumn->GetVisibleIndex();
		if(nIndex != 0)
			nIndex = nIndex - 1;

		if(nIndex == pColumn->GetVisibleIndex())
			return;

		pColumn = pColumnList->GetVisibleColumn(nIndex);
		pFocused = pDataRow->GetItem(pColumn);
		
		if(range == _SelectionRange::One)
		{
			if(GridControl->FullRowSelect == true)
				Selector->SelectItems(pDataRow, GrSelectionType_Normal);
			else
				Selector->SelectItem(pFocused, GrSelectionType_Normal);
			Selector->SetAnchor(pFocused);
		}
		else
		{
			Selector->SelectItems(
				Selector->GetColumnSelections(pFocused), 
				Selector->GetRowSelections(pFocused),
				GridControl->NativeSelectionType);
		}
		Focuser->Set(pFocused);

		HorizontalScroll->EnsureVisible(pColumn);
		Invalidate();
	}

	void CellIterator::MoveRight(_SelectionRange range)
	{
		GrItem* pFocused = Focuser->GetItem();
		if(pFocused == NULL)
			return;

		GrColumn*	pColumn = pFocused->GetColumn();
		GrDataRow*		pDataRow	= pFocused->GetDataRow();

		GrColumnList* pColumnList = pColumn->GetColumnList();

		uint nVisibleColumnCount = pColumnList->GetVisibleColumnCount();
		uint nIndex = pColumn->GetVisibleIndex() + 1;
		if(nIndex >= nVisibleColumnCount)
			nIndex = nVisibleColumnCount - 1;

		if(nIndex == pColumn->GetVisibleIndex())
			return;

		pColumn = pColumnList->GetVisibleColumn(nIndex);
		pFocused = pDataRow->GetItem(pColumn);
		
		if(range == _SelectionRange::One)
		{
			if(GridControl->FullRowSelect == true)
				Selector->SelectItems(pDataRow, GrSelectionType_Normal);
			else
				Selector->SelectItem(pFocused, GrSelectionType_Normal);
			Selector->SetAnchor(pFocused);
		}
		else
		{
			Selector->SelectItems(
				Selector->GetColumnSelections(pFocused), 
				Selector->GetRowSelections(pFocused),
				GridControl->NativeSelectionType);
		}
		Focuser->Set(pFocused);

		HorizontalScroll->EnsureVisible(pColumn);
		Invalidate();
	}

	
	void CellIterator::SelectOne(IDataRow* pDataRow)
	{
		switch(pDataRow->GetRowType())
		{
		case GrRowType_InsertionRow:
		case GrRowType_DataRow:
			{
				GrDataRow* pRow = dynamic_cast<GrDataRow*>(pDataRow);
				GrItem* pOldFocusItem = Focuser->GetItem();
				GrItem* pFocusItem;

				GrColumn* pColumn;

				if(pOldFocusItem != NULL)
					pColumn = pOldFocusItem->GetColumn();
				else
					pColumn = Selector->GetColumnAnchor();
				
				if(pColumn == NULL)
				{
					GrColumnList* pColumnList = GridCore->GetColumnList();
					pColumn = pColumnList->GetVisibleColumn(0);
				}
				pFocusItem = pRow->GetItem(pColumn);

				if(GridControl->FullRowSelect == true)
					Selector->SelectItems(pRow, GrSelectionType_Normal);
				else
					Selector->SelectItem(pFocusItem, GrSelectionType_Normal);
				Selector->SetAnchor(pFocusItem);
				Focuser->Set(pFocusItem);
			}
			break;
		default:
			{
				Selector->ClearSelection();
				Focuser->Set(pDataRow->GetFocusable(nullptr));
			}
			break;
		}
		Selector->SetSelectionGroup(pDataRow);
		Invalidate();
	}

	void CellIterator::SelectMulti(IDataRow* pBegin, IDataRow* pEnd)
	{
		if(pBegin->GetRowType() == GrRowType_InsertionRow || pEnd->GetRowType() == GrRowType_InsertionRow)
			return;

		GrIndexRange columnRange(Selector->GetColumnAnchor()->GetVisibleIndex(), m_pLastDataColumn->GetVisibleIndex());
		GrIndexRange rowRange;
			
		IDataRow* pRowAnchor = Selector->GetRowAnchor();
		if(pRowAnchor->GetRowType() == GrRowType_InsertionRow)
		{
			Selector->SetRowAnchor(pBegin);
			rowRange = GrIndexRange(pBegin->GetVisibleIndex(), pEnd->GetVisibleIndex());

		}
		else
		{
			rowRange = GrIndexRange(pRowAnchor->GetVisibleIndex(), pEnd->GetVisibleIndex());
		}

		columnRange.SetMaxValue(columnRange.GetMaxValue() + 1);
		rowRange.SetMaxValue(rowRange.GetMaxValue() + 1);

		Selector->SelectItems(columnRange, rowRange, GridControl->NativeSelectionType);

		switch(pEnd->GetRowType())
		{
		case GrRowType_DataRow:
			{
				GrDataRow* pRow = dynamic_cast<GrDataRow*>(pEnd);
				GrItem* pOldFocusItem = Focuser->GetItem();
				GrItem* pFocusItem;

				GrColumn* pColumn;

				if(pOldFocusItem != NULL)
					pColumn = pOldFocusItem->GetColumn();
				else
					pColumn = m_pLastDataColumn;
				
				if(pColumn == NULL)
				{
					GrColumnList* pColumnList = GridCore->GetColumnList();
					pColumn = pColumnList->GetVisibleColumn(0);
				}
				pFocusItem = pRow->GetItem(pColumn);

				Focuser->Set(pFocusItem);
			}
			break;
		default:
			{
				Focuser->Set(pEnd->GetFocusable(0));
			}
			break;
		}
		Invalidate();
	}

	void CellIterator::gridControl_OnFocusedCellChanged(object^ /*sender*/, CellEventArgs^ e)
	{
		Cell^ cell = e->Cell;
		if(cell != nullptr)
		{
			m_pLastDataColumn = cell->NativeRef->GetColumn();
		}
		else
		{

		}
	}

	HScrollProperty^ CellIterator::HorizontalScroll::get()
	{
		return GridControl->HorizontalScroll;
	}

	VScrollProperty^ CellIterator::VerticalScroll::get()
	{
		return GridControl->VerticalScroll;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/