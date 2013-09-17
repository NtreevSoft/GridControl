#pragma once
#include "GrGridCell.h"

class GrDataRowList : public GrUpdatableRow
{
    struct GrCache
    {
        int height;
        bool expanded;
    };

    typedef std::vector<GrColumn*> _Columns;
    typedef std::vector<GrDataRow*> _DataRows;
    typedef std::vector<IDataRow*> _IDataRows;
    typedef std::vector<GrGroupRow*> _GroupRows;
    typedef std::map<std::wstring, GrCache> _MapCaches;

    typedef GrEvent<GrEventArgs, GrDataRowList> _GrEvent;
    typedef GrEvent<GrDataRowEventArgs, GrDataRowList> _GrDataRowEvent;
    typedef GrEvent<GrDataRowInsertingEventArgs, GrDataRowList> _GrDataRowInsertingEvent;
    typedef GrEvent<GrDataRowInsertedEventArgs, GrDataRowList> _GrDataRowInsertedEvent;

public:
    GrDataRowList();
    virtual ~GrDataRowList();

    void Reserve(uint reserve);

    void AdjustRowHeight();
    int GetRowWidth() const;
    void SetRowWidth(int width);

    uint GetVisibleRowCount() const;
    IDataRow* GetVisibleRow(uint index) const;

    uint GetVisibleDataRowCount() const;
    GrDataRow* GetVisibleDataRow(uint index) const;

    GrDataRow* NewDataRowFromInsertion();
    GrDataRow* NewDataRow();

    GrInsertionRow* GetInsertionRow() const;

    void AddDataRow(GrDataRow* pDataRow);
    void RemoveDataRow(GrDataRow* pDataRow);
    void InsertDataRow(GrDataRow* pDataRow, uint index);
	void MoveDataRow(GrDataRow* pDataRow, uint index);
    uint GetDataRowCount() const;
    GrDataRow* GetDataRow(uint index) const;
    void Clear();

    uint GetDisplayableRowCount() const;
    IDataRow* GetDisplayableRow(uint index) const;
    int GetDisplayableBottom() const;
    int GetVisibleBottom() const;

    uint ClipFrom(uint visibleFrom) const;
    uint ClipFrom(const GrRect& displayRect, uint visibleFrom) const;
    uint ClipTo(uint visibleTo) const;
    uint ClipTo(const GrRect& displayRect, uint visibleTo) const;
    IDataRow* HitTest(int y) const;
    GrIndexRange HitTest(int y, IDataRow* pRowAnchor) const;
    void BringIntoView(IDataRow* pDataRow);
	void DisplayFirst(IDataRow* pDataRow);
	int GetDisplayOffset() const;

    void Sort(GrColumn* pColumn);

    void SetZeroBasedRowIndex(bool b);
    bool GetZeroBasedRowIndex() const;

    bool GetRowNumberVisible() const;
    void SetRowNumberVisible(bool b);

    int CellStart() const;
    uint GetMargin() const;

    void SetFitChanged();
    void SetVisibleChanged();
    void SetHeightChanged();
    void SetListChanged();

    _GrEvent VisibleChanged;

    _GrDataRowInsertingEvent DataRowInserting;
    _GrDataRowInsertedEvent DataRowInserted;
    _GrDataRowEvent DataRowRemoved;
	_GrDataRowEvent DataRowMoved;
    _GrEvent VisibleHeightChanged;

    virtual GrRect GetBounds() const;
    virtual bool ShouldClip(const GrRect& displayRect, uint horizontal, uint vertical) const;
    virtual void Clip(const GrRect& displayRect, uint horizontal, uint vertical);
    virtual int GetClipPriority() const { return 1; }
    virtual bool ShouldUpdate() const;
    virtual void Update(bool force = false);
    virtual int GetUpdatePriority() const { return UPDATEPRIORITY_DATAROWLIST; }

    virtual GrRowType GetRowType() const { return GrRowType_DataRowList; }
    virtual GrCell* HitTest(const GrPoint& location) const;
    virtual int GetWidth() const { return 0; }
    virtual int GetHeight() const { return m_displayableHeight; }
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
	

protected:
    virtual void OnGridCoreAttached();
    virtual void OnYChanged();

    virtual void OnDataRowInserting(GrDataRowInsertingEventArgs* e);
    virtual void OnDataRowInserted(GrDataRowInsertedEventArgs* e);
    virtual void OnDataRowRemoved(GrDataRowEventArgs* e);
	virtual void OnDataRowMoved(GrDataRowEventArgs* e);
    virtual void OnVisibleHeightChanged(GrEventArgs* e);

private:
    GrGroupRow* CreateGroupRow(GrRow* pParent, GrColumn* pColumn, const std::wstring& itemText);
    GrDataRow* CreateInsertionRow();
    void BuildGroup(GrRow* pParent, uint groupLevel);
    void BuildChildRowList();
    void BuildVisibleRowList();
    void RepositionVisibleRowList();
    void BuildCache();
    void DeleteObjects();
    void UpdateVertScroll(const GrRect& displayRect);
    void GetVisibleList(GrRow* pRow, GrRowArray* pVisible) const;
    void GetMaxDepth(GrRow* pRow, uint* depth) const;
    
    void groupPanel_Changed(GrObject* pSender, GrEventArgs* e);
    void groupPanel_Expanded(GrObject* pSender, GrGroupEventArgs* e);
    void groupPanel_SortChanged(GrObject* pSender, GrGroupEventArgs* e);
    void gridCore_Created(GrObject* pSender, GrEventArgs* e);
    void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);
    void columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnSortTypeChanged(GrObject* pSender, GrColumnEventArgs* e);
	void columnList_ColumnPaddingChanged(GrObject* pSender, GrColumnEventArgs* e);
    void focuser_FocusedChanged(GrObject* pSender, GrFocusChangeArgs* e);

private:
    int m_rowWidth;

    _DataRows m_vecDataRowsRemoved;
    _DataRows m_vecDataRows;
    _DataRows m_vecVisibleDataRows;
    _IDataRows m_vecVisibleRows;
    _IDataRows m_vecDisplayableRows;
    _GroupRows m_vecGroupRows;

    _MapCaches m_mapCache;
    _Columns m_vecColumns;

    uint m_usedGroupRow;
    uint m_groupCount;
    uint m_dataRowID;
    uint m_margin;
	int m_offset;

    int m_displayableBottom;
    int m_visibleBottom;
    int m_visibleHeight;
    int m_displayableHeight;

    GrIndexRange m_selectingRange;
    GrDataRow* m_pFocusedDataRow;
    GrRect m_bound;
    GrDataRow* m_pInsertionRow;

    GrDataRow* m_pItems;
    int m_itemsCount;

    bool m_visibleRowNumber;
    bool m_zeroBasedRowIndex;
    bool m_updating;

    int m_clippedHeight;
    uint m_clippedIndex;

    // flags
    bool m_listChanged;
    bool m_visibleChanged;
    bool m_fitChanged;
    bool m_heightChanged;
};