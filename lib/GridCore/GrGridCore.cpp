//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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
#include "GrGridCore.h"
#include "GrGridRenderer.h"
#include "GrGridInternal.h"
#include <assert.h>

class GrDefaultInvalidator : public GrGridInvalidator
{
public:
	void Invalidate() {};
	void Invalidate(int /*x*/, int /*y*/, int /*width*/, int /*height*/) {};
	void Lock() {};
	void Unlock() {};
	void Reset() {};
	bool IsInvalidated() const { return true; }

};

class GrMouseOverer : public GrObject
{
public:
	GrMouseOverer()
	{
		m_pMouseOvered = NULL;
	}

	bool SetMouseOver(GrCell* pCell, GrPoint ptLocalPos)
	{
		bool bSuccess = false;
		if(m_pMouseOvered != pCell)
			bSuccess = true;

		if(bSuccess == true)
		{
			if(m_pMouseOvered != NULL)
				m_pGridCore->Invalidate(m_pMouseOvered->GetDisplayRect());
			if(pCell != NULL)
				m_pGridCore->Invalidate(pCell->GetDisplayRect());
		}

		m_pMouseOvered = pCell;

		GrMouseOverState state;
		if(m_pMouseOvered)
			state = m_pMouseOvered->HitMouseOverTest(ptLocalPos);
		else
			state = GrMouseOverState_None;

		if(m_mouseOverState != state)
			bSuccess = true;
		m_mouseOverState = state;
		return bSuccess;
	}

	GrCell* GetMouseOver() const
	{
		return m_pMouseOvered;
	}

	GrMouseOverState GetMouseOverState() const
	{
		return m_mouseOverState;
	}

protected:
	virtual void OnGridCoreAttached()
	{
		GrObject::OnGridCoreAttached();
		m_pGridCore->Cleared.Add(this, &GrMouseOverer::gridCore_Cleared);
	}

private:
	void gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
	{
		m_pMouseOvered	 = NULL;
		m_mouseOverState = GrMouseOverState_None;
	}

private:
	GrCell*					m_pMouseOvered;
	GrMouseOverState		m_mouseOverState;
};

class GrMousePresser : public GrObject
{
public:
	GrMousePresser()
	{
		m_pMousePressed	 = NULL;
	}

	void SetMousePressed(GrCell* pCell)
	{
		if(m_pMousePressed != NULL)
			m_pGridCore->Invalidate(m_pMousePressed->GetDisplayRect());
		if(pCell != NULL)
			m_pGridCore->Invalidate(pCell->GetDisplayRect());

		m_pMousePressed = pCell;
	}

	void SetMouseUnpressed()
	{
		if(m_pMousePressed != NULL)
			m_pGridCore->Invalidate(m_pMousePressed->GetDisplayRect());
		m_pMousePressed = NULL;
	}

	GrCell* GetMousePressed() const
	{
		return m_pMousePressed;
	}

protected:
	virtual void OnGridCoreAttached()
	{
		GrObject::OnGridCoreAttached();
		m_pGridCore->Cleared.Add(this, &GrMousePresser::gridCore_Cleared);
	}

private:
	void gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
	{
		m_pMousePressed	 = NULL;
	}

private:
	GrCell*	m_pMousePressed;
};

void GrGridCore::OnFontChanged()
{
	int nDefHeight = (int)(m_pFont->GetHeight() / 0.56f);
	m_pColumnList->SetHeight(nDefHeight);
	m_pInsertionRow->SetHeight(nDefHeight);
}

void SignalFunc(int /*sigNum*/)
{
	throw _Exception("");
}

GrGridCore::GrGridCore(void)
{
	m_pInvalidator			= NULL;
	m_pDefaultInvalidator	= new GrDefaultInvalidator();
	m_nCreatedCell			= 0;
	m_nAttachedCount		= 0;

	m_nGroupingMargin		= 0;
	m_dwFlag				= 0;

	m_rtDisplay.DoEmpty();

	m_bMarginVisible		= false;
	m_bUpdating				= false;

	m_bShowClippedText		= true;

	m_bAutoFitColumn		= false;
	m_bAutoFitRow			= false;
	m_nColumnSplitter		= 10;
	m_nRowSplitter			= 3;

	m_nReservedColumn		= 0;
	m_nReservedRow			= 0;

	m_bFullRowSelect		= false;
	m_bSelectionVisible		= true;
	m_bRowHighlight			= false;
	m_rowHighlightType		= GrRowHighlightType_Fill;

	m_pFont					= NULL;
	m_pDefaultStyle			= new GrStyle;
	m_pStyle				= NULL;

	m_pItemSelector	= new GrItemSelector();
	m_pFocuser		= new GrFocuser();
	m_pHitTester	= new GrHitTester();
	m_pTextUpdater	= new GrTextUpdater();
	m_pMouseOverer	= new GrMouseOverer();
	m_pMousePresser	= new GrMousePresser();
	m_pRootRow		= new GrRootRow();
	m_pCaption		= new GrCaption();
	m_pGroupingList	= new GrGroupingList();
	m_pColumnList	= new GrColumnList();
	m_pDataRowList	= new GrDataRowList();
	m_pInsertionRow = new GrInsertionRow();
	m_pSplitterRow	= new GrRowSplitter();

	AttachObject(m_pItemSelector);
	AttachObject(m_pFocuser);
	AttachObject(m_pHitTester);
	AttachObject(m_pTextUpdater);
	AttachObject(m_pMouseOverer);
	AttachObject(m_pMousePresser);
	AttachObject(m_pRootRow);
	AttachObject(m_pCaption);
	AttachObject(m_pGroupingList);
	AttachObject(m_pColumnList);
	AttachObject(m_pDataRowList);
	AttachObject(m_pInsertionRow);
	AttachObject(m_pSplitterRow);

	m_pRootRow->AddChild(m_pCaption);
	m_pRootRow->AddChild(m_pGroupingList);
	m_pRootRow->AddChild(m_pColumnList);
	m_pRootRow->AddChild(m_pInsertionRow);
	m_pRootRow->AddChild(m_pSplitterRow);
	m_pRootRow->AddChild(m_pDataRowList);

	m_pGroupingList->Changed.Add(this, &GrGridCore::groupingList_Changed);
	m_pColumnList->ColumnInserted.Add(this, &GrGridCore::columnList_ColumnInserted);
	m_pColumnList->ColumnRemoved.Add(this, &GrGridCore::columnList_ColumnRemoved);
	m_pColumnList->ColumnWidthChanged.Add(this, &GrGridCore::columnList_ColumnWidthChanged);
	m_pColumnList->ColumnWordwrapChanged.Add(this, &GrGridCore::columnList_ColumnWordwrapChanged);
	m_pColumnList->ColumnHorzAlignChanged.Add(this, &GrGridCore::columnList_ColumnHorzAlignChanged);
	m_pColumnList->ColumnVertAlignChanged.Add(this, &GrGridCore::columnList_ColumnVertAlignChanged);
	m_pColumnList->ColumnPaddingChanged.Add(this, &GrGridCore::columnList_ColumnPaddingChanged);

	OnCreated(&GrEventArgs::Empty);
}

GrGridCore::~GrGridCore(void)
{
	delete m_pMousePresser;
	delete m_pMouseOverer;
	delete m_pHitTester;
	delete m_pFocuser;
	delete m_pItemSelector;

	delete m_pSplitterRow;
	delete m_pInsertionRow;
	delete m_pDataRowList;
	delete m_pColumnList;
	delete m_pGroupingList;
	delete m_pCaption;
	delete m_pRootRow;

	delete m_pStyle;

	delete m_pDefaultInvalidator;

	if(m_nCreatedCell != 0)
		throw _Exception("Some obejcts are not deleted");
}

void GrGridCore::Reserve(uint nColumnCount, uint nRowCount)
{
	m_nReservedColumn = nColumnCount + 2;
	m_nReservedRow	  = nRowCount + 2;

	m_pDataRowList->Reserve(m_nReservedRow);
	m_pColumnList->Reserve(m_nReservedColumn);

	OnCapacityChanged(&GrEventArgs::Empty);
}

uint GrGridCore::GetReservedColumn() const
{
	return m_nReservedColumn;
}

uint GrGridCore::GetReservedRow() const
{
	return m_nReservedRow;
}

bool GrGridCore::SetMouseOver(GrCell* pCell, GrPoint ptLocalPos)
{
	return m_pMouseOverer->SetMouseOver(pCell, ptLocalPos);
}

GrCell* GrGridCore::GetMouseOver() const
{
	return m_pMouseOverer->GetMouseOver();
}

GrMouseOverState GrGridCore::GetMouseOverState() const
{
	return m_pMouseOverer->GetMouseOverState();
}

void GrGridCore::SetMousePressed(GrCell* pCell)
{
	m_pMousePresser->SetMousePressed(pCell);	
}

void GrGridCore::SetMouseUnpressed()
{
	m_pMousePresser->SetMouseUnpressed();	
}

GrCell* GrGridCore::GetMousePressed() const
{
	return m_pMousePresser->GetMousePressed();
}

bool GrGridCore::GetMarginVisible() const
{
	return m_bMarginVisible;
}

void GrGridCore::SetMarginVisible(bool b)
{
	m_bMarginVisible = b;
}

bool GrGridCore::GetInsertionRowVisible() const
{
	return m_pInsertionRow->GetVisible();
}

bool GrGridCore::GetRowHighlight() const
{
	return m_bRowHighlight;
}

void GrGridCore::SetRowHighlight(bool b)
{
	m_bRowHighlight = b;
}

GrRowHighlightType GrGridCore::GetRowHighlightType() const
{
	return m_rowHighlightType;
}

void GrGridCore::SetRowHighlightType(GrRowHighlightType type)
{
	m_rowHighlightType = type;
}

bool GrGridCore::Update()
{
	if(m_bUpdating == true)
		return 0;

	m_bUpdating = true;

	m_pTextUpdater->UpdateTextBound();

	m_pRootRow->Update(false);

	m_pTextUpdater->UpdateTextBound();
	m_pTextUpdater->UpdateTextAlign();

	m_bUpdating = false;
	return true;
}

const GrRect* GrGridCore::GetDisplayRect() const
{
	return &m_rtDisplay;
}

void GrGridCore::Clip(uint nStartCol, uint nStartRow)
{
	m_pRootRow->Clip(&m_rtDisplay, nStartCol, nStartRow);
}

void GrGridCore::SetDisplayRect(GrRect rtDisplay)
{
	if(m_rtDisplay == rtDisplay)
		return;
	m_rtDisplay = rtDisplay;
	Invalidate();
}

GrRect GrGridCore::GetDisplayableRect() const
{
	GrRect rtDisplayable = m_rtDisplay;
	if(GetMarginVisible() == false)
	{
		rtDisplayable.right = m_pColumnList->GetDisplayableRight();
	}

	rtDisplayable.bottom = m_pDataRowList->GetDisplayableBottom();
	return rtDisplayable;
}

void GrGridCore::Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const
{
	m_pRootRow->Render(pRenderer, pClipping);
	GetInvalidator()->Reset();
}

bool GrGridCore::CanBeGrouped() const
{
	return m_pGroupingList->CanBeGrouped();
}

void GrGridCore::EnableGrouping(bool b) const
{
	m_pGroupingList->EnableGrouping(b);
	m_pRootRow->SetVisibleChanged();
}

int GrGridCore::GetGroupingMargin() const
{
	return m_nGroupingMargin;
}

bool GrGridCore::IsGrouped() const
{
	return m_pGroupingList->GetGroupingCount() > 0 ? true : false;
}

void GrGridCore::Clear()
{
	Update();

	m_nGroupingMargin = 0;
	m_rtDisplay.DoEmpty();

	OnCleared(&GrEventArgs::Empty);

	//int n = GrCell::m_snCreated;
	//if(m_nInitialObjectCount != n)
	//	throw new exception("Clear시 해제 되지 않은 개체가 있습니다");
}

void GrGridCore::Invalidate()
{
	GetInvalidator()->Invalidate();
}
	
void GrGridCore::Invalidate(int x, int y, int width, int height)
{
	GetInvalidator()->Invalidate(x, y, width, height);
}

void GrGridCore::Invalidate(const GrRect& rect)
{
	GetInvalidator()->Invalidate(rect);	
}

void GrGridCore::LockInvalidate()
{
	GetInvalidator()->Lock();
}

void GrGridCore::UnlockInvalidate()
{
	GetInvalidator()->Unlock();
}

void GrGridCore::ResetInvalidate()
{
	GetInvalidator()->Reset();
}

bool GrGridCore::IsInvalidated() const
{
	return GetInvalidator()->IsInvalidated();
}

void GrGridCore::ShowClippedText(bool bShow)
{
	m_bShowClippedText = bShow;
}

void GrGridCore::columnList_ColumnInserted(GrObject* /*pSender*/, GrColumnEventArgs* /*e*/)
{
	m_pTextUpdater->AddTextBound(m_pCaption);
	m_pTextUpdater->AddTextBound(m_pGroupingList);
}

void GrGridCore::columnList_ColumnRemoved(GrObject* /*pSender*/, GrColumnEventArgs* /*e*/)
{
	m_pTextUpdater->AddTextBound(m_pCaption);
	m_pTextUpdater->AddTextBound(m_pGroupingList);
}

void GrGridCore::columnList_ColumnHorzAlignChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();
	m_pTextUpdater->AddTextAlign(pColumn);
}

void GrGridCore::columnList_ColumnVertAlignChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();
	m_pTextUpdater->AddTextAlign(pColumn);
}

void GrGridCore::columnList_ColumnWidthChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	if(m_bUpdating == true)
		return;
	GrColumn* pColumn = e->GetColumn();
	m_pTextUpdater->AddTextBound(pColumn);
}

void GrGridCore::columnList_ColumnWordwrapChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	if(m_bUpdating == true)
		return;
	GrColumn* pColumn = e->GetColumn();
	m_pTextUpdater->AddTextBound(pColumn);
}

void GrGridCore::columnList_ColumnPaddingChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
	GrColumn* pColumn = e->GetColumn();
	m_pTextUpdater->AddTextBound(pColumn);
}

void GrGridCore::SetFont(GrFont* pFont)
{
	if(m_pFont == pFont)
		return;
	m_pFont = pFont;
	OnFontChanged();
}

#ifdef _MANAGED
void GrGridCore::SetFont(System::Drawing::Font^ font)
{
	SetFont(GrFontManager::FromManagedFont(font));
}
#endif
GrFont* GrGridCore::GetFont() const
{
	if(m_pFont == NULL)
		return GetDefaultFont();
	return m_pFont;
}

GrFont* GrGridCore::GetFont(void* hFont) const
{
	return GrFontManager::GetFontDesc(hFont);
}

GrFont* GrGridCore::GetDefaultFont() const
{
	return GrFontManager::GetDefaultFont();
}

void GrGridCore::SetStyle(GrStyle* pStyle)
{
	m_pStyle = pStyle;
}

GrStyle* GrGridCore::GetStyle() const
{
	if(m_pStyle == NULL)
		return m_pDefaultStyle;
	return m_pStyle;
}

void GrGridCore::AttachObject(GrObject* pObject)
{
	if(pObject->m_pGridCore != NULL)
		return;
	pObject->m_pGridCore = this;
	pObject->OnGridCoreAttached();

	m_nAttachedCount++;
}

void GrGridCore::DetachObject(GrObject* pObject)
{
	if(pObject->m_pGridCore == NULL)
		return;
	pObject->OnGridCoreDetached();
	pObject->m_pGridCore = NULL;
	m_nAttachedCount--;
}

void GrGridCore::groupingList_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
	uint nGroupingCount = m_pGroupingList->GetGroupingCount();
	if(nGroupingCount == 0)
		m_nGroupingMargin = 0;
	else
		m_nGroupingMargin = (nGroupingCount + 1) * DEF_GROUP_WIDTH;
}

void GrGridCore::OnCreated(GrEventArgs* e)
{
	Created(this, e);
	Update();
}

void GrGridCore::OnCleared(GrEventArgs* e)
{
	Cleared(this, e);
}

void GrGridCore::OnCapacityChanged(GrEventArgs* e)
{
	CapacityChanged(this, e);
}

void GrGridCore::SetAutoFitColumn(bool b)
{
	m_bAutoFitColumn = b;
}

void GrGridCore::SetAutoFitRow(bool b)
{
	m_bAutoFitRow = b;
}

bool GrGridCore::GetAutoFitColumn() const
{
	return m_bAutoFitColumn;
}

bool GrGridCore::GetAutoFitRow() const
{
	return m_bAutoFitRow;
}

void GrGridCore::SetColumnSplitter(int value)
{
	m_nColumnSplitter = value;
}

void GrGridCore::SetRowSplitter(int value)
{
	m_nRowSplitter = value;
}

int GrGridCore::GetColumnSplitter() const
{
	return m_nColumnSplitter;
}

int GrGridCore::GetRowSplitter() const
{
	return m_nRowSplitter;
}

bool GrGridCore::GetFullRowSelect() const
{
	return m_bFullRowSelect;
}

void GrGridCore::SetFullRowSelect(bool b)
{
	m_bFullRowSelect = b;
}

bool GrGridCore::GetSelectionVisible() const
{
	return m_bSelectionVisible;
}

void GrGridCore::SetSelectionVisible(bool b)
{
	m_bSelectionVisible = b;
}