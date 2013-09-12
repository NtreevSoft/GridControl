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


#include "GrColumnList.h"
#include "GrDataRowList.h"
#include "GrGridCore.h"
#include <assert.h>

//bool GrColumnList::SortColumnOnFreezable::operator () (const GrColumn* pColumn1, const GrColumn* pColumn2)
//{
//    if(pColumn1->GetFreezablePriority() == pColumn2->GetFreezablePriority())
//        return pColumn1->GetIndex() < pColumn2->GetIndex();
//    return pColumn1->GetFreezablePriority() < pColumn2->GetFreezablePriority();
//}
//
//bool GrColumnList::SortColumnOnUnfreezable::operator () (const GrColumn* pColumn1, const GrColumn* pColumn2)
//{
//    if(pColumn1->GetPriority() == pColumn2->GetPriority())
//        return pColumn1->GetIndex() < pColumn2->GetIndex();
//    return pColumn1->GetPriority() < pColumn2->GetPriority();
//}

GrColumnList::GrColumnList()
    : Columns(m_vecColumns), Visibles(m_vecVisibleColumns), Displayables(m_vecDisplayableColumns)
{
    m_pColumnSplitter = new GrColumnSplitter(this);

    SetTextVisible(false);
    SetText(L"ColumnList");

    m_frozenCount = 0;
    m_groupCount = 0;
    m_pSortColumn = nullptr;
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

    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    pDataRowList->DataRowRemoved.Add(this, &GrColumnList::dataRowList_DataRowRemoved);

    m_pGridCore->Cleared.Add(this, &GrColumnList::gridCore_Cleared);
    m_pGridCore->FontChanged.Add(this, &GrColumnList::gridCore_FontChanged);
    m_pGridCore->DisplayRectChanged.Add(this, &GrColumnList::gridCore_DisplayRectChanged);
}

GrColumnList::~GrColumnList()
{
    for(auto value : m_vecColumns)
    {
        delete value;
    }

    for(auto value : m_vecColumnsRemoved)
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

    //pColumn->m_freezablePriority = pColumn->GetIndex();
    //pColumn->m_priority = pColumn->GetIndex();

    m_pGridCore->AttachObject(pColumn);

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

    pColumn->SetSelected(false);
    pColumn->KillFocus();

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

GrColor GrColumnList::GetForeColor() const
{
    GrColor color = GrUpdatableRow::GetForeColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != nullptr)
        return pStyle->GetRowForeColor();

    return GrUpdatableRow::GetForeColor();
}

GrColor GrColumnList::GetBackColor() const
{
    GrColor color = GrUpdatableRow::GetBackColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != nullptr)
        return pStyle->GetRowBackColor();

    return GrUpdatableRow::GetBackColor();
}

GrColor GrColumnList::GetLineColor() const
{
    GrColor color = GrUpdatableRow::GetLineColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != nullptr)
        return pStyle->GetRowLineColor();

    return GrUpdatableRow::GetLineColor();
}

GrFont* GrColumnList::GetFont() const
{
    GrFont* pFont = GrUpdatableRow::GetFontCore();
    if(pFont != nullptr)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != nullptr)
        return pStyle->GetRowFont();

    return GrUpdatableRow::GetFont();
}

uint GrColumnList::GetColumnCount() const
{
    return m_vecColumns.size();
}

GrColumn* GrColumnList::HitTest(int x) const
{
    for(auto value : m_vecDisplayableColumns)
    {
        if(x >= value->GetX() && x < value->GetRight())
            return value;
    }
    return nullptr;
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

    Update();

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

    pHorzScroll->SetValue((int)newValue);
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
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    int x = pDataRowList->CellStart();

    for(auto value : m_vecDisplayableColumns)
    {
        value->SetDisplayable(false);
    }
    m_vecDisplayableColumns.clear();

    for(uint i=0 ; i<m_frozenCount ; i++)
    {
        GrColumn* pColumn = GetVisibleColumn(i);

        if(x > displayRect.right)
            break;

        pColumn->SetX(x);
        pColumn->SetDisplayable(true);
        pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());

        int width = pColumn->GetWidth();

        if(x + width > displayRect.right)
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

    m_pColumnSplitter->SetX(x);

    if(m_pColumnSplitter->GetVisible() == true)
        x += m_pColumnSplitter->GetWidth();

    for(uint i=m_frozenCount+horizontal ; i<GetVisibleColumnCount() ; i++)
    {
        GrColumn* pColumn = GetVisibleColumn(i);

        if(x > displayRect.right)
            break;

        pColumn->SetX(x);
        pColumn->SetDisplayable(true);
        pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());

        int width = pColumn->GetWidth();

        if(x + width > displayRect.right)
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
    pHorzScroll->SetMaximum((int)(unfrozenCount - countPerPage + largeChange - 1));
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

//bool GrColumnList::MoveToFrozen(GrColumn* pColumn, GrColumn* pWhere)
//{
//    if(pWhere && pWhere->GetFrozen() == false)
//        return false;
//
//    if(pColumn == pWhere)
//        return false;
//
//    _Columns vecFrozens;
//    vecFrozens.reserve(GetColumnCount());
//
//    for(auto value : m_vecColumns)
//    {
//        if(value == pColumn)
//            continue;
//        vecFrozens.push_back(value);
//    }
//
//    std::sort(vecFrozens.begin(), vecFrozens.end(), SortColumnOnFreezable());
//
//    _Columns::iterator itorWhere = std::find(vecFrozens.begin(), vecFrozens.end(), pWhere);
//    vecFrozens.insert(itorWhere, pColumn);
//    pColumn->m_frozen = true;
//
//    int priority = 0;
//    for(auto value : vecFrozens)
//    {
//        value->SetFreezablePriority(priority);
//        priority++;
//    }
//
//    SetVisibleChanged();
//
//    return true;
//}
//
//bool GrColumnList::MoveToUnfrozen(GrColumn* pColumn, GrColumn* pWhere)
//{
//    if(pWhere && pWhere->GetFrozen() == true)
//        return false;
//
//    if(pColumn == pWhere)
//        return false;
//
//    _Columns vecUnfrozens;
//    vecUnfrozens.reserve(GetColumnCount());
//
//    for(auto value : m_vecColumns)
//    {
//        if(value == pColumn)
//            continue;
//        vecUnfrozens.push_back(value);
//    }
//
//    std::sort(vecUnfrozens.begin(), vecUnfrozens.end(), SortColumnOnUnfreezable());
//
//    _Columns::iterator itorWhere = std::find(vecUnfrozens.begin(), vecUnfrozens.end(), pWhere);
//    vecUnfrozens.insert(itorWhere, pColumn);
//    pColumn->m_frozen = false;
//
//    int priority = 0;
//
//    for(auto value : vecUnfrozens)
//    {
//        value->SetPriority(priority);
//        priority++;
//    }
//
//    SetVisibleChanged();
//
//    return true;
//}

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

void GrColumnList::dataRowList_DataRowRemoved(GrObject* /*pSender*/, GrDataRowEventArgs* /*e*/)
{
    SetWidthChanged();
}

void GrColumnList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_groupCount = 0;
    m_pSortColumn = nullptr;
    m_frozenCount = 0;
    m_columnID = 0;

    if(m_pColumnSplitter->GetVisible() == true)
        m_displayableRight = m_pColumnSplitter->GetRight();
    else
        m_displayableRight = GetRight();

    for(auto value : m_vecColumns)
    {
        delete value;
    }

    for(auto value : m_vecColumnsRemoved)
    {
        value->SetColumnID(INVALID_INDEX);
    }

    m_vecColumns.clear();
    m_vecVisibleColumns.clear();
    m_vecDisplayableColumns.clear();
}

void GrColumnList::gridCore_FontChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrFont* pFont = GetPaintingFont();
    int height = (int)(pFont->GetHeight() + pFont->GetExternalLeading()) + GetPadding().GetVertical();
    SetHeight(height);
    for(auto value : m_vecColumns)
    {
        m_pGridCore->GetTextUpdater()->AddTextBounds(value);
    }
}

void GrColumnList::gridCore_DisplayRectChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    SetWidthChanged();
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
	else if(eventName.compare(L"ColumnVisibleIndexChanged") == 0)
        OnColumnVisibleIndexChanged((GrColumnEventArgs*)e);
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
    if(pHitted == nullptr)
        return nullptr;

    for(auto value : m_vecDisplayableColumns)
    {
        int x = value->GetX();
        if(location.x >= x && location.x < x + value->GetWidth())
            return value;
    }

    GrRect bound = m_pColumnSplitter->GetRect();
    if(bound.Contains(location) == true)
        return m_pColumnSplitter;

    if(ContainsHorz(location.x) == false)
        return nullptr;

    return pHitted;
}

void GrColumnList::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    GrRect paintRect = GetRect();
    GrFlag paintStyle = ToPaintStyle();

    pPainter->DrawRow(paintStyle, paintRect, GetPaintingLineColor(), GetPaintingBackColor());

    if(pDataRowList->GetMargin() != 0)
    {
        GrStyle* pStyle = m_pGridCore->GetStyle();
        
        GrColor backColor = pStyle != nullptr ? pStyle->GetColumnBackColor() : m_pGridCore->GetBackColor();
        GrColor lineColor = pStyle != nullptr ? pStyle->GetColumnLineColor() : m_pGridCore->GetLineColor();
        paintRect.left = paintRect.right;
        paintRect.right = pDataRowList->CellStart();
        pPainter->DrawColumn(GrPaintStyle_Default, paintRect, lineColor, backColor);
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

void GrColumnList::BuildVisibleColumnList()
{
    m_vecVisibleColumns.clear();
    m_vecDisplayableColumns.clear();

    m_frozenCount = 0;
    _Columns visibleCores, visibles;
    visibleCores.reserve(m_vecColumns.size());
    visibles.reserve(m_vecColumns.size());

    for(auto value : m_vecColumns)
    {
        value->SetDisplayable(false);
        value->SetDisplayIndex(INVALID_INDEX);
        value->SetClipped(false);

        if(value->GetVisible() == false)
            continue;

        if(value->GetFrozen() == true)
            m_frozenCount++;

        if(value->m_visibleIndex == INVALID_INDEX)
            visibleCores.push_back(value);
        else
            visibles.push_back(value);
        //m_vecVisibleColumns.push_back(value);
    }
    std::sort(visibleCores.begin(), visibleCores.end(), GrColumn::SortColumnByVisible());
    uint index = 0;
    for(auto value : visibleCores)
    {
        value->m_visibleIndexCore = index++;
    }

    m_vecVisibleColumns.insert(m_vecVisibleColumns.end(), visibleCores.begin(), visibleCores.end());
    m_vecVisibleColumns.insert(m_vecVisibleColumns.end(), visibles.begin(), visibles.end());

    std::sort(m_vecVisibleColumns.begin(), m_vecVisibleColumns.end(), GrColumn::SortColumnByVisible());
    index = 0;
    for(auto value : m_vecVisibleColumns)
    {
        value->m_visibleIndexCore = index++;
        value->m_visibleIndex = INVALID_INDEX;
    }
    m_widthChanged = true;
}

void GrColumnList::AdjustColumnWidth()
{
    for(auto value : m_vecColumns)
    {
        value->AdjustWidth();
    }
    m_widthChanged = true;
}

void GrColumnList::RepositionColumnList()
{
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    int x = pDataRowList->CellStart();

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

void GrColumnList::OnColumnVisibleIndexChanged(GrColumnEventArgs* e)
{
	SetVisibleChanged();
	ColumnVisibleIndexChanged(this, e);
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