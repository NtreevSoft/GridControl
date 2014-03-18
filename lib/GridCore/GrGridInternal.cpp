//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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


#include "GrGridInternal.h"
#include "GrGridPainter.h"
#include "GrGridCore.h"
#include "GrColumnList.h"
#include <assert.h>
#include <math.h>
#include <memory.h>
#include <limits.h>

void GrTextUtil::SingleLine(GrLineDesc* pLine, const std::wstring& cellText, const GrFont* pFont)
{
    memset(pLine, 0, sizeof(GrLineDesc));
    pLine->length = (int)cellText.length();
    pLine->width = pFont->GetStringWidth(cellText);
}

void GrTextUtil::MultiLine(_TextLines* pLines, const std::wstring& cellText, int cellWidth, const GrFont* pFont, bool wordWrap)
{
    if(wordWrap == true)
        DoMultilineWordWrap(pLines, cellText, cellWidth, pFont);
    else
        DoMultiline(pLines, cellText, pFont);
}

void GrTextUtil::DoMultiline(_TextLines* pLines, const std::wstring& cellText, const GrFont* pFont)
{
    uint pos = 0;
    GrLineDesc cl;
    cl.width = 0;
    cl.length = 0;
    cl.textBegin = 0;

    while(pos != cellText.length())
    {
        wchar_t s = cellText.at(pos++);
        int charWidth = pFont->GetCharacterWidth(s);
        if(s == L'\n')
        {
            pLines->push_back(cl);
            cl.textBegin = pos;
            cl.length = 0;
            cl.width = 0;
        }
        else 
        {
            cl.width += charWidth;
            cl.length++;
        }
    }

    pLines->push_back(cl);
}

void GrTextUtil::WordWrap(WordList* pWordList, const std::wstring& cellText, const GrFont* pFont, int cellWidth)
{
    bool wordBreak = false;

    GrWordDesc wd;
    uint pos = 0;
    memset(&wd, 0, sizeof(GrWordDesc));

    while(pos != cellText.length())
    {
        wchar_t s = cellText.at(pos);
        int width = pFont->GetCharacterWidth(s);

        if((wordBreak == true && s != L' ') || wd.width + width > cellWidth || s > 0xff || s == L'\n')
        {
            pWordList->push_back(wd);
            memset(&wd, 0, sizeof(GrWordDesc));
            wd.pos = pos;
            wordBreak = false;
        }

        wd.length++;
        wd.width += width;

        if(s == L' ')
            wordBreak = true;
        else
            wd.validWidth += width;

        pos++;
    }

    if(wd.length != 0)
    {
        pWordList->push_back(wd);
    }
}

void GrTextUtil::DoMultilineWordWrap(_TextLines* pLines, const std::wstring& cellText, int cellWidth, const GrFont* pFont)
{
    WordList words;
    WordWrap(&words, cellText, pFont, cellWidth);

    int pos=0;
    GrLineDesc cl;
    memset(&cl, 0, sizeof(GrLineDesc));
    for(auto value : words)
    {
        if(cl.width + value.validWidth > cellWidth || cellText.at(value.pos) == L'\n')
        {
            pLines->push_back(cl);
            memset(&cl, 0, sizeof(GrLineDesc));
            cl.textBegin = pos;
        }

        cl.width += value.width;
        cl.length += value.length;
        pos += value.length;
    }

    pLines->push_back(cl);
}

GrSelectionTimer::GrSelectionTimer()
{
    m_pTimer = nullptr;
    m_horzEnable = true;
    m_vertEnable = true;

    m_horzScroll = 0;
    m_vertScroll = 0;

    m_scrollAcceleration = true;
    m_intervalAcceleration = true;
}

GrSelectionTimer::~GrSelectionTimer()
{
    if(m_pTimer != nullptr)
        m_pGridCore->GetGridWindow()->DestroyTimer(m_pTimer);
}

bool GrSelectionTimer::DoScroll()
{
    bool result = false;
    if(GetHScrollEnabled() == true)
    {
        int newHorz = m_pHorzScroll->GetValue() + m_horzScroll;
        if(m_pHorzScroll->DoScroll(newHorz) == true)
            result = true;
    }

    if(GetVScrollEnabled() == true)
    {
        int newVert = m_pVertScroll->GetValue() + m_vertScroll;
        if(m_pVertScroll->DoScroll(newVert) == true)
            result = true;
    }
    return result;
}

int GrSelectionTimer::ComputeSpeed(int pos, int length) const
{
    if(length == 0)
        throw _Exception("DivideByZeroException");
    else if(length == 1)
        return 3;
    return (int)ceilf( (pos / (float)length) * 3.0f);
}

void GrSelectionTimer::SetMouseLocation(GrPoint point)
{
    GrRect inside = GetInsideRectangle();
    GrRect outside = GetOutsideRectangle();

    m_vertScroll = 0;
    m_horzScroll = 0;

    if(outside.Contains(point) == false)
        return;

    if(GetHScrollEnabled() == true)
    {
        if(point.x < inside.left && point.x >= outside.left)
        {
            if(m_pHorzScroll->GetValue() > m_pHorzScroll->GetMinimum())
            {
                m_horzScroll = -ComputeSpeed(inside.left - point.x, inside.left - outside.left);
            }
        }
        else if(point.x >= inside.right && point.x < outside.right)
        {
            if(m_pHorzScroll->GetValue() < m_pHorzScroll->GetMaximum() - m_pHorzScroll->GetLargeChange() + 1)
            {
                m_horzScroll = ComputeSpeed(point.x - inside.right, outside.right - inside.right);
            }
        } 
    }

    if(GetVScrollEnabled() == true)
    {
        if(point.y < inside.top && point.y >= outside.top)
        {
            if(m_pVertScroll->GetValue() > m_pVertScroll->GetMinimum())
            {
                m_vertScroll = -ComputeSpeed(inside.top - point.y, inside.top - outside.top);
            }
        }
        else if(point.y >= inside.bottom && point.y < outside.bottom)
        {
            if(m_pVertScroll->GetValue() < m_pVertScroll->GetMaximum() - m_pVertScroll->GetLargeChange() + 1)
            {
                m_vertScroll = ComputeSpeed(point.y - inside.bottom, outside.bottom - inside.bottom);
            }
        } 
    }

    int speed = std::max(abs(m_horzScroll), abs(m_vertScroll));
    if(m_scrollAcceleration == false)
        speed = 1;
    time_t interval;
    switch(speed)
    {
    case 1:
        interval = 200;
        break;
    case 2:
        interval = 125;
        break;
    default:
        interval = 50;
        break;
    }

    if(m_intervalAcceleration == false)
        interval = 200;

    m_pTimer->SetInterval(interval);
}

bool GrSelectionTimer::GetHScrollEnabled() const
{
    return m_horzEnable && m_pHorzScroll->GetVisible() == true;
}

void GrSelectionTimer::SetHScrollEnabled(bool value)
{
    m_horzEnable = value;
}

bool GrSelectionTimer::GetVScrollEnabled() const
{
    return m_vertEnable && m_pVertScroll->GetVisible() == true; 
}

void GrSelectionTimer::SetVScrollEnabled(bool value)
{
    m_vertEnable = value;
}

bool GrSelectionTimer::CanHScroll() const
{
    return m_horzScroll != 0;
}

bool GrSelectionTimer::CanVScroll() const
{
    return m_vertScroll != 0;
}

bool GrSelectionTimer::GetScrollAcceleration() const
{
    return m_scrollAcceleration;
}

void GrSelectionTimer::SetScrollAcceleration(bool value)
{
    m_scrollAcceleration = value;
}

bool GrSelectionTimer::GetIntervalAcceleration() const
{
    return m_intervalAcceleration;
}

void GrSelectionTimer::SetIntervalAcceleration(bool value)
{
    m_intervalAcceleration = value;
}

GrRect GrSelectionTimer::GetInsideRectangle() const
{
    GrGridWindow* pGridWindow = m_pGridCore->GetGridWindow();
    return pGridWindow->ClientToScreen(m_pGridCore->GetDataRect());
}

GrRect GrSelectionTimer::GetOutsideRectangle() const
{
    GrGridWindow* pGridWindow = m_pGridCore->GetGridWindow();
    return pGridWindow->GetSrceenRect();
}

GrScroll* GrSelectionTimer::GetHorzScroll() const
{
    return m_pHorzScroll;
}

GrScroll* GrSelectionTimer::GetVertScroll() const 
{
    return m_pVertScroll;
}

void GrSelectionTimer::OnGridCoreAttached()
{
    GrObject::OnGridCoreAttached();

    GrGridWindow* pGridWindow = m_pGridCore->GetGridWindow();

    m_pHorzScroll = pGridWindow->GetHorzScroll();
    m_pVertScroll = pGridWindow->GetVertScroll();

    m_pTimer = pGridWindow->CreateTimer();
    m_pTimer->Elapsed.Add(this, &GrSelectionTimer::timer_Elapsed);

    m_pGridCore->AttachObject(m_pTimer);
}

void GrSelectionTimer::Start()
{
    m_pTimer->Start();
}

void GrSelectionTimer::Stop()
{
    m_pTimer->Stop();
}

void GrSelectionTimer::SetInterval(time_t interval)
{
    m_pTimer->SetInterval(interval);
}

void GrSelectionTimer::timer_Elapsed(GrObject* /*pSender*/, GrElapsedEventArgs* e)
{
    Elapsed.Raise(this, e);
}

GrFocuserInternal::GrFocuserInternal()
{
    m_pFocusing = nullptr;
}

void GrFocuserInternal::SetFocusing(IFocusable* pFocusable)
{
    if(m_pFocusing == pFocusable)
        return;

    if(m_pFocusing != nullptr && m_pFocusing->GetDisplayable() == true)
    {
		IDataRow* pDataRow = m_pFocusing->GetDataRow();
		if(m_pGridCore->GetRowHighlight() == true || m_pGridCore->GetFullRowSelect() == true)
		{
			GrRect rect = pDataRow->GetRect();
			rect.right = m_pGridCore->GetDisplayRect().right;
			rect.Expand(2);
			m_pGridCore->Invalidate(rect);
		}
		else
		{
			m_pFocusing->Invalidate();
		}
    }

    m_pFocusing = pFocusable;

    if(pFocusable != nullptr && pFocusable->GetDisplayable() == true)
    {
        pFocusable->Invalidate();
    }

    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    pItemSelector->Selecting(pFocusable);
}

void GrFocuserInternal::OnGridCoreAttached()
{
    GrFocuser::OnGridCoreAttached();
    m_pGridCore->Created.Add(this, &GrFocuserInternal::gridCore_Created);
    m_pGridCore->Cleared.Add(this, &GrFocuserInternal::gridCore_Cleared);
}

void GrFocuserInternal::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_pFocusing = nullptr;
}

void GrFocuserInternal::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    pItemSelector->SelectingEnd.Add(this, &GrFocuserInternal::itemSelector_SelectingEnd);
}

void GrFocuserInternal::OnFocusChanging(GrFocusChangeArgs* e)
{
    if(m_pGridCore->GetMultiSelect() == false)
    {
        IFocusable* pFocusable = e->GetFocusable();
        if(pFocusable != nullptr)
        {
            GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pFocusable->GetDataRow());
            GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
            if(pDataRow != nullptr && pDataRow->GetFullSelected() == true)
                pItemSelector->SelectDataRow(pDataRow, GrSelectionType_Normal);
            else
                pItemSelector->Select(pFocusable);
            
        }
    }

    GrFocuser::OnFocusChanging(e);
}

IFocusable* GrFocuserInternal::GetFocusing() const
{
    return m_pFocusing;
}

void GrFocuserInternal::itemSelector_SelectingEnd(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    if(m_pFocusing != nullptr)
    {
        Set(m_pFocusing);
        m_pFocusing = nullptr;
    }
}

GrItemSelectorInternal::GrItemSelectorInternal()
{
    m_bSelecting = false;
}

void GrItemSelectorInternal::BeginSelecting()
{
    if(m_bSelecting == true)
        EndSelecting(GrSelectionType_Normal);

    m_bSelecting = true;
    if(m_pGridCore->GetMultiSelect() == false)
        ClearSelection();
    OnSelectingBegin(&GrEventArgs::Empty);
}

void GrItemSelectorInternal::EndSelecting(GrSelectionType selectionType)
{
    if(m_bSelecting == false)
        return;

    GrItems items;
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

    for(uint y=m_rowSelecting.GetMinValue() ; y<m_rowSelecting.GetMaxValue() ; y++)
    {
        GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(y));
        if(pDataRow == nullptr)
            continue;
        for(uint x=m_columnSelecting.GetMinValue() ; x<m_columnSelecting.GetMaxValue() ; x++)
        {
            GrColumn* pColumn = pColumnList->GetVisibleColumn(x);
            GrItem* pItem = pDataRow->GetItem(pColumn);
            assert(pItem);
            items.insert(pItem);
        }
    }

    SelectItems(&items, selectionType);

    m_columnSelecting = GrIndexRange::Empty;
    m_rowSelecting = GrIndexRange::Empty;

    m_bSelecting = false;
    OnSelectingEnd(&GrEventArgs::Empty);
}

bool GrItemSelectorInternal::IsSelecting() const
{
    return m_bSelecting;
}

bool GrItemSelectorInternal::IsSelecting(const GrColumn* pColumn) const
{
    if(m_bSelecting == false)
        return false;
    return m_columnSelecting.IsIn(pColumn->GetVisibleIndex());
}

bool GrItemSelectorInternal::IsSelecting(const GrDataRow* pDataRow) const
{
    if(m_bSelecting == false)
        return false;
    return m_rowSelecting.IsIn(pDataRow->GetVisibleIndex());
}

GrRect GrItemSelectorInternal::GetInvalidateRect(GrIndexRange visibleColumnRange, GrIndexRange visibleRowRange) const
{
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

    if(visibleColumnRange.GetLength() == 0 || visibleRowRange.GetLength() == 0)
        return GrRect::Empty;

    GrRect rect(INT_MAX, INT_MAX, INT_MIN, INT_MIN);

    for(uint i=visibleColumnRange.GetMinValue() ; i<visibleColumnRange.GetMaxValue() ; i++)
    {
        GrColumn* pColumn = pColumnList->GetVisibleColumn(i);
        if(pColumn->GetDisplayable() == false)
            continue;
        GrRect displayRect = pColumn->GetRect();
        rect.left = std::min(rect.left, displayRect.left);
        rect.right = std::max(rect.right, displayRect.right);
    }

    for(uint y=visibleRowRange.GetMinValue() ; y<visibleRowRange.GetMaxValue() ; y++)
    {
        IDataRow* pDataRow = pDataRowList->GetVisibleRow(y);
        if(pDataRow->GetDisplayable() == false)
            continue;
        GrRect displayRect = pDataRow->GetRect();
        rect.top = std::min(rect.top, displayRect.top);
        rect.bottom = std::max(rect.bottom, displayRect.bottom);
    }
    return rect;
}

void GrItemSelectorInternal::Selecting(GrIndexRange visibleColumnRange, GrIndexRange visibleRowRange)
{
    if(m_bSelecting == false || m_pGridCore->GetMultiSelect() == false)
        return;

    if(m_columnSelecting != visibleColumnRange || m_rowSelecting != visibleRowRange)
    {
        GrRect rect = GetInvalidateRect(m_columnSelecting, m_rowSelecting);
        if(rect.IsEmpty() == false)
            m_pGridCore->Invalidate(rect);

        rect = GetInvalidateRect(visibleColumnRange, visibleRowRange);
        if(rect.IsEmpty() == false)
            m_pGridCore->Invalidate(rect);
    }

    SetColumnSelectingRange(visibleColumnRange);
    SetRowSelectingRange(visibleRowRange);
}

void GrItemSelectorInternal::SetColumnSelectingRange(GrIndexRange visibleRange)
{
    m_columnSelecting = visibleRange;
}

void GrItemSelectorInternal::SetRowSelectingRange(GrIndexRange visibleRange)
{
    m_rowSelecting = visibleRange;
}

void GrItemSelectorInternal::Select(IFocusable* pFocusable)
{
    GrItem* pFocusedItem = dynamic_cast<GrItem*>(pFocusable);
    if(pFocusedItem == nullptr)
        return;

    BeginSelection();
    ClearSelection();

    if(pFocusedItem != nullptr)
    {
        if(m_pGridCore->GetFullRowSelect() == true)
        {
            GrColumnList* pColumnList = m_pGridCore->GetColumnList();
            GrDataRow* pDataRow = pFocusedItem->GetDataRow();
            for(uint i=0 ; i<pColumnList->GetVisibleColumnCount() ; i++)
            {
                GrColumn* pColumn = pColumnList->GetVisibleColumn(i);
                GrItem* pItem = pDataRow->GetItem(pColumn);
                DoSelect(pItem);
            }
        }
        else
        {
            DoSelect(pFocusedItem);
        }
    }

    SetColumnSelectingRange(GrIndexRange::Empty);
    SetRowSelectingRange(GrIndexRange::Empty);

    EndSelection();
}

void GrItemSelectorInternal::Selecting(IFocusable* pFocusable)
{
    if(m_pGridCore->GetMultiSelect() == true)
        return;

    GrItem* pItem = dynamic_cast<GrItem*>(pFocusable);
    if(pItem != nullptr)
    {
        GrColumn* pColumn = pItem->GetColumn();
        GrDataRow* pDataRow = pItem->GetDataRow();

        uint visibleColumnIndex = pColumn->GetVisibleIndex();
        uint visibleRowIndex = pDataRow->GetVisibleIndex();

        if(m_pGridCore->GetFullRowSelect() == true)
        {
            GrColumnList* pColumnList = m_pGridCore->GetColumnList();
            SetColumnSelectingRange(GrIndexRange(0, pColumnList->GetVisibleColumnCount()));
        }
        else
        {
            SetColumnSelectingRange(GrIndexRange(visibleColumnIndex, visibleColumnIndex+1));
        }

        SetRowSelectingRange(GrIndexRange(visibleRowIndex, visibleRowIndex+1));
    }
    else
    {
        SetColumnSelectingRange(GrIndexRange::Empty);
        SetRowSelectingRange(GrIndexRange::Empty);
    }
}

void GrItemSelectorInternal::OnSelectingBegin(GrEventArgs* e)
{
    SelectingBegin(this, e);
}

void GrItemSelectorInternal::OnSelectingEnd(GrEventArgs* e)
{
    SelectingEnd(this, e);
}