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


#include "GrGridCore.h"
#include "GrGridPainter.h"
#include "GrGridInternal.h"
#include <assert.h>

const GrClickEditing GrGridCore::DefaultClickEditing = GrClickEditing_FocusedClick;

class GrMouseOverer : public GrObject
{
public:
    GrMouseOverer()
    {
        m_pMouseOvered = NULL;
    }

    bool SetMouseOver(GrCell* pCell, const GrPoint& localLocation)
    {
        bool success = false;
        if(m_pMouseOvered != pCell)
            success = true;

        if(success == true)
        {
            if(m_pMouseOvered != NULL)
                m_pGridCore->Invalidate(m_pMouseOvered->GetRect());
            if(pCell != NULL)
                m_pGridCore->Invalidate(pCell->GetRect());
        }

        m_pMouseOvered = pCell;

        int state;
        if(m_pMouseOvered)
            state = m_pMouseOvered->HitMouseOverTest(localLocation);
        else
            state = 0;

        if(m_mouseOverState != state)
        {
            success = true;
            if(m_pMouseOvered != NULL)
                m_pGridCore->Invalidate(m_pMouseOvered->GetRect());
        }
        m_mouseOverState = state;
        return success;
    }

    GrCell* GetMouseOver() const
    {
        return m_pMouseOvered;
    }

    int GetMouseOverState() const
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
        m_pMouseOvered = NULL;
        m_mouseOverState = 0;
    }

private:
    GrCell* m_pMouseOvered;
    int m_mouseOverState;
};

class GrMousePresser : public GrObject
{
public:
    GrMousePresser()
    {
        m_pMousePressed = NULL;
    }

    void SetMousePress(GrCell* pCell)
    {
        if(m_pMousePressed != NULL)
            m_pGridCore->Invalidate(m_pMousePressed->GetRect());
        if(pCell != NULL)
            m_pGridCore->Invalidate(pCell->GetRect());

        m_pMousePressed = pCell;
    }

    void SetMouseUnpress()
    {
        if(m_pMousePressed != NULL)
            m_pGridCore->Invalidate(m_pMousePressed->GetRect());
        m_pMousePressed = NULL;
    }

    GrCell* GetMousePress() const
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
        m_pMousePressed = NULL;
    }

private:
    GrCell* m_pMousePressed;
};

void SignalFunc(int /*sigNum*/)
{
    throw _Exception("");
}

GrGridCore::GrGridCore(GrGridWindow* pGridWindow) : m_pGridWindow(pGridWindow)
{
    m_createdCell = 0;
    m_attachedCount = 0;

    m_groupMargin = 0;
    m_updating = false;

    m_autoFitColumn = false;
    m_autoFitColumnType = GrAutoFitColumnType_ColumnIncluded;
    m_autoFitRow = false;
    m_columnSplitterWidth= 10;
    m_rowSplitterHeight = 3;

    m_reservedColumn = 0;
    m_reservedRow = 0;

    m_fullRowSelect = false;
    m_selectionVisible = true;
    m_rowHighlight = false;
    m_rowHighlightType = GrRowHighlightType_Fill;
    m_columnMovable = true;
    m_columnSortable = true;
    m_columnResizable= true;
    m_columnFreezable = true;
    m_readOnly = false;
    m_rowResizable = true;
    m_hideSelection = false;
    m_multiSelect = true;
    m_rowVisible = true;
    m_clickEditing = DefaultClickEditing;


    m_pFont = NULL;
    m_pDefaultStyle = new GrStyle();
    m_pStyle = NULL;

    m_pItemSelector = new GrItemSelectorInternal();
    m_pFocuser = new GrFocuserInternal();
    m_pTextUpdater = new GrTextUpdater();
    m_pMouseOverer = new GrMouseOverer();
    m_pMousePresser = new GrMousePresser();
    m_pRootRow = new GrRootRow();
    m_pCaption = new GrCaption();
    m_pGroupPanel = new GrGroupPanel();
    m_pColumnList = new GrColumnList();
    m_pDataRowList = new GrDataRowList();
    m_pSplitterRow = new GrRowSplitter();
    m_pFocusMover = new GrFocusMover();
    m_pStateManager = new GrStateManager();

    m_pInvalidator = m_pGridWindow->GetInvalidator();

    AttachObject(m_pItemSelector);
    AttachObject(m_pFocuser);
    AttachObject(m_pTextUpdater);
    AttachObject(m_pMouseOverer);
    AttachObject(m_pMousePresser);
    AttachObject(m_pRootRow);
    AttachObject(m_pCaption);
    AttachObject(m_pGroupPanel);
    AttachObject(m_pColumnList);
    AttachObject(m_pDataRowList);
    AttachObject(m_pSplitterRow);
    AttachObject(m_pStateManager);
    AttachObject(m_pGridWindow);
    AttachObject(m_pFocusMover);

    m_pRootRow->AddChild(m_pCaption);
    m_pRootRow->AddChild(m_pGroupPanel);
    m_pRootRow->AddChild(m_pColumnList);
    m_pRootRow->AddChild(m_pDataRowList->GetInsertionRow());
    m_pRootRow->AddChild(m_pSplitterRow);
    m_pRootRow->AddChild(m_pDataRowList);

    m_pGroupPanel->Changed.Add(this, &GrGridCore::groupPanel_Changed);
    m_pColumnList->ColumnInserted.Add(this, &GrGridCore::columnList_ColumnInserted);
    m_pColumnList->ColumnRemoved.Add(this, &GrGridCore::columnList_ColumnRemoved);
    m_pColumnList->ColumnWidthChanged.Add(this, &GrGridCore::columnList_ColumnWidthChanged);
    m_pColumnList->ColumnWordwrapChanged.Add(this, &GrGridCore::columnList_ColumnWordwrapChanged);
    m_pColumnList->ColumnHorzAlignChanged.Add(this, &GrGridCore::columnList_ColumnHorzAlignChanged);
    m_pColumnList->ColumnVertAlignChanged.Add(this, &GrGridCore::columnList_ColumnVertAlignChanged);
    m_pColumnList->ColumnPaddingChanged.Add(this, &GrGridCore::columnList_ColumnPaddingChanged);

    m_pFocuser->FocusChanging.Add(this, &GrGridCore::focuser_FocusChanging);

    this->Created(this, &GrEventArgs::Empty);
}

GrGridCore::~GrGridCore()
{
    delete m_pStateManager;
    delete m_pFocusMover;
    delete m_pSplitterRow;
    delete m_pDataRowList;
    delete m_pColumnList;
    delete m_pGroupPanel;
    delete m_pCaption;
    delete m_pRootRow;
    delete m_pMousePresser;
    delete m_pMouseOverer;
    delete m_pTextUpdater;
    delete m_pFocuser;
    delete m_pItemSelector;

    delete m_pDefaultStyle;
    delete m_pStyle;

    if(m_createdCell != 0)
        throw _Exception("Some obejcts are not deleted");
}

void GrGridCore::Reserve(uint columnCount, uint rowCount)
{
    m_reservedColumn = columnCount + 2;
    m_reservedRow = rowCount + 2;

    m_pDataRowList->Reserve(m_reservedRow);
    m_pColumnList->Reserve(m_reservedColumn);

    OnCapacityChanged(&GrEventArgs::Empty);
}

uint GrGridCore::GetReservedColumn() const
{
    return m_reservedColumn;
}

uint GrGridCore::GetReservedRow() const
{
    return m_reservedRow;
}

bool GrGridCore::SetMouseOver(GrCell* pCell, const GrPoint& localLocation)
{
    return m_pMouseOverer->SetMouseOver(pCell, localLocation);
}

GrCell* GrGridCore::GetMouseOver() const
{
    return m_pMouseOverer->GetMouseOver();
}

int GrGridCore::GetMouseOverState() const
{
    return m_pMouseOverer->GetMouseOverState();
}

void GrGridCore::SetMousePress(GrCell* pCell)
{
    m_pMousePresser->SetMousePress(pCell); 
}

void GrGridCore::SetMouseUnpress()
{
    m_pMousePresser->SetMouseUnpress(); 
}

GrCell* GrGridCore::GetMousePress() const
{
    return m_pMousePresser->GetMousePress();
}

bool GrGridCore::GetInsertionRowVisible() const
{
    return GetInsertionRow()->GetVisible();
}

void GrGridCore::SetInsertionRowVisible(bool b)
{
    GetInsertionRow()->SetVisible(b);
}

bool GrGridCore::GetRowHighlight() const
{
    return m_rowHighlight;
}

void GrGridCore::SetRowHighlight(bool b)
{
    if(m_rowHighlight == b)
        return;
    m_rowHighlight = b;
    Invalidate();
}

GrRowHighlightType GrGridCore::GetRowHighlightType() const
{
    return m_rowHighlightType;
}

void GrGridCore::SetRowHighlightType(GrRowHighlightType type)
{
    if(m_rowHighlightType == type)
        return;
    m_rowHighlightType = type;
    Invalidate();
}

bool GrGridCore::Update(bool force)
{
    if(m_updating == true)
        return 0;

    m_updating = true;

    m_pTextUpdater->UpdateTextBounds();
    m_pRootRow->Update(force);
    m_pTextUpdater->UpdateTextBounds();
    m_pTextUpdater->UpdateTextAlign();

    m_updating = false;
    return true;
}

const GrRect& GrGridCore::GetDisplayRect() const
{
    return m_displayRect;
}

void GrGridCore::SetDisplayRect(const GrRect& displayRect)
{
    if(m_displayRect == displayRect)
        return;
    m_displayRect = displayRect;
    m_pRootRow->SetHeightChanged();
    m_pColumnList->SetWidthChanged();
    Invalidate();
}

GrRect GrGridCore::GetBounds() const
{
    return m_pRootRow->GetBounds();
}

GrRect GrGridCore::GetDataRect() const
{
    GrRect dataRect;
    dataRect.left = m_pColumnList->GetUnfrozenX();
    dataRect.top = m_pDataRowList->GetY();
    dataRect.right = std::min(GetBounds().right, m_displayRect.right);
    dataRect.bottom = std::min(GetBounds().bottom, m_displayRect.bottom);
    return dataRect;
}

void GrGridCore::PrePaint(GrGridPainter* /*pPainter*/, const GrRect& /*clipRect*/) const
{

}

void GrGridCore::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    m_pRootRow->Paint(pPainter, clipRect);
}

void GrGridCore::PostPaint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
{
    m_pStateManager->OnPaint(pPainter); 
}

void GrGridCore::BeginPaint()
{
    uint horz = GetHorzScroll()->GetVisible() == true ? GetHorzScroll()->GetValue() : GetHorzScroll()->GetMinimum();
    uint vert = GetVertScroll()->GetVisible() == true ? GetVertScroll()->GetValue() : GetVertScroll()->GetMinimum();
    m_pRootRow->Clip(m_displayRect, horz, vert);
}

void GrGridCore::EndPaint()
{
    m_pInvalidator->Reset();
}

bool GrGridCore::GetGroupable() const
{
    return m_pGroupPanel->GetGroupable();
}

void GrGridCore::SetGroupable(bool b) const
{
    m_pGroupPanel->SetGroupable(b);
    m_pRootRow->SetVisibleChanged();
}

bool GrGridCore::GetColumnMovable() const
{
    return m_columnMovable;
}

void GrGridCore::SetColumnMovable(bool b)
{
    m_columnMovable = b;
}

bool GrGridCore::GetColumnSortable() const
{
    return m_columnSortable;
}

void GrGridCore::SetColumnSortable(bool b)
{
    m_columnSortable = b;
}

bool GrGridCore::GetColumnResizable() const
{
    return m_columnResizable;
}

void GrGridCore::SetColumnResizable(bool b)
{
    m_columnResizable = b;
}

bool GrGridCore::GetColumnFreezable() const
{
    return m_columnFreezable;
}

void GrGridCore::SetColumnFreezable(bool b)
{
    m_columnFreezable = b;
}

bool GrGridCore::GetReadOnly() const
{
    return m_readOnly;
}

void GrGridCore::SetReadOnly(bool value)
{
    if(m_readOnly == value)
        return;
    m_readOnly = value;
    Invalidate();
}

bool GrGridCore::GetRowResizable() const
{
    return m_rowResizable;
}

void GrGridCore::SetRowResizable(bool b)
{
    m_rowResizable = b;
}

bool GrGridCore::GetHideSelection() const
{
    return m_hideSelection;
}

void GrGridCore::SetHideSelection(bool b)
{
    m_hideSelection = b;
}

bool GrGridCore::GetMultiSelect() const
{
    return m_multiSelect;
}

void GrGridCore::SetMultiSelect(bool b)
{
    m_multiSelect = b;
}

GrClickEditing GrGridCore::GetClickEditing() const
{
    return m_clickEditing;
}

void GrGridCore::SetClickEditing(GrClickEditing clickEditing)
{
    if(clickEditing == GrClickEditing_Default)
        clickEditing = DefaultClickEditing;
    m_clickEditing = clickEditing;
}

int GrGridCore::GetGroupMargin() const
{
    return m_groupMargin;
}

bool GrGridCore::IsGrouped() const
{
    return m_pGroupPanel->GetGroupCount() > 0 ? true : false;
}

void GrGridCore::Clear()
{
    Update(true);
    m_groupMargin = 0;
    OnCleared(&GrEventArgs::Empty);
    Update(true);
    //int n = GrCell::m_snCreated;
    //if(m_nInitialObjectCount != n)
    // throw new exception("Clear시 해제 되지 않은 개체가 있습니다");
}

void GrGridCore::Invalidate()
{
    m_pInvalidator->Invalidate();
}

void GrGridCore::Invalidate(int x, int y, int width, int height)
{
    m_pInvalidator->Invalidate(x, y, width, height);
}

void GrGridCore::Invalidate(const GrRect& rect)
{
    m_pInvalidator->Invalidate(rect); 
}

void GrGridCore::LockInvalidate()
{
    m_pInvalidator->Lock();
}

void GrGridCore::UnlockInvalidate()
{
    m_pInvalidator->Unlock();
}

void GrGridCore::ResetInvalidate()
{
    m_pInvalidator->Reset();
}

bool GrGridCore::IsInvalidated() const
{
    return m_pInvalidator->IsInvalidated();
}

void GrGridCore::focuser_FocusChanging(GrObject* /*pSender*/, GrFocusChangeArgs* /*e*/)
{
    if(m_pStateManager->GetGridState() == GrGridState_ItemEditing)
        m_pStateManager->ChangeDefaultState();
}

void GrGridCore::columnList_ColumnInserted(GrObject* /*pSender*/, GrColumnEventArgs* /*e*/)
{
    m_pTextUpdater->AddTextBounds(m_pCaption);
    m_pTextUpdater->AddTextBounds(m_pGroupPanel);
}

void GrGridCore::columnList_ColumnRemoved(GrObject* /*pSender*/, GrColumnEventArgs* /*e*/)
{
    m_pTextUpdater->AddTextBounds(m_pCaption);
    m_pTextUpdater->AddTextBounds(m_pGroupPanel);
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
    if(m_updating == true)
        return;
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetItemWordWrap() == true)
        m_pTextUpdater->AddTextBounds(pColumn);
}

void GrGridCore::columnList_ColumnWordwrapChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    if(m_updating == true)
        return;
    GrColumn* pColumn = e->GetColumn();
    m_pTextUpdater->AddTextBounds(pColumn);
}

void GrGridCore::columnList_ColumnPaddingChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetItemWordWrap() == true)
        m_pTextUpdater->AddTextBounds(pColumn);
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

    m_attachedCount++;
}

void GrGridCore::DetachObject(GrObject* pObject)
{
    if(pObject->m_pGridCore == NULL)
        return;
    pObject->OnGridCoreDetached();
    pObject->m_pGridCore = NULL;
    m_attachedCount--;
}

void GrGridCore::EditItem(GrItem* pItem, GrEditingReason reason)
{
    static GrEditingReason temp;
    temp = reason;

    if(pItem->GetDisplayable() == false)
    {
        pItem->BringIntoView();
    }
    m_pStateManager->ChangeState(GrGridState_ItemEditing, pItem, &temp);
}

void GrGridCore::BringIntoView(GrItem* pItem)
{
    if(pItem == NULL)
        return;

    Update();
    m_pColumnList->BringIntoView(pItem->GetColumn());
    m_pDataRowList->BringIntoView(pItem->GetDataRow());
}

void GrGridCore::Invoke(std::wstring eventName, GrEventArgs* e)
{
    if(eventName.compare(L"EditValue") == 0)
    {
        OnEditValue((GrEditEventArgs*)e);
    }
    else if(eventName.compare(L"ItemMouseEnter") == 0)
    {
        OnItemMouseEnter((GrItemMouseEventArgs*)e);
    }
    else if(eventName.compare(L"ItemMouseMove") == 0)
    {
        OnItemMouseMove((GrItemMouseEventArgs*)e);
    }
    else if(eventName.compare(L"ItemMouseLeave") == 0)
    {
        OnItemMouseLeave((GrItemMouseEventArgs*)e);
    }
    else if(eventName.compare(L"ItemMouseClick") == 0)
    {
        OnItemMouseClick((GrItemMouseEventArgs*)e);
    }
    else if(eventName.compare(L"ItemMouseDoubleClick") == 0)
    {
        OnItemMouseDoubleClick((GrItemMouseEventArgs*)e);
    }
    else if(eventName.compare(L"RowMouseEnter") == 0)
    {
        OnRowMouseEnter((GrRowMouseEventArgs*)e);
    }
    else if(eventName.compare(L"RowMouseMove") == 0)
    {
        OnRowMouseMove((GrRowMouseEventArgs*)e);
    }
    else if(eventName.compare(L"RowMouseLeave") == 0)
    {
        OnRowMouseLeave((GrRowMouseEventArgs*)e);
    }
    else if(eventName.compare(L"close") == 0)
    {
        m_pStateManager->ChangeDefaultState();
    }
    else if(eventName.compare(L"FontChanged") == 0)
    {
        OnFontChanged(e);
    }
    else
    {
        m_pColumnList->Invoke(eventName, e);
    }
}

void GrGridCore::OnEditValue(GrEditEventArgs* e)
{
    m_pGridWindow->OnEditValue(e);
    m_pInvalidator->Invalidate();
}

void GrGridCore::OnItemMouseEnter(GrItemMouseEventArgs* e)
{
    ItemMouseEnter(this, e);
}

void GrGridCore::OnItemMouseMove(GrItemMouseEventArgs* e)
{
    ItemMouseMove(this, e);
}

void GrGridCore::OnItemMouseLeave(GrItemMouseEventArgs* e)
{
    ItemMouseLeave(this, e);
}

void GrGridCore::OnItemMouseClick(GrItemMouseEventArgs* e)
{
    ItemMouseClick(this, e);
}

void GrGridCore::OnItemMouseDoubleClick(GrItemMouseEventArgs* e)
{
    ItemMouseDoubleClick(this, e);
}

void GrGridCore::OnRowMouseEnter(GrRowMouseEventArgs* e)
{
    RowMouseEnter(this, e);
}

void GrGridCore::OnRowMouseMove(GrRowMouseEventArgs* e)
{
    RowMouseMove(this, e);
}

void GrGridCore::OnRowMouseLeave(GrRowMouseEventArgs* e)
{
    RowMouseLeave(this, e);
}

void GrGridCore::OnFontChanged(GrEventArgs* e)
{
    FontChanged(this, e);
}

void GrGridCore::groupPanel_Changed(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    uint groupCount = m_pGroupPanel->GetGroupCount();
    if(groupCount == 0)
        m_groupMargin = 0;
    else
        m_groupMargin = (groupCount + 1) * DEF_GROUP_WIDTH;
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
    m_autoFitColumn = b;
}

void GrGridCore::SetAutoFitRow(bool b)
{
    m_autoFitRow = b;
}

bool GrGridCore::GetAutoFitColumn() const
{
    return m_autoFitColumn;
}

bool GrGridCore::GetAutoFitRow() const
{
    return m_autoFitRow;
}

GrAutoFitColumnType GrGridCore::GetAutoFitColumnType() const
{
    return m_autoFitColumnType;
}

void GrGridCore::GetAutoFitColumnType(GrAutoFitColumnType value)
{
    if(m_autoFitColumnType == value)
        return;
    m_autoFitColumnType = value;

    if(m_autoFitColumn == true)
    {
        m_pColumnList->SetWidthChanged();
    }
}

void GrGridCore::SetColumnSplitter(int value)
{
    m_columnSplitterWidth = value;
}

void GrGridCore::SetRowSplitter(int value)
{
    m_rowSplitterHeight = value;
}

int GrGridCore::GetColumnSplitter() const
{
    return m_columnSplitterWidth;
}

int GrGridCore::GetRowSplitter() const
{
    return m_rowSplitterHeight;
}

bool GrGridCore::GetFullRowSelect() const
{
    return m_fullRowSelect;
}

void GrGridCore::SetFullRowSelect(bool b)
{
    m_fullRowSelect = b;
}

bool GrGridCore::GetSelectionVisible() const
{
    return m_selectionVisible;
}

void GrGridCore::SetSelectionVisible(bool b)
{
    m_selectionVisible = b;
}

bool GrGridCore::GetRowVisible() const
{
    return m_rowVisible;
}

void GrGridCore::SetRowVisible(bool b)
{
    if(m_rowVisible == b)
        return;
    m_rowVisible = b;
    m_pColumnList->SetVisibleChanged();
    Invalidate();
}

bool GrGridCore::HitTest(const GrPoint& location, GrHitTest* pHitTest) const
{
    GrCell* pCell = m_pRootRow->HitTest(location);

    if(pCell != NULL)
    {
        pHitTest->pHitted = pCell;
        pHitTest->localHit = location - pCell->GetLocation();
        return true;
    } 
    return false;
}