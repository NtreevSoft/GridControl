﻿//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include <map>

class GrGridCore : public GrObject
{
    typedef GrEvent<GrEventArgs, GrGridCore>        _GrEvent;
    typedef GrEvent<GrColumnEventArgs, GrGridCore>  _GrColumnEvent;
    typedef GrEvent<GrEditEventArgs, GrGridCore> _GrBeginEditEvent;
    typedef GrEvent<GrItemMouseEventArgs, GrGridCore> _GrItemMouseEvent;

public:
    GrGridCore(GrGridWindow* pGridWindow);
    virtual ~GrGridCore(void);

    void                    Reserve(uint columnCount, uint rowCount);
    uint                    GetReservedColumn() const;
    uint                    GetReservedRow() const;

    void                    Clear();
    bool                    Update(bool force = false);

    void                    Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;    

    GrRootRow*              GetRootRow() const { return m_pRootRow; }
    GrDataRowList*          GetDataRowList() const { return m_pDataRowList; }
    GrColumnList*           GetColumnList() const { return m_pColumnList; }
    GrInsertionRow*         GetInsertionRow() const { return m_pInsertionRow; }
    GrCaption*              GetCaptionRow() const { return m_pCaption; }
    GrGroupingList*         GetGroupingList() const { return m_pGroupingList; }
    GrItemSelector*         GetItemSelector() const { return m_pItemSelector; }
    GrFocuser*              GetFocuser() const { return m_pFocuser; }
    GrFocusMover*           GetFocusMover() const { return m_pFocusMover; }
    GrTextUpdater*          GetTextUpdater() const { return m_pTextUpdater; }
    GrScroll*               GetHorzScroll() const { return m_pGridWindow->GetHorzScroll(); }
    GrScroll*               GetVertScroll() const { return m_pGridWindow->GetVertScroll(); }
    GrGridWindow*           GetGridWindow() const { return m_pGridWindow; }
    GrStateManager*         GetStateManager() const { return m_pStateManager; }

    const GrRect&           GetDisplayRect() const;
    void                    SetDisplayRect(const GrRect& displayRect);

    GrRect                  GetBounds() const;
    GrRect                  GetDataRect() const;

    bool                    GetAutoFitColumn() const;
    void                    SetAutoFitColumn(bool b);
    bool                    GetAutoFitRow() const;
    void                    SetAutoFitRow(bool b);
    int                     GetColumnSplitter() const;
    void                    SetColumnSplitter(int value);
    int                     GetRowSplitter() const;
    void                    SetRowSplitter(int value);
    bool                    GetInsertionRowVisible() const;
    void                    SetInsertionRowVisible(bool b);
    bool                    GetRowHighlight() const;
    void                    SetRowHighlight(bool b);
    GrRowHighlightType      GetRowHighlightType() const;
    void                    SetRowHighlightType(GrRowHighlightType type);
    bool                    GetFullRowSelect() const;
    void                    SetFullRowSelect(bool b);
    bool                    GetSelectionVisible() const;
    void                    SetSelectionVisible(bool b);
    bool                    GetRowVisible() const;
    void                    SetRowVisible(bool b);

    bool                    IsGrouped() const;
    bool                    IsUpdating() const { return m_updating; }

    bool                    CanBeGrouping() const;
    void                    EnableGrouping(bool b) const;
    bool                    CanBeColumnMoving() const;
    void                    EnableColumnMoving(bool b);
    bool                    CanBeColumnSorting() const;
    void                    EnableColumnSorting(bool b);
    bool                    CanBeColumnResizing() const;
    void                    EnableColumnResizing(bool b);
    bool                    CanBeColumnFrozing() const;
    void                    EnableColumnFrozing(bool b);
    bool                    GetReadOnly() const;
    void                    SetReadOnly(bool value);
    bool                    CanBeRowResizing() const;
    void                    EnableRowResizing(bool b);
    bool                    GetHideSelection() const;
    void                    SetHideSelection(bool b);
    bool                    GetMultiSelect() const;
    void                    SetMultiSelect(bool b);
    GrClickEditing          GetClickEditing() const;
    void                    SetClickEditing(GrClickEditing clickEditing);


    int                     GetGroupingMargin() const;

    void                    EditItem(GrItem* pItem, GrEditingReason reason);

    void                    BringIntoView(GrItem* pItem);

    bool                    SetMouseOver(GrCell* pCell, const GrPoint& localLocation);
    GrCell*                 GetMouseOver() const;
    int                     GetMouseOverState() const;
    void                    SetMousePress(GrCell* pCell);
    void                    SetMouseUnpress();
    GrCell*                 GetMousePress() const;

    void                    Invalidate();
    void                    Invalidate(int x, int y, int width, int height);
    void                    Invalidate(const GrRect& rect);
    void                    LockInvalidate();
    void                    UnlockInvalidate();
    void                    ResetInvalidate();
    bool                    IsInvalidated() const;

    bool                    HitTest(const GrPoint& location, GrHitTest* pHitTest) const;

    GrStyle*                GetStyle() const;
    void                    SetStyle(GrStyle* pStyle);

    void                    AttachObject(GrObject* pObject);
    void                    DetachObject(GrObject* pObject);

    void                    Invoke(std::wstring eventName, GrEventArgs* e);

    _GrEvent                Created;
    _GrEvent                Cleared;
    _GrEvent                GroupingChanged;
    _GrEvent                CapacityChanged;
    _GrEvent                FontChanged;

    _GrItemMouseEvent       ItemMouseEnter;
    _GrItemMouseEvent       ItemMouseMove;
    _GrItemMouseEvent       ItemMouseLeave;
    _GrItemMouseEvent       ItemMouseClick;
    _GrItemMouseEvent       ItemMouseDoubleClick;


protected:
    virtual void            OnCleared(GrEventArgs* e);
    virtual void            OnCapacityChanged(GrEventArgs* e);

    virtual void            OnEditValue(GrEditEventArgs* e);
    virtual void            OnItemMouseEnter(GrItemMouseEventArgs* e);
    virtual void            OnItemMouseMove(GrItemMouseEventArgs* e);
    virtual void            OnItemMouseLeave(GrItemMouseEventArgs* e);
    virtual void            OnItemMouseClick(GrItemMouseEventArgs* e);
    virtual void            OnItemMouseDoubleClick(GrItemMouseEventArgs* e);

    virtual void            OnFontChanged(GrEventArgs* e);

private:
    void                    focuser_FocusChanging(GrObject* pSender, GrFocusChangeArgs* e);

    void                    columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
    void                    columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
    void                    columnList_ColumnWidthChanged(GrObject* pSender, GrColumnEventArgs* e);
    void                    columnList_ColumnWordwrapChanged(GrObject* pSender, GrColumnEventArgs* e);
    void                    columnList_ColumnHorzAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
    void                    columnList_ColumnVertAlignChanged(GrObject* pSender, GrColumnEventArgs* e);
    void                    columnList_ColumnPaddingChanged(GrObject* pSender, GrColumnEventArgs* e);

    void                    groupingList_Changed(GrObject* pSender, GrEventArgs* e);

private:
    GrRootRow*              m_pRootRow;
    GrCaption*              m_pCaption;
    GrGroupingList*         m_pGroupingList;
    GrColumnList*           m_pColumnList;
    GrInsertionRow*         m_pInsertionRow;
    GrRowSplitter*          m_pSplitterRow;
    GrDataRowList*          m_pDataRowList;
    GrFocusMover*           m_pFocusMover;

    GrStateManager*         m_pStateManager;
    GrGridWindow*           m_pGridWindow;
    GrInvalidator*          m_pInvalidator;

    GrRect                  m_displayRect;

    int                     m_groupingMargin;

    GrItemSelector*         m_pItemSelector;
    GrFocuser*              m_pFocuser;
    GrTextUpdater*          m_pTextUpdater;
    class GrMouseOverer*    m_pMouseOverer;
    class GrMousePresser*   m_pMousePresser;

    //ulong                   m_flag;

    uint                    m_reservedColumn;
    uint                    m_reservedRow;

    bool                    m_autoFitColumn;
    bool                    m_autoFitRow;
    int                     m_columnSplitterWidth;
    int                     m_rowSplitterHeight;

    bool                    m_updating;
    bool                    m_fullRowSelect;
    bool                    m_selectionVisible;
    bool                    m_rowHighlight;
    GrRowHighlightType      m_rowHighlightType;
    bool                    m_columnMoving;
    bool                    m_columnSorting;
    bool                    m_columnResizing;
    bool                    m_columnFrozing;
    bool                    m_readOnly;
    bool                    m_rowResizing;
    bool                    m_hideSelection;
    bool                    m_multiSelect;
    bool                    m_rowVisible;
    GrClickEditing          m_clickEditing;

    GrFont*                 m_pFont;
    GrStyle*                m_pStyle;
    GrStyle*                m_pDefaultStyle;

    int                     m_attachedCount;
    int                     m_createdCell;  friend class GrCell;

public:
    static const GrClickEditing DefaultClickEditing;
};