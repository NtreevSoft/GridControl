//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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


#include "GrGridCell.h"
#include "GrGridCore.h"
#include "GrGridPainter.h"
#include "GrGridInternal.h"
#include <assert.h>
#include <limits.h>

IFocusable* IFocusable::Null = 0;
uint GrCell::m_snID = 0;

int GrRow::DefaultHeight = 21;

GrEventArgs GrEventArgs::Empty;

class SortColumn
{
public:
    bool operator () (const GrColumn* pColumn1, const GrColumn* pColumn2)
    {
        if(pColumn1->GetFrozen() == pColumn2->GetFrozen())
        {
            if(pColumn1->GetPriority() == pColumn2->GetPriority())
                return pColumn1->GetIndex() < pColumn2->GetIndex();
            return pColumn1->GetPriority() < pColumn2->GetPriority();
        }
        return pColumn2->GetFrozen() < pColumn1->GetFrozen();
    }
};

GrMouseEventArgs::GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys)
: m_location(location), m_modifierKeys(modifierKeys), m_delta(0)
{

}

GrMouseEventArgs::GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys, int delta)
: m_location(location), m_modifierKeys(modifierKeys), m_delta(delta)
{

}

const GrPoint& GrMouseEventArgs::GetLocation() const
{
    return m_location;
}

int GrMouseEventArgs::GetX() const
{
    return m_location.x;
}

int GrMouseEventArgs::GetY() const
{
    return m_location.y;
}

GrKeys GrMouseEventArgs::GetModifierKeys() const
{
    return m_modifierKeys;
}

bool GrMouseEventArgs::GetControlKey() const
{
    return m_modifierKeys & GrKeys_Control ? true : false;
}

bool GrMouseEventArgs::GetAltKey() const
{
    return m_modifierKeys & GrKeys_Alt ? true : false;
}

bool GrMouseEventArgs::GetShiftKey() const
{
    return m_modifierKeys & GrKeys_Shift ? true : false;
}

int GrMouseEventArgs::GetDelta() const
{
    return m_delta;
}

GrColumnMouseEventArgs::GrColumnMouseEventArgs(GrColumn* pColumn, const GrPoint& location, GrKeys modifierKeys)
: GrMouseEventArgs(location, modifierKeys), m_pColumn(pColumn), m_handled(false)
{

}

GrColumn* GrColumnMouseEventArgs::GetColumn() const
{
    return m_pColumn;
}

bool GrColumnMouseEventArgs::GetHandled() const
{
    return m_handled;
}

void GrColumnMouseEventArgs::SetHandled(bool value)
{
    m_handled = value;
}

GrCellMouseEventArgs::GrCellMouseEventArgs(GrCell* pCell, const GrPoint& location, GrKeys modifierKeys)
: GrMouseEventArgs(location, modifierKeys), m_pCell(pCell)
{

}

GrCell* GrCellMouseEventArgs::GetCell() const
{
    return m_pCell;
}

GrItemMouseEventArgs::GrItemMouseEventArgs(GrItem* pItem, const GrPoint& location, GrKeys modifierKeys)
: GrMouseEventArgs(location, modifierKeys), m_pItem(pItem), m_handled(false)
{

}

GrItem* GrItemMouseEventArgs::GetItem() const
{
    return m_pItem;
}

bool GrItemMouseEventArgs::GetHandled() const
{
    return m_handled;
}

void GrItemMouseEventArgs::SetHandled(bool value)
{
    m_handled = value;
}

GrRowMouseEventArgs::GrRowMouseEventArgs(GrRow* pRow, const GrPoint& location, GrKeys modifierKeys)
: GrMouseEventArgs(location, modifierKeys), m_pRow(pRow), m_handled(false)
{

}

GrRow* GrRowMouseEventArgs::GetRow() const
{
    return m_pRow;
}

bool GrRowMouseEventArgs::GetHandled() const
{
    return m_handled;
}

void GrRowMouseEventArgs::SetHandled(bool value)
{
    m_handled = value;
}

GrFocusChangeArgs::GrFocusChangeArgs(IFocusable* pFocusable)
: m_pFocusable(pFocusable)
{

}

IFocusable* GrFocusChangeArgs::GetFocusable() const 
{ 
    return m_pFocusable; 
}

GrColumnList::GrColumnList()
{
    m_pColumnSplitter = new GrColumnSplitter(this);

    SetTextVisible(false);
    SetText(L"ColumnList");

    m_frozenCount = 0;
    m_groupCount = 0;
    m_pSortColumn = NULL;
    m_displayableRight = 0;
    m_columnID = 0;

    m_visibleChanged = false;
    m_fitChanged = false;
    m_widthChanged = false;
    m_clippedIndex = INVALID_INDEX;

    SetResizable(false);
}

void GrColumnList::OnGridCoreAttached()
{
    GrRow::OnGridCoreAttached();
    m_pGridCore->AttachObject(m_pColumnSplitter);

    GrGroupPanel* pGroupList = m_pGridCore->GetGroupPanel();
    pGroupList->Changed.Add(this, &GrColumnList::groupPanel_Changed);

    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    pFocuser->FocusChanged.Add(this, &GrColumnList::gridCore_FocusChanged);

    m_pGridCore->Cleared.Add(this, &GrColumnList::gridCore_Cleared);
    m_pGridCore->FontChanged.Add(this, &GrColumnList::gridCore_FontChanged);
}

GrColumnList::~GrColumnList()
{
    for_each(_Columns, m_vecColumns, value)
    {
        delete value;
    }

    for_each(_Columns, m_vecColumnsRemoved, value)
    {
        delete value;
    }

    delete m_pColumnSplitter;
}

int GrColumnList::GetWidth() const
{
    if(m_pGridCore->GetRowVisible() == false)
        return 0;
    return m_pGridCore->GetDataRowList()->GetRowWidth();
}

bool GrColumnList::ShouldUpdate() const
{
    return m_visibleChanged == true ||
        m_fitChanged == true ||
        m_widthChanged == true;
}

void GrColumnList::Update(bool force)
{
    if(m_visibleChanged == true || force == true)
        BuildVisibleColumnList();

    if(m_fitChanged == true)
        AdjustColumnWidth();

    if(m_widthChanged == true)
        RepositionColumnList();

    m_visibleChanged = false;
    m_fitChanged = false;
    m_widthChanged = false;


    bool visible = false;
    if(GetUnfrozenColumnCount() != 0)
    {
        visible = m_visibleRight > m_pGridCore->GetDisplayRect().right ? true : false;
    }
    m_pGridCore->GetHorzScroll()->SetVisible(visible);
}

void GrColumnList::Reserve(uint reserve)
{
    m_vecColumns.reserve(reserve);
    m_vecVisibleColumns.reserve(reserve);
    m_vecDisplayableColumns.reserve(reserve);
    m_vecColumnsRemoved.reserve(reserve);
}

void GrColumnList::AddColumn(GrColumn* pColumn)
{
    InsertColumn(pColumn, m_vecColumns.size());
}

void GrColumnList::InsertColumn(GrColumn* pColumn, uint index)
{
    if(pColumn->GetIndex() != INVALID_INDEX)
        throw _Exception("이미 등록되어 있습니다");

    if(pColumn->GetColumnID() == INVALID_INDEX)
    {
        pColumn->SetColumnID(m_columnID++);
    }

    _Columns::iterator itor = find(m_vecColumnsRemoved.begin(), m_vecColumnsRemoved.end(), pColumn);
    if(itor != m_vecColumnsRemoved.end())
        m_vecColumnsRemoved.erase(itor);

    itor = m_vecColumns.insert(m_vecColumns.begin() + index, pColumn);
    index = std::min(index, m_vecColumns.size());
    for( ; itor != m_vecColumns.end() ; itor++)
    {
        (*itor)->SetIndex(index++);
    }

    m_pGridCore->AttachObject(pColumn);

    if(m_vecVisibleColumns.size() == 0)
        pColumn->SetPriority(m_vecColumns.size());

    GrColumnEventArgs e(pColumn);
    OnColumnInserted(&e);

    pColumn->GroupChanged.Add(this, &GrColumnList::column_GroupChanged);
    assert(m_pGridCore->IsInvalidated() == true);
}

void GrColumnList::RemoveColumn(GrColumn* pColumn)
{
    _Columns::iterator itor = std::find(m_vecColumns.begin(), m_vecColumns.end(), pColumn);

    uint index = pColumn->GetIndex();
    for(_Columns::iterator next = itor + 1 ; next != m_vecColumns.end() ; next++)
    {
        (*next)->SetIndex(index);
        index++;
    }

    m_pGridCore->DetachObject(pColumn);

    pColumn->SetIndex(INVALID_INDEX);

    m_vecColumns.erase(itor);
    m_vecColumnsRemoved.push_back(pColumn);

    pColumn->GroupChanged.Remove(this, &GrColumnList::column_GroupChanged);
    GrColumnEventArgs e(pColumn);
    OnColumnRemoved(&e);

    assert(m_pGridCore->IsInvalidated() == true);
}

GrRect GrColumnList::GetBounds() const
{
    return GrRect(GetX(), GetY(), m_displayableRight, GetY() + GetHeight());
}

uint GrColumnList::GetColumnCount() const
{
    return m_vecColumns.size();
}

GrColumn* GrColumnList::HitTest(int x) const
{
    for_each(_Columns, m_vecDisplayableColumns, value)
    {
        if(x >= value->GetX() && x < value->GetRight())
            return value;
    }
    return NULL;
}

GrIndexRange GrColumnList::HitTest(int x, GrColumn* pColumnAnchor) const
{
    GrIndexRange indexRange;
    const uint anchorIndex = pColumnAnchor->GetVisibleIndex();

    if(pColumnAnchor->GetFrozen() == true)
    {
        if(x < GetRight())
        {
            indexRange.SetRange(0, anchorIndex + 1);
        }
        else if(x >= m_frozenRight)
        {
            indexRange.SetRange(anchorIndex, GetFrozenColumnCount());
        }
        else
        {
            GrColumn* pColumn = HitTest(x);
            uint columnIndex = pColumn->GetVisibleIndex();
            uint minIndex = std::min(columnIndex, anchorIndex);
            uint maxIndex = std::max(columnIndex, anchorIndex);
            indexRange.SetRange(minIndex, maxIndex + 1);
        }
    }
    else
    {
        if(x < m_unfrozenX)
        {
            uint index = m_clippedIndex - 1;
            if(index >= GetUnfrozenColumnCount())
                index = 0;

            GrColumn* pColumn = GetUnfrozenColumn(index);
            indexRange.SetRange(pColumn->GetVisibleIndex(), anchorIndex + 1);
        }
        else if(x >= m_displayableRight)
        {
            uint displayableCount = GetDisplayableColumnCount();
            GrColumn* pColumn = GetDisplayableColumn(displayableCount - 1);
            indexRange.SetRange(anchorIndex, pColumn->GetVisibleIndex() + 1);
        }
        else
        {
            GrColumn* pColumn = HitTest(x);
            uint columnIndex = pColumn->GetVisibleIndex();
            uint minIndex = std::min(columnIndex, anchorIndex);
            uint maxIndex = std::max(columnIndex, anchorIndex);
            indexRange.SetRange(minIndex, maxIndex + 1);
        }
    }

    return indexRange;
}

void GrColumnList::BringIntoView(GrColumn* pColumn)
{
    if(pColumn->ShouldBringIntoView() == false || pColumn->GetFrozen() == true)
        return;

    GrScroll* pHorzScroll = m_pGridCore->GetHorzScroll();
    uint visibleIndex = pColumn->GetUnfrozenIndex();

    uint newValue;
    if(visibleIndex < (uint)pHorzScroll->GetValue())
    {
        newValue = visibleIndex;
    }
    else
    {
        newValue = ClipTo(visibleIndex);
    }

    pHorzScroll->SetValue(newValue);
    m_pGridCore->Invalidate();
}

void GrColumnList::SetFitChanged()
{
    if(m_fitChanged == true)
        return;
    m_fitChanged = true;
    m_pGridCore->Invalidate();
}

void GrColumnList::SetVisibleChanged()
{
    if(m_visibleChanged == true)
        return;
    m_visibleChanged = true;
    m_pGridCore->Invalidate();
}

void GrColumnList::SetWidthChanged()
{
    if(m_widthChanged == true)
        return;
    m_widthChanged = true;
    m_pGridCore->Invalidate();
}

GrColumn* GrColumnList::GetFirstSortColumn() const
{
    return m_pSortColumn;
}

bool GrColumnList::ShouldClip(const GrRect& displayRect, uint horizontal, uint /*vertical*/) const
{
    if(m_clippedIndex == horizontal && displayRect.GetWidth() == m_clippedWidth)
        return false;
    return true;
}

void GrColumnList::Clip(const GrRect& displayRect, uint horizontal, uint /*vertical*/)
{
    int x = GetX() + GetWidth() + m_pGridCore->GetGroupMargin();

    for_each(_Columns, m_vecDisplayableColumns, value)
    {
        value->SetDisplayable(false);
    }
    m_vecDisplayableColumns.clear();

    for(uint i=0 ; i<m_frozenCount ; i++)
    {
        GrColumn* pColumn = GetVisibleColumn(i);
        pColumn->SetX(x);
        pColumn->SetDisplayable(true);
        pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());
        pColumn->SetClipped(false);

        x += pColumn->GetWidth();
        m_vecDisplayableColumns.push_back(pColumn);
    }

    m_pColumnSplitter->SetX(x);

    if(m_pColumnSplitter->GetVisible() == true)
        x += m_pColumnSplitter->GetWidth();

    for(uint i=m_frozenCount+horizontal ; i<GetVisibleColumnCount() ; i++)
    {
        GrColumn* pColumn = GetVisibleColumn(i);

        if(x >= displayRect.right)
            break;

        pColumn->SetX(x);
        pColumn->SetDisplayable(true);
        pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());

        int width = pColumn->GetWidth();

        if(x + width >= displayRect.right)
        {
            pColumn->SetClipped(true);
            x = displayRect.right;
        }
        else
        {
            pColumn->SetClipped(false);
            x += width;
        }
        m_vecDisplayableColumns.push_back(pColumn);

    }

    m_displayableRight = x;

    if(m_clippedWidth != displayRect.GetWidth() || m_clippedIndex != horizontal)
        UpdateHorzScroll(displayRect);

    m_clippedIndex = horizontal;
    m_clippedWidth = displayRect.GetWidth();
}

void GrColumnList::UpdateHorzScroll(const GrRect& displayRect)
{
    GrScroll* pHorzScroll = m_pGridCore->GetHorzScroll();
    if(pHorzScroll->GetVisible() == false)
        return;

    int visibleWidth = m_visibleRight - m_unfrozenX;
    int displayableWidth = displayRect.right - m_unfrozenX;

    uint unfrozenCount = GetUnfrozenColumnCount();

    int countPerPage = 0;
    int tempWidth = displayableWidth;
    for(uint i=unfrozenCount-1 ; i<unfrozenCount ; i--)
    {
        const GrColumn* pColumn = GetUnfrozenColumn(i);
        int columnWidth = pColumn->GetWidth();
        if(columnWidth < tempWidth)
        {
            countPerPage++;
            tempWidth -= columnWidth;
        }
        else
        {
            break;
        }
    }
    countPerPage = std::max(1, countPerPage);

    int largeChange = (int)((float)unfrozenCount * (float)displayableWidth / (float)visibleWidth);
    largeChange = std::max(1, largeChange);

    pHorzScroll->SetMinimum(0);
    pHorzScroll->SetMaximum(unfrozenCount - countPerPage + largeChange - 1);
    pHorzScroll->SetLargeChange(largeChange);
}

uint GrColumnList::ClipFrom(uint visibleFrom) const
{
    return ClipFrom(m_pGridCore->GetDisplayRect(), visibleFrom);
}

uint GrColumnList::ClipFrom(const GrRect& displayRect, uint visibleFrom) const
{
    int displayWidth = displayRect.GetWidth() - m_unfrozenX;
    int displayX = displayWidth;
    uint visibleTo = visibleFrom;

    for(uint i=visibleFrom ; i<GetUnfrozenColumnCount() ; i++)
    {
        GrColumn* pColumn = GetUnfrozenColumn(i);
        displayX -= pColumn->GetWidth();
        if(displayX <= 0)
            break;
        visibleTo = i;
    }
    return visibleTo;
}

uint GrColumnList::ClipTo(uint visibleTo) const
{
    return ClipTo(m_pGridCore->GetDisplayRect(), visibleTo);
}

uint GrColumnList::ClipTo(const GrRect& displayRect, uint visibleTo) const
{
    int displayWidth = displayRect.GetWidth() - m_unfrozenX;
    int displayX = displayWidth;
    uint visibleFrom = visibleTo;

    for(uint i=visibleTo ; i<GetUnfrozenColumnCount() ; i--)
    {
        GrColumn* pColumn = GetUnfrozenColumn(i);
        displayX -= pColumn->GetWidth();
        if(displayX <= 0)
            break;
        visibleFrom = i;
    }
    return visibleFrom;
}

bool GrColumnList::MoveToFrozen(GrColumn* pColumn, GrColumn* pWhere)
{
    if(pWhere && pWhere->GetFrozen() == false)
        return false;

    if(pColumn == pWhere)
        return false;

    _Columns vecFrozens, vecUnfrozens;
    vecFrozens.reserve(GetColumnCount());
    vecUnfrozens.reserve(GetColumnCount());

    for_each(_Columns, m_vecColumns, value)
    {
        if(value == pColumn)
            continue;
        if(value->GetFrozen() == true)
            vecFrozens.push_back(value);
        else
            vecUnfrozens.push_back(value);
    }

    std::sort(vecFrozens.begin(), vecFrozens.end(), SortColumn());
    std::sort(vecUnfrozens.begin(), vecUnfrozens.end(), SortColumn());

    _Columns::iterator itorWhere = std::find(vecFrozens.begin(), vecFrozens.end(), pWhere);
    vecFrozens.insert(itorWhere, pColumn);
    pColumn->m_frozen = true;

    int priority = 0;
    for_each(_Columns, vecFrozens, value)
    {
        value->SetPriority(priority);
        priority++;
    }

    for_each(_Columns, vecUnfrozens, value)
    {
        value->SetPriority(priority);
        priority++;
    }

    SetVisibleChanged();

    return true;
}

bool GrColumnList::MoveToUnfrozen(GrColumn* pColumn, GrColumn* pWhere)
{
    if(pWhere && pWhere->GetFrozen() == true)
        return false;

    if(pColumn == pWhere)
        return false;

    _Columns vecFrozens, vecUnfrozens;
    vecFrozens.reserve(GetColumnCount());
    vecUnfrozens.reserve(GetColumnCount());

    for_each(_Columns, m_vecColumns, value)
    {
        if(value == pColumn)
            continue;
        if(value->GetFrozen() == true)
            vecFrozens.push_back(value);
        else
            vecUnfrozens.push_back(value);
    }

    std::sort(vecFrozens.begin(), vecFrozens.end(), SortColumn());
    std::sort(vecUnfrozens.begin(), vecUnfrozens.end(), SortColumn());

    _Columns::iterator itorWhere = std::find(vecUnfrozens.begin(), vecUnfrozens.end(), pWhere);
    vecUnfrozens.insert(itorWhere, pColumn);
    pColumn->m_frozen = false;

    int priority = 0;
    for_each(_Columns, vecFrozens, value)
    {
        value->SetPriority(priority);
        priority++;
    }

    for_each(_Columns, vecUnfrozens, value)
    {
        value->SetPriority(priority);
        priority++;
    }

    SetVisibleChanged();

    return true;
}

void GrColumnList::column_GroupChanged(GrObject* pSender, GrEventArgs* /*e*/)
{
    GrColumnEventArgs ce((GrColumn*)pSender);
    OnColumnGroupChanged(&ce);
}

void GrColumnList::groupPanel_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrGroupPanel* pGroupList = m_pGridCore->GetGroupPanel();
    m_groupCount = pGroupList->GetGroupCount();
    m_widthChanged = true;
    m_clippedIndex = INVALID_INDEX;

    assert(m_pGridCore->IsInvalidated() == true);
}

void GrColumnList::gridCore_FocusChanged(GrObject* /*pSender*/, GrFocusChangeArgs* /*e*/)
{

}

void GrColumnList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_groupCount = 0;
    m_pSortColumn = NULL;
    m_frozenCount = 0;
    m_columnID = 0;

    if(m_pColumnSplitter->GetVisible() == true)
        m_displayableRight = m_pColumnSplitter->GetRight();
    else
        m_displayableRight = GetRight();

    for_each(_Columns, m_vecColumns, value)
    {
        delete value;
    }

    for_each(_Columns, m_vecColumnsRemoved, value)
    {
        value->SetColumnID(INVALID_INDEX);
    }

    m_vecColumns.clear();
    m_vecVisibleColumns.clear();
    m_vecDisplayableColumns.clear();
}

void GrColumnList::gridCore_FontChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrFont* pFont = m_pGridCore->GetGridWindow()->GetFont();
    int height = (int)(pFont->GetHeight() * 1.25f) + GetPadding(true).GetVertical();
    SetHeight(height);
}

void GrColumnList::Invoke(std::wstring eventName, GrEventArgs* e)
{
    if(eventName.compare(L"ColumnSortTypeChanged") == 0)
        OnColumnSortTypeChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnFrozenChanged") == 0)
        OnColumnFrozenChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnWidthChanged") == 0)
        OnColumnWidthChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnWordwrapChanged") == 0)
        OnColumnWordwrapChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnMultilineChanged") == 0)
        OnColumnMultilineChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnHorzAlignChanged") == 0)
        OnColumnHorzAlignChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnVertAlignChanged") == 0)
        OnColumnVertAlignChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnPaddingChanged") == 0)
        OnColumnPaddingChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnGroupChanged") == 0)
        OnColumnGroupChanged((GrColumnEventArgs*)e);
    else if(eventName.compare(L"ColumnMouseMove") == 0)
        OnColumnMouseMove((GrColumnMouseEventArgs*)e);
    else if(eventName.compare(L"ColumnMouseEnter") == 0)
        OnColumnMouseEnter((GrColumnMouseEventArgs*)e);
    else if(eventName.compare(L"ColumnMouseLeave") == 0)
        OnColumnMouseLeave((GrColumnMouseEventArgs*)e);
    else if(eventName.compare(L"ColumnMouseDown") == 0)
        OnColumnMouseDown((GrColumnMouseEventArgs*)e);
    else if(eventName.compare(L"ColumnMouseUp") == 0)
        OnColumnMouseUp((GrColumnMouseEventArgs*)e);
    else
        throw _Exception("Not implemented event");
}

GrCell* GrColumnList::HitTest(const GrPoint& location) const
{
    GrCell* pHitted = GrUpdatableRow::HitTest(location);
    if(pHitted == NULL)
        return NULL;

    for_each(_Columns, m_vecDisplayableColumns, value)
    {
        int x = value->GetX();
        if(location.x >= x && location.x < x + value->GetWidth())
            return value;
    }

    GrRect bound = m_pColumnSplitter->GetRect();
    if(bound.Contains(location) == true)
        return m_pColumnSplitter;

    if(ContainsHorz(location.x) == false)
        return NULL;

    return pHitted;
}

void GrColumnList::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    GrFlag paintStyle = ToPaintStyle();

    pPainter->DrawColumn(paintStyle, paintRect, GrColor::White);

    if(m_groupCount > 0)
    {
        paintRect.left = paintRect.right;
        paintRect.right = CellStart();
        pPainter->DrawColumn(0, paintRect, GrColor::White);
    }

    for(uint j=0 ; j<m_vecDisplayableColumns.size() ; j++)
    {
        const GrColumn* pColumn = m_vecDisplayableColumns[j];
        if(pColumn->GetX() > clipRect.right || pColumn->GetRight() < clipRect.left)
            continue;
        pColumn->Paint(pPainter, clipRect);
    }

    PaintSplitter(pPainter, clipRect);
}

void GrColumnList::PaintSplitter(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    if(m_pColumnSplitter->GetVisible() == true)
        m_pColumnSplitter->Paint(pPainter, clipRect);
}

GrColumn* GrColumnList::GetColumn(uint index) const
{
    assert(index < m_vecColumns.size());
    return m_vecColumns[index];
}

GrCell::GrCell() : m_padding(GrPadding::Default)
{
    m_pFont = NULL;
    m_id = m_snID++;
    m_pTextUpdater = NULL;

    m_textBoundsChanged = false;
    m_textAlignChanged = false;
    m_textClipped = false;
    m_textVisible = true;

    Tag = NULL;
}

GrCell::~GrCell()
{
#ifdef _MANAGED
    ManagedRef = nullptr;
#endif
}

void GrCell::SetPadding(const GrPadding& padding)
{
    m_padding = padding;
    SetTextBoundsChanged();
}

GrRect GrCell::GetRect() const
{
    GrRect rect;
    rect.left = GetX();
    rect.top = GetY();
    rect.right = rect.left + GetWidth();
    rect.bottom = rect.top + GetHeight();
    return rect;
}

GrRect GrCell::GetClientRect() const
{
    GrRect clientRect;
    GrPadding padding = GetPadding();
    clientRect.left = padding.left;
    clientRect.top = padding.top;
    clientRect.right = GetWidth() - (padding.right);
    clientRect.bottom = GetHeight() - (padding.bottom);

    return clientRect;
}

GrPoint GrCell::GetLocation() const
{
    return GrPoint(GetX(), GetY());
}

GrSize GrCell::GetSize() const
{
    return GrSize(GetWidth(), GetHeight());
}

uint GrCell::GetTextLineCount() const
{
    return m_vecTextLine.size();
}

const GrLineDesc& GrCell::GetTextLine(uint index) const
{
    return m_vecTextLine[index];
}

const GrRect& GrCell::GetTextBounds() const
{
    return m_textBounds;
}

void GrCell::AlignText()
{
    GrFont* pFont = GetPaintingFont();
    GrPoint startLocation;
    const GrPadding padding = GetPadding();
    const int width = GetWidth() - (padding.left + padding.right);
    const int height = GetHeight() - (padding.top + padding.bottom);

    int lineHeight = pFont->GetHeight() + pFont->GetExternalLeading();

    for(uint i=0 ; i<m_vecTextLine.size() ; i++)
    {
        GrLineDesc& cl = m_vecTextLine[i];

        switch(GetTextHorzAlign())
        {
        case GrHorzAlign_Left:
            startLocation.x = 0;
            break;
        case GrHorzAlign_Center:
            startLocation.x = (width - cl.width) / 2;
            break;
        case GrHorzAlign_Right:
            startLocation.x = width - cl.width;
            break;
        default:
            break;
        }

        switch(GetTextVertAlign())
        {
        case GrVertAlign_Top:
            startLocation.y = i * lineHeight;
            break;
        case GrVertAlign_Center:
            startLocation.y = (height - GetTextLineCount() * lineHeight) / 2 + i * lineHeight;
            break;
        case GrVertAlign_Bottom:
            startLocation.y = height - ((i+1) * lineHeight);
            break;
        default:
            break;
        }

        if(height < lineHeight)
            startLocation.y = i * lineHeight;

        cl.x = startLocation.x + padding.left;
        cl.y = startLocation.y + padding.top;

        if(i==0)
        {
            m_textBounds.SetLocation(cl.x, cl.y);
        }
    }
}

void GrCell::ComputeTextBounds()
{
    GrFont* pFont = GetPaintingFont();
    GrPadding padding = GetPadding();
    m_vecTextLine.clear();

    int maxWidth = 0;
    int maxHeight = 0;
    GrRect oldTextBounds = m_textBounds;

    if(m_text.length() > 0)
    {
        int cellWidth = GetWidth() - (padding.left + padding.right);

        if(GetTextMulitiline() == false)
        {
            GrLineDesc textLine;
            GrTextUtil::SingleLine(&textLine, m_text, pFont);
            m_vecTextLine.push_back(textLine);
        }
        else
        {
            GrTextUtil::MultiLine(&m_vecTextLine, m_text, cellWidth, pFont, GetTextWordWrap());
        }

        for_each(_TextLines, m_vecTextLine, value)
        {
            const GrLineDesc& lineDesc = value;
            maxWidth = std::max(maxWidth, lineDesc.width);
            maxHeight += pFont->GetHeight() + pFont->GetExternalLeading();
        }
    }
    else
    {
        maxHeight = pFont->GetHeight() + pFont->GetExternalLeading();
    }

    //maxHeight += pFont->GetLeading();

    if(maxWidth + (padding.left + padding.right) > GetWidth() || 
        maxHeight + padding.top + padding.bottom > GetHeight())
        m_textClipped = true;
    else
        m_textClipped = false;

    m_textBounds.SetSize(GrSize(maxWidth, maxHeight));
    if(m_textBounds != oldTextBounds)
        OnTextSizeChanged();
}

bool GrCell::GetTextVisible() const
{
    return m_textVisible;
}

void GrCell::SetTextVisible(bool b)
{
    m_textVisible = b;
}

bool GrCell::GetTextClipped() const
{
    return m_textClipped;
}

void GrCell::DrawText(GrGridPainter* pPainter, GrColor foreColor, const GrRect& paintRect, const GrRect* pClipRect) const
{
    if(GetTextVisible() == false)
        return;

    GrFont* pFont = GetPaintingFont();
    GrRect textRect;
    for(uint i=0 ; i<GetTextLineCount() ; i++)
    {
        const GrLineDesc& cl = GetTextLine(i);
        textRect.left = paintRect.left + cl.x;
        textRect.top = paintRect.top + cl.y;
        textRect.right = paintRect.left + cl.x + cl.width;
        textRect.bottom = paintRect.top + cl.y + pFont->GetHeight();
        if(textRect.top > paintRect.bottom || textRect.bottom <= paintRect.top)
            continue;

        if(GetTextClipped() == true || pClipRect != NULL)
        {
            GrRect clipRect = paintRect;
            clipRect.Contract(GetPadding());

            if(pClipRect != NULL)
            {
                clipRect.right = std::min(clipRect.right, pClipRect->right);
                clipRect.bottom = std::min(clipRect.bottom, pClipRect->bottom);
            }

            pPainter->DrawText(pFont, GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor, &clipRect);
            //pPainter->DrawRectangle(textRect, GrColor::Black);
        }
        else
        {
            pPainter->DrawText(pFont, GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor);
            //pPainter->DrawRectangle(textRect, GrColor::Black);
        }
    }
}

void GrCell::OnTextSizeChanged()
{

}

void GrCell::OnGridCoreAttached()
{
    GrObject::OnGridCoreAttached();
    m_pTextUpdater = m_pGridCore->GetTextUpdater();
    m_pTextUpdater->AddTextBounds(this);
    m_pTextUpdater->AddTextAlign(this);
}

void GrCell::OnGridCoreDetached()
{
    if(m_textAlignChanged == true)
        m_pTextUpdater->RemoveTextAlign(this);
    if(m_textBoundsChanged == true)
        m_pTextUpdater->RemoveTextBounds(this);
    m_pTextUpdater = NULL;
    GrObject::OnGridCoreDetached();
}

bool GrCell::Contains(const GrPoint& point) const
{
    return ContainsHorz(point.x) && ContainsVert(point.y);
}

bool GrCell::ContainsHorz(int x) const
{
    if(x < GetX() || x >= GetRight())
        return false;
    return true;
}
bool GrCell::ContainsVert(int y) const
{
    if(y < GetY() || y >= GetBottom())
        return false;
    return true;
}

const std::wstring& GrCell::GetText() const
{
    return m_text;
}

void GrCell::SetText(const std::wstring& text)
{
    if(m_text.compare(text) == 0)
        return;
    m_text = text;
    OnTextChanged();
}

void GrCell::SetTextBoundsChanged()
{
    if(m_pTextUpdater != NULL)
    {
        m_pTextUpdater->AddTextBounds(this);
        if(GetDisplayable() == true)
        {
            Invalidate();
        }
    }
}

void GrCell::SetTextAlignChanged()
{
    if(m_pTextUpdater != NULL)
    {
        m_pTextUpdater->AddTextAlign(this);
        if(GetDisplayable() == true)
        {
            Invalidate();
        }
    }
}

void GrCell::Invalidate(bool thisOnly)
{
    if(m_pGridCore == NULL || GetDisplayable() == false)
        return;

    if(thisOnly == true)
        m_pGridCore->Invalidate(GetRect());
    else
        m_pGridCore->Invalidate();
}

void GrCell::OnTextChanged()
{
    SetTextBoundsChanged();

}

GrHorzAlign GrCell::GetTextHorzAlign() const
{
    return GrHorzAlign_Left;
}

GrVertAlign GrCell::GetTextVertAlign() const
{
    return GrVertAlign_Top;
}

bool GrCell::GetTextWordWrap() const
{
    return false;
}

GrFont* GrCell::GetPaintingFont() const
{
    if(m_pFont == NULL)
    {
        return m_pGridCore->GetGridWindow()->GetFont();
    }
    return m_pFont;
}

GrColor GrCell::GetPaintingForeColor() const
{
    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(m_foreColor == GrColor::Empty)
        return pStyle->GetCellForeColor();
    return m_foreColor;
}

GrColor GrCell::GetPaintingBackColor() const
{
    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(m_backColor == GrColor::Empty)
        return pStyle->GetCellBackColor();
    return m_backColor;
}

GrColor GrCell::GetForeColor(bool /*inherited*/) const
{
    return m_foreColor;
}

GrColor GrCell::GetBackColor(bool /*inherited*/) const
{
    return m_backColor;
}

GrPadding GrCell::GetPadding(bool /*inherited*/) const
{
    return m_padding;
}

GrFont* GrCell::GetFont(bool /*inherited*/) const
{
    return m_pFont;
} 

GrFlag GrCell::ToPaintStyle() const
{
    GrFlag flag;
    if(GetMouseOvered() == true)
        flag += GrPaintStyle_Mouseover;
    if(GetMousePressed() == true)
        flag += GrPaintStyle_Pressed;
    return flag;
}

void GrCell::SetFont(GrFont* pFont)
{
    if(m_pFont == pFont)
        return;
    m_pFont = pFont;
    SetTextBoundsChanged();
}

uint GrCell::GetID() const
{
    return m_id;
}

void GrCell::SetBackColor(const GrColor& color)
{
    m_backColor = color;
    Invalidate();
}

void GrCell::SetForeColor(const GrColor& color)
{ 
    m_foreColor = color;
    Invalidate();
}

int GrCell::HitMouseOverTest(const GrPoint& localLocation) const
{
    if(localLocation.x < 0 || localLocation.y < 0 || 
        localLocation.x >= GetWidth() || localLocation.y >= GetHeight())
        return 0;
    return 1;
}

bool GrCell::GetMouseOvered() const
{
    if(m_pGridCore == NULL)
        return false;
    return m_pGridCore->GetMouseOver() == this;
}

int GrCell::GetMouseOverState() const
{
    if(m_pGridCore == NULL)
        return 0;
    return m_pGridCore->GetMouseOverState();
}

bool GrCell::GetMousePressed() const
{
    if(m_pGridCore == NULL)
        return false;
    return m_pGridCore->GetMousePress() == this;
}

GrItem::GrItem(GrColumn* pColumn, GrDataRow* pDataRow)
{
    m_pColumn = pColumn;
    m_pDataRow = pDataRow;
    m_readOnly = false;
    m_selected = false;
    m_colorLocked= false;
}

int GrItem::HitMouseOverTest(const GrPoint& localLocation) const
{
    if(GrCell::HitMouseOverTest(localLocation) == 0)
        return 0;

    if(GetReadOnly() == true)
        return GrMouseOverState_In;

    if(m_pColumn->GetItemType() == GrItemType_Control)
        return GrMouseOverState_In;

    GrRect controlRect = GetControlRect();
    if(controlRect.Contains(localLocation) == true)
    {
        switch(m_pColumn->GetItemTypeShow())
        {
        case GrItemTypeShow_FocusedOnly:
            {
                if(GetFocused() == true)
                    return GrMouseOverState_Control;
            }
            break;
        case GrItemTypeShow_SelectedOnly:
            {
                if(GetSelected() == true)
                    return GrMouseOverState_Control;
            }
            break;
        case GrItemTypeShow_Always:
            return GrMouseOverState_Control;
            break;
        default:
            break;
        }
    }

    return GrMouseOverState_In;
}

GrRect GrItem::GetControlRect() const
{
    GrRect rect;
    rect.left = GetWidth() - DEF_CONTROL_WIDTH;
    rect.top = 0;
    rect.right = GetWidth() - 1;
    rect.bottom = GetHeight() - 1;
    return rect;
}

bool GrItem::GetControlVisible() const
{
    if(m_pColumn->GetItemType() == GrItemType_Control)
        return false;

    bool showControl = false;

    switch(m_pColumn->GetItemTypeShow())
    {
    case GrItemTypeShow_SelectedOnly:
        if(IsItemSelecting() == true || GetSelected() == true)
            showControl = true;
        break;
    case GrItemTypeShow_FocusedOnly:
        if(GetFocused() == true)
            showControl = true;
        break;
    case GrItemTypeShow_Always:
        showControl = true;
        break;
    default:
        break;
    }

    if(GetReadOnly() == true)
        showControl = false;

    return showControl;
}

GrColumn* GrItem::GetColumn() const
{
    return m_pColumn; 
}

GrDataRow* GrItem::GetDataRow() const
{ 
    return m_pDataRow; 
}

GrRow* GrItem::GetRow() const
{
    return GetDataRow();
}

bool GrItem::GetVisible() const
{
    return m_pColumn->GetVisible(); 
}

bool GrItem::GetReadOnly() const
{
    return (m_pColumn->GetReadOnly() == true || m_pDataRow->GetReadOnly() == true || m_readOnly == true);
}

void GrItem::SetReadOnly(bool b)
{
    m_readOnly = b;
}

void GrItem::SetSelected(bool b)
{
    if(m_selected == b)
        return;
    GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
    if(b == true)
        pItemSelector->SelectItem(this, GrSelectionType_Add);
    else
        pItemSelector->SelectItem(this, GrSelectionType_Remove);
}

bool GrItem::GetSelected() const
{
    return m_selected;
}

void GrItem::SetFocused(bool b)
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(b == true)
        pFocuser->Set(this);
    else
        pFocuser->Set(IFocusable::Null);
}

const std::wstring& GrItem::GetErrorDescription() const
{
    return m_errorDescription;
}

void GrItem::SetErrorDescription(const std::wstring& errorDescription)
{
    if(m_errorDescription.compare(errorDescription) == 0)
        return;
    m_errorDescription = errorDescription;
}

void GrItem::LockColor(bool b)
{
    if(m_colorLocked == b)
        return;
    m_colorLocked = b;
    Invalidate();
}

bool GrItem::GetFocused() const
{
    const GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    return pFocuser->Get() == this;
}

bool GrItem::GetClipped() const
{
    if(m_pColumn->GetClipped() == true || m_pDataRow->GetClipped() == true)
        return true;
    return false;
}

GrHorzAlign GrItem::GetTextHorzAlign() const
{
    return m_pColumn->GetItemHorzAlign();
}

GrVertAlign GrItem::GetTextVertAlign() const
{
    return m_pColumn->GetItemVertAlign();
}

bool GrItem::GetTextWordWrap() const
{
    return m_pColumn->GetItemWordWrap();
}

bool GrItem::GetTextMulitiline() const
{
    return m_pColumn->GetItemMultiline();
}

int GrItem::GetX() const
{
    return m_pColumn->GetX();
}

int GrItem::GetY() const
{
    return m_pDataRow->GetY();
}

int GrItem::GetWidth() const
{
    return m_pColumn->GetWidth();
}

int GrItem::GetHeight() const
{
    return m_pDataRow->GetHeight();
}

bool GrItem::ShouldBringIntoView() const
{
    return (m_pColumn->ShouldBringIntoView() || m_pDataRow->ShouldBringIntoView());
}

int GrItem::GetMinHeight() const
{
    int itemHeight = std::max(GetTextBounds().GetHeight(), m_pColumn->GetItemMinHeight());
    itemHeight += GetPadding().GetVertical();
    return itemHeight;
}

bool GrItem::GetDisplayable() const
{
    return (m_pColumn->GetDisplayable() && m_pDataRow->GetDisplayable());
}

void GrItem::BringIntoView()
{
    m_pGridCore->BringIntoView(this);
}

bool GrItem::IsItemSelecting() const
{
    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    if(pItemSelector->IsSelecting(m_pColumn) == false)
        return false;
    if(pItemSelector->IsSelecting(m_pDataRow) == false)
        return false;
    return true;
}

GrFlag GrItem::ToPaintStyle() const
{
    GrFlag flag = GrCell::ToPaintStyle();
    if(m_pGridCore->GetSelectionVisible() == false)
        return flag;

    if(IsItemSelecting() == true || GetSelected() == true)
    {
        flag += GrPaintStyle_Selected;
    }

    if(GetFocused() == true)
    {
        flag += GrPaintStyle_Focused;
    }
    return flag;
}

GrFont* GrItem::GetPaintingFont() const
{
    return GetFont();
}

GrColor GrItem::GetPaintingForeColor() const
{
    const GrColor foreColor = GetForeColor();
    const GrStyle* pStyle = m_pGridCore->GetStyle();
    GrColor clr = foreColor;

    if(m_colorLocked == true)
        return foreColor;

    if(GetFocused() == true)
    {
        clr = pStyle->FocusedForeColor;
    }
    else if(GetSelected() == true || IsItemSelecting() == true)
    {
        if(m_pGridCore->GetSelectionVisible() == true)
            clr = pStyle->SelectedForeColor;
    }
    return clr;
}

GrColor GrItem::GetPaintingBackColor() const
{
    const GrColor backColor = GetBackColor();
    const GrStyle* pStyle = m_pGridCore->GetStyle();
    GrColor clr = backColor;

    if(m_colorLocked == true)
        return backColor;

    if(GetFocused() == true)
    {
        clr = pStyle->FocusedBackColor;
    }
    else if(GetSelected() == true || IsItemSelecting() == true)
    {
        if(m_pGridCore->GetSelectionVisible() == true)
            clr = pStyle->SelectedBackColor;
    }
    else if(m_pDataRow->HasFocused() == true && 
        m_pGridCore->GetRowHighlight() == true && 
        m_pGridCore->GetRowHighlightType() != GrRowHighlightType_Line)
    {
        clr = pStyle->RowHighlightBackColor;
    }

    if(GetMouseOvered() == true)
    {
        clr *= 0.9f;
    }

    return clr;
}

GrColor GrItem::GetForeColor(bool inherited) const
{
    GrColor pureColor = GrCell::GetForeColor(false);
    if(inherited == false)
        return pureColor;

    GrColor color = pureColor;
    if(color != GrColor::Empty)
        return color;

    color = m_pDataRow->GetItemForeColor();
    if(color != GrColor::Empty)
        return color;

    color = m_pColumn->GetItemForeColor();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    return pStyle->GetRowItemForeColor(m_pDataRow->GetVisibleDataRowIndex());
}

GrColor GrItem::GetBackColor(bool inherited) const
{
    GrColor pureColor = GrCell::GetBackColor(false);
    if(inherited == false)
        return pureColor;

    GrColor color = pureColor;
    if(color != GrColor::Empty)
        return color;

    color = m_pDataRow->GetItemBackColor();
    if(color != GrColor::Empty)
        return color;

    color = m_pColumn->GetItemBackColor();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    return pStyle->GetRowItemBackColor(m_pDataRow->GetVisibleDataRowIndex());
}

GrPadding GrItem::GetPadding(bool inherited) const
{
    GrPadding purePadding = GrCell::GetPadding(false);
    if(inherited == false)
        return purePadding;

    GrPadding padding = purePadding;
    if(padding != GrPadding::Default)
        return padding;

    padding = m_pColumn->GetItemPadding();
    if(m_pColumn->GetItemIcon() == true)
        padding.left += (DEF_ICON_SIZE + padding.left);

    if(padding != GrPadding::Default)
        return padding;

    return GrPadding::Default;
}

GrFont* GrItem::GetFont(bool inherited) const
{
    GrFont* pPureFont = GrCell::GetFont(false);
    if(inherited == false)
        return pPureFont;

    GrFont* pFont = pPureFont;
    if(pFont != NULL)
        return pFont;

    pFont = m_pDataRow->GetItemFont();
    if(pFont != NULL)
        return pFont;

    pFont = m_pColumn->GetItemFont();
    if(pFont != NULL)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    pFont = pStyle->GetRowItemFont(m_pDataRow->GetVisibleDataRowIndex());
    if(pFont != NULL)
        return pFont;
    return m_pGridCore->GetGridWindow()->GetFont();
}

GrControlState GrItem::GetControlState() const
{
    if(GetMouseOvered() == false)
        return GrControlState_Normal;
    if(m_pGridCore->GetMouseOverState() == GrMouseOverState_Control)
    {
        if(GetMousePressed() == true)
            return GrControlState_Pressed;
        else 
            return GrControlState_Hot;
    }
    return GrControlState_Normal;
}

void GrItem::OnTextSizeChanged()
{
    GrCell::OnTextSizeChanged();

    if(m_pGridCore->GetAutoFitColumn() == true)
    {
        m_pColumn->SetFit();
    }

    if(m_pGridCore->GetAutoFitRow() == true)
    {
        m_pDataRow->SetFit();
    }
}

void GrItem::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();

    if(paintRect.left > clipRect.right || paintRect.right <= clipRect.left)
        return;

    GrFlag paintStyle = ToPaintStyle();

    GrColor backColor = GetPaintingBackColor();
    GrColor foreColor = GetPaintingForeColor();

    if(GetClipped() == true)
        pPainter->DrawItem(paintStyle, paintRect, backColor, &clipRect);
    else
        pPainter->DrawItem(paintStyle, paintRect, backColor);

    if(m_pColumn->m_customItemPaint == true)
    {
        //(*m_pColumn->m_fnCustomPaint)(pPainter, this, paintRect, m_pColumn->m_userData);
        return;
    }

    if(m_pColumn->GetItemTextVisible() == true)
    {
        if(GetClipped() == true)
            DrawText(pPainter, foreColor, paintRect, &clipRect);
        else
            DrawText(pPainter, foreColor, paintRect);
    }

    if(GetControlVisible() == true)
    {
        GrRect buttonRect = GetControlRect() + paintRect.GetLocation();
        switch(m_pColumn->GetItemType())
        {
        case GrItemType_DropDown:
            pPainter->DrawDropDown(buttonRect, GetControlState());
            break;
        case GrItemType_Modal:
            pPainter->DrawModal(buttonRect, GetControlState());
            break;
        default:
            break;
        }
    }
}

GrItem* GrDataRow::GetItem(const GrColumn* pColumn) const
{
    uint columnID = pColumn->GetColumnID();
    assert(columnID < m_vecItems.size());
    return m_vecItems[columnID];
}

GrItem* GrDataRow::GetItem(const GrItem* pOtherItem) const
{
    return GetItem(pOtherItem->GetColumn());
}

bool GrDataRow::GetReadOnly() const
{
    if(m_pGridCore != NULL && m_pGridCore->GetReadOnly() == true)
        return true;
    return m_readOnly;
}

void GrDataRow::SetReadOnly(bool b)
{
    m_readOnly = b;
}

void GrDataRow::SetSelected(bool b)
{
    if(GetSelected() == b)
        return;
    GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
    if(b == true)
        pItemSelector->SelectItems(this, GrSelectionType_Add);
    else
        pItemSelector->SelectItems(this, GrSelectionType_Remove);
}

bool GrDataRow::GetSelected() const
{
    return m_selectedCells > 0;
}

bool GrDataRow::GetSelecting() const
{
    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    return pItemSelector->IsSelecting(this);
}

bool GrDataRow::GetFullSelected() const
{
    uint visibleColumns = m_pGridCore->GetColumnList()->GetVisibleColumnCount();
    if(visibleColumns == 0)
        return false;
    if(m_selectedCells == visibleColumns)
        return true;
    return false;
}

bool GrDataRow::GetVisible() const
{
    bool visible = GrRow::GetVisible();
    if(visible == false)
        return false;
    GrGroupRow* pGroupRow = dynamic_cast<GrGroupRow*>(GetParent());
    if(pGroupRow == NULL)
        return visible;

    return pGroupRow->IsExpanded();
}

void GrDataRow::SetVisibleDataRowIndex(uint index)
{
    m_visibleDataRowIndex = index;
}

void GrDataRow::SetDataRowIndex(uint index)
{
    wchar_t numberText[10];
    if(m_pDataRowList->GetZeroBasedRowIndex() == true)
        swprintf(numberText, 10, L"%d", index);
    else
        swprintf(numberText, 10, L"%d", index + 1);

    m_dataRowIndex = index;
    SetText(numberText);
}

void GrDataRow::SetDataRowID(uint index)
{
    m_dataRowID = index;
}

uint GrDataRow::GetVisibleDataRowIndex() const
{
    if(GetVisible() == false)
        return INVALID_INDEX;
    return m_visibleDataRowIndex;
}

uint GrDataRow::GetDataRowIndex() const
{
    return m_dataRowIndex;
}

uint GrDataRow::GetDataRowID() const
{
    return m_dataRowID;
}

GrColor GrDataRow::GetItemBackColor() const
{
    return m_itemBackColor;
}

GrColor GrDataRow::GetItemForeColor() const
{
    return m_itemForeColor;
}

GrFont* GrDataRow::GetItemFont() const
{
    return m_pItemFont;
}

void GrDataRow::SetItemBackColor(const GrColor& color)
{
    if(m_itemBackColor == color)
        return;
    m_itemBackColor = color;
    InvalidateRow();
}

void GrDataRow::SetItemForeColor(const GrColor& color)
{
    if(m_itemForeColor == color)
        return;
    m_itemForeColor = color;
    InvalidateRow();
}

void GrDataRow::SetItemFont(GrFont* pFont)
{
    if(m_pItemFont == pFont)
        return;
    m_pItemFont = pFont;
    InvalidateRow();
}

IFocusable* GrDataRow::GetFocusable(GrColumn* pColumn) const
{
    return GetItem(pColumn);
}

int GrDataRow::GetMinHeight() const
{
    int height = IDataRow::GetMinHeight();

    for_each(_Items, m_vecItems, value)
    {
        if(value->GetVisible() == false)
            continue;
        height = std::max(height, value->GetMinHeight());
    }
    return height;
}

GrCell* GrDataRow::HitTest(const GrPoint& location) const
{
    if(ContainsVert(location.y) == false)
        return NULL;

    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
    {
        GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
        if(pColumn->ContainsHorz(location.x) == true)
            return GetItem(pColumn);
    }

    GrRect bound = pColumnList->GetColumnSplitter()->GetRect();
    if(bound.Contains(location) == true)
        return pColumnList->GetColumnSplitter();

    if(ContainsHorz(location.x) == false)
        return NULL;

    return const_cast<GrDataRow*>(this);
}

void GrDataRow::OnHeightAdjusted()
{
    IDataRow::OnHeightAdjusted();
    if(m_pTextUpdater == NULL)
        return;
    for_each(_Items, m_vecItems, value)
    {
        m_pTextUpdater->AddTextAlign(value);
    }
    m_pGridCore->Invalidate();
}

GrColumn::GrColumn()
{
    m_pColumnList = NULL;
    m_visible = true;
    m_readOnly = false;
    m_sortable = true;
    m_groupable = true;
    m_movable = true;
    m_resizable = true;
    m_frozen = false;
    m_selected = false;
    m_fullSelected = false;
    m_fitting = false;
    m_displayable = false;
    m_clipped = false;
    m_grouped = false;

    m_visibleIndex = INVALID_INDEX;
    m_displayIndex = INVALID_INDEX;
    m_index = INVALID_INDEX;
    m_columnID = INVALID_INDEX;

    m_x = 0;
    m_width = 100;
    m_minWidth = 30;
    m_maxWidth = 10000;
    m_sortType = GrSort_None;

    m_selectedCells = 0;
    m_priority = GetID();

    m_itemType = GrItemType_Control;
    m_itemTypeShow = GrItemTypeShow_SelectedOnly;
    m_itemHorzAlign = GrHorzAlign_Left;
    m_itemVertAlign = GrVertAlign_Top;
    m_itemWordWrap = false;
    m_itemMultiline = false;
    m_itemBackColor = GrColor::Empty;
    m_itemForeColor = GrColor::Empty;
    m_itemPadding = GrPadding::Default;
    m_pItemFont = NULL;
    m_itemMinHeight = 0;
    m_itemClickEditing = GrClickEditing_Default;
    m_itemTextVisible = true;
    m_itemIcon = false;

    m_comparer[GrSort_None] = 0;
    m_comparer[GrSort_Up] = 0;
    m_comparer[GrSort_Down] = 0;

    m_customItemPaint = false;

    m_fnColumnBackgroundPaint= NULL;
    m_fnColumnContentsPaint = NULL;
    m_pColumnPaintData = NULL;

    m_pGroup = new GrGroup(this);
}

void GrColumn::OnGridCoreAttached()
{
    GrCell::OnGridCoreAttached();
    m_pColumnList = m_pGridCore->GetColumnList();
    m_pGridCore->AttachObject(m_pGroup);
}

void GrColumn::OnGridCoreDetached()
{
    GrCell::OnGridCoreDetached();
    m_pGridCore->DetachObject(m_pGroup);
    m_pColumnList = NULL;
}

void GrColumn::OnTextChanged()
{
    GrCell::OnTextChanged();
    if(m_pGridCore == NULL)
        return;

    if(GetDisplayable() == false)
        return;
    m_pGridCore->Invalidate(GetRect());
}

GrColumn::~GrColumn()
{
    m_visibleIndex = INVALID_INDEX;
    m_displayIndex = INVALID_INDEX;
    m_index = INVALID_INDEX;
    m_columnID = INVALID_INDEX;

    delete m_pGroup;
    m_pGroup = NULL;

#ifdef _MANAGED
    System::Object^ managedRef = this->ManagedRef;
    System::ComponentModel::IComponent^ component = safe_cast<System::ComponentModel::IComponent^>(managedRef);
    if(component != nullptr)
    {
        delete this->ManagedRef;
        this->ManagedRef = nullptr;
    }
#endif
}

void GrColumn::SetSelected(bool b)
{
    GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
    if(b == true)
        pItemSelector->SelectItems(this, GrSelectionType_Add);
    else
        pItemSelector->SelectItems(this, GrSelectionType_Remove);
}

bool GrColumn::GetSelected() const
{
    return m_selectedCells > 0;
}

bool GrColumn::GetFullSelected() const
{
    uint visibleRows = m_pGridCore->GetDataRowList()->GetVisibleDataRowCount();
    if(visibleRows == 0)
        return false;
    if(m_selectedCells == (int)visibleRows)
        return true;
    return false;
}

bool GrColumn::HasFocused() const
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->GetFocusedColumn() == this)
        return true;
    return false;
}

int GrColumn::GetPriority() const
{
    return m_priority;
}

void GrColumn::SetPriority(int priority)
{
    m_priority = priority;
}

bool GrColumn::GetGrouped() const
{
    return m_grouped;
}

void GrColumn::SetGrouped(bool b)
{
    if(m_grouped == b)
        return;

    m_grouped = b;
    GroupChanged(this, &GrEventArgs::Empty);
}

GrGroup* GrColumn::GetGroup() const
{
    return m_pGroup;
}

void GrColumn::SetDisplayable(bool b)
{
    m_displayable = b;
}

bool GrColumn::GetDisplayable() const
{
    if(GetVisible() == false)
        return false;
    return m_displayable;
}

void GrColumn::SetDisplayIndex(uint index)
{
    m_displayIndex = index;
}

uint GrColumn::GetDisplayIndex() const
{
    return m_displayIndex;
}

void GrColumn::SetVisibleIndex(uint index)
{
    m_visibleIndex = index;
}

uint GrColumn::GetVisibleIndex() const
{
    return m_visibleIndex;
}

uint GrColumn::GetFreezableIndex() const
{
    assert(m_frozen == true);
    return m_visibleIndex;
}

uint GrColumn::GetUnfrozenIndex() const
{
    assert(m_frozen == false);
    return m_visibleIndex - m_pGridCore->GetColumnList()->GetFrozenColumnCount();
}

uint GrColumn::GetIndex() const
{
    return m_index;
}

void GrColumn::SetIndex(uint index)
{
    m_index = index;
}

uint GrColumn::GetColumnID() const
{
    return m_columnID;
}

void GrColumn::SetColumnID(uint id)
{
    m_columnID = id;
}

void GrColumn::SetClipped(bool b)
{
    m_clipped = b;
}

GrRow* GrColumn::GetRow() const
{
    return m_pColumnList; 
}

bool GrColumn::GetClipped() const
{
    return m_clipped;
}

bool GrColumn::ShouldBringIntoView() const
{
    if(m_displayable == false || m_clipped == true)
        return true;
    if(m_frozen == true)
        return false;
    return false;
}

void GrColumn::SetSortType(GrSort sortType)
{
    if(m_sortable == false)
        return;
    if(m_pColumnList == NULL)
        return;
    m_sortType = sortType;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnSortTypeChanged", &e);
    }
}

GrSort GrColumn::GetSortType() const
{
    if(m_pColumnList == NULL)
        return GrSort_None;
    if(m_pColumnList->GetFirstSortColumn() != this)
        return GrSort_None;
    return m_sortType;
}

void GrColumn::SetSortComparer(GrSort sortType, FuncComparer comparer)
{
    m_comparer[(int)sortType] = comparer;
}

FuncComparer GrColumn::GetSortComparer(GrSort sortType) const
{
    return m_comparer[(int)sortType];
}

void GrColumn::SetSortable(bool b)
{
    m_sortable = b;
}

bool GrColumn::GetSortable() const
{
    return m_sortable;
}

void GrColumn::SetGroupable(bool b)
{
    m_groupable = b;
}

bool GrColumn::GetGroupable() const
{
    return m_groupable;
}

void GrColumn::SetWidth(int width) 
{ 
    width = std::max(width, m_minWidth);
    width = std::min(width, m_maxWidth);

    if(m_width == width)
        return;

    m_width = width;

    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnWidthChanged", &e);
    }
}

void GrColumn::SetMinWidth(int minWidth)
{
    m_minWidth = minWidth;
}

void GrColumn::SetMaxWidth(int maxWidth)
{
    m_maxWidth = maxWidth;
}

int GrColumn::GetMinWidth() const
{
    return m_minWidth;
}

int GrColumn::GetMaxWidth() const
{
    return m_maxWidth;
}

bool GrColumn::GetMovable() const
{
    return m_movable;
}

void GrColumn::SetMovable(bool b)
{
    m_movable = b;
}

bool GrColumn::GetResizable() const
{
    return m_resizable;
}

void GrColumn::SetResizable(bool b)
{
    m_resizable = b;
}

bool GrColumn::GetFrozen() const
{
    return m_frozen;
}

void GrColumn::SetFrozen(bool b)
{
    if(m_frozen == b)
        return;
    m_frozen = b;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnFrozenChanged", &e);
    }
}

void GrColumn::AdjustWidth()
{
    if(m_fitting == false)
        return;

    const GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

    int width = m_minWidth;
    if(m_pGridCore->GetAutoFitColumnType() == GrAutoFitColumnType_ColumnIncluded)
    {
        int columnWidth = GetTextBounds().GetWidth() + GetPadding().GetHorizontal();
        width = std::max(m_minWidth, columnWidth);
    }

    for(uint i=0 ; i<pDataRowList->GetVisibleDataRowCount() ; i++)
    {
        GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
        GrItem* pItem = pDataRow->GetItem(this);
        int itemWidth = pItem->GetTextBounds().GetWidth() + pItem->GetPadding().GetHorizontal();
        width = std::max(width, itemWidth);
    }

    // insertion Row
    {
        GrDataRow* pDataRow = m_pGridCore->GetInsertionRow();
        GrItem* pItem = pDataRow->GetItem(this);
        int itemWidth = pItem->GetTextBounds().GetWidth() + pItem->GetPadding().GetHorizontal();
        width = std::max(width, itemWidth);
    }

    if(GetItemType() != GrItemType_Control)
        width += DEF_CONTROL_WIDTH;

    if(m_width != width)
        m_width = width;
    m_fitting = false;
}

void GrColumn::SetFit()
{
    if(m_fitting == true)
        return;
    m_fitting = true;
    m_pColumnList->SetFitChanged();
}

int GrColumn::GetX() const 
{ 
    return m_x; 
}

int GrColumn::GetY() const
{ 
    return m_pColumnList->GetY(); 
}

int GrColumn::GetWidth() const
{
    return m_width;
}

int GrColumn::GetHeight() const
{
    return m_pColumnList->GetHeight();
}

bool GrColumn::GetVisible() const
{
    return m_visible;
}

bool GrColumn::GetReadOnly() const
{
    if(m_pGridCore != NULL && m_pGridCore->GetReadOnly() == true)
        return true;
    return m_readOnly;
}

GrHorzAlign GrColumn::GetTextHorzAlign() const
{
    return GrHorzAlign_Center;
}

GrVertAlign GrColumn::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

GrItemType GrColumn::GetItemType() const
{
    return m_itemType;
}

GrItemTypeShow GrColumn::GetItemTypeShow() const
{
    return m_itemTypeShow;
}

GrHorzAlign GrColumn::GetItemHorzAlign() const
{
    return m_itemHorzAlign;
}

GrVertAlign GrColumn::GetItemVertAlign() const
{
    return m_itemVertAlign;
}

bool GrColumn::GetItemWordWrap() const
{
    return m_itemWordWrap;
}

bool GrColumn::GetItemMultiline() const
{
    return m_itemMultiline;
}

const GrColor& GrColumn::GetItemForeColor() const
{
    return m_itemForeColor;
}

const GrColor& GrColumn::GetItemBackColor() const
{
    return m_itemBackColor;
}

const GrPadding& GrColumn::GetItemPadding() const
{
    return m_itemPadding;
}

GrFont* GrColumn::GetItemFont() const
{
    return m_pItemFont;
}

int GrColumn::GetItemMinHeight() const
{
    return m_itemMinHeight;
}

GrClickEditing GrColumn::GetItemClickEditing() const
{
    return m_itemClickEditing;
}

bool GrColumn::GetItemTextVisible() const
{
    return m_itemTextVisible;
}

bool GrColumn::GetItemIcon() const
{
    return m_itemIcon;
}

GrColumnList* GrColumn::GetColumnList() const
{
    return m_pColumnList;
}

void GrColumn::SetItemHorzAlign(GrHorzAlign horzAlign)
{
    if(m_itemHorzAlign == horzAlign)
        return;
    m_itemHorzAlign = horzAlign;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnHorzAlignChanged", &e);
    }
}

void GrColumn::SetItemVertAlign(GrVertAlign vertAlign)
{
    if(m_itemVertAlign == vertAlign)
        return;
    m_itemVertAlign = vertAlign;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnVertAlignChanged", &e);
    }
}

void GrColumn::SetItemWordWrap(bool b)
{
    if(m_itemWordWrap == b)
        return;
    m_itemWordWrap = b;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnWordwrapChanged", &e);
    }
}

void GrColumn::SetItemMultiline(bool b)
{
    if(m_itemMultiline == b)
        return;
    m_itemMultiline = b;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnMultilineChanged", &e);
    }
}

void GrColumn::SetItemForeColor(const GrColor& color)
{
    m_itemForeColor = color;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
}

void GrColumn::SetItemBackColor(const GrColor& color)
{
    m_itemBackColor = color;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
}

void GrColumn::SetItemPadding(const GrPadding& padding)
{
    m_itemPadding = padding;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnPaddingChanged", &e);
    }
}

void GrColumn::SetItemFont(GrFont* pFont)
{
    m_pItemFont = pFont;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
}

void GrColumn::SetItemMinHeight(int height)
{
    if(m_itemMinHeight == height)
        return;
    m_itemMinHeight = height;

    if(m_pGridCore != NULL && m_pGridCore->GetAutoFitRow() == true)
    {
        GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
        for(uint i=0 ; i<pDataRowList->GetVisibleDataRowCount() ; i++)
        {
            GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
            pDataRow->SetFit();
        }
        m_pGridCore->GetInsertionRow()->SetFit();
        m_pGridCore->Invalidate();
    }
}

void GrColumn::SetItemClickEditing(GrClickEditing clickEditing)
{
    m_itemClickEditing = clickEditing;
}

void GrColumn::SetItemTextVisible(bool b)
{
    m_itemTextVisible = b;
}

void GrColumn::SetItemIcon(bool b)
{
    m_itemIcon = b;
}

void GrColumn::SetVisible(bool b)
{
    if(m_visible == b)
        return;
    m_visible = b;

    if(m_pGridCore == NULL)
        return;

    if(m_visible == false)
    {
        GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        if(pFocuser->GetFocusedColumn() == this)
            pFocuser->Set(IFocusable::Null);
    }

    m_pColumnList->SetVisibleChanged();
}

void GrColumn::SetReadOnly(bool b)
{
    if(m_readOnly == b)
        return;
    m_readOnly = b;
}

void GrColumn::SetItemType(GrItemType type)
{
    if(m_itemType == type)
        return;
    m_itemType = type;
}

void GrColumn::SetTooltip(const wchar_t* tooltip)
{
    m_tooltip = tooltip;
}

const wchar_t* GrColumn::GetTooltip() const
{
    return m_tooltip.c_str();
}

bool GrColumn::GetSelecting() const
{
    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    return pItemSelector->IsSelecting(this);
}

GrColor GrColumn::GetPaintingBackColor() const
{
    GrStyle* pStyle = m_pGridCore->GetStyle();
    GrColor backColor;
    if(GetGrouped() == true)
        backColor = pStyle->GetGroupBackColor(GetGroup()->GetGroupLevel());
    else
        return GrCell::GetPaintingBackColor();

    if(GetFullSelected() == true)
        backColor = backColor * 0.9f;
    else if(GetSelected() == true || GetSelecting() == true)
        backColor = backColor * 0.9f;

    if(HasFocused() == true)
    {
        backColor *= 0.9f;
    }

    return backColor;
}

GrFlag GrColumn::ToPaintStyle() const
{
    GrFlag flag = GrCell::ToPaintStyle();
    if(GetSelecting() == true)
        flag.Add(GrPaintStyle_Selected);
    return flag;
}

void GrColumn::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();

    if(m_fnColumnBackgroundPaint == NULL || (*m_fnColumnBackgroundPaint)(pPainter, this, paintRect, m_pColumnPaintData) == false)
    {
        GrFlag paintStyle = ToPaintStyle();
        GrColor backColor = GetPaintingBackColor();
        if(GetClipped() == true)
            pPainter->DrawColumn(paintStyle, paintRect, backColor, &clipRect);
        else
            pPainter->DrawColumn(paintStyle, paintRect, backColor);
    }

    if(m_fnColumnContentsPaint == NULL || (*m_fnColumnContentsPaint)(pPainter, this, paintRect, m_pColumnPaintData) == false)
    {
        GrColor foreColor = GetPaintingForeColor();

        GrSort sortType = GetSortType();
        GrRect textRect = paintRect;
        if(sortType != GrSort_None)
        {
            GrRect sortRect;
            sortRect.right = paintRect.right - 6;
            sortRect.left = sortRect.right - 10;
            sortRect.top = (paintRect.bottom + paintRect.top) / 2 - 5;
            sortRect.bottom = sortRect.top + 10;
            pPainter->DrawSortGlyph(sortRect, sortType);

            textRect.right = sortRect.left;
        }

        if(GetTextLineCount() > 0)
        {
            const GrLineDesc& cl = GetTextLine(0);
            if(GetClipped() == true)
                pPainter->DrawColumnText(GetPaintingFont(), GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor, &clipRect);
            else
                pPainter->DrawColumnText(GetPaintingFont(), GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor);
        }
    }
}

void GrColumnList::BuildVisibleColumnList()
{
    m_vecVisibleColumns.clear();
    m_vecDisplayableColumns.clear();

    m_frozenCount = 0;
    for_each(_Columns, m_vecColumns, value)
    {
        value->SetDisplayable(false);
        value->SetDisplayIndex(INVALID_INDEX);
        value->SetClipped(false);

        if(value->GetVisible() == false)
            continue;
        m_vecVisibleColumns.push_back(value);
    }

    std::sort(m_vecVisibleColumns.begin(), m_vecVisibleColumns.end(), SortColumn());

    for_each(_Columns, m_vecVisibleColumns, value)
    {
        if(value->GetFrozen() == true)
            m_frozenCount++;
        uint index = value.GetIndex();
        value->SetVisibleIndex(index);
    }
    m_widthChanged = true;
}

void GrColumnList::AdjustColumnWidth()
{
    for_each(_Columns, m_vecColumns, value)
    {
        value->AdjustWidth();
    }
    m_widthChanged = true;
}

void GrColumnList::RepositionColumnList()
{
    int x = GetX() + GetWidth() + m_pGridCore->GetGroupMargin();

    for(uint i=0 ; i<GetFrozenColumnCount() ; i++)
    {
        GrColumn* pColumn = GetFrozenColumn(i);
        x += pColumn->GetWidth();
    }

    m_frozenRight = x;

    if(m_pColumnSplitter->GetVisible() == true)
        x += m_pColumnSplitter->GetWidth();

    m_unfrozenX = x;

    for(uint i=0 ; i<GetUnfrozenColumnCount() ; i++)
    {
        GrColumn* pColumn = GetUnfrozenColumn(i);
        x += pColumn->GetWidth();
    }
    m_visibleRight = x;

    m_boundWidth = x - GetX();
    m_clippedIndex = INVALID_INDEX;
}

void GrColumnList::OnColumnInserted(GrColumnEventArgs* e)
{
    SetVisibleChanged();
    ColumnInserted(this, e);
}

void GrColumnList::OnColumnRemoved(GrColumnEventArgs* e)
{
    SetVisibleChanged();
    ColumnRemoved(this, e);
}

void GrColumnList::OnColumnSortTypeChanged(GrColumnEventArgs* e)
{
    m_pSortColumn = e->GetColumn();
    m_pGridCore->Invalidate();

    ColumnSortTypeChanged(this, e);
}

void GrColumnList::OnColumnFrozenChanged(GrColumnEventArgs* e)
{
    SetVisibleChanged();
    ColumnFrozenChanged(this, e);
}

void GrColumnList::OnColumnWidthChanged(GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetDisplayIndex() != INVALID_INDEX)
    {
        m_pGridCore->Invalidate();
    }
    m_widthChanged = true;

    ColumnWidthChanged(this, e);
}

void GrColumnList::OnColumnWordwrapChanged(GrColumnEventArgs* e)
{
    ColumnWordwrapChanged(this, e);
}

void GrColumnList::OnColumnMultilineChanged(GrColumnEventArgs* e)
{
    ColumnMultilineChanged(this, e);
}

void GrColumnList::OnColumnHorzAlignChanged(GrColumnEventArgs* e)
{
    ColumnHorzAlignChanged(this, e);
}

void GrColumnList::OnColumnVertAlignChanged(GrColumnEventArgs* e)
{
    ColumnVertAlignChanged(this, e);
}

void GrColumnList::OnColumnPaddingChanged(GrColumnEventArgs* e)
{
    ColumnPaddingChanged(this, e);
}

void GrColumnList::OnColumnGroupChanged(GrColumnEventArgs* e)
{
    ColumnGroupChanged(this, e);
}

void GrColumnList::OnColumnMouseMove(GrColumnMouseEventArgs* e)
{
    ColumnMouseMove(this, e);
}

void GrColumnList::OnColumnMouseEnter(GrColumnMouseEventArgs* e)
{
    ColumnMouseEnter(this, e);
}

void GrColumnList::OnColumnMouseLeave(GrColumnMouseEventArgs* e)
{
    ColumnMouseLeave(this, e);
}

void GrColumnList::OnColumnMouseDown(GrColumnMouseEventArgs* e)
{
    ColumnMouseDown(this, e);
}

void GrColumnList::OnColumnMouseUp(GrColumnMouseEventArgs* e)
{
    ColumnMouseUp(this, e);
}

uint GrColumnList::GetVisibleColumnCount() const
{
    return m_vecVisibleColumns.size();
}

GrColumn* GrColumnList::GetVisibleColumn(uint index) const
{
    assert(index < m_vecVisibleColumns.size());
    return m_vecVisibleColumns[index];
}

int GrColumnList::GetVisibleRight() const
{
    return m_visibleRight;
}

uint GrColumnList::GetFrozenColumnCount() const
{
    return m_frozenCount;
}

GrColumn* GrColumnList::GetFrozenColumn(uint index) const
{
    assert(index < m_frozenCount);
    return GetVisibleColumn(index);
}

uint GrColumnList::GetUnfrozenColumnCount() const
{
    assert(GetVisibleColumnCount() >= m_frozenCount);
    return GetVisibleColumnCount() - m_frozenCount;
}

GrColumn* GrColumnList::GetUnfrozenColumn(uint index) const
{
    assert(index + m_frozenCount < GetVisibleColumnCount());
    return GetVisibleColumn(index + m_frozenCount);
}

uint GrColumnList::GetDisplayableColumnCount() const
{
    return m_vecDisplayableColumns.size();
}

GrColumn* GrColumnList::GetDisplayableColumn(uint index) const
{
    assert(index < m_vecDisplayableColumns.size());
    return m_vecDisplayableColumns[index];
}

void GrRow::SetY(int y)
{
    if(m_y == y)
        return;
    m_y = y;
    OnYChanged();
}

void GrRow::SetHeight(int height)
{
    if(m_height == height)
        return;

    m_height = height;
    OnHeightChanged();
}

void GrRow::SetFit()
{
    m_fitting = true;

    OnFitted();
}

void GrRow::OnTextSizeChanged()
{
    GrCell::OnTextSizeChanged();

    if(m_pGridCore->GetAutoFitRow() == true && GetHeight() != GetTextBounds().GetHeight())
    {
        SetFit();
    }
}

void GrRow::OnHeightChanged()
{
    this->SetTextAlignChanged();
}

void GrRow::OnFitted()
{
    SetTextAlignChanged();
}

int GrRow::GetMinHeight() const
{
    return GetTextBounds().GetHeight() + GetPadding(true).GetVertical();
}

int GrRow::GetHeight() const
{
    return m_height;
}

void GrRow::AdjustHeight()
{
    if(m_fitting == false)
        return;

    int height = GetMinHeight();

    if(m_height != height)
    {
        SetHeight(height);
        OnHeightAdjusted();
    }
    m_fitting = false;
}

GrRow::GrRow() 
{
    m_y = 0;
    m_visible = true;
    m_resizable = true;

    m_height = GrRow::DefaultHeight;
    m_pParent = NULL;
    m_hierarchyLevel= 0;
    m_fitting = false;
}

void GrRow::Sort(GrSort sortType)
{
    switch(sortType)
    {
    case GrSort_Up:
        Sort(GrSortFunc::SortRowsUp, 0);
        break;
    case GrSort_Down:
        Sort(GrSortFunc::SortRowsDown, 0);
        break;
    case GrSort_None:
        Sort(GrSortFunc::SortRowsNone, 0);
        break;
    default:
        break;
    }
}

class SortDesc
{
public:
    SortDesc(GrGridCore* pGridCore, FuncSortRow fn, void* userData) : 
      m_pGridCore(pGridCore), m_fn(fn), m_userData(userData) {}

      bool operator () (const GrRow* pRow1, const GrRow* pRow2)
      {
          return (*m_fn)(m_pGridCore, pRow1, pRow2, m_userData);
      }

      GrGridCore* m_pGridCore;
      FuncSortRow m_fn;
      void* m_userData;
};

void GrRow::Sort(FuncSortRow fnSort, void* userData)
{
    std::sort(m_vecChilds.begin(), m_vecChilds.end(), SortDesc(m_pGridCore, fnSort, userData));
}

GrCell* GrRow::HitTest(const GrPoint& location) const
{
    if(GetDisplayable() == false)
        return NULL;

    GrRect bound = GetBounds();
    if(bound.Contains(location) == false)
        return NULL;

    return const_cast<GrRow*>(this);
}

void GrRow::GetVisibleList(GrRowArray* pVisible) const
{
    if(GetVisible() == false)
        return;
    pVisible->push_back(const_cast<GrRow*>(this));
    for(uint i=0 ; i<GetChildCount() ; i++)
    {
        const GrRow* pChild = GetChild(i);
        pChild->GetVisibleList(pVisible);
    }
}

GrDataRow::GrDataRow()
{
    SetDisplayable(false);

    m_readOnly = false;
    m_selectedCells = 0;
    m_pItemFont = NULL;
    m_dataRowIndex = INVALID_INDEX;
    m_dataRowID = INVALID_INDEX;
}

void GrDataRow::OnGridCoreAttached()
{
    IDataRow::OnGridCoreAttached();

    for_each(_Items, m_vecItems, value)
    {
        m_pGridCore->AttachObject(value);
    }
}

void GrDataRow::OnGridCoreDetached()
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->GetFocusedRow() == this)
        pFocuser->Set(IFocusable::Null);
    SetSelected(false);

    for_each(_Items, m_vecItems, value)
    {
        m_pGridCore->DetachObject(value);
    }

    IDataRow::OnGridCoreDetached();
}

void GrDataRow::ClearItem()
{
    for_each(_Items, m_vecItems, value)
    {
        delete value;
    }
    m_vecItems.clear();
}

GrDataRow::~GrDataRow()
{
    ClearItem(); 
}

void GrDataRow::AddItem(GrColumn* pColumn)
{
    uint columnID = pColumn->GetColumnID();
    GrItem* pItem = NULL;
    if(columnID >= m_vecItems.size())
    {
        pItem = new GrItem(pColumn, this);
        m_vecItems.push_back(pItem);
    }
    else
    {
        pItem = GetItem(pColumn);
    }

    if(m_pGridCore != NULL)
        m_pGridCore->AttachObject(pItem);
}

void GrDataRow::Reserve(uint count)
{
    m_vecItems.reserve(count);
}

void GrDataRow::SetVisible(bool b)
{
    if(m_visible == b)
        return;
    m_visible = b;

    if(m_pDataRowList != NULL)
    {
        m_pDataRowList->SetVisibleChanged();
        GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        if(pFocuser->GetFocusedRow() == this)
            pFocuser->Set(IFocusable::Null);
    }
}

int GrRow::GetX() const
{
    return m_pGridCore->GetDisplayRect().left;
}

int GrRow::GetY() const
{
    return m_y;
}

GrColor GrDataRow::GetPaintingBackColor() const
{
    GrColor backColor = GrColor::White;

    if(GetFullSelected() == true)
    {
        backColor = backColor * 0.9f;
    }
    else if(GetSelected() == true || GetSelecting() == true)
    {
        backColor = backColor * 0.9f;
    }

    if(HasFocused() == true)
    {
        backColor *= 0.0f;
    }

    return backColor;
}

GrFlag GrDataRow::ToPaintStyle() const
{
    GrFlag flag = GrRow::ToPaintStyle();
    if(GetSelecting() == true)
        flag.Add(GrPaintStyle_Selected);
    return flag;
}

void GrDataRow::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    GrFlag paintStyle = ToPaintStyle();
    GrColor foreColor = GrColor::Black;
    GrColor backColor = GetPaintingBackColor();

    if(m_pGridCore->GetRowVisible() == true)
    {
        if(GetClipped() == true)
            pPainter->DrawRow(paintStyle, paintRect, backColor, &clipRect);
        else
            pPainter->DrawRow(paintStyle, paintRect, backColor);

        if(m_pDataRowList->GetRowNumberVisible() == true)
        {
            if(GetClipped() == true)
                DrawText(pPainter, foreColor, paintRect, &clipRect);
            else
                DrawText(pPainter, foreColor, paintRect);
        }
    }

    if(m_pGridCore->IsGrouped() == true)
    {
        paintRect.left = paintRect.right;
        paintRect.right = CellStart();
        if(GetRowType() == GrRowType_InsertionRow)
            pPainter->DrawItem(0, paintRect, GrColor::White);
        else if(GetVisibleIndex() == m_pDataRowList->GetVisibleRowCount() - 1)
            pPainter->DrawItem(0, paintRect, GrColor::White);
        else
            pPainter->DrawItem(GrPaintStyle_NoBottomLine, paintRect, GrColor::White, &clipRect);
    }

    const GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
    {
        const GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
        if(pColumn->GetX() > clipRect.right || pColumn->GetRight() < clipRect.left)
            continue;
        const GrItem* pItem = GetItem(pColumn);
        pItem->Paint(pPainter, clipRect);
    }
}

bool GrRow::GetVisible() const
{
    return m_visible;
}

void GrRow::SetResizable(bool b)
{
    m_resizable = b;
}

bool GrRow::GetResizable() const
{
    return m_resizable;
}

int GrRow::CellStart() const
{
    int start = GetX() + GetWidth();
    return start + m_pGridCore->GetGroupMargin();
}

void GrRow::InvalidateRow()
{
    if(m_pGridCore == NULL)
        return;
    GrRect rect = GetRect();
    rect.right = m_pGridCore->GetDisplayRect().right;
    rect.Expand(2, 2, 2, 2);

    m_pGridCore->Invalidate(rect);
}

GrHorzAlign GrRow::GetTextHorzAlign() const
{
    return GrHorzAlign_Center;
}

GrVertAlign GrRow::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

IDataRow::IDataRow() : m_pDataRowList(0)
{
    m_displayable = true;
    m_clipped = false;
    m_visibleIndex = 0;
    m_displayIndex = 0;
}

int IDataRow::GetY() const
{
    if(m_pDataRowList == NULL)
        throw _Exception("GridCore에 붙지 않았음");

    if(GetDisplayable() == false)
    {
        uint paintingCount = m_pDataRowList->GetDisplayableRowCount();
        if(paintingCount == 0)
            return GrRow::GetY();

        IDataRow* pFirstPainting = m_pDataRowList->GetDisplayableRow(0);
        int offset = 0;
        if(pFirstPainting != this)
        {
            offset = pFirstPainting->GetY() - GrRow::GetY();
        }
        return m_pDataRowList->GetY() - offset;
    }

    return GrRow::GetY();
}

void IDataRow::SetVisibleIndex(uint index)
{
    m_visibleIndex = index;
}

uint IDataRow::GetVisibleIndex() const
{
    return m_visibleIndex;
}

void IDataRow::SetDisplayIndex(uint index)
{
    m_displayIndex = index;
}

uint IDataRow::GetDisplayIndex() const
{
    return m_displayIndex;
}

void IDataRow::SetClipped(bool b)
{
    m_clipped = b;
}

bool IDataRow::GetClipped() const
{
    return m_clipped;
}

bool IDataRow::ShouldBringIntoView() const
{
    if(GetVisible() == false)
        throw _Exception("");
    if(m_displayable == false || m_clipped == true)
        return true;
    return false;
}

void IDataRow::SetDisplayable(bool b)
{ 
    m_displayable = b; 
}

bool IDataRow::GetDisplayable() const
{
    if(GetVisible() == false)
        return false;
    return m_displayable; 
}

int IDataRow::GetWidth() const
{
    if(m_pDataRowList == NULL)
        return 0;
    if(m_pGridCore->GetRowVisible() == false)
        return 0;
    return m_pDataRowList->GetRowWidth();
}

void IDataRow::OnFitted()
{
    GrRow::OnFitted();
    m_pDataRowList->SetFitChanged();
}

void IDataRow::OnGridCoreAttached()
{
    GrRow::OnGridCoreAttached();
    m_pDataRowList = m_pGridCore->GetDataRowList();
}

void IDataRow::OnHeightChanged()
{
    GrRow::OnHeightChanged();
    if(m_pDataRowList != NULL)
        m_pDataRowList->SetHeightChanged();
}

bool IDataRow::HasFocused() const
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->GetFocusedRow() == this)
        return true;
    return false;
}

GrRowSplitter::GrRowSplitter()
{
    SetText(L"splitter");
    SetHeight(DefaultSplitterHeight);
    SetResizable(false);

    SetTextVisible(false);
}

bool GrRowSplitter::GetVisible() const
{
    return m_pGridCore->GetInsertionRow()->GetVisible();
}

int GrRowSplitter::GetWidth() const
{
    return m_pGridCore->GetBounds().GetWidth();
}

void GrRowSplitter::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
{
    pPainter->DrawRowSplitter(GetRect());
}

GrGroupRow::GrGroupRow() 
{
    m_groupLevel = 0;
    m_expanded = true;
    m_pColumn = NULL;

    m_pLabel = new GrGroupHeader(this);
}

GrGroupRow::~GrGroupRow()
{
    delete m_pLabel;
}

GrGroupHeader* GrGroupRow::GetLabel() const
{
    return m_pLabel;
}

void GrGroupRow::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    GrRect paintRect = GetRect();
    if(paintRect.top > clipRect.bottom || paintRect.bottom <= clipRect.top)
        return;

    if(m_pGridCore->GetRowVisible() == true)
    {
        GrColor color = GrColor::White;
        GrFlag paintStyle = ToPaintStyle();

        if(pFocuser->GetFocusedRow() == this)
        {
            color.r = (byte)((float)color.r * 0.75f);
            color.g = (byte)((float)color.g * 0.75f);
            color.b = (byte)((float)color.b * 0.75f);
        }

        pPainter->DrawRow(paintStyle, paintRect, color);
    }

    paintRect.left = paintRect.right;
    paintRect.right = paintRect.left + m_groupLevel * DEF_GROUP_WIDTH;

    pPainter->DrawItem(GrPaintStyle_NoBottomLine | GrPaintStyle_NoRightLine, paintRect, GrColor::White);
    m_pLabel->Paint(pPainter, clipRect);
}

bool GrGroupRow::GetVisible() const
{
    GrGroupRow* pParentGroupRow = dynamic_cast<GrGroupRow*>(GetParent());
    if(pParentGroupRow == NULL)
        return GrRow::GetVisible();

    return pParentGroupRow->IsExpanded();
}

IFocusable* GrGroupRow::GetFocusable(GrColumn* /*pColumn*/) const
{
    return m_pLabel;
}

GrCell* GrGroupRow::HitTest(const GrPoint& location) const
{
    if(ContainsVert(location.y) == false)
        return NULL;

    if(m_pLabel->ContainsHorz(location.x) == true)
        return m_pLabel;

    if(ContainsHorz(location.x) == false)
        return NULL;

    return const_cast<GrGroupRow*>(this);
}

GrCaption::GrCaption()
{
    m_horzAlign = GrHorzAlign_Left;
    m_vertAlign = GrVertAlign_Center;

    m_visible = true;
}

GrHorzAlign GrCaption::GetTextHorzAlign() const
{
    return m_horzAlign;
}

GrVertAlign GrCaption::GetTextVertAlign() const
{
    return m_vertAlign;
}

void GrCaption::SetVisible(bool b)
{
    if(m_visible == b)
        return;

    m_visible = b;

    if(m_pGridCore == NULL)
        return;
    m_pGridCore->GetRootRow()->SetVisibleChanged();
    m_pGridCore->Invalidate();
}

bool GrCaption::GetVisible() const
{
    return m_visible;
}

void GrCaption::SetTextHorzAlign(GrHorzAlign align)
{
    if(m_horzAlign == align)
        return;
    m_horzAlign = align;
    SetTextAlignChanged();
}

void GrCaption::SetTextVertAlign(GrVertAlign align)
{
    if(m_vertAlign == align)
        return;
    m_vertAlign = align;
    SetTextAlignChanged();
}

void GrCaption::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    if(paintRect.top > clipRect.bottom || paintRect.bottom <= clipRect.top)
        return;

    GrFlag paintStyle = ToPaintStyle();
    GrColor foreColor = GetPaintingForeColor();
    GrColor backColor = GetPaintingBackColor();

    pPainter->DrawItem(paintStyle, paintRect, backColor);

    DrawText(pPainter, foreColor, paintRect, &paintRect);
}

int GrCaption::GetWidth() const
{
    return m_pGridCore->GetBounds().GetWidth();
}

void GrCaption::OnGridCoreAttached()
{
    GrUpdatableRow::OnGridCoreAttached();
    m_pGridCore->FontChanged.Add(this, &GrCaption::gridCore_FontChanged);
}

void GrCaption::gridCore_FontChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_pTextUpdater->AddTextBounds(this);
}

GrGroupPanel::GrGroupPanel()
{
    m_enableGroup = true;
    m_visible = true;
    m_groupChanged = false;
}

GrGroupPanel::~GrGroupPanel()
{

}

uint GrGroupPanel::GetGroupCount() const
{
    if(m_enableGroup == false)
        return 0;
    return m_vecGroups.size();
}

GrGroup* GrGroupPanel::GetGroup(uint level) const
{
    return m_vecGroups[level];
}

void GrGroupPanel::ExpandGroup(uint level, bool /*expand*/)
{
    GrGroup* pGroup = m_vecGroups[level];
    pGroup->SetExpanded(true);
}

void GrGroupPanel::SetGroupSortState(uint level, GrSort sortType)
{
    GrGroup* pGroup = m_vecGroups[level];
    pGroup->SetSortType(sortType);
}

GrCell* GrGroupPanel::HitTest(const GrPoint& location) const
{
    if(ContainsVert(location.y) == false)
        return NULL;

    for_each(_Groups, m_vecGroups, value)
    {
        GrRect rect = value->GetRect();
        if(rect.Contains(location) == true)
            return value;
    }

    return const_cast<GrGroupPanel*>(this);
}

bool GrGroupPanel::GetVisible() const 
{
    if(m_enableGroup == false)
        return false;
    return m_visible; 
}

int GrGroupPanel::GetMinHeight() const
{
    if(m_vecGroups.size() == 0)
    {
        return GrRow::GetMinHeight();
    }
    return m_vecGroups[0]->GetHeight() + 20;
}

void GrGroupPanel::SetVisible(bool b)
{
    m_visible = b;
    GrRootRow* pHeaderList = dynamic_cast<GrRootRow*>(GetParent());
    pHeaderList->SetVisibleChanged();
}

bool GrGroupPanel::GetGroupable() const
{
    return m_enableGroup;
}

void GrGroupPanel::SetGroupable(bool b)
{
    if(m_enableGroup == b)
        return;
    m_enableGroup = b;
    Changed(this, &GrEventArgs::Empty);
}

void GrGroupPanel::OnYChanged()
{
    GrUpdatableRow::OnYChanged();
    RepositionGroup();
}

void GrGroupPanel::RepositionGroup()
{
    GrPoint pt;
    pt.x = GetX();
    pt.y = GetY();

    pt.x += 10;
    pt.y += 10;
    for_each(_Groups, m_vecGroups, value)
    {
        value->SetPosition(pt);
        pt.x += value->GetWidth() + 10;
    }
}

bool GrGroupPanel::ShouldUpdate() const
{
    return m_groupChanged == true;
}

void GrGroupPanel::Update(bool /*force*/)
{
    if(m_groupChanged == true)
        RepositionGroup();

    if(m_vecGroups.size() == 0)
        SetTextVisible(true);
    else
        SetTextVisible(false);
    m_groupChanged = false;
}

void GrGroupPanel::ResetGroupLevel()
{
    for_each(_Groups, m_vecGroups, value)
    {
        value->SetGroupLevelCore(value.GetIndex());
    }
}

void GrGroupPanel::AddGroup(GrGroup* pGroup)
{
    _Groups::iterator itor = find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);

    if(itor != m_vecGroups.end())
        throw _Exception("이미 Group이 되어 있습니다.");

    uint level = pGroup->GetGroupLevel();
    if(level > m_vecGroups.size())
        m_vecGroups.push_back(pGroup);
    else
        m_vecGroups.insert(m_vecGroups.begin() + level, pGroup);
    pGroup->SetText();
    pGroup->LevelChanged.Add(this, &GrGroupPanel::groupInfo_LevelChanged);

    ResetGroupLevel();
    SetFit();
    m_pGridCore->Invalidate();
    m_groupChanged = true;
    Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupPanel::RemoveGroup(GrGroup* pGroup)
{
    _Groups::iterator itor = find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);

    if(itor == m_vecGroups.end())
        throw _Exception("Group이 되어 있지 않은데 해제하려고 합니다.");

    pGroup->LevelChanged.Remove(this, &GrGroupPanel::groupInfo_LevelChanged);
    (*itor)->SetGroupLevelCore(INVALID_INDEX);
    m_vecGroups.erase(itor);

    ResetGroupLevel();
    SetFit();
    m_pGridCore->Invalidate();
    m_groupChanged = true;
    Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupPanel::groupInfo_LevelChanged(GrObject* pSender, GrEventArgs* /*e*/)
{
    GrGroup* pGroup = (GrGroup*)pSender;
    _Groups::iterator itor = std::find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);
    *itor = NULL;

    uint index = std::min(pGroup->GetGroupLevel(), m_vecGroups.size());

    m_vecGroups.insert(m_vecGroups.begin() + index, pGroup);
    m_vecGroups.erase(std::find(m_vecGroups.begin(), m_vecGroups.end(), (GrGroup*)NULL));

    for_each(_Groups, m_vecGroups, value)
    {
        value->SetGroupLevelCore(value.GetIndex());
    }

    Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupPanel::NotifyExpanded(GrGroup* pGroup)
{
    GrGroupEventArgs e(pGroup);
    Expanded.Raise(this, &e);
}

void GrGroupPanel::NotifySortChanged(GrGroup* pGroup)
{
    GrGroupEventArgs e(pGroup);
    SortChanged.Raise(this, &e);;
}

void GrGroupPanel::OnGridCoreAttached()
{
    GrRow::OnGridCoreAttached();
    m_pGridCore->Cleared.Add(this, &GrGroupPanel::gridCore_Cleared);
    m_pGridCore->Created.Add(this, &GrGroupPanel::gridCore_Created);
    m_pGridCore->FontChanged.Add(this, &GrGroupPanel::gridCore_FontChanged);
}

void GrGroupPanel::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_vecGroups.clear();
    SetFit();
}

void GrGroupPanel::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    pColumnList->ColumnGroupChanged.Add(this, &GrGroupPanel::columnList_ColumnGroupChanged);
    pColumnList->ColumnInserted.Add(this, &GrGroupPanel::columnList_ColumnInserted);
    pColumnList->ColumnRemoved.Add(this, &GrGroupPanel::columnList_ColumnRemoved);
}

void GrGroupPanel::gridCore_FontChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_pTextUpdater->AddTextBounds(this);

    for_each(_Groups, m_vecGroups, value)
    {
        m_pTextUpdater->AddTextBounds(value);
    }
}

void GrGroupPanel::columnList_ColumnInserted(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetGrouped() == false)
        return;
    AddGroup(pColumn->GetGroup());
}

void GrGroupPanel::columnList_ColumnRemoved(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetGrouped() == false)
        return;
    RemoveGroup(pColumn->GetGroup());
}

void GrGroupPanel::columnList_ColumnGroupChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetGrouped() == true)
        AddGroup(pColumn->GetGroup());
    else
        RemoveGroup(pColumn->GetGroup());
}

void GrGroupPanel::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    if(paintRect.top > clipRect.bottom || paintRect.bottom <= clipRect.top)
        return;

    GrFlag paintStyle = ToPaintStyle();
    GrColor foreColor = GetPaintingForeColor();
    GrColor backColor = GetPaintingBackColor();

    pPainter->DrawItem(paintStyle, paintRect, backColor);

    for_each(_Groups, m_vecGroups, value)
    {
        value->Paint(pPainter, clipRect);
    }

    DrawText(pPainter, foreColor, paintRect, &clipRect);
}

int GrGroupPanel::GetWidth() const
{
    return m_pGridCore->GetBounds().GetWidth();
}

uint GrRow::GetChildCount() const
{
    return m_vecChilds.size();
}

GrRow* GrRow::GetChild(uint index) const
{
    assert(index < m_vecChilds.size());
    return m_vecChilds[index];
}

void GrRow::AddChild(GrRow* pRow)
{
    m_vecChilds.push_back(pRow);
    pRow->m_pParent = this;
    pRow->m_hierarchyLevel = m_hierarchyLevel + 1;
}

void GrRow::ReserveChild(uint reserve)
{
    m_vecChilds.reserve(reserve);
}

void GrRow::ClearChild()
{
    for_each(GrRowArray, m_vecChilds, value)
    {
        value->m_pParent = NULL;
        value->m_hierarchyLevel = 0;
    }
    m_vecChilds.clear();
}

GrRow* GrRow::GetParent() const
{
    return m_pParent;
}

uint GrRow::GetHierarchyLevel() const
{
    return m_hierarchyLevel;
}

void GrGroupRow::Expand(bool b)
{
    if(m_expanded == b)
        return;
    if(m_pDataRowList != NULL)
        m_pDataRowList->SetVisibleChanged();
    m_expanded = b;
}

bool GrGroupRow::IsExpanded() const
{
    return m_expanded;
}

void GrGroupRow::OnHeightChanged()
{
    IDataRow::OnHeightChanged();
    SetTextAlignChanged();
}

void GrGroupRow::OnUpdatePositionCell(int x, GrRect* pBounds)
{
    pBounds->left = x;
    pBounds->right = m_pLabel->GetX() + m_pLabel->GetWidth();
}

void GrGroupRow::OnGridCoreAttached()
{
    IDataRow::OnGridCoreAttached();
    m_pGridCore->AttachObject(m_pLabel);
}

void GrGroupRow::SetReference(GrColumn* pColumn, const std::wstring& itemText)
{
    m_pColumn = pColumn;
    m_itemText = itemText;

    m_key = pColumn->GetText();
    m_key += L" | ";
    m_key += itemText;
}

void GrGroupRow::ProcessAfterGroup()
{
    GrGroupRow* pParent = dynamic_cast<GrGroupRow*>(GetParent());
    if(pParent)
    {
        m_groupLevel = pParent->m_groupLevel + 1;
    }
    else
    {
        m_groupLevel = 0;
    }

    std::wstring text = GetText();


    //wchar_t itemText[30];
    //swprintf(itemText, 30, L" - %d items", GetChildCount());
    //text = m_itemText + itemText;
    //m_pLabel->SetText(text.c_str());
    m_pLabel->SetText(m_itemText.c_str());
}

GrColumn* GrGroupRow::GetColumn() const
{
    return m_pColumn;
}

GrInsertionRow::GrInsertionRow()
{
    SetText(L"☞");
    SetDisplayable(true);
}

void GrInsertionRow::OnGridCoreAttached()
{
    GrDataRow::OnGridCoreAttached();
}

void GrInsertionRow::OnHeightChanged()
{
    GrRow::OnHeightChanged();
    GrRootRow* pHeaderRow = dynamic_cast<GrRootRow*>(GetParent());
    if(pHeaderRow != NULL)
        pHeaderRow->SetHeightChanged();
}

void GrInsertionRow::SetVisible(bool b)
{
    GrDataRow::SetVisible(b);
    GrRootRow* pHeaderList = dynamic_cast<GrRootRow*>(GetParent());

    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->GetFocusedRow() == this)
        pFocuser->Set(IFocusable::Null);

    if(GetSelected() == true)
    {
        GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
        pItemSelector->SelectItems(this, GrSelectionType_Remove);
    }

    pHeaderList->SetVisibleChanged();
}

GrDataRowList::GrDataRowList()
{
    SetText(L"DataRowList");

    m_rowWidth = 50;
    m_displayableBottom = 0;

    m_groupCount = 0;
    m_usedGroupRow = 0;
    m_pFocusedDataRow = NULL;
    m_dataRowID = 0;
    m_visibleRowNumber = true;
    m_clippedIndex = INVALID_INDEX;
    m_zeroBasedRowIndex = false;

    m_listChanged = false;
    m_visibleChanged = false;
    m_fitChanged = false;
}

void GrDataRowList::OnGridCoreAttached()
{
    GrRow::OnGridCoreAttached();

    m_vecDataRows.reserve(m_pGridCore->GetReservedRow());

    GrGroupPanel* pGroupList = m_pGridCore->GetGroupPanel();

    pGroupList->Changed.Add(this, &GrDataRowList::groupPanel_Changed);
    pGroupList->Expanded.Add(this, &GrDataRowList::groupPanel_Expanded);
    pGroupList->SortChanged.Add(this, &GrDataRowList::groupPanel_SortChanged);

    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    pFocuser->FocusChanged.Add(this, &GrDataRowList::focuser_FocusedChanged);

    m_pGridCore->Cleared.Add(this, &GrDataRowList::gridCore_Cleared);
    m_pGridCore->Created.Add(this, &GrDataRowList::gridCore_Created);
    m_pGridCore->FontChanged.Add(this, &GrDataRowList::gridCore_FontChanged);

    for_each(_DataRows, m_vecDataRows, value)
    {
        m_pGridCore->AttachObject(value);
    }

    for_each(_GroupRows, m_vecGroupRows, value)
    {
        m_pGridCore->AttachObject(value);
    }

    m_pInsertionRow = m_pGridCore->GetInsertionRow();

    m_pInsertionRow->SetVisibleDataRowIndex(INSERTION_ROW);
    m_pInsertionRow->SetVisibleIndex(INSERTION_ROW);
}

void GrDataRowList::OnYChanged()
{
    RepositionVisibleRowList();
}

void GrDataRowList::OnDataRowInserted(GrDataRowEventArgs* e)
{
    SetListChanged();
    m_pGridCore->Invalidate();

    DataRowInserted(this, e);
}

void GrDataRowList::OnDataRowRemoved(GrDataRowEventArgs* e)
{
    GrDataRow* pDataRow = e->GetDataRow();

    pDataRow->SetDataRowIndex(INVALID_INDEX);
    pDataRow->SetSelected(false);

    if(pDataRow->HasFocused() == true)
        m_pGridCore->GetFocuser()->Set(IFocusable::Null);

    m_pGridCore->Invalidate();

    Update(true);

    DataRowRemoved(this, e);
}

GrGroupRow* GrDataRowList::CreateGroupRow(GrRow* pParent, GrColumn* pColumn, const std::wstring& itemText)
{
    GrGroupRow* pGroupRow;
    if(m_vecGroupRows.size() == m_usedGroupRow)
    {
        pGroupRow = new GrGroupRow();
        m_vecGroupRows.push_back(pGroupRow);
    }
    else
    {
        pGroupRow = m_vecGroupRows[m_usedGroupRow];
        pGroupRow->ClearChild();
    }

    pParent->AddChild(pGroupRow);

    pGroupRow->SetTextVisible(false);
    pGroupRow->SetReference(pColumn, itemText);
    pGroupRow->m_groupLevel = 0;

    _MapCaches::iterator itor = m_mapCache.find(pGroupRow->GetKey());
    if(itor == m_mapCache.end())
    {
        const GrPadding padding = pGroupRow->GetPadding(true);
        int height = GetPaintingFont()->GetHeight() + padding.GetVertical();
        pGroupRow->SetHeight(height);
        pGroupRow->Expand(true);
    }
    else
    {
        pGroupRow->SetHeight((*itor).second.height);
        pGroupRow->Expand((*itor).second.expanded);
    }

    m_pGridCore->AttachObject(pGroupRow);

    m_usedGroupRow++;
    return pGroupRow;
}

GrDataRowList::~GrDataRowList()
{
    DeleteObjects();
}

void GrDataRowList::BuildGroup(GrRow* pParent, uint groupLevel)
{
    GrGroupPanel* pGroupList = m_pGridCore->GetGroupPanel();
    m_pGridCore->AttachObject(pGroupList);
    GrColumn* pColumn = pGroupList->GetGroup(groupLevel)->GetColumn();

    _DataRows vecSort;

    FuncSortRow fnSort;
    switch(pColumn->GetGroup()->GetSortType())
    {
    case GrSort_Up:
        fnSort = GrSortFunc::SortDataRowSortUp;
        break;
    default:
        fnSort = GrSortFunc::SortDataRowSortDown;
        break;
    }
    pParent->Sort(fnSort, (void*)pColumn);

    vecSort.reserve(pParent->GetChildCount());
    for(uint i=0 ; i<pParent->GetChildCount() ; i++)
    {
        GrRow* pChild = pParent->GetChild(i);
        if(pChild->GetRowType() == GrRowType_DataRow)
            vecSort.push_back((GrDataRow*)pChild);
    }

    pParent->ClearChild();

    std::wstring itemText;
    _DataRows::iterator itor = vecSort.begin();
    GrGroupRow* pGroupRow = NULL;

    while(itor != vecSort.end())
    {
        GrDataRow* pRow = *itor;
        GrItem* pItem = pRow->GetItem(pColumn);
        std::wstring nextItemText = pItem->GetText();

        if(itemText != nextItemText || itor == vecSort.begin())
        {
            itemText = nextItemText;
            pGroupRow = CreateGroupRow(pParent, pColumn, itemText.c_str());
        }
        pGroupRow->AddChild(pRow);
        itor++;
    }

    uint nextGroupLevel = groupLevel+1;
    for(uint i=0 ; i<pParent->GetChildCount() ; i++)
    {
        GrGroupRow* pRow = (GrGroupRow*)pParent->GetChild(i);
        pRow->ProcessAfterGroup();
        if(nextGroupLevel < m_groupCount)
            BuildGroup(pRow, nextGroupLevel);
    }
}

void GrDataRowList::BuildCache()
{
    m_mapCache.clear();
    for(uint i=0 ; i<m_usedGroupRow ; i++)
    {
        GrGroupRow* pGroupRow = m_vecGroupRows[i];

        GrCache cache;
        cache.height = pGroupRow->GetHeight();
        cache.expanded = pGroupRow->IsExpanded();

        m_mapCache.insert(_MapCaches::value_type(pGroupRow->m_key, cache));
    }
}

void GrDataRowList::BuildChildRowList()
{
    ClearChild();

    ReserveChild(m_vecDataRows.size());
    for_each(_DataRows, m_vecDataRows, value)
    {
        AddChild(value);
    }

    if(m_groupCount == 0)
    {
        m_usedGroupRow = 0;
        m_mapCache.clear();

        GrColumn* pColumn = m_pGridCore->GetColumnList()->GetFirstSortColumn();
        if(pColumn)
            Sort(pColumn);
        else
            GrRow::Sort(GrSort_None);
    }
    else
    {
        BuildCache();
        m_usedGroupRow = 0;
        BuildGroup(this, 0);
    }

    SetVisibleChanged();
}

void GrDataRowList::AdjustRowHeight()
{
    for_each(_IDataRows, m_vecVisibleRows, value)
    {
        value->AdjustHeight();
    }
    m_pInsertionRow->AdjustHeight();

    m_fitChanged = false;
    m_heightChanged = true;

    m_pGridCore->Invalidate();
}

void GrDataRowList::SetFitChanged()
{
    m_fitChanged = true;
}

void GrDataRowList::SetVisibleChanged()
{
    if(m_visibleChanged == false)
    {
        m_pGridCore->Invalidate();
    }

    m_visibleChanged = true;
}

void GrDataRowList::SetListChanged()
{
    if(m_listChanged == false)
    {
        m_pGridCore->Invalidate();
    }
    m_listChanged = true;
}

void GrDataRowList::SetHeightChanged()
{
    if(m_heightChanged == false)
    {
        m_pGridCore->Invalidate();
    }
    m_heightChanged = true;
}

void GrDataRowList::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    for_each(_IDataRows, m_vecDisplayableRows, value)
    {
        int y = value->GetY();
        int b = value->GetBottom();
        if(y >= clipRect.bottom || b < clipRect.top)
            continue;

        value->Paint(pPainter, clipRect);
    }

    if(m_pFocusedDataRow != NULL && m_pFocusedDataRow->GetDisplayable() == true)
    {
        if(m_pGridCore->GetRowHighlight() == true && m_pGridCore->GetRowHighlightType() != GrRowHighlightType_Fill)
        {
            GrRect highlightRect = m_pFocusedDataRow->GetRect();
            GrStyle* pStyle = m_pGridCore->GetStyle();
            GrRect displayRect = m_pGridCore->GetDisplayRect();
            highlightRect.top--;
            highlightRect.right = m_pGridCore->GetColumnList()->GetBounds().right;
            pPainter->DrawRectangle(highlightRect, pStyle->RowHighlightForeColor);
        }
    }
}

void GrDataRowList::Reserve(uint reserve)
{
    m_vecDataRows.reserve(reserve);
}

void GrDataRowList::groupPanel_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrGroupPanel* pGroupList = m_pGridCore->GetGroupPanel();
    m_groupCount = pGroupList->GetGroupCount();
    m_clippedIndex = INVALID_INDEX;

    BuildChildRowList();
}

void GrDataRowList::groupPanel_Expanded(GrObject* /*pSender*/, GrGroupEventArgs* e)
{
    GrColumn* pColumn = e->m_pGroup->GetColumn();
    for_each(_GroupRows, m_vecGroupRows, value)
    {
        if(value->GetColumn() == pColumn)
            value->Expand(e->m_pGroup->GetExpanded());
    }
    SetVisibleChanged();
}

void GrDataRowList::groupPanel_SortChanged(GrObject* /*pSender*/, GrGroupEventArgs* e)
{
    GrColumn* pColumn = e->m_pGroup->GetColumn();

    std::set<GrRow*> parentRows;
    for(uint i=0 ; i<m_usedGroupRow ; i++)
    {
        GrGroupRow* pGroupRow = m_vecGroupRows[i];

        if(pGroupRow->GetColumn() == pColumn)
            parentRows.insert(pGroupRow->GetParent());
    }

    for_each(std::set<GrRow*>, parentRows, value)
    {
        value->Sort(e->m_pGroup->GetSortType());
    }
    SetVisibleChanged();
}

void GrDataRowList::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    pColumnList->ColumnInserted.Add(this, &GrDataRowList::columnList_ColumnInserted);
    pColumnList->ColumnRemoved.Add(this, &GrDataRowList::columnList_ColumnRemoved);
    pColumnList->ColumnSortTypeChanged.Add(this, &GrDataRowList::columnList_ColumnSortTypeChanged);

    m_pInsertionRow->SetDataRowID(INSERTION_ROW);
}

void GrDataRowList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    ClearChild();

    m_groupCount = 0;
    m_usedGroupRow = 0;
    m_pFocusedDataRow = NULL;
    m_dataRowID = 0;

    m_pInsertionRow->ClearItem();

    DeleteObjects();

    m_vecVisibleDataRows.clear();
    m_vecVisibleRows.clear();
    m_vecDisplayableRows.clear();

    m_mapCache.clear();
    m_vecColumns.clear();
}

void GrDataRowList::gridCore_FontChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    _DataRows vecDataRows = m_vecDataRows;
    vecDataRows.push_back(m_pInsertionRow);

    for_each(_DataRows, vecDataRows, value)
    {
        GrDataRow* pDataRow = value;
        m_pTextUpdater->AddTextBounds(pDataRow);

        for(uint i=0 ; i<pColumnList->GetColumnCount() ; i++)
        {
            GrColumn* pColumn = pColumnList->GetColumn(i);
            GrItem* pItem = pDataRow->GetItem(pColumn);
            m_pTextUpdater->AddTextBounds(pItem);
        }
    }

    for_each(_GroupRows, m_vecGroupRows, value)
    {
        m_pTextUpdater->AddTextBounds(value);
        m_pTextUpdater->AddTextBounds(value->GetLabel());
    }
}

void GrDataRowList::columnList_ColumnInserted(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();

    if(pColumn->GetColumnID() >= m_vecColumns.size())
    {
        m_vecColumns.push_back(pColumn);
    }

    m_pInsertionRow->AddItem(pColumn);
    for_each(_DataRows, m_vecDataRows, value)
    {
        value->AddItem(pColumn);
    }

    for_each(_DataRows, m_vecDataRowsRemoved, value)
    {
        value->AddItem(pColumn);
    }
}

void GrDataRowList::columnList_ColumnRemoved(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrItem* pItem = m_pInsertionRow->GetItem(e->GetColumn());
    m_pGridCore->DetachObject(pItem);

    for_each(_DataRows, m_vecDataRows, value)
    {
        pItem = value->GetItem(e->GetColumn());
        m_pGridCore->DetachObject(pItem);
    }
}

void GrDataRowList::columnList_ColumnSortTypeChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    Sort(pColumn);
}

void GrDataRowList::focuser_FocusedChanged(GrObject* pSender, GrFocusChangeArgs* /*e*/)
{
    GrFocuser* pFocuser = (GrFocuser*)pSender;
    m_pFocusedDataRow = dynamic_cast<GrDataRow*>(pFocuser->GetFocusedRow());
}

void GrDataRowList::BuildVisibleRowList()
{
    GrRowArray vecVisibles;
    vecVisibles.reserve(m_pGridCore->GetReservedRow());
    GetVisibleList(&vecVisibles);

    m_vecVisibleRows.clear();
    m_vecVisibleRows.reserve(vecVisibles.size());
    m_vecVisibleDataRows.clear();
    m_vecVisibleDataRows.reserve(vecVisibles.size());

    m_visibleHeight = 0;
    for_each(GrRowArray, vecVisibles, value)
    {
        IDataRow* pDataRowBase = dynamic_cast<IDataRow*>(value.GetValue());
        if(pDataRowBase == NULL)
            continue;
        GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowBase);
        if(pDataRow)
        {
            pDataRow->SetVisibleDataRowIndex(m_vecVisibleDataRows.size());
            m_vecVisibleDataRows.push_back(pDataRow);
        }
        pDataRowBase->SetVisibleIndex(m_vecVisibleRows.size());
        m_vecVisibleRows.push_back(pDataRowBase);

        m_visibleHeight += pDataRowBase->GetHeight();
    }

    for_each(_IDataRows, m_vecDisplayableRows, value)
    {
        value->SetDisplayable(false);
        value->SetDisplayIndex(INVALID_INDEX);
        value->SetClipped(false);
    }

    m_vecDisplayableRows.clear();
    m_heightChanged = true;
}

void GrDataRowList::RepositionVisibleRowList()
{
    int y = GetY();
    for_each(_IDataRows, m_vecVisibleRows, value)
    {
        y += value->GetHeight();
    }
    m_visibleBottom = y;
    m_visibleHeight = m_visibleBottom - GetY();
    m_clippedIndex = INVALID_INDEX;
}

bool GrDataRowList::ShouldUpdate() const
{
    return m_listChanged == true ||
        m_visibleChanged == true ||
        m_fitChanged == true ||
        m_heightChanged == true;
}

void GrDataRowList::Update(bool force)
{
    if(m_listChanged == true || force == true)
        BuildChildRowList();
    if(m_visibleChanged == true)
        BuildVisibleRowList();
    if(m_fitChanged == true)
        AdjustRowHeight();
    if(m_heightChanged == true)
        RepositionVisibleRowList();

    m_listChanged = false;
    m_visibleChanged = false;
    m_fitChanged = false;
    m_heightChanged = false;

    bool visible = false;
    if(GetVisibleRowCount() != 0)
    {
        visible = m_visibleBottom > m_pGridCore->GetDisplayRect().bottom ? true : false;
    }

    m_pGridCore->GetVertScroll()->SetVisible(visible);
}

uint GrDataRowList::GetVisibleDataRowCount() const
{
    return m_vecVisibleDataRows.size();
}

GrDataRow* GrDataRowList::GetVisibleDataRow(uint index) const
{
    if(index == INSERTION_ROW)
        return m_pInsertionRow;
    if(index >= m_vecVisibleDataRows.size())
        throw _Exception("잘못된 인덱스");
    return m_vecVisibleDataRows[index];
}

int GrDataRowList::GetRowWidth() const
{
    return m_rowWidth;
}

void GrDataRowList::SetRowWidth(int width)
{
    m_rowWidth = width;
}

uint GrDataRowList::GetVisibleRowCount() const
{
    return m_vecVisibleRows.size();
}

IDataRow* GrDataRowList::GetVisibleRow(uint index) const
{
    if(index == INSERTION_ROW)
        return m_pGridCore->GetInsertionRow();
    if(index >= m_vecVisibleRows.size())
        throw _Exception("잘못된 인덱스");
    return m_vecVisibleRows[index];
}

void GrDataRowList::AddDataRow(GrDataRow* pDataRow)
{
    InsertDataRow(pDataRow, m_vecDataRows.size());
}

void GrDataRowList::InsertDataRow(GrDataRow* pDataRow, uint index)
{
    if(pDataRow->GetDataRowIndex() != INVALID_INDEX || pDataRow == m_pInsertionRow)
        throw _Exception("이미 등록되어 있습니다");

    if(pDataRow->GetDataRowID() == INVALID_INDEX)
    {
        pDataRow->SetDataRowID(m_dataRowID++);
        pDataRow->Reserve(m_vecColumns.size());
        for_each(_Columns, m_vecColumns, value)
        {
            pDataRow->AddItem(value);
        }
    }
    else
    {
        _DataRows::iterator itor = find(m_vecDataRowsRemoved.begin(), m_vecDataRowsRemoved.end(), pDataRow);
        m_vecDataRowsRemoved.erase(itor);
    }

    m_pGridCore->AttachObject(pDataRow);

    _DataRows::iterator itor = m_vecDataRows.insert(m_vecDataRows.begin() + index, pDataRow);
    index = std::min(index, m_vecDataRows.size());
    for( ; itor != m_vecDataRows.end() ; itor++)
    {
        (*itor)->SetDataRowIndex(index++);
    }

    GrDataRowEventArgs e(pDataRow);
    OnDataRowInserted(&e);
}

void GrDataRowList::RemoveDataRow(GrDataRow* pDataRow)
{
    if(pDataRow->GetDataRowIndex() == INVALID_INDEX)
        return;

    _DataRows::iterator itor = std::find(m_vecDataRows.begin(), m_vecDataRows.end(), pDataRow);
    if(itor == m_vecDataRows.end())
        return;

    uint dataRowIndex = pDataRow->GetDataRowIndex();
    for(_DataRows::iterator next = itor + 1 ; next != m_vecDataRows.end() ; next++)
    {
        (*next)->SetDataRowIndex(dataRowIndex);
        dataRowIndex++;
    }

    m_vecDataRowsRemoved.push_back(pDataRow);
    m_vecDataRows.erase(itor);
    
    GrDataRowEventArgs e(pDataRow);
    OnDataRowRemoved(&e);
}

GrDataRow* GrDataRowList::NewDataRowFromInsertion()
{
    GrDataRow* pNewRow = new GrDataRow();

    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    GrInsertionRow* pInsertionRow = m_pGridCore->GetInsertionRow();

    for(uint i=0 ; i<pColumnList->GetColumnCount() ; i++)
    {
        GrColumn* pColumn = pColumnList->GetColumn(i);
        GrItem* pNewCell = pNewRow->GetItem(pColumn);
        GrItem* pEditCell = pInsertionRow->GetItem(pColumn);

        pNewCell->SetText(pEditCell->GetText());
        pEditCell->SetText(L"");
    }
    return pNewRow;
}

uint GrDataRowList::GetDataRowCount() const
{
    return m_vecDataRows.size();
}

GrDataRow* GrDataRowList::GetDataRow(uint index) const
{
    if(index == INSERTION_ROW)
        return m_pGridCore->GetInsertionRow();
    return m_vecDataRows[index];
}

void GrDataRowList::Clear()
{
    for_each(_DataRows, m_vecDataRows, value)
    {
        value->SetDataRowIndex(INVALID_INDEX);
    }

    m_vecDataRowsRemoved.insert(m_vecDataRowsRemoved.end(), m_vecDataRows.begin(), m_vecDataRows.end());
    m_vecDataRows.clear();

    Update(true);
    m_pGridCore->Invalidate();
}

void GrDataRowList::DeleteObjects()
{
    for_each(_DataRows, m_vecDataRows, value)
    {
        delete value;
    }

    for_each(_DataRows, m_vecDataRowsRemoved, value)
    {
        delete value;
    }

    for_each(_GroupRows, m_vecGroupRows, value)
    {
        delete value;
    }

    m_vecDataRows.clear();
    m_vecDataRowsRemoved.clear();
    m_vecGroupRows.clear();
}

uint GrDataRowList::GetDisplayableRowCount() const
{
    return m_vecDisplayableRows.size();
}

IDataRow* GrDataRowList::GetDisplayableRow(uint index) const
{
    assert(index < m_vecDisplayableRows.size());
    return m_vecDisplayableRows[index];
}

int GrDataRowList::GetDisplayableBottom() const
{
    return m_displayableBottom;
}

IDataRow* GrDataRowList::HitTest(int y) const
{
    for_each(_IDataRows, m_vecDisplayableRows, value)
    {
        if(y >= value->GetY() && y < value->GetBottom())
            return value;
    }
    return NULL;
}

GrIndexRange GrDataRowList::HitTest(int y, IDataRow* pRowAnchor) const
{
    const uint anchorIndex = pRowAnchor->GetVisibleIndex();
    GrIndexRange indexRange;

    if(pRowAnchor->GetRowType() == GrRowType_InsertionRow)
    {
        indexRange.SetRange(INSERTION_ROW, INSERTION_ROW+1);
    }
    else
    {
        if(y >= GetBottom())
        {
            uint displayCount = GetDisplayableRowCount();
            IDataRow* pDataRow = GetDisplayableRow(displayCount - 1);
            indexRange.SetRange(anchorIndex, pDataRow->GetVisibleIndex() + 1);
        }
        else if(y < GetY())
        {
            IDataRow* pFirst = GetDisplayableRow(0);
            indexRange.SetRange(pFirst->GetVisibleIndex(), anchorIndex + 1);
        }
        else
        {
            uint index = 0;
            IDataRow* pDataRow = HitTest(y);
            if(pDataRow != NULL)
                index = pDataRow->GetVisibleIndex();

            if(index < anchorIndex)
                indexRange.SetRange(index, anchorIndex + 1);
            else
                indexRange.SetRange(anchorIndex, index + 1);
        } 
    }

    return indexRange;
}

void GrDataRowList::BringIntoView(IDataRow* pDataRow)
{
    if(pDataRow->ShouldBringIntoView() == false)
        return;

    GrRect dataRect = m_pGridCore->GetDataRect();
    GrScroll* pVertScroll = m_pGridCore->GetVertScroll();

    if(pDataRow->GetHeight() > dataRect.GetHeight())
    {
        pVertScroll->SetValue(pDataRow->GetVisibleIndex());
    }
    else
    {
        uint visibleIndex = pDataRow->GetVisibleIndex();

        uint newValue;
        if(visibleIndex < (uint)pVertScroll->GetValue())
        {
            newValue = visibleIndex;
        }
        else
        {
            newValue = ClipTo(visibleIndex);
        }
        pVertScroll->SetValue(newValue);
    }
    m_pGridCore->Invalidate();
}

GrRect GrDataRowList::GetBounds() const
{
    GrRect bound = m_bound;
    bound.right = m_pGridCore->GetColumnList()->GetBounds().right;
    return bound;
}

bool GrDataRowList::ShouldClip(const GrRect& displayRect, uint /*horizontal*/, uint vertical) const
{
    if(m_clippedIndex == vertical && displayRect.GetHeight() == m_clippedHeight)
        return false;
    return true;
}

void GrDataRowList::Clip(const GrRect& displayRect, uint /*horizontal*/, uint vertical)
{
    int displayY = GetY();

    for_each(_IDataRows, m_vecDisplayableRows, value)
    {
        value->SetDisplayable(false);
        value->SetDisplayIndex(INVALID_INDEX);
        value->SetClipped(false);
    }
    m_vecDisplayableRows.clear();

    uint displayIndex = 0;
    for(uint i=vertical ; i<GetVisibleRowCount() ; i++)
    {
        IDataRow* pIDataRow = GetVisibleRow(i);

        if(displayY >= displayRect.bottom)
            break;

        pIDataRow->SetY(displayY);
        pIDataRow->SetDisplayable(true);
        pIDataRow->SetDisplayIndex(displayIndex);

        if(displayY + pIDataRow->GetHeight() >= displayRect.bottom)
            pIDataRow->SetClipped(true);
        else
            pIDataRow->SetClipped(false);

        m_vecDisplayableRows.push_back(pIDataRow);
        displayY += pIDataRow->GetHeight();
        displayIndex++;
    }

    m_bound.left = GetX();
    m_bound.top = GetY();
    m_bound.right = GetRight();
    m_bound.bottom = displayY;

    m_displayableHeight = displayY - GetY();
    m_displayableBottom = std::min(displayRect.bottom, displayY);

    if(m_clippedHeight != displayRect.GetHeight() || m_clippedIndex != vertical)
        UpdateVertScroll(displayRect);

    m_clippedHeight = displayRect.GetHeight();
    m_clippedIndex = vertical;
}

void GrDataRowList::UpdateVertScroll(const GrRect& displayRect)
{
    GrScroll* pVertScroll = m_pGridCore->GetVertScroll();

    if(pVertScroll->GetVisible() == false)
        return;

    const uint visibleRowCount = GetVisibleRowCount();
    const int visibleHeight = m_visibleBottom - GetY();
    const int displayableHeight = displayRect.bottom - GetY();

    int countPerPage = 0;
    int tempHeight = displayableHeight;
    for(uint i=visibleRowCount-1 ; i<visibleRowCount ; i--)
    {
        const IDataRow* pDataRow = GetVisibleRow(i);
        int rowHeight = pDataRow->GetHeight();
        if(rowHeight < tempHeight)
        {
            countPerPage++;
            tempHeight -= rowHeight;
        }
        else
        {
            break;
        }
    }
    countPerPage = std::max(1, countPerPage);

    int largeChange = (int)((float)visibleRowCount * (float)displayableHeight / (float)visibleHeight);
    largeChange = std::max(1, largeChange);

    pVertScroll->SetMinimum(0);
    pVertScroll->SetMaximum(visibleRowCount - countPerPage + largeChange - 1);
    pVertScroll->SetLargeChange(largeChange);
}

uint GrDataRowList::ClipFrom(uint visibleFrom) const
{
    return ClipFrom(m_pGridCore->GetDisplayRect(), visibleFrom);
}

uint GrDataRowList::ClipFrom(const GrRect& displayRect, uint visibleFrom) const
{
    int displayHeight = displayRect.GetHeight() - GetY();
    int displayY = displayHeight;
    uint visibleTo = visibleFrom;

    for(uint i=visibleFrom ; i<GetVisibleRowCount() ; i++)
    {
        IDataRow* pDataRow = GetVisibleRow(i);
        displayY -= pDataRow->GetHeight();
        if(displayY <= 0)
            break;
        visibleTo = i;
    }
    return visibleTo;
}

uint GrDataRowList::ClipTo(uint visibleTo) const
{
    return ClipTo(m_pGridCore->GetDisplayRect(), visibleTo);
}

uint GrDataRowList::ClipTo(const GrRect& displayRect, uint visibleTo) const
{
    int displayHeight = displayRect.GetHeight() - GetY();
    int displayY = displayHeight;
    uint visibleFrom = visibleTo;

    for(uint i=visibleTo ; i<GetVisibleRowCount() ; i--)
    {
        IDataRow* pDataRow = GetVisibleRow(i);
        displayY -= pDataRow->GetHeight();
        if(displayY <= 0)
            break;
        visibleFrom = i;
    }
    return visibleFrom;
}

void GrDataRowList::SetZeroBasedRowIndex(bool b)
{
    if(m_zeroBasedRowIndex == b)
        return;

    m_zeroBasedRowIndex = b;

    uint index = 0;
    for_each(_DataRows, m_vecDataRows, value)
    {
        value->SetDataRowIndex(index);
    }
}

bool GrDataRowList::GetZeroBasedRowIndex() const
{
    return m_zeroBasedRowIndex;
}

bool GrDataRowList::GetRowNumberVisible() const
{
    return m_visibleRowNumber;
}

void GrDataRowList::SetRowNumberVisible(bool b)
{
    m_visibleRowNumber = b;
    m_pGridCore->Invalidate();
}

void GrDataRowList::Sort(GrColumn* pColumn)
{
    FuncSortRow fnSort = (FuncSortRow)pColumn->GetSortComparer(pColumn->GetSortType());
    if(fnSort == NULL)
    {
        switch(pColumn->GetSortType())
        {
        case GrSort_Up:

            fnSort = GrSortFunc::SortDataRowSortUp;
            break;
        case GrSort_Down:
            fnSort = GrSortFunc::SortDataRowSortDown;
            break;
        default:
            fnSort = GrSortFunc::SortDataRowSortNone;
            break;
        }
    }

    if(m_groupCount == 0)
    {
        GrRow::Sort(fnSort, (void*)pColumn);
    }
    else
    {
        for(uint i=0 ; i<m_usedGroupRow ; i++)
        {
            GrGroupRow* pGroupRow = m_vecGroupRows[i];
            if(pGroupRow->GetGroupLevel() == m_groupCount - 1)
                pGroupRow->Sort(fnSort, (void*)pColumn);
        }
    }
    SetVisibleChanged();
}

GrCell* GrDataRowList::HitTest(const GrPoint& location) const
{
    GrCell* pHitted = GrUpdatableRow::HitTest(location);
    if(pHitted == NULL)
        return NULL;

    for_each(_IDataRows, m_vecDisplayableRows, value)
    {
        GrCell* pSubHitted = value->HitTest(location);
        if(pSubHitted != NULL)
            return pSubHitted;
    }

    return NULL;
}

GrColumnSplitter::GrColumnSplitter(GrColumnList* pColumnList) : m_pColumnList(pColumnList)
{
    SetTextVisible(false);
    SetText(L"ColumnSplitter");

    m_visible = true;
}

int GrColumnSplitter::GetX() const
{
    return m_x; 
}

int GrColumnSplitter::GetY() const
{ 
    return m_pColumnList->GetY(); 
}

int GrColumnSplitter::GetWidth() const
{ 
    return DefaultSplitterWidth;
}

int GrColumnSplitter::GetHeight() const
{
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    return pDataRowList->GetDisplayableBottom() - m_pColumnList->GetY();
}

bool GrColumnSplitter::GetVisible() const
{
    return m_visible;
}

void GrColumnSplitter::SetVisible(bool b)
{
    if(m_visible == b)
        return;
    m_visible = b;

    m_pColumnList->SetVisibleChanged();
}

bool GrColumnSplitter::GetDisplayable() const 
{
    return true;
}

void GrColumnSplitter::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const 
{
    pPainter->DrawItem(0, GetRect(), GrColor::White);
}

GrGroupHeader::GrGroupHeader(GrGroupRow* pRow) : m_pRow(pRow)
{

}

int GrGroupHeader::HitMouseOverTest(const GrPoint& localLocation) const
{
    if(GrCell::HitMouseOverTest(localLocation) == 0)
        return 0;

    if(localLocation.x < DEF_GROUP_WIDTH)
        return GrMouseOverState_Control;
    return GrMouseOverState_In;
}

GrFlag GrGroupHeader::ToPaintStyle() const
{
    GrFlag flag = GrCell::ToPaintStyle();

    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->Get() == this)
        flag += GrPaintStyle_Focused;
    return flag;
}

GrRow* GrGroupHeader::GetRow() const 
{
    return m_pRow; 
}

IDataRow* GrGroupHeader::GetDataRow() const
{
    return m_pRow; 
}

bool GrGroupHeader::GetDisplayable() const
{
    return m_pRow->GetDisplayable();
}

int GrGroupHeader::GetX() const
{
    return m_pRow->GetRect().right + (m_pRow->GetGroupLevel() * DEF_GROUP_WIDTH);
}

int GrGroupHeader::GetY() const
{
    return m_pRow->GetY();
}

int GrGroupHeader::GetWidth() const
{
    return m_pGridCore->GetBounds().right - GetX();
}

int GrGroupHeader::GetHeight() const
{
    return m_pRow->GetHeight();
}

GrHorzAlign GrGroupHeader::GetTextHorzAlign() const
{
    return GrHorzAlign_Left;
}

GrVertAlign GrGroupHeader::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

void GrGroupHeader::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
{
    GrColumn* pColumn = m_pRow->GetColumn();
    GrRect paintRect = GetRect();
    GrRect displayRect = m_pGridCore->GetDisplayRect();

    paintRect.left += DEF_GROUP_WIDTH;
    paintRect.right = std::min(paintRect.right, displayRect.right);

    GrFlag paintStyle = ToPaintStyle();
    GrColor backColor = pColumn->GetPaintingBackColor();
    GrColor foreColor = pColumn->GetPaintingForeColor();

    pPainter->DrawRow(paintStyle, paintRect, backColor);

    DrawText(pPainter, foreColor, paintRect);

    GrRect glyphRect = paintRect;
    glyphRect.left = GetX();
    glyphRect.right = GetX() + DEF_GROUP_WIDTH;

    pPainter->DrawTreeGlyph(glyphRect, m_pRow->IsExpanded());
}

GrGroup::GrGroup(GrColumn* pColumn) 
: m_pColumn(pColumn)
{
    m_pGroupPanel = NULL;
    m_grouped = false;
    m_expanded = true;
    m_sortType = GrSort_Up;
    m_level = INVALID_INDEX;
}

void GrGroup::OnGridCoreAttached()
{
    GrCell::OnGridCoreAttached();
    m_pGroupPanel = m_pGridCore->GetGroupPanel();
}

void GrGroup::OnGridCoreDetached()
{
    m_pGroupPanel = NULL;
    GrCell::OnGridCoreDetached();
}

bool GrGroup::GetGrouped() const
{
    return m_pColumn->GetGrouped();
}

void GrGroup::SetGrouped(bool b)
{
    m_pColumn->SetGrouped(b);
}

void GrGroup::SetExpanded(bool b)
{
    if(m_pGroupPanel->GetGroupable() == false)
        return;
    m_expanded = b;
    m_pGroupPanel->NotifyExpanded(this);
}

bool GrGroup::GetExpanded() const
{
    return m_expanded;
}

GrSort GrGroup::GetSortType() const
{
    return m_sortType;
}

void GrGroup::SetSortType(GrSort sortType)
{
    if(m_pGroupPanel->GetGroupable() == false)
        return;
    m_sortType = (sortType == GrSort_Up) ? GrSort_Up : GrSort_Down;
    m_pGroupPanel->NotifySortChanged(this);
}

uint GrGroup::GetGroupLevel() const
{
    return m_level;
}

void GrGroup::SetGroupLevel(uint level)
{
    if(m_level == level)
        return;

    m_level = level;
    if(GetGrouped() == true)
        LevelChanged(this, &GrEventArgs::Empty);
}

void GrGroup::SetGroupLevelCore(uint level)
{
    m_level = level;
}

void GrGroup::SetText()
{
    GrCell::SetText(m_pColumn->GetText());
}

int GrGroup::GetX() const
{
    return m_pt.x;
}

int GrGroup::GetY() const
{
    return m_pt.y;
}

int GrGroup::GetWidth() const
{
    return GetTextBounds().GetWidth() + 
        (int)(GetPaintingFont()->GetHeight() * 0.25f) + 
        GetPadding().GetHorizontal() + SortGlyphSize;
}

int GrGroup::GetHeight() const
{
    return (int)(GetPaintingFont()->GetHeight() * 1.25f) + GetPadding().GetVertical();
}

GrHorzAlign GrGroup::GetTextHorzAlign() const
{
    return GrHorzAlign_Left;
}

GrVertAlign GrGroup::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

bool GrGroup::GetDisplayable() const
{
    return m_pGroupPanel->GetDisplayable();
}

void GrGroup::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
{
    GrRect paintRect = GetRect();
    GrFlag paintStyle = ToPaintStyle();

    GrColor backColor = m_pColumn->GetPaintingBackColor();
    GrColor foreColor = m_pColumn->GetPaintingForeColor();
    GrPadding padding = GetPadding();

    pPainter->DrawColumn(paintStyle, paintRect, backColor);

    GrRect sortRect;
    sortRect.right = paintRect.right - padding.right;
    sortRect.left = sortRect.right - SortGlyphSize;
    sortRect.top = (paintRect.bottom + paintRect.top - SortGlyphSize) / 2;
    sortRect.bottom = sortRect.top + SortGlyphSize;

    pPainter->DrawSortGlyph(sortRect, m_sortType);
    DrawText(pPainter, foreColor, paintRect);
}

void GrGroup::SetPosition(GrPoint pt)
{
    m_pt = pt;
}

GrColumn* GrGroup::GetColumn() const
{
    return m_pColumn;
}

GrRootRow::GrRootRow()
{
    m_width = 0;
    m_height = 0;
    SetText(L"Root");
    m_visibleChanged = true;
    m_fitChanged = false;
    m_heightChanged = false;
}

void GrRootRow::Update(bool force)
{
    for_each(_Updatables, m_vecUpdatables, value)
    {
        value->Update(force);
    }

    if(m_visibleChanged == true || force == true)
        BuildVisibleList();

    if(m_fitChanged == true || force == true)
        AdjustRowHeight();

    if(m_heightChanged == true || force == true)
        RepositionVisibleList();

    m_width = m_pColumnList->GetBounds().GetWidth();

    m_heightChanged = false;
    m_visibleChanged = false;
    m_fitChanged = false;
}

bool GrRootRow::ShouldClip() const
{
    return true;
}

void GrRootRow::Clip(const GrRect& displayRect, uint horizontal, uint vertical)
{
    m_bound.SetLocation(displayRect.GetLocation());
    m_bound.right = displayRect.left;
    m_bound.bottom = displayRect.top;

    for_each(_Updatables, m_vecUpdatables, value)
    {
        if(value->ShouldClip(displayRect, horizontal, vertical) == true)
            value->Clip(displayRect, horizontal, vertical);
    }

    for_each(_Updatables, m_vecUpdatables, value)
    {
        m_bound.right = std::max(value->GetBounds().right, m_bound.right);
        m_bound.bottom = std::max(value->GetBounds().bottom, m_bound.bottom);
    }
}

void GrRootRow::BuildVisibleList()
{
    m_vecVisibleRows.clear();

    for(uint i=0 ; i<GetChildCount() ; i++)
    {
        GrRow* pChild = GetChild(i);
        if(pChild->GetVisible() == true)
            m_vecVisibleRows.push_back(pChild);
    }
    m_heightChanged = true;
}

void GrRootRow::AdjustRowHeight()
{
    for(uint i=0 ; i<GetChildCount() ; i++)
    {
        GrRow* pChild = GetChild(i);
        pChild->AdjustHeight();
    }
    m_heightChanged = true;
}

void GrRootRow::RepositionVisibleList()
{
    int y = GetY();
    for_each(_Rows, m_vecVisibleRows, value)
    {
        value->SetY(y);
        y += value->GetHeight();
    }
    m_height = y - GetY();
}

void GrRootRow::SetVisibleChanged()
{
    m_visibleChanged = true;
    m_pGridCore->Invalidate();
}

void GrRootRow::SetFitChanged()
{
    m_fitChanged = true;
}

void GrRootRow::SetHeightChanged()
{
    m_heightChanged = true;
}

int GrRootRow::GetX() const
{
    return m_pGridCore->GetDisplayRect().left;
}

int GrRootRow::GetY() const
{
    return m_pGridCore->GetDisplayRect().top;
}

void GrRootRow::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    for_each(_Rows, m_vecVisibleRows, value)
    {
        value->Paint(pPainter, clipRect);
    }
}

GrCell* GrRootRow::HitTest(const GrPoint& location) const
{
    GrCell* pHitted = GrRow::HitTest(location);
    if(pHitted == NULL)
        return NULL;

    for_each(_Rows, m_vecVisibleRows, value)
    {
        GrCell* pSubHitted = value->HitTest(location);
        if(pSubHitted != NULL)
            return pSubHitted;
    }

    return NULL;
}

void GrRootRow::OnGridCoreAttached()
{
    GrRow::OnGridCoreAttached();

    m_pGridCore->Created.Add(this, &GrRootRow::gridCore_Created);
}

void GrRootRow::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrRow::OnGridCoreAttached();

    for(uint i=0 ; i<GetChildCount() ; i++)
    {
        GrUpdatableRow* pUpdatableRow = dynamic_cast<GrUpdatableRow*>(GetChild(i));
        if(pUpdatableRow == NULL)
            continue;
        m_vecUpdatables.push_back(pUpdatableRow);
    }

    std::sort(m_vecUpdatables.begin(), m_vecUpdatables.end(), SortUpdatable());

    m_pColumnList = m_pGridCore->GetColumnList();
}

bool GrSortFunc::SortRowsDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData)
{
    return SortRowsUp(pGridCore, pRow2, pRow1, userData);
}

bool GrSortFunc::SortRowsUp(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*userData*/)
{
    int result = pRow1->GetText().compare(pRow2->GetText());
    if(result == 0)
        return pRow1->GetID() < pRow2->GetID();
    return result < 0 ? true : false;
}

bool GrSortFunc::SortRowsNone(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*userData*/)
{
    return pRow1->GetID() < pRow2->GetID();
}

bool GrSortFunc::SortDataRowSortDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData)
{
    return SortDataRowSortUp(pGridCore, pRow2, pRow1, userData);
}

bool GrSortFunc::SortDataRowSortNone(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*userData*/)
{
    GrDataRow* pDataRow1 = (GrDataRow*)pRow1;
    GrDataRow* pDataRow2 = (GrDataRow*)pRow2;
    return pDataRow1->GetDataRowIndex() < pDataRow2->GetDataRowIndex();
}

bool GrSortFunc::SortDataRowSortUp(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* userData)
{
    if(pRow1->GetRowType() == pRow2->GetRowType())
    {
        if(pRow1->GetRowType() == GrRowType_DataRow)
        {
            GrColumn* pColumn = (GrColumn*)userData;
            const GrItem* pItem1= ((const GrDataRow*)pRow1)->GetItem(pColumn);
            const GrItem* pItem2 = ((const GrDataRow*)pRow2)->GetItem(pColumn);
            return pItem1->GetText().compare(pItem2->GetText()) < 0 ? true : false;
        }
        else if(pRow1->GetRowType() == GrRowType_GroupRow)
        {
            return pRow1->GetText().compare(pRow2->GetText()) < 0 ? true : false;
        }
        else
        {
            return pRow1->GetID() < pRow2->GetID();
        }
    }
    return pRow1->GetRowType() < pRow2->GetRowType();
}

bool GrUpdatableRow::ShouldUpdate() const
{
    return false;
}

void GrUpdatableRow::Update(bool /*force*/)
{

}

int GrUpdatableRow::GetUpdatePriority() const
{
    return INT_MAX; 
}

bool GrUpdatableRow::ShouldClip(const GrRect& /*displayRect*/, uint /*horizontal*/, uint /*vertical*/) const
{
    return false; 
}

void GrUpdatableRow::Clip(const GrRect& /*displayRect*/, uint /*horizontal*/, uint /*vertical*/)
{

}

int GrUpdatableRow::GetClipPriority() const
{
    return INT_MAX; 
}

void GrUpdatableRow::OnFitted()
{
    GrRow::OnFitted();

    if(m_pGridCore == NULL)
        return;
    m_pGridCore->GetRootRow()->SetFitChanged();
}

void GrUpdatableRow::OnHeightChanged()
{
    GrRow::OnHeightChanged();

    if(m_pGridCore == NULL)
        return;
    m_pGridCore->GetRootRow()->SetHeightChanged();
    m_pGridCore->Invalidate();
}