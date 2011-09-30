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


#pragma once
#include "GrGridBase.h"
#include "GrGridEvent.h"
#include "GrGridCell.h"

class GrFocuser : public GrObject
{
	typedef GrEventBase<GrEventArgs, GrFocuser>		_GrEvent;
public:
	GrFocuser();

	void					Set(IFocusable* pFocusable);
	void					Set(GrColumn* pColumn);
	void					Set(IDataRow* pDataRowBase);

	GrColumn*				GetLastFocusedColumn() const;
	IDataRow*				GetLastFocusedRow() const;

	GrColumn*				GetFocusedColumn() const;
	IDataRow*				GetFocusedRow() const;

	IFocusable*				Get() const;
	GrItem*					GetItem() const;
	bool					Has() const;

	void					SetFocusing(IFocusable* pFocusable);

public:
	_GrEvent				FocusChanging;
	_GrEvent				FocusChanged;

protected:
	virtual void			OnFocusChanging(GrEventArgs* e);
	virtual void			OnFocusChanged(GrEventArgs* e);

	virtual void			OnGridCoreAttached();

private:
	void					gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
	void					gridCore_Created(GrObject* pSender, GrEventArgs* e);
	void					itemSelector_SelectingEnd(GrObject* pSender, GrEventArgs* e);

	void					ResetVariables();

	GrColumn*				GetFocusableColumn() const;
	GrDataRow*				GetFocusableDataRow() const;

private:
	IFocusable*				m_pFocus;
	GrItem*					m_pFocusItem;

	GrItem*					m_pLastFocusedItem;
	GrColumn*				m_pLastFocusedColumn;
	IDataRow*				m_pLastFocusedRow;

	IFocusable*				m_pFocusing;
};

struct GrHitTest
{
	GrCell*			pHitted;
	GrPoint			ptLocal;
	GrRect			rtRect;
};

class GrHitTester : public GrObject
{
public:
	GrHitTester();

	bool			Test(GrPoint pt, GrHitTest* pHitTest) const;
	bool			Test(GrRect rtSelection, GrItems* pTested) const;
	bool			DisplayTest(GrPoint pt, GrHitTest* pHitTest) const;

protected:
	virtual void	OnGridCoreAttached();

private:
	void			gridCore_Created(GrObject* pSender, GrEventArgs* e);

private:
	GrColumnList*	m_pColumnList;
	GrDataRowList*	m_pDataRowList;

};	

typedef std::vector<GrColumn*>		GrSelectedColumns;
typedef std::vector<GrDataRow*>		GrSelectedRows;

class GrItemSelector : public GrObject
{
	typedef GrEventBase<GrEventArgs, GrItemSelector>		_GrEvent;

	typedef std::set<GrItem*>		_SelectedItems;
	typedef std::set<GrColumn*>		_SelectedColumns;
	typedef std::set<GrDataRow*>	_SelectedRows;
public:
	GrItemSelector();

	void					SelectItem(GrItem* pItem, GrSelectionType selectType);
	void					SelectItems(const GrItems* pItems, GrSelectionType selectType);
	void					SelectItems(GrRect rtSelection, GrSelectionType selectType);
	void					SelectItems(GrItem* pBegin, GrItem* pEnd, GrSelectionType selectType);
	void					SelectItems(GrIndexRange visibleColumnIndex, GrIndexRange visibleRowIndex, GrSelectionType selectType);
	void					SelectItems(GrColumn* pColumn, GrSelectionType selectType);
	void					SelectItems(GrDataRow* pDataRow, GrSelectionType selectType);
	void					SelectColumns(const GrColumns* pColumns, GrSelectionType selectType);
	void					SelectColumns(GrColumn* pFrom, GrColumn* pTo, GrSelectionType selectType);
	void					SelectColumns(GrRect rtSelection, GrSelectionType selectType);
	void					SelectDataRows(const GrDataRows* pDataRows, GrSelectionType selectType);
	void					SelectDataRows(GrDataRow* pFrom, GrDataRow* pTo, GrSelectionType selectType);
	void					SelectDataRows(IDataRow* pFrom, IDataRow* pTo, GrSelectionType selectType);
	void					SelectDataRows(GrRect rtSelection, GrSelectionType selectType);
	void					SelectAll();
	void					ClearSelection();

	const GrSelectedColumns*GetSelectedColumns() const;
	const GrSelectedRows*	GetSelectedRows() const;

	void					LockSelectionUpdate();
	void					UnlockSelectionUpdate();

	bool					CanSelect(IDataRow* pDataRow) const;
	bool					CanSelect(GrItem* pItem) const;
	void					SetSelectionGroup(uint nSelectionGroup);
	void					SetSelectionGroup(IDataRow* pDataRow);
	void					SetSelectionGroup(GrItem* pItem);

	// selecting
	void					BeginSelecting();
	void					Selecting(GrRect rtSelection);
	void					Selecting(GrIndexRange visibleColumnIndex, GrIndexRange visibleRowIndex);
	void					EndSelecting(GrSelectionType selectionType);
	bool					IsSelecting() const;
	bool					IsSelecting(const GrColumn* pColumn) const;
	bool					IsSelecting(const GrDataRow* pDataRow) const;

	// anchor
	void					SetAnchor(GrItem* pItem);
	void					SetColumnAnchor(GrItem* pItem);
	void					SetColumnAnchor(GrColumn* pColumn);

	void					SetRowAnchor(GrItem* pItem);
	void					SetRowAnchor(IDataRow* pDataRow);

	GrColumn*				GetColumnAnchor() const;
	IDataRow*				GetRowAnchor() const;

	GrIndexRange			GetColumnSelections(GrItem* pItem) const;
	GrIndexRange			GetColumnSelections(GrColumn* pColumn) const;
	GrIndexRange			GetRowSelections(GrItem* pItem) const;
	GrIndexRange			GetRowSelections(IDataRow* pDataRow) const;

public:		// event
	_GrEvent				SelectionChanged;
	_GrEvent				SelectedRowsChanged;
	_GrEvent				SelectedColumnsChanged;

	_GrEvent				SelectingBegin;
	_GrEvent				SelectingEnd;

protected:
	virtual void			OnSelectionChanged(GrEventArgs* e);
	virtual void			OnSelectedColumnsChanged(GrEventArgs* e);
	virtual void			OnSelectedRowsChanged(GrEventArgs* e);

	virtual void			OnSelectingBegin(GrEventArgs* e);
	virtual void			OnSelectingEnd(GrEventArgs* e);

	virtual void			OnGridCoreAttached();

private:
	void					DoSelect(GrItem* pItem);
	void					DoDeselect(GrItem* pItem);
	void					BeginSelection();
	void					EndSelection();

	void					AddInvalidatedRectangle(const GrRect& rect);

	void					ResetVariables();

	void					SetColumnSelectingRange(GrIndexRange range);
	void					SetRowSelectingRange(GrIndexRange range);

	void					gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
	void					gridCore_Created(GrObject* pSender, GrEventArgs* e);
	void					dataRowList_RowVisibleChanged(GrObject* pSender, GrEventArgs* e);

private:
	_SelectedItems			m_selectedItems;
	_SelectedColumns		m_selectedColumns;
	_SelectedRows			m_selectedRows;

	_SelectedItems			m_oldSelectedItems;
	_SelectedColumns		m_oldSelectedColumns;
	_SelectedRows			m_oldSelectedRows;


	GrSelectedColumns		m_externalSelectedColumns;
	GrSelectedRows			m_externalSelectedRows;

	bool					m_bSelectionLock;
	bool					m_bSelecting;
	uint					m_nSelectionGroup;

	GrIndexRange			m_columnSelecting;
	GrIndexRange			m_rowSelecting;

	GrColumn*				m_pAnchorColumn;
	IDataRow*				m_pAnchorDataRow;

	GrHitTester*			m_pHitTester;
	GrRect					m_invalidate;

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

typedef std::vector<GrCell*>		GrCells;

class GrTextUpdater : public GrObject
{
public:
	GrTextUpdater();
	virtual ~GrTextUpdater();

	void AddTextBound(GrCell* pCell);
	void AddTextBound(GrColumn* pColumn);
	void AddTextAlign(GrCell* pCell);
	void AddTextAlign(GrColumn* pColumn);

	void RemoveTextBound(GrCell* pCell);
	void RemoveTextAlign(GrCell* pCell);

	void UpdateTextBound();
	void UpdateTextAlign();

protected:
	virtual void OnGridCoreAttached();

private:
	void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
	void gridCore_CapacityChanged(GrObject* pSender, GrEventArgs* e);

private:
	GrCells					m_vecTextBounds;
	GrCells					m_vecTextAligns;

	uint					m_nBaseCapacity;
};

