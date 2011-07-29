﻿#include "StdAfx.h"
#include "GrGridCoreComponents.h"
#include "GrGridCore.h"
#include <assert.h>

#if _MSC_VER >= 1600
#include <iterator>
#endif

GrFocuser::GrFocuser()
{
	ResetVariables();
}

void GrFocuser::OnGridCoreAttached()
{
	GrObject::OnGridCoreAttached();

	m_pGridCore->Cleared.Add(this, &GrFocuser::gridCore_Cleared);
	m_pGridCore->Created.Add(this, &GrFocuser::gridCore_Created);
}

GrDataRow* GrFocuser::GetFocusableDataRow() const
{
	IDataRow* pDataRow = GetFocusedRow();

	if(pDataRow == NULL)
	{
		GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
		if(pDataRowList->GetVisibleDataRowCount() == 0)
			return NULL;
		pDataRow = pDataRowList->GetVisibleDataRow(0);
	}

	return dynamic_cast<GrDataRow*>(pDataRow);
}

GrColumn* GrFocuser::GetFocusableColumn() const
{
	GrColumn* pColumn = GetFocusedColumn();

	if(pColumn == NULL)
	{
		pColumn = m_pLastFocusedColumn;
	}

	if(pColumn == NULL)
	{
		GrColumnList* pColumnList = m_pGridCore->GetColumnList();
		if(pColumnList->GetVisibleColumnCount() == 0)
			return NULL;
		pColumn = pColumnList->GetVisibleColumn(0);
	}

	return pColumn;
}

void GrFocuser::Set(GrColumn* pColumn)
{
	GrDataRow* pDataRow = GetFocusableDataRow();
	if(pDataRow == NULL)
		return;

	GrItem* pItem = pDataRow->GetItem(pColumn);
	Set(pItem);
}

void GrFocuser::Set(IDataRow* pDataRow)
{
	GrColumn* pColumn = GetFocusableColumn();
	IFocusable* pFocusable = NULL;
	if(pColumn)
		pFocusable = pDataRow->GetFocusable(pColumn);
	Set(pFocusable);
}

void GrFocuser::Set(IFocusable* pFocusable)
{
	if(m_pFocus == pFocusable)
		return;

	OnFocusChanging(&GrEventArgs::Empty);

	if(pFocusable)
	{
		m_pFocus = pFocusable;
		m_pLastFocusedRow = pFocusable->GetIDataRow();
	}
	else
	{
		m_pFocus		= NULL;
	}

	OnFocusChanged(&GrEventArgs::Empty);
}

GrColumn* GrFocuser::GetLastFocusedColumn() const
{
	return m_pLastFocusedColumn;
}

IDataRow* GrFocuser::GetLastFocusedRow() const
{
	return m_pLastFocusedRow;
}

GrColumn* GrFocuser::GetFocusedColumn() const
{
	if(m_pFocusItem == NULL)
		return NULL;
	return m_pFocusItem->GetColumn();
}

IDataRow* GrFocuser::GetFocusedRow() const
{
	if(m_pFocus == NULL)
		return NULL;
	return m_pFocus->GetIDataRow();
}

IFocusable* GrFocuser::Get() const
{
	if(m_pFocusing != NULL)
		return m_pFocusing;
	return m_pFocus;
}

bool GrFocuser::Has() const
{
	return m_pFocus != NULL ? true : false;
}

GrItem* GrFocuser::GetItem() const
{
	return dynamic_cast<GrItem*>(Get());
}

void GrFocuser::SetFocusing(IFocusable* pFocusable)
{
	m_pFocusing = pFocusable;
	m_pGridCore->Invalidate();
}

void GrFocuser::OnFocusChanging(GrEventArgs* e)
{
	GrCell* pCell = dynamic_cast<GrCell*>(m_pFocus);
	if(pCell != NULL)
	{
		if(m_pGridCore->GetRowHighlight() == true)
		{
			GrRow* pRow = pCell->GetRow();
			GrRect rt = pRow->GetDisplayRect();
			rt.right = m_pGridCore->GetDisplayRect()->right;
			rt.Expand(1, 1, 1, 1);
			m_pGridCore->Invalidate(rt);
		}
		else
		{
			m_pGridCore->Invalidate(pCell->GetDisplayRect());
		}
	}

	FocusChanging(this, e);
}

void GrFocuser::OnFocusChanged(GrEventArgs* e)
{
	FocusChanged(this, e);

	GrItem* pItem = dynamic_cast<GrItem*>(m_pFocus);
	if(pItem != NULL)
	{
		m_pLastFocusedColumn = pItem->GetColumn();
		m_pLastFocusedItem = pItem;
	}

	GrCell* pCell = dynamic_cast<GrCell*>(m_pFocus);
	if(pCell != NULL)
	{
		if(m_pGridCore->GetRowHighlight() == true)
		{
			GrRow* pRow = pCell->GetRow();
			GrRect rt = pRow->GetDisplayRect();
			rt.right = m_pGridCore->GetDisplayRect()->right;
			rt.Expand(1, 1, 1, 1);
			m_pGridCore->Invalidate(rt);
		}
		else
		{
			m_pGridCore->Invalidate(pCell->GetDisplayRect());
		}
	}
}

void GrFocuser::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	ResetVariables();
}

void GrFocuser::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrItemSelector* pSelector = m_pGridCore->GetItemSelector();
	pSelector->SelectingEnd.Add(this, &GrFocuser::itemSelector_SelectingEnd);
}

void GrFocuser::itemSelector_SelectingEnd(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	if(m_pFocusing != NULL)
	{
		Set(m_pFocusing);
		m_pFocusing = NULL;
	}
}

void GrFocuser::ResetVariables()
{
	m_pFocus = NULL;
	m_pFocusItem = NULL;

	m_pLastFocusedItem = NULL;
	m_pLastFocusedColumn = NULL;
	m_pLastFocusedRow = NULL;
	m_pFocusing = NULL;
}

GrHitTester::GrHitTester()
{
	
}

void GrHitTester::OnGridCoreAttached()
{
	GrObject::OnGridCoreAttached();
	m_pGridCore->Created.Add(this, &GrHitTester::gridCore_Created);
}

bool GrHitTester::Test(GrPoint pt, GrHitTest* pHitTest) const
{
	GrRow* pRootRow = m_pGridCore->GetRoot();
	const GrCell* pCell = pRootRow->HitTest(pt);

	if(pCell)
	{
		pHitTest->pHitted		= const_cast<GrCell*>(pCell);
		pHitTest->ptLocal		= pt - pCell->GetPosition();
		pHitTest->rtRect		= pCell->GetRect();

		pHitTest->rtRect.left 	= pCell->GetX();
		pHitTest->rtRect.top  	= pCell->GetY();
		pHitTest->rtRect.right  = pCell->GetX() + pCell->GetWidth();
		pHitTest->rtRect.bottom = pCell->GetY() + pCell->GetHeight();
		return true;
	}	

	return false;
}

bool GrHitTester::Test(GrRect rtSelection, GrItems* pTested) const
{
	GrColumns	dataColumns;
	GrDataRows		dataRows;

	m_pColumnList->HitTest(rtSelection, &dataColumns);
	m_pDataRowList->HitTest(rtSelection.top, rtSelection.bottom, &dataRows);

	if(dataColumns.size() == 0 || dataRows.size() == 0)
		return false;

	for_stl_const(GrDataRows, dataRows, itor)
	{
		GrDataRow* pDataRow = *itor;
		for_stl_const(GrColumns, dataColumns, itor_)
		{
			GrColumn* pColumn = *itor_;
			pTested->insert(pDataRow->GetItem(pColumn));
		}
	}
	return true;
}

bool GrHitTester::DisplayTest(GrPoint pt, GrHitTest* pHitTest) const
{
	GrRow* pRootRow = m_pGridCore->GetRoot();
	const GrCell* pCell = pRootRow->HitDisplayTest(pt);

	if(pCell)
	{
		pHitTest->pHitted	= const_cast<GrCell*>(pCell);
		pHitTest->ptLocal	= pt - pCell->GetDisplayPosition();
		pHitTest->rtRect	= pCell->GetDisplayRect();
		return true;
	}	

	return false;
}

void GrHitTester::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	m_pColumnList	= m_pGridCore->GetColumnList();
	m_pDataRowList	  = m_pGridCore->GetDataRowList();
}

GrItemSelector::GrItemSelector()
{
	ResetVariables();
}

void GrItemSelector::OnGridCoreAttached()
{
	GrObject::OnGridCoreAttached();
	m_pGridCore->Cleared.Add(this, &GrItemSelector::gridCore_Cleared);
	m_pGridCore->Created.Add(this, &GrItemSelector::gridCore_Created);
}

void GrItemSelector::SelectItem(GrItem* pItem, GrSelectionType selectType)
{
	if(pItem == NULL)
		throw new std::exception("item is null");

	GrItems items;
	items.insert(pItem);

	if(selectType == GrSelectionType_Normal)
		SetSelectionGroup(pItem);
	SelectItems(&items, selectType);
}

void GrItemSelector::SelectItems(const GrItems* pItems, GrSelectionType selectType)
{
	BeginSelection();
	switch(selectType)
	{
	case GrSelectionType_Normal:
		{
			GrItems difference;

			if(m_selectedItems.size() > 0)
			{
				set_difference(
					m_selectedItems.begin(), m_selectedItems.end(), 
					pItems->begin(), pItems->end(), 
					inserter(difference, difference.begin())
					);

				for_stl_const(GrItems, difference, itor)
				{
					DoDeselect(*itor);
				}
			}

			for_stl_const_ptr(GrItems, pItems, itor)
			{
				DoSelect(*itor);
			}
		}
		break;
	case GrSelectionType_Add:
		{
			for_stl_const_ptr(GrItems, pItems, itor)
			{
				DoSelect(*itor);
			}
		}
		break;
	case GrSelectionType_Remove:
		{
			for_stl_const_ptr(GrItems, pItems, itor)
			{
				DoDeselect(*itor);
			}
		}
		break;
	}
	EndSelection();
}

void GrItemSelector::SelectItems(GrRect rtGlobal, GrSelectionType selectType)
{
	GrItems items;
	m_pHitTester->Test(rtGlobal, &items);
	SelectItems(&items, selectType);
}

void GrItemSelector::SelectItems(GrItem* pBegin, GrItem* pEnd, GrSelectionType selectType)
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	GrItems items;

	uint nBeginColumn, nEndColumn, nBeginRow, nEndRow;

	nBeginColumn = pBegin->GetColumn()->GetVisibleIndex();
	nEndColumn = pEnd->GetColumn()->GetVisibleIndex();

	nBeginRow = pBegin->GetDataRow()->GetVisibleDataRowIndex();
	nEndRow = pEnd->GetDataRow()->GetVisibleDataRowIndex();

	if(nBeginColumn > nEndColumn)
	{
		uint nTemp = nBeginColumn;
		nBeginColumn = nEndColumn;
		nEndColumn = nTemp;
	}

	if(nBeginRow > nEndRow)
	{
		uint nTemp = nBeginRow;
		nBeginRow = nEndRow;
		nEndRow = nTemp;
	}

	for(uint i=nBeginRow ; i<=nEndRow ; i++)
	{
		GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
		for(uint j=nBeginColumn ; j<=nEndColumn ; j++)
		{
			GrColumn* pColumn = pColumnList->GetVisibleColumn(j);
			GrItem* pItem = pDataRow->GetItem(pColumn);
			items.insert(pItem);
		}
	}
	SelectItems(&items, selectType);
}

void GrItemSelector::SelectItems(GrIndexRange visibleColumnIndex, GrIndexRange visibleRowIndex, GrSelectionType selectType)
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

	GrItems items;
	for(GrIndexRange::value_type y = visibleRowIndex.GetMinValue() ; y < visibleRowIndex.GetMaxValue() ; y++)
	{
		GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(y));
		if(pDataRow == NULL)
			continue;

		for(GrIndexRange::value_type x = visibleColumnIndex.GetMinValue() ; x < visibleColumnIndex.GetMaxValue() ; x++)
		{
			GrColumn* pColumn = pColumnList->GetVisibleColumn(x);
			GrItem* pItem = pDataRow->GetItem(pColumn);
			items.insert(pItem);
		}
	}
	SelectItems(&items, selectType);
}

void GrItemSelector::SelectDataRows(const GrDataRows* pDataRows, GrSelectionType selectType)
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();

	GrItems items;
	for_stl_const_ptr(GrDataRows, pDataRows, itor)
	{
		GrDataRows::value_type value = *itor;
		for(uint i=0 ; i<pColumnList->GetVisibleColumnCount() ; i++)
		{
			GrColumn* pColumn = pColumnList->GetVisibleColumn(i);
			GrItem* pItem = value->GetItem(pColumn);
			items.insert(pItem);
		}
	}
	SelectItems(&items, selectType);
}

void GrItemSelector::SelectDataRows(GrDataRow* pFrom, GrDataRow* pTo, GrSelectionType selectType)
{
	if(pFrom->GetSelectionGroup() != pTo->GetSelectionGroup())
		throw new std::exception();

	GrIndexRange indexRange(pFrom->GetVisibleDataRowIndex(), pTo->GetVisibleDataRowIndex());

	GrDataRows dataRows;
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	for(uint i=indexRange.GetMinValue() ; i<=indexRange.GetMaxValue() ; i++)
	{
		GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
		dataRows.insert(pDataRow);
	}

	SelectDataRows(&dataRows, selectType);
}

void GrItemSelector::SelectDataRows(IDataRow* pFrom, IDataRow* pTo, GrSelectionType selectType)
{
	if(pFrom->GetSelectionGroup() != pTo->GetSelectionGroup())
		throw new std::exception();

	GrIndexRange indexRange(pFrom->GetVisibleIndex(), pTo->GetVisibleIndex());

	GrDataRows dataRows;
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	for(uint i=indexRange.GetMinValue() ; i<=indexRange.GetMaxValue() ; i++)
	{
		GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(i));
		if(pDataRow != NULL)
			dataRows.insert(pDataRow);
	}

	SelectDataRows(&dataRows, selectType);
}

void GrItemSelector::SelectDataRows(GrRect rtSelection, GrSelectionType selectType)
{
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	GrDataRows dataRows;
	pDataRowList->HitTest(rtSelection.top, rtSelection.bottom, &dataRows);
	SelectDataRows(&dataRows, selectType);
}

void GrItemSelector::SelectItems(GrDataRow* pDataRow, GrSelectionType selectType)
{
	GrDataRows dataRows;
	if(pDataRow)
		dataRows.insert(pDataRow);
	SelectDataRows(&dataRows, selectType);
}

void GrItemSelector::SelectColumns(const GrColumns* pColumns, GrSelectionType selectType)
{
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	GrItems items;
	for_stl_const_ptr(GrColumns, pColumns, itor)
	{
		GrColumns::value_type value = *itor;
		for(uint i=0 ; i<pDataRowList->GetVisibleDataRowCount() ; i++)
		{
			GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
			GrItem* pItem = pDataRow->GetItem(value);
			items.insert(pItem);
		}				
	}

	SelectItems(&items, selectType);
}

void GrItemSelector::SelectColumns(GrColumn* pFrom, GrColumn* pTo, GrSelectionType selectType)
{
	GrIndexRange indexRange(pFrom->GetVisibleIndex(), pTo->GetVisibleIndex());

	GrColumns dataRows;
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	for(uint i=indexRange.GetMinValue() ; i<=indexRange.GetMaxValue() ; i++)
	{
		GrColumn* pColumn = dynamic_cast<GrColumn*>(pColumnList->GetVisibleColumn(i));
		if(pColumn != NULL)
			dataRows.insert(pColumn);
	}

	SelectColumns(&dataRows, selectType);
}

void GrItemSelector::SelectColumns(GrRect rtSelection, GrSelectionType selectType)
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	GrColumns dataColumns;
	pColumnList->HitTest(rtSelection, &dataColumns);
	SelectColumns(&dataColumns, selectType);
}

void GrItemSelector::SelectItems(GrColumn* pColumn, GrSelectionType selectType)
{
	GrColumns dataColumns;
	if(pColumn)
		dataColumns.insert(pColumn);
	SelectColumns(&dataColumns, selectType);
}

void GrItemSelector::SelectAll()
{
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	GrDataRows dataRows;
	for(uint i=0 ; i<pDataRowList->GetDataRowCount() ; i++)
	{
		GrDataRow* pDataRow = pDataRowList->GetDataRow(i);
		dataRows.insert(pDataRow);
	}
	SelectDataRows(&dataRows, GrSelectionType_Add);
}

void GrItemSelector::ClearSelection()
{
	BeginSelection();

	GrItems items = m_selectedItems;
	for_stl_const(GrItems, items, itor)
	{
		DoDeselect(*itor);
	}
	m_selectedItems.clear();
	m_selectedColumns.clear();
	m_selectedRows.clear();
	EndSelection();
}

void GrItemSelector::ResetVariables()
{
	m_selectedItems.clear();
	m_selectedColumns.clear();
	m_selectedRows.clear();

	m_bSelectionLock = false;
	m_bSelecting = false;
	m_nSelectionGroup = 0;

	m_pAnchorColumn = NULL;
	m_pAnchorDataRow = NULL;
}


const GrSelectedColumns* GrItemSelector::GetSelectedColumns() const
{
	return &m_externalSelectedColumns;
}

const GrSelectedRows* GrItemSelector::GetSelectedRows() const
{
	return &m_externalSelectedRows;
}

void GrItemSelector::LockSelectionUpdate()
{
	if(m_bSelectionLock == true)
		throw "already locked";

	BeginSelection();
	m_bSelectionLock = true;
}

void GrItemSelector::UnlockSelectionUpdate()
{
	if(m_bSelectionLock == false)
		throw "did not locked";
	m_bSelectionLock = false;
	EndSelection();
}

bool GrItemSelector::CanSelect(IDataRow* pDataRow) const
{
	return pDataRow->GetSelectionGroup() == m_nSelectionGroup ? true : false;
}

bool GrItemSelector::CanSelect(GrItem* pItem) const
{
	return CanSelect(pItem->GetDataRow());
}

void GrItemSelector::SetSelectionGroup(uint nSelectionGroup)
{
	m_nSelectionGroup = nSelectionGroup;
}

void GrItemSelector::SetSelectionGroup(IDataRow* pDataRow)
{
	SetSelectionGroup(pDataRow->GetSelectionGroup());
}

void GrItemSelector::SetSelectionGroup(GrItem* pItem)
{
	SetSelectionGroup(pItem->GetDataRow());
}

void GrItemSelector::OnSelectionChanged(GrEventArgs* e)
{
	SelectionChanged.Raise(this, e);
}

void GrItemSelector::OnSelectedColumnsChanged(GrEventArgs* e)
{
	GrSelectedColumns& selections = m_externalSelectedColumns;
	selections.clear();
	selections.reserve(m_selectedColumns.size());
	selections.insert(selections.begin(), m_selectedColumns.begin(), m_selectedColumns.end());
	SortSelections<GrColumn> pr;
	std::sort(selections.begin(), selections.end(), pr);
	SelectedColumnsChanged.Raise(this, e);
}

void GrItemSelector::OnSelectedRowsChanged(GrEventArgs* e)
{
	GrSelectedRows& selections = m_externalSelectedRows;
	selections.clear();
	selections.reserve(m_selectedRows.size());
	selections.insert(selections.begin(), m_selectedRows.begin(), m_selectedRows.end());
	SortSelections<GrDataRow> pr;
	std::sort(selections.begin(), selections.end(), pr);
	SelectedRowsChanged.Raise(this, e);
}

void GrItemSelector::OnSelectingBegin(GrEventArgs* e)
{
	SelectingBegin(this, e);
}

void GrItemSelector::OnSelectingEnd(GrEventArgs* e)
{
	SelectingEnd(this, e);
}

void GrItemSelector::DoSelect(GrItem* pItem)
{
	if(pItem->m_bSelected == true)
		return;

	GrColumn* pColumn = pItem->GetColumn();
	GrDataRow* pDataRow = pItem->GetDataRow();
	if(pDataRow->GetSelectionGroup() != m_nSelectionGroup)
		return;

	pItem->m_bSelected = true;

	pColumn->m_nSelectedCells++;
	pDataRow->m_nSelectedCells++;
	if(pColumn->m_nSelectedCells > 0)
		m_selectedColumns.insert(pColumn);
#pragma message (__FILE__ "(" STRING(__LINE__) "): insertion Row Select에 대한 점검")
	if(pDataRow->m_nSelectedCells > 0 && pDataRow->GetRowType() != GrRowType_InsertionRow)
		m_selectedRows.insert(pDataRow);
	m_selectedItems.insert(pItem);

	if(pItem->GetDisplayable() == true)
		AddInvalidatedRectangle(pItem->GetDisplayRect());
}

void GrItemSelector::DoDeselect(GrItem* pItem)
{
	if(pItem->m_bSelected == false)
		return;

	GrColumn*   pColumn = pItem->GetColumn();
	GrDataRow*	pDataRow = pItem->GetDataRow();

	pItem->m_bSelected = false;

	pColumn->m_nSelectedCells--;
	pDataRow->m_nSelectedCells--;
	if(pColumn->m_nSelectedCells == 0)
		m_selectedColumns.erase(pColumn);
	if(pDataRow->m_nSelectedCells == 0)
		m_selectedRows.erase(pDataRow);
	m_selectedItems.erase(pItem);

	if(pItem->GetDisplayable() == true)
		AddInvalidatedRectangle(pItem->GetDisplayRect());
}

void GrItemSelector::BeginSelection()
{
	if(m_bSelectionLock == true)
		return;
	m_oldSelectedColumns = m_selectedColumns;
	m_oldSelectedRows	 = m_selectedRows;
	m_oldSelectedItems	 = m_selectedItems;

	m_invalidate.DoEmpty();
}

void GrItemSelector::EndSelection()
{
	if(m_bSelectionLock == true)
		return;

	if(m_oldSelectedColumns != m_selectedColumns)
	{
		OnSelectedColumnsChanged(&GrEventArgs::Empty);
	}

	if(m_oldSelectedRows != m_selectedRows)
	{
		OnSelectedRowsChanged(&GrEventArgs::Empty);
	}

	if(m_oldSelectedItems != m_selectedItems)
	{
		OnSelectionChanged(&GrEventArgs::Empty);
	}

	m_pGridCore->Invalidate(m_invalidate);
}

void GrItemSelector::AddInvalidatedRectangle(const GrRect& rect)
{
	if(m_invalidate.IsEmpty() == true)
	{
		m_invalidate = rect;
	}
	else
	{
		m_invalidate.left = std::min(m_invalidate.left, rect.left);
		m_invalidate.top = std::min(m_invalidate.top, rect.top);
		m_invalidate.right = std::max(m_invalidate.right, rect.right);
		m_invalidate.bottom = std::max(m_invalidate.bottom, rect.bottom);
	}
}

void GrItemSelector::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	ResetVariables();
}

void GrItemSelector::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	m_pHitTester = m_pGridCore->GetHitTester();

	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	pDataRowList->VisibleChanged.Add(this, &GrItemSelector::dataRowList_RowVisibleChanged);
}

void GrItemSelector::dataRowList_RowVisibleChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	OnSelectedRowsChanged(&GrEventArgs::Empty);
}

void GrItemSelector::BeginSelecting()
{
	if(m_bSelecting == true)
		EndSelecting(GrSelectionType_Normal);

	m_bSelecting = true;
	OnSelectingBegin(&GrEventArgs::Empty);
}

void GrItemSelector::Selecting(GrIndexRange visibleColumnIndex, GrIndexRange visibleRowIndex)
{
	if(m_bSelecting == false)
		return;
	m_pGridCore->Invalidate();
	SetColumnSelectingRange(visibleColumnIndex);
	SetRowSelectingRange(visibleRowIndex);
}

void GrItemSelector::SetColumnSelectingRange(GrIndexRange range)
{
	m_columnSelecting = range;
}

void GrItemSelector::SetRowSelectingRange(GrIndexRange range)
{
	m_rowSelecting = range;
}

void GrItemSelector::Selecting(GrRect rtSelection)
{
	if(m_bSelecting == false)
		return;

	m_pGridCore->Invalidate();
	GrIndexRange range;
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	GrInsertionRow* pInsertionRow = m_pGridCore->GetInsertionRow();

	if(pColumnList->HitTest(rtSelection.left, rtSelection.right, &range) == true)
		SetColumnSelectingRange(range);
	else
		SetColumnSelectingRange(GrIndexRange::Empty);

	if(GetRowAnchor() == pInsertionRow)
	{
		SetRowSelectingRange(GrIndexRange(INSERTION_ROW, INSERTION_ROW+1));
	}
	else
	{
		if(pDataRowList->HitTest(rtSelection.top, rtSelection.bottom, &range) == true)
			SetRowSelectingRange(range);
		else
			SetRowSelectingRange(GrIndexRange::Empty);
	}
}

void GrItemSelector::EndSelecting(GrSelectionType selectionType)
{
	if(m_bSelecting == false)
		return;

	GrItems items;
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

	for(uint y=m_rowSelecting.GetMinValue() ; y<m_rowSelecting.GetMaxValue() ; y++)
	{
		GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(y));
		if(pDataRow == NULL)
			continue;
		for(uint x=m_columnSelecting.GetMinValue() ; x<m_columnSelecting.GetMaxValue() ; x++)
		{
			GrColumn* pColumn = pColumnList->GetVisibleColumn(x);
			GrItem* pItem = pDataRow->GetItem(pColumn);
			assert(pItem);
			items.insert(pItem);
		}
	}

	SelectItems(&items, selectionType);

	m_columnSelecting = GrIndexRange::Empty;
	m_rowSelecting = GrIndexRange::Empty;

	m_bSelecting = false;
	OnSelectingEnd(&GrEventArgs::Empty);
}

bool GrItemSelector::IsSelecting() const
{
	return m_bSelecting;
}

bool GrItemSelector::IsSelecting(const GrColumn* pColumn) const
{
	if(m_bSelecting == false)
		return false;
	return m_columnSelecting.IsIn(pColumn->GetVisibleIndex());
}

bool GrItemSelector::IsSelecting(const GrDataRow* pDataRow) const
{
	if(m_bSelecting == false)
		return false;
	return m_rowSelecting.IsIn(pDataRow->GetVisibleIndex());
}

void GrItemSelector::SetAnchor(GrItem* pItem)
{
	SetColumnAnchor(pItem);
	SetRowAnchor(pItem);
}

void GrItemSelector::SetColumnAnchor(GrItem* pItem)
{
	SetColumnAnchor(pItem->GetColumn());
}

void GrItemSelector::SetColumnAnchor(GrColumn* pColumn)
{
	m_pAnchorColumn = pColumn;
}

void GrItemSelector::SetRowAnchor(GrItem* pItem)
{
	SetRowAnchor(pItem->GetDataRow());
}

void GrItemSelector::SetRowAnchor(IDataRow* pDataRow)
{
	m_pAnchorDataRow = pDataRow;
}

GrColumn* GrItemSelector::GetColumnAnchor() const
{
	if(m_pAnchorColumn == NULL)
	{
		GrColumnList* pColumnList = m_pGridCore->GetColumnList();
		uint nVisibleColumnCount = pColumnList->GetVisibleColumnCount();
		if(nVisibleColumnCount == 0)
			return NULL;

		return pColumnList->GetVisibleColumn(0);
	}
	return m_pAnchorColumn;
}

IDataRow* GrItemSelector::GetRowAnchor() const
{
	if(m_pAnchorDataRow == NULL)
	{
		GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
		uint nVisibleRowCount = pDataRowList->GetVisibleRowCount();
		if(nVisibleRowCount == 0)
			throw new std::exception();

		return pDataRowList->GetVisibleRow(0);
	}
	return m_pAnchorDataRow;
}

GrIndexRange GrItemSelector::GetColumnSelections(GrItem* pItem) const
{
	return GetColumnSelections(pItem->GetColumn());
}

GrIndexRange GrItemSelector::GetColumnSelections(GrColumn* pColumn) const
{
	GrIndexRange range(m_pAnchorColumn->GetVisibleIndex(), pColumn->GetVisibleIndex());
	range.SetMaxValue(range.GetMaxValue() + 1);
	return range;
}

GrIndexRange GrItemSelector::GetRowSelections(GrItem* pItem) const
{
	return GetRowSelections(pItem->GetDataRow());
}

GrIndexRange GrItemSelector::GetRowSelections(IDataRow* pDataRow) const
{
	if(pDataRow->GetSelectionGroup() != m_pAnchorDataRow->GetSelectionGroup())
		return GrIndexRange::Empty;

	GrIndexRange range(m_pAnchorDataRow->GetVisibleIndex(), pDataRow->GetVisibleIndex());
	range.SetMaxValue(range.GetMaxValue() + 1);
	return range;
}

GrTextUpdater::GrTextUpdater()
{
	m_nBaseCapacity = 50;
	m_vecTextBounds.reserve(m_nBaseCapacity);
	m_vecTextAligns.reserve(m_nBaseCapacity);
}

GrTextUpdater::~GrTextUpdater()
{
}


void GrTextUpdater::OnGridCoreAttached()
{
	m_pGridCore->Cleared.Add(this, &GrTextUpdater::gridCore_Cleared);
	m_pGridCore->CapacityChanged.Add(this, &GrTextUpdater::gridCore_CapacityChanged);
}

void GrTextUpdater::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	UpdateTextBound();
	UpdateTextAlign();
}

void GrTextUpdater::gridCore_CapacityChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	uint nCapacity = m_pGridCore->GetReservedColumn() * m_pGridCore->GetReservedRow() + m_nBaseCapacity;
	m_vecTextBounds.reserve(nCapacity);
	m_vecTextAligns.reserve(nCapacity);
}

void GrTextUpdater::AddTextBound(GrCell* pCell)
{
	if(pCell->m_bTextBound == true)
		return;
#ifdef _DEBUG
	if(m_vecTextBounds.capacity() == m_vecTextBounds.size())
#ifdef _MANAGED
		System::Console::WriteLine("용량 늘려 {0}", (int)m_vecTextBounds.capacity());
#else
		wprintf(L"용량 늘려 %d", (int)m_vecTextBounds.capacity());
#endif
#endif
	m_vecTextBounds.push_back(pCell);
	pCell->m_bTextBound = true;
}

void GrTextUpdater::AddTextBound(GrColumn* pColumn)
{
	GrInsertionRow* pInsertionRow = m_pGridCore->GetInsertionRow();
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	AddTextBound((GrCell*)pColumn);


	GrItem* pItem = pInsertionRow->GetItem(pColumn);
	AddTextBound(pItem);

	for(uint i=0 ; i<pDataRowList->GetDataRowCount() ; i++)
	{
		GrDataRow* pDataRow = pDataRowList->GetDataRow(i);
		pItem = pDataRow->GetItem(pColumn);
		AddTextBound(pItem);
	}
}

void GrTextUpdater::AddTextAlign(GrCell* pCell)
{
	if(pCell->m_bTextAlign == true)
		return;
	m_vecTextAligns.push_back(pCell);
	pCell->m_bTextAlign = true;
}

void GrTextUpdater::AddTextAlign(GrColumn* pColumn)
{
	GrInsertionRow* pInsertionRow = m_pGridCore->GetInsertionRow();
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	AddTextAlign((GrCell*)pColumn);


	GrItem* pItem = pInsertionRow->GetItem(pColumn);
	AddTextAlign(pItem);

	for(uint i=0 ; i<pDataRowList->GetDataRowCount() ; i++)
	{
		GrDataRow* pDataRow = pDataRowList->GetDataRow(i);
		pItem = pDataRow->GetItem(pColumn);
		AddTextAlign(pItem);
	}
}

void GrTextUpdater::RemoveTextBound(GrCell* pCell)
{
	if(pCell->m_bTextBound == false)
		return;
}

void GrTextUpdater::RemoveTextAlign(GrCell* pCell)
{
	if(pCell->m_bTextAlign == false)
		return;
}

void GrTextUpdater::UpdateTextBound()
{
	//uint nSize = m_vecTextBounds.size();
	for_stl_const(GrCells, m_vecTextBounds, itor)
	{
		GrCells::value_type value = *itor;

		value->ComputeTextBound();
		value->m_bTextBound = false;
		
		if(value->m_bTextAlign == false)
		{
			m_vecTextAligns.push_back(value);
			value->m_bTextAlign = true;
		}
	}
	m_vecTextBounds.clear();
}

void GrTextUpdater::UpdateTextAlign()
{
	for_stl_const(GrCells, m_vecTextAligns, itor)
	{
		GrCells::value_type value = *itor;
		value->AlignText();
		value->m_bTextAlign = false;
	}
	m_vecTextAligns.clear();
}