//=====================================================================================================================
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
#include "GrGridType.h"
#include "GrGridEvent.h"
#include <time.h>

#ifdef _MANAGED
#include <vcclr.h>
#endif

#define INVALID_INDEX   ((uint)-1)
#define INSERTION_ROW   ((uint)-8)

#define UPDATEPRIORITY_DATAROWLIST  1
#define UPDATEPRIORITY_COLUMNLIST   2
#define UPDATEPRIORITY_GROUPINLIST  3


class GrGridCore;
class GrRow;
class GrDataRow;
class GrColumn;
class GrGroupingInfo;
class GrGridPainter;

class GrColumnList;
class GrDataRowList;
class GrGroupingRow;
class GrColumnSplitter;
class GrFont;
class GrItem;

class IDataRow;
class IFocusable;

typedef bool (*FuncSortRow)(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData);

enum GrHorzAlign
{
    GrHorzAlign_Left,
    GrHorzAlign_Center,
    GrHorzAlign_Right,

    GrHorzAlign_Count,
};

enum GrVertAlign
{
    GrVertAlign_Top,
    GrVertAlign_Center,
    GrVertAlign_Bottom,

    GrVertAlign_Count,
};

enum GrMouseOverState
{
    GrMouseOverState_None,
    GrMouseOverState_In,
    GrMouseOverState_Control,

    GrMouseOverState_Count,
};

enum GrItemTypeShow
{
    GrItemTypeShow_FocusedOnly,
    GrItemTypeShow_SelectedOnly,
    GrItemTypeShow_Always,

    GrItemTypeShow_Count,
};

enum GrItemType
{
    GrItemType_Control,
    GrItemType_DropDown,
    GrItemType_Modal,

    GrItemType_Count,
};

enum GrClickEditing
{
    GrClickEditing_Default,
    GrClickEditing_Click,
    GrClickEditing_DoubleClick,
    GrClickEditing_FocusedClick,
};

class GrSortFunc
{
public:
    static bool SortRowsUp(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData);
    static bool SortRowsDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData);
    static bool SortRowsNone(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData);
    static bool SortDataRowSortUp(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData);
    static bool SortDataRowSortDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData);
    static bool SortDataRowSortNone(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData);
};

class GrColumnEventArgs : public GrEventArgs
{
public:
    GrColumnEventArgs(GrColumn* pColumn) 
        : m_pColumn(pColumn) 
    {

    }

    GrColumn* GetColumn() const
    {
        return m_pColumn;
    }

private:
    GrColumn* m_pColumn;
};

class GrMouseEventArgs : public GrEventArgs
{
public:
    GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys);
    GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys, int delta);

    const GrPoint&  GetLocation() const;
    int             GetX() const;
    int             GetY() const;
    bool            GetControlKey() const;
    bool            GetAltKey() const;
    bool            GetShiftKey() const;
    GrKeys          GetModifierKeys() const;
    int             GetDelta() const;

private:
    GrPoint         m_location;
    GrKeys          m_modifierKeys;
    int             m_delta;
};

class GrColumnMouseEventArgs : public GrMouseEventArgs
{
public:
    GrColumnMouseEventArgs(GrColumn* pColumn, const GrPoint& location, GrKeys modifierKeys);

    GrColumn*       GetColumn() const;
    bool            GetHandled() const;
    void            SetHandled(bool value);

private:
    GrColumn*       m_pColumn;
    bool            m_handled;
};

class GrCellMouseEventArgs : public GrMouseEventArgs
{
public:
    GrCellMouseEventArgs(GrCell* pCell, const GrPoint& location, GrKeys modifierKeys);

    GrCell*         GetCell() const;

private:
    GrCell*         m_pCell;
};

class GrItemMouseEventArgs : public GrMouseEventArgs
{
public:
    GrItemMouseEventArgs(GrItem* pItem, const GrPoint& location, GrKeys modifierKeys);

    GrItem*         GetItem() const;
    bool            GetHandled() const;
    void            SetHandled(bool value);

private:
    GrItem*         m_pItem;
    bool            m_handled;
};

class GrGroupingEventArgs : public GrEventArgs
{
public:
    GrGroupingEventArgs(GrGroupingInfo* pGroupingInfo) : m_pGroupingInfo(pGroupingInfo) {}

    GrGroupingInfo* m_pGroupingInfo;
};

class GrFocusChangeArgs : public GrEventArgs
{
public:
    GrFocusChangeArgs(IFocusable* pFocusable);

    IFocusable*     GetFocusable() const;

private:
    IFocusable*     m_pFocusable;
};


struct GrLineDesc
{
    int    textBegin;
    int    length;
    int    width;
    int    x;
    int    y;
};

typedef std::vector<GrLineDesc> _TextLines;

class GrCell : public GrObject
{
public:
    GrCell();
    virtual ~GrCell();

    virtual int         GetX() const = 0;
    virtual int         GetY() const = 0;
    virtual int         GetWidth() const = 0;
    virtual int         GetHeight() const = 0;
    virtual GrRow*      GetRow() const = 0;

    virtual GrCellType  GetCellType() const = 0;
    virtual bool        GetVisible() const = 0;
    virtual bool        GetDisplayable() const = 0;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const = 0;

    bool                Contains(const GrPoint& point) const;
    bool                ContainsHorz(int x) const;
    bool                ContainsVert(int y) const;
    bool                GetMouseOvered() const;
    bool                GetMousePressed() const;
    int                 GetMouseOverState() const;

    GrRect              GetClientRect() const;
    GrRect              GetRect() const;
    GrPoint             GetLocation() const;
    GrSize              GetSize() const;
    uint                GetID() const;
    int                 GetBottom() const { return GetY() + GetHeight(); }
    int                 GetRight() const { return GetX() + GetWidth(); }

    void                SetFont(GrFont* pFont);
    void                SetPadding(const GrPadding& padding);

    // text method
    const wchar_t*      GetText() const;
    void                SetText(const wchar_t* text);
    uint                GetTextLineCount() const;
    const GrLineDesc&   GetTextLine(uint index) const;
    const GrRect&       GetTextBounds() const;
    void                AlignText();
    void                ComputeTextBounds();
    bool                GetTextVisible() const;
    bool                GetTextClipped() const;
    void                SetTextVisible(bool b);

    virtual GrFlag      ToPaintStyle() const;
    void                DrawText(GrGridPainter* pPainter, GrColor foreColor, const GrRect& paintRect, const GrRect* pClipRect = NULL) const;

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;
    virtual bool        GetTextWordWrap() const;
    virtual bool        GetTextMulitiline() const { return false; }

    virtual GrColor     GetPaintingForeColor() const;
    virtual GrColor     GetPaintingBackColor() const;
    virtual GrFont*     GetPaintingFont() const;

    virtual GrColor     GetForeColor(bool inherited = true) const;
    virtual GrColor     GetBackColor(bool inherited = true) const;
    virtual GrPadding   GetPadding(bool inherited = true) const;
    virtual GrFont*     GetFont(bool inherited = true) const;

    void                SetBackColor(const GrColor& color);
    void                SetForeColor(const GrColor& color);
    virtual int         HitMouseOverTest(const GrPoint& localLocation) const;

    GrGridCore*         GetGridCore() const { return m_pGridCore; }

protected:
    virtual void        OnTextChanged();
    virtual void        OnTextSizeChanged();

    virtual void        OnGridCoreAttached();
    virtual void        OnGridCoreDetached();

    void                SetTextBoundsChanged();
    void                SetTextAlignChanged();

    void                Invalidate(bool thisOnly = true);

public:
    void*               Tag;

protected:
    class GrTextUpdater* m_pTextUpdater;

private:
    std::wstring        m_text;

    _TextLines          m_vecTextLine;
    GrRect              m_textBounds;

    GrColor             m_backColor;
    GrColor             m_foreColor;
    GrFont*             m_pFont;

    GrPadding           m_padding;
    uint                m_id;

    bool                m_textBoundsChanged;
    bool                m_textAlignChanged;
    bool                m_textClipped;
    bool                m_textVisible;

    static uint         m_snID;
    friend class GrGridCore;
    friend class GrTextUpdater;

#ifdef _MANAGED
public:
    gcroot<System::Object^> ManagedRef;
#endif
};


typedef std::vector<GrRow*> GrRowArray;

typedef void (*FuncCustomPaint)(GrGridPainter* pPainter, const GrItem* pItem, const GrRect& paintRect, void* userData);
typedef bool (*FuncColumnPaint)(GrGridPainter* pPainter, const GrColumn* pColumn, const GrRect& paintRect, void* userData);

typedef bool (*FuncComparer)(GrGridCore* pGridCore, const GrDataRow* pRow1, const GrDataRow* pRow2, const GrColumn* pColumn);

class GrColumn : public GrCell
{
    typedef GrEvent<GrEventArgs, GrColumn> _GrEvent;
public:
    GrColumn();

    GrColumnList*       GetColumnList() const;

    GrItemType          GetItemType() const;
    GrItemTypeShow      GetItemTypeShow() const;
    GrHorzAlign         GetItemHorzAlign() const;
    GrVertAlign         GetItemVertAlign() const;
    bool                GetItemWordWrap() const;
    bool                GetItemMultiline() const;
    const GrColor&      GetItemForeColor() const;
    const GrColor&      GetItemBackColor() const;
    const GrPadding&    GetItemPadding() const;
    GrFont*             GetItemFont() const;
    int                 GetItemMinHeight() const;
    GrClickEditing      GetItemClickEditing() const;
    bool                GetItemTextVisible() const;
    bool                GetItemIcon() const;

    uint                GetDisplayIndex() const;
    uint                GetVisibleIndex() const;
    uint                GetFrozenIndex() const;
    uint                GetUnfrozenIndex() const;
    uint                GetIndex() const;
    uint                GetColumnID() const;

    void                SetItemType(GrItemType type);
    void                SetItemHorzAlign(GrHorzAlign horzAlign);
    void                SetItemVertAlign(GrVertAlign vertAlign);
    void                SetItemWordWrap(bool b);
    void                SetItemMultiline(bool b);
    void                SetItemForeColor(const GrColor& color);
    void                SetItemBackColor(const GrColor& color);
    void                SetItemPadding(const GrPadding& padding);
    void                SetItemFont(GrFont* pFont);
    void                SetItemMinHeight(int height);
    void                SetItemClickEditing(GrClickEditing clickEditing);
    void                SetItemTextVisible(bool b);
    void                SetItemIcon(bool b);

    bool                GetMovable() const;
    void                SetMovable(bool b);
    bool                GetResizable() const;
    void                SetResizable(bool b);
    bool                GetFrozen() const;
    void                SetFrozen(bool b);

    bool                GetReadOnly() const;
    void                SetReadOnly(bool b);

    void                SetVisible(bool b);
    void                SetMinWidth(int minWidth);
    void                SetMaxWidth(int maxWidth);
    int                 GetMinWidth() const;
    int                 GetMaxWidth() const;

    void                SetSelected(bool b);
    bool                GetSelected() const;
    bool                GetFullSelected() const;
    bool                GetSelecting() const;

    bool                HasFocused() const;

    void                AdjustWidth();
    void                SetFit();

    void                EnableSort(bool b = true);
    bool                CanBeSort() const;

    void                EnableGrouping(bool b = true);
    bool                CanBeGrouping() const;

    void                SetClipped(bool b);
    bool                GetClipped() const;

    bool                ShouldBringIntoView() const;

    void                SetWidth(int width);

    void                SetTooltip(const wchar_t* tooltip);
    const wchar_t*      GetTooltip() const;

    int                 GetPriority() const;

    bool                GetGrouped() const;
    void                SetGrouped(bool b);
    GrGroupingInfo*     GetGroupingInfo() const;

    void                SetSortType(GrSort sortType);
    GrSort              GetSortType() const;
    void                SetSortComparer(GrSort sortType, FuncComparer comparer);
    FuncComparer        GetSortComparer(GrSort sortType) const;


    virtual GrCellType  GetCellType() const { return GrCellType_Column; }
    virtual int         GetX() const;
    virtual int         GetY() const;
    virtual int         GetWidth() const;
    virtual int         GetHeight() const;

    virtual bool        GetVisible() const;

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

    virtual GrRow*      GetRow() const;

    virtual GrFlag      ToPaintStyle() const;
    virtual GrColor     GetPaintingBackColor() const;
    virtual bool        GetDisplayable() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

public:
    _GrEvent            GroupingChanged;    


protected:
    virtual void        OnGridCoreAttached();
    virtual void        OnGridCoreDetached();

    virtual void        OnTextChanged();

private:
    virtual ~GrColumn();

    void                SetPriority(int priority);

    void                SetX(int x) { m_x = x; };
    void                SetDisplayable(bool b);
    void                SetDisplayIndex(uint index);
    void                SetVisibleIndex(uint index);
    void                SetIndex(uint index);
    void                SetColumnID(uint index);

public:
    bool                m_customItemPaint;

    FuncColumnPaint     m_fnColumnBackgroundPaint;
    FuncColumnPaint     m_fnColumnContentsPaint;
    void*               m_pColumnPaintData;

private:
    GrColumnList*       m_pColumnList;

    int                 m_selectedCells;    friend class GrItemSelector;
    int                 m_priority;
    int                 m_x;
    int                 m_width;

    int                 m_defaultWidth;
    int                 m_minWidth;
    int                 m_maxWidth;

    bool                m_visible;
    bool                m_readOnly;
    bool                m_canBeSorted;
    bool                m_canBeGrouped;
    bool                m_movable;
    bool                m_resizable;
    bool                m_frozen;
    bool                m_selected;
    bool                m_fullSelected;
    bool                m_fitting;
    bool                m_grouped;

    uint                m_displayIndex;
    uint                m_visibleIndex;
    uint                m_selectedIndex;
    uint                m_index;
    uint                m_columnID;

    bool                m_displayable;
    bool                m_clipped;

    GrItemType          m_itemType;
    GrItemTypeShow      m_itemTypeShow;
    GrHorzAlign         m_itemHorzAlign;
    GrVertAlign         m_itemVertAlign;
    bool                m_itemWordWrap;
    bool                m_itemMultiline;
    GrColor             m_itemBackColor;
    GrColor             m_itemForeColor;
    GrPadding           m_itemPadding;
    GrFont*             m_pItemFont;
    int                 m_itemMinHeight;
    GrClickEditing      m_itemClickEditing;
    bool                m_itemTextVisible;
    bool                m_itemIcon;

    GrGroupingInfo*     m_pGroupingInfo;

    GrSort              m_sortType;
    FuncComparer        m_comparer[GrSort_Count];

    std::wstring        m_tooltip;

    friend class GrGroupingInfo;
    friend class GrColumnList;
};

enum GrRowUpdate 
{
    GrRowUpdate_Cell    = 1,
    GrRowUpdate_Row     = 2,
    GrRowUpdate_Both    = 3,
};

class GrRow : public GrCell
{
public:
    GrRow();

    void                SetY(int y);
    virtual int         GetY() const;

    void                SetHeight(int height);
    virtual int         GetHeight() const;

    void                SetResizable(bool b);
    bool                GetResizable() const;

    void                AdjustHeight();

    virtual int         GetMinHeight() const;

    virtual GrRow*      GetRow() const { return const_cast<GrRow*>(this); }
    virtual GrCellType  GetCellType() const { return GrCellType_Row; }
    virtual int         GetX() const;
    virtual bool        GetDisplayable() const { return true; }
    virtual bool        GetVisible() const;
    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;
    virtual GrRowType   GetRowType() const { return GrRowType_Dummy; }

    virtual bool        ShouldBringIntoView() const { return false; }
    virtual GrCell*     HitTest(const GrPoint& location) const;

    uint                GetChildCount() const;
    GrRow*              GetChild(uint index) const;
    GrRow*              GetParent() const;
    uint                GetHierarchyLevel() const;

    void                SetFit();
    virtual void        Paint(GrGridPainter* /*pPainter*/, const GrRect& /*clipRect*/) const {};

    virtual void        GetVisibleList(GrRowArray* pVisible) const;
    virtual GrRect      GetBounds() const { return GetRect(); }
    virtual GrPadding   GetPadding(bool /*inherited*/) const { return GrPadding::Default; }

    virtual void        Sort(GrSort sortType);
    virtual void        Sort(FuncSortRow fnSort, void* userData);

    void                AddChild(GrRow* pRow);
    void                ReserveChild(uint reserve);
    void                ClearChild();

public:
    static int          DefaultHeight;

protected:
    virtual void        OnTextSizeChanged();
    virtual void        OnHeightChanged();
    virtual void        OnYChanged() {};
    virtual void        OnHeightAdjusted() {};

    virtual void        OnFitted();

    int                 CellStart() const;

    void                InvalidateRow();

protected:
    bool                m_visible;

private:
    GrRowArray          m_vecChilds;
    GrRow*              m_pParent;
    uint                m_hierarchyLevel;
    int                 m_y;

    bool                m_resizable;

    int                 m_height;
    bool                m_fitting;

    friend class GrGridCore;

private:
    class SortDesc
    {
    public:
        SortDesc(GrGridCore* pGridCore, FuncSortRow fn, void* userData) : 
          m_pGridCore(pGridCore), m_fn(fn), m_userData(userData) {}

          bool operator () (const GrRow* pRow1, const GrRow* pRow2)
          {
              return (*m_fn)(m_pGridCore, pRow1, pRow2, m_userData);
          }

          GrGridCore*       m_pGridCore;
          FuncSortRow       m_fn;
          void*             m_userData;
    };
};

class IDataRow : public GrRow
{
public: 
    IDataRow();

    virtual int         GetY() const;
    virtual int         GetWidth() const;

    virtual bool        GetDisplayable() const;
    void                SetDisplayable(bool b);

    void                SetVisibleIndex(uint index);
    uint                GetVisibleIndex() const;

    void                SetDisplayIndex(uint index);
    uint                GetDisplayIndex() const;

    void                SetClipped(bool b);
    bool                GetClipped() const;

    bool                ShouldBringIntoView() const;
    bool                HasFocused() const;

    virtual IFocusable* GetFocusable(GrColumn* pColumn) const = 0;
    virtual uint        GetSelectionGroup() const { return 0; }
    virtual bool        GetFullSelected() const { return false; }

protected:
    virtual void        OnFitted();
    virtual void        OnGridCoreAttached();
    virtual void        OnHeightChanged();

protected:
    GrDataRowList*      m_pDataRowList;

private:
    bool                m_displayable;
    bool                m_clipped;
    uint                m_visibleIndex;
    uint                m_displayIndex;
};

class GrDataRow : public IDataRow
{
protected:
    typedef std::vector<GrItem*> _Items;
public:
    GrDataRow();
    virtual ~GrDataRow();

    GrItem*             GetItem(const GrColumn* pColumn) const;
    GrItem*             GetItem(const GrItem* pOtherItem) const;

    bool                GetReadOnly() const;
    void                SetReadOnly(bool b = true);

    void                SetSelected(bool b);
    bool                GetSelected() const;
    bool                GetSelecting() const;

    uint                GetVisibleDataRowIndex() const;
    uint                GetDataRowIndex() const;
    uint                GetDataRowID() const;

    GrColor             GetItemBackColor() const;
    GrColor             GetItemForeColor() const;
    GrFont*             GetItemFont() const;
    void                SetItemBackColor(const GrColor& color);
    void                SetItemForeColor(const GrColor& color);
    void                SetItemFont(GrFont* pFont);

    virtual bool        GetFullSelected() const;

    virtual bool        GetVisible() const;
    virtual void        SetVisible(bool b);

    virtual GrRowType   GetRowType() const { return GrRowType_DataRow; }
    virtual GrFlag      ToPaintStyle() const;
    virtual GrColor     GetPaintingBackColor() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual IFocusable* GetFocusable(GrColumn* pColumn) const;
    virtual int         GetMinHeight() const;

    virtual GrCell*     HitTest(const GrPoint& location) const;

protected:
    virtual void        OnGridCoreAttached();
    virtual void        OnGridCoreDetached();
    virtual void        OnHeightAdjusted();

private:
    void                SetVisibleDataRowIndex(uint index);
    void                SetDataRowIndex(uint index);
    void                SetDataRowID(uint index);
    void                AddItem(GrColumn* pColumn);
    void                Reserve(uint count);
    void                ClearItem();
    void                SetFocusedItem(bool b);        // called by GrGridCore


private:
    _Items              m_vecItems;
    bool                m_readOnly;

    GrColor             m_itemBackColor;
    GrColor             m_itemForeColor;
    GrFont*             m_pItemFont;

    uint                m_visibleDataRowIndex;
    uint                m_dataRowIndex;
    uint                m_dataRowID;

private:    // friend variables;
    uint                m_selectedCells;

    friend class GrItemSelector;
    friend class GrGridCore;
    friend class GrDataRowList;
};

typedef std::set<GrDataRow*>    GrDataRows;

class IFocusable
{
public:
    virtual IDataRow*   GetDataRow() const = 0;
    virtual GrRect      GetDisplayRect() const = 0;
    virtual bool        GetDisplayable() const = 0;
    virtual GrCellType  GetCellType() const = 0;

    static IFocusable*  Null;
};

class GrItem : public GrCell, public IFocusable
{
public:
    GrItem(GrColumn* pColumn, GrDataRow* pDataRow);

    GrRect              GetControlRect() const;
    bool                GetControlVisible() const;
    GrControlState      GetControlState() const;
    GrDataRow*          GetDataRow() const;
    GrColumn*           GetColumn() const;
    bool                GetSelected() const;
    bool                GetFocused() const;
    bool                IsItemSelecting() const;

    void                SetReadOnly(bool b);
    void                SetSelected(bool b);
    void                SetFocused(bool b);

    bool                ShouldBringIntoView() const;
    int                 GetMinHeight() const;
    void                BringIntoView();

    virtual int         HitMouseOverTest(const GrPoint& localLocation) const;

    virtual bool        GetVisible() const;
    virtual bool        GetReadOnly() const;
    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;
    virtual bool        GetTextWordWrap() const;
    virtual bool        GetTextMulitiline() const;
    virtual bool        GetClipped() const;

    virtual int         GetX() const;
    virtual int         GetY() const;
    virtual int         GetWidth() const;
    virtual int         GetHeight() const;
    virtual GrCellType  GetCellType() const { return GrCellType_Item; }

    virtual bool        GetDisplayable() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow*      GetRow() const;

    virtual GrFlag      ToPaintStyle() const;
    virtual GrFont*     GetPaintingFont() const;
    virtual GrColor     GetPaintingForeColor() const;
    virtual GrColor     GetPaintingBackColor() const;

    virtual GrColor     GetForeColor(bool inherited = true) const;
    virtual GrColor     GetBackColor(bool inherited = true) const;
    virtual GrPadding   GetPadding(bool inherited = true) const;
    virtual GrFont*     GetFont(bool inherited = true) const;

    void                LockColor(bool b);

protected:
    virtual void        OnTextSizeChanged();

private:
    void                DoSelected(bool b);
    virtual GrRect      GetDisplayRect() const { return GetRect(); }

private:
    GrDataRow*          m_pDataRow;
    GrColumn*           m_pColumn;

    bool                m_readOnly;
    bool                m_selected;
    bool                m_colorLocked;

    friend class GrItemSelector;
};

typedef std::set<GrItem*> GrItems;

class GrUpdatableRow : public GrRow
{
public:
    virtual bool        ShouldUpdate() const;
    virtual void        Update(bool force);
    virtual int         GetUpdatePriority() const;

    virtual bool        ShouldClip(const GrRect& displayRect, uint horizontal, uint vertical) const;
    virtual void        Clip(const GrRect& displayRect, uint horizontal, uint vertical);
    virtual int         GetClipPriority() const;

protected:
    virtual void        OnFitted();
    virtual void        OnHeightChanged();
};

class GrRootRow : public GrRow
{
    typedef std::vector<GrRow*>    _Rows;
    typedef std::vector<GrUpdatableRow*>    _Updatables;
public:
    GrRootRow();

    void                SetVisibleChanged();
    void                SetFitChanged();
    void                SetHeightChanged();

    void                Update(bool force = false);

    bool                ShouldClip() const;
    void                Clip(const GrRect& displayRect, uint horizontal, uint vertical);

    virtual int         GetX() const;
    virtual int         GetY() const;
    virtual int         GetWidth() const { return m_width; }
    virtual int         GetHeight() const { return m_height; }

    virtual GrRect      GetBounds() const { return m_bound; }

    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    virtual GrCell*     HitTest(const GrPoint& location) const;

protected:
    virtual void        OnGridCoreAttached();

private:
    void                BuildVisibleList();
    void                AdjustRowHeight();
    void                RepositionVisibleList();

    void                gridCore_Created(GrObject* pSender, GrEventArgs* e);

private:
    _Rows               m_vecVisibleRows;
    _Updatables         m_vecUpdatables;
    bool                m_visibleChanged;
    bool                m_fitChanged;
    bool                m_heightChanged;

    int                 m_width;
    int                 m_height;

    GrRect              m_bound;
    GrColumnList*       m_pColumnList;

private:
    class SortUpdatable
    {
    public:
        bool operator () (const GrUpdatableRow* p1, const GrUpdatableRow* p2)
        {
            return p1->GetUpdatePriority() < p2->GetUpdatePriority();
        }
    };
};

class GrInsertionRow : public GrDataRow
{
public:
    GrInsertionRow();

    virtual GrRowType   GetRowType() const { return GrRowType_InsertionRow; }
    virtual void        SetVisible(bool b);

    virtual uint        GetSelectionGroup() const { return 1; }

protected:
    virtual void        OnGridCoreAttached();
    virtual void        OnHeightChanged();
};

class GrDataRowList : public GrUpdatableRow
{
    struct GrCache
    {
        int     height;
        bool    expanded;
    };

    typedef std::vector<GrColumn*>          _Columns;
    typedef std::vector<GrDataRow*>         _DataRows;
    typedef std::vector<IDataRow*>          _IDataRows;
    typedef std::vector<GrGroupingRow*>     _GroupingRows;
    typedef std::map<std::wstring, GrCache> _MapCaches;

    typedef GrEvent<GrEventArgs, GrDataRowList> _GrEvent;
public:
    GrDataRowList();
    virtual ~GrDataRowList();

    void                Reserve(uint reserve);

    void                AdjustRowHeight();
    int                 GetRowWidth() const;
    void                SetRowWidth(int width);

    uint                GetVisibleRowCount() const;
    IDataRow*           GetVisibleRow(uint index) const;

    uint                GetVisibleDataRowCount() const;
    GrDataRow*          GetVisibleDataRow(uint index) const;

    GrDataRow*          NewDataRowFromInsertion();

    void                AddDataRow(GrDataRow* pDataRow);
    void                RemoveDataRow(GrDataRow* pDataRow);
    void                InsertDataRow(GrDataRow* pDataRow, uint index);
    uint                GetDataRowCount() const;
    GrDataRow*          GetDataRow(uint index) const;
    void                Clear();

    uint                GetDisplayableRowCount() const;
    IDataRow*           GetDisplayableRow(uint index) const;
    int                 GetDisplayableBottom() const;

    uint                ClipFrom(uint visibleFrom) const;
    uint                ClipFrom(const GrRect& displayRect, uint visibleFrom) const;
    uint                ClipTo(uint visibleTo) const;
    uint                ClipTo(const GrRect& displayRect, uint visibleTo) const;
    IDataRow*           HitTest(int y) const;
    GrIndexRange        HitTest(int y, IDataRow* pRowAnchor) const;
    void                BringIntoView(IDataRow* pDataRow);

    void                Sort(GrColumn* pColumn);

    void                SetZeroBasedRowIndex(bool b);
    bool                GetZeroBasedRowIndex() const;

    bool                GetRowNumberVisible() const;
    void                SetRowNumberVisible(bool b);

    void                SetFitChanged();
    void                SetVisibleChanged();
    void                SetHeightChanged();
    void                SetListChanged();

    _GrEvent            VisibleChanged;

    virtual GrRect      GetBounds() const;
    virtual bool        ShouldClip(const GrRect& displayRect, uint horizontal, uint vertical) const;
    virtual void        Clip(const GrRect& displayRect, uint horizontal, uint vertical);
    virtual int         GetClipPriority() const { return 1; }
    virtual bool        ShouldUpdate() const;
    virtual void        Update(bool force = false);
    virtual int         GetUpdatePriority() const { return UPDATEPRIORITY_DATAROWLIST; }

    virtual GrRowType   GetRowType() const { return GrRowType_DataRowList; }
    virtual GrCell*     HitTest(const GrPoint& location) const;
    virtual int         GetWidth() const { return 0; }
    virtual int         GetHeight() const { return m_displayableHeight; }
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

protected:
    virtual void        OnGridCoreAttached();
    virtual void        OnYChanged();

private:
    GrGroupingRow*      CreateGroupingRow(GrRow* pParent, GrColumn* pColumn, const std::wstring& itemText);
    void                BuildGrouping(GrRow* pParent, uint groupingLevel);
    void                BuildChildRowList();
    void                BuildVisibleRowList();
    void                RepositionVisibleRowList();
    void                BuildCache();
    void                DeleteObjects();
    void                UpdateVertScroll(const GrRect& displayRect);

    void                groupingList_Changed(GrObject* pSender, GrEventArgs* e);
    void                groupingList_Expanded(GrObject* pSender, GrGroupingEventArgs* e);
    void                groupingList_SortChanged(GrObject* pSender, GrGroupingEventArgs* e);
    void                gridCore_Created(GrObject* pSender, GrEventArgs* e);
    void                gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void                gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);
    void                columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
    void                columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
    void                columnList_ColumnSortTypeChanged(GrObject* pSender, GrColumnEventArgs* e);
    void                focuser_FocusedChanged(GrObject* pSender, GrFocusChangeArgs* e);

private:
    int                 m_rowWidth;

    _DataRows           m_vecDataRowsRemoved;
    _DataRows           m_vecDataRows;
    _DataRows           m_vecVisibleDataRows;
    _IDataRows          m_vecVisibleRows;
    _IDataRows          m_vecDisplayableRows;
    _GroupingRows       m_vecGroupingRows;
    GrInsertionRow*     m_pInsertionRow;

    _MapCaches          m_mapCache;
    _Columns            m_vecColumns;

    uint                m_usedGroupingRow;
    uint                m_groupingCount;
    uint                m_dataRowID;

    int                 m_displayableBottom;
    int                 m_visibleBottom;
    int                 m_visibleHeight;
    int                 m_displayableHeight;

    GrIndexRange        m_selectingRange;
    GrDataRow*          m_pFocusedDataRow;
    GrRect              m_bound;

    bool                m_visibleRowNumber;
    bool                m_zeroBasedRowIndex;

    int                 m_clippedHeight;
    uint                m_clippedIndex;

    // flags
    bool                m_listChanged;
    bool                m_visibleChanged;
    bool                m_fitChanged;
    bool                m_heightChanged;
};

class GrColumnList : public GrUpdatableRow
{
    typedef std::vector<GrColumn*>        _Columns;
    typedef GrEvent<GrColumnEventArgs, GrColumnList> _GrColumnEvent;
    typedef GrEvent<GrColumnMouseEventArgs, GrColumnList> _GrColumnMouseEvent;

public:
    GrColumnList();
    virtual ~GrColumnList();

    void                Reserve(uint reserve);

    void                AddColumn(GrColumn* pColumn);
    void                RemoveColumn(GrColumn* pColumn);
    void                InsertColumn(GrColumn* pColumn, uint index);

    uint                GetColumnCount() const;
    GrColumn*           GetColumn(uint index) const;

    void                AdjustColumnWidth();

    uint                GetVisibleColumnCount() const;
    GrColumn*           GetVisibleColumn(uint index) const;
    int                 GetVisibleRight() const;

    uint                GetFrozenColumnCount() const;
    GrColumn*           GetFrozenColumn(uint index) const;

    uint                GetUnfrozenColumnCount() const;
    GrColumn*           GetUnfrozenColumn(uint index) const;
    int                 GetUnfrozenX() const { return m_unfrozenX; }

    GrColumnSplitter*   GetColumnSplitter() const { return m_pColumnSplitter; }

    uint                GetDisplayableColumnCount() const;
    GrColumn*           GetDisplayableColumn(uint index) const;

    GrColumn*           GetFirstSortColumn() const;

    bool                MoveToFrozen(GrColumn* pColumn, GrColumn* pWhere);
    bool                MoveToUnfrozen(GrColumn* pColumn, GrColumn* pWhere);

    uint                ClipFrom(uint visibleFrom) const;
    uint                ClipFrom(const GrRect& displayRect, uint visibleFrom) const;
    uint                ClipTo(uint visibleTo) const;
    uint                ClipTo(const GrRect& displayRect, uint visibleTo) const;

    GrColumn*           HitTest(int x) const;
    GrIndexRange        HitTest(int x, GrColumn* pColumnAnchor) const;
    void                BringIntoView(GrColumn* pColumn);

    void                SetFitChanged();
    void                SetVisibleChanged();
    void                SetWidthChanged();

    virtual bool        ShouldClip(const GrRect& displayRect, uint horizontal, uint vertical) const;
    virtual void        Clip(const GrRect& displayRect, uint horizontal, uint vertical);
    virtual int         GetClipPriority() const { return 0; }
    virtual bool        ShouldUpdate() const;
    virtual void        Update(bool force = false);
    virtual int         GetUpdatePriority() const { return UPDATEPRIORITY_COLUMNLIST; }

    virtual GrCell*     HitTest(const GrPoint& location) const;

    virtual bool        GetDisplayable() const { return true; }
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    void                PaintSplitter(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrCellType  GetCellType() const { return GrCellType_Root; }
    virtual int         GetWidth() const;

    virtual bool        GetVisible() const { return true; }
    virtual bool        GetReadOnly() const { return true; }
    virtual GrItemType  GetItemType() const { return GrItemType_Control; }
    virtual GrRowType   GetRowType() const { return GrRowType_ColumnList; }

    virtual int         GetMinHeight() const { return GetHeight(); }
    virtual GrRect      GetBounds() const;

    virtual void        Invoke(std::wstring eventName, GrEventArgs* e);

    _GrColumnEvent      ColumnInserted;
    _GrColumnEvent      ColumnRemoved;
    _GrColumnEvent      ColumnSortTypeChanged;
    _GrColumnEvent      ColumnFrozenChanged;
    _GrColumnEvent      ColumnWidthChanged;
    _GrColumnEvent      ColumnWordwrapChanged;
    _GrColumnEvent      ColumnMultilineChanged;
    _GrColumnEvent      ColumnHorzAlignChanged;
    _GrColumnEvent      ColumnVertAlignChanged;
    _GrColumnEvent      ColumnPaddingChanged;
    _GrColumnEvent      ColumnGroupingChanged;

    _GrColumnMouseEvent ColumnMouseMove;
    _GrColumnMouseEvent ColumnMouseEnter;
    _GrColumnMouseEvent ColumnMouseLeave;
    _GrColumnMouseEvent ColumnMouseDown;
    _GrColumnMouseEvent ColumnMouseUp;

protected:
    virtual void        OnGridCoreAttached();

    virtual void        OnColumnInserted(GrColumnEventArgs* e);
    virtual void        OnColumnRemoved(GrColumnEventArgs* e);

    virtual void        OnColumnSortTypeChanged(GrColumnEventArgs* e);
    virtual void        OnColumnFrozenChanged(GrColumnEventArgs* e);
    virtual void        OnColumnWidthChanged(GrColumnEventArgs* e);
    virtual void        OnColumnWordwrapChanged(GrColumnEventArgs* e);
    virtual void        OnColumnMultilineChanged(GrColumnEventArgs* e);
    virtual void        OnColumnHorzAlignChanged(GrColumnEventArgs* e);
    virtual void        OnColumnVertAlignChanged(GrColumnEventArgs* e);
    virtual void        OnColumnPaddingChanged(GrColumnEventArgs* e);
    virtual void        OnColumnGroupingChanged(GrColumnEventArgs* e);

    virtual void        OnColumnMouseMove(GrColumnMouseEventArgs* e);
    virtual void        OnColumnMouseEnter(GrColumnMouseEventArgs* e);
    virtual void        OnColumnMouseLeave(GrColumnMouseEventArgs* e);
    virtual void        OnColumnMouseDown(GrColumnMouseEventArgs* e);
    virtual void        OnColumnMouseUp(GrColumnMouseEventArgs* e);

    void                BuildVisibleColumnList();
    void                RepositionColumnList();

private:
    void                groupingList_Changed(GrObject* pSender, GrEventArgs* e);
    void                gridCore_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e);
    void                gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void                gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);
    void                column_GroupingChanged(GrObject* pSender, GrEventArgs* e);

private:
    bool                IsColumnSelecting(GrColumn* pColumn) const;
    void                UpdateHorzScroll(const GrRect& displayRect);

private:
    _Columns            m_vecColumns;
    _Columns            m_vecColumnsRemoved;
    _Columns            m_vecVisibleColumns;
    _Columns            m_vecDisplayableColumns;
    int                 m_columnID;

    int                 m_unfrozenX;
    int                 m_frozenRight;
    int                 m_displayableRight;
    int                 m_visibleRight;
    int                 m_boundWidth;

    uint                m_frozenCount;
    uint                m_groupingCount;

    GrIndexRange        m_selectingRange;
    GrColumn*           m_pSortColumn;
    GrColumnSplitter*   m_pColumnSplitter;

    int                 m_clippedWidth;
    uint                m_clippedIndex;

    // flag
    bool                m_visibleChanged;
    bool                m_fitChanged;
    bool                m_widthChanged;
};

class GrColumnSplitter : public GrCell
{
public:
    GrColumnSplitter(GrColumnList* pColumnList);

    virtual GrCellType  GetCellType() const { return GrCellType_Splitter; }
    virtual int         GetX() const;
    virtual int         GetY() const;
    virtual int         GetWidth() const;
    virtual int         GetHeight() const;
    virtual bool        GetVisible() const;

    virtual GrHorzAlign GetTextHorzAlign() const { return GrHorzAlign_Left; }
    virtual GrVertAlign GetTextVertAlign() const { return GrVertAlign_Top; }

    virtual bool        GetDisplayable() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow*      GetRow() const    { return m_pColumnList; }

    void                SetVisible(bool b);
    GrColumnList*       GetColumnList() const { return m_pColumnList; }

private:
    void                SetX(int x) { m_x = x; }

private:
    GrColumnList*       m_pColumnList;
    int                 m_x;
    bool                m_visible;
    friend class GrColumnList;

public:
    static const int DefaultSplitterWidth = 5;
};

class GrRowSplitter : public GrRow
{
public:
    GrRowSplitter();

    virtual GrRowType   GetRowType() const { return GrRowType_Splitter; }
    virtual int         GetWidth() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    virtual bool        GetVisible() const;
    virtual int         GetMinHeight() const { return DefaultSplitterHeight; }
    virtual bool        GetResizable() const { return false; }

public:
    static const int DefaultSplitterHeight = 3;
};

class GrGroupingCell : public GrCell, public IFocusable
{
public:
    GrGroupingCell(GrGroupingRow* pRow);

    virtual GrCellType  GetCellType() const { return GrCellType_Grouping; }
    virtual int         GetX() const;
    virtual int         GetY() const;
    virtual int         GetWidth() const;
    virtual int         GetHeight() const;
    virtual bool        GetVisible() const { return true; }

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

    virtual int         HitMouseOverTest(const GrPoint& localLocation) const;
    virtual bool        GetDisplayable() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow*      GetRow() const;
    virtual GrFlag      ToPaintStyle() const;

private:
    virtual GrRect      GetDisplayRect() const { return GetRect(); }
    virtual IDataRow*   GetDataRow() const;

private:
    GrGroupingRow*      m_pRow;
};

class GrGroupingRow : public IDataRow
{
public:
    GrGroupingRow();
    virtual ~GrGroupingRow();

    GrColumn*           GetColumn() const;

    void                Expand(bool b = true);
    bool                IsExpanded() const;

    GrGroupingCell*     GetLabel() const;
    uint                GetGroupingLevel() const { return m_groupingLevel; }

    virtual GrRowType   GetRowType() const { return GrRowType_GroupingRow; }
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual bool        GetVisible() const;
    virtual IFocusable* GetFocusable(GrColumn* pColumn) const;

    virtual GrCell*     HitTest(const GrPoint& location) const;

    std::wstring        GetKey() const { return m_key; }

protected:
    virtual void        OnHeightChanged();
    virtual void        OnUpdatePositionCell(int x, GrRect* pBounds);
    virtual void        OnGridCoreAttached();

private:
    void                SetReference(GrColumn* pColumn, const std::wstring& itemText);        // called by GrDataRowList;
    void                ProcessAfterGrouping();                    // called by GrDataRowList;
    friend class GrDataRowList;

private:
    bool                m_expanded;
    uint                m_groupingLevel;

    GrColumn*           m_pColumn;
    std::wstring        m_itemText;
    std::wstring        m_key;
private:
    GrGroupingCell*     m_pLabel;

#ifdef _MANAGED
public:
    gcroot<System::Object^> ManagedRef;
#endif
};

class GrCaption : public GrUpdatableRow
{
public:
    GrCaption();

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

    void                SetTextHorzAlign(GrHorzAlign align);
    void                SetTextVertAlign(GrVertAlign align);

    virtual GrRowType   GetRowType() const { return GrRowType_Caption; }
    virtual int         GetWidth() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual bool        GetVisible() const;
    void                SetVisible(bool b);

protected:
    virtual void        OnGridCoreAttached();

private:
    void                gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);

private:
    GrHorzAlign         m_horzAlign;
    GrVertAlign         m_vertAlign;

    bool                m_visible;

#ifdef _MANAGED
public:
    gcroot<System::Object^> ManagedRef;
#endif
};

class GrGroupingList : public GrUpdatableRow
{
    typedef std::vector<GrGroupingInfo*>                 _Groupings;
    typedef GrEvent<GrEventArgs, GrGroupingList>         _GrEvent;
    typedef GrEvent<GrGroupingEventArgs, GrGroupingList> _GrGroupingEvent;
public:
    GrGroupingList();
    virtual ~GrGroupingList();

    uint                GetGroupingCount() const;
    GrGroupingInfo*     GetGrouping(uint level) const;

    void                ExpandGrouping(uint level, bool expand);
    void                SetGroupingSortState(uint level, GrSort sortType);

    void                SetVisible(bool b);

    bool                CanBeGrouping() const;
    void                EnableGrouping(bool b);

    void                NotifyExpanded(GrGroupingInfo* pGroupingInfo);
    void                NotifySortChanged(GrGroupingInfo* pGroupingInfo);

    virtual bool        ShouldUpdate() const;
    virtual void        Update(bool force);
    virtual int         GetUpdatePriority() const { return UPDATEPRIORITY_GROUPINLIST; }

    virtual GrRowType   GetRowType() const { return GrRowType_GroupingList; }
    virtual int         GetWidth() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrHorzAlign GetTextHorzAlign() const { return GrHorzAlign_Left; }
    virtual bool        GetTextMultiline() const { return false; }

    virtual bool        GetVisible() const;
    virtual int         GetMinHeight() const;

    virtual GrCell*     HitTest(const GrPoint& location) const;

    _GrEvent            Changed;
    _GrGroupingEvent    Expanded;
    _GrGroupingEvent    SortChanged;

protected:
    virtual void        OnGridCoreAttached();
    virtual void        OnYChanged();

private:
    void                gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void                gridCore_Created(GrObject* pSender, GrEventArgs* e);
    void                gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);

    void                columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
    void                columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
    void                columnList_ColumnGroupingChanged(GrObject* pSender, GrColumnEventArgs* e);

    void                groupingInfo_LevelChanged(GrObject* pSender, GrEventArgs* e);

    void                ResetGroupingLevel();
    void                AddGrouping(GrGroupingInfo* pGroupingInfo);
    void                RemoveGrouping(GrGroupingInfo* pGroupingInfo);
    void                RepositionGrouping();

private:
    _Groupings          m_vecGroupings;
    bool                m_enableGrouping;
    bool                m_groupingChanged;
};

class GrGroupingInfo : public GrCell
{
    typedef std::map<uint, GrGroupingRow*> _MapGroupingRows;
    typedef GrEvent<GrEventArgs, GrGroupingInfo> _GrEvent;
public:
    GrGroupingInfo(GrColumn* pColumn);

    GrColumn*           GetColumn() const;

    bool                GetGrouped() const;
    void                SetGrouped(bool b);

    void                SetExpanded(bool b);
    bool                GetExpanded() const;

    void                SetSortType(GrSort sortType);
    GrSort              GetSortType() const;

    uint                GetGroupingLevel() const;
    void                SetGroupingLevel(uint level);

    void                SetText();

public: 
    virtual GrCellType  GetCellType() const { return GrCellType_GroupingInfo; }

    virtual int         GetX() const;
    virtual int         GetY() const;
    virtual int         GetWidth() const;
    virtual int         GetHeight() const;
    virtual bool        GetVisible() const { return true; }

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

    virtual bool        GetDisplayable() const;
    virtual void        Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow*      GetRow() const { return m_pGroupingList; }

public:
    _GrEvent            LevelChanged;

protected:
    virtual void        OnGridCoreAttached();
    virtual void        OnGridCoreDetached();

private:
    void                SetPosition(GrPoint pt);
    void                SetGroupingLevelCore(uint level);

private:
    GrGroupingList*     m_pGroupingList;
    GrColumn*           m_pColumn;
    GrPoint             m_pt;
    bool                m_expanded;
    bool                m_grouped;
    GrSort              m_sortType;

    _MapGroupingRows    m_mapGroupingRows;
    uint                m_level;

    friend class GrGroupingList;
};