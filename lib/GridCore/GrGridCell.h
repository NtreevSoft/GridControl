﻿//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4646.22417
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
namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class Column;
    ref class CaptionRow;
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/
#endif

static const unsigned int INVALID_INDEX = ((uint)-1);
static const unsigned int INSERTION_ROW = ((uint)-8);

static const int UPDATEPRIORITY_DATAROWLIST = 1;
static const int UPDATEPRIORITY_COLUMNLIST = 2;
static const int UPDATEPRIORITY_GROUPPANEL = 3;


class GrGridCore;
class GrRow;
class GrDataRow;
class GrColumn;
class GrGroup;
class GrGridPainter;

class GrColumnList;
class GrDataRowList;
class GrGroupRow;
class GrColumnSplitter;
class GrFont;
class GrItem;

class IDataRow;
class IFocusable;

typedef GrDataRow GrInsertionRow;
typedef std::set<GrItem*> GrItems;

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

class GrDataRowEventArgs : public GrEventArgs
{
public:
    GrDataRowEventArgs(GrDataRow* pDataRow) 
        : m_pDataRow(pDataRow) 
    {

    }

    GrDataRow* GetDataRow() const
    {
        return m_pDataRow;
    }

private:
    GrDataRow* m_pDataRow;
};

class GrDataRowInsertedEventArgs : public GrDataRowEventArgs
{
public:
    GrDataRowInsertedEventArgs(GrDataRow* pDataRow, GrDataRowInsertType insertType)
        : GrDataRowEventArgs(pDataRow), m_insertType(insertType)
    {

    }

    GrDataRowInsertType GetInsertType() const { return m_insertType; }
    void SetInsertType(GrDataRowInsertType insertType) { m_insertType = insertType; }

private:
    GrDataRowInsertType m_insertType;
};

class GrDataRowInsertingEventArgs : public GrDataRowEventArgs
{
public:
    GrDataRowInsertingEventArgs(GrDataRow* pDataRow, GrDataRowInsertType insertType)
        : GrDataRowEventArgs(pDataRow), m_cancel(false), m_insertType(insertType)
    {

    }

    GrDataRowInsertType GetInsertType() const { return m_insertType; }
    void SetInsertType(GrDataRowInsertType insertType) { m_insertType = insertType; }

    bool GetCancel() const { return m_cancel; }
    void SetCancel(bool b) { m_cancel = b; }

private:
    bool m_cancel;
    GrDataRowInsertType m_insertType;
};

class GrMouseEventArgs : public GrEventArgs
{
public:
    GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys);
    GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys, int delta);

    const GrPoint& GetLocation() const;
    int GetX() const;
    int GetY() const;
    bool GetControlKey() const;
    bool GetAltKey() const;
    bool GetShiftKey() const;
    GrKeys GetModifierKeys() const;
    int GetDelta() const;

private:
    GrPoint m_location;
    GrKeys m_modifierKeys;
    int m_delta;
};

class GrCellMouseEventArgs : public GrMouseEventArgs
{
public:
    GrCellMouseEventArgs(GrCell* pCell, const GrPoint& location, GrKeys modifierKeys);

    GrCell* GetCell() const;

private:
    GrCell* m_pCell;
};

class GrColumnMouseEventArgs : public GrMouseEventArgs
{
public:
    GrColumnMouseEventArgs(GrColumn* pColumn, const GrPoint& location, GrKeys modifierKeys);

    GrColumn* GetColumn() const;
    bool GetHandled() const;
    void SetHandled(bool value);

private:
    GrColumn* m_pColumn;
    bool m_handled;
};

class GrRowMouseEventArgs : public GrMouseEventArgs
{
public:
    GrRowMouseEventArgs(GrRow* pRow, const GrPoint& location, GrKeys modifierKeys);

    GrRow* GetRow() const;
    bool GetHandled() const;
    void SetHandled(bool value);

private:
    GrRow* m_pRow;
    bool m_handled;
};

class GrItemMouseEventArgs : public GrMouseEventArgs
{
public:
    GrItemMouseEventArgs(GrItem* pItem, const GrPoint& location, GrKeys modifierKeys);

    GrItem* GetItem() const;
    bool GetHandled() const;
    void SetHandled(bool value);

private:
    GrItem* m_pItem;
    bool m_handled;
};

class GrGroupEventArgs : public GrEventArgs
{
public:
    GrGroupEventArgs(GrGroup* pGroup) : m_pGroup(pGroup) {}

    GrGroup* m_pGroup;
};

class GrFocusChangeArgs : public GrEventArgs
{
public:
    GrFocusChangeArgs(IFocusable* pFocusable);

    IFocusable* GetFocusable() const;

private:
    IFocusable* m_pFocusable;
};


struct GrLineDesc
{
    int textBegin;
    int length;
    int width;
    int x;
    int y;
};

typedef std::vector<GrLineDesc> _TextLines;

class GrCell : public GrObject
{
public:
    GrCell();
    virtual ~GrCell();

    virtual int GetX() const = 0;
    virtual int GetY() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual GrRow* GetRow() const = 0;

    virtual GrCellType GetCellType() const = 0;
    virtual bool GetVisible() const = 0;
    virtual bool GetDisplayable() const = 0;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const = 0;

    bool Contains(const GrPoint& point) const;
    bool ContainsHorz(int x) const;
    bool ContainsVert(int y) const;
    bool GetMouseOvered() const;
    bool GetMousePressed() const;
    int GetMouseOverState() const;

    GrRect GetClientRect() const;
    GrRect GetRect() const;
    GrPoint GetLocation() const;
    GrSize GetSize() const;
    uint GetID() const;
    int GetBottom() const { return GetY() + GetHeight(); }
    int GetRight() const { return GetX() + GetWidth(); }

	void SetBackColor(const GrColor& color);
    void SetForeColor(const GrColor& color);
	void SetLineColor(const GrColor& color);
    void SetFont(GrFont* pFont);
    void SetPadding(const GrPadding& padding);

	GrColor GetForeColorCore() const;
    GrColor GetBackColorCore() const;
	GrColor GetLineColorCore() const;
	GrFont*	GetFontCore() const;

	// text method
    const std::wstring& GetText() const;
    void SetText(const std::wstring& text);
    uint GetTextLineCount() const;
    const GrLineDesc& GetTextLine(uint index) const;
    const GrRect& GetTextBounds() const;
    void AlignText();
    void ComputeTextBounds();
    bool GetTextVisible() const;
    bool GetTextClipped() const;
    void SetTextVisible(bool b);

    virtual GrFlag ToPaintStyle() const;
    void DrawText(GrGridPainter* pPainter, GrColor foreColor, const GrRect& paintRect, const GrRect* pClipRect = NULL) const;

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;
    virtual bool GetTextWordWrap() const;
    virtual bool GetTextMulitiline() const { return false; }

    virtual GrColor GetPaintingForeColor() const;
    virtual GrColor GetPaintingBackColor() const;
	virtual GrColor GetPaintingLineColor() const;
    virtual GrFont* GetPaintingFont() const;

    virtual GrColor GetForeColor() const;
    virtual GrColor GetBackColor() const;
	virtual GrColor GetLineColor() const;
    virtual GrPadding GetPadding(bool inherited = true) const;
    virtual GrFont* GetFont() const;

    virtual int HitMouseOverTest(const GrPoint& localLocation) const;

    GrGridCore* GetGridCore() const { return m_pGridCore; }

protected:
    virtual void OnTextChanged();
    virtual void OnTextSizeChanged();

    virtual void OnGridCoreAttached();
    virtual void OnGridCoreDetached();

    void SetTextBoundsChanged();
    void SetTextAlignChanged();

    void Invalidate(bool thisOnly = true);

public:
    void* Tag;

protected:
    class GrTextUpdater* m_pTextUpdater;

private:
    std::wstring m_text;

    _TextLines m_vecTextLine;
    GrRect m_textBounds;

    GrColor m_backColor;
    GrColor m_foreColor;
	GrColor m_lineColor;
    GrFont* m_pFont;

    GrPadding m_padding;
    uint m_id;

    bool m_textBoundsChanged;
    bool m_textAlignChanged;
    bool m_textClipped;
    bool m_textVisible;

    static uint m_snID;
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

    GrColumnList* GetColumnList() const;

    GrItemType GetItemType() const;
    GrItemTypeShow GetItemTypeShow() const;
    GrHorzAlign GetItemHorzAlign() const;
    GrVertAlign GetItemVertAlign() const;
    bool GetItemWordWrap() const;
    bool GetItemMultiline() const;
    const GrColor& GetItemForeColor() const;
    const GrColor& GetItemBackColor() const;
    const GrPadding& GetItemPadding() const;
    GrFont* GetItemFont() const;
    int GetItemMinHeight() const;
    GrClickEditing GetItemClickEditing() const;
    bool GetItemTextVisible() const;
    bool GetItemIcon() const;

    uint GetDisplayIndex() const;
    uint GetVisibleIndex() const;
    uint GetFrozenIndex() const;
    uint GetUnfrozenIndex() const;
    uint GetIndex() const;
    uint GetColumnID() const;

    void SetItemType(GrItemType type);
    void SetItemHorzAlign(GrHorzAlign horzAlign);
    void SetItemVertAlign(GrVertAlign vertAlign);
    void SetItemWordWrap(bool b);
    void SetItemMultiline(bool b);
    void SetItemForeColor(const GrColor& color);
    void SetItemBackColor(const GrColor& color);
    void SetItemPadding(const GrPadding& padding);
    void SetItemFont(GrFont* pFont);
    void SetItemMinHeight(int height);
    void SetItemClickEditing(GrClickEditing clickEditing);
    void SetItemTextVisible(bool b);
    void SetItemIcon(bool b);

    bool GetMovable() const;
    void SetMovable(bool b);
    bool GetResizable() const;
    void SetResizable(bool b);
    bool GetFrozen() const;
    void SetFrozen(bool b);

    bool GetReadOnly() const;
    void SetReadOnly(bool b);

    void SetVisible(bool b);
    void SetMinWidth(int minWidth);
    void SetMaxWidth(int maxWidth);
    int GetMinWidth() const;
    int GetMaxWidth() const;

    void SetSelected(bool b);
    bool GetSelected() const;
    bool GetFullSelected() const;
    bool GetSelecting() const;

    bool HasFocused() const;

    void AdjustWidth();
    void SetFit();

    void SetSortable(bool b = true);
    bool GetSortable() const;

    void SetGroupable(bool b = true);
    bool GetGroupable() const;

    void SetClipped(bool b);
    bool GetClipped() const;

    bool ShouldBringIntoView() const;

    void SetWidth(int width);

    void SetTooltip(const wchar_t* tooltip);
    const wchar_t* GetTooltip() const;

    int GetFreezablePriority() const;
    int GetUnfreezablePriority() const;

    void SetFreezablePriority(int priority);
    void SetUnfreezablePriority(int priority);

    bool GetGrouped() const;
    void SetGrouped(bool b);
    GrGroup* GetGroup() const;

    void SetSortType(GrSort sortType);
    GrSort GetSortType() const;
    void SetSortComparer(GrSort sortType, FuncComparer comparer);
    FuncComparer GetSortComparer(GrSort sortType) const;

    virtual GrCellType GetCellType() const { return GrCellType_Column; }
    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;

    virtual bool GetVisible() const;

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

	virtual GrColor GetForeColor() const;
	virtual GrColor GetBackColor() const;
	virtual GrColor GetLineColor() const;
	virtual GrFont* GetFont() const;

    virtual GrRow* GetRow() const;

    virtual GrFlag ToPaintStyle() const;
    virtual GrColor GetPaintingForeColor() const;
    virtual GrColor GetPaintingBackColor() const;
    virtual bool GetDisplayable() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

public:
    _GrEvent GroupChanged; 


protected:
    virtual void OnGridCoreAttached();
    virtual void OnGridCoreDetached();

    virtual void OnTextChanged();

private:
    virtual ~GrColumn();

    void SetX(int x) { m_x = x; };
    void SetDisplayable(bool b);
    void SetDisplayIndex(uint index);
    void SetVisibleIndex(uint index);
    void SetIndex(uint index);
    void SetColumnID(uint index);

    void AddSelection(GrItem* pItem);
    void RemoveSelection(GrItem* pItem);
    void ClearSelection();
    void SetFullSelected();

public:
    bool m_customItemPaint;

    FuncColumnPaint m_fnColumnBackgroundPaint;
    FuncColumnPaint m_fnColumnContentsPaint;
    void* m_pColumnPaintData;

private:
    GrColumnList* m_pColumnList;

    GrItems m_selectedCells; 
    friend class GrItemSelector;
    int m_freezablePriority;
    int m_unfreezablePriority;
    int m_x;
    int m_width;
    int m_fitWidth;

    int m_defaultWidth;
    int m_minWidth;
    int m_maxWidth;

    bool m_visible;
    bool m_readOnly;
    bool m_sortable;
    bool m_groupable;
    bool m_movable;
    bool m_resizable;
    bool m_frozen;
    bool m_selected;
    //bool m_fullSelected;
    bool m_fitting;
    bool m_grouped;

    uint m_displayIndex;
    uint m_visibleIndex;
    uint m_selectedIndex;
    uint m_index;
    uint m_columnID;

    bool m_displayable;
    bool m_clipped;

    GrItemType m_itemType;
    GrItemTypeShow m_itemTypeShow;
    GrHorzAlign m_itemHorzAlign;
    GrVertAlign m_itemVertAlign;
    bool m_itemWordWrap;
    bool m_itemMultiline;
    GrColor m_itemBackColor;
    GrColor m_itemForeColor;
    GrPadding m_itemPadding;
    GrFont* m_pItemFont;
    int m_itemMinHeight;
    GrClickEditing m_itemClickEditing;
    bool m_itemTextVisible;
    bool m_itemIcon;

    GrGroup* m_pGroup;

    GrSort m_sortType;
    FuncComparer m_comparer[GrSort_Count];

    std::wstring m_tooltip;

    friend class GrGroup;
    friend class GrColumnList;

#ifdef _MANAGED
    friend ref class Ntreev::Windows::Forms::Grid::Column;
private:
    bool ShouldSerializeWidth();
    bool ShouldSerializePriorityOnFrozen();
    bool ShouldSerializePriorityOnUnfrozen();
#endif
};

enum GrRowUpdate 
{
    GrRowUpdate_Cell = 1,
    GrRowUpdate_Row = 2,
    GrRowUpdate_Both = 3,
};

class GrRow : public GrCell
{
public:
    GrRow();

    void SetY(int y);
    virtual int GetY() const;

    void SetHeight(int height);
    virtual int GetHeight() const;

    void SetResizable(bool b);
    virtual bool GetResizable() const;

    void AdjustHeight();

    virtual int GetMinHeight() const;

    virtual GrRow* GetRow() const { return const_cast<GrRow*>(this); }
    virtual GrCellType GetCellType() const { return GrCellType_Row; }
    virtual int GetX() const;
    virtual bool GetDisplayable() const { return true; }
    virtual bool GetVisible() const;
    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;
    virtual GrRowType GetRowType() const { return GrRowType_Dummy; }

    virtual bool ShouldBringIntoView() const { return false; }
    virtual GrCell* HitTest(const GrPoint& location) const;

    uint GetChildCount() const;
    GrRow* GetChild(uint index) const;
    GrRow* GetParent() const;
    uint GetDepth() const;

    void SetFit();
    virtual void Paint(GrGridPainter* /*pPainter*/, const GrRect& /*clipRect*/) const {};

    //virtual void GetVisibleList(GrRowArray* pVisible) const;
    virtual GrRect GetBounds() const { return GetRect(); }
    virtual GrPadding GetPadding(bool /*inherited*/) const { return GrPadding::Default; }

    virtual void Sort(GrSort sortType);
    virtual void Sort(FuncSortRow fnSort, void* userData);

    void AddChild(GrRow* pRow);
    void ReserveChild(uint reserve);
    void ClearChild();

public:
    static int DefaultHeight;

protected:
    virtual void OnGridCoreAttached();
    virtual void OnTextSizeChanged();
    virtual void OnHeightChanged();
    virtual void OnYChanged() {};
    virtual void OnHeightAdjusted() {};

    virtual void OnFitted();

    void InvalidateRow();
    void UpdateDepth(GrRow* pRow);

protected:
    bool m_visible;

private:
    GrRowArray m_vecChilds;
    GrRow* m_pParent;
    uint m_depth;

    int m_y;
    int m_height;

    bool m_resizable;
    bool m_fitting;

    friend class GrGridCore;

private:
    class SortDesc
    {
    public:
        SortDesc(GrGridCore* pGridCore, FuncSortRow fn, void* userData)
            : m_pGridCore(pGridCore), m_fn(fn), m_userData(userData)
        {
        }

        bool operator () (const GrRow* pRow1, const GrRow* pRow2)
        {
            return (*m_fn)(m_pGridCore, pRow1, pRow2, m_userData);
        }

        GrGridCore* m_pGridCore;
        FuncSortRow m_fn;
        void* m_userData;
    };
};

class IDataRow : public GrRow
{
public: 
    IDataRow();

    virtual int GetY() const;
    virtual int GetWidth() const;

    virtual bool GetDisplayable() const;
    void SetDisplayable(bool b);

    void SetVisibleIndex(uint index);
    uint GetVisibleIndex() const;

    void SetDisplayIndex(uint index);
    uint GetDisplayIndex() const;

    void SetClipped(bool b);
    bool GetClipped() const;

    bool ShouldBringIntoView() const;
    bool HasFocused() const;

    unsigned int GetSelectionGroup() const { return m_selectionGroup; }

    virtual IFocusable* GetFocusable(GrColumn* pColumn) const = 0;
    virtual bool GetFullSelected() const { return false; }

	virtual GrColor GetForeColor() const;
    virtual GrColor GetBackColor() const;
	virtual GrColor GetLineColor() const;
    virtual GrFont* GetFont() const;

protected:
    virtual void OnFitted();
    virtual void OnGridCoreAttached();
    virtual void OnHeightChanged();

    GrColor GetCellBackColor() const;
    GrColor GetCellLineColor() const;

protected:
    GrDataRowList* m_pDataRowList;

private:
    bool m_displayable;
    bool m_clipped;
    uint m_visibleIndex;
    uint m_displayIndex;
    int m_selectionGroup;
};

class GrDataRow : public IDataRow
{
protected:
    typedef std::vector<GrItem*> _Items;

    GrDataRow();
    virtual ~GrDataRow();

public:

    GrItem* GetItem(const GrColumn* pColumn) const;
    GrItem* GetItem(const GrItem* pOtherItem) const;

    bool GetReadOnly() const;
    void SetReadOnly(bool b = true);

    void SetSelected(bool b);
    bool GetSelected() const;
    bool GetSelecting() const;

    uint GetVisibleDataRowIndex() const;
    uint GetDataRowIndex() const;
    uint GetDataRowID() const;

    GrColor GetItemBackColor() const;
    GrColor GetItemForeColor() const;
    GrFont* GetItemFont() const;
    void SetItemBackColor(const GrColor& color);
    void SetItemForeColor(const GrColor& color);
    void SetItemFont(GrFont* pFont);

    virtual bool GetFullSelected() const;

    virtual bool GetVisible() const;
    virtual void SetVisible(bool b);

    virtual GrRowType GetRowType() const { return GetDataRowID() == INSERTION_ROW ? GrRowType_InsertionRow : GrRowType_DataRow; }
    virtual GrFlag ToPaintStyle() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual IFocusable* GetFocusable(GrColumn* pColumn) const;
    virtual int GetMinHeight() const;

    virtual GrCell* HitTest(const GrPoint& location) const;

protected:
    virtual void OnGridCoreAttached();
    virtual void OnGridCoreDetached();
    virtual void OnHeightAdjusted();
    virtual void OnHeightChanged();

private:
    void SetVisibleDataRowIndex(uint index);
    void SetDataRowIndex(uint index);
    void SetDataRowID(uint index);
    void AddItem(GrColumn* pColumn);
    void Reserve(uint count);
    void ClearItem();

    void AddSelection(GrItem* pItem);
    void RemoveSelection(GrItem* pItem);
    void ClearSelection();
    void SetFullSelected();

private:
    _Items m_vecItems;
    bool m_readOnly;

    GrColor m_itemBackColor;
    GrColor m_itemForeColor;
    GrFont* m_pItemFont;

    uint m_visibleDataRowIndex;
    uint m_dataRowIndex;
    uint m_dataRowID;

private: // friend variables;
    //uint m_selectedCells;
    GrItems m_selectedCells;
    bool m_selected;

    friend class GrItemSelector;
    friend class GrGridCore;
    friend class GrDataRowList;
};

class GrGridCell;

class GrGridRow : public IDataRow
{
public:
    GrGridRow(GrGridCore* pChildGrid);

    virtual IFocusable* GetFocusable(GrColumn* pColumn) const;

    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

protected:
    virtual void OnGridCoreAttached();


private:
    GrGridCell* m_pCell;
    GrGridCore* m_pChildGrid;
};

typedef std::set<GrDataRow*> GrDataRows;

class IFocusable
{
public:
    virtual IDataRow* GetDataRow() const = 0;
    virtual GrRect GetDisplayRect() const = 0;
    virtual bool GetDisplayable() const = 0;
    virtual GrCellType GetCellType() const = 0;

    static IFocusable* Null;
};

class GrItem : public GrCell, public IFocusable
{
public:
    GrItem(GrColumn* pColumn, GrDataRow* pDataRow);

    GrRect GetControlRect() const;
    bool GetControlVisible() const;
    GrControlState GetControlState() const;
    GrDataRow* GetDataRow() const;
    GrColumn* GetColumn() const;
    bool GetSelected() const;
    bool GetFocused() const;
    bool IsItemSelecting() const;

    void SetReadOnly(bool b);
    void SetSelected(bool b);
    void SetFocused(bool b);

    const std::wstring& GetErrorDescription() const;
    void SetErrorDescription(const std::wstring& errorDescription);

    bool ShouldBringIntoView() const;
    int GetMinHeight() const;
    void BringIntoView();

    virtual int HitMouseOverTest(const GrPoint& localLocation) const;

    virtual bool GetVisible() const;
    virtual bool GetReadOnly() const;
    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;
    virtual bool GetTextWordWrap() const;
    virtual bool GetTextMulitiline() const;
    virtual bool GetClipped() const;

    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual GrCellType GetCellType() const { return GrCellType_Item; }

    virtual bool GetDisplayable() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow* GetRow() const;

    virtual GrFlag ToPaintStyle() const;
    virtual GrFont* GetPaintingFont() const;
    virtual GrColor GetPaintingForeColor() const;
    virtual GrColor GetPaintingBackColor() const;

    virtual GrColor GetForeColor() const;
    virtual GrColor GetBackColor() const;
    virtual GrPadding GetPadding(bool inherited = true) const;
    virtual GrFont* GetFont() const;

    void LockColor(bool b);

protected:
    virtual void OnTextSizeChanged();

private:
    void DoSelected(bool b);
    virtual GrRect GetDisplayRect() const { return GetRect(); }

private:
    GrDataRow* m_pDataRow;
    GrColumn* m_pColumn;

    bool m_readOnly;
    bool m_selected;
    bool m_colorLocked;
    std::wstring m_errorDescription;

    friend class GrItemSelector;
};

class GrGridCell : public GrCell, public IFocusable
{
public:
    GrGridCell(GrGridRow* pGridRow, GrGridCore* pChildGrid);

    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual GrRow* GetRow() const;

    virtual GrCellType GetCellType() const;
    virtual bool GetVisible() const;
    virtual bool GetDisplayable() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual IDataRow* GetDataRow() const;

private:
    virtual GrRect GetDisplayRect() const { return GetRect(); }
private:
    GrGridRow* m_pGridRow;
    GrGridCore* m_pChildGrid;
};

class GrUpdatableRow : public GrRow
{
public:
    virtual bool ShouldUpdate() const;
    virtual void Update(bool force);
    virtual int GetUpdatePriority() const;

    virtual bool ShouldClip(const GrRect& displayRect, uint horizontal, uint vertical) const;
    virtual void Clip(const GrRect& displayRect, uint horizontal, uint vertical);
    virtual int GetClipPriority() const;

protected:
    virtual void OnFitted();
    virtual void OnHeightChanged();
};

class GrRootRow : public GrRow
{
    typedef std::vector<GrRow*> _Rows;
    typedef std::vector<GrUpdatableRow*> _Updatables;
public:
    GrRootRow();

    void SetVisibleChanged();
    void SetFitChanged();
    void SetHeightChanged();

    void Update(bool force = false);

    bool ShouldClip() const;
    void Clip(const GrRect& displayRect, uint horizontal, uint vertical);

    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWidth() const { return m_width; }
    virtual int GetHeight() const { return m_height; }

    virtual GrRect GetBounds() const { return m_bound; }

    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    virtual GrCell* HitTest(const GrPoint& location) const;

protected:
    virtual void OnGridCoreAttached();

private:
    void BuildVisibleList();
    void AdjustRowHeight();
    void RepositionVisibleList();

    void gridCore_Created(GrObject* pSender, GrEventArgs* e);
    void dataRowList_DataRowInserted(GrObject* pSender, GrDataRowInsertedEventArgs* e);

private:
    _Rows m_vecVisibleRows;
    _Updatables m_vecUpdatables;
    bool m_visibleChanged;
    bool m_fitChanged;
    bool m_heightChanged;

    int m_width;
    int m_height;

    GrRect m_bound;
    GrColumnList* m_pColumnList;
    GrDataRow* m_pInsertionRow;

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

class GrColumnList : public GrUpdatableRow
{
    typedef std::vector<GrColumn*> _Columns;
    typedef GrEvent<GrColumnEventArgs, GrColumnList> _GrColumnEvent;
    typedef GrEvent<GrColumnMouseEventArgs, GrColumnList> _GrColumnMouseEvent;

public:
    GrColumnList();
    virtual ~GrColumnList();

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

    GrColumn* GetFirstSortColumn() const;

    bool MoveToFrozen(GrColumn* pColumn, GrColumn* pWhere);
    bool MoveToUnfrozen(GrColumn* pColumn, GrColumn* pWhere);

    uint ClipFrom(uint visibleFrom) const;
    uint ClipFrom(const GrRect& displayRect, uint visibleFrom) const;
    uint ClipTo(uint visibleTo) const;
    uint ClipTo(const GrRect& displayRect, uint visibleTo) const;

    GrColumn* HitTest(int x) const;
    GrIndexRange HitTest(int x, GrColumn* pColumnAnchor) const;
    void BringIntoView(GrColumn* pColumn);

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

    _GrColumnMouseEvent ColumnMouseMove;
    _GrColumnMouseEvent ColumnMouseEnter;
    _GrColumnMouseEvent ColumnMouseLeave;
    _GrColumnMouseEvent ColumnMouseDown;
    _GrColumnMouseEvent ColumnMouseUp;

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
    void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);
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
};

class GrColumnSplitter : public GrCell
{
public:
    GrColumnSplitter(GrColumnList* pColumnList);

    virtual GrCellType GetCellType() const { return GrCellType_Splitter; }
    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual bool GetVisible() const;

    virtual GrHorzAlign GetTextHorzAlign() const { return GrHorzAlign_Left; }
    virtual GrVertAlign GetTextVertAlign() const { return GrVertAlign_Top; }

    virtual bool GetDisplayable() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow* GetRow() const { return m_pColumnList; }

    void SetVisible(bool b);
    GrColumnList* GetColumnList() const { return m_pColumnList; }

private:
    void SetX(int x) { m_x = x; }

private:
    GrColumnList* m_pColumnList;
    int m_x;
    bool m_visible;
    friend class GrColumnList;

public:
    static const int DefaultSplitterWidth = 5;
};

class GrRowSplitter : public GrRow
{
public:
    GrRowSplitter();

    virtual GrRowType GetRowType() const { return GrRowType_Splitter; }
    virtual int GetWidth() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
    virtual bool GetVisible() const;
    virtual int GetMinHeight() const { return DefaultSplitterHeight; }
    virtual bool GetResizable() const { return false; }

public:
    static const int DefaultSplitterHeight = 3;
};

class GrGroupHeader : public GrCell, public IFocusable
{
public:
    GrGroupHeader(GrGroupRow* pRow);

    virtual GrCellType GetCellType() const { return GrCellType_GroupHeader; }
    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual bool GetVisible() const { return true; }

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

    virtual int HitMouseOverTest(const GrPoint& localLocation) const;
    virtual bool GetDisplayable() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow* GetRow() const;
    virtual GrFlag ToPaintStyle() const;

	virtual GrColor GetForeColor() const;
    virtual GrColor GetBackColor() const;
	virtual GrColor GetLineColor() const;
    virtual GrFont* GetFont() const;

private:
    virtual GrRect GetDisplayRect() const { return GetRect(); }
    virtual IDataRow* GetDataRow() const;

private:
    GrGroupRow* m_pRow;
};

class GrGroupRow : public IDataRow
{
public:
    GrGroupRow();
    virtual ~GrGroupRow();

    GrColumn* GetColumn() const;

    void Expand(bool b = true);
    bool IsExpanded() const;

    GrGroupHeader* GetLabel() const;
    uint GetGroupLevel() const { return m_groupLevel; }

    virtual GrRowType GetRowType() const { return GrRowType_GroupRow; }
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual bool GetVisible() const;
    virtual IFocusable* GetFocusable(GrColumn* pColumn) const;

    virtual GrCell* HitTest(const GrPoint& location) const;

    std::wstring GetKey() const { return m_key; }

protected:
    virtual void OnHeightChanged();
    virtual void OnUpdatePositionCell(int x, GrRect* pBounds);
    virtual void OnGridCoreAttached();

private:
    void SetReference(GrColumn* pColumn, const std::wstring& itemText); // called by GrDataRowList;
    void ProcessAfterGroup(); // called by GrDataRowList;
    friend class GrDataRowList;

private:
    bool m_expanded;
    uint m_groupLevel;

    GrColumn* m_pColumn;
    std::wstring m_itemText;
    std::wstring m_key;
private:
    GrGroupHeader* m_pLabel;
};

class GrCaption : public GrUpdatableRow
{
    typedef GrEvent<GrEventArgs, GrCaption> _GrEvent;
public:
    GrCaption();

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

    void SetTextHorzAlign(GrHorzAlign align);
    void SetTextVertAlign(GrVertAlign align);

    virtual GrRowType GetRowType() const { return GrRowType_Caption; }
    virtual int GetWidth() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual bool GetVisible() const;
    void SetVisible(bool b);

    virtual GrColor GetForeColor() const;
	virtual GrColor GetBackColor() const;
	virtual GrColor GetLineColor() const;
	virtual GrFont* GetFont() const;

    _GrEvent HeightChanged;

protected:
    virtual void OnGridCoreAttached();
    virtual void OnHeightChanged(GrEventArgs* e);
    virtual void OnHeightChanged();

private:
    void gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);

private:
    GrHorzAlign m_horzAlign;
    GrVertAlign m_vertAlign;

    bool m_visible;

#ifdef _MANAGED
    friend ref class Ntreev::Windows::Forms::Grid::CaptionRow;
private:
    bool ShouldSerializeHeight();
#endif
};

class GrGroupPanel : public GrUpdatableRow
{
    typedef std::vector<GrGroup*> _Groups;
    typedef GrEvent<GrEventArgs, GrGroupPanel> _GrEvent;
    typedef GrEvent<GrGroupEventArgs, GrGroupPanel> _GrGroupEvent;
public:
    GrGroupPanel();
    virtual ~GrGroupPanel();

    uint GetGroupCount() const;
    GrGroup* GetGroup(uint level) const;

    void ExpandGroup(uint level, bool expand);
    void SetGroupSortState(uint level, GrSort sortType);

    void SetVisible(bool b);

    bool GetGroupable() const;
    void SetGroupable(bool b);

    void NotifyExpanded(GrGroup* pGroup);
    void NotifySortChanged(GrGroup* pGroup);

    virtual bool ShouldUpdate() const;
    virtual void Update(bool force);
    virtual int GetUpdatePriority() const { return UPDATEPRIORITY_GROUPPANEL; }

    virtual GrRowType GetRowType() const { return GrRowType_GroupPanel; }
    virtual int GetWidth() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrHorzAlign GetTextHorzAlign() const { return GrHorzAlign_Left; }
    virtual bool GetTextMultiline() const { return false; }

    virtual bool GetVisible() const;
    virtual int GetMinHeight() const;

    virtual GrCell* HitTest(const GrPoint& location) const;

    virtual GrColor GetForeColor() const;
	virtual GrColor GetBackColor() const;
	virtual GrColor GetLineColor() const;
	virtual GrFont* GetFont() const;
    virtual bool GetResizable() const { return false; }

    _GrEvent Changed;
    _GrGroupEvent Expanded;
    _GrGroupEvent SortChanged;

protected:
    virtual void OnGridCoreAttached();
    virtual void OnYChanged();

private:
    void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void gridCore_Created(GrObject* pSender, GrEventArgs* e);
    void gridCore_FontChanged(GrObject* pSender, GrEventArgs* e);

    void columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
    void columnList_ColumnGroupChanged(GrObject* pSender, GrColumnEventArgs* e);

    void groupInfo_LevelChanged(GrObject* pSender, GrEventArgs* e);

    void ResetGroupLevel();
    void AddGroup(GrGroup* pGroup);
    void RemoveGroup(GrGroup* pGroup);
    void RepositionGroup();

private:
    _Groups m_vecGroups;
    bool m_enableGroup;
    bool m_groupChanged;
};

class GrGroup : public GrCell
{
    typedef std::map<uint, GrGroupRow*> _MapGroupRows;
    typedef GrEvent<GrEventArgs, GrGroup> _GrEvent;
public:
    GrGroup(GrColumn* pColumn);

    GrColumn* GetColumn() const;

    bool GetGrouped() const;
    void SetGrouped(bool b);

    void SetExpanded(bool b);
    bool GetExpanded() const;

    void SetSortType(GrSort sortType);
    GrSort GetSortType() const;

    uint GetGroupLevel() const;
    void SetGroupLevel(uint level);

    void SetText();

public: 
    virtual GrCellType GetCellType() const { return GrCellType_Group; }

    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual bool GetVisible() const { return true; }

    virtual GrHorzAlign GetTextHorzAlign() const;
    virtual GrVertAlign GetTextVertAlign() const;

    virtual bool GetDisplayable() const;
    virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    virtual GrRow* GetRow() const { return m_pGroupPanel; }

public:
    _GrEvent LevelChanged;
    static const int SortGlyphSize = 10;

protected:
    virtual void OnGridCoreAttached();
    virtual void OnGridCoreDetached();

private:
    void SetPosition(GrPoint pt);
    void SetGroupLevelCore(uint level);

private:
    GrGroupPanel* m_pGroupPanel;
    GrColumn* m_pColumn;
    GrPoint m_pt;
    bool m_expanded;
    bool m_grouped;
    GrSort m_sortType;

    _MapGroupRows m_mapGroupRows;
    uint m_level;

    friend class GrGroupPanel;
};