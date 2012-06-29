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


#pragma once
#include "GrGridBase.h"
#include "GrGridEvent.h"
#include "GrGridCell.h"
#include "GrGridWindow.h"

class GrFocuser : public GrObject
{
    typedef GrEvent<GrFocusChangeArgs, GrFocuser> _GrEvent;
public:
    GrFocuser();

    void Set(IFocusable* pFocusable);
    void Set(GrColumn* pColumn);
    void Set(IDataRow* pDataRow);

    GrColumn* GetLastFocusedColumn() const;
    IDataRow* GetLastFocusedRow() const;

    GrColumn* GetFocusedColumn() const;
    IDataRow* GetFocusedRow() const;

    IFocusable* Get() const;
    GrItem* GetItem() const;
    bool Has() const;

public:
    _GrEvent FocusChanging;
    _GrEvent FocusChanged;

protected:
    virtual void OnFocusChanging(GrFocusChangeArgs* e);
    virtual void OnFocusChanged(GrFocusChangeArgs* e);
    virtual void OnGridCoreAttached();
    virtual IFocusable* GetFocusing() const;

private:
    void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void gridCore_Created(GrObject* pSender, GrEventArgs* e);

    void ResetVariables();
    void Invalidate(IFocusable* pFocusable);

    GrColumn* GetFocusableColumn() const;
    GrDataRow* GetFocusableDataRow() const;

private:
    IFocusable* m_pFocus;
    GrItem* m_pFocusItem;

    GrItem* m_pLastFocusedItem;
    GrColumn* m_pLastFocusedColumn;
    IDataRow* m_pLastFocusedRow;
};

typedef std::set<GrItem*> GrItems;
typedef std::set<GrColumn*> GrColumns;
typedef std::set<GrDataRow*> GrDataRows;

typedef std::vector<GrColumn*> GrSelectedColumns;
typedef std::vector<GrDataRow*> GrSelectedRows;

class GrItemSelector : public GrObject
{
    typedef GrEvent<GrEventArgs, GrItemSelector> _GrEvent;
public:
    GrItemSelector();

    void SelectItem(GrItem* pItem, GrSelectionType selectType);
    void SelectItems(const GrItems* pItems, GrSelectionType selectType);
    void SelectItems(GrItem* pBegin, GrItem* pEnd, GrSelectionType selectType);
    void SelectItems(GrIndexRange visibleColumnIndex, GrIndexRange visibleRowIndex, GrSelectionType selectType);
    void SelectColumn(GrColumn* pColumn, GrSelectionType selectType);
    void SelectDataRow(GrDataRow* pDataRow, GrSelectionType selectType);
    void SelectColumns(const GrColumns* pColumns, GrSelectionType selectType);
    void SelectColumns(GrColumn* pFrom, GrColumn* pTo, GrSelectionType selectType);
    void SelectDataRows(const GrDataRows* pDataRows, GrSelectionType selectType);
    void SelectDataRows(GrDataRow* pFrom, GrDataRow* pTo, GrSelectionType selectType);
    void SelectDataRows(IDataRow* pFrom, IDataRow* pTo, GrSelectionType selectType);
    void SelectAll();
    void ClearSelection();

    const GrSelectedColumns*GetSelectedColumns() const;
    const GrSelectedRows* GetSelectedRows() const;

    void LockSelectionUpdate();
    void UnlockSelectionUpdate();

    bool CanSelect(IDataRow* pDataRow) const;
    bool CanSelect(GrItem* pItem) const;
    void SetSelectionGroup(uint selectionGroup);
    void SetSelectionGroup(IDataRow* pDataRow);
    void SetSelectionGroup(GrItem* pItem);

    // anchor
    void SetAnchor(GrItem* pItem);
    void SetColumnAnchor(GrItem* pItem);
    void SetColumnAnchor(GrColumn* pColumn);

    void SetRowAnchor(GrItem* pItem);
    void SetRowAnchor(IDataRow* pDataRow);

    GrColumn* GetColumnAnchor() const;
    IDataRow* GetRowAnchor() const;

    GrIndexRange GetColumnSelections(GrItem* pItem) const;
    GrIndexRange GetColumnSelections(GrColumn* pColumn) const;
    GrIndexRange GetRowSelections(GrItem* pItem) const;
    GrIndexRange GetRowSelections(IDataRow* pDataRow) const;

public: // event
    _GrEvent SelectionChanged;
    _GrEvent SelectedRowsChanged;
    _GrEvent SelectedColumnsChanged;


protected:
    virtual void OnSelectionChanged(GrEventArgs* e);
    virtual void OnSelectedColumnsChanged(GrEventArgs* e);
    virtual void OnSelectedRowsChanged(GrEventArgs* e);

    virtual void OnGridCoreAttached();

protected:
    void DoSelect(GrItem* pItem);
    void DoDeselect(GrItem* pItem);
    void BeginSelection();
    void EndSelection();

    void DoSelectDataRow(GrDataRow* pDataRow);
    void DoDeselectDataRow(GrDataRow* pDataRow);

private:
    void AddInvalidatedRectangle(const GrRect& rect);

    void ResetVariables();

    void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void gridCore_Created(GrObject* pSender, GrEventArgs* e);
    void dataRowList_RowVisibleChanged(GrObject* pSender, GrEventArgs* e);

private:
    //GrItems m_selectedItems;
    GrColumns m_selectedColumns;
    GrDataRows m_selectedRows;

    //GrItems m_oldSelectedItems;
    GrColumns m_oldSelectedColumns;
    GrDataRows m_oldSelectedRows;


    GrSelectedColumns m_externalSelectedColumns;
    GrSelectedRows m_externalSelectedRows;

    bool m_selectionLock;
    bool m_selecting;
    uint m_selectionGroup;

    GrIndexRange m_columnSelecting;
    GrIndexRange m_rowSelecting;

    GrColumn* m_pAnchorColumn;
    IDataRow* m_pAnchorDataRow;

    GrRect m_invalidate;

    template<class T>
    class SortSelections
    {
    public:
        bool operator () (const T* p1, const T* p2)
        {
            return p1->GetVisibleIndex() < p2->GetVisibleIndex();
        }
    };
}; 

typedef std::vector<GrCell*> GrCells;

class GrTextUpdater : public GrObject
{
public:
    GrTextUpdater();
    virtual ~GrTextUpdater();

    void AddTextBounds(GrCell* pCell);
    void AddTextBoundsByColumn(GrColumn* pColumn);
    void AddTextAlign(GrCell* pCell);
    void AddTextAlignByColumn(GrColumn* pColumn);

    void RemoveTextBounds(GrCell* pCell);
    void RemoveTextAlign(GrCell* pCell);

    void UpdateTextBounds();
    void UpdateTextAlign();

protected:
    virtual void OnGridCoreAttached();

private:
    void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void gridCore_CapacityChanged(GrObject* pSender, GrEventArgs* e);

private:
    GrCells m_vecTextBounds;
    GrCells m_vecTextAligns;

    uint m_nBaseCapacity;
};

class GrScroll;

class GrFocusMover : public GrObject
{
    enum GrScrollEventType
    {
        GrScrollEventType_First,
        GrScrollEventType_Last,
        GrScrollEventType_SmallIncrement,
        GrScrollEventType_SmallDecrement,
        GrScrollEventType_LargeIncrement,
        GrScrollEventType_LargeDecrement,
    };

public:
    GrFocusMover();

    void FirstCell(GrSelectionRange range);
    void LastCell(GrSelectionRange range);
    void PageUp(GrSelectionRange range);
    void PageDown(GrSelectionRange range);
    void FirstRow(GrSelectionRange range);
    void LastRow(GrSelectionRange range);
    void MoveLeft(GrSelectionRange range);
    void MoveUp(GrSelectionRange range);
    void MoveRight(GrSelectionRange range);
    void MoveDown(GrSelectionRange range);

    void BringIntoView(IDataRow* pDataRow);
    void BringIntoView(GrColumn* pColumn);

protected:
    void OnGridCoreAttached();

private: // methods
    void focuser_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e);
    void gridCore_Created(GrObject* pSender, GrEventArgs* e);

    void SelectOne(IDataRow* pDataRow);
    void SelectMulti(IDataRow* pBegin, IDataRow* pEnd);

    bool DoHorzScroll(GrScrollEventType type);
    bool DoVertScroll(GrScrollEventType type);

private: // variables
    GrDataRowList* m_pDataRowList;
    GrColumnList* m_pColumnList;
    GrColumn* m_pLastDataColumn;
    GrScroll* m_pHorzScroll;
    GrScroll* m_pVertScroll;
    GrFocuser* m_pFocuser;
    GrItemSelector* m_pItemSelector;
    GrGridWindow* m_pGridWindow;
};