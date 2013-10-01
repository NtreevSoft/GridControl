#pragma once
#include "GrGridCell.h"

class GrColumnList : public GrUpdatableRow
{
    typedef std::vector<GrColumn*> _Columns;
    typedef GrEvent<GrColumnEventArgs, GrColumnList> _GrColumnEvent;
    typedef GrEvent<GrColumnMouseEventArgs, GrColumnList> _GrColumnMouseEvent;

public:
    GrColumnList();
    virtual ~GrColumnList();
    
    GrColumnList& operator=(const GrColumnList&) {}

    void Reserve(uint reserve);
    void AddColumn(GrColumn* pColumn);
    void RemoveColumn(GrColumn* pColumn);
    void InsertColumn(GrColumn* pColumn, uint index);

    uint GetColumnCount() const;
    GrColumn* GetColumn(uint index) const;

    void AdjustColumnWidth();

    uint GetVisibleColumnCount() const;
    GrColumn* GetVisibleColumn(uint index) const;
    int GetVisibleRight() const;

    uint GetFrozenColumnCount() const;
    GrColumn* GetFrozenColumn(uint index) const;

    uint GetUnfrozenColumnCount() const;
    GrColumn* GetUnfrozenColumn(uint index) const;
    int GetUnfrozenX() const { return m_unfrozenX; }

    GrColumnSplitter* GetColumnSplitter() const { return m_pColumnSplitter; }

    uint GetDisplayableColumnCount() const;
    GrColumn* GetDisplayableColumn(uint index) const;
	int GetDisplayableRight() const { return m_displayableRight; }

    GrColumn* GetFirstSortColumn() const;

    //bool MoveToFrozen(GrColumn* pColumn, GrColumn* pWhere);
    //bool MoveToUnfrozen(GrColumn* pColumn, GrColumn* pWhere);

    uint ClipFrom(uint visibleFrom) const;
    uint ClipFrom(const GrRect& displayRect, uint visibleFrom) const;
    uint ClipTo(uint visibleTo) const;
    uint ClipTo(const GrRect& displayRect, uint visibleTo) const;

    GrColumn* HitTest(int x) const;
    GrIndexRange HitTest(int x, GrColumn* pColumnAnchor) const;
    void BringIntoView(GrColumn* pColumn);
	void DisplayFirst(GrColumn* pColumn);

    void SetFitChanged();
    void SetVisibleChanged();
    void SetWidthChanged();

    virtual bool ShouldClip(const GrRect& displayRect, uint horizontal, uint vertical) const;
    virtual void Clip(const GrRect& displayRect, uint horizontal, uint vertical);
    virtual int GetClipPriority() const { return 0; }
    virtual bool ShouldUpdate() const;
    virtual void Update(bool force = false);
    virtual int GetUpdatePriority() const { return UPDATEPRIORITY_COLUMNLIST; }

    virtual GrCell* HitTest(const GrPoint& location) const;

    virtual bool GetDisplayable() const { return true; }
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    void PaintSplitter(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrCellType GetCellType() const { return GrCellType_Root; }
    virtual int GetWidth() const;

    virtual bool GetVisible() const { return true; }
    virtual bool GetReadOnly() const { return true; }
    virtual GrItemType GetItemType() const { return GrItemType_Control; }
    virtual GrRowType GetRowType() const { return GrRowType_ColumnList; }

    virtual int GetMinHeight() const { return GetHeight(); }
    virtual GrRect GetBounds() const;

	virtual GrColor GetForeColor() const;
    virtual GrColor GetBackColor() const;
	virtual GrColor GetLineColor() const;
    virtual GrFont* GetFont() const;

    virtual void Invoke(std::wstring eventName, GrEventArgs* e);

    _GrColumnEvent ColumnInserted;
    _GrColumnEvent ColumnRemoved;
    _GrColumnEvent ColumnSortTypeChanged;
    _GrColumnEvent ColumnFrozenChanged;
    _GrColumnEvent ColumnWidthChanged;
    _GrColumnEvent ColumnWordwrapChanged;
    _GrColumnEvent ColumnMultilineChanged;
    _GrColumnEvent ColumnHorzAlignChanged;
    _GrColumnEvent ColumnVertAlignChanged;
    _GrColumnEvent ColumnPaddingChanged;
    _GrColumnEvent ColumnGroupChanged;
	_GrColumnEvent ColumnVisibleIndexChanged;

    _GrColumnMouseEvent ColumnMouseMove;
    _GrColumnMouseEvent ColumnMouseEnter;
    _GrColumnMouseEvent ColumnMouseLeave;
    _GrColumnMouseEvent ColumnMouseDown;
    _GrColumnMouseEvent ColumnMouseUp;


    const std::vector<GrColumn*>& Columns;
    const std::vector<GrColumn*>& Visibles;
    const std::vector<GrColumn*>& Displayables;

protected:
    virtual void OnGridCoreAttached();

    virtual void OnColumnInserted(GrColumnEventArgs* e);
    virtual void OnColumnRemoved(GrColumnEventArgs* e);

    virtual void OnColumnSortTypeChanged(GrColumnEventArgs* e);
    virtual void OnColumnFrozenChanged(GrColumnEventArgs* e);
    virtual void OnColumnWidthChanged(GrColumnEventArgs* e);
    virtual void OnColumnWordwrapChanged(GrColumnEventArgs* e);
    virtual void OnColumnMultilineChanged(GrColumnEventArgs* e);
    virtual void OnColumnHorzAlignChanged(GrColumnEventArgs* e);
    virtual void OnColumnVertAlignChanged(GrColumnEventArgs* e);
    virtual void OnColumnPaddingChanged(GrColumnEventArgs* e);
    virtual void OnColumnGroupChanged(GrColumnEventArgs* e);
	virtual void OnColumnVisibleIndexChanged(GrColumnEventArgs* e);

    virtual void OnColumnMouseMove(GrColumnMouseEventArgs* e);
    virtual void OnColumnMouseEnter(GrColumnMouseEventArgs* e);
    virtual void OnColumnMouseLeave(GrColumnMouseEventArgs* e);
    virtual void OnColumnMouseDown(GrColumnMouseEventArgs* e);
    virtual void OnColumnMouseUp(GrColumnMouseEventArgs* e);

    void BuildVisibleColumnList();
    void RepositionColumnList();

private:
    void groupPanel_Changed(GrObject* pSender, GrEventArgs* e);
    void gridCore_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e);
    void dataRowList_DataRowRemoved(GrObject* pSender, GrDataRowEventArgs* e);
    void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);
    void gridCore_DisplayRectChanged(GrObject* pSender, GrEventArgs* e);
    void column_GroupChanged(GrObject* pSender, GrEventArgs* e);

private:
    bool IsColumnSelecting(GrColumn* pColumn) const;
    void UpdateHorzScroll(const GrRect& displayRect);

private:
    _Columns m_vecColumns;
    _Columns m_vecColumnsRemoved;
    _Columns m_vecVisibleColumns;
    _Columns m_vecDisplayableColumns;
    int m_columnID;

    int m_unfrozenX;
    int m_frozenRight;
    int m_displayableRight;
    int m_visibleRight;
    int m_boundWidth;

    uint m_frozenCount;
    uint m_groupCount;

    GrIndexRange m_selectingRange;
    GrColumn* m_pSortColumn;
    GrColumnSplitter* m_pColumnSplitter;

    int m_clippedWidth;
    uint m_clippedIndex;

    // flag
    bool m_visibleChanged;
    bool m_fitChanged;
    bool m_widthChanged;

    class SortColumnOnFreezable
    {
    public:
        bool operator () (const GrColumn* pColumn1, const GrColumn* pColumn2);
    };

    class SortColumnOnUnfreezable
    {
    public:
        bool operator () (const GrColumn* pColumn1, const GrColumn* pColumn2);
    };
};