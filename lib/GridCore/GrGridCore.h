#pragma once
#include "GrGridBase.h"
#include "GrGridCell.h"
#include "GrGridRenderer.h"
#include "GrGridCoreComponents.h"

class GrItemSelector;
class GrFocuser;
class GrHitTester;

struct GrHitTestRange
{
	uint		nStartVisibleRow;
	uint		nEndVisibleRow;
	uint		nStartVisibleCol;
	uint		nEndVisibleCol;
};

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

	bool					ShouldUpdate() const;
	bool					Update();

	GrRow*					GetRoot() const { return (GrRow*)m_pRootRow; }

	GrDataRowList*			GetDataRowList() const { return m_pDataRowList; }
	GrColumnList*			GetColumnList() const { return m_pColumnList; }
	GrInsertionRow*			GetInsertionRow() const { return m_pInsertionRow; }
	GrCaption*				GetCaptionRow() const { return m_pCaption; }
	GrGroupingList*			GetGroupingList() const { return m_pGroupingList; }
	GrHeaderRow*			GetHeaderList() const { return m_pHeaderList; }
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

	void					SetWidthChanged();
	void					SetHeightChanged();

	bool					IsGrouped() const;
	bool					IsUpdating() const { return m_bUpdating; }

	bool					CanBeGrouped() const;
	void					EnableGrouping(bool b) const;

	int						GetGroupingMargin() const;
	void					ShowClippedText(bool bShow = true);

	void 					Invalidate();
	void 					Invalidate(int x, int y, int width, int height);
	void 					Invalidate(const GrRect& rect);

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

	void					columnList_ColumnWidthChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnWordwrapChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnHorzAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnVertAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
	void					columnList_ColumnPaddingChanged(GrObject* pSender, GrColumnEventArgs* e);

	void					groupingList_Changed(GrObject* pSender, GrEventArgs* e);

private:
	GrRow*					m_pRootRow;
	GrHeaderRow*			m_pHeaderList;
	GrCaption*				m_pCaption;
	GrGroupingList*			m_pGroupingList;
	GrColumnList*			m_pColumnList;
	GrInsertionRow*			m_pInsertionRow;
	GrRowSplitter*			m_pSplitterRow;
	GrDataRowList*			m_pDataRowList;

	GrGridInvalidator*		m_pInvalidator;
	GrGridInvalidator*		m_pDefaultInvalidator;

	GrRect					m_rtDisplay;
	uint					m_nVScrollValue;
	uint					m_nHScrollValue;

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

	bool					m_bNeedToColumnClipping;
	bool					m_bNeedToRowClipping;
	bool					m_bMarginVisible;
	
	bool					m_bWidthChanged;
	bool					m_bHeightChanged;

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