#include "StdAfx.h"
#include "GrGridCell.h"
#include "GrGridCore.h"
#include "GrGridRenderer.h"
#include "GrGridInternal.h"

#include <assert.h>


IFocusable* IFocusable::Null = 0;
uint GrCell::m_snID = 0;

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

GrColumnList::GrColumnList()
{
	m_pColumnSplitter = new GrColumnSplitter(this);

	AddFlag(GRCF_SYSTEM_OBJCT | GRCF_HIDE_TEXT);
	SetHeight(25);
	SetText(L"ColumnList");

	m_nFrozenCount			= 0;
	m_nGroupingCount		= 0;
	m_pSortColumn			= NULL;
	m_nDisplayableRight		= 0;
	m_nColumnID				= 0;

	m_bVisibleChanged		= false;
	m_bFitChanged			= false;

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
	for_stl_const(_Columns, m_vecColumns, itor)
	{
		_Columns::value_type value = *itor; 
		delete value;
	}

	for_stl_const(_Columns, m_vecColumnsRemoved, itor)
	{
		_Columns::value_type value = *itor;
		delete value;
	}

	delete m_pColumnSplitter;
}

int GrColumnList::GetWidth() const
{
	return m_pGridCore->GetDataRowList()->GetRowWidth();
}

void GrColumnList::Update(bool force)
{
	if(m_bVisibleChanged == true || force == true)
		BuildVisibleColumnList();

	if(m_bFitChanged == true)
		AdjustColumnWidth();

	m_bVisibleChanged	= false;
	m_bFitChanged		= false;
}

void GrColumnList::Reserve(uint reserve)
{
	m_vecColumns.reserve(reserve);
}

void GrColumnList::AddColumn(GrColumn* pColumn)
{
	InsertColumn(pColumn, m_vecColumns.size());
}

void GrColumnList::InsertColumn(GrColumn* pColumn, uint nIndex)
{
	if(pColumn->GetIndex() != INVALID_INDEX)
		throw new std::exception("이미 등록되어 있습니다");

	if(pColumn->GetColumnID() == INVALID_INDEX)
	{
		pColumn->SetColumnID(m_nColumnID++);
	}
	else
	{
		_Columns::iterator itor = find(m_vecColumnsRemoved.begin(), m_vecColumnsRemoved.end(), pColumn);
		m_vecColumnsRemoved.erase(itor);
	}

	m_pGridCore->AttachObject(pColumn);

	_Columns::iterator itor = m_vecColumns.insert(m_vecColumns.begin() + nIndex, pColumn);
	nIndex = std::min(nIndex, m_vecColumns.size());
	for( ; itor != m_vecColumns.end() ; itor++)
	{
		(*itor)->SetIndex(nIndex++);
	}

	if(m_vecVisibleColumns.size() == 0)
		pColumn->SetPriority(m_vecColumns.size());

	SetVisibleChanged();

	GrColumnEventArgs e(pColumn);
	OnColumnInserted(&e);

	m_pGridCore->Invalidate();
}

void GrColumnList::RemoveColumn(GrColumn* pColumn)
{
	pColumn->SetGrouped(false);

	_Columns::iterator itor = std::find(m_vecColumns.begin(), m_vecColumns.end(), pColumn);

	uint nIndex = pColumn->GetIndex();
	for(_Columns::iterator next = itor + 1 ; next != m_vecColumns.end() ; next++)
	{
		(*next)->SetIndex(nIndex);
		nIndex++;
	}

	pColumn->SetIndex(INVALID_INDEX);

	m_vecColumns.erase(itor);
	m_vecColumnsRemoved.push_back(pColumn);

	Update(true);

	GrColumnEventArgs e(pColumn);
	OnColumnRemoved(&e);
	m_pGridCore->Invalidate();
}

uint GrColumnList::GetCellCount() const
{
	return m_vecColumns.size() + 2;
}

GrCell* GrColumnList::GetCell(uint nIndex)
{
	if(nIndex == 0)
		return this;
	else if(nIndex == m_vecColumns.size() + 1)
		return m_pColumnSplitter;
	return m_vecColumns[nIndex - 1];
}

const GrCell* GrColumnList::GetCell(uint nIndex) const
{
	if(nIndex == 0)
		return this;
	else if(nIndex == m_vecColumns.size() + 1)
		return m_pColumnSplitter;
	return m_vecColumns[nIndex - 1];
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
	m_bFitChanged = true;
}

void GrColumnList::SetVisibleChanged()
{
	m_bVisibleChanged = true;
	m_pGridCore->Invalidate();
}

GrColumn* GrColumnList::GetFirstSortColumn() const
{
	return m_pSortColumn;
}

void GrColumnList::Clip(const GrRect* pDisplayRect, uint nVisibleStart)
{
	GrRect rtDisplay	= *pDisplayRect;
	int nDisplayX		= GetDisplayX() + GetWidth() + m_pGridCore->GetGroupingMargin();

	for_stl(_Columns, m_vecDisplayableColumns, itor)
	{
		_Columns::value_type value = *itor;
		value->SetDisplayable(false);
	}
	m_vecDisplayableColumns.clear();

	for(uint i=0 ; i<m_nFrozenCount ; i++)
	{
		GrColumn* pColumn = GetVisibleColumn(i);
		pColumn->SetDisplayX(nDisplayX);
		pColumn->SetDisplayable(true);
		pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());
		m_vecDisplayableColumns.push_back(pColumn);
		nDisplayX += pColumn->GetWidth();
		pColumn->SetClipped(false);
	}

	m_pColumnSplitter->m_nDisplayX = nDisplayX;

	nDisplayX += DEF_SPLITTER_SIZE;

	
	for(uint i=m_nFrozenCount+nVisibleStart ; i<GetVisibleColumnCount() ; i++)
	{
		GrColumn* pColumn = GetVisibleColumn(i);

		if(nDisplayX >= rtDisplay.right)
			break;

		pColumn->SetDisplayX(nDisplayX);
		pColumn->SetDisplayable(true);
		pColumn->SetDisplayIndex(m_vecDisplayableColumns.size());

		if(nDisplayX + pColumn->GetWidth() >= rtDisplay.right)
			pColumn->SetClipped(true);
		else
			pColumn->SetClipped(false);

		m_vecDisplayableColumns.push_back(pColumn);
		nDisplayX += pColumn->GetWidth();
	}

	m_nDisplayableRight = std::min(pDisplayRect->right, nDisplayX);
}

uint GrColumnList::ClipFrom(uint nVisibleFrom) const
{
	return ClipFrom(m_pGridCore->GetDisplayRect(), nVisibleFrom);
}

uint GrColumnList::ClipFrom(const GrRect* pDisplayRect, uint nVisibleFrom) const
{
	GrColumn* pColumn = GetUnfrozenColumn(nVisibleFrom);
	if(pColumn == NULL)
		throw std::exception("잘못된 인덱스");

	int x = pColumn->GetX() + (pDisplayRect->GetWidth() - m_nUnfrozenX);

	GrColumn* pLastColumn = HitTest(x);
	if(pLastColumn == NULL)
		return GrUtil::LastIndex(GetUnfrozenColumnCount());

	return pLastColumn->GetScrollableIndex();
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

	_Columns vecFrozens, vecScrollables;
	vecFrozens.reserve(GetColumnCount());
	vecScrollables.reserve(GetColumnCount());

	for_stl_const(_Columns, m_vecColumns, itor)
	{
		_Columns::value_type value = *itor;
		if(value == pColumn)
			continue;
		if(value->GetFrozen() == true)
			vecFrozens.push_back(value);
		else
			vecScrollables.push_back(value);
	}

	sort(vecFrozens.begin(), vecFrozens.end(), SortColumn());
	sort(vecScrollables.begin(), vecScrollables.end(), SortColumn());

	_Columns::iterator itorWhere = std::find(vecFrozens.begin(), vecFrozens.end(), pWhere);
	vecFrozens.insert(itorWhere, pColumn);
	pColumn->m_bFrozen = true;

	int nPriority = 0;
	for_stl_const(_Columns, vecFrozens, itor)
	{
		_Columns::value_type value = *itor;
		value->SetPriority(nPriority);
		nPriority++;
	}

	for_stl_const(_Columns, vecScrollables, itor)
	{
		_Columns::value_type value = *itor;
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

	_Columns vecFrozens, vecScrollables;
	vecFrozens.reserve(GetColumnCount());
	vecScrollables.reserve(GetColumnCount());

	for_stl_const(_Columns, m_vecColumns, itor)
	{
		_Columns::value_type value = *itor;
		if(value == pColumn)
			continue;
		if(value->GetFrozen() == true)
			vecFrozens.push_back(value);
		else
			vecScrollables.push_back(value);
	}

	sort(vecFrozens.begin(), vecFrozens.end(), SortColumn());
	sort(vecScrollables.begin(), vecScrollables.end(), SortColumn());

	_Columns::iterator itorWhere = std::find(vecScrollables.begin(), vecScrollables.end(), pWhere);
	vecScrollables.insert(itorWhere, pColumn);
	pColumn->m_bFrozen = false;

	int nPriority = 0;
	for_stl_const(_Columns, vecFrozens, itor)
	{
		_Columns::value_type value = *itor;
		value->SetPriority(nPriority);
		nPriority++;
	}

	for_stl_const(_Columns, vecScrollables, itor)
	{
		_Columns::value_type value = *itor;
		value->SetPriority(nPriority);
		nPriority++;
	}

	SetVisibleChanged();

	return true;
}

void GrColumnList::groupingList_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrGroupingList* pGroupingList = m_pGridCore->GetGroupingList();
	m_nGroupingCount = pGroupingList->GetGroupingCount();
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
	m_nDisplayableRight = m_pColumnSplitter->m_nDisplayX + m_pColumnSplitter->GetWidth();

	for_stl_const(_Columns, m_vecColumns, itor)
	{
		_Columns::value_type value = *itor;
		delete value;
	}

	for_stl_const(_Columns, m_vecColumnsRemoved, itor)
	{
		_Columns::value_type value = *itor;
		delete value;
	}
	
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
	m_pColumnSplitter->Render(pRenderer, pClipping);
}

GrColumn* GrColumnList::GetColumn(uint nIndex) const
{
	assert(nIndex < m_vecColumns.size());
	return m_vecColumns[nIndex];
}

GrCell::GrCell() : m_padding(5, 2, 5, 2)
{
	m_dwFlag			= 0;
	m_nTextLineHeight	= 0;

	m_pFont				= NULL;
	m_nID				= m_snID++;

	m_bTextBound		= false;
	m_bTextAlign		= false;
	m_pTextUpdater		= NULL;
}

GrCell::~GrCell()
{

}

GrPadding GrCell::GetPadding() const
{
	return m_padding;
}

void GrCell::SetPadding(GrPadding padding)
{
	m_padding = padding;
	SetTextBoundChanged();
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
	rtClient.left	= m_padding.left;
	rtClient.top	= m_padding.top;
	rtClient.right	= GetWidth()  - m_padding.right;
	rtClient.bottom	= GetHeight() - m_padding.bottom;
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
	if(GetTextMulitiline() == false)
		return 1;
	return m_vecTextLine.size();
}

const GrLineDesc* GrCell::GetTextLine(uint nIndex) const
{
	if(GetTextMulitiline() == false)
		return &m_textLine;
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
	const int nWidth  = GetWidth()  - (m_padding.left + m_padding.right);
	const int nHeight = GetHeight() - (m_padding.top  + m_padding.bottom);
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

		cl.x = ptStart.x + m_padding.left;
		cl.y = ptStart.y + m_padding.top;

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
	m_vecTextLine.clear();

	int nMaxWidth  = 0;
	int nMaxHeight = 0;

	GrRect rtOldTextBound = m_rtTextBound;
	int nCellWidth = GetWidth() - (m_padding.left + m_padding.right);
	//assert(nCellWidth > 0);

	if(GetTextMulitiline() == false)
	{
		GrTextUtil::SingleLine(&m_textLine, m_strText, pFont);
	}
	else
	{
		GrTextUtil::MultiLine(&m_vecTextLine, m_strText, nCellWidth, pFont, GetTextWordWrap());
	}

	m_nTextLineHeight = pFont->GetHeight();

	for(uint i=0 ; i<GetTextLineCount() ; i++)
	{
		const GrLineDesc* pLineDesc = GetTextLine(i);
		nMaxWidth	= std::max(nMaxWidth, pLineDesc->nWidth);
	}
	nMaxWidth	+= (m_padding.left + m_padding.right);

	nMaxHeight = GetTextLineCount() * m_nTextLineHeight + m_padding.top + m_padding.bottom;

	if(nMaxWidth > GetWidth() || nMaxHeight > GetHeight())
		AddFlag(GRCF_TEXT_CLIPPED);
	else
		RemoveFlag(GRCF_TEXT_CLIPPED);

	m_rtTextBound.left	= 0;
	m_rtTextBound.top	= 0;
	m_rtTextBound.right = nMaxWidth;
	m_rtTextBound.bottom= nMaxHeight;
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
			rtClip.Contract(m_padding);

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
	m_pTextUpdater->RemoveTextBound(this);
	m_pTextUpdater->RemoveTextAlign(this);
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
}

uint GrCell::GetID() const
{
	return m_nID;
}

void	 GrCell::SetBackColor(GrColor color)
{
	m_backColor = color;
}

void	 GrCell::SetForeColor(GrColor color)
{
	m_foreColor = color;
}

bool	 GrCell::GetMouseOvered() const
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

bool	 GrCell::GetMousePressed() const
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

bool GrItem::NeedToEnsure() const
{
	return (m_pColumn->NeedToEnsure() || m_pDataRow->NeedToEnsure());
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

GrState::Type GrItem::GetControlState() const
{
	if(GetMouseOvered() == false)
		return GrState::Normal;
	if(m_pGridCore->GetMouseOverState() == GrMouseOverState_Control)
	{
		if(GetMousePressed() == true)
			return GrState::Pressed;
		else 
			return GrState::MouseOver;
	}
	return GrState::Normal;
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
	GrColID columnID = pColumn->GetColumnID();
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

uint GrDataRow::GetCellCount() const
{
	return m_vecItems.size() + 1;
}

GrCell* GrDataRow::GetCell(uint nIndex)
{
	if(nIndex == 0)
		return this;
	return m_vecItems[nIndex - 1];
}

const GrCell* GrDataRow::GetCell(uint nIndex) const
{
	if(nIndex == 0)
		return this;
	return m_vecItems[nIndex - 1];
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

void GrDataRow::OnUpdatePositionCell(int /*x*/, GrUpdateDesc* pUpdateDesc)
{
	const GrRect* pColumnBound = m_pGridCore->GetColumnList()->GetBound();

	pUpdateDesc->nMin	= pColumnBound->left;
	pUpdateDesc->nMax	= pColumnBound->right;
}

GrColumn::GrColumn()
{
	//SetText(L"Column");
	m_pColumnList		= NULL;
	m_bVisible			= true;
	m_bReadOnly			= false;
	m_bCanBeSorted		= true;
	m_bMovable			= true;
	m_bResizable		= true;
	m_bFrozen			= false;
	m_bClippedTextVisible = true;
	m_bSelected			= false;
	m_bFullSelected		= false;
	m_bFitting			= false;
	m_bDisplayable		= false;
	m_bClipped			= false;

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
	m_pItemFont			= NULL;

	m_pGroupingInfo		= NULL;

	m_sortType			= GrSort::None;
	m_comparer[GrSort::None]	= 0;
	m_comparer[GrSort::Up]		= 0;
	m_comparer[GrSort::Down]	= 0;

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

GrColumn::~GrColumn()
{
	delete m_pGroupingInfo;
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
	return m_pGroupingInfo->GetGrouped();
}

void GrColumn::SetGrouped(bool b)
{
	m_pGroupingInfo->SetGrouped(b);
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

uint GrColumn::GetScrollableIndex() const
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
bool GrColumn::NeedToEnsure() const
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

void GrColumn::SetSortType(GrSort::Type sortType)
{
	if(m_bCanBeSorted == false)
		return;
	if(m_pColumnList == NULL)
		return;
	m_sortType = sortType;
	if(m_nIndex != INVALID_INDEX)
		m_pColumnList->NotifySortTypeChanged(this);
}

GrSort::Type GrColumn::GetSortType() const
{
	if(m_pColumnList == NULL)
		return GrSort::None;
	if(m_pColumnList->GetFirstSortColumn() != this)
		return GrSort::None;
	return m_sortType;
}

void GrColumn::SetSortComparer(GrSort::Type sortType, FuncComparer comparer)
{
	m_comparer[(int)sortType] = comparer;
}

FuncComparer GrColumn::GetSortComparer(GrSort::Type sortType) const
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

void GrColumn::SetWidth(int width) 
{ 
	m_nWidth = width;

	m_nWidth = std::max(m_nWidth, m_nMinWidth);
	m_nWidth = std::min(m_nWidth, m_nMaxWidth);

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

GrColor GrColumn::GetItemBackColor() const
{
	return m_itemBackColor;
}
GrColor GrColumn::GetItemForeColor() const
{
	return m_itemForeColor;
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

void GrColumn::SetItemBackColor(GrColor color)
{
	m_itemBackColor = color;
}

void GrColumn::SetItemForeColor(GrColor color)
{
	m_itemForeColor = color;
}

void GrColumn::SetItemFont(GrFont* pFont)
{
	m_pItemFont = pFont;
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

		GrSort::Type sortType = GetSortType();
		GrRect rtText = rtRender;
		if(sortType != GrSort::None)
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

	m_vecVisibleColumns.reserve(GetColumnCount());
	m_vecDisplayableColumns.reserve(GetColumnCount());

	m_nFrozenCount = 0;
	for_stl_const(_Columns, m_vecColumns, itor)
	{
		_Columns::value_type value = *itor;
		value->SetDisplayable(false);
		value->SetDisplayIndex(INVALID_INDEX);
		value->SetClipped(false);

		if(value->GetVisible() == false)
			continue;
		m_vecVisibleColumns.push_back(value);
	}

	sort(m_vecVisibleColumns.begin(), m_vecVisibleColumns.end(), SortColumn());

	uint nIndex = 0;
	for_stl_const(_Columns, m_vecVisibleColumns, itor)
	{
		_Columns::value_type value = *itor;
		if(value->GetFrozen() == true)
			m_nFrozenCount++;
		value->SetVisibleIndex(nIndex++);
	}

	m_pGridCore->SetWidthChanged();
}

void GrColumnList::AdjustColumnWidth()
{
	for_stl_const(_Columns, m_vecColumns, itor)
	{
		_Columns::value_type value = *itor;
		value->AdjustWidth();
	}
	m_pGridCore->SetWidthChanged();
}

void GrColumnList::OnPositionUpdated(GrPoint /*pt*/)
{
	m_mapColumnByPosition.clear();

	for_stl_const(_Columns, m_vecVisibleColumns, itor)
	{
		_Columns::value_type value = *itor;
		int nKey = value->GetX() + value->GetWidth() - 1;
		m_mapColumnByPosition.insert(_MapColumnPos::value_type(nKey, value));
	}	
}

void GrColumnList::OnUpdatePositionCell(int x, GrUpdateDesc* pUpdateDesc)
{
	GrRow::OnUpdatePositionCell(x, pUpdateDesc);
	x = CellStart();

	for(uint i=0 ; i<GetFrozenColumnCount() ; i++)
	{
		GrColumn* pColumn = GetFrozenColumn(i);
		pColumn->SetX(x);
		x += pColumn->GetWidth();
	}

	x += m_pColumnSplitter->GetWidth();

	m_nUnfrozenX = x;

	for(uint i=0 ; i<GetUnfrozenColumnCount() ; i++)
	{
		GrColumn* pColumn = GetUnfrozenColumn(i);
		pColumn->SetX(x);
		x += pColumn->GetWidth();
	}

	pUpdateDesc->nMax = std::max(pUpdateDesc->nMax, x);
}

void GrColumnList::OnColumnInserted(GrColumnEventArgs* e)
{
	ColumnInserted(this, e);
}

void GrColumnList::OnColumnRemoved(GrColumnEventArgs* e)
{
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

uint GrColumnList::GetVisibleColumnCount() const
{
	return m_vecVisibleColumns.size();
}

GrColumn* GrColumnList::GetVisibleColumn(uint nIndex) const
{
	assert(nIndex < m_vecVisibleColumns.size());
	return m_vecVisibleColumns[nIndex];
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
	m_nY = y;
}

void GrRow::SetHeight(int height)
{
	m_nHeight = height;
	OnHeightChanged();

	if(m_pGridCore == NULL)
		return;

	for(uint i=0 ; i<GetCellCount() ; i++)
	{
		GrCell* pCell = GetCell(i);
		m_pTextUpdater->AddTextAlign(pCell);
	}

	m_pGridCore->SetHeightChanged();
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
	for(uint i=0 ; i<GetCellCount() ; i++)
	{
		GrCell* pCell = GetCell(i);
		int nCellHeight = pCell->GetTextBound()->GetHeight() + DEF_LEADING;
		nHeight = std::max(nHeight, nCellHeight);
	}

	if(m_nHeight != nHeight && m_pTextUpdater != NULL)
	{
		for(uint i=0 ; i<GetCellCount() ; i++)
		{
			GrCell* pCell = GetCell(i);
			m_pTextUpdater->AddTextAlign(pCell);
		}
	}
	m_nHeight = nHeight;
	m_bFitting = false;
}

void GrRow::SetMinHeight(int height)
{
	m_nMinHeight = height;
}

int GrRow::GetMinHeight() const
{
	return m_nMinHeight;
}

void GrRow::SetMaxHeight(int height)
{
	m_nMaxHeight = height;
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

	m_nHeight			= DEF_ROW_HEIGHT;
	m_nMinHeight		= 0;
	m_nMaxHeight		= 10000;
	m_pParent			= NULL;
	m_nHierarchyLevel	= 0;
	m_bFitting			= false;
}

void GrRow::OnGridCoreAttached()
{
	GrCell::OnGridCoreAttached();
	m_pGridCore->SetHeightChanged();
}

void GrRow::Sort(GrSort::Type sortType)
{
	switch(sortType)
	{
	case GrSort::Up:
		Sort(GrSortFunc::SortRowsUp, 0);
		break;
	case GrSort::Down:
		Sort(GrSortFunc::SortRowsDown, 0);
		break;
	case GrSort::None:
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

const GrCell* GrRow::HitTest(GrPoint pt) const
{
	if(m_rtBound.IsIn(pt) == false)
		return NULL;

	for(uint i=0 ; i<GetCellCount() ; i++)
	{
		const GrCell* pCell = GetCell(i);
		if(pCell->GetVisible() == false)
			continue;
		if(pCell->IsIn(pt) == true)
		{
			//pCell->IsIn(pt);
			return pCell;
		}
	}

	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		const GrRow* pChild = GetChild(i);
		const GrCell* pHit = pChild->HitTest(pt);
		if(pHit)
			return pHit;
	}

	return NULL;
}

const GrCell* GrRow::HitDisplayTest(GrPoint pt) const
{
	if(GetDisplayable() == false)
		return NULL;

	if(pt.x > m_pGridCore->GetDisplayRect()->right)
		return NULL;

	for(uint i=0 ; i<GetCellCount() ; i++)
	{
		const GrCell* pCell = GetCell(i);
		if(pCell->GetVisible() == false)
			continue;
		if(pCell->GetDisplayable() == false)
			continue;

		GrRect rtDisplay = pCell->GetDisplayRect();
		if(rtDisplay.IsIn(pt) == true)
		{
			return pCell;
		}
	}

	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		const GrRow* pChild = GetChild(i);
		const GrCell* pHit = pChild->HitDisplayTest(pt);
		if(pHit)
			return pHit;
	}

	return NULL;
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

GrPoint GrRow::UpdatePosition(GrPoint pt, GrRowUpdate updateType)
{
	if(updateType & GrRowUpdate_Cell)
	{
		GrUpdateDesc updateDesc;
		OnUpdatePositionCell(pt.x, &updateDesc);
		m_rtBound.left	= updateDesc.nMin;
		m_rtBound.right = updateDesc.nMax;
	}

	if(updateType & GrRowUpdate_Row)
	{
		GrUpdateDesc updateDesc;
		OnUpdatePositionRow(pt.y, &updateDesc);
		m_rtBound.top	 = updateDesc.nMin;
		m_rtBound.bottom = updateDesc.nMax;
		pt.y = updateDesc.nMax;
	}

	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		GrRow* pChild = GetChild(i);
		if(pChild->GetVisible() == false)
			continue;
		pt	= pChild->UpdatePosition(pt, updateType);

		const GrRect* pChildBound = pChild->GetBound();
		if(updateType & GrRowUpdate_Cell)
		{
			m_rtBound.left		= std::min(pChildBound->left,	m_rtBound.left);
			m_rtBound.right		= std::max(pChildBound->right,	m_rtBound.right);
		}

		if(updateType & GrRowUpdate_Row)
		{
			m_rtBound.top		= std::min(pChildBound->top,	m_rtBound.top);
			m_rtBound.bottom	= std::max(pChildBound->bottom,	m_rtBound.bottom);
		}
	}

	OnPositionUpdated(pt);

	return pt;
}

void GrRow::OnHeightChanged()
{

}

void GrRow::OnUpdatePositionCell(int x, GrUpdateDesc* pUpdateDesc)
{
	pUpdateDesc->nMin = x;
	pUpdateDesc->nMax = x + GetWidth();
}

void GrRow::OnUpdatePositionRow(int y, GrUpdateDesc* pUpdateDesc)
{
	SetY(y);

	pUpdateDesc->nMin = y;
	pUpdateDesc->nMax = y + GetHeight();
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
	
	for_stl_const(_Items, m_vecItems, itor)
	{
		_Items::value_type value = *itor;
		m_pGridCore->AttachObject(value);
	}
}

void GrDataRow::OnGridCoreDetached()
{
	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
	if(pFocuser->GetFocusedRow() == this)
		pFocuser->Set(IFocusable::Null);
	SetSelected(false);

	IDataRow::OnGridCoreDetached();
}

void GrDataRow::ClearItem()
{
	for_stl_const(_Items, m_vecItems, itor)
	{
		_Items::value_type value = *itor;
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
	GrItem* pItem = new GrItem(pColumn, this);
	if(m_pGridCore != NULL)
		m_pGridCore->AttachObject(pItem);
	m_vecItems.push_back(pItem);
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
		backColor = backColor * 0.9f;

	else if(GetSelected() == true || GetSelecting() == true)
		backColor = backColor * 0.9f;

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
	GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

	GrFlag	renderStyle = ToRenderStyle();
	GrColor	foreColor	= GrColor::Black;
	GrColor backColor   = GetRenderingBackColor();

	if(GetClipped() == true)
		pRenderer->DrawHeader(renderStyle, &rtRender, backColor, pClipping);
	else
		pRenderer->DrawHeader(renderStyle, &rtRender, backColor);

	if(pDataRowList->GetRowNumberVisible() == true)
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
		else if(GetVisibleIndex() == pDataRowList->GetVisibleRowCount() - 1)
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
		throw std::exception("GridCore에 붙지 않았음");

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

bool IDataRow::NeedToEnsure() const
{
	if(GetVisible() == false)
		throw new std::exception();
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
		color.r = (byte)((float)color.r * 0.75f);
		color.g = (byte)((float)color.g * 0.75f);
		color.b = (byte)((float)color.b * 0.75f);
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

uint GrGroupingRow::GetCellCount() const
{
	return 2;
}

GrCell* GrGroupingRow::GetCell(uint nIndex)
{
	if(nIndex == 0)
		return this;
	return m_pLabel;
}

const GrCell* GrGroupingRow::GetCell(uint nIndex) const
{
	if(nIndex == 0)
		return this;
	return m_pLabel;
}

IFocusable* GrGroupingRow::GetFocusable(GrColumn* /*pColumn*/) const
{
	return m_pLabel;
}

GrCaption::GrCaption()
{
	SetHeight(20);
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
	m_bVisible = b;
	GrHeaderRow* pHeaderList = dynamic_cast<GrHeaderRow*>(GetParent());
	pHeaderList->SetVisibleChanged();
}

bool GrCaption::GetVisible() const
{
	return m_bVisible;
}

void GrCaption::OnFitted()
{
	GrRow::OnFitted();
	m_pGridCore->GetHeaderList()->SetFitChanged();
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
	ComputeLayout();
	m_bEnableGrouping = true;
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

GrColumn* GrGroupingList::GetGroupingColumn(uint nLevel) const
{
	return GetGrouping(nLevel)->GetColumn();
}

void GrGroupingList::ChangeGroupingInfo(GrGroupingInfo* pGroupingInfo, GrGroupingInfo* pWhere)
{
	assert(pGroupingInfo);
	if(pGroupingInfo == pWhere)
		return;
	_Groupings::iterator itor = remove(m_vecGroupings.begin(), m_vecGroupings.end(), pGroupingInfo);
	m_vecGroupings.erase(itor);

	if(pWhere)
		itor = find(m_vecGroupings.begin(), m_vecGroupings.end(), pWhere);
	else
		itor = m_vecGroupings.end();

	m_vecGroupings.insert(itor, pGroupingInfo);

	for(uint i=0 ; i<m_vecGroupings.size() ; i++)
	{
		m_vecGroupings[i]->m_nGroupingLevel = i;
	}

	Changed.Raise(this, &GrEventArgs::Empty);
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

void GrGroupingList::SetGroupingSortState(uint nLevel, GrSort::Type sortType)
{
	GrGroupingInfo* pGroupingInfo = m_vecGroupings[nLevel];
	pGroupingInfo->SetSortType(sortType);
}

bool GrGroupingList::GetVisible() const 
{
	return GetEnableGrouping(); 
}

bool GrGroupingList::GetEnableGrouping() const
{
	return m_bEnableGrouping;
}

void GrGroupingList::SetEnableGrouping(bool b)
{
	if(m_bEnableGrouping == b)
		return;
	m_bEnableGrouping = b;
	Changed(this, &GrEventArgs::Empty);
}

void GrGroupingList::NotifyGroupingChanged(GrGroupingInfo* pGroupingInfo, uint* pLevel)
{
	_Groupings::iterator itor = find(m_vecGroupings.begin(), m_vecGroupings.end(), pGroupingInfo);

	if(pGroupingInfo->GetGrouped() == true)
	{
		if(itor == m_vecGroupings.end())
		{
			*pLevel = m_vecGroupings.size();
			m_vecGroupings.push_back(pGroupingInfo);
		}
	}
	else
	{
		if(itor != m_vecGroupings.end())
		{
			*pLevel = INVALID_INDEX;
			m_vecGroupings.erase(itor);
		}
	}

	ComputeLayout();

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

void GrGroupingList::ComputeLayout()
{
	if(m_vecGroupings.size() == 0)
	{
		SetHeight(20);
		RemoveFlag(GRCF_HIDE_TEXT);
	}
	else
	{
		SetHeight(m_vecGroupings[0]->GetHeight() + 20);
		AddFlag(GRCF_HIDE_TEXT);
	}
}

uint GrGroupingList::GetCellCount() const
{
	return m_vecGroupings.size() + 1;
}

GrCell* GrGroupingList::GetCell(uint nIndex)
{
	if(m_vecGroupings.size() == nIndex)
		return this;
	return GetGrouping(nIndex);
}

const GrCell* GrGroupingList::GetCell(uint nIndex) const
{
	if(m_vecGroupings.size() == nIndex)
		return this;
	return GetGrouping(nIndex);
}

void GrGroupingList::OnGridCoreAttached()
{
	GrRow::OnGridCoreAttached();
	m_pGridCore->Cleared.Add(this, &GrGroupingList::gridCore_Cleared);
}

void GrGroupingList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	m_vecGroupings.clear();
	ComputeLayout();
}

void GrGroupingList::OnUpdatePositionRow(int y, GrUpdateDesc* pUpdateDesc)
{
	GrRow::OnUpdatePositionRow(y, pUpdateDesc);
	GrPoint pt;
	pt.y = GetY() + 10;
	for_stl_const(_Groupings, m_vecGroupings, itor)
	{
		_Groupings::value_type value = *itor;
		pt.x = value->GetX();
		value->SetPosition(pt);
	}
}

void GrGroupingList::OnUpdatePositionCell(int x, GrUpdateDesc* pUpdateDesc)
{
	GrRow::OnUpdatePositionCell(x, pUpdateDesc);
	GrPoint pt;
	pt.x = GetX();
	pt.y = GetY();

	pt.x += 10;
	pt.y += 10;
	for_stl_const(_Groupings, m_vecGroupings, itor)
	{
		_Groupings::value_type value = *itor;
		value->SetPosition(pt);
		pt.x += value->GetWidth() + 10;
	}
}

void GrGroupingList::OnFitted()
{
	GrRow::OnFitted();
	m_pGridCore->GetHeaderList()->SetFitChanged();
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

	for_stl_const(_Groupings, m_vecGroupings, itor)
	{
		GrGroupingInfo* pGroupingInfo = *itor;
		pGroupingInfo->Render(pRenderer, pClipping);
	}

	RenderText(pRenderer, foreColor, &rtRender);
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

	m_rtBound += pRow->m_rtBound;
}

void GrRow::ReserveChild(uint reserve)
{
	m_vecChilds.reserve(reserve);
}

void GrRow::ClearChild()
{
	for_stl_const(GrRowArray, m_vecChilds, itor)
	{
		GrRowArray::value_type value = *itor;
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
	SetHeight(DEF_ROW_HEIGHT);
	SetDisplayable(true);
	AddFlag(GRCF_SYSTEM_OBJCT);
}

void GrInsertionRow::OnGridCoreAttached()
{
	GrDataRow::OnGridCoreAttached();

	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	SetY(pColumnList->GetHeight());
}

void GrInsertionRow::SetVisible(bool b)
{
	GrDataRow::SetVisible(b);
	GrHeaderRow* pHeaderList = dynamic_cast<GrHeaderRow*>(GetParent());

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

void GrDataRowFinder::Build(const _DataRows* pRows)
{
	m_rows.clear();

	for_stl_const_ptr(_DataRows, pRows, itor)
	{
		_DataRows::value_type value = *itor;
		int nKey = value->GetY() + value->GetHeight() - 1;
		m_rows.insert(_Type::value_type(nKey, value));
	}
}

IDataRow* GrDataRowFinder::Find(int y) const
{
	_Type::const_iterator itor = m_rows.lower_bound(y);
	if(itor == m_rows.end())
		return NULL;

	return (*itor).second;
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


	for_stl_const(_DataRows, m_vecDataRows, itor)
	{
		_DataRows::value_type value = *itor;
		m_pGridCore->AttachObject(value);
	}

	for_stl_const(_GroupingRows, m_vecGroupingRows, itor)
	{
		_GroupingRows::value_type value = *itor;
		m_pGridCore->AttachObject(value);
	}
	
	m_pInsertionRow = m_pGridCore->GetInsertionRow();

	m_pInsertionRow->SetVisibleDataRowIndex(INSERTION_ROW);
	m_pInsertionRow->SetVisibleIndex(INSERTION_ROW);
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
	GrColumn* pColumn = pGroupingList->GetGroupingColumn(nGroupingLevel);

	_DataRows vecSort;

	FuncSortRow fnSort;
	switch(pColumn->GetGroupingInfo()->GetSortType())
	{
	case GrSort::Up:
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
	for_stl_const(_DataRows, m_vecDataRows, itor)
	{
		_DataRows::value_type value = *itor;
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
			GrRow::Sort(GrSort::None);
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
	for_stl_const(_IDataRows, m_vecVisibleRows, itor)
	{
		_IDataRows::value_type value = *itor;
		value->AdjustHeight();
	}
	m_pInsertionRow->AdjustHeight();
	m_pGridCore->SetHeightChanged();
	m_bFitChanged = false;

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

void GrDataRowList::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	for_stl_const(_IDataRows, m_vecDisplayableRows, itor)
	{
		_IDataRows::value_type value = *itor;

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
	for_stl_const(_GroupingRows, m_vecGroupingRows, itor)
	{
		_GroupingRows::value_type value = *itor;
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

	for_stl_const(GrRows, setParent, itor)
	{
		(*itor)->Sort(e->m_pGroupingInfo->GetSortType());
	}
	SetVisibleChanged();
}

void GrDataRowList::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrColumnList* pColumnList = m_pGridCore->GetColumnList();
	pColumnList->ColumnInserted.Add(this, &GrDataRowList::columnList_ColumnBinded);

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

	m_rowFinder.Build(&m_vecVisibleRows);
}

void GrDataRowList::columnList_ColumnBinded(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();

	if(pColumn->GetColumnID() >= m_vecColumns.size())
	{
		m_vecColumns.push_back(pColumn);
		m_pInsertionRow->AddItem(pColumn);
		for_stl_const(_DataRows, m_vecDataRows, itor)
		{
			_DataRows::value_type value = *itor;
			value->AddItem(pColumn);
		}

		for_stl_const(_DataRows, m_vecDataRowsRemoved, itor)
		{
			_DataRows::value_type value = *itor;
			value->AddItem(pColumn);
		}
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

	for_stl_const(GrRowArray, vecVisibles, itor)
	{
		GrRowArray::value_type value = *itor;
		IDataRow* pDataRowBase = dynamic_cast<IDataRow*>(value);
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
	}

	for_stl_const(_IDataRows, m_vecDisplayableRows, itor)
	{
		_IDataRows::value_type value = *itor;
		value->SetDisplayable(false);
		value->SetDisplayIndex(INVALID_INDEX);
		value->SetClipped(false);
	}

	m_vecDisplayableRows.clear();
	m_pGridCore->SetHeightChanged();

	VisibleChanged(this, &GrEventArgs::Empty);
}

void GrDataRowList::Update(bool force)
{
	if(m_bListChanged == true || force == true)
		BuildChildRowList();
	if(m_bVisibleChanged == true || force == true)
		BuildVisibleRowList();
	if(m_bFitChanged == true || force == true)
		AdjustRowHeight();

	m_bListChanged		= false;
	m_bVisibleChanged	= false;
	m_bFitChanged		= false;
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
		throw new std::exception("잘못된 인덱스");
	return m_vecVisibleDataRows[nIndex];
}

int GrDataRowList::GetRowWidth() const
{
	return m_nRowWidth;
}

void GrDataRowList::SetRowWidth(int nWidth)
{
	m_nRowWidth = nWidth;
	m_pGridCore->SetWidthChanged();
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
		throw new std::exception("잘못된 인덱스");
	return m_vecVisibleRows[nIndex];
}

void GrDataRowList::AddDataRow(GrDataRow* pDataRow)
{
	InsertDataRow(pDataRow, m_vecDataRows.size());
}

void GrDataRowList::InsertDataRow(GrDataRow* pDataRow, uint nIndex)
{
	if(pDataRow->GetDataRowIndex() != INVALID_INDEX || pDataRow == m_pInsertionRow)
		throw new std::exception("이미 등록되어 있습니다");

	if(pDataRow->GetDataRowID() == INVALID_INDEX)
	{
		pDataRow->SetDataRowID(m_nDataRowID++);
		pDataRow->Reserve(m_vecColumns.size());
		for_stl_const(_Columns, m_vecColumns, itor)
		{
			_Columns::value_type value = *itor;
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


	Update(true);
	m_rowFinder.Build(&m_vecVisibleRows);
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
	for_stl(_DataRows, m_vecDataRows, itor)
	{
		_DataRows::value_type value = *itor;
		value->SetDataRowIndex(INVALID_INDEX);
	}

	m_vecDataRowsRemoved.insert(m_vecDataRowsRemoved.end(), m_vecDataRows.begin(), m_vecDataRows.end());
	m_vecDataRows.clear();

	Update(true);
	m_rowFinder.Build(&m_vecVisibleRows);
}

void GrDataRowList::OnPositionUpdated(GrPoint /*pt*/)
{
	m_mapRowByPosition.clear();

	for_stl_const(_IDataRows, m_vecVisibleRows, itor)
	{
		_IDataRows::value_type value = *itor;
		int nKey = value->GetY() + value->GetHeight() - 1;
		m_mapRowByPosition.insert(_MapDataRowPos::value_type(nKey, value));
	}	

	m_rowFinder.Build(&m_vecVisibleRows);
}

void GrDataRowList::DeleteObjects()
{
	for_stl_const(_DataRows, m_vecDataRows, itor)
	{
		GrDataRow* pDataRow = *itor;
		delete pDataRow;
	}

	for_stl_const(_DataRows, m_vecDataRowsRemoved, itor)
	{
		_DataRows::value_type value = *itor;
		delete value;
	}

	for_stl_const(_GroupingRows, m_vecGroupingRows, itor)
	{
		_GroupingRows::value_type value = *itor;
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

void GrDataRowList::Clip(const GrRect* pDisplayRect, uint nVisibleStart)
{
	int nDisplayY = GetDisplayY();

	for_stl(_IDataRows, m_vecDisplayableRows, itor)
	{
		_IDataRows::value_type value = *itor;
		value->SetDisplayable(false);
		value->SetDisplayIndex(INVALID_INDEX);
		value->SetClipped(false);
	}
	m_vecDisplayableRows.clear();

	uint nDisplayIndex = 0;
	for(uint i=nVisibleStart ; i<GetVisibleRowCount() ; i++)
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
}

uint GrDataRowList::ClipFrom(uint nVisibleFrom) const
{
	return ClipFrom(m_pGridCore->GetDisplayRect(), nVisibleFrom);
}

uint GrDataRowList::ClipFrom(const GrRect* pDisplayRect, uint nVisibleFrom) const
{
	IDataRow* pDataRow = GetVisibleRow(nVisibleFrom);
	if(pDataRow == NULL)
		throw std::exception("잘못된 인덱스");

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
	for_stl_const(_DataRows, m_vecDataRows, itor)
	{
		_DataRows::value_type value = *itor;
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
}

void GrDataRowList::Sort(GrColumn* pColumn)
{
	FuncSortRow fnSort = (FuncSortRow)pColumn->GetSortComparer(pColumn->GetSortType());
	if(fnSort == NULL)
	{
		switch(pColumn->GetSortType())
		{
		case GrSort::Up:
				
			fnSort = GrSortFunc::SortDataRowSortUp;
			break;
		case GrSort::Down:
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

const GrCell* GrDataRowList::HitDisplayTest(GrPoint pt) const
{
	if(pt.y > m_nDisplayableBottom)
		return NULL;
	int y = DisplayToGlobal(pt.y);
	const IDataRow* pDataRow = m_rowFinder.Find(y);
	if(pDataRow == NULL)
		return NULL;
	if(y < pDataRow->GetY())
		return NULL;

	if(pt.x < pDataRow->GetDisplayX() + pDataRow->GetWidth())
		return pDataRow;

	if(pDataRow->GetRowType() == GrRowType_GroupingRow)
	{
		return pDataRow->GetCell(1);
	}

	GrColumnList* pColumnList = m_pGridCore->GetColumnList();

	if(pt.x >= pColumnList->GetDisplayableRight())
		return NULL;

	int x = pColumnList->DisplayToGlobal(pt.x);
	GrColumn* pColumn = pColumnList->HitTest(x);
	if(pColumn)
	{
		return ((GrDataRow*)pDataRow)->GetItem(pColumn);
	}

	return NULL;
}


GrColumnSplitter::GrColumnSplitter(GrColumnList* pColumnList) : m_pColumnList(pColumnList)
{
	AddFlag(GRCF_SYSTEM_OBJCT);
	AddFlag(GRCF_HIDE_TEXT);

	SetText(L"ColumnSplitter");
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
	return true;
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
	m_sortType = GrSort::Up;
	m_nGroupingLevel = INVALID_INDEX;
}

void GrGroupingInfo::OnGridCoreAttached()
{
	GrCell::OnGridCoreAttached();
	SetText(m_pColumn->GetText());
	m_pGroupingList = m_pGridCore->GetGroupingList();
	m_pGroupingList->NotifyGroupingChanged(this, &m_nGroupingLevel);
}

void GrGroupingInfo::OnGridCoreDetached()
{
	m_pGroupingList->NotifyGroupingChanged(this, &m_nGroupingLevel);
	m_pGroupingList = NULL;
	GrCell::OnGridCoreDetached();
}

void GrGroupingInfo::SetGrouped(bool b)
{
	if(m_bGrouped == b)
		return;
	m_bGrouped = b;
	SetText(m_pColumn->GetText());

	if(m_pGroupingList != NULL)
	{
		m_pGroupingList->NotifyGroupingChanged(this, &m_nGroupingLevel);
	}
}

bool GrGroupingInfo::GetGrouped() const
{
	return m_bGrouped;
}

void GrGroupingInfo::SetExpanded(bool b)
{
	if(m_pGroupingList->GetEnableGrouping() == false)
		return;
	m_bExpanded = b;
	m_pGroupingList->NotifyExpanded(this);
}

bool GrGroupingInfo::GetExpanded() const
{
	return m_bExpanded;
}

GrSort::Type GrGroupingInfo::GetSortType() const
{
	return m_sortType;
}

void GrGroupingInfo::SetSortType(GrSort::Type sortType)
{
	if(m_pGroupingList->GetEnableGrouping() == false)
		return;
	m_sortType = (sortType == GrSort::Up) ? GrSort::Up : GrSort::Down;
	m_pGroupingList->NotifySortChanged(this);
}

uint GrGroupingInfo::GetGroupingLevel() const
{
	return m_nGroupingLevel;
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
}

void GrRootRow::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		GrRow* pChild = GetChild(i);
		pChild->Render(pRenderer, pClipping);
	}
}

GrPoint GrRootRow::GetDisplayPosition() const
{
	return m_pGridCore->GetDisplayRect()->GetLocation();
}

GrHeaderRow::GrHeaderRow()
{
	SetText(L"HeaderList");
	AddFlag(GRCF_SYSTEM_OBJCT);
	m_bVisibleChanged	= true;
	m_bFitChanged		= false;
}

void GrHeaderRow::Update()
{
	if(m_bVisibleChanged == true)
		BuildVisibleList();

	if(m_bFitChanged == true)
		AdjustRowHeight();
	
	m_bVisibleChanged	= false;
	m_bFitChanged		= false;
}

void GrHeaderRow::BuildVisibleList()
{
	m_vecVisibleRows.clear();

	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		GrRow* pChild = GetChild(i);
		if(pChild->GetVisible() == true)
			m_vecVisibleRows.push_back(pChild);
	}
	m_pGridCore->SetHeightChanged();
}

void GrHeaderRow::AdjustRowHeight()
{
	for(uint i=0 ; i<GetChildCount() ; i++)
	{
		GrRow* pChild = GetChild(i);
		pChild->AdjustHeight();
	}
	m_pGridCore->SetHeightChanged();
}

void GrHeaderRow::SetVisibleChanged()
{
	m_bVisibleChanged = true;
}

void GrHeaderRow::SetFitChanged()
{
	m_bFitChanged = true;
}

void GrHeaderRow::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	for_stl_const(_Rows, m_vecVisibleRows, itor)
	{
		_Rows::value_type value = *itor;
		value->Render(pRenderer, pClipping);
	}
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