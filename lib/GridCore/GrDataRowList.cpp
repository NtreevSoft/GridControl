#include "GrDataRowList.h"
#include "GrGridCore.h"
#include "GrColumnList.h"
#include <assert.h>

GrDataRowList::GrDataRowList()
{
	SetText(L"DataRowList");

	m_rowWidth = 50;
	m_displayableBottom = 0;
	m_margin = 0;

	m_groupCount = 0;
	m_usedGroupRow = 0;
	m_pFocusedDataRow = nullptr;
	m_pInsertionRow = CreateInsertionRow();
	m_dataRowID = 0;
	m_displayableHeight = 0;
	m_visibleRowNumber = true;
	m_clippedIndex = INVALID_INDEX;
	m_zeroBasedRowIndex = false;

	m_listChanged = false;
	m_visibleChanged = false;
	m_fitChanged = false;
	m_updating = false;

	m_pItems = nullptr;
	m_itemsCount = 0;;
}

void GrDataRowList::OnGridCoreAttached()
{
	GrRow::OnGridCoreAttached();

	m_vecDataRows.reserve(m_pGridCore->GetReservedRow());

	GrGroupPanel* pGroupPanel = m_pGridCore->GetGroupPanel();

	pGroupPanel->Changed.Add(this, &GrDataRowList::groupPanel_Changed);
	pGroupPanel->Expanded.Add(this, &GrDataRowList::groupPanel_Expanded);
	pGroupPanel->SortChanged.Add(this, &GrDataRowList::groupPanel_SortChanged);

	GrFocuser* pFocuser = m_pGridCore->GetFocuser();
	pFocuser->FocusChanged.Add(this, &GrDataRowList::focuser_FocusedChanged);

	m_pGridCore->Cleared.Add(this, &GrDataRowList::gridCore_Cleared);
	m_pGridCore->Created.Add(this, &GrDataRowList::gridCore_Created);
	m_pGridCore->FontChanged.Add(this, &GrDataRowList::gridCore_FontChanged);

	for(auto item : m_vecDataRows)
	{
		m_pGridCore->AttachObject(item);
	}

	for(auto item : m_vecGroupRows)
	{
		m_pGridCore->AttachObject(item);
	}

	m_pGridCore->AttachObject(m_pInsertionRow);
}

void GrDataRowList::OnYChanged()
{
	RepositionVisibleRowList();
}

void GrDataRowList::OnDataRowInserting(GrDataRowInsertingEventArgs* e)
{
	DataRowInserting(this, e);
}

void GrDataRowList::OnDataRowInserted(GrDataRowInsertedEventArgs* e)
{
	SetListChanged();
	m_pGridCore->Invalidate();

	DataRowInserted(this, e);
}

void GrDataRowList::OnDataRowRemoved(GrDataRowEventArgs* e)
{
	GrDataRow* pDataRow = e->GetDataRow();

	pDataRow->SetDataRowIndexCore(INVALID_INDEX);
	pDataRow->SetSelected(false);
	pDataRow->KillFocus();

	SetListChanged();
	m_pGridCore->Invalidate();

	DataRowRemoved(this, e);
}

void GrDataRowList::OnDataRowMoved(GrDataRowEventArgs* e)
{
	SetListChanged();
	m_pGridCore->Invalidate();
	m_pGridCore->Update();
	DataRowMoved(this, e);
}

void GrDataRowList::OnVisibleHeightChanged(GrEventArgs* e)
{
	VisibleHeightChanged(this, e);
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
	}

	pParent->AddChild(pGroupRow);

	pGroupRow->SetTextVisible(false);
	pGroupRow->SetReference(pColumn, itemText);
	pGroupRow->m_groupLevel = 0;

	auto itor = m_mapCache.find(pGroupRow->GetKey());
	if(itor == m_mapCache.end())
	{
		const GrPadding padding = pGroupRow->GetPadding();
		int height = GetPaintingFont()->GetHeight() + padding.GetVertical();
		pGroupRow->SetHeight(height);
		pGroupRow->Expand(true);
	}
	else
	{
		pGroupRow->SetHeight(itor->second.height);
		pGroupRow->Expand(itor->second.expanded);
	}

	m_pGridCore->AttachObject(pGroupRow);

	m_usedGroupRow++;
	return pGroupRow;
}

GrDataRow* GrDataRowList::CreateInsertionRow()
{
	GrDataRow* pInsertionRow = new GrDataRow();
	pInsertionRow->SetText(L"☞");
	pInsertionRow->SetVisibleDataRowIndex(INSERTION_ROW);
	pInsertionRow->SetVisibleIndex(INSERTION_ROW);
	pInsertionRow->SetDisplayIndex(INSERTION_ROW);
	pInsertionRow->SetDataRowID(INSERTION_ROW);
	pInsertionRow->SetDisplayable(true);

	return pInsertionRow;
}

GrDataRowList::~GrDataRowList()
{
	delete m_pInsertionRow;
	DeleteObjects();
}

int GrDataRowList::CellStart() const
{
	int start = GetX();
	if(m_pGridCore->GetRowVisible() == true)
		start += m_rowWidth;
	return start + m_margin;
}

uint GrDataRowList::GetMargin() const
{
	return m_margin;
}

void GrDataRowList::BuildGroup(GrRow* pParent, uint groupLevel)
{
	GrGroupPanel* pGroupPanel = m_pGridCore->GetGroupPanel();
	m_pGridCore->AttachObject(pGroupPanel);
	GrColumn* pColumn = pGroupPanel->GetGroup(groupLevel)->GetColumn();

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
	auto itor = vecSort.begin();
	GrGroupRow* pGroupRow = nullptr;

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
	for(auto value : m_vecDataRows)
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
			GrRow::Sort(GrSortFunc::SortDataRowSortNone, nullptr);
	}
	else
	{
		BuildCache();

		for(auto value : m_vecGroupRows)
		{
			value->ClearChild();
		}
		m_usedGroupRow = 0;
		BuildGroup(this, 0);
	}

	SetVisibleChanged();
}

void GrDataRowList::AdjustRowHeight()
{
	for(auto value : m_vecVisibleRows)
	{
		value->AdjustHeight();
	}

	GetInsertionRow()->AdjustHeight();

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
	for(auto value : m_vecDisplayableRows)
	{
		int y = value->GetY();
		int b = value->GetBottom();
		if(y >= clipRect.bottom || b < clipRect.top)
			continue;

		value->Paint(pPainter, clipRect);
	}

	if(m_pFocusedDataRow != nullptr && m_pFocusedDataRow->GetDisplayable() == true)
	{
		if(m_pGridCore->GetRowHighlight() == true && m_pGridCore->GetRowHighlightType() != GrRowHighlightType_Fill)
		{
			GrRect highlightRect = m_pFocusedDataRow->GetRect();
			const GrStyle* pStyle = m_pGridCore->GetStyle();
			if(pStyle == nullptr)
				pStyle = &GrStyle::Default;
			GrRect displayRect = m_pGridCore->GetDisplayRect();
			highlightRect.top--;
			if(m_pGridCore->GetFillBlank() == false)
				highlightRect.right = m_pGridCore->GetColumnList()->GetBounds().right;
			else
				highlightRect.right = displayRect.right;
			pPainter->DrawRectangle(highlightRect, pStyle->RowHighlightLineColor);
		}
	}
}

void GrDataRowList::Reserve(uint reserve)
{
	m_vecDataRows.reserve(reserve);
	m_vecDataRowsRemoved.reserve(reserve);
}

void GrDataRowList::groupPanel_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	GrGroupPanel* pGroupPanel = m_pGridCore->GetGroupPanel();
	m_groupCount = pGroupPanel->GetGroupCount();
	m_clippedIndex = INVALID_INDEX;

	uint groupCount = pGroupPanel->GetGroupCount();
	if(groupCount == 0)
		m_margin = 0;
	else
		m_margin = (groupCount + 1) * DEF_GROUP_WIDTH;

	BuildChildRowList();
}

void GrDataRowList::groupPanel_Expanded(GrObject* /*pSender*/, GrGroupEventArgs* e)
{
	GrColumn* pColumn = e->m_pGroup->GetColumn();
	for(auto value : m_vecGroupRows)
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

	for(auto value : parentRows)
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
	pColumnList->ColumnPaddingChanged.Add(this, &GrDataRowList::columnList_ColumnPaddingChanged);
}

void GrDataRowList::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	ClearChild();

	m_groupCount = 0;
	m_usedGroupRow = 0;
	m_pFocusedDataRow = nullptr;
	m_dataRowID = 0;
	m_margin = 0;

	GetInsertionRow()->ClearItem();

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
	vecDataRows.push_back(GetInsertionRow());

	GrTextUpdater* pTextUpdater = m_pGridCore->GetTextUpdater();

	for(auto value : vecDataRows)
	{
		GrDataRow* pDataRow = value;
		pTextUpdater->AddTextBounds(pDataRow);

		for(uint i=0 ; i<pColumnList->GetColumnCount() ; i++)
		{
			GrColumn* pColumn = pColumnList->GetColumn(i);
			GrItem* pItem = pDataRow->GetItem(pColumn);
			pTextUpdater->AddTextBounds(pItem);
		}
	}

	for(auto value : m_vecGroupRows)
	{
		pTextUpdater->AddTextBounds(value);
		pTextUpdater->AddTextBounds(value->GetLabel());
	}
}

void GrDataRowList::columnList_ColumnInserted(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();

	if(pColumn->GetColumnID() >= m_vecColumns.size())
	{
		m_vecColumns.push_back(pColumn);
	}

	GetInsertionRow()->AddItem(pColumn);
	for(auto value : m_vecDataRows)
	{
		value->AddItem(pColumn);
	}

	for(auto value : m_vecDataRowsRemoved)
	{
		value->AddItem(pColumn);
	}
}

void GrDataRowList::columnList_ColumnRemoved(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrItem* pItem = GetInsertionRow()->GetItem(e->GetColumn());
	m_pGridCore->DetachObject(pItem);

	for(auto value : m_vecDataRows)
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

void GrDataRowList::columnList_ColumnPaddingChanged(GrObject* /*pSender*/, GrColumnEventArgs* /*e*/)
{
	m_pInsertionRow->SetFit();
	for(auto value : m_vecDataRows)
	{
		value->SetFit();
	}
}

void GrDataRowList::focuser_FocusedChanged(GrObject* pSender, GrFocusChangeArgs* /*e*/)
{
	GrFocuser* pFocuser = (GrFocuser*)pSender;
	m_pFocusedDataRow = dynamic_cast<GrDataRow*>(pFocuser->GetFocusedRow());
}

void GrDataRowList::GetVisibleList(GrRow* pRow, GrRowArray* pVisible) const
{
	if(pRow->GetVisible() == false)
		return;
	pVisible->push_back(pRow);
	for(uint i=0 ; i<pRow->GetChildCount() ; i++)
	{
		GrRow* pChild = pRow->GetChild(i);
		GetVisibleList(pChild, pVisible);
	}
}

void GrDataRowList::GetMaxDepth(GrRow* pRow, uint* depth) const
{
	*depth = std::max(*depth, pRow->GetDepth());
	for(uint i=0 ; i<pRow->GetChildCount() ; i++)
	{
		GrRow* pChild = pRow->GetChild(i);

		GetMaxDepth(pChild, depth);
	}
}

void GrDataRowList::BuildVisibleRowList()
{
	if(m_updating == true)
		return;
	m_updating = true;
	GrRowArray vecVisibles;
	vecVisibles.reserve(m_pGridCore->GetReservedRow());
	uint level = 0;
	GetVisibleList(this, &vecVisibles);
	GetMaxDepth(this, &level);

	uint temp = level - GetDepth() ;
	if(temp <= 1)
		m_margin = 0;
	else
		m_margin = (temp - 1) * DEF_GROUP_WIDTH;

	std::set<IDataRow*> oldVisibles(m_vecVisibleRows.begin(), m_vecVisibleRows.end());

	m_vecVisibleRows.clear();
	m_vecVisibleRows.reserve(vecVisibles.size());
	m_vecVisibleDataRows.clear();
	m_vecVisibleDataRows.reserve(vecVisibles.size());

	for(auto value : vecVisibles)
	{
		IDataRow* pDataRowBase = dynamic_cast<IDataRow*>(value);
		if(pDataRowBase == nullptr)
			continue;
		GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowBase);
		if(pDataRow)
		{
			pDataRow->SetVisibleDataRowIndex(m_vecVisibleDataRows.size());
			m_vecVisibleDataRows.push_back(pDataRow);
		}
		pDataRowBase->SetVisibleIndex(m_vecVisibleRows.size());
		m_vecVisibleRows.push_back(pDataRowBase);

		oldVisibles.erase(pDataRowBase);
	}

	for(auto value : oldVisibles)
	{
		value->SetVisibleIndex(INVALID_INDEX);
	}

	for(auto value : m_vecDisplayableRows)
	{
		value->SetDisplayable(false);
		value->SetDisplayIndex(INVALID_INDEX);
		value->SetClipped(false);
	}

	m_vecDisplayableRows.clear();
	m_heightChanged = true;

	m_updating = false;
}

void GrDataRowList::RepositionVisibleRowList()
{
	int oldVisibleHeight = m_visibleHeight;
	int y = GetY();
	for(auto value : m_vecVisibleRows)
	{
		value->SetY(y);
		y += value->GetHeight();
	}
	m_visibleBottom = y;
	m_visibleHeight = m_visibleBottom - GetY();
	m_clippedIndex = INVALID_INDEX;

	if(m_visibleHeight != oldVisibleHeight)
	{
		OnVisibleHeightChanged(&GrEventArgs::Empty);
	}
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
		return GetInsertionRow();
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
	if(pDataRow->GetDataRowIndex() != INVALID_INDEX)
		throw _Exception("이미 등록되어 있습니다");

	GrDataRowInsertType insertType = pDataRow == m_pInsertionRow ? GrDataRowInsertType_Insertion : GrDataRowInsertType_New;

	GrDataRowInsertingEventArgs e1(pDataRow, insertType);
	OnDataRowInserting(&e1);

	if(e1.GetCancel() == true)
		return;

	/*    if(pDataRow->GetDataRowID() == INVALID_INDEX)
	{
	pDataRow->SetDataRowID(m_dataRowID++);
	pDataRow->Reserve(m_vecColumns.size());
	for_each(_Columns, m_vecColumns, value)
	{
	pDataRow->AddItem(value);
	}
	}
	else */if(pDataRow->GetDataRowID() == INSERTION_ROW)
	{
		pDataRow->SetDataRowID(m_dataRowID++);
	}
	else
	{
		_DataRows::iterator itor = std::find(m_vecDataRowsRemoved.begin(), m_vecDataRowsRemoved.end(), pDataRow);
		m_vecDataRowsRemoved.erase(itor);
	}

	m_pGridCore->AttachObject(pDataRow);

	_DataRows::iterator itor = m_vecDataRows.insert(m_vecDataRows.begin() + index, pDataRow);
	index = std::min(index, m_vecDataRows.size());
	for( ; itor != m_vecDataRows.end() ; itor++)
	{
		(*itor)->SetDataRowIndexCore(index++);
	}

	if(pDataRow == m_pInsertionRow)
	{
		m_pInsertionRow = CreateInsertionRow();
		m_pGridCore->AttachObject(m_pInsertionRow);

		m_pInsertionRow->Reserve(m_vecColumns.size());
		for(auto value : m_vecColumns)
		{
			m_pInsertionRow->AddItem(value);
		}
	}

	GrDataRowInsertedEventArgs e2(pDataRow, insertType);
	OnDataRowInserted(&e2);
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
		(*next)->SetDataRowIndexCore(dataRowIndex);
		dataRowIndex++;
	}

	m_vecDataRowsRemoved.push_back(pDataRow);
	m_vecDataRows.erase(itor);

	GrDataRowEventArgs e(pDataRow);
	OnDataRowRemoved(&e);
}

void GrDataRowList::MoveDataRow(GrDataRow* pDataRow, uint index)
{
	if(pDataRow->GetDataRowIndex() == index)
		return;

	_DataRows::iterator itor = std::find(m_vecDataRows.begin(), m_vecDataRows.end(), pDataRow);
	
	if(index >= m_vecDataRows.size())
	{
		m_vecDataRows.erase(itor);
		m_vecDataRows.push_back(pDataRow);
	}
	else
	{
		GrDataRow* pTargetDataRow = m_vecDataRows[index];
		m_vecDataRows.erase(itor);
		_DataRows::iterator itorAt = std::find(m_vecDataRows.begin(), m_vecDataRows.end(), pTargetDataRow);
		m_vecDataRows.insert(itorAt, pDataRow);
	}

	index = 0;
	for(auto item : m_vecDataRows)
	{
		item->SetDataRowIndexCore(index++);
	}
	
	GrDataRowEventArgs e(pDataRow);
	OnDataRowMoved(&e);
}

GrDataRow* GrDataRowList::NewDataRowFromInsertion()
{
	AddDataRow(m_pInsertionRow);
	m_pGridCore->Update();

	return m_pInsertionRow;
}

GrDataRow* GrDataRowList::NewDataRow()
{
	GrDataRow* pDataRow = new GrDataRow();
	pDataRow->SetDataRowID(m_dataRowID++);
	pDataRow->Reserve(m_vecColumns.size());
	for(auto value : m_vecColumns)
	{
		pDataRow->AddItem(value);
	}
	m_vecDataRowsRemoved.push_back(pDataRow);
	return pDataRow;
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
	m_pGridCore->GetItemSelector()->ClearSelection();
	m_pGridCore->GetFocuser()->Reset();
	for(auto value : m_vecDataRows)
	{
		value->SetDataRowIndexCore(INVALID_INDEX);
	}

	m_vecDataRowsRemoved.insert(m_vecDataRowsRemoved.end(), m_vecDataRows.begin(), m_vecDataRows.end());
	m_vecDataRows.clear();

	Update(true);
	m_pGridCore->Invalidate();
}

void GrDataRowList::DeleteObjects()
{
	for(auto value : m_vecDataRows)
	{
		delete value;
	}

	for(auto value : m_vecDataRowsRemoved)
	{
		delete value;
	}

	for(auto value : m_vecGroupRows)
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

int GrDataRowList::GetVisibleBottom() const
{
	return m_visibleBottom;
}

IDataRow* GrDataRowList::HitTest(int y) const
{
	for(auto value : m_vecDisplayableRows)
	{
		if(y >= value->GetY() && y < value->GetBottom())
			return value;
	}
	return nullptr;
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
			if(pDataRow != nullptr)
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

	Update();

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
	m_pGridCore->Update();
}

void GrDataRowList::DisplayFirst(IDataRow* pDataRow)
{
	GrScroll* pVertScroll = m_pGridCore->GetVertScroll();
	pVertScroll->SetValue(pDataRow->GetVisibleIndex());
	m_pGridCore->Invalidate();
	m_pGridCore->Update();
}

int GrDataRowList::GetDisplayOffset() const
{
	return m_offset;
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

	std::set<IDataRow*> oldDisplayableRows(m_vecDisplayableRows.begin(), m_vecDisplayableRows.end());

	m_vecDisplayableRows.clear();

	uint displayIndex = 0;

	m_offset = 0;
	for(uint i=0 ; i<vertical ; i++)
	{
		m_offset += m_vecVisibleRows[i]->GetHeight();
	}

	_IDataRows vecVisibleRows(m_vecVisibleRows.begin() + vertical, m_vecVisibleRows.end());
	for(auto item : vecVisibleRows)
	{
		if(displayY > displayRect.bottom)
			break;

		bool clipped = displayY + item->GetHeight() > displayRect.bottom;

		//item->SetY(displayY);
		item->SetDisplayable(true);
		item->SetDisplayIndex(displayIndex);
		item->SetClipped(clipped);
		item->OnYChanged();

		m_vecDisplayableRows.push_back(item);
		displayY += item->GetHeight();
		displayIndex++;

		oldDisplayableRows.erase(item);
	}

	for(auto item : oldDisplayableRows)
	{
		item->SetDisplayable(false);
		item->SetDisplayIndex(INVALID_INDEX);
		item->SetClipped(false);
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

GrInsertionRow* GrDataRowList::GetInsertionRow() const
{
	return m_pInsertionRow;
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

	for(uint i=visibleTo ; i<m_vecVisibleRows.size() ; i--)
	{
		IDataRow* item = m_vecVisibleRows[i];
		displayY -= item->GetHeight();
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
	for(auto item : m_vecDataRows)
	{
		item->SetDataRowIndexCore(index++);
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
	if(fnSort == nullptr)
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
	if(pHitted == nullptr)
		return nullptr;

	for(auto item : m_vecDisplayableRows)
	{
		GrCell* pSubHitted = item->HitTest(location);
		if(pSubHitted != nullptr)
			return pSubHitted;
	}

	return nullptr;
}