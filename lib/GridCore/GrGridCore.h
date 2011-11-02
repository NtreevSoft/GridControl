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


#pragma once
#include "GrGridBase.h"
#include "GrGridCell.h"
#include "GrGridRenderer.h"
#include "GrGridCoreComponents.h"

class GrItemSelector;
class GrFocuser;
class GrHitTester;

class GrGridCore : public GrObject
{
	typedef GrEventBase<GrEventArgs, GrGridCore>		_GrEvent;
	typedef GrEventBase<GrColumnEventArgs, GrGridCore>	_GrColumnEvent;
	typedef GrEventBase<GrRowEventArgs, GrGridCore>		_GrRowEvent;

public:
	GrGridCore(void);
	virtual ~GrGridCore(void);

	void					Reserve(uint nColumnCount, uint nRowCount);
	uint					GetReservedColumn() const;
	uint					GetReservedRow() const;

	void					SetAutoFitColumn(bool b);
	void					SetAutoFitRow(bool b);
	bool					GetAutoFitColumn() const;
	bool					GetAutoFitRow() const;

	void					SetColumnSplitter(int value);
	void					SetRowSplitter(int value);
	int						GetColumnSplitter() const;
	int						GetRowSplitter() const;

	void					Clear();

	bool					SetMouseOver(GrCell* pCell, GrPoint ptLocalPos);
	GrCell*					GetMouseOver() const;
	GrMouseOverState		GetMouseOverState() const;
	void					SetMousePressed(GrCell* pCell);
	void					SetMouseUnpressed();
	GrCell*					GetMousePressed() const;

	bool					GetMarginVisible() const;
	void					SetMarginVisible(bool b);
	bool					GetInsertionRowVisible() const;
	bool					GetRowHighlight() const;
	void					SetRowHighlight(bool b);
	GrRowHighlightType		GetRowHighlightType() const;
	void					SetRowHighlightType(GrRowHighlightType type);

	bool					Update();

	GrRootRow*				GetRoot() const { return m_pRootRow; }

	GrDataRowList*			GetDataRowList() const { return m_pDataRowList; }
	GrColumnList*			GetColumnList() const { return m_pColumnList; }
	GrInsertionRow*			GetInsertionRow() const { return m_pInsertionRow; }
	GrCaption*				GetCaptionRow() const { return m_pCaption; }
	GrGroupingList*			GetGroupingList() const { return m_pGroupingList; }
	GrItemSelector*			GetItemSelector() const { return m_pItemSelector; }
	GrFocuser*				GetFocuser() const { return m_pFocuser; }
	GrTextUpdater*			GetTextUpdater() const { return m_pTextUpdater; }
	GrHitTester*			GetHitTester() const { return m_pHitTester; }
	GrGridInvalidator*		GetInvalidator() const { return m_pInvalidator == NULL ? m_pDefaultInvalidator : m_pInvalidator; }

	const GrRect*			GetDisplayRect() const;
	void					SetDisplayRect(GrRect rtDisplay);

	GrRect					GetDisplayableRect() const;

	bool					GetFullRowSelect() const;
	void					SetFullRowSelect(bool b);
	bool					GetSelectionVisible() const;
	void					SetSelectionVisible(bool b);

	void					SetInvalidator(GrGridInvalidator* pInvalidator) { m_pInvalidator = pInvalidator; }

	void					Clip(uint nStartCol, uint nStartRow);
	void					Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;	

	bool					IsGrouped() const;
	bool					IsUpdating() const { return m_bUpdating; }

	bool					CanBeGrouped() const;
	void					EnableGrouping(bool b) const;

	int						GetGroupingMargin() const;
	void					ShowClippedText(bool bShow = true);

	void 					Invalidate();
	void 					Invalidate(int x, int y, int width, int height);
	void 					Invalidate(const GrRect& rect);
	void					LockInvalidate();
	void					UnlockInvalidate();
	void					ResetInvalidate();
	bool					IsInvalidated() const;

	_GrEvent				Created;
	_GrEvent				Cleared;
	_GrEvent				GroupingChanged;
	_GrEvent				CapacityChanged;

	void					OnFontChanged();
	GrFont*					GetFont() const;
	GrFont*					GetFont(void* hFont) const;
	GrFont*					GetDefaultFont() const;
	void					SetFont(GrFont* pFont);
#ifdef _MANAGED
	void					SetFont(System::Drawing::Font^ font);
#endif
	GrStyle*				GetStyle() const;
	void					SetStyle(GrStyle* pStyle);

	void					AttachObject(GrObject* pObject);
	void					DetachObject(GrObject* pObject);

protected:
	virtual void			OnCreated(GrEventArgs* e);
	virtual void			OnCleared(GrEventArgs* e);
	virtual void			OnCapacityChanged(GrEventArgs* e);

private:
	GrItem*					GetFirstVisibleItem() const;

	void					focusSetter_FocusChanging(GrObject* pSender, GrEventArgs* e);
	void					focusSetter_FocusChanged(GrObject* pSender, GrEventArgs* e);

	void					columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnWidthChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnWordwrapChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnHorzAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnVertAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnPaddingChanged(GrObject* pSender, GrColumnEventArgs* e);

	void					groupingList_Changed(GrObject* pSender, GrEventArgs* e);

private:
	GrRootRow*				m_pRootRow;
	GrCaption*				m_pCaption;
	GrGroupingList*			m_pGroupingList;
	GrColumnList*			m_pColumnList;
	GrInsertionRow*			m_pInsertionRow;
	GrRowSplitter*			m_pSplitterRow;
	GrDataRowList*			m_pDataRowList;

	GrGridInvalidator*		m_pInvalidator;
	GrGridInvalidator*		m_pDefaultInvalidator;

	GrRect					m_rtDisplay;

	int						m_nGroupingMargin;

	GrItemSelector*			m_pItemSelector;
	GrFocuser*				m_pFocuser;
	GrHitTester*			m_pHitTester;
	GrTextUpdater*			m_pTextUpdater;
	class GrMouseOverer*	m_pMouseOverer;
	class GrMousePresser*	m_pMousePresser;

	ulong					m_dwFlag;
	bool					m_bShowClippedText;

	uint					m_nReservedColumn;
	uint					m_nReservedRow;

	bool					m_bMarginVisible;


	bool					m_bAutoFitColumn;
	bool					m_bAutoFitRow;
	int						m_nColumnSplitter;
	int						m_nRowSplitter;

	bool					m_bUpdating;
	bool					m_bFullRowSelect;
	bool					m_bSelectionVisible;
	bool					m_bRowHighlight;
	GrRowHighlightType		m_rowHighlightType;

	GrFont*					m_pFont;
	GrStyle*				m_pStyle;
	GrStyle*				m_pDefaultStyle;

	int						m_nAttachedCount;
	int						m_nCreatedCell;	friend class GrCell;
};