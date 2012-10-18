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
#include "GrGridCell.h"
#include "GrGridPainter.h"
#include "GrGridCoreComponents.h"
#include "GrGridStateManager.h"
#include "GrGridWindow.h"
#include "GrDataRowList.h"
#include "GrColumnList.h"
#include <map>

class GrGridCore : public GrObject
{
    typedef GrEvent<GrEventArgs, GrGridCore> _GrEvent;
    typedef GrEvent<GrColumnEventArgs, GrGridCore> _GrColumnEvent;
    typedef GrEvent<GrEditEventArgs, GrGridCore> _GrBeginEditEvent;
    typedef GrEvent<GrItemMouseEventArgs, GrGridCore> _GrItemMouseEvent;
    typedef GrEvent<GrRowMouseEventArgs, GrGridCore> _GrRowMouseEvent;

public:
    GrGridCore(GrGridWindow* pGridWindow);
    virtual ~GrGridCore(void);

    void Reserve(uint columnCount, uint rowCount);
    uint GetReservedColumn() const;
    uint GetReservedRow() const;

    void Clear();
    bool Update(bool force = false);

    GrRootRow* GetRootRow() const { return m_pRootRow; }
    GrDataRowList* GetDataRowList() const { return m_pDataRowList; }
    GrColumnList* GetColumnList() const { return m_pColumnList; }
    GrInsertionRow* GetInsertionRow() const { return m_pDataRowList->GetInsertionRow(); }
    GrCaption* GetCaptionRow() const { return m_pCaption; }
    GrGroupPanel* GetGroupPanel() const { return m_pGroupPanel; }
    GrItemSelector* GetItemSelector() const { return m_pItemSelector; }
    GrFocuser* GetFocuser() const { return m_pFocuser; }
    GrFocusMover* GetFocusMover() const { return m_pFocusMover; }
    GrTextUpdater* GetTextUpdater() const { return m_pTextUpdater; }
    GrScroll* GetHorzScroll() const { return m_pGridWindow->GetHorzScroll(); }
    GrScroll* GetVertScroll() const { return m_pGridWindow->GetVertScroll(); }
    GrGridWindow* GetGridWindow() const { return m_pGridWindow; }
    GrStateManager* GetStateManager() const { return m_pStateManager; }

    const GrRect& GetDisplayRect() const;
    void SetDisplayRect(const GrRect& displayRect);

    GrRect GetBounds() const;
    GrRect GetVisibleBounds() const;
    GrRect GetDataRect() const;

    bool GetAutoFitColumn() const;
    void SetAutoFitColumn(bool b);
    GrAutoFitColumnType GetAutoFitColumnType() const;
    void GetAutoFitColumnType(GrAutoFitColumnType value);
    bool GetAutoFitRow() const;
    void SetAutoFitRow(bool b);
    int GetColumnSplitter() const;
    void SetColumnSplitter(int value);
    int GetRowSplitter() const;
    void SetRowSplitter(int value);
    bool GetInsertionRowVisible() const;
    void SetInsertionRowVisible(bool b);
    bool GetRowHighlight() const;
    void SetRowHighlight(bool b);
    GrRowHighlightType GetRowHighlightType() const;
    void SetRowHighlightType(GrRowHighlightType type);
    bool GetFullRowSelect() const;
    void SetFullRowSelect(bool b);
    bool GetSelectionVisible() const;
    void SetSelectionVisible(bool b);
    bool GetRowVisible() const;
    void SetRowVisible(bool b);

    bool IsGrouped() const;
    bool IsUpdating() const { return m_updating; }

    bool GetGroupable() const;
    void SetGroupable(bool b) const;
    bool GetColumnMovable() const;
    void SetColumnMovable(bool b);
    bool GetColumnSortable() const;
    void SetColumnSortable(bool b);
    bool GetColumnResizable() const;
    void SetColumnResizable(bool b);
    bool GetColumnFreezable() const;
    void SetColumnFreezable(bool b);
    bool GetReadOnly() const;
    void SetReadOnly(bool value);
    bool GetRowResizable() const;
    void SetRowResizable(bool b);
    bool GetHideSelection() const;
    void SetHideSelection(bool b);
    bool GetMultiSelect() const;
    void SetMultiSelect(bool b);
    GrClickEditing GetClickEditing() const;
    void SetClickEditing(GrClickEditing clickEditing);

	GrColor GetForeColor() const;
	GrColor GetBackColor() const;
	GrColor GetLineColor() const;
	GrFont* GetFont() const;
    GrPadding GetPadding() const;

	void SetForeColor(GrColor foreColor);
	void SetBackColor(GrColor backColor);
	void SetLineColor(GrColor lineColor);
	void SetFont(GrFont* pFont);
    void SetPadding(GrPadding padding);

    //int GetGroupMargin() const;
    void EditItem(GrItem* pItem, GrEditingReason reason);
    void BringIntoView(GrItem* pItem);

    bool SetMouseOver(GrCell* pCell, const GrPoint& localLocation);
    GrCell* GetMouseOver() const;
    int GetMouseOverState() const;
    void SetMousePress(GrCell* pCell);
    void SetMouseUnpress();
    GrCell* GetMousePress() const;

    void Invalidate();
    void Invalidate(int x, int y, int width, int height);
    void Invalidate(const GrRect& rect);
    void LockInvalidate();
    void UnlockInvalidate();
    void ResetInvalidate();
    bool IsInvalidated() const;

    bool HitTest(const GrPoint& location, GrHitTest* pHitTest) const;

    GrStyle* GetStyle() const;
    void SetStyle(GrStyle* pStyle);

    void AttachObject(GrObject* pObject);
    void DetachObject(GrObject* pObject);

    void Invoke(std::wstring eventName, GrEventArgs* e);

    _GrEvent Created;
    _GrEvent Cleared;
    _GrEvent CapacityChanged;
    _GrEvent FontChanged;

    _GrItemMouseEvent ItemMouseEnter;
    _GrItemMouseEvent ItemMouseMove;
    _GrItemMouseEvent ItemMouseLeave;
    _GrItemMouseEvent ItemMouseClick;
    _GrItemMouseEvent ItemMouseDoubleClick;

    _GrRowMouseEvent RowMouseEnter;
    _GrRowMouseEvent RowMouseMove;
    _GrRowMouseEvent RowMouseLeave;

protected:
    virtual void OnCleared(GrEventArgs* e);
    virtual void OnCapacityChanged(GrEventArgs* e);

    virtual void OnEditValue(GrEditEventArgs* e);
    virtual void OnItemMouseEnter(GrItemMouseEventArgs* e);
    virtual void OnItemMouseMove(GrItemMouseEventArgs* e);
    virtual void OnItemMouseLeave(GrItemMouseEventArgs* e);
    virtual void OnItemMouseClick(GrItemMouseEventArgs* e);
    virtual void OnItemMouseDoubleClick(GrItemMouseEventArgs* e);

    virtual void OnRowMouseEnter(GrRowMouseEventArgs* e);
    virtual void OnRowMouseMove(GrRowMouseEventArgs* e);
    virtual void OnRowMouseLeave(GrRowMouseEventArgs* e);

    virtual void OnFontChanged(GrEventArgs* e);

    virtual void PrePaint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    virtual void PostPaint(GrGridPainter* pPainter, const GrRect& clipRect) const;

private:
    void focuser_FocusChanging(GrObject* pSender, GrFocusChangeArgs* e);

    void columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnWidthChanged(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnWordwrapChanged(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnHorzAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnVertAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnPaddingChanged(GrObject* pSender, GrColumnEventArgs* e);

    void BeginPaint();
    void EndPaint();

private:
    GrRootRow* m_pRootRow;
    GrCaption* m_pCaption;
    GrGroupPanel* m_pGroupPanel;
    GrColumnList* m_pColumnList;
    GrRowSplitter* m_pSplitterRow;
    GrDataRowList* m_pDataRowList;
    GrFocusMover* m_pFocusMover;

    GrStateManager* m_pStateManager;
    GrGridWindow* m_pGridWindow;
    GrInvalidator* m_pInvalidator;

    GrRect m_displayRect;

    GrItemSelector* m_pItemSelector;
    GrFocuser* m_pFocuser;
    GrTextUpdater* m_pTextUpdater;
    class GrMouseOverer* m_pMouseOverer;
    class GrMousePresser* m_pMousePresser;

    //ulong m_flag;

    uint m_reservedColumn;
    uint m_reservedRow;

    bool m_autoFitColumn;
    GrAutoFitColumnType m_autoFitColumnType;
    bool m_autoFitRow;
    int m_columnSplitterWidth;
    int m_rowSplitterHeight;

    bool m_updating;
    bool m_fullRowSelect;
    bool m_selectionVisible;
    bool m_rowHighlight;
    GrRowHighlightType m_rowHighlightType;
    bool m_columnMovable;
    bool m_columnSortable;
    bool m_columnResizable;
    bool m_columnFreezable;
    bool m_readOnly;
    bool m_rowResizable;
    bool m_rowVisible;
    bool m_hideSelection;
    bool m_multiSelect;
    GrClickEditing m_clickEditing;

	GrColor m_foreColor;
	GrColor m_backColor;
	GrColor m_lineColor;
    GrFont* m_pFont;
    GrPadding m_padding;

    GrStyle* m_pStyle;

    int m_attachedCount;
    int m_createdCell; friend class GrCell;

public:
    static const GrClickEditing DefaultClickEditing;

    friend class GrGridWindow;
};