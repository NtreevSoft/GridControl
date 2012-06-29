//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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


#include "GrGridCoreComponents.h"
#include "GrGridCore.h"
#include "GrGridStateManager.h"
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

    GrFocusChangeArgs e(pFocusable);
    OnFocusChanging(&e);

    if(pFocusable)
    {
        m_pFocus = pFocusable;
        m_pLastFocusedRow = pFocusable->GetDataRow();
    }
    else
    {
        m_pFocus = NULL;
    }

    OnFocusChanged(&e);
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
    return m_pFocus->GetDataRow();
}

IFocusable* GrFocuser::Get() const
{
    IFocusable* pFocusing = GetFocusing();
    if(pFocusing != NULL)
        return pFocusing;
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

IFocusable* GrFocuser::GetFocusing() const
{
    return NULL;
}

void GrFocuser::Invalidate(IFocusable* pFocusable)
{
    if(pFocusable == NULL)
        return;

    GrRect rect;
    IDataRow* pDataRow = pFocusable->GetDataRow();
    if(m_pGridCore->GetRowHighlight() == true || m_pGridCore->GetFullRowSelect() == true)
    {
        rect = pDataRow->GetRect();
        rect.right = m_pGridCore->GetDisplayRect().right;
        rect.Expand(2, 2, 2, 2);
    }
    else
    {
        rect = pFocusable->GetDisplayRect();
        m_pGridCore->Invalidate(pDataRow->GetRect());
    }
    m_pGridCore->Invalidate(rect);
}

void GrFocuser::OnFocusChanging(GrFocusChangeArgs* e)
{
    FocusChanging(this, e);
    Invalidate(m_pFocus);
}

void GrFocuser::OnFocusChanged(GrFocusChangeArgs* e)
{
    FocusChanged(this, e);

    GrItem* pItem = dynamic_cast<GrItem*>(m_pFocus);
    if(pItem != NULL)
    {
        m_pLastFocusedColumn = pItem->GetColumn();
        m_pLastFocusedItem = pItem;
    }

    Invalidate(e->GetFocusable());
}

void GrFocuser::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    ResetVariables();
}

void GrFocuser::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{

}

void GrFocuser::ResetVariables()
{
    m_pFocus = NULL;
    m_pFocusItem = NULL;

    m_pLastFocusedItem = NULL;
    m_pLastFocusedColumn = NULL;
    m_pLastFocusedRow = NULL;
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
        throw _Exception("item is null");

    GrItems items;
    items.insert(pItem);

    if(selectType == GrSelectionType_Normal)
        SetSelectionGroup(pItem);
    SelectItems(&items, selectType);
}

void GrItemSelector::SelectItems(const GrItems* pItems, GrSelectionType selectType)
{
    if(m_pGridCore->GetMultiSelect() == false)
        return;

    BeginSelection();
    switch(selectType)
    {
    case GrSelectionType_Normal:
        {
           ClearSelection();

            for_each(GrItems, *pItems, value)
            {
                DoSelect(value);
            }
        }
        break;
    case GrSelectionType_Add:
        {
            for_each(GrItems, *pItems, value)
            {
                DoSelect(value);
            }
        }
        break;
    case GrSelectionType_Remove:
        {
            for_each(GrItems, *pItems, value)
            {
                DoDeselect(value);
            }
        }
        break;
    }
    EndSelection();
}

void GrItemSelector::SelectItems(GrItem* pBegin, GrItem* pEnd, GrSelectionType selectType)
{
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    GrItems items;

    uint beginColumn, endColumn, beginRow, endRow;

    beginColumn = pBegin->GetColumn()->GetVisibleIndex();
    endColumn = pEnd->GetColumn()->GetVisibleIndex();

    beginRow = pBegin->GetDataRow()->GetVisibleDataRowIndex();
    endRow = pEnd->GetDataRow()->GetVisibleDataRowIndex();

    if(beginColumn > endColumn)
    {
        std::swap(beginColumn, endColumn);
    }

    if(beginRow > endRow)
    {
        std::swap(beginRow, endRow);
    }

    for(uint i=beginRow ; i<=endRow ; i++)
    {
        GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
        for(uint j=beginColumn ; j<=endColumn ; j++)
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
    for(uint y = visibleRowIndex.GetMinValue() ; y < visibleRowIndex.GetMaxValue() ; y++)
    {
        GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(y));
        if(pDataRow == NULL)
            continue;

        for(uint x = visibleColumnIndex.GetMinValue() ; x < visibleColumnIndex.GetMaxValue() ; x++)
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
    BeginSelection();
    switch(selectType)
    {
    case GrSelectionType_Normal:
        {
            ClearSelection();

            for_each(GrDataRows, *pDataRows, value)
            {
                DoSelectDataRow(value);
            }
        }
        break;
    case GrSelectionType_Add:
        {
            for_each(GrDataRows, *pDataRows, value)
            {
                DoSelectDataRow(value);
            }
        }
        break;
    case GrSelectionType_Remove:
        {
            for_each(GrDataRows, *pDataRows, value)
            {
                DoDeselectDataRow(value);
            }
        }
        break;
    }
    EndSelection();
}

void GrItemSelector::SelectDataRows(GrDataRow* pFrom, GrDataRow* pTo, GrSelectionType selectType)
{
    if(pFrom->GetSelectionGroup() != pTo->GetSelectionGroup())
        throw _Exception("");

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
        throw _Exception("");

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

void GrItemSelector::SelectDataRow(GrDataRow* pDataRow, GrSelectionType selectType)
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
    for_each(GrColumns, *pColumns, value)
    {
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

void GrItemSelector::SelectColumn(GrColumn* pColumn, GrSelectionType selectType)
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
    m_pGridCore->Invalidate();
}

void GrItemSelector::ClearSelection()
{
    BeginSelection();
    for_each(GrDataRows, m_selectedRows, value)
    {
        DoDeselectDataRow(value);
    }

    for_each(GrColumns, m_selectedColumns, value)
    {
        value->m_selected = false;
        value->m_selectedCells.clear();
    }
    m_selectedColumns.clear();
    m_selectedRows.clear();
    EndSelection();
}

void GrItemSelector::ResetVariables()
{
    //m_selectedItems.clear();
    m_selectedColumns.clear();
    m_selectedRows.clear();

    m_selectionLock = false;
    m_selecting = false;
    m_selectionGroup = 0;

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
    if(m_selectionLock == true)
        throw "already locked";

    BeginSelection();
    m_selectionLock = true;
}

void GrItemSelector::UnlockSelectionUpdate()
{
    if(m_selectionLock == false)
        throw "did not locked";
    m_selectionLock = false;
    EndSelection();
}

bool GrItemSelector::CanSelect(IDataRow* pDataRow) const
{
    return pDataRow->GetSelectionGroup() == m_selectionGroup ? true : false;
}

bool GrItemSelector::CanSelect(GrItem* pItem) const
{
    return CanSelect(pItem->GetDataRow());
}

void GrItemSelector::SetSelectionGroup(uint selectionGroup)
{
    m_selectionGroup = selectionGroup;
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

void GrItemSelector::DoSelect(GrItem* pItem)
{
    if(pItem->m_selected == true)
        return;

    GrColumn* pColumn = pItem->GetColumn();
    GrDataRow* pDataRow = pItem->GetDataRow();
    if(pDataRow->GetSelectionGroup() != m_selectionGroup)
        return;

    pItem->m_selected = true;
    pDataRow->AddSelection(pItem);
    pColumn->AddSelection(pItem);

    m_selectedRows.insert(pDataRow);
    m_selectedColumns.insert(pColumn);

    if(pItem->GetDisplayable() == true)
        AddInvalidatedRectangle(pItem->GetDisplayRect());
}

void GrItemSelector::DoDeselect(GrItem* pItem)
{
    if(pItem->m_selected == false)
        return;

    GrColumn* pColumn = pItem->GetColumn();
    GrDataRow* pDataRow = pItem->GetDataRow();

    pItem->m_selected = false;
    pDataRow->RemoveSelection(pItem);
    pColumn->RemoveSelection(pItem);

    if(pDataRow->GetSelected() == false)
        m_selectedRows.erase(pDataRow);

    if(pColumn->GetSelected() == false)
        m_selectedColumns.erase(pColumn);

    if(pItem->GetDisplayable() == true)
        AddInvalidatedRectangle(pItem->GetDisplayRect());
}

void GrItemSelector::BeginSelection()
{
    if(m_selectionLock == true)
        return;
    m_oldSelectedColumns = m_selectedColumns;
    m_oldSelectedRows = m_selectedRows;

    m_invalidate.DoEmpty();
}

void GrItemSelector::EndSelection()
{
    if(m_selectionLock == true)
        return;

    bool changed = false;
    if(m_oldSelectedColumns != m_selectedColumns)
    {
        OnSelectedColumnsChanged(&GrEventArgs::Empty);
        changed = true;
    }

    if(m_oldSelectedRows != m_selectedRows)
    {
        OnSelectedRowsChanged(&GrEventArgs::Empty);
        changed = true;
    }

    if(changed== true)
    {
        OnSelectionChanged(&GrEventArgs::Empty);
    }

    m_pGridCore->Invalidate(m_invalidate);
}

void GrItemSelector::DoSelectDataRow(GrDataRow* pDataRow)
{
    if(pDataRow->GetFullSelected() == true)
        return;

    if(pDataRow->GetSelectionGroup() != m_selectionGroup)
        return;

    m_selectedRows.insert(pDataRow);
    pDataRow->SetFullSelected();

    if(pDataRow->GetDisplayable() == true)
    {
        GrRect dataRowRect = pDataRow->GetRect();
        dataRowRect.right = m_pGridCore->GetDisplayRect().right;
        AddInvalidatedRectangle(dataRowRect);
    }
}

void GrItemSelector::DoDeselectDataRow(GrDataRow* pDataRow)
{
    if(pDataRow->GetSelected() == false)
        return;

    for_each(std::set<GrItem*>, pDataRow->m_selectedCells, item)
    {
        item->m_selected = false;
    }
    
    pDataRow->ClearSelection();
    m_selectedRows.erase(pDataRow);

    if(pDataRow->GetDisplayable() == true)
    {
        GrRect dataRowRect = pDataRow->GetRect();
        dataRowRect.right = m_pGridCore->GetDisplayRect().right;
        AddInvalidatedRectangle(dataRowRect);
    }
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
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    pDataRowList->VisibleChanged.Add(this, &GrItemSelector::dataRowList_RowVisibleChanged);
}

void GrItemSelector::dataRowList_RowVisibleChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    OnSelectedRowsChanged(&GrEventArgs::Empty);
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
        uint visibleColumnCount = pColumnList->GetVisibleColumnCount();
        if(visibleColumnCount == 0)
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
        uint visibleRowCount = pDataRowList->GetVisibleRowCount();
        if(visibleRowCount == 0)
            throw _Exception("");

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
    UpdateTextBounds();
    UpdateTextAlign();
}

void GrTextUpdater::gridCore_CapacityChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    uint capacity = m_pGridCore->GetReservedColumn() * m_pGridCore->GetReservedRow() + m_nBaseCapacity;
    m_vecTextBounds.reserve(capacity);
    m_vecTextAligns.reserve(capacity);
}

void GrTextUpdater::AddTextBounds(GrCell* pCell)
{
    if(pCell->m_textBoundsChanged == true)
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
    pCell->m_textBoundsChanged = true;
}

void GrTextUpdater::AddTextBoundsByColumn(GrColumn* pColumn)
{
    GrInsertionRow* pInsertionRow = m_pGridCore->GetInsertionRow();
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    AddTextBounds((GrCell*)pColumn);

    GrItem* pItem = pInsertionRow->GetItem(pColumn);
    AddTextBounds(pItem);

    for(uint i=0 ; i<pDataRowList->GetDataRowCount() ; i++)
    {
        GrDataRow* pDataRow = pDataRowList->GetDataRow(i);
        pItem = pDataRow->GetItem(pColumn);
        AddTextBounds(pItem);
    }
}

void GrTextUpdater::AddTextAlign(GrCell* pCell)
{
    if(pCell->m_textAlignChanged == true)
        return;
    m_vecTextAligns.push_back(pCell);
    pCell->m_textAlignChanged = true;
}

void GrTextUpdater::AddTextAlignByColumn(GrColumn* pColumn)
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

void GrTextUpdater::RemoveTextBounds(GrCell* pCell)
{
    GrCells::iterator itor = std::find(m_vecTextBounds.begin(), m_vecTextBounds.end(), pCell);
    if(itor != m_vecTextBounds.end())
    {
        pCell->m_textBoundsChanged = false;
        m_vecTextBounds.erase(itor);
    }
}

void GrTextUpdater::RemoveTextAlign(GrCell* pCell)
{
    GrCells::iterator itor = std::find(m_vecTextAligns.begin(), m_vecTextAligns.end(), pCell);
    if(itor != m_vecTextAligns.end())
    {
        pCell->m_textAlignChanged = false;
        m_vecTextAligns.erase(itor);
    }
}

void GrTextUpdater::UpdateTextBounds()
{
    for_each(GrCells, m_vecTextBounds, value)
    {
        value->ComputeTextBounds();
        value->m_textBoundsChanged = false;

        if(value->m_textAlignChanged == false)
        {
            m_vecTextAligns.push_back(value);
            value->m_textAlignChanged = true;
        }
    }
    m_vecTextBounds.clear();
}

void GrTextUpdater::UpdateTextAlign()
{
    for_each(GrCells, m_vecTextAligns, value)
    {
        value->AlignText();
        value->m_textAlignChanged = false;
    }
    m_vecTextAligns.clear();
}

GrFocusMover::GrFocusMover()
{

}

void GrFocusMover::OnGridCoreAttached()
{
    GrObject::OnGridCoreAttached();
    m_pGridCore->Created.Add(this, &GrFocusMover::gridCore_Created);
}

void GrFocusMover::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_pDataRowList = m_pGridCore->GetDataRowList(); 
    m_pColumnList = m_pGridCore->GetColumnList();
    m_pHorzScroll = m_pGridCore->GetHorzScroll();
    m_pVertScroll = m_pGridCore->GetVertScroll();
    m_pFocuser = m_pGridCore->GetFocuser();
    m_pItemSelector= m_pGridCore->GetItemSelector();
    m_pGridWindow = m_pGridCore->GetGridWindow();

    m_pFocuser->FocusChanged.Add(this, &GrFocusMover::focuser_FocusChanged);
}

void GrFocusMover::focuser_FocusChanged(GrObject* /*pSender*/, GrFocusChangeArgs* /*e*/)
{
    GrItem* pItem = m_pFocuser->GetItem();
    if(pItem != NULL)
    {
        m_pLastDataColumn = pItem->GetColumn();
    }
}

void GrFocusMover::FirstCell(GrSelectionRange range)
{
    GrItem* pFocused = m_pFocuser->GetItem();
    if(pFocused == NULL)
        return;

    GrColumn* pColumn = pFocused->GetColumn();
    GrDataRow* pDataRow = pFocused->GetDataRow();

    GrColumnList* m_pColumnList = pColumn->GetColumnList();

    if(pColumn->GetFrozen() == true)
    {
        uint index = pColumn->GetFrozenIndex();
        if(index == 0)
            return;

        index = index - 1;
        pColumn = m_pColumnList->GetFrozenColumn(0);
    }
    else
    {
        uint frozenColumnCount = m_pColumnList->GetFrozenColumnCount();

        uint index = pColumn->GetUnfrozenIndex();
        if(index == 0 && frozenColumnCount > 0)
        {
            pColumn = m_pColumnList->GetFrozenColumn(0);
        }
        else
        {
            pColumn = m_pColumnList->GetUnfrozenColumn(0);
        }
    }

    pFocused = pDataRow->GetItem(pColumn);

    if(range == GrSelectionRange_One)
    {
        m_pItemSelector->SetColumnAnchor(pColumn);
        m_pItemSelector->SelectItem(pFocused, m_pGridWindow->GetSelectionType());
    }
    else
    {
        m_pItemSelector->SelectItems(
            m_pItemSelector->GetColumnSelections(pFocused), 
            m_pItemSelector->GetRowSelections(pFocused),
            m_pGridWindow->GetSelectionType());
    }
    m_pFocuser->Set(pFocused);
    DoHorzScroll(GrScrollEventType_First);
    BringIntoView(pColumn);
}

void GrFocusMover::LastCell(GrSelectionRange range)
{
    GrItem* pFocused = m_pFocuser->GetItem();
    if(pFocused == NULL)
        return;

    GrColumn* pColumn = pFocused->GetColumn();
    GrDataRow* pDataRow = pFocused->GetDataRow();

    GrColumnList* m_pColumnList = pColumn->GetColumnList();

    uint visibleColumnCount = m_pColumnList->GetVisibleColumnCount();
    uint index = visibleColumnCount - 1;

    if(index == pColumn->GetVisibleIndex())
        return;

    pColumn = m_pColumnList->GetVisibleColumn(index);
    pFocused = pDataRow->GetItem(pColumn);

    if(range == GrSelectionRange_One)
    {
        m_pItemSelector->SelectItem(pFocused, m_pGridWindow->GetSelectionType());
        m_pItemSelector->SetAnchor(pFocused);
    }
    else
    {
        m_pItemSelector->SelectItems(
            m_pItemSelector->GetColumnSelections(pFocused), 
            m_pItemSelector->GetRowSelections(pFocused),
            m_pGridWindow->GetSelectionType());
    }
    m_pFocuser->Set(pFocused);

    DoHorzScroll(GrScrollEventType_Last);
    BringIntoView(pColumn);
}

void GrFocusMover::PageUp(GrSelectionRange range)
{
    IFocusable* pFocused = m_pFocuser->Get();
    if(pFocused == NULL)
        return;
    if(m_pDataRowList->GetVisibleRowCount() == 0)
        return;

    IDataRow* pDataRow = pFocused->GetDataRow();
    uint index = pDataRow->GetVisibleIndex();

    switch(index)
    {
    case 0:
        if(m_pGridCore->GetInsertionRowVisible() == true)
            index = INSERTION_ROW;
        else
            return;
        break;
    case INSERTION_ROW:
        return;
    default:
        index = m_pDataRowList->ClipTo(index);
        break;
    }

    IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(index);

    if(range == GrSelectionRange_One)
        SelectOne(pNewDataRow);
    else
        SelectMulti(pDataRow, pNewDataRow);

    if(pDataRow->GetDisplayable() == true)
        DoVertScroll(GrScrollEventType_LargeDecrement);
    else
        BringIntoView(pNewDataRow);
}

void GrFocusMover::PageDown(GrSelectionRange range)
{
    IFocusable* pFocused = m_pFocuser->Get();
    if(pFocused == NULL)
        return;
    if(m_pDataRowList->GetVisibleRowCount() == 0)
        return;

    IDataRow* pDataRow = pFocused->GetDataRow();
    uint index = pDataRow->GetVisibleIndex();
    uint newIndex;

    if(index == INSERTION_ROW)
        newIndex = 0;
    else
        newIndex = m_pDataRowList->ClipFrom(index);

    if(newIndex == index)
    {
        if(newIndex + 1 != m_pDataRowList->GetVisibleRowCount())
            newIndex++;
    }

    IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(newIndex);

    if(range == GrSelectionRange_One)
        SelectOne(pNewDataRow);
    else
        SelectMulti(pDataRow, pNewDataRow);

    if(pDataRow != m_pGridCore->GetInsertionRow() && pDataRow->GetDisplayable() == true)
        DoVertScroll(GrScrollEventType_LargeIncrement);
    else
        BringIntoView(pNewDataRow);
}

void GrFocusMover::FirstRow(GrSelectionRange range)
{
    IFocusable* pFocused = m_pFocuser->Get();
    if(pFocused == NULL)
        return;
    if(m_pDataRowList->GetVisibleRowCount() == 0)
        return;

    IDataRow* pDataRow = pFocused->GetDataRow();
    uint newIndex = 0;

    IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(newIndex);

    if(range == GrSelectionRange_One)
        SelectOne(pNewDataRow);
    else
        SelectMulti(pDataRow, pNewDataRow);

    DoVertScroll(GrScrollEventType_First);
}

void GrFocusMover::LastRow(GrSelectionRange range)
{
    IFocusable* pFocused = m_pFocuser->Get();
    if(pFocused == NULL)
        return;
    if(m_pDataRowList->GetVisibleRowCount() == 0)
        return;

    IDataRow* pDataRow = pFocused->GetDataRow();
    uint newIndex = m_pDataRowList->GetVisibleRowCount() - 1;

    IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(newIndex);

    if(range == GrSelectionRange_One)
        SelectOne(pNewDataRow);
    else
        SelectMulti(pDataRow, pNewDataRow);

    DoVertScroll(GrScrollEventType_Last);
}

void GrFocusMover::MoveUp(GrSelectionRange range)
{
    IFocusable* pFocused = m_pFocuser->Get();
    if(pFocused == IFocusable::Null)
        return;
    if(m_pDataRowList->GetVisibleRowCount() == 0)
        return;

    IDataRow* pDataRow = pFocused->GetDataRow();
    uint index = pDataRow->GetVisibleIndex();

    switch(index)
    {
    case 0:
        if(m_pGridCore->GetInsertionRowVisible() == true)
            index = INSERTION_ROW;
        else
            return;
        break;
    case INSERTION_ROW:
        return;
    default:
        index = index - 1;
        break;
    }

    IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(index);

    if(range == GrSelectionRange_One)
        SelectOne(pNewDataRow);
    else
        SelectMulti(pDataRow, pNewDataRow);

    BringIntoView(pNewDataRow);
}

void GrFocusMover::MoveDown(GrSelectionRange range)
{
    IFocusable* pFocused = m_pFocuser->Get();
    if(pFocused == IFocusable::Null)
        return;

    if(m_pDataRowList->GetVisibleRowCount() == 0)
        return;

    IDataRow* pDataRow = pFocused->GetDataRow();
    uint index = pDataRow->GetVisibleIndex();
    uint lastIndex = m_pDataRowList->GetVisibleRowCount() - 1;

    if(index == lastIndex)
        return;
    else if(index == INSERTION_ROW)
        index = 0;
    else
        index = index + 1;

    IDataRow* pNewDataRow = m_pDataRowList->GetVisibleRow(index);

    if(range == GrSelectionRange_One)
        SelectOne(pNewDataRow);
    else
        SelectMulti(pDataRow, pNewDataRow);

    BringIntoView(pNewDataRow);
}

void GrFocusMover::MoveLeft(GrSelectionRange range)
{
    GrItem* pFocused = m_pFocuser->GetItem();
    if(pFocused == NULL)
        return;

    GrColumn* pColumn = pFocused->GetColumn();
    GrDataRow* pDataRow = pFocused->GetDataRow();

    GrColumnList* m_pColumnList = pColumn->GetColumnList();

    uint index = pColumn->GetVisibleIndex();
    if(index != 0)
        index = index - 1;

    if(index == pColumn->GetVisibleIndex())
        return;

    pColumn = m_pColumnList->GetVisibleColumn(index);
    pFocused = pDataRow->GetItem(pColumn);

    if(range == GrSelectionRange_One)
    {
        if(m_pGridCore->GetFullRowSelect() == true)
            m_pItemSelector->SelectDataRow(pDataRow, GrSelectionType_Normal);
        else
            m_pItemSelector->SelectItem(pFocused, GrSelectionType_Normal);
        m_pItemSelector->SetAnchor(pFocused);
    }
    else
    {
        m_pItemSelector->SelectItems(
            m_pItemSelector->GetColumnSelections(pFocused), 
            m_pItemSelector->GetRowSelections(pFocused),
            m_pGridWindow->GetSelectionType());
    }
    m_pFocuser->Set(pFocused);

    BringIntoView(pColumn);
}

void GrFocusMover::MoveRight(GrSelectionRange range)
{
    GrItem* pFocused = m_pFocuser->GetItem();
    if(pFocused == NULL)
        return;

    GrColumn* pColumn = pFocused->GetColumn();
    GrDataRow* pDataRow = pFocused->GetDataRow();

    GrColumnList* m_pColumnList = pColumn->GetColumnList();

    uint visibleColumnCount = m_pColumnList->GetVisibleColumnCount();
    uint index = pColumn->GetVisibleIndex() + 1;
    if(index >= visibleColumnCount)
        index = visibleColumnCount - 1;

    if(index == pColumn->GetVisibleIndex())
        return;

    pColumn = m_pColumnList->GetVisibleColumn(index);
    pFocused = pDataRow->GetItem(pColumn);

    if(range == GrSelectionRange_One)
    {
        if(m_pGridCore->GetFullRowSelect() == true)
            m_pItemSelector->SelectDataRow(pDataRow, GrSelectionType_Normal);
        else
            m_pItemSelector->SelectItem(pFocused, GrSelectionType_Normal);
        m_pItemSelector->SetAnchor(pFocused);
    }
    else
    {
        m_pItemSelector->SelectItems(
            m_pItemSelector->GetColumnSelections(pFocused), 
            m_pItemSelector->GetRowSelections(pFocused),
            m_pGridWindow->GetSelectionType());
    }
    m_pFocuser->Set(pFocused);

    BringIntoView(pColumn);
}


void GrFocusMover::SelectOne(IDataRow* pDataRow)
{
    switch(pDataRow->GetRowType())
    {
    case GrRowType_InsertionRow:
    case GrRowType_DataRow:
        {
            GrDataRow* pRow = dynamic_cast<GrDataRow*>(pDataRow);
            GrItem* pOldFocusItem = m_pFocuser->GetItem();
            GrItem* pFocusItem;

            GrColumn* pColumn;

            if(pOldFocusItem != NULL)
                pColumn = pOldFocusItem->GetColumn();
            else
                pColumn = m_pItemSelector->GetColumnAnchor();

            if(pColumn == NULL)
            {
                GrColumnList* m_pColumnList = m_pGridCore->GetColumnList();
                pColumn = m_pColumnList->GetVisibleColumn(0);
            }
            pFocusItem = pRow->GetItem(pColumn);

            if(m_pGridCore->GetFullRowSelect() == true)
                m_pItemSelector->SelectDataRow(pRow, GrSelectionType_Normal);
            else
                m_pItemSelector->SelectItem(pFocusItem, GrSelectionType_Normal);
            m_pItemSelector->SetAnchor(pFocusItem);
            m_pFocuser->Set(pFocusItem);
        }
        break;
    default:
        {
            m_pItemSelector->ClearSelection();
            m_pFocuser->Set(pDataRow->GetFocusable(NULL));
        }
        break;
    }
    m_pItemSelector->SetSelectionGroup(pDataRow);
}

void GrFocusMover::SelectMulti(IDataRow* pBegin, IDataRow* pEnd)
{
    if(pBegin->GetRowType() == GrRowType_InsertionRow || pEnd->GetRowType() == GrRowType_InsertionRow)
        return;

    GrIndexRange columnRange(m_pItemSelector->GetColumnAnchor()->GetVisibleIndex(), m_pLastDataColumn->GetVisibleIndex());
    GrIndexRange rowRange;

    IDataRow* pRowAnchor = m_pItemSelector->GetRowAnchor();
    if(pRowAnchor->GetRowType() == GrRowType_InsertionRow)
    {
        m_pItemSelector->SetRowAnchor(pBegin);
        rowRange = GrIndexRange(pBegin->GetVisibleIndex(), pEnd->GetVisibleIndex());
    }
    else
    {
        rowRange = GrIndexRange(pRowAnchor->GetVisibleIndex(), pEnd->GetVisibleIndex());
    }

    columnRange.SetMaxValue(columnRange.GetMaxValue() + 1);
    rowRange.SetMaxValue(rowRange.GetMaxValue() + 1);

    m_pItemSelector->SelectItems(columnRange, rowRange, m_pGridWindow->GetSelectionType());

    switch(pEnd->GetRowType())
    {
    case GrRowType_DataRow:
        {
            GrDataRow* pRow = dynamic_cast<GrDataRow*>(pEnd);
            GrItem* pOldFocusItem = m_pFocuser->GetItem();
            GrItem* pFocusItem;

            GrColumn* pColumn;

            if(pOldFocusItem != NULL)
                pColumn = pOldFocusItem->GetColumn();
            else
                pColumn = m_pLastDataColumn;

            if(pColumn == NULL)
            {
                GrColumnList* m_pColumnList = m_pGridCore->GetColumnList();
                pColumn = m_pColumnList->GetVisibleColumn(0);
            }
            pFocusItem = pRow->GetItem(pColumn);

            m_pFocuser->Set(pFocusItem);
        }
        break;
    default:
        {
            m_pFocuser->Set(pEnd->GetFocusable(0));
        }
        break;
    }
}

void GrFocusMover::BringIntoView(IDataRow* pDataRow)
{
    m_pDataRowList->BringIntoView(pDataRow);
}

void GrFocusMover::BringIntoView(GrColumn* pColumn)
{
    m_pColumnList->BringIntoView(pColumn);
}

bool GrFocusMover::DoHorzScroll(GrScrollEventType type)
{
    GrColumnList* m_pColumnList = m_pGridCore->GetColumnList();

    int value = m_pHorzScroll->GetValue();
    switch(type)
    {
    case GrScrollEventType_First:
        value = m_pHorzScroll->GetMinimum();
        break;
    case GrScrollEventType_SmallDecrement:
        value -= m_pHorzScroll->GetSmallChange();
        break;
    case GrScrollEventType_LargeDecrement:
        {
            int oldValue = value;
            value = (int)m_pColumnList->ClipTo((uint)value);
            if(value == oldValue)
            {
                if(value != 0)
                    value--;
            }
        }
        break;
    case GrScrollEventType_LargeIncrement:
        {
            int oldValue = value;
            value = (int)m_pColumnList->ClipFrom((uint)value);
            if(value == oldValue)
            {
                if((uint)value + 1 != m_pColumnList->GetVisibleColumnCount())
                    value++;
            }
        }
        break;
    case GrScrollEventType_SmallIncrement:
        value += m_pHorzScroll->GetSmallChange();
        break;
    case GrScrollEventType_Last:
        value = m_pHorzScroll->ValidateValue(m_pHorzScroll->GetMaximum());
        break;
    default:
        throw _Exception("");
    }

    return m_pHorzScroll->DoScroll(value);
}

bool GrFocusMover::DoVertScroll(GrScrollEventType type)
{
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

    int value = m_pVertScroll->GetValue();
    switch(type)
    {
    case GrScrollEventType_First:
        value = m_pVertScroll->GetMinimum();
        break;
    case GrScrollEventType_SmallDecrement:
        value -= m_pVertScroll->GetSmallChange();
        break;
    case GrScrollEventType_LargeDecrement:
        {
            int oldValue = value;
            value = (int)pDataRowList->ClipTo((uint)value);
            if(value == oldValue)
            {
                if(value != 0)
                    value--;
            }
        }
        break;
    case GrScrollEventType_LargeIncrement:
        {
            int oldValue = value;
            value = (int)pDataRowList->ClipFrom((uint)value);
            if(value == oldValue)
            {
                if((uint)value + 1 != pDataRowList->GetVisibleRowCount())
                    value++;
            }
        }
        break;
    case GrScrollEventType_SmallIncrement:
        value += m_pVertScroll->GetSmallChange();
        break;
    case GrScrollEventType_Last:
        value = m_pVertScroll->ValidateValue(m_pVertScroll->GetMaximum());
        break;
    default:
        throw _Exception("");
    }

    return m_pVertScroll->DoScroll(value);
}