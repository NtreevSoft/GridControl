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


#include "StdAfx.h"
#include "GrGridCell.h"
#include "GrGridCore.h"
#include "GrGridRenderer.h"
#include "GrGridInternal.h"

#include <assert.h>

IFocusable* IFocusable::Null = 0;
uint GrCell::m_snID = 0;

GrEventArgs GrEventArgs::Empty;

int GrRow::DefaultHeight = 21;
int	GrRow::DefaultMinHeight = 0;
int	GrRow::DefaultMaxHeight = 10000;

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

class SortUpdatable
{
public:
	bool operator () (const GrUpdatableRow* p1, const GrUpdatableRow* p2)
	{
		return p1->GetUpdatePriority() < p2->GetUpdatePriority();
	}
};

class SortClippable
{
public:
	bool operator () (const GrUpdatableRow* p1, const GrUpdatableRow* p2)
	{
		return p1->GetClipPriority() < p2->GetClipPriority();
	}
};

GrColumnList::GrColumnList()
{
	m_pColumnSplitter = new GrColumnSplitter(this);

	AddFlag(GRCF_SYSTEM_OBJCT | GRCF_HIDE_TEXT);
	//SetHeight(25);
	SetText(L"ColumnList");

	m_nFrozenCount			= 0;
	m_nGroupingCount		= 0;
	m_pSortColumn			= NULL;
	m_nDisplayableRight		= 0;
	m_nColumnID				= 0;

	m_bVisibleChanged		= false;
	m_bFitChanged			= false;
	m_bWidthChanged			= false;
	m_nClippedIndex			= INVALID_INDEX;

	SetResizable(false);
}

void GrColumnList::OnGridCoreAttached()
{
	GrRow::OnGridCoreAttached();
	m_pGridCore->AttachObject(m_pColumnSplitter);

	GrGroupingList* pGroupingList = m_pGridCore->GetGroupingList();
	pGroupingList->Changed.Add(this, &GrColumnList::groupingList_Changed);

	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
	pFocuser->FocusChanged.Add(this, &GrColumnList::gridCore_FocusChanged);

	m_pGridCore->Cleared.Add(this, &GrColumnList::gridCore_Cleared);
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
	return m_pGridCore->GetDataRowList()->GetRowWidth();
}

bool GrColumnList::ShouldUpdate() const
{
	return m_bVisibleChanged == true ||
		m_bFitChanged == true ||
		m_bWidthChanged	== true;
}

void GrColumnList::Update(bool force)
{
	if(m_bVisibleChanged == true || force == true)
		BuildVisibleColumnList();

	if(m_bFitChanged == true)
		AdjustColumnWidth();

	if(m_bWidthChanged == true)
		RepositionColumnList();	

	m_bVisibleChanged	= false;
	m_bFitChanged		= false;
	m_bWidthChanged		= false;
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

void GrColumnList::InsertColumn(GrColumn* pColumn, uint nIndex)
{
	if(pColumn->GetIndex() != INVALID_INDEX)
		throw _Exception("이미 등록되어 있습니다");

	if(pColumn->GetColumnID() == INVALID_INDEX)
	{
		pColumn->SetColumnID(m_nColumnID++);
	}
	
	_Columns::iterator itor = find(m_vecColumnsRemoved.begin(), m_vecColumnsRemoved.end(), pColumn);
	if(itor != m_vecColumnsRemoved.end())
		m_vecColumnsRemoved.erase(itor);

	itor = m_vecColumns.insert(m_vecColumns.begin() + nIndex, pColumn);
	nIndex = std::min(nIndex, m_vecColumns.size());
	for( ; itor != m_vecColumns.end() ; itor++)
	{
		(*itor)->SetIndex(nIndex++);
	}

	m_pGridCore->AttachObject(pColumn);

	if(m_vecVisibleColumns.size() == 0)
		pColumn->SetPriority(m_vecColumns.size());

	GrColumnEventArgs e(pColumn);
	OnColumnInserted(&e);

	pColumn->GroupingChanged.Add(this, &GrColumnList::column_GroupingChanged);
	assert(m_pGridCore->IsInvalidated() == true);
}

void GrColumnList::RemoveColumn(GrColumn* pColumn)
{
	_Columns::iterator itor = std::find(m_vecColumns.begin(), m_vecColumns.end(), pColumn);

	uint nIndex = pColumn->GetIndex();
	for(_Columns::iterator next = itor + 1 ; next != m_vecColumns.end() ; next++)
	{
		(*next)->SetIndex(nIndex);
		nIndex++;
	}

	m_pGridCore->DetachObject(pColumn);

	pColumn->SetIndex(INVALID_INDEX);

	m_vecColumns.erase(itor);
	m_vecColumnsRemoved.push_back(pColumn);

	pColumn->GroupingChanged.Remove(this, &GrColumnList::column_GroupingChanged);
	GrColumnEventArgs e(pColumn);
	OnColumnRemoved(&e);

	assert(m_pGridCore->IsInvalidated() == true);
}

GrRect GrColumnList::GetBound() const
{
	return GrRect(GetX(), GetY(), m_nVisibleRight, GetY() + GetHeight());
}

GrRect GrColumnList::GetDisplayBound() const
{
	return GrRect(GetDisplayX(), GetDisplayY(), m_nDisplayableRight, GetDisplayY() + GetHeight());
}

uint GrColumnList::GetColumnCount() const
{
	return m_vecColumns.size();
}

bool GrColumnList::HitTest(int x1, int x2, GrIndexRange* pRange) const
{
	assert(m_vecVisibleColumns.size());
	assert(x1 <= x2);

	_MapColumnPos::const_iterator itor = m_mapColumnByPosition.lower_bound(x1);
	if(itor == m_mapColumnByPosition.end())
		return false;

	uint nStart = (*itor).second->GetVisibleIndex();
	uint nEnd;

	itor = m_mapColumnByPosition.lower_bound(x2);
	if(itor == m_mapColumnByPosition.end())
		nEnd = this->GetVisibleColumnCount();
	else
		nEnd = (*itor).second->GetVisibleIndex() + 1;

	pRange->SetRange(nStart, nEnd);
	return true;
}

void GrColumnList::HitTest(GrRect rt, GrColumns* pTested) const
{
	_MapColumnPos::const_iterator itor = m_mapColumnByPosition.lower_bound(rt.left);
	if(itor == m_mapColumnByPosition.end())
		return;

	uint nStart = (*itor).second->GetVisibleIndex();
	uint nEnd;

	itor = m_mapColumnByPosition.lower_bound(rt.right);
	if(itor == m_mapColumnByPosition.end())
		nEnd = this->GetVisibleColumnCount();
	else
		nEnd = (*itor).second->GetVisibleIndex() + 1;

	for(uint i=nStart ; i<nEnd ; i++)
	{
		pTested->insert(GetVisibleColumn(i));
	}
}

GrColumn* GrColumnList::HitTest(int x) const
{
	_MapColumnPos::const_iterator itor = m_mapColumnByPosition.lower_bound(x);
	if(itor == m_mapColumnByPosition.end())
		return NULL;

	GrColumn* pColumn = (*itor).second;
	if(x < pColumn->GetX())
		return NULL;

	return pColumn;
}

void GrColumnList::SetFitChanged()
{
	if(m_bFitChanged == true)
		return;
	m_bFitChanged = true;
	m_pGridCore->Invalidate();
}

void GrColumnList::SetVisibleChanged()
{
	if(m_bVisibleChanged == true)
		return;
	m_bVisibleChanged = true;
	m_pGridCore->Invalidate();
}

GrColumn* GrColumnList::GetFirstSortColumn() const
{
	return m_pSortColumn;
}

bool GrColumnList::ShouldClip(const GrRect* pDisplayRect, uint horizontal, uint /*vertical*/) const
{
	if(m_nClippedIndex == horizontal && pDisplayRect->GetWidth() == m_nClippedWidth)
		return false;
	return true;
}

void GrColumnList::Clip(const GrRect* pDisplayRect, uint horizontal, uint /*vertical*/)
{
	GrRect rtDisplay	= *pDisplayRect;
	int nDisplayX		= GetDisplayX() + GetWidth() + m_pGridCore->GetGroupingMargin();

	for_each(_Columns, m_vecDisplayableColumns, value)
	{
		value->SetDisplayable(false);
	}
	m_vecDisplayableColumns.clear();

	for(uint i=0 ; i<m_nFrozenCount ; i++)
	{
		GrColumn* pColumn = GetVisibleColumn(i);
		pColumn->SetDisplayX(nDisplayX);
		pColumn->SetDisplayable(true);
		pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());
		pColumn->SetClipped(false);

		int width = pColumn->GetWidth();
		nDisplayX += width;

		m_vecDisplayableColumns.push_back(pColumn);
	}

	m_pColumnSplitter->m_nDisplayX = nDisplayX;

	if(m_pColumnSplitter->GetVisible() == true)
		nDisplayX += m_pColumnSplitter->GetWidth();

	for(uint i=m_nFrozenCount+horizontal ; i<GetVisibleColumnCount() ; i++)
	{
		GrColumn* pColumn = GetVisibleColumn(i);

		if(nDisplayX >= rtDisplay.right)
			break;

		pColumn->SetDisplayX(nDisplayX);
		pColumn->SetDisplayable(true);
		pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());

		int width = pColumn->GetWidth();

		if(nDisplayX + width >= rtDisplay.right)
			pColumn->SetClipped(true);
		else
			pColumn->SetClipped(false);

		m_vecDisplayableColumns.push_back(pColumn);
		nDisplayX += width;
	}

	m_nDisplayableRight = std::min(pDisplayRect->right, nDisplayX);
	m_nClippedIndex = horizontal;
	m_nClippedWidth = pDisplayRect->GetWidth();
}

uint GrColumnList::ClipFrom(uint nVisibleFrom) const
{
	return ClipFrom(m_pGridCore->GetDisplayRect(), nVisibleFrom);
}

uint GrColumnList::ClipFrom(const GrRect* pDisplayRect, uint nVisibleFrom) const
{
	GrColumn* pColumn = GetUnfrozenColumn(nVisibleFrom);
	if(pColumn == NULL)
		throw _Exception("잘못된 인덱스");

	int x = pColumn->GetX() + (pDisplayRect->GetWidth() - m_nUnfrozenX);

	GrColumn* pLastColumn = HitTest(x);
	if(pLastColumn == NULL)
		return GrUtil::LastIndex(GetUnfrozenColumnCount());

	return pLastColumn->GetUnfrozenIndex();
}

uint GrColumnList::ClipTo(uint nVisibleTo) const
{
	return ClipTo(m_pGridCore->GetDisplayRect(), nVisibleTo);
}

uint GrColumnList::ClipTo(const GrRect* pDisplayRect, uint nVisibleTo) const
{
	int nDisplayWidth = pDisplayRect->GetWidth() - m_nUnfrozenX;
	int nDisplayX = nDisplayWidth;
	uint visibleFrom = nVisibleTo;

	for(uint i=nVisibleTo ; i>=0 && i<GetUnfrozenColumnCount() ; i--)
	{
		GrColumn* pColumn = GetUnfrozenColumn(i);
		nDisplayX -= pColumn->GetWidth();
		if(nDisplayX <= 0)
			break;
		visibleFrom = i;
	}
	return visibleFrom;
}

int GrColumnList::DisplayToGlobal(int x) const
{
	int nDisplayLeft = m_pGridCore->GetDisplayRect()->left;
	if(x < m_nUnfrozenX)
		return x - nDisplayLeft;

	if(GetDisplayableColumnCount() == 0)
		return x - nDisplayLeft;

	if(GetUnfrozenColumnCount() == 0)
		return x - nDisplayLeft;

	GrColumn* pColumn = GetDisplayableColumn(m_nFrozenCount);

	int nOffset = pColumn->GetX() - m_nUnfrozenX;
	x += nOffset;
	return x - nDisplayLeft;
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

	sort(vecFrozens.begin(), vecFrozens.end(), SortColumn());
	sort(vecUnfrozens.begin(), vecUnfrozens.end(), SortColumn());

	_Columns::iterator itorWhere = std::find(vecFrozens.begin(), vecFrozens.end(), pWhere);
	vecFrozens.insert(itorWhere, pColumn);
	pColumn->m_bFrozen = true;

	int nPriority = 0;
	for_each(_Columns, vecFrozens, value)
	{
		value->SetPriority(nPriority);
		nPriority++;
	}

	for_each(_Columns, vecUnfrozens, value)
	{
		value->SetPriority(nPriority);
		nPriority++;
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

	sort(vecFrozens.begin(), vecFrozens.end(), SortColumn());
	sort(vecUnfrozens.begin(), vecUnfrozens.end(), SortColumn());

	_Columns::iterator itorWhere = std::find(vecUnfrozens.begin(), vecUnfrozens.end(), pWhere);
	vecUnfrozens.insert(itorWhere, pColumn);
	pColumn->m_bFrozen = false;

	int nPriority = 0;
	for_each(_Columns, vecFrozens, value)
	{
		value->SetPriority(nPriority);
		nPriority++;
	}

	for_each(_Columns, vecUnfrozens, value)
	{
		value->SetPriority(nPriority);
		nPriority++;
	}

	SetVisibleChanged();

	return true;
}

void GrColumnList::column_GroupingChanged(GrObject* pSender, GrEventArgs* /*e*/)
{
	GrColumnEventArgs ce((GrColumn*)pSender);
	OnColumnGroupingChanged(&ce);
}

void GrColumnList::groupingList_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrGroupingList* pGroupingList = m_pGridCore->GetGroupingList();
	m_nGroupingCount = pGroupingList->GetGroupingCount();
	m_bWidthChanged = true;

	assert(m_pGridCore->IsInvalidated() == true);
}

void GrColumnList::gridCore_FocusChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{

}

void GrColumnList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	m_nGroupingCount = 0;
	m_pSortColumn    = NULL;
	m_nFrozenCount	 = 0;
	m_nColumnID		 = 0;
	m_nDisplayableRight = m_pColumnSplitter->m_nDisplayX;
	if(m_pColumnSplitter->GetVisible() == true)
		m_nDisplayableRight += m_pColumnSplitter->GetWidth();

	for_each(_Columns, m_vecColumns, value)
	{
		delete value;
	}

	for_each(_Columns, m_vecColumnsRemoved, value)
	{
		value->SetColumnID(INVALID_INDEX);
	}
	//m_vecColumnsRemoved.clear();
	
	m_vecColumns.clear();
	m_mapColumnByPosition.clear();
	m_vecVisibleColumns.clear();
	m_vecDisplayableColumns.clear();
}

void GrColumnList::NotifySortTypeChanged(GrColumn* pColumn)
{
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	m_pSortColumn = pColumn;
	pDataRowList->Sort(pColumn);

	m_pGridCore->Invalidate();
}

void GrColumnList::NotifyWidthChanged(GrColumn* pColumn)
{
	GrColumnEventArgs e(pColumn);
	OnColumnWidthChanged(&e);

	if(pColumn->GetDisplayIndex() != INVALID_INDEX)
	{
		m_pGridCore->Invalidate();
	}
	m_bWidthChanged = true;
}

void GrColumnList::NotifyFrozenChanged(GrColumn* pColumn)
{
	SetVisibleChanged();
	GrColumnEventArgs e(pColumn);
	OnColumnFrozenChanged(&e);
}

void GrColumnList::NotifyWordwrapChanged(GrColumn* pColumn)
{
	GrColumnEventArgs e(pColumn);
	OnColumnWordwrapChanged(&e);
}

void GrColumnList::NotifyMultilineChanged(GrColumn* pColumn)
{
	GrColumnEventArgs e(pColumn);
	OnColumnMultilineChanged(&e);
}

void GrColumnList::NotifyHorzAlignChanged(GrColumn* pColumn)
{
	GrColumnEventArgs e(pColumn);
	OnColumnHorzAlignChanged(&e);
}

void GrColumnList::NotifyVertAlignChanged(GrColumn* pColumn)
{
	GrColumnEventArgs e(pColumn);
	OnColumnVertAlignChanged(&e);
}

void GrColumnList::NotifyPaddingChanged(GrColumn* pColumn)
{
	GrColumnEventArgs e(pColumn);
	OnColumnPaddingChanged(&e);
}

GrCell* GrColumnList::HitTest(const GrPoint& pt) const
{
	GrCell* pHitted = GrUpdatableRow::HitTest(pt);
	if(pHitted == NULL)
		return NULL;
	
	GrColumn* pHittedColumn = HitTest(pt.x);
	if(pHittedColumn != NULL)
		return pHittedColumn;

	GrRect bound = m_pColumnSplitter->GetRect();
	if(bound.IsIn(pt) == true)
		return m_pColumnSplitter;

	return NULL;
}

GrCell* GrColumnList::HitDisplayTest(const GrPoint& pt) const
{
	GrCell* pHitted = GrUpdatableRow::HitDisplayTest(pt);
	if(pHitted == NULL)
		return NULL;
	
	for_each(_Columns, m_vecDisplayableColumns, value)
	{
		int x = value->GetDisplayX();
		if(pt.x >= x && pt.x < x + value->GetWidth())
			return value;
	}

	GrRect bound = m_pColumnSplitter->GetDisplayRect();
	if(bound.IsIn(pt) == true)
		return m_pColumnSplitter;

	return NULL;
}

void GrColumnList::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	GrRect rtRender = GetDisplayRect();
	GrFlag renderStyle = ToRenderStyle();

	pRenderer->DrawHeader(renderStyle, &rtRender);

	if(m_nGroupingCount > 0)
	{
		rtRender.left	= rtRender.right;
		rtRender.right	= CellStart();
		pRenderer->DrawHeader(0, &rtRender);
	}

	int nRight = rtRender.right;
	for(uint j=0 ; j<m_vecDisplayableColumns.size() ; j++)
	{
		const GrColumn* pColumn = m_vecDisplayableColumns[j];
		int x = pColumn->GetDisplayX();
		int r = x + pColumn->GetWidth();

		if(x < pClipping->right && r >= pClipping->left)
			pColumn->Render(pRenderer, pClipping);
		nRight = pColumn->GetDisplayX() + pColumn->GetWidth();
	}

	const GrRect* pDisplayRect = m_pGridCore->GetDisplayRect();
	if(m_pGridCore->GetMarginVisible() == true && nRight < pDisplayRect->right)
	{
		rtRender.left  = nRight;
		rtRender.right = pDisplayRect->right + 1;
		pRenderer->DrawCell(0, GrColor::White, &rtRender);
	}

	RenderSplitter(pRenderer, pClipping);
}

void GrColumnList::RenderSplitter(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	if(m_pColumnSplitter->GetVisible() == true)
		m_pColumnSplitter->Render(pRenderer, pClipping);
}

GrColumn* GrColumnList::GetColumn(uint nIndex) const
{
	assert(nIndex < m_vecColumns.size());
	return m_vecColumns[nIndex];
}

GrCell::GrCell() : m_padding(GrPadding::Default)
{
	m_dwFlag			= 0;
	m_nTextLineHeight	= 0;

	m_pFont				= NULL;
	m_nID				= m_snID++;

	m_bTextBound		= false;
	m_bTextAlign		= false;
	m_pTextUpdater		= NULL;

	Tag = NULL;
}

GrCell::~GrCell()
{

}

void GrCell::SetPadding(GrPadding padding)
{
	m_padding = padding;
	SetTextBoundChanged();
	Invalidate();
}

GrRect GrCell::GetRect() const
{
	GrRect rt;
	rt.left		= GetX();
	rt.top		= GetY();
	rt.right	= rt.left + GetWidth();
	rt.bottom	= rt.top  + GetHeight();

	return rt;
}

GrRect GrCell::GetClientRect() const
{
	GrRect rtClient;
	GrPadding padding = GetPadding();
	rtClient.left	= padding.left;
	rtClient.top	= padding.top;
	rtClient.right	= GetWidth()  - padding.right;
	rtClient.bottom	= GetHeight() - padding.bottom;
	return rtClient;
}

GrPoint GrCell::GetPosition() const
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

const GrLineDesc* GrCell::GetTextLine(uint nIndex) const
{
	return &(m_vecTextLine[nIndex]);
}

uint GrCell::GetTextLineHeight() const
{
	return m_nTextLineHeight;
}

const GrRect* GrCell::GetTextBound() const
{
	return &m_rtTextBound;
}

void GrCell::AlignText()
{
	GrPoint ptStart;
	GrPadding padding = GetPadding();
	const int nWidth  = GetWidth()  - (padding.left + padding.right);
	const int nHeight = GetHeight() - (padding.top  + padding.bottom);
	GrSize szTextBound = m_rtTextBound.GetSize();

	for(uint i=0 ; i<GetTextLineCount() ; i++)
	{
		GrLineDesc& cl = const_cast<GrLineDesc&>(*GetTextLine(i));

		switch(GetTextHorzAlign())
		{
		case GrHorzAlign_Left:
			ptStart.x = 0;
			break;
		case GrHorzAlign_Center:
			ptStart.x = (nWidth - cl.nWidth) / 2;
			break;
		case GrHorzAlign_Right:
			ptStart.x = nWidth - cl.nWidth;
			break;
		}

		switch(GetTextVertAlign())
		{
		case GrVertAlign_Top:
			ptStart.y = i * m_nTextLineHeight;
			break;
		case GrVertAlign_Center:
			ptStart.y = (nHeight - GetTextLineCount() * m_nTextLineHeight) / 2 + i * m_nTextLineHeight;
			break;
		case GrVertAlign_Bottom:
			ptStart.y = nHeight - ((i+1) * m_nTextLineHeight);
			break;
		}

		cl.x = ptStart.x + padding.left;
		cl.y = ptStart.y + padding.top;

		if(i==0)
		{
			m_rtTextBound.left = cl.x;
			m_rtTextBound.top  = cl.y;
		}
		else
		{
			m_rtTextBound.left = std::min(m_rtTextBound.left, cl.x);
			m_rtTextBound.top  = std::min(m_rtTextBound.top,  cl.y);
		}
	}

	if(szTextBound.width > GetWidth() || szTextBound.height > GetHeight())
		AddFlag(GRCF_TEXT_CLIPPED);
	else
		RemoveFlag(GRCF_TEXT_CLIPPED);

	m_rtTextBound.right	 = m_rtTextBound.left + szTextBound.width;
	m_rtTextBound.bottom = m_rtTextBound.top  + szTextBound.height;
}

void GrCell::ComputeTextBound()
{
	GrFont* pFont = GetRenderingFont();
	GrPadding padding = GetPadding();
	m_vecTextLine.clear();

	int nMaxWidth  = 0;
	int nMaxHeight = 0;
	GrRect rtOldTextBound = m_rtTextBound;

	if(m_strText.size() > 0)
	{
		int nCellWidth = GetWidth() - (padding.left + padding.right);
		//assert(nCellWidth > 0);

		if(GetTextMulitiline() == false)
		{
			GrLineDesc textLine;
			GrTextUtil::SingleLine(&textLine, m_strText, pFont);
			m_vecTextLine.push_back(textLine);
		}
		else
		{
			GrTextUtil::MultiLine(&m_vecTextLine, m_strText, nCellWidth, pFont, GetTextWordWrap());
		}

		for_each(std::vector<GrLineDesc>, m_vecTextLine, value)
		{
			const GrLineDesc& lineDesc = value;
			nMaxWidth = std::max(nMaxWidth, lineDesc.nWidth);
		}
	}

	m_nTextLineHeight = pFont->GetHeight();

	nMaxWidth += (padding.left + padding.right);
	nMaxHeight = padding.top + padding.bottom;

	if(m_vecTextLine.size() == 0)
		nMaxHeight += m_nTextLineHeight;
	else
		nMaxHeight += m_vecTextLine.size() * m_nTextLineHeight;

	if(nMaxWidth > GetWidth() || nMaxHeight > GetHeight())
		AddFlag(GRCF_TEXT_CLIPPED);
	else
		RemoveFlag(GRCF_TEXT_CLIPPED);
	
	m_rtTextBound = GrRect(0, 0, nMaxWidth, nMaxHeight);
	OnTextBoundChanged(&m_rtTextBound, &rtOldTextBound);
}

bool GrCell::IsTextVisible() const
{
	if(HasFlag(GRCF_TEXT_IS_NULL) == true)
		return false;
	if(HasFlag(GRCF_HIDE_TEXT) == true)
		return false;
	return true;
}

bool GrCell::IsTextClipped() const
{
	return HasFlag(GRCF_TEXT_CLIPPED);
}

void GrCell::RenderText(GrGridRenderer* pRenderer, GrColor foreColor, const GrRect* pDisplayRect, const GrRect* pClipping) const
{
	if(IsTextVisible() == false)
		return;

	GrRect rtText;
	for(uint i=0 ; i<GetTextLineCount() ; i++)
	{
		const GrLineDesc* cl = GetTextLine(i);
		rtText.left		= pDisplayRect->left + cl->x;
		rtText.top		= pDisplayRect->top  + cl->y;
		rtText.right	= pDisplayRect->left + cl->nWidth;
		rtText.bottom	= pDisplayRect->top  + GetTextLineHeight();
		if(rtText.top > pDisplayRect->bottom || rtText.bottom <= pDisplayRect->top)
			continue;

		if(IsTextClipped() == true || pClipping != NULL)
		{
			GrRect rtClip = *pDisplayRect;
			rtClip.Contract(GetPadding());

			if(pClipping != NULL)
			{
				if(rtClip.right > pClipping->right)
					rtClip.right = pClipping->right;
				if(rtClip.bottom > pClipping->bottom)
					rtClip.bottom = pClipping->bottom;
			}

			pRenderer->DrawText(GetRenderingFont(), GetText() + cl->nTextBegin, cl->nLength, foreColor, &rtText, &rtClip);
		}
		else
		{
			pRenderer->DrawText(GetRenderingFont(), GetText() + cl->nTextBegin, cl->nLength, foreColor, &rtText);
		}
	}
}

void GrCell::OnTextBoundChanged(const GrRect* pCur, const GrRect* pOld)
{
	if(pCur->GetHeight() != pOld->GetHeight())
	{
		GrRow* pRow = GetRow();
		if(m_pGridCore->GetAutoFitRow() == true)
			pRow->SetFit();
		pRow->SetTextAlignChanged();
	}
}

void GrCell::OnGridCoreAttached()
{
	GrObject::OnGridCoreAttached();
	m_pTextUpdater = m_pGridCore->GetTextUpdater();
	m_pTextUpdater->AddTextBound(this);
	m_pTextUpdater->AddTextAlign(this);
}

void GrCell::OnGridCoreDetached()
{
	if(m_bTextAlign == true)
		m_pTextUpdater->RemoveTextAlign(this);
	if(m_bTextBound == true)
		m_pTextUpdater->RemoveTextBound(this);
	m_pTextUpdater = NULL;
	GrObject::OnGridCoreDetached();
}

bool GrCell::IsIn(GrPoint pt) const
{
	if(pt.x < GetX() || pt.x >= GetX() + GetWidth() || 
		pt.y < GetY() || pt.y >= GetY() + GetHeight())
		return false;
	return true;
}

const wchar_t* GrCell::GetText() const
{
	return m_strText.c_str();
}

const wchar_t* GrCell::GetTextLineSource() const
{
	static std::wstring s = L"";
	if(m_strText == L"NULL")
		return s.c_str();
	return GetText();
}

void GrCell::SetText(const wchar_t* strText)
{
	if(m_strText.compare(strText) == 0)
		return;
	m_strText = strText;
	if(m_strText.compare(L"NULL") == 0)
		AddFlag(GRCF_TEXT_IS_NULL);
	else
		RemoveFlag(GRCF_TEXT_IS_NULL);
	OnTextChanged();
}

void GrCell::SetTextBoundChanged()
{
	if(m_pTextUpdater != NULL)
		m_pTextUpdater->AddTextBound(this);
}

void GrCell::SetTextAlignChanged()
{
	if(m_pTextUpdater != NULL)
		m_pTextUpdater->AddTextAlign(this);
}

void GrCell::Invalidate(bool thisOnly)
{
	if(m_pGridCore == NULL || GetDisplayable() == false)
		return;

	if(thisOnly == true)
		m_pGridCore->Invalidate(GetDisplayRect());
	else
		m_pGridCore->Invalidate();
}

void GrCell::OnTextChanged()
{
	if(GetTextWordWrap() == true && m_pGridCore->GetAutoFitRow() == true)
		GetRow()->SetFit();
	SetTextBoundChanged();
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

GrFont* GrCell::GetRenderingFont() const
{
	if(m_pFont == NULL)
	{
		if(m_pGridCore == NULL)
		{
			return GrFontManager::GetDefaultFont();
		}
		return m_pGridCore->GetFont();
	}
	return m_pFont;
}

GrColor GrCell::GetRenderingForeColor() const
{
	GrStyle* pStyle = m_pGridCore->GetStyle();
	if(m_foreColor == GrColor::Empty)
		return pStyle->GetCellForeColor();
	return m_foreColor;
}

GrColor GrCell::GetRenderingBackColor() const
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

ulong GrCell::AddFlag(ulong dwAdd)
{
	ulong dwReturn = dwAdd & ~m_dwFlag;
	m_dwFlag |= dwAdd;
	return dwReturn;
}

ulong GrCell::RemoveFlag(ulong dwRemove)
{
	ulong dwReturn = dwRemove & m_dwFlag;
	m_dwFlag &= ~dwRemove;
	return dwReturn;
}

bool GrCell::HasFlag(ulong dwFlag) const
{
	return m_dwFlag & dwFlag ? true : false;
}

ulong GrCell::GetFlag(ulong dwMask) const
{
	return m_dwFlag & dwMask;
}

GrFlag GrCell::ToRenderStyle() const
{
	GrFlag flag;
	if(GetMouseOvered() == true)
		flag += GRRS_MOUSEOVER;
	if(GetMousePressed() == true)
		flag += GRRS_PRESSED;
	return flag;
}

void GrCell::SetFont(GrFont* pFont)
{
	if(m_pFont == pFont)
		return;
	m_pFont = pFont;
	SetTextBoundChanged();
	Invalidate();
}

uint GrCell::GetID() const
{
	return m_nID;
}

void GrCell::SetBackColor(GrColor color)
{
	m_backColor = color;
	Invalidate();
}

void GrCell::SetForeColor(GrColor color)
{ 
	m_foreColor = color;
	Invalidate();
}

bool GrCell::GetMouseOvered() const
{
	if(m_pGridCore == NULL)
		return false;
	return m_pGridCore->GetMouseOver() == this;
}

GrMouseOverState GrCell::GetMouseOverState() const
{
	if(m_pGridCore == NULL)
		return GrMouseOverState_None;
	return m_pGridCore->GetMouseOverState();
}

bool GrCell::GetMousePressed() const
{
	if(m_pGridCore == NULL)
		return false;
	return m_pGridCore->GetMousePressed() == this;
}

GrItem::GrItem(GrColumn* pColumn, GrDataRow* pDataRow)
	: m_pColumn(pColumn), m_pDataRow(pDataRow)
{
	m_bReadOnly			= false;
	m_bSelected			= false;
	m_bColorLocked		= false;


#ifdef _MANAGED
	ManagedRef = nullptr;
#endif
}

GrMouseOverState GrItem::HitMouseOverTest(GrPoint pt) const
{
	if(pt.x < 0 || pt.x >= GetWidth() || pt.y < 0 || pt.y >= GetHeight())
		return GrMouseOverState_None;

	if(m_pColumn == NULL)
		return GrMouseOverState_In;

	if(GetReadOnly() == true)
		return GrMouseOverState_In;

	if(m_pColumn->GetItemType() == GrItemType_Control)
		return GrMouseOverState_In;

	GrRect rtControl = GetControlRect();
	if(rtControl.IsIn(pt) == true)
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
		}
	}

	return GrMouseOverState_In;
}

GrRect GrItem::GetControlRect() const
{
	GrRect rt;
	rt.left		= GetWidth() - DEF_CONTROL_WIDTH;
	rt.top		= 0;
	rt.right	= GetWidth() - 1;
	rt.bottom	= GetHeight() - 1;

	return rt;
}

IDataRow* GrItem::GetIDataRow() const
{
	return m_pDataRow;
}

IFocusable* GrItem::GetNextFocusable() const
{
	GrColumnList* pColumnList = m_pColumn->GetColumnList();
	assert(pColumnList->GetVisibleColumnCount());
	uint nVisibleIndex = m_pColumn->GetVisibleIndex() + 1;
	uint nLastIndex = GrUtil::LastIndex(pColumnList->GetVisibleColumnCount());

	if(nVisibleIndex > nLastIndex)
		nVisibleIndex = nLastIndex;

	GrColumn* pColumn = pColumnList->GetVisibleColumn(nVisibleIndex);
	return GetDataRow()->GetItem(pColumn);
}

IFocusable* GrItem::GetPrevFocusable() const
{
	GrColumnList* pColumnList = m_pColumn->GetColumnList();
	assert(pColumnList->GetVisibleColumnCount());
	uint nVisibleIndex = m_pColumn->GetVisibleIndex();
	if(nVisibleIndex != 0)
		nVisibleIndex--;
	GrColumn* pColumn = pColumnList->GetVisibleColumn(nVisibleIndex);
	return GetDataRow()->GetItem(pColumn);
}

GrColumn* GrItem::GetColumn() const
{
	return m_pColumn; 
}

GrDataRow* GrItem::GetDataRow() const
{ 
	return m_pDataRow; 
}

const GrRow* GrItem::GetRow() const
{
	return GetDataRow();
}

GrRow* GrItem::GetRow()
{
	return GetDataRow();
}

bool GrItem::GetVisible() const
{
	return m_pColumn->GetVisible();	
}

bool GrItem::GetReadOnly() const
{
	return (m_pColumn->GetReadOnly() || m_pDataRow->GetReadOnly() || m_bReadOnly);
}

void GrItem::SetReadOnly(bool b)
{
	m_bReadOnly = b;
}

void GrItem::SetSelected(bool b)
{
	if(m_bSelected == b)
		return;
	GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
	if(b == true)
		pItemSelector->SelectItem(this, GrSelectionType_Add);
	else
		pItemSelector->SelectItem(this, GrSelectionType_Remove);
}

bool GrItem::GetSelected() const
{
	return m_bSelected;
}

void GrItem::SetFocused(bool b)
{
	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
	if(b == true)
		pFocuser->Set(this);
	else
		pFocuser->Set(IFocusable::Null);
}

void GrItem::LockColor(bool b)
{
	m_bColorLocked = b;
}

bool GrItem::GetFocused() const
{
	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
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

GrPoint GrItem::GetDisplayPosition() const
{
	return GrPoint(m_pColumn->GetDisplayX(), m_pDataRow->GetDisplayY());
}

bool GrItem::ShouldEnsureVisible() const
{
	return (m_pColumn->ShouldEnsureVisible() || m_pDataRow->ShouldEnsureVisible());
}

bool GrItem::GetDisplayable() const
{
	return (m_pColumn->GetDisplayable() && m_pDataRow->GetDisplayable());
}

bool GrItem::IsItemSelecting() const
{
	GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
	if(pItemSelector->IsSelecting(m_pColumn) == false)
		return false;
	if(pItemSelector->IsSelecting(m_pDataRow) == false)
		return false;
	return true;
}

GrFlag GrItem::ToRenderStyle() const
{
	GrFlag flag = GrCell::ToRenderStyle();
	if(m_pGridCore->GetSelectionVisible() == false)
		return flag;

	if(IsItemSelecting() == true || GetSelected() == true)
	{
		flag += GRRS_SELECTED;
	}

	if(GetFocused() == true)
	{
		flag += GRRS_FOCUSED;
	}
	return flag;
}

GrFont*	GrItem::GetRenderingFont() const
{
	return GetFont();
}

GrColor GrItem::GetRenderingForeColor() const
{
	const GrColor foreColor = GetForeColor();
	const GrStyle* pStyle = m_pGridCore->GetStyle();
	GrColor clr = foreColor;

	if(m_bColorLocked == true)
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

GrColor GrItem::GetRenderingBackColor() const
{
	const GrColor backColor = GetBackColor();
	const GrStyle* pStyle = m_pGridCore->GetStyle();
	GrColor clr = backColor;

	if(m_bColorLocked == true)
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
	else if(m_pDataRow->HasFocused() == true  && 
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

GrColor	GrItem::GetForeColor(bool inherited) const
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

GrColor	GrItem::GetBackColor(bool inherited) const
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
	return m_pGridCore->GetFont();
}

GrState GrItem::GetControlState() const
{
	if(GetMouseOvered() == false)
		return GrState_Normal;
	if(m_pGridCore->GetMouseOverState() == GrMouseOverState_Control)
	{
		if(GetMousePressed() == true)
			return GrState_Pressed;
		else 
			return GrState_MouseOver;
	}
	return GrState_Normal;
}

void GrItem::OnTextChanged()
{
	GrCell::OnTextChanged();
}

void GrItem::OnTextBoundChanged(const GrRect* pCur, const GrRect* pOld)
{
	GrCell::OnTextBoundChanged(pCur, pOld);

	if(pCur->GetWidth() != pOld->GetWidth())
	{
		if(m_pGridCore->GetAutoFitColumn() == true)
			m_pColumn->SetFit();
	}
}

void GrItem::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	GrRect rtRender = GetDisplayRect();

	if(rtRender.left > pClipping->right || rtRender.right <= pClipping->left)
		return;

	GrFlag renderStyle = ToRenderStyle();

	GrColor backColor	= GetRenderingBackColor();
	GrColor foreColor	= GetRenderingForeColor();

	if(GetClipped() == true)
		pRenderer->DrawCell(renderStyle, backColor, &rtRender, pClipping);
	else
		pRenderer->DrawCell(renderStyle, backColor, &rtRender);

	if(m_pColumn->m_bCustomItemRender == true)
	{
		//(*m_pColumn->m_fnCustomRender)(pRenderer, this, rtRender, m_pColumn->m_dwUserData);
		return;
	}

	if(GetClipped() == true)
		RenderText(pRenderer, foreColor, &rtRender, pClipping);
	else
		RenderText(pRenderer, foreColor, &rtRender);
	

	bool bShowControl = false;
	switch(m_pColumn->GetItemTypeShow())
	{
	case GrItemTypeShow_SelectedOnly:
		if(renderStyle.Has(GRRS_SELECTED) == true)
			bShowControl = true;
		break;
	case GrItemTypeShow_FocusedOnly:
		if(GetFocused() == true)
			bShowControl = true;
		break;
	case GrItemTypeShow_Always:
		bShowControl = true;
		break;
	}

	if(GetReadOnly() == true)
		bShowControl = false;

	if(bShowControl == true)
	{
		GrRect rtButton = GetControlRect() + rtRender.GetLocation();
		switch(m_pColumn->GetItemType())
		{
		case GrItemType_DropDown:
			pRenderer->DrawDropDown(&rtButton, GetControlState());
			break;
		case GrItemType_Modal:
			pRenderer->DrawModal(&rtButton, GetControlState());
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
	return m_bReadOnly;
}

void GrDataRow::SetReadOnly(bool b)
{
	m_bReadOnly = b;
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
	return m_nSelectedCells > 0;
}

bool GrDataRow::GetSelecting() const
{
	GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
	return pItemSelector->IsSelecting(this);
}

bool GrDataRow::GetFullSelected() const
{
	uint nVisibleColumns = m_pGridCore->GetColumnList()->GetVisibleColumnCount();
	if(nVisibleColumns == 0)
		return false;
	if(m_nSelectedCells == nVisibleColumns)
		return true;
	return false;
}

bool GrDataRow::GetVisible() const
{
	bool bVisible = GrRow::GetVisible();
	if(bVisible == false)
		return false;
	GrGroupingRow* pGroupingRow = dynamic_cast<GrGroupingRow*>(GetParent());
	if(pGroupingRow == NULL)
		return bVisible;

	return pGroupingRow->IsExpanded();
}

void GrDataRow::SetVisibleDataRowIndex(uint nIndex)
{
	m_nVisibleDataRowIndex = nIndex;
}

void GrDataRow::SetDataRowIndex(uint nIndex)
{
	wchar_t strNumber[10];
	if(m_pDataRowList->GetZeroBasedRowIndex() == true)
		swprintf(strNumber, 10, L"%d", nIndex);
	else
		swprintf(strNumber, 10, L"%d", nIndex + 1);

	m_nDataRowIndex = nIndex;
	SetText(strNumber);
}

void GrDataRow::SetDataRowID(uint nIndex)
{
	m_nDataRowID = nIndex;
}

uint GrDataRow::GetVisibleDataRowIndex() const
{
	if(GetVisible() == false)
		return INVALID_INDEX;
	return m_nVisibleDataRowIndex;
}

uint GrDataRow::GetDataRowIndex() const
{
	return m_nDataRowIndex;
}

uint GrDataRow::GetDataRowID() const
{
	return m_nDataRowID;
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

void GrDataRow::SetItemBackColor(GrColor color)
{
	m_itemBackColor = color;
}

void GrDataRow::SetItemForeColor(GrColor color)
{
	m_itemForeColor = color;
}

void GrDataRow::SetItemFont(GrFont* pFont)
{
	m_pItemFont = pFont;
}

IFocusable* GrDataRow::GetFocusable(GrColumn* pColumn) const
{
	return GetItem(pColumn);
}

int GrDataRow::GetFitHeight() const
{
	int height = IDataRow::GetFitHeight();

	for_each(_Items, m_vecItems, value)
	{
		int nItemHeight = value->GetTextBound()->GetHeight() + DEF_LEADING;
		height = std::max(height, nItemHeight);
	}
	return height;
}

GrCell* GrDataRow::HitTest(const GrPoint& pt) const
{
	int y = GetY();
	if(pt.y < y || pt.y >= y + GetHeight())
		return NULL;

	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	GrColumn* pColumn = pColumnList->HitTest(pt.x);
	if(pColumn != NULL)
		return pColumn;

	GrRect bound = pColumnList->GetColumnSplitter()->GetRect();
	if(bound.IsIn(pt) == true)
		return pColumnList->GetColumnSplitter();

	int x = GetX();
	if(pt.x < x || pt.x >= x + GetWidth())
		return NULL;

	return const_cast<GrDataRow*>(this);
}

GrCell* GrDataRow::HitDisplayTest(const GrPoint& pt) const
{
	int y = GetDisplayY();
	if(pt.y < y || pt.y >= y + GetHeight())
		return NULL;

	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
	{
		GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
		int x = pColumn->GetDisplayX();

		if(pt.x >= x && pt.x < x + pColumn->GetWidth())
			return GetItem(pColumn);
	}

	GrRect bound = pColumnList->GetColumnSplitter()->GetDisplayRect();
	if(bound.IsIn(pt) == true)
		return pColumnList->GetColumnSplitter();

	int x = GetDisplayX();
	if(pt.x < x || pt.x >= x + GetWidth())
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
	//SetText(L"Column");
	m_pColumnList		= NULL;
	m_bVisible			= true;
	m_bReadOnly			= false;
	m_bCanBeSorted		= true;
	m_bCanBeGrouped		= true;
	m_bMovable			= true;
	m_bResizable		= true;
	m_bFrozen			= false;
	m_bClippedTextVisible = true;
	m_bSelected			= false;
	m_bFullSelected		= false;
	m_bFitting			= false;
	m_bDisplayable		= false;
	m_bClipped			= false;
	m_bGrouped			= false;

	m_nVisibleIndex		= INVALID_INDEX;
	m_nDisplayIndex		= INVALID_INDEX;
	m_nIndex			= INVALID_INDEX;
	m_nColumnID			= INVALID_INDEX;


	m_nX				= 0;
	m_nDisplayX			= 0;
	m_nWidth			= 100;
	m_nMinWidth 		= 30;
	m_nMaxWidth 		= 10000;
	m_itemType			= GrItemType_Control;
	m_itemTypeShow		= GrItemTypeShow_SelectedOnly;

	m_nSelectedCells	= 0;

	m_nPriority			= GetID();

	m_itemHorzAlign		= GrHorzAlign_Left;
	m_itemVertAlign		= GrVertAlign_Top;
	m_itemWordWrap		= false;
	m_itemMultiline		= false;
	m_itemBackColor		= GrColor::Empty;
	m_itemForeColor		= GrColor::Empty;
	m_itemPadding		= GrPadding::Default;
	m_pItemFont			= NULL;

	m_pGroupingInfo		= NULL;

	m_sortType			= GrSort_None;
	m_comparer[GrSort_None]	= 0;
	m_comparer[GrSort_Up]	= 0;
	m_comparer[GrSort_Down]	= 0;

	m_bCustomItemRender			= false;

	m_fnColumnBackgroundRender	= NULL;
	m_fnColumnContentsRender	= NULL;
	m_pColumnRenderData			= NULL;

	m_pGroupingInfo		= new GrGroupingInfo(this);
}

void GrColumn::OnGridCoreAttached()
{
	GrCell::OnGridCoreAttached();
	m_pColumnList = m_pGridCore->GetColumnList();
	m_pGridCore->AttachObject(m_pGroupingInfo);
}

void GrColumn::OnGridCoreDetached()
{
	GrCell::OnGridCoreDetached();
	m_pGridCore->DetachObject(m_pGroupingInfo);
	m_pColumnList = NULL;
}

void GrColumn::OnTextChanged()
{
	GrCell::OnTextChanged();
	if(m_pGridCore == NULL)
		return;

	if(GetDisplayable() == false)
		return;
	m_pGridCore->Invalidate(GetDisplayRect());
}

GrColumn::~GrColumn()
{
	m_nVisibleIndex		= INVALID_INDEX;
	m_nDisplayIndex		= INVALID_INDEX;
	m_nIndex			= INVALID_INDEX;
	m_nColumnID			= INVALID_INDEX;

	delete m_pGroupingInfo;

#ifdef _MANAGED
	System::ComponentModel::IComponent^ managedRef = this->ManagedRef;
	if(managedRef != nullptr)
	{
		delete this->ManagedRef;
		this->ManagedRef = nullptr;
	}

#endif
}

GrMouseOverState GrColumn::HitMouseOverTest(GrPoint pt) const
{
	if(pt.x < 0 || pt.x >= GetWidth() || pt.y < 0 || pt.y >= GetHeight())
		return GrMouseOverState_None;
	return GrMouseOverState_In;
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
	return m_nSelectedCells > 0;
}

bool GrColumn::GetFullSelected() const
{
	uint nVisibleRows = m_pGridCore->GetDataRowList()->GetVisibleDataRowCount();
	if(nVisibleRows == 0)
		return false;
	if(m_nSelectedCells == (int)nVisibleRows)
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
	return m_nPriority;
}

void GrColumn::SetPriority(int nPriority)
{
	m_nPriority = nPriority;
}

bool GrColumn::GetGrouped() const
{
	return m_bGrouped;
}

void GrColumn::SetGrouped(bool b)
{
	if(m_bGrouped == b)
		return;

	m_bGrouped = b;
	GroupingChanged(this, &GrEventArgs::Empty);
}

GrGroupingInfo* GrColumn::GetGroupingInfo() const
{
	return m_pGroupingInfo;
}

void GrColumn::SetDisplayable(bool b)
{
	m_bDisplayable = b;
}

bool GrColumn::GetDisplayable() const
{
	if(GetVisible() == false)
		return false;
	return m_bDisplayable;
}

void GrColumn::SetDisplayIndex(uint nIndex)
{
	m_nDisplayIndex = nIndex;
}

uint GrColumn::GetDisplayIndex() const
{
	return m_nDisplayIndex;
}

void GrColumn::SetVisibleIndex(uint nIndex)
{
	m_nVisibleIndex = nIndex;
}

uint GrColumn::GetVisibleIndex() const
{
	return m_nVisibleIndex;
}

uint GrColumn::GetFrozenIndex() const
{
	assert(m_bFrozen == true);
	return m_nVisibleIndex;
}

uint GrColumn::GetUnfrozenIndex() const
{
	assert(m_bFrozen == false);
	return m_nVisibleIndex - m_pGridCore->GetColumnList()->GetFrozenColumnCount();
}

uint GrColumn::GetIndex() const
{
	return m_nIndex;
}

void GrColumn::SetIndex(uint nIndex)
{
	m_nIndex = nIndex;
}

uint GrColumn::GetColumnID() const
{
	return m_nColumnID;
}

void GrColumn::SetColumnID(uint id)
{
	m_nColumnID = id;
}

void GrColumn::SetClipped(bool b)
{
	m_bClipped = b;
}

const wchar_t* GrColumn::GetTextLineSource() const
{
	return GetText();
}

GrRow* GrColumn::GetRow()
{
	return m_pColumnList; 
}

const GrRow* GrColumn::GetRow() const
{
	return m_pColumnList; 
}

bool GrColumn::GetClipped() const
{
	return m_bClipped;
}
bool GrColumn::ShouldEnsureVisible() const
{
	if(m_bDisplayable == false || m_bClipped == true)
		return true;
	if(m_bFrozen == true)
		return false;
	return false;
}

bool GrColumn::GetClippedTextVisible() const
{
	return m_bClippedTextVisible;
}

void GrColumn::SetClippedTextVisible(bool b)
{
	m_bClippedTextVisible = b;
}

void GrColumn::SetSortType(GrSort sortType)
{
	if(m_bCanBeSorted == false)
		return;
	if(m_pColumnList == NULL)
		return;
	m_sortType = sortType;
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifySortTypeChanged(this);
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

void GrColumn::EnableSort(bool bEnable)
{
	m_bCanBeSorted = bEnable;
}

bool GrColumn::CanBeSort() const
{
	return m_bCanBeSorted;
}

void GrColumn::EnableGrouping(bool bEnable)
{
	m_bCanBeGrouped = bEnable;
}

bool GrColumn::CanBeGrouped() const
{
	return m_bCanBeGrouped;
}

void GrColumn::SetWidth(int width) 
{ 
	width = std::max(width, m_nMinWidth);
	width = std::min(width, m_nMaxWidth);

	if(m_nWidth == width)
		return;

	m_nWidth = width;

	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifyWidthChanged(this);
}

void GrColumn::SetMinWidth(int nMinWidth)
{
	m_nMinWidth = nMinWidth;
}

void GrColumn::SetMaxWidth(int nMaxWidth)
{
	m_nMaxWidth = nMaxWidth;
}

int GrColumn::GetMinWidth() const
{
	return m_nMinWidth;
}

int GrColumn::GetMaxWidth() const
{
	return m_nMaxWidth;
}

bool GrColumn::GetMovable() const
{
	return m_bMovable;
}

void GrColumn::SetMovable(bool bMovable)
{
	m_bMovable = bMovable;
}

bool GrColumn::GetResizable() const
{
	return m_bResizable;
}

void GrColumn::SetResizable(bool b)
{
	m_bResizable = b;
}

bool GrColumn::GetFrozen() const
{
	return m_bFrozen;
}

void GrColumn::SetFrozen(bool b)
{
	if(m_bFrozen == b)
		return;
	m_bFrozen = b;
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifyFrozenChanged(this);
}

void GrColumn::AdjustWidth()
{
	if(m_bFitting == false)
		return;

	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

	int nWidth = m_nMinWidth;
	for(uint i=0 ; i<pDataRowList->GetVisibleDataRowCount() ; i++)
	{
		GrDataRow*	pDataRow = pDataRowList->GetVisibleDataRow(i);
		GrItem*		pItem	 = pDataRow->GetItem(this);
		int nItemWidth = pItem->GetTextBound()->GetWidth();
		nWidth = std::max(nWidth, nItemWidth);
	}

	// insertion Row
	{
		GrDataRow*	pDataRow = m_pGridCore->GetInsertionRow();
		GrItem*		pItem	 = pDataRow->GetItem(this);
		int nItemWidth = pItem->GetTextBound()->GetWidth();
		nWidth = std::max(nWidth, nItemWidth);
	}
	if(GetItemType() != GrItemType_Control)
		nWidth += DEF_CONTROL_WIDTH;

	if(m_nWidth != nWidth)
		m_nWidth = nWidth;
	m_bFitting = false;
}

void GrColumn::SetFit()
{
	if(m_bFitting == true)
		return;
	m_bFitting = true;
	m_pColumnList->SetFitChanged();
}

int GrColumn::GetX() const 
{ 
	return m_nX; 
}

int GrColumn::GetY() const
{ 
	return GetRow()->GetY(); 
}

int GrColumn::GetWidth() const
{
	return m_nWidth;
}

int GrColumn::GetHeight() const
{
	return m_pColumnList->GetHeight();
}

bool GrColumn::GetVisible() const
{
	return m_bVisible;
}

bool GrColumn::GetReadOnly() const
{
	return m_bReadOnly;
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

GrColor GrColumn::GetItemForeColor() const
{
	return m_itemForeColor;
}

GrColor GrColumn::GetItemBackColor() const
{
	return m_itemBackColor;
}

GrPadding GrColumn::GetItemPadding() const
{
	return m_itemPadding;
}

GrFont* GrColumn::GetItemFont() const
{
	return m_pItemFont;
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
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifyHorzAlignChanged(this);
}

void GrColumn::SetItemVertAlign(GrVertAlign vertAlign)
{
	if(m_itemVertAlign == vertAlign)
		return;
	m_itemVertAlign = vertAlign;
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifyVertAlignChanged(this);
}

void GrColumn::SetItemWordWrap(bool b)
{
	if(m_itemWordWrap == b)
		return;
	m_itemWordWrap = b;
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifyWordwrapChanged(this);
}

void GrColumn::SetItemMultiline(bool b)
{
	if(m_itemMultiline == b)
		return;
	m_itemMultiline = b;
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifyMultilineChanged(this);
}

void GrColumn::SetItemForeColor(GrColor color)
{
	m_itemForeColor = color;
	if(m_pGridCore != NULL)
		m_pGridCore->Invalidate();
}

void GrColumn::SetItemBackColor(GrColor color)
{
	m_itemBackColor = color;
	if(m_pGridCore != NULL)
		m_pGridCore->Invalidate();
}

void GrColumn::SetItemPadding(GrPadding padding)
{
	m_itemPadding = padding;
	if(m_pGridCore != NULL)
		m_pGridCore->Invalidate();
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifyPaddingChanged(this);
}

void GrColumn::SetItemFont(GrFont* pFont)
{
	m_pItemFont = pFont;
	if(m_pGridCore != NULL)
		m_pGridCore->Invalidate();
}

void GrColumn::SetVisible(bool b)
{
	if(m_bVisible == b)
		return;
	m_bVisible = b;

	if(m_pGridCore == NULL)
		return;

	if(m_bVisible == false)
	{
		if(GetGrouped() == true)
			SetGrouped(false);

		GrFocuser* pFocuser = m_pGridCore->GetFocuser();
		if(pFocuser->GetFocusedColumn() == this)
			pFocuser->Set(IFocusable::Null);
	}

	m_pColumnList->SetVisibleChanged();
}

void GrColumn::SetReadOnly(bool bReadOnly)
{
	if(m_bReadOnly == bReadOnly)
		return;
	m_bReadOnly = bReadOnly;
}

void GrColumn::SetItemType(GrItemType type)
{
	if(m_itemType == type)
		return;
	m_itemType = type;
}

void GrColumn::SetTooltip(const wchar_t* strTooltip)
{
	m_strTooltip = strTooltip;
}

const wchar_t* GrColumn::GetTooltip() const
{
	return m_strTooltip.c_str();
}

bool GrColumn::GetSelecting() const
{
	GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
	return pItemSelector->IsSelecting(this);
}

GrColor GrColumn::GetRenderingBackColor() const
{
	GrStyle* pStyle = m_pGridCore->GetStyle();
	GrColor backColor;
	if(GetGrouped() == true)
		backColor = pStyle->GetGroupingBackColor(GetGroupingInfo()->GetGroupingLevel());
	else
		return GrCell::GetRenderingBackColor();

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

GrFlag GrColumn::ToRenderStyle() const
{
	GrFlag flag = GrCell::ToRenderStyle();
	if(GetSelecting() == true)
		flag.Add(GRRS_SELECTED);
	return flag;
}

GrPoint GrColumn::GetDisplayPosition() const
{
	return GrPoint(GetDisplayX(), m_pColumnList->GetDisplayY());
}

void GrColumn::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	GrRect rtRender = GetDisplayRect();
	if(rtRender.left > pClipping->right || rtRender.right <= pClipping->left)
		return;

	if(m_fnColumnBackgroundRender == NULL || (*m_fnColumnBackgroundRender)(pRenderer, this, rtRender, m_pColumnRenderData) == false)
	{
		GrFlag	renderStyle = ToRenderStyle();
		GrColor backColor   = GetRenderingBackColor();
		if(GetClipped() == true)
			pRenderer->DrawHeader(renderStyle, &rtRender, backColor, pClipping);
		else
			pRenderer->DrawHeader(renderStyle, &rtRender, backColor);
	}

	if(m_fnColumnContentsRender == NULL || (*m_fnColumnContentsRender)(pRenderer, this, rtRender, m_pColumnRenderData) == false)
	{
		GrColor foreColor   = GetRenderingForeColor();

		GrSort sortType = GetSortType();
		GrRect rtText = rtRender;
		if(sortType != GrSort_None)
		{
			GrRect rtSort;
			rtSort.right	= rtRender.right - 6;
			rtSort.left		= rtSort.right - 10;
			rtSort.top		= (rtRender.bottom + rtRender.top) / 2 - 5;
			rtSort.bottom	= rtSort.top + 10;
			pRenderer->DrawSortGlyph(&rtSort, sortType);

			rtText.right = rtSort.left;
		}

		if(GetTextLineCount() > 0)
		{
			const GrLineDesc* cl = GetTextLine(0);
			if(GetClipped() == true)
				pRenderer->DrawTextEllipsis(GetRenderingFont(), GetText() + cl->nTextBegin, cl->nLength, foreColor, &rtText, pClipping);
			else
				pRenderer->DrawTextEllipsis(GetRenderingFont(), GetText() + cl->nTextBegin, cl->nLength, foreColor, &rtText);

		}
	}
}

void GrColumn::RenderTooltip(GrGridRenderer* pRenderer, GrPoint ptStart) const
{
	GrRect rt, rtBack;
	GrFont* pFont = GetFont();

	rt.left		= ptStart.x;
	rt.top		= ptStart.y;
	rt.right	= ptStart.x + pFont->GetStringWidth(m_strTooltip.c_str());
	rt.bottom	= ptStart.y + pFont->GetHeight();

	int nRight = m_pGridCore->GetDisplayRect()->right;
	if(rt.right > nRight)
	{
		rt.Offset(nRight - rt.right, 0);
		rt.Offset(-8, 0);
	}
	rtBack = rt;
	rtBack.Expand(4, 4, 4, 4);

	pRenderer->DrawTooltip(&rtBack);

	pRenderer->DrawText(GetRenderingFont(), m_strTooltip.c_str(), m_strTooltip.length(), GrColor::Black, &rt);
}

void GrColumnList::BuildVisibleColumnList()
{
	m_vecVisibleColumns.clear();
	m_vecDisplayableColumns.clear();

	m_nFrozenCount = 0;
	for_each(_Columns, m_vecColumns, value)
	{
		value->SetDisplayable(false);
		value->SetDisplayIndex(INVALID_INDEX);
		value->SetClipped(false);

		if(value->GetVisible() == false)
			continue;
		m_vecVisibleColumns.push_back(value);
	}

	sort(m_vecVisibleColumns.begin(), m_vecVisibleColumns.end(), SortColumn());

	for_each(_Columns, m_vecVisibleColumns, value)
	{
		if(value->GetFrozen() == true)
			m_nFrozenCount++;
		uint nIndex = value.GetIndex();
		value->SetVisibleIndex(nIndex);
	}
	m_bWidthChanged = true;
}

void GrColumnList::AdjustColumnWidth()
{
	for_each(_Columns, m_vecColumns, value)
	{
		value->AdjustWidth();
	}
	m_bWidthChanged = true;
}

void GrColumnList::RepositionColumnList()
{
	int x = GetX() + GetWidth() + m_pGridCore->GetGroupingMargin();

	for(uint i=0 ; i<GetFrozenColumnCount() ; i++)
	{
		GrColumn* pColumn = GetFrozenColumn(i);
		pColumn->SetX(x);
		x += pColumn->GetWidth();
	}

	if(m_pColumnSplitter->GetVisible() == true)
		x += m_pColumnSplitter->GetWidth();

	m_nUnfrozenX = x;

	for(uint i=0 ; i<GetUnfrozenColumnCount() ; i++)
	{
		GrColumn* pColumn = GetUnfrozenColumn(i);
		pColumn->SetX(x);
		x += pColumn->GetWidth();
	}
	m_nVisibleRight = x;

	m_nBoundWidth = x - GetX();

	m_mapColumnByPosition.clear();
	for_each(_Columns, m_vecVisibleColumns, value)
	{
		int nKey = value->GetX() + value->GetWidth() - 1;
		m_mapColumnByPosition.insert(_MapColumnPos::value_type(nKey, value));
	}	

	m_nClippedIndex = INVALID_INDEX;
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
	ColumnSortTypeChanged(this, e);
}

void GrColumnList::OnColumnFrozenChanged(GrColumnEventArgs* e)
{
	ColumnFrozenChanged(this, e);
}

void GrColumnList::OnColumnWidthChanged(GrColumnEventArgs* e)
{
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

void GrColumnList::OnColumnGroupingChanged(GrColumnEventArgs* e)
{
	ColumnGroupingChanged(this, e);
}

uint GrColumnList::GetVisibleColumnCount() const
{
	return m_vecVisibleColumns.size();
}

GrColumn* GrColumnList::GetVisibleColumn(uint nIndex) const
{
	assert(nIndex < m_vecVisibleColumns.size());
	return m_vecVisibleColumns[nIndex];
}

int GrColumnList::GetVisibleRight() const
{
	return m_nVisibleRight;
}

uint GrColumnList::GetFrozenColumnCount() const
{
	return m_nFrozenCount;
}

GrColumn* GrColumnList::GetFrozenColumn(uint nIndex) const
{
	assert(nIndex < m_nFrozenCount);
	return GetVisibleColumn(nIndex);
}

uint GrColumnList::GetUnfrozenColumnCount() const
{
	assert(GetVisibleColumnCount() >= m_nFrozenCount);
	return GetVisibleColumnCount() - m_nFrozenCount;
}

GrColumn* GrColumnList::GetUnfrozenColumn(uint nIndex) const
{
	assert(nIndex + m_nFrozenCount < GetVisibleColumnCount());
	return GetVisibleColumn(nIndex + m_nFrozenCount);
}

uint GrColumnList::GetDisplayableColumnCount() const
{
	return m_vecDisplayableColumns.size();
}

GrColumn* GrColumnList::GetDisplayableColumn(uint nIndex) const
{
	assert(nIndex < m_vecDisplayableColumns.size());
	return m_vecDisplayableColumns[nIndex];
}

int GrColumnList::GetDisplayableRight() const
{
	return m_nDisplayableRight;
}

void GrRow::SetY(int y)
{
	if(m_nY == y)
		return;
	m_nY = y;
	OnYChanged();
}

void GrRow::SetHeight(int height)
{
	if(m_nHeight == height)
		return;

	m_nHeight = height;
	OnHeightChanged();
}

void GrRow::SetFit()
{
	if(GetFittable() == false)
		return;
	m_bFitting = true;

	OnFitted();
}

void GrRow::OnFitted()
{
	SetTextAlignChanged();
}

int GrRow::GetFitHeight() const
{
	int nHeight = GetTextBound()->GetHeight();
	return std::max(nHeight, m_nMinHeight);
}

int GrRow::GetHeight() const
{
	return m_nHeight;
}

void GrRow::AdjustHeight()
{
	if(m_bFitting == false)
		return;

	int nHeight = GetFitHeight();

	if(m_nHeight != nHeight)
	{
		SetHeight(nHeight);
		OnHeightAdjusted();
	}
	m_bFitting = false;
}

void GrRow::SetMinHeight(int height)
{
	m_nMinHeight = height;
	if(m_pGridCore != NULL)
		m_pGridCore->Invalidate();
}

int GrRow::GetMinHeight() const
{
	return m_nMinHeight;
}

void GrRow::SetMaxHeight(int height)
{
	m_nMaxHeight = height;
	if(m_pGridCore != NULL)
		m_pGridCore->Invalidate();
}

int GrRow::GetMaxHeight() const
{
	return m_nMaxHeight;
}

GrRow::GrRow() 
{
	m_nY				= 0;
	m_bVisible			= true;
	m_bResizable		= true;

	m_nHeight			= GrRow::DefaultHeight;
	m_nMinHeight		= GrRow::DefaultMinHeight;
	m_nMaxHeight		= GrRow::DefaultMaxHeight;
	m_pParent			= NULL;
	m_nHierarchyLevel	= 0;
	m_bFitting			= false;
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
	}
}

void GrRow::Sort(FuncSortRow fnSort, void* dwUserData)
{
	class SortDesc
	{
	public:
		SortDesc(GrGridCore* pGridCore, FuncSortRow fn, void* dwUserData) : 
		  m_pGridCore(pGridCore), m_fn(fn), m_dwUserData(dwUserData) {}

		  bool operator () (const GrRow* pRow1, const GrRow* pRow2)
		  {
			  return (*m_fn)(m_pGridCore, pRow1, pRow2, m_dwUserData);
		  }

		  GrGridCore*		m_pGridCore;
		  FuncSortRow		m_fn;
		  void*				m_dwUserData;
	};
	sort(m_vecChilds.begin(), m_vecChilds.end(), SortDesc(m_pGridCore, fnSort, dwUserData));
}

GrCell* GrRow::HitTest(const GrPoint& pt) const
{
	if(GetVisible() == false)
		return NULL;

	GrRect bound = GetBound();
	if(bound.IsIn(pt) == false)
		return NULL;

	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		GrRow* pChild = GetChild(i);
		GrCell* pHit = pChild->HitTest(pt);
		if(pHit != NULL)
			return pHit;
	}

	return const_cast<GrRow*>(this);
}

GrCell* GrRow::HitDisplayTest(const GrPoint& pt) const
{
	if(GetDisplayable() == false)
		return NULL;

	GrRect bound = GetDisplayBound();
	if(bound.IsIn(pt) == false)
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

	m_bReadOnly			= false;
	m_nSelectedCells	= 0;
	m_pItemFont			= NULL;
	m_nDataRowIndex		= INVALID_INDEX;
	m_nDataRowID		= INVALID_INDEX;
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

#ifdef _MANAGED
	this->ManagedRef = nullptr;
#endif
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
	if(m_bVisible == b)
		return;
	m_bVisible = b;

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
	return 0;
}

int GrRow::GetDisplayX() const
{
	return m_pGridCore->GetDisplayRect()->left + GetX();
}

int GrRow::GetDisplayY() const
{
	return m_pGridCore->GetDisplayRect()->top + GetY();
}

GrColor GrDataRow::GetRenderingBackColor() const
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

GrFlag GrDataRow::ToRenderStyle() const
{
	GrFlag flag = GrRow::ToRenderStyle();
	if(GetSelecting() == true)
		flag.Add(GRRS_SELECTED);
	return flag;
}

void GrDataRow::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	GrRect rtRender = GetDisplayRect();
	GrStyle* pStyle	= m_pGridCore->GetStyle();

	GrFlag	renderStyle = ToRenderStyle();
	GrColor	foreColor	= GrColor::Black;
	GrColor backColor   = GetRenderingBackColor();

	if(GetClipped() == true)
		pRenderer->DrawHeader(renderStyle, &rtRender, backColor, pClipping);
	else
		pRenderer->DrawHeader(renderStyle, &rtRender, backColor);

	if(m_pDataRowList->GetRowNumberVisible() == true)
	{
		if(GetClipped() == true)
			RenderText(pRenderer, foreColor, &rtRender, pClipping);
		else
			RenderText(pRenderer, foreColor, &rtRender);
	}

	if(m_pGridCore->IsGrouped() == true)
	{
		rtRender.left	= rtRender.right;
		rtRender.right	= CellStart();
		if(GetRowType() == GrRowType_InsertionRow)
			pRenderer->DrawCell(0, GrColor::White, &rtRender);
		else if(GetVisibleIndex() == m_pDataRowList->GetVisibleRowCount() - 1)
			pRenderer->DrawCell(0, GrColor::White, &rtRender);
		else
			pRenderer->DrawCell(GRRS_NO_BOTTOM_LINE, GrColor::White, &rtRender);
	}

	int nRight = rtRender.right;
	const GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
	{
		const GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
		const GrItem* pItem = GetItem(pColumn);
		pItem->Render(pRenderer, pClipping);
		nRight = pItem->GetDisplayPosition().x + pItem->GetWidth();
	}

	bool bRenderHighlight = HasFocused() == true && m_pGridCore->GetRowHighlight() == true;

	const GrRect* pDisplayRect = m_pGridCore->GetDisplayRect();
	if(m_pGridCore->GetMarginVisible() == true && nRight < pDisplayRect->right)
	{
		GrColor rowColor;
		if(bRenderHighlight == true && m_pGridCore->GetRowHighlightType() != GrRowHighlightType_Line)
			rowColor = pStyle->RowHighlightBackColor;
		else
			rowColor = pStyle->GetRowItemBackColor(GetVisibleDataRowIndex());
		GrRect margin = rtRender;
		margin.left  = nRight;
		margin.right = pDisplayRect->right + 1;

		pRenderer->DrawCell(0, rowColor, &margin);
	}
}

bool GrRow::GetVisible() const
{
	return m_bVisible;
}

void GrRow::SetResizable(bool b)
{
	m_bResizable = b;
}

bool GrRow::GetResizable() const
{
	return m_bResizable;
}

int GrRow::CellStart() const
{
	int nStart = GetDisplayX() + GetWidth();
	return nStart + m_pGridCore->GetGroupingMargin();
}

GrHorzAlign GrRow::GetTextHorzAlign() const
{
	return GrHorzAlign_Center;
}

GrVertAlign	GrRow::GetTextVertAlign() const
{
	return GrVertAlign_Center;
}

IDataRow::IDataRow() : m_pDataRowList(0)
{
	m_nDisplayY			= INT_MIN;
	m_bDisplayable		= true;
	m_bClipped			= false;
	m_nVisibleIndex		= 0;
	m_nDisplayIndex		= 0;
}

void IDataRow::SetDisplayY(int y)
{
	m_nDisplayY = y;
}

int IDataRow::GetDisplayY() const
{
	if(m_pDataRowList == NULL)
		throw _Exception("GridCore에 붙지 않았음");

	if(m_nDisplayY == INT_MIN)
		return GrRow::GetDisplayY();

	if(GetDisplayable() == false)
	{
		uint nRenderingCount = m_pDataRowList->GetDisplayableRowCount();
		if(nRenderingCount == 0)
			return GrRow::GetDisplayY();

		IDataRow* pFirstRendering = m_pDataRowList->GetDisplayableRow(0);
		int nOffset = pFirstRendering->GetDisplayY() - GrRow::GetDisplayY();
		return m_pDataRowList->GetDisplayY() - nOffset;
	}

	return m_nDisplayY;
}

void IDataRow::SetVisibleIndex(uint nIndex)
{
	m_nVisibleIndex = nIndex;
}

uint IDataRow::GetVisibleIndex() const
{
	return m_nVisibleIndex;
}

void IDataRow::SetDisplayIndex(uint nIndex)
{
	m_nDisplayIndex = nIndex;
}

uint IDataRow::GetDisplayIndex() const
{
	return m_nDisplayIndex;
}

void IDataRow::SetClipped(bool b)
{
	m_bClipped = b;
}

bool IDataRow::GetClipped() const
{
	return m_bClipped;
}

bool IDataRow::ShouldEnsureVisible() const
{
	if(GetVisible() == false)
		throw _Exception("");
	if(m_bDisplayable == false || m_bClipped == true)
		return true;
	return false;
}

void IDataRow::SetDisplayable(bool b)
{ 
	m_bDisplayable = b; 
}

bool IDataRow::GetDisplayable() const
{
	if(GetVisible() == false)
		return false;
	return m_bDisplayable; 
}

int IDataRow::GetWidth() const
{
	if(m_pDataRowList == NULL)
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
	SetHeight(3);
	SetResizable(false);

	AddFlag(GRCF_SYSTEM_OBJCT);
	AddFlag(GRCF_HIDE_TEXT);
}

bool GrRowSplitter::GetVisible() const
{
	return m_pGridCore->GetInsertionRow()->GetVisible();
}

int GrRowSplitter::GetWidth() const
{
	return m_pGridCore->GetDisplayableRect().GetWidth();
}

void GrRowSplitter::Render(GrGridRenderer* pRenderer, const GrRect* /*pClipping*/) const
{
	GrRect rtRender = GetDisplayRect();
	pRenderer->DrawSplitterRow(&rtRender);
}

GrGroupingRow::GrGroupingRow() 
{
	AddFlag(GRCF_SYSTEM_OBJCT);

	m_nGroupingLevel	= 0;
	m_bExpanded			= true;
	m_pColumn		= NULL;

	m_pLabel = new GrGroupingCell(this);
}

GrGroupingRow::~GrGroupingRow()
{
	delete m_pLabel;
}

GrGroupingCell* GrGroupingRow::GetLabel() const
{
	return m_pLabel;
}

void GrGroupingRow::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
	GrRect rtRender = GetDisplayRect();
	if(rtRender.top > pClipping->bottom || rtRender.bottom <= pClipping->top)
		return;

	GrColor color(255,255,255);
	GrFlag	renderStyle = ToRenderStyle();

	if(pFocuser->GetFocusedRow() == this)
	{
		color.bytes.r = (byte)((float)color.bytes.r * 0.75f);
		color.bytes.g = (byte)((float)color.bytes.g * 0.75f);
		color.bytes.b = (byte)((float)color.bytes.b * 0.75f);
	}


	pRenderer->DrawHeader(renderStyle, &rtRender, color);

	rtRender.left	= rtRender.right;
	rtRender.right	= rtRender.left + m_nGroupingLevel * DEF_GROUP_WIDTH;

	pRenderer->DrawCell(GRRS_NO_BOTTOM_LINE|GRRS_NO_RIGHT_LINE, GrColor::White, &rtRender);

	//GetDisplayPosition().x	= rt.right;
	m_pLabel->Render(pRenderer, pClipping);
}

bool GrGroupingRow::GetVisible() const
{
	GrGroupingRow* pParentGroupingRow = dynamic_cast<GrGroupingRow*>(GetParent());
	if(pParentGroupingRow == NULL)
		return GrRow::GetVisible();

	return pParentGroupingRow->IsExpanded();
}

IFocusable* GrGroupingRow::GetFocusable(GrColumn* /*pColumn*/) const
{
	return m_pLabel;
}

GrCell* GrGroupingRow::HitTest(const GrPoint& pt) const
{
	int y = GetY();
	if(pt.y < y || pt.y >= y + GetHeight())
		return NULL;

	int x = m_pLabel->GetX();

	if(pt.x >= x && pt.x < x + m_pLabel->GetWidth())
		return m_pLabel;

	x = GetX();
	if(pt.x < x || pt.x >= x + GetWidth())
		return NULL;

	return const_cast<GrGroupingRow*>(this);
}

GrCell* GrGroupingRow::HitDisplayTest(const GrPoint& pt) const
{
	int y = GetDisplayY();
	if(pt.y < y || pt.y >= y + GetHeight())
		return NULL;

	int x = m_pLabel->GetX();

	if(pt.x >= x && pt.x < x + m_pLabel->GetWidth())
		return m_pLabel;

	x = GetDisplayX();
	if(pt.x < x || pt.x >= x + GetWidth())
		return NULL;

	return const_cast<GrGroupingRow*>(this);
}

GrCaption::GrCaption()
{
	SetText(L"제목.");

	AddFlag(GRCF_SYSTEM_OBJCT);
	m_horzAlign = GrHorzAlign_Left;
	m_vertAlign = GrVertAlign_Center;

	m_bVisible = true;
}

GrHorzAlign GrCaption::GetTextHorzAlign() const
{
	return m_horzAlign;
}

GrVertAlign	GrCaption::GetTextVertAlign() const
{
	return m_vertAlign;
}

void GrCaption::SetVisible(bool b)
{
	if(m_bVisible == b)
		return;

	m_bVisible = b;

	if(m_pGridCore != NULL)
		return;
	m_pGridCore->GetRootRow()->SetVisibleChanged();
	m_pGridCore->Invalidate();
}

bool GrCaption::GetVisible() const
{
	return m_bVisible;
}

void GrCaption::OnTextChanged()
{
	GrRow::OnTextChanged();
	Invalidate(false);
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

void GrCaption::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	GrRect rtRender = GetDisplayRect();
	if(rtRender.top > pClipping->bottom || rtRender.bottom <= pClipping->top)
		return;

	GrFlag	renderStyle = ToRenderStyle();
	GrColor	foreColor	= GetRenderingForeColor();
	GrColor backColor   = GetRenderingBackColor();

	if(m_pGridCore->GetMarginVisible() == true)
		renderStyle.Add(GRRS_NO_RIGHT_LINE);
	pRenderer->DrawCell(renderStyle, backColor, &rtRender);

	RenderText(pRenderer, foreColor, &rtRender);
}

int GrCaption::GetWidth() const
{
	return m_pGridCore->GetDisplayableRect().GetWidth();
}

GrGroupingList::GrGroupingList()
{
	AddFlag(GRCF_SYSTEM_OBJCT);
	SetText(L"그룹핑할 컬럼을 이쪽으로 드래그하여 옮기세요");
	m_bEnableGrouping	= true;
	m_bVisible			= true;
	m_bGroupingChanged	= false;
}

GrGroupingList::~GrGroupingList()
{

}

uint GrGroupingList::GetGroupingCount() const
{
	if(m_bEnableGrouping == false)
		return 0;
	return m_vecGroupings.size();
}

GrGroupingInfo* GrGroupingList::GetGrouping(uint nLevel) const
{
	return m_vecGroupings[nLevel];
}

void GrGroupingList::ExpandGrouping(uint nLevel, bool /*bExpand*/)
{
	GrGroupingInfo* pGroupingInfo = m_vecGroupings[nLevel];
	pGroupingInfo->SetExpanded(true);
}

void GrGroupingList::SetGroupingSortState(uint nLevel, GrSort sortType)
{
	GrGroupingInfo* pGroupingInfo = m_vecGroupings[nLevel];
	pGroupingInfo->SetSortType(sortType);
}

GrCell* GrGroupingList::HitTest(const GrPoint& pt) const
{
	int y = GetY();
	if(pt.y < y || pt.y >= y + GetHeight())
		return NULL;

	for_each(_Groupings, m_vecGroupings, value)
	{
		GrRect rect = value->GetRect();
		if(rect.IsIn(pt) == true)
			return value;
	}

	return const_cast<GrGroupingList*>(this);
}

GrCell* GrGroupingList::HitDisplayTest(const GrPoint& pt) const
{
	int y = GetDisplayY();
	if(pt.y < y || pt.y >= y + GetHeight())
		return NULL;

	for_each(_Groupings, m_vecGroupings, value)
	{
		GrRect rect = value->GetDisplayRect();
		if(rect.IsIn(pt) == true)
			return value;
	}

	return const_cast<GrGroupingList*>(this);
}

bool GrGroupingList::GetVisible() const 
{
	if(m_bEnableGrouping == false)
		return false;
	return m_bVisible; 
}

int GrGroupingList::GetFitHeight() const
{
	if(m_vecGroupings.size() == 0)
	{
		return GrRow::DefaultHeight;
	}
	return m_vecGroupings[0]->GetHeight() + 20;
}

void GrGroupingList::SetVisible(bool b)
{
	m_bVisible = b;
	GrRootRow* pHeaderList = dynamic_cast<GrRootRow*>(GetParent());
	pHeaderList->SetVisibleChanged();
}

bool GrGroupingList::CanBeGrouped() const
{
	return m_bEnableGrouping;
}

void GrGroupingList::EnableGrouping(bool b)
{
	if(m_bEnableGrouping == b)
		return;
	m_bEnableGrouping = b;
	Changed(this, &GrEventArgs::Empty);
}

void GrGroupingList::OnYChanged()
{
	GrUpdatableRow::OnYChanged();
	RepositionGrouping();
}

void GrGroupingList::RepositionGrouping()
{
	GrPoint pt;
	pt.x = GetX();
	pt.y = GetY();

	pt.x += 10;
	pt.y += 10;
	for_each(_Groupings, m_vecGroupings, value)
	{
		value->SetPosition(pt);
		pt.x += value->GetWidth() + 10;
	}
}

bool GrGroupingList::ShouldUpdate() const
{
	return m_bGroupingChanged == true;
}

void GrGroupingList::Update(bool /*force*/)
{
	if(m_bGroupingChanged == true)
		RepositionGrouping();

	if(m_vecGroupings.size() == 0)
		RemoveFlag(GRCF_HIDE_TEXT);
	else
		AddFlag(GRCF_HIDE_TEXT);
	m_bGroupingChanged = false;
}

void GrGroupingList::ResetGroupingLevel()
{
	for_each(_Groupings, m_vecGroupings, value)
	{
		value->SetGroupingLevelCore(value.GetIndex());
	}
}

void GrGroupingList::AddGrouping(GrGroupingInfo* pGroupingInfo)
{
	_Groupings::iterator itor = find(m_vecGroupings.begin(), m_vecGroupings.end(), pGroupingInfo);

	if(itor != m_vecGroupings.end())
		throw _Exception("이미 Grouping이 되어 있습니다.");

	uint level = pGroupingInfo->GetGroupingLevel();
	if(level > m_vecGroupings.size())
		m_vecGroupings.push_back(pGroupingInfo);
	else
		m_vecGroupings.insert(m_vecGroupings.begin() + level, pGroupingInfo);
	pGroupingInfo->SetText();
	pGroupingInfo->LevelChanged.Add(this, &GrGroupingList::groupingInfo_LevelChanged);

	ResetGroupingLevel();
	SetFit();
	m_pGridCore->Invalidate();
	m_bGroupingChanged = true;
	Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupingList::RemoveGrouping(GrGroupingInfo* pGroupingInfo)
{
	_Groupings::iterator itor = find(m_vecGroupings.begin(), m_vecGroupings.end(), pGroupingInfo);

	if(itor == m_vecGroupings.end())
		throw _Exception("Grouping이 되어 있지 않은데 해제하려고 합니다.");

	pGroupingInfo->LevelChanged.Remove(this, &GrGroupingList::groupingInfo_LevelChanged);
	(*itor)->SetGroupingLevelCore(INVALID_INDEX);
	m_vecGroupings.erase(itor);

	ResetGroupingLevel();
	SetFit();
	m_pGridCore->Invalidate();
	m_bGroupingChanged = true;
	Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupingList::groupingInfo_LevelChanged(GrObject* pSender, GrEventArgs* /*e*/)
{
	GrGroupingInfo* pGroupingInfo = (GrGroupingInfo*)pSender;
	_Groupings::iterator itor = std::find(m_vecGroupings.begin(), m_vecGroupings.end(), pGroupingInfo);
	*itor = NULL;

	uint nWhere = std::min(pGroupingInfo->GetGroupingLevel(), m_vecGroupings.size());

	m_vecGroupings.insert(m_vecGroupings.begin() + nWhere, pGroupingInfo);
	m_vecGroupings.erase(std::find(m_vecGroupings.begin(), m_vecGroupings.end(), (GrGroupingInfo*)NULL));
	
	for_each(_Groupings, m_vecGroupings, value)
	{
		value->SetGroupingLevelCore(value.GetIndex());
	}

	Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupingList::NotifyExpanded(GrGroupingInfo* pGroupingInfo)
{
	GrGroupingEventArgs e(pGroupingInfo);
	Expanded.Raise(this, &e);
}

void GrGroupingList::NotifySortChanged(GrGroupingInfo* pGroupingInfo)
{
	GrGroupingEventArgs e(pGroupingInfo);
	SortChanged.Raise(this, &e);;
}

void GrGroupingList::OnGridCoreAttached()
{
	GrRow::OnGridCoreAttached();
	m_pGridCore->Cleared.Add(this, &GrGroupingList::gridCore_Cleared);
	m_pGridCore->Created.Add(this, &GrGroupingList::gridCore_Created);
}

void GrGroupingList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	m_vecGroupings.clear();
	SetFit();
}

void GrGroupingList::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	pColumnList->ColumnGroupingChanged.Add(this, &GrGroupingList::columnList_ColumnGroupingChanged);
	pColumnList->ColumnInserted.Add(this, &GrGroupingList::columnList_ColumnInserted);
	pColumnList->ColumnRemoved.Add(this, &GrGroupingList::columnList_ColumnRemoved);
}

void GrGroupingList::columnList_ColumnInserted(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();
	if(pColumn->GetGrouped() == false)
		return;
	AddGrouping(pColumn->GetGroupingInfo());
}

void GrGroupingList::columnList_ColumnRemoved(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();
	if(pColumn->GetGrouped() == false)
		return;
	RemoveGrouping(pColumn->GetGroupingInfo());
}

void GrGroupingList::columnList_ColumnGroupingChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();
	if(pColumn->GetGrouped() == true)
		AddGrouping(pColumn->GetGroupingInfo());
	else
		RemoveGrouping(pColumn->GetGroupingInfo());
}

void GrGroupingList::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	GrRect rtRender = GetDisplayRect();
	if(rtRender.top > pClipping->bottom || rtRender.bottom <= pClipping->top)
		return;

	GrFlag	renderStyle = ToRenderStyle();
	GrColor	foreColor	= GetRenderingForeColor();
	GrColor backColor   = GetRenderingBackColor();

	if(m_pGridCore->GetMarginVisible() == true)
		renderStyle.Add(GRRS_NO_RIGHT_LINE);

	pRenderer->DrawCell(renderStyle, backColor, &rtRender);

	for_each(_Groupings, m_vecGroupings, value)
	{
		value->Render(pRenderer, pClipping);
	}

	RenderText(pRenderer, foreColor, &rtRender, pClipping);
}

int GrGroupingList::GetWidth() const
{
	return m_pGridCore->GetDisplayableRect().GetWidth();
}

GrPoint GrRow::GetDisplayPosition() const
{
	return GrPoint(GetDisplayX(), GetDisplayY());
}

uint GrRow::GetChildCount() const
{
	return m_vecChilds.size();
}

GrRow* GrRow::GetChild(uint nIndex) const
{
	assert(nIndex < m_vecChilds.size());
	return m_vecChilds[nIndex];
}

void GrRow::AddChild(GrRow* pRow)
{
	m_vecChilds.push_back(pRow);
	pRow->m_pParent = this;
	pRow->m_nHierarchyLevel = m_nHierarchyLevel + 1;
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
		value->m_nHierarchyLevel = 0;
	}
	m_vecChilds.clear();
}

GrRow* GrRow::GetParent() const
{
	return m_pParent;
}

uint GrRow::GetHierarchyLevel() const
{
	return m_nHierarchyLevel;
}

void GrGroupingRow::Expand(bool bExpand)
{
	if(m_bExpanded == bExpand)
		return;
	if(m_pDataRowList != NULL)
		m_pDataRowList->SetVisibleChanged();
	m_bExpanded = bExpand;
}

bool GrGroupingRow::IsExpanded() const
{
	return m_bExpanded;
}

void GrGroupingRow::OnHeightChanged()
{
	IDataRow::OnHeightChanged();
	SetTextAlignChanged();
}

void GrGroupingRow::OnUpdatePositionCell(int x, GrRect* pBound)
{
	pBound->left	= x;
	pBound->right	= m_pLabel->GetX() + m_pLabel->GetWidth();
}

void GrGroupingRow::OnGridCoreAttached()
{
	IDataRow::OnGridCoreAttached();
	m_pGridCore->AttachObject(m_pLabel);
}

void GrGroupingRow::SetRefColumn(GrColumn* pColumn)
{
	m_pColumn = pColumn;
}

void GrGroupingRow::ProcessAfterGrouping()
{
	GrGroupingRow* pParent = dynamic_cast<GrGroupingRow*>(GetParent());
	if(pParent)
	{
		m_nGroupingLevel = pParent->m_nGroupingLevel + 1;
	}
	else
	{
		m_nGroupingLevel = 0;
	}

	std::wstring strText = GetText();


	wchar_t strItems[30];
	swprintf(strItems, 30, L" - %d items", GetChildCount());
	strText += strItems;
	m_pLabel->SetText(strText.c_str());

	return;
}

GrColumn* GrGroupingRow::GetColumn() const
{
	return m_pColumn;
}

GrInsertionRow::GrInsertionRow()
{
	SetText(L"☞");
	SetDisplayable(true);
	AddFlag(GRCF_SYSTEM_OBJCT);
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
	AddFlag(GRCF_SYSTEM_OBJCT);

	m_nRowWidth				= 50;
	m_nDisplayableBottom	= 0;

	m_nGroupingCount		= 0;
	m_nUsedGroupingRow		= 0;
	m_pFocusedDataRow		= NULL;
	m_nDataRowID			= 0;
	m_bVisibleRowNumber		= true;
	m_nClippedIndex			= INVALID_INDEX;
	m_bZeroBasedRowIndex	= false;

	m_bListChanged			= false;
	m_bVisibleChanged		= false;
	m_bFitChanged			= false;
}

void GrDataRowList::OnGridCoreAttached()
{
	GrRow::OnGridCoreAttached();

	m_vecDataRows.reserve(m_pGridCore->GetReservedRow());

	GrGroupingList* pGroupingList = m_pGridCore->GetGroupingList();

	pGroupingList->Changed.Add(this,		&GrDataRowList::groupingList_Changed);
	pGroupingList->Expanded.Add(this,		&GrDataRowList::groupingList_Expanded);
	pGroupingList->SortChanged.Add(this,	&GrDataRowList::groupingList_SortChanged);

	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
	pFocuser->FocusChanged.Add(this, &GrDataRowList::focuser_FocusedChanged);

	m_pGridCore->Cleared.Add(this, &GrDataRowList::gridCore_Cleared);
	m_pGridCore->Created.Add(this, &GrDataRowList::gridCore_Created);

	for_each(_DataRows, m_vecDataRows, value)
	{
		m_pGridCore->AttachObject(value);
	}

	for_each(_GroupingRows, m_vecGroupingRows, value)
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

GrGroupingRow* GrDataRowList::CreateGroupingRow(GrRow* pParent, GrColumn* pColumn, const wchar_t* strText)
{
	GrGroupingRow* pGroupingRow;
	if(m_vecGroupingRows.size() == m_nUsedGroupingRow)
	{
		pGroupingRow = new GrGroupingRow();
		m_vecGroupingRows.push_back(pGroupingRow);
	}
	else
	{
		pGroupingRow = m_vecGroupingRows[m_nUsedGroupingRow];
		pGroupingRow->ClearChild();
	}

	pParent->AddChild(pGroupingRow);
	pGroupingRow->SetText(strText);

	GrGroupingRow* pParentGroupingRow = dynamic_cast<GrGroupingRow*>(pParent);
	if(pParentGroupingRow)
	{
		pGroupingRow->m_strKey = pParentGroupingRow->m_strKey;
		pGroupingRow->m_strKey += L" | ";
		pGroupingRow->m_strKey += strText;
	}
	else
	{
		pGroupingRow->m_strKey = strText;
	}

	pGroupingRow->AddFlag(GRCF_HIDE_TEXT);
	pGroupingRow->SetRefColumn(pColumn);
	pGroupingRow->m_nGroupingLevel = 0;

	_MapCaches::iterator itor = m_mapCache.find(pGroupingRow->m_strKey);
	if(itor == m_mapCache.end())
	{
		int nHeight = (int)(m_pGridCore->GetFont()->GetHeight() / 0.65f);
		pGroupingRow->SetHeight(nHeight);
		pGroupingRow->Expand(true);
	}
	else
	{
		pGroupingRow->SetHeight((*itor).second.nHeight);
		pGroupingRow->Expand((*itor).second.bExpanded);
	}

	m_pGridCore->AttachObject(pGroupingRow);

	m_nUsedGroupingRow++;
	return pGroupingRow;
}

GrDataRowList::~GrDataRowList()
{
	DeleteObjects();
}


void GrDataRowList::BuildGrouping(GrRow* pParent, uint nGroupingLevel)
{
	GrGroupingList* pGroupingList = m_pGridCore->GetGroupingList();
	m_pGridCore->AttachObject(pGroupingList);
	GrColumn* pColumn = pGroupingList->GetGrouping(nGroupingLevel)->GetColumn();

	_DataRows vecSort;

	FuncSortRow fnSort;
	switch(pColumn->GetGroupingInfo()->GetSortType())
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

	std::wstring strCur;
	_DataRows::iterator itor = vecSort.begin();
	GrGroupingRow* pGroupingRow = NULL;

	while(itor != vecSort.end())
	{
		GrDataRow*	pRow	= *itor;
		GrItem*		pItem	= pRow->GetItem(pColumn);
		std::wstring		strNext = pItem->GetText();

		if(strCur != strNext || itor == vecSort.begin())
		{
			strCur = strNext;
			pGroupingRow = CreateGroupingRow(pParent, pColumn, strCur.c_str());
		}
		pGroupingRow->AddChild(pRow);
		itor++;
	}

	uint nNextGroupingLevel = nGroupingLevel+1;
	for(uint i=0 ; i<pParent->GetChildCount() ; i++)
	{
		GrGroupingRow* pRow = (GrGroupingRow*)pParent->GetChild(i);
		pRow->ProcessAfterGrouping();
		if(nNextGroupingLevel < m_nGroupingCount)
			BuildGrouping(pRow, nNextGroupingLevel);
	}
}

void GrDataRowList::BuildCache()
{
	m_mapCache.clear();
	for(uint i=0 ; i<m_nUsedGroupingRow ; i++)
	{
		GrGroupingRow* pGroupingRow = m_vecGroupingRows[i];

		GrCache cache;
		cache.nHeight	= pGroupingRow->GetHeight();
		cache.bExpanded = pGroupingRow->IsExpanded();

		m_mapCache.insert(_MapCaches::value_type(pGroupingRow->m_strKey, cache));
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

	if(m_nGroupingCount == 0)
	{
		m_nUsedGroupingRow = 0;
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
		m_nUsedGroupingRow = 0;
		BuildGrouping(this, 0);
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

	m_bFitChanged		= false;
	m_bHeightChanged	= true;

	m_pGridCore->Invalidate();
}

void GrDataRowList::SetFitChanged()
{
	m_bFitChanged = true;
}

void GrDataRowList::SetVisibleChanged()
{
	m_bVisibleChanged = true;
}

void GrDataRowList::SetListChanged()
{
	m_bListChanged = true;
}

void GrDataRowList::SetHeightChanged()
{
	m_bHeightChanged = true;
}

void GrDataRowList::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	for_each(_IDataRows, m_vecDisplayableRows, value)
	{
		int y = value->GetDisplayY();
		int b = y + value->GetHeight() ;
		if(y >= pClipping->bottom || b < pClipping->top)
			continue;
		
		value->Render(pRenderer, pClipping);
	}

	if(m_pFocusedDataRow != NULL && m_pFocusedDataRow->GetDisplayable() == true)
	{
		if(m_pGridCore->GetRowHighlight() == true && m_pGridCore->GetRowHighlightType() != GrRowHighlightType_Fill)
		{
			GrRect highlight = m_pFocusedDataRow->GetDisplayRect();
			GrStyle* pStyle = m_pGridCore->GetStyle();
			const GrRect* pDisplayRect = m_pGridCore->GetDisplayRect();
			highlight.top--;
			if(m_pGridCore->GetMarginVisible() == true)
				highlight.right = pDisplayRect->right + 1;
			else
				highlight.right = m_pGridCore->GetColumnList()->GetDisplayableRight();
			pRenderer->DrawRectangle(&highlight, pStyle->RowHighlightForeColor, 1);
		}
	}
}

void GrDataRowList::Reserve(uint reserve)
{
	m_vecDataRows.reserve(reserve);
}

void GrDataRowList::groupingList_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrGroupingList* pGroupingList = m_pGridCore->GetGroupingList();
	m_nGroupingCount = pGroupingList->GetGroupingCount();

	BuildChildRowList();
}

void GrDataRowList::groupingList_Expanded(GrObject* /*pSender*/, GrGroupingEventArgs* e)
{
	GrColumn* pColumn = e->m_pGroupingInfo->GetColumn();
	for_each(_GroupingRows, m_vecGroupingRows, value)
	{
		if(value->GetColumn() == pColumn)
			value->Expand(e->m_pGroupingInfo->GetExpanded());
	}
	SetVisibleChanged();
}

void GrDataRowList::groupingList_SortChanged(GrObject* /*pSender*/, GrGroupingEventArgs* e)
{
	GrColumn* pColumn = e->m_pGroupingInfo->GetColumn();

	GrRows setParent;
	for(uint i=0 ; i<m_nUsedGroupingRow ; i++)
	{
		GrGroupingRow* pGroupingRow = m_vecGroupingRows[i];

		if(pGroupingRow->GetColumn() == pColumn)
			setParent.insert(pGroupingRow->GetParent());
	}

	for_each(GrRows, setParent, value)
	{
		value->Sort(e->m_pGroupingInfo->GetSortType());
	}
	SetVisibleChanged();
}

//void GrDataRowList::columnList_ColumnRemoved(void

void GrDataRowList::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	pColumnList->ColumnInserted.Add(this, &GrDataRowList::columnList_ColumnInserted);
	pColumnList->ColumnRemoved.Add(this, &GrDataRowList::columnList_ColumnRemoved);

	m_pInsertionRow->SetDataRowID(INSERTION_ROW);
}

void GrDataRowList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	ClearChild();

	m_nGroupingCount	= 0;
	m_nUsedGroupingRow	= 0;
	m_pFocusedDataRow	= NULL;
	m_nDataRowID		= 0;

	m_pInsertionRow->ClearItem();

	DeleteObjects();

	m_vecVisibleDataRows.clear();
	m_mapRowByPosition.clear();
	m_vecVisibleRows.clear();
	m_vecDisplayableRows.clear();

	m_mapCache.clear();
	m_vecColumns.clear();
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

void GrDataRowList::focuser_FocusedChanged(GrObject* pSender, GrEventArgs* /*e*/)
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

	m_nVisibleHeight = 0;
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

		m_nVisibleHeight += pDataRowBase->GetHeight();
	}

	for_each(_IDataRows, m_vecDisplayableRows, value)
	{
		value->SetDisplayable(false);
		value->SetDisplayIndex(INVALID_INDEX);
		value->SetClipped(false);
	}

	m_vecDisplayableRows.clear();
	m_bHeightChanged = true;
}

void GrDataRowList::RepositionVisibleRowList()
{
	int y = GetY();
	for_each(_IDataRows, m_vecVisibleRows, value)
	{
		value->SetY(y);
		y += value->GetHeight();
	}
	m_nVisibleBottom = y;

	m_mapRowByPosition.clear();
	for_each(_IDataRows, m_vecVisibleRows, value)
	{
		int nKey = value->GetY() + value->GetHeight() - 1;
		m_mapRowByPosition.insert(_MapDataRowPos::value_type(nKey, value));
	}	
}

bool GrDataRowList::ShouldUpdate() const
{
	return m_bListChanged == true ||
		m_bVisibleChanged == true ||
		m_bFitChanged == true ||
		m_bHeightChanged == true;
}

void GrDataRowList::Update(bool force)
{
	if(m_bListChanged == true || force == true)
		BuildChildRowList();
	if(m_bVisibleChanged == true)
		BuildVisibleRowList();
	if(m_bFitChanged == true)
		AdjustRowHeight();
	if(m_bHeightChanged == true)
		RepositionVisibleRowList();

	m_bListChanged		= false;
	m_bVisibleChanged	= false;
	m_bFitChanged		= false;
	m_bHeightChanged	= false;
}

uint GrDataRowList::GetVisibleDataRowCount() const
{
	return m_vecVisibleDataRows.size();
}

GrDataRow* GrDataRowList::GetVisibleDataRow(uint nIndex) const
{
	if(nIndex == INSERTION_ROW)
		return m_pInsertionRow;
	if(nIndex >= m_vecVisibleDataRows.size())
		throw _Exception("잘못된 인덱스");
	return m_vecVisibleDataRows[nIndex];
}

int GrDataRowList::GetRowWidth() const
{
	return m_nRowWidth;
}

void GrDataRowList::SetRowWidth(int nWidth)
{
	m_nRowWidth = nWidth;
}

uint GrDataRowList::GetVisibleRowCount() const
{
	return m_vecVisibleRows.size();
}

IDataRow* GrDataRowList::GetVisibleRow(uint nIndex) const
{
	if(nIndex == INSERTION_ROW)
		return m_pGridCore->GetInsertionRow();
	if(nIndex >= m_vecVisibleRows.size())
		throw _Exception("잘못된 인덱스");
	return m_vecVisibleRows[nIndex];
}

void GrDataRowList::AddDataRow(GrDataRow* pDataRow)
{
	InsertDataRow(pDataRow, m_vecDataRows.size());
}

void GrDataRowList::InsertDataRow(GrDataRow* pDataRow, uint nIndex)
{
	if(pDataRow->GetDataRowIndex() != INVALID_INDEX || pDataRow == m_pInsertionRow)
		throw _Exception("이미 등록되어 있습니다");

	if(pDataRow->GetDataRowID() == INVALID_INDEX)
	{
		pDataRow->SetDataRowID(m_nDataRowID++);
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

	_DataRows::iterator itor = m_vecDataRows.insert(m_vecDataRows.begin() + nIndex, pDataRow);
	nIndex = std::min(nIndex, m_vecDataRows.size());
	for( ; itor != m_vecDataRows.end() ; itor++)
	{
		(*itor)->SetDataRowIndex(nIndex++);
	}

	SetListChanged();
	m_pGridCore->Invalidate();
}

void GrDataRowList::RemoveDataRow(GrDataRow* pDataRow)
{
	if(pDataRow->GetDataRowIndex() == INVALID_INDEX)
		return;

	_DataRows::iterator itor = std::find(m_vecDataRows.begin(), m_vecDataRows.end(), pDataRow);
	if(itor == m_vecDataRows.end())
		return;

	uint nDataRowIndex = pDataRow->GetDataRowIndex();
	for(_DataRows::iterator next = itor + 1 ; next != m_vecDataRows.end() ; next++)
	{
		(*next)->SetDataRowIndex(nDataRowIndex);
		nDataRowIndex++;
	}

	m_vecDataRowsRemoved.push_back(pDataRow);
	m_vecDataRows.erase(itor);
	pDataRow->SetDataRowIndex(INVALID_INDEX);
	pDataRow->SetSelected(false);
	
	if(pDataRow->HasFocused() == true)
		m_pGridCore->GetFocuser()->Set(IFocusable::Null);

	m_pGridCore->Invalidate();

	Update(true);
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

GrDataRow* GrDataRowList::GetDataRow(uint nIndex) const
{
	if(nIndex == INSERTION_ROW)
		return m_pGridCore->GetInsertionRow();
	return m_vecDataRows[nIndex];
}

void GrDataRowList::ClearDataRow()
{
	for_each(_DataRows, m_vecDataRows, value)
	{
		value->SetDataRowIndex(INVALID_INDEX);
	}

	m_vecDataRowsRemoved.insert(m_vecDataRowsRemoved.end(), m_vecDataRows.begin(), m_vecDataRows.end());
	m_vecDataRows.clear();

	Update(true);
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

	for_each(_GroupingRows, m_vecGroupingRows, value)
	{
		delete value;
	}

	m_vecDataRows.clear();
	m_vecDataRowsRemoved.clear();
	m_vecGroupingRows.clear();
}

uint GrDataRowList::GetDisplayableRowCount() const
{
	return m_vecDisplayableRows.size();
}

IDataRow* GrDataRowList::GetDisplayableRow(uint nIndex) const
{
	assert(nIndex < m_vecDisplayableRows.size());
	return m_vecDisplayableRows[nIndex];
}

int GrDataRowList::GetDisplayableBottom() const
{
	return m_nDisplayableBottom;
}

void GrDataRowList::HitTest(int y1, int y2, GrDataRows* pTested) const
{
	_MapDataRowPos::const_iterator itor = m_mapRowByPosition.lower_bound(y1);
	if(itor == m_mapRowByPosition.end())
		return;

	uint nStart = (*itor).second->GetVisibleIndex();
	uint nEnd;

	itor = m_mapRowByPosition.lower_bound(y2);
	if(itor == m_mapRowByPosition.end())
		nEnd = this->GetVisibleRowCount();
	else
		nEnd = (*itor).second->GetVisibleIndex() + 1;

	for(uint i=nStart ; i<nEnd ; i++)
	{
		GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(GetVisibleRow(i));
		if(pDataRow)
			pTested->insert(pDataRow);
	}

}

bool GrDataRowList::HitTest(int y1, int y2, GrIndexRange* pRange) const
{
	assert(m_vecVisibleRows.size());
	assert(y1 <= y2);

	_MapDataRowPos::const_iterator itor = m_mapRowByPosition.lower_bound(y1);
	if(itor == m_mapRowByPosition.end())
		return false;

	uint nStart = (*itor).second->GetVisibleIndex();
	uint nEnd;

	itor = m_mapRowByPosition.lower_bound(y2);
	if(itor == m_mapRowByPosition.end())
		nEnd = this->GetVisibleRowCount();
	else
		nEnd = (*itor).second->GetVisibleIndex() + 1;

	pRange->SetRange(nStart, nEnd);
	return true;
}

IDataRow* GrDataRowList::HitTest(int y) const
{
	_MapDataRowPos::const_iterator itor = m_mapRowByPosition.lower_bound(y);
	if(itor == m_mapRowByPosition.end())
		return NULL;
	IDataRow* pDataRow = (*itor).second;
	if(y < pDataRow->GetY())
		return NULL;
	return pDataRow;
}

GrRect GrDataRowList::GetBound() const
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	return GrRect(GetX(), GetY(), pColumnList->GetVisibleRight(), m_nVisibleBottom);
}

GrRect GrDataRowList::GetDisplayBound() const
{
	GrRootRow* pRootRow = m_pGridCore->GetRootRow();
	return GrRect(GetDisplayX(), GetDisplayY(), pRootRow->GetDisplayBound().right, m_nDisplayableBottom);
}

bool GrDataRowList::ShouldClip(const GrRect* pDisplayRect, uint /*horizontal*/, uint vertical) const
{
	if(m_nClippedIndex == vertical && pDisplayRect->GetHeight() == m_nClippedHeight)
		return false;
	return true;
}

void GrDataRowList::Clip(const GrRect* pDisplayRect, uint /*horizontal*/, uint vertical)
{
	int nDisplayY = GetDisplayY();

	for_each(_IDataRows, m_vecDisplayableRows, value)
	{
		value->SetDisplayable(false);
		value->SetDisplayIndex(INVALID_INDEX);
		value->SetClipped(false);
	}
	m_vecDisplayableRows.clear();

	uint nDisplayIndex = 0;
	for(uint i=vertical ; i<GetVisibleRowCount() ; i++)
	{
		IDataRow* pIDataRow = GetVisibleRow(i);

		if(nDisplayY >= pDisplayRect->bottom)
			break;

		pIDataRow->SetDisplayY(nDisplayY);
		pIDataRow->SetDisplayable(true);
		pIDataRow->SetDisplayIndex(nDisplayIndex);

		if(nDisplayY + pIDataRow->GetHeight() >= pDisplayRect->bottom)
			pIDataRow->SetClipped(true);
		else
			pIDataRow->SetClipped(false);

		m_vecDisplayableRows.push_back(pIDataRow);
		nDisplayY += pIDataRow->GetHeight();
		nDisplayIndex++;
	}

	m_nDisplayableBottom = std::min(pDisplayRect->bottom, nDisplayY);
	m_nClippedHeight = pDisplayRect->GetHeight();
	m_nClippedIndex  = vertical;
}

uint GrDataRowList::ClipFrom(uint nVisibleFrom) const
{
	return ClipFrom(m_pGridCore->GetDisplayRect(), nVisibleFrom);
}

uint GrDataRowList::ClipFrom(const GrRect* pDisplayRect, uint nVisibleFrom) const
{
	IDataRow* pDataRow = GetVisibleRow(nVisibleFrom);
	if(pDataRow == NULL)
		throw _Exception("잘못된 인덱스");

	int y = pDataRow->GetY() + (pDisplayRect->GetHeight() - GetY());

	IDataRow* pLastRow = HitTest(y);
	if(pLastRow == NULL)
		return GrUtil::LastIndex(GetVisibleRowCount());

	return pLastRow->GetVisibleIndex();
}

uint GrDataRowList::ClipTo(uint nVisibleTo) const
{
	return ClipTo(m_pGridCore->GetDisplayRect(), nVisibleTo);
}

uint GrDataRowList::ClipTo(const GrRect* pDisplayRect, uint nVisibleTo) const
{
	int nDisplayHeight = pDisplayRect->GetHeight() - GetY();
	int nDisplayY = nDisplayHeight;
	uint visibleFrom = nVisibleTo;

	for(uint i=nVisibleTo ; i>=0 && i<GetVisibleRowCount() ; i--)
	{
		IDataRow* pDataRow = GetVisibleRow(i);
		nDisplayY -= pDataRow->GetHeight();
		if(nDisplayY <= 0)
			break;
		visibleFrom = i;
	}
	return visibleFrom;
}

void GrDataRowList::SetZeroBasedRowIndex(bool b)
{
	if(m_bZeroBasedRowIndex == b)
		return;

	m_bZeroBasedRowIndex = b;
	
	uint nIndex = 0;
	for_each(_DataRows, m_vecDataRows, value)
	{
		value->SetDataRowIndex(nIndex);
	}
}

bool GrDataRowList::GetZeroBasedRowIndex() const
{
	return m_bZeroBasedRowIndex;
}

int GrDataRowList::DisplayToGlobal(int y) const
{
	uint nDisplayableRowCount = GetDisplayableRowCount();
	if(nDisplayableRowCount == 0)
		return y - m_pGridCore->GetDisplayRect()->top;;

	const IDataRow* pFirstDisplayableRow = GetDisplayableRow(0);
	int nOffset = pFirstDisplayableRow->GetY() - GetY();
	y += nOffset;
	return y - m_pGridCore->GetDisplayRect()->top;
}

bool GrDataRowList::GetRowNumberVisible() const
{
	return m_bVisibleRowNumber;
}

void GrDataRowList::SetRowNumberVisible(bool b)
{
	m_bVisibleRowNumber = b;
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

	if(m_nGroupingCount == 0)
	{
		GrRow::Sort(fnSort, (void*)pColumn);
	}
	else
	{
		for(uint i=0 ; i<m_nUsedGroupingRow ; i++)
		{
			GrGroupingRow* pGroupingRow = m_vecGroupingRows[i];
			if(pGroupingRow->GetGroupingLevel() == m_nGroupingCount - 1)
				pGroupingRow->Sort(fnSort, (void*)pColumn);
		}
	}
	SetVisibleChanged();
}

GrCell* GrDataRowList::HitTest(const GrPoint& pt) const
{
	GrCell* pHitted = GrUpdatableRow::HitTest(pt);
	if(pHitted == NULL)
		return NULL;

	IDataRow* pDataRow = HitTest(pt.y);
	if(pDataRow != NULL)
		return pDataRow;
	
	return NULL;
}

GrCell* GrDataRowList::HitDisplayTest(const GrPoint& pt) const
{
	GrCell* pHitted = GrUpdatableRow::HitDisplayTest(pt);
	if(pHitted == NULL)
		return NULL;

	for_each(_IDataRows, m_vecDisplayableRows, value)
	{
		GrCell* pSubHitted = value->HitDisplayTest(pt);
		if(pSubHitted != NULL)
			return pSubHitted;
	}
	
	return NULL;
}

GrColumnSplitter::GrColumnSplitter(GrColumnList* pColumnList) : m_pColumnList(pColumnList)
{
	AddFlag(GRCF_SYSTEM_OBJCT);
	AddFlag(GRCF_HIDE_TEXT);

	SetText(L"ColumnSplitter");

	m_bVisible = true;
}

int GrColumnSplitter::GetX() const
{
	return 0; 
}

int GrColumnSplitter::GetY() const
{ 
	return 0; 
}

int GrColumnSplitter::GetWidth() const
{ 
	return DEF_SPLITTER_SIZE;
}

int GrColumnSplitter::GetHeight() const
{
	
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
	return pDataRowList->GetDisplayableBottom() - m_pColumnList->GetDisplayY();
}

bool GrColumnSplitter::GetVisible() const
{
	return m_bVisible;
}

void GrColumnSplitter::SetVisible(bool b)
{
	if(m_bVisible == b)
		return;
	m_bVisible = b;

	m_pColumnList->SetVisibleChanged();
}

GrMouseOverState GrColumnSplitter::HitMouseOverTest(GrPoint /*pt*/) const 
{
	return GrMouseOverState_In; 
}

GrPoint GrColumnSplitter::GetDisplayPosition() const 
{
	return GrPoint(m_nDisplayX, m_pColumnList->GetDisplayY()); 
}

bool GrColumnSplitter::GetDisplayable() const 
{
	return true;
}

void GrColumnSplitter::Render(GrGridRenderer* pRenderer, const GrRect* /*pClipping*/) const 
{
	GrRect rtRender = GetDisplayRect();
	pRenderer->DrawCell(0, GrColor::White, &rtRender);
}

GrGroupingCell::GrGroupingCell(GrGroupingRow* pRow) : m_pRow(pRow)
{
	AddFlag(GRCF_SYSTEM_OBJCT);
}

GrMouseOverState GrGroupingCell::HitMouseOverTest(GrPoint pt) const
{
	if(pt.x < 0 || pt.x >= GetWidth() || pt.y < 0 || pt.y >= GetHeight())
		return GrMouseOverState_None;

	if(pt.x < DEF_GROUP_WIDTH)
		return GrMouseOverState_Control;
	return GrMouseOverState_In;
}

GrFlag GrGroupingCell::ToRenderStyle() const
{
	GrFlag flag = GrCell::ToRenderStyle();

	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
	if(pFocuser->Get() == this)
		flag += GRRS_FOCUSED;
	return flag;
}

const GrRow* GrGroupingCell::GetRow() const 
{
	return m_pRow; 
}

GrRow* GrGroupingCell::GetRow()
{
	return m_pRow; 
}

IDataRow* GrGroupingCell::GetIDataRow() const
{
	return m_pRow; 
}

IFocusable* GrGroupingCell::GetNextFocusable() const
{
	const IFocusable* p = this;
	return const_cast<IFocusable*>(p);
}

IFocusable* GrGroupingCell::GetPrevFocusable() const
{
	const IFocusable* p = this;
	return const_cast<IFocusable*>(p);
}

GrPoint GrGroupingCell::GetDisplayPosition() const
{
	return GrPoint(GetX(), m_pRow->GetDisplayY());
}

bool GrGroupingCell::GetDisplayable() const
{
	return m_pRow->GetDisplayable();
}

int GrGroupingCell::GetX() const
{
	GrRect rtRow;
	GrGroupingRow* pRow = (GrGroupingRow*)GetRow();
	rtRow = pRow->GetRect();
	return rtRow.right + (pRow->GetGroupingLevel() * DEF_GROUP_WIDTH);
}

int GrGroupingCell::GetY() const
{
	return m_pRow->GetY();
}

int GrGroupingCell::GetWidth() const
{
	GrGroupingRow* pRow = (GrGroupingRow*)GetRow();
	if(m_pGridCore->GetMarginVisible() == true)
		return m_pGridCore->GetDisplayRect()->GetWidth() - m_pRow->GetWidth() - (pRow->GetGroupingLevel() * DEF_GROUP_WIDTH);
	return m_pGridCore->GetColumnList()->GetDisplayableRight() - m_pRow->GetWidth() - (pRow->GetGroupingLevel() * DEF_GROUP_WIDTH);
	
}

int GrGroupingCell::GetHeight() const
{
	return m_pRow->GetHeight();
}

GrHorzAlign GrGroupingCell::GetTextHorzAlign() const
{
	return GrHorzAlign_Left;
}

GrVertAlign GrGroupingCell::GetTextVertAlign() const
{
	return GrVertAlign_Center;
}

void GrGroupingCell::Render(GrGridRenderer* pRenderer, const GrRect* /*pClipping*/) const
{
	GrColumn* pColumn = m_pRow->GetColumn();
	GrRect rtRender = GetDisplayRect();
	rtRender.left += DEF_GROUP_WIDTH;

	GrFlag	renderStyle = ToRenderStyle();
	GrColor	backColor = pColumn->GetRenderingBackColor();
	GrColor foreColor = pColumn->GetRenderingForeColor();

	pRenderer->DrawHeader(renderStyle, &rtRender, backColor);
	pRenderer->DrawCellLine(rtRender.left, rtRender.top-1, rtRender.right, rtRender.top-1);
	pRenderer->DrawCellLine(rtRender.left, rtRender.top-1, rtRender.left, rtRender.bottom);
	
	RenderText(pRenderer, foreColor, &rtRender);

	GrRect rtGlyph = rtRender;
	rtGlyph.left  = GetDisplayPosition().x;
	rtGlyph.right = GetDisplayPosition().x + DEF_GROUP_WIDTH;

	pRenderer->DrawCell(GRRS_NO_BOTTOM_LINE|GRRS_NO_RIGHT_LINE, GrColor::White, &rtGlyph);
	pRenderer->DrawTreeGlyph(&rtGlyph, m_pRow->IsExpanded());

	const GrRect* pDisplayRect = m_pGridCore->GetDisplayRect();
	if(m_pGridCore->GetMarginVisible() == true && rtRender.right < pDisplayRect->GetWidth())
	{
		rtRender.left  = rtRender.right;
		rtRender.right = pDisplayRect->GetWidth();
		pRenderer->DrawCell(0, backColor, &rtRender);
	}
}

GrGroupingInfo::GrGroupingInfo(GrColumn* pColumn) 
	: m_pColumn(pColumn)
{
	m_pGroupingList = NULL;
	m_pt.x = 0;
	m_pt.y = 0;
	m_bGrouped	= false;
	m_bExpanded = true;
	m_sortType	= GrSort_Up;
	m_nLevel	= INVALID_INDEX;
}

void GrGroupingInfo::OnGridCoreAttached()
{
	GrCell::OnGridCoreAttached();
	m_pGroupingList = m_pGridCore->GetGroupingList();
}

void GrGroupingInfo::OnGridCoreDetached()
{
	m_pGroupingList = NULL;
	GrCell::OnGridCoreDetached();
}

bool GrGroupingInfo::GetGrouped() const
{
	return m_pColumn->GetGrouped();
}

void GrGroupingInfo::SetGrouped(bool b)
{
	m_pColumn->SetGrouped(b);
}

void GrGroupingInfo::SetExpanded(bool b)
{
	if(m_pGroupingList->CanBeGrouped() == false)
		return;
	m_bExpanded = b;
	m_pGroupingList->NotifyExpanded(this);
}

bool GrGroupingInfo::GetExpanded() const
{
	return m_bExpanded;
}

GrSort GrGroupingInfo::GetSortType() const
{
	return m_sortType;
}

void GrGroupingInfo::SetSortType(GrSort sortType)
{
	if(m_pGroupingList->CanBeGrouped() == false)
		return;
	m_sortType = (sortType == GrSort_Up) ? GrSort_Up : GrSort_Down;
	m_pGroupingList->NotifySortChanged(this);
}

uint GrGroupingInfo::GetGroupingLevel() const
{
	return m_nLevel;
}

void GrGroupingInfo::SetGroupingLevel(uint level)
{
	if(m_nLevel == level)
		return;

	m_nLevel = level;
	if(GetGrouped() == true)
		LevelChanged(this, &GrEventArgs::Empty);
}

void GrGroupingInfo::SetGroupingLevelCore(uint level)
{
	m_nLevel = level;
}

void GrGroupingInfo::SetText()
{
	GrCell::SetText(m_pColumn->GetText());
}

int GrGroupingInfo::GetX() const
{
	return m_pt.x;
}

int GrGroupingInfo::GetY() const
{
	return m_pt.y;
}

int GrGroupingInfo::GetWidth() const
{
	return GetTextBound()->GetWidth() + 20;
}

int GrGroupingInfo::GetHeight() const
{
	return GetRenderingFont()->GetHeight() + GetPadding().GetVertical() + 10;
}

GrHorzAlign GrGroupingInfo::GetTextHorzAlign() const
{
	return GrHorzAlign_Left;
}

GrVertAlign GrGroupingInfo::GetTextVertAlign() const
{
	return GrVertAlign_Center;
}

GrMouseOverState GrGroupingInfo::HitMouseOverTest(GrPoint /*pt*/) const
{
	return GrMouseOverState_In;
}

GrPoint GrGroupingInfo::GetDisplayPosition() const
{
	return GetPosition();
}

bool GrGroupingInfo::GetDisplayable() const
{
	return GetRow()->GetDisplayable();
}

void GrGroupingInfo::Render(GrGridRenderer* pRenderer, const GrRect* /*pClipping*/) const
{
	GrRect rtRender = GetDisplayRect();
	GrFlag renderStyle = ToRenderStyle();

	GrColor backColor = m_pColumn->GetRenderingBackColor();
	GrColor foreColor = m_pColumn->GetRenderingForeColor();

	
	pRenderer->DrawHeader(renderStyle, &rtRender, backColor);

	GrRect rtSort;
	rtSort.right	= rtRender.right - 6;
	rtSort.left		= rtSort.right - 10;
	rtSort.top		= (rtRender.bottom + rtRender.top) / 2 - 5;
	rtSort.bottom	= rtSort.top + 10;

	pRenderer->DrawSortGlyph(&rtSort, m_sortType);
	RenderText(pRenderer, foreColor, &rtRender);
}

void GrGroupingInfo::SetPosition(GrPoint pt)
{
	m_pt = pt;
}

GrColumn* GrGroupingInfo::GetColumn() const
{
	return m_pColumn;
}

GrRootRow::GrRootRow()
{
	SetText(L"Root");
	AddFlag(GRCF_SYSTEM_OBJCT);
	m_bVisibleChanged	= true;
	m_bFitChanged		= false;
	m_bHeightChanged	= false;
}

bool GrRootRow::ShouldUpdate() const
{
	return m_bHeightChanged == true ||
		m_bVisibleChanged == true ||
		m_bFitChanged == true;
}

void GrRootRow::Update(bool force)
{
	for_each(_Updatables, m_vecUpdatables, value)
	{
		value->Update(force);
	}

	if(m_bVisibleChanged == true)
		BuildVisibleList();

	if(m_bFitChanged == true)
		AdjustRowHeight();

	if(m_bHeightChanged == true)
		RepositionVisibleList();

	m_nWidth = m_pColumnList->GetBound().GetWidth();
	
	m_bHeightChanged	= false;
	m_bVisibleChanged	= false;
	m_bFitChanged		= false;
}

bool GrRootRow::ShouldClip() const
{
	return true;
}

void GrRootRow::Clip(const GrRect* pDisplayRect, uint horizontal, uint vertical)
{
	m_rtDisplayBound.SetLocation(pDisplayRect->GetLocation());
	m_rtDisplayBound.bottom = pDisplayRect->top;

	for_each(_Updatables, m_vecUpdatables, value)
	{
		value->Clip(pDisplayRect, horizontal, vertical);

		m_rtDisplayBound.right  = std::max(value->GetDisplayBound().right,  m_rtDisplayBound.right);
		m_rtDisplayBound.bottom = std::max(value->GetDisplayBound().bottom, m_rtDisplayBound.bottom);
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
	m_bHeightChanged = true;
}

void GrRootRow::AdjustRowHeight()
{
	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		GrRow* pChild = GetChild(i);
		pChild->AdjustHeight();
	}
	m_bHeightChanged = true;
}

void GrRootRow::RepositionVisibleList()
{
	int y = GetY();
	for_each(_Rows, m_vecVisibleRows, value)
	{
		value->SetY(y);
		y += value->GetHeight();
	}
	m_nHeight = y - GetY();
}

void GrRootRow::SetVisibleChanged()
{
	m_bVisibleChanged = true;
	m_pGridCore->Invalidate();
}

void GrRootRow::SetFitChanged()
{
	m_bFitChanged = true;
}

void GrRootRow::SetHeightChanged()
{
	m_bHeightChanged = true;
}

GrSize GrRootRow::GetDisplaySize() const
{
	return m_rtDisplayBound.GetSize();
}

GrPoint GrRootRow::GetDisplayPosition() const
{
	return m_rtDisplayBound.GetLocation();
}

void GrRootRow::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	for_each(_Rows, m_vecVisibleRows, value)
	{
		value->Render(pRenderer, pClipping);
	}
}

GrCell* GrRootRow::HitTest(const GrPoint& pt) const
{
	GrCell* pHitted = GrRow::HitTest(pt);
	if(pHitted == NULL)
		return NULL;

	for_each(_Rows, m_vecVisibleRows, value)
	{
		GrCell* pSubHitted = value->HitTest(pt);
		if(pSubHitted != NULL)
			return pSubHitted;
	}

	return NULL;
}

GrCell* GrRootRow::HitDisplayTest(const GrPoint& pt) const
{
	GrCell* pHitted = GrRow::HitDisplayTest(pt);
	if(pHitted == NULL)
		return NULL;

	for_each(_Rows, m_vecVisibleRows, value)
	{
		GrCell* pSubHitted = value->HitDisplayTest(pt);
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
		m_vecClippables.push_back(pUpdatableRow);
	}

	std::sort(m_vecUpdatables.begin(), m_vecUpdatables.end(), SortUpdatable());
	std::sort(m_vecClippables.begin(), m_vecClippables.end(), SortClippable());

	m_pColumnList = m_pGridCore->GetColumnList();
}

bool GrSortFunc::SortRowsDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData)
{
	return SortRowsUp(pGridCore, pRow2, pRow1, dwUserData);
}

bool GrSortFunc::SortRowsUp(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*dwUserData*/)
{
	int nResult = wcscmp(pRow1->GetText(), pRow2->GetText());
	if(nResult == 0)
		return pRow1->GetID() < pRow2->GetID();
	return nResult < 0 ? true : false;
}

bool GrSortFunc::SortRowsNone(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*dwUserData*/)
{
	return pRow1->GetID() < pRow2->GetID();
}

bool GrSortFunc::SortDataRowSortDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData)
{
	return SortDataRowSortUp(pGridCore, pRow2, pRow1, dwUserData);
}

bool GrSortFunc::SortDataRowSortNone(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*dwUserData*/)
{
	GrDataRow* pDataRow1 = (GrDataRow*)pRow1;
	GrDataRow* pDataRow2 = (GrDataRow*)pRow2;
	return pDataRow1->GetDataRowIndex() < pDataRow2->GetDataRowIndex();
}

bool GrSortFunc::SortDataRowSortUp(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData)
{
	if(pRow1->GetRowType() == pRow2->GetRowType())
	{
		if(pRow1->GetRowType() == GrRowType_DataRow)
		{
			GrColumn* pColumn = (GrColumn*)dwUserData;
			const GrItem* pItem1= ((const GrDataRow*)pRow1)->GetItem(pColumn);
			const GrItem* pItem2 = ((const GrDataRow*)pRow2)->GetItem(pColumn);
			return wcscmp(pItem1->GetText(), pItem2->GetText()) < 0 ? true : false;
		}
		else if(pRow1->GetRowType() == GrRowType_GroupingRow)
		{
			return wcscmp(pRow1->GetText(), pRow2->GetText()) < 0 ? true : false;
		}
		else
		{
			return pRow1->GetID() < pRow2->GetID();
		}
	}
	return pRow1->GetRowType() < pRow2->GetRowType();
}

bool SortRowNormal::operator () (const GrRow* pRow1, const GrRow* pRow2)
{
	return pRow1->GetID() < pRow2->GetID();
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

bool GrUpdatableRow::ShouldClip(const GrRect* /*pDisplayRect*/, uint /*horizontal*/, uint /*vertical*/) const
{
	return false; 
}

void GrUpdatableRow::Clip(const GrRect* /*pDisplayRect*/, uint /*horizontal*/, uint /*vertical*/)
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