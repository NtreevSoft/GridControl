#include "GrGridStateManager.h"
#include "GrGridCore.h"
#include <limits.h>

#define STRING2(x) #x
#define STRING(x) STRING2(x)

GrStateManager::GrStateManager()
{
    using namespace GridStateClass;

    m_states.push_back(new Normal());
    m_states.push_back(new RootPressing());
    m_states.push_back(new ColumnPressing());
    m_states.push_back(new ColumnResizing());
    m_states.push_back(new ColumnSplitterMoving());
    m_states.push_back(new RowPressing());
    m_states.push_back(new RowResizing());
    m_states.push_back(new GroupingInfoPressing());
    m_states.push_back(new GroupingCellPressing());
    m_states.push_back(new GroupingExpandPressing()); 
    m_states.push_back(new ItemPressing());
    m_states.push_back(new ItemButtonPressing());
    m_states.push_back(new ItemEditing());

    m_state         = NULL;
    m_defaultState  = m_states[0];
    m_dragging      = false;
}

GrStateManager::Invalidator::Invalidator(GrGridCore* pGridCore)
: m_pGridCore(pGridCore)
{
    m_pGridCore->LockInvalidate();
}

GrStateManager::Invalidator::~Invalidator()
{
    m_pGridCore->UnlockInvalidate();
}

GrStateManager::~GrStateManager()
{
    for_each(_States, m_states, value)
    {
        delete value;
    }
}

void GrStateManager::OnGridCoreAttached()
{
    GrObject::OnGridCoreAttached();

    m_pGridWindow = m_pGridCore->GetGridWindow();

    for_each(_States, m_states, value)
    {
        m_pGridCore->AttachObject(value);
    }

    ChangeDefaultState();
}

void GrStateManager::ChangeDefaultState()
{
    if(m_state != m_defaultState)
        ChangeState(m_defaultState, NULL, NULL);
}

void GrStateManager::OnMouseDown(const GrPoint& location, GrKeys modifierKeys)
{
    Invalidator invalidator(m_pGridCore);

    m_downState = NULL;
    GrHitTest hitTest;

    if(m_pGridCore->HitTest(location, &hitTest) == true)
    {
        GrStateBase* hittedState = GetHitTest(hitTest.pHitted, hitTest.localHit);
        if(hittedState == NULL)
        {
            ChangeDefaultState();
            return;
        }
        ChangeState(hittedState, hitTest.pHitted, NULL);
        m_downState = m_state;

        if(m_state == hittedState)
        {
            GrStateMouseEventArgs e(location, modifierKeys, hitTest.pHitted, hitTest.localHit, hittedState->GetState(), NULL);
            hittedState->OnMouseDown(&e);

            if(e.GetNextState() != hittedState->GetState())
            {
                ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
            }

            m_downLocation = location;
            m_dragRectangle = ComputeDragRectangle(location);
        }
    }
}

GrRect GrStateManager::ComputeDragRectangle(const GrPoint& location)
{
    GrSize dragSize = m_pGridWindow->GetDragSize();
    return GrRect(GrPoint(location.x - (dragSize.width / 2),location.y - (dragSize.height / 2)),dragSize);
}

void GrStateManager::OnMouseMove(const GrPoint& location, GrKeys modifierKeys, bool pressed)
{
    Invalidator invalidator(m_pGridCore);

    GrHitTest hitTest;
    m_pGridCore->HitTest(location, &hitTest);

    if(pressed == true)
    {
        if(m_state == NULL || m_state->GetDragable() == false)
            return;

        if(m_dragging == false && m_dragRectangle.Contains(location) == false)
        {
            m_dragging = true;
            m_state->OnMouseDragBegin(location);
        }

        if(m_dragging == true)
        {
            m_state->OnMouseDragMove(location, hitTest);
            m_pGridWindow->SetCursor(m_state->GetCursor());
        }
    }
    else
    {
        GrCursor cursor = m_defaultState->GetCursor();

        if(hitTest.pHitted != NULL)
        {
            GrStateBase* hittedState = GetHitTest(hitTest.pHitted, hitTest.localHit);
            if(hittedState != NULL)
                cursor = hittedState->GetCursor();
        }

        GrStateMouseEventArgs e(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_state->GetState());
        m_state->OnMouseMove(&e);

        if(e.GetHandled() == false)
            m_pGridWindow->SetCursor(cursor);
    }
}

void GrStateManager::OnMouseUp(const GrPoint& location, GrKeys modifierKeys)
{
    Invalidator invalidator(m_pGridCore);

    if(m_state != NULL)
    {
        GrHitTest hitTest;
        m_pGridCore->HitTest(location, &hitTest);

        if(m_dragging == true)
        {
            m_state->OnMouseDragEnd(false, hitTest);
            m_dragging = false;
        }

        GrStateMouseEventArgs e(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_defaultState->GetState());
        m_state->OnMouseUp(&e);

        if(e.GetNextState() != m_state->GetState())
        {
            ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
        }
    }
}

void GrStateManager::OnMouseClick(const GrPoint& location, GrKeys modifierKeys)
{
    Invalidator invalidator(m_pGridCore);

    if(m_downState != NULL)
    {
        GrHitTest hitTest;
        m_pGridCore->HitTest(location, &hitTest);

        GrStateMouseEventArgs e(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_downState->GetState());

        m_downState->OnMouseClick(&e);

        if(e.GetNextState() != m_downState->GetState())
        {
            ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
        }
    }
}

void GrStateManager::OnMouseDoubleClick(const GrPoint& location, GrKeys modifierKeys)
{
    Invalidator invalidator(m_pGridCore);

    if(m_downState != NULL)
    {
        GrHitTest hitTest;
        m_pGridCore->HitTest(location, &hitTest);

        GrStateMouseEventArgs e(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_downState->GetState());
        m_downState->OnMouseDoubleClick(&e);

        if(e.GetNextState() != m_downState->GetState())
        {
            ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
        }
    }
}

void GrStateManager::OnMouseWheel(const GrPoint& location, GrKeys modifierKeys, int delta)
{
    Invalidator invalidator(m_pGridCore);

    if(m_state != NULL)
    {
        GrMouseEventArgs e(location, modifierKeys, delta);
        m_state->OnMouseWheel(&e);
    }
}

void GrStateManager::OnPaint(GrGridPainter* pGridPainter)
{
    if(m_state != NULL)
        m_state->OnPaintAdornments(pGridPainter, m_pGridCore->GetDisplayRect());
}

void GrStateManager::OnKeyDown(GrKeys key, GrKeys modifierKeys)
{
    Invalidator invalidator(m_pGridCore);

    if(m_dragging == true)
    {
        if(key == GrKeys_Escape)
        {
            m_state->OnMouseDragEnd(true, GrHitTest::Empty);
            m_dragging = false;

            GrStateMouseEventArgs e(GrPoint::Empty, modifierKeys, NULL, GrPoint::Empty, m_defaultState->GetState());
            m_state->OnMouseUp(&e);

            if(e.GetNextState() == m_defaultState->GetState())
                ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
        }
    }
    else if(m_state != NULL)
    {
        m_state->OnKeyDown(key, modifierKeys);
    }
}

void GrStateManager::OnKeyUp(GrKeys key, GrKeys modifierKeys)
{
    if(m_state != NULL)
    {
        m_state->OnKeyUp(key, modifierKeys);
    }
}

void GrStateManager::OnClearing()
{
    ChangeDefaultState();
}

void GrStateManager::ChangeState(GrGridState state, GrCell* pCell, void* userData)
{
    ChangeState(m_states[(int)state], pCell, userData);
}

void GrStateManager::ChangeState(GrStateBase* state, GrCell* pHitted, void* userData)
{
    GrStateBase* oldState = m_state;
    if(oldState != NULL)
    {
        GrStateEventArgs e(NULL, oldState->GetState(), NULL);
        oldState->OnEnd(&e);
        if(e.GetNextState() != oldState->GetState())
        {
            ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
            return;
        }
    }

    GrStateEventArgs e(pHitted, state->GetState(), userData);
    state->OnBegin(&e);

    m_state = state;
    //m_pGridWindow->SetCursor(m_state->GetCursor());

    if(e.GetNextState() != m_state->GetState())
        ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
}

GrGridState GrStateManager::GetHitTest(const GrPoint& location)
{
    GrHitTest hitTest;
    if(this->m_pGridCore->HitTest(location, &hitTest) == false)
        return GrGridState_Unknown;
    GrStateBase* base = GetHitTest(hitTest.pHitted, hitTest.localHit);
    return base != NULL ? base->GetState() : GrGridState_Unknown;
}

GrStateBase* GrStateManager::GetHitTest(GrCell* pHitted, const GrPoint& localHitted)
{
    for_each(_States, m_states, value)
    {
        if(value->GetHitTest(pHitted, localHitted) == true)
        {
            return value;
        }
    }
    return NULL;
}

GrStateBase::GrStateBase()
{

}

GrStateBase::~GrStateBase()
{

}

bool GrStateBase::GetHitTest(GrCell* /*pHitted*/, const GrPoint& /*localLocation*/)
{
    return false;
}


void GrStateBase::OnBegin(GrStateEventArgs* /*e*/)
{

}

void GrStateBase::OnPaintAdornments(GrGridPainter* /*g*/, const GrRect& /*displayRect*/)
{

}

void GrStateBase::OnEnd(GrStateEventArgs* /*e*/)
{

}


void GrStateBase::OnMouseDown(GrStateMouseEventArgs* /*e*/)
{

}

void GrStateBase::OnMouseMove(GrStateMouseEventArgs* /*e*/)
{

}

void GrStateBase::OnMouseUp(GrStateMouseEventArgs* /*e*/)
{

}

void GrStateBase::OnMouseClick(GrStateMouseEventArgs* /*e*/)
{

}

void GrStateBase::OnMouseDoubleClick(GrStateMouseEventArgs* /*e*/)
{

}

void GrStateBase::OnMouseWheel(GrMouseEventArgs* /*e*/)
{

}

void GrStateBase::OnKeyDown(GrKeys /*key*/, GrKeys /*modifierKeys*/)
{

}

void GrStateBase::OnKeyUp(GrKeys /*key*/, GrKeys /*modifierKeys*/)
{

}

void GrStateBase::OnMouseDragBegin(const GrPoint& /*location*/)
{

}

void GrStateBase::OnMouseDragEnd(bool /*cancel*/, const GrHitTest& /*hitTest*/)
{

}

void GrStateBase::OnMouseDragMove(const GrPoint& /*location*/, const GrHitTest& /*hitTest*/)
{

}

void GrStateBase::OnGridCoreAttached()
{
    GrObject::OnGridCoreAttached();
    m_pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    m_pFocuser      = dynamic_cast<GrFocuserInternal*>(m_pGridCore->GetFocuser());
    m_pGridWindow   = m_pGridCore->GetGridWindow();
}


namespace GridStateClass
{
    Normal::Normal()
    {
        m_pCell = NULL;
    }

    void Normal::OnGridCoreAttached()
    {
        GrStateBase::OnGridCoreAttached();
        m_pGridCore->Cleared.Add(this, &Normal::gridCore_Cleared);
    }

    void Normal::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
    {
        m_pCell = NULL;
    }

    void Normal::OnBegin(GrStateEventArgs* /*e*/)
    {
        m_pGridCore->SetMouseUnpress();
    }

    void Normal::OnMouseMove(GrStateMouseEventArgs* e)
    {
        GrColumnList* pColumnList = m_pGridCore->GetColumnList();
        GrCell* pCell = e->GetCell();

        m_pGridCore->SetMouseOver(pCell, e->GetLocalHit());

        if(pCell != m_pCell)
        {
            if(m_pCell != NULL)
            {
                switch(m_pCell->GetCellType())
                {
                case GrCellType_Item:
                    {
                        GrItemMouseEventArgs e1((GrItem*)m_pCell, e->GetLocalHit(), e->GetModifierKeys());
                        m_pGridCore->Invoke(L"ItemMouseLeave", &e1);
                    }
                    break;
                case GrCellType_Column:
                    {
                        GrColumnMouseEventArgs e2((GrColumn*)m_pCell, e->GetLocalHit(), e->GetModifierKeys());
                        pColumnList->Invoke(L"ColumnMouseLeave", &e2);
                    }
                    break;
                default:
                    break;
                }
            }

            if(pCell != NULL)
            {
                switch(pCell->GetCellType())
                {
                case GrCellType_Item:
                    {
                        GrItemMouseEventArgs e1((GrItem*)pCell, e->GetLocalHit(), e->GetModifierKeys());
                        m_pGridCore->Invoke(L"ItemMouseEnter", &e1);
                        m_pGridCore->Invoke(L"ItemMouseMove", &e1);
                        if(e1.GetHandled() == true)
                        {
                            e->SetHandled(true);
                        }
                    }
                    break;
                case GrCellType_Column:
                    {
                        GrColumnMouseEventArgs e2((GrColumn*)pCell, e->GetLocalHit(), e->GetModifierKeys());
                        pColumnList->Invoke(L"ColumnMouseEnter", &e2);
                        pColumnList->Invoke(L"ColumnMouseMove", &e2);
                        if(e2.GetHandled() == true)
                        {
                            e->SetHandled(true);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            m_pCell = pCell;
        }
        else if(m_pCell != NULL)
        {
            switch(m_pCell->GetCellType())
            {
            case GrCellType_Item:
                {
                    GrItemMouseEventArgs e1((GrItem*)m_pCell, e->GetLocalHit(), e->GetModifierKeys());
                    m_pGridCore->Invoke(L"ItemMouseMove", &e1);
                    if(e1.GetHandled() == true)
                    {
                        e->SetHandled(true);
                    }
                }
                break;
            case GrCellType_Column:
                {
                    GrColumnMouseEventArgs e2((GrColumn*)m_pCell, e->GetLocalHit(), e->GetModifierKeys());
                    pColumnList->Invoke(L"ColumnMouseMove", &e2);
                    if(e2.GetHandled() == true)
                    {
                        e->SetHandled(true);
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    void Normal::OnMouseWheel(GrMouseEventArgs* e)
    {
        GrScroll* pVertScroll = m_pGridCore->GetVertScroll();
        GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

        if(pVertScroll->GetVisible() == false)
            return;

        int scrollLine = m_pGridWindow->GetMouseWheelScrollLines();

        int value = e->GetDelta() / m_pGridWindow->GetMouseWheelScrollDelta() * scrollLine;
        int rowCount = (int)pDataRowList->GetDisplayableRowCount();

        if(rowCount < abs(value))
        {
            if(value < 0)
                value = -rowCount;
            else
                value = rowCount;
        }

        value = pVertScroll->GetValue() - value;
        value = pVertScroll->ValidateValue(value);
        pVertScroll->SetValue(value);
        m_pGridCore->Invalidate();
    }

    void Normal::OnKeyDown(GrKeys key, GrKeys modifierKeys)
    {
        GrFocusMover* pMover = m_pGridCore->GetFocusMover();

        GrSelectionRange selectionRange = m_pGridWindow->GetSelectionRange();

        switch(key)
        {
        case GrKeys_Tab:
            {
                if(modifierKeys & GrKeys_Shift)
                    pMover->MoveLeft(GrSelectionRange_One);
                else
                    pMover->MoveRight(GrSelectionRange_One);
            }
            break;
        case GrKeys_Left:
            {
                IFocusable* pFocusable = m_pFocuser->Get();
                if(pFocusable != NULL)
                {
                    if(pFocusable->GetCellType() == GrCellType_Grouping)
                    {
                        GrGroupingRow* pGrGroupingRow = (GrGroupingRow*)pFocusable->GetDataRow();
                        pGrGroupingRow->Expand(false);
                    }
                    else
                    {
                        pMover->MoveLeft(GrSelectionRange_One);
                    }
                }
            }
            break;
        case GrKeys_Up:
            {
                pMover->MoveUp(GrSelectionRange_One);
            }
            break;
        case GrKeys_Right:
            {
                IFocusable* pFocusable = m_pFocuser->Get();
                if(pFocusable != NULL)
                {
                    if(pFocusable->GetCellType() == GrCellType_Grouping)
                    {
                        GrGroupingRow* pGrGroupingRow = (GrGroupingRow*)pFocusable->GetDataRow();
                        pGrGroupingRow->Expand(true);
                    }
                    else
                    {
                        pMover->MoveRight(GrSelectionRange_One);
                    }
                }
            }
            break;
        case GrKeys_Down:
            {
                pMover->MoveDown(GrSelectionRange_One);
            }
            break;

        case GrKeys_End:
            {
                if(modifierKeys & GrKeys_Control)
                    pMover->LastRow(selectionRange);
                else
                    pMover->LastCell(selectionRange);
            }
            break;
        case GrKeys_Home:
            {
                if(modifierKeys & GrKeys_Control)
                    pMover->FirstRow(selectionRange);
                else
                    pMover->FirstCell(selectionRange);
            }
            break;
        case GrKeys_PageUp:
            {
                pMover->PageUp(selectionRange);
            }
            break;
        case GrKeys_PageDown:
            {
                pMover->PageDown(selectionRange);
            }
            break;

            //case GrKeys_Enter:
            //    {
            //        if(m_focusedCell != nullptr)
            //        {
            //            if(m_focusedCell->Row == m_insertionRow)
            //            {
            //                Row^ row = AddNewRowFromInsertion();
            //                if(row == nullptr)
            //                    break;

            //                Ntreev::Windows::Forms::Grid::Cell^ cell = row[m_focusedCell->Column];
            //                cell->Select(_SelectionType::Normal);
            //                cell->Focus();
            //                cell->BringIntoView();
            //            }
            //            else
            //            {
            //                EditCell(m_focusedCell, gcnew EditingReason());
            //            }
            //        }
            //    }
            //    break;
            //case GrKeys_F12:
            //    {
            //        if(m_insertionRow->IsVisible == true)
            //        {
            //            m_insertionRow->Select(_SelectionType::Normal);
            //            m_insertionRow->Focus();
            //            m_insertionRow->BringIntoView();
            //        }
            //    }
            //    break;
            //case GrKeys_F2:
            //    {
            //        if(m_focusedCell != nullptr)
            //        {
            //            EditCell(m_focusedCell, gcnew EditingReason());
            //        }
            //        else
            //        {
            //            break;
            //        }
            //    }
            //    break;
            //case GrKeys_Escape:
            //    {
            //        if(m_focusedCell != nullptr && m_focusedCell->IsEdited == true)
            //        {
            //            m_focusedCell->CancelEdit();
            //            if(m_pGridCore->IsGrouped() == true)
            //                m_pGridCore->GetDataRowList()->Update(true);
            //        }
            //        else if(FocusedRow != nullptr && FocusedRow->IsEdited == true)
            //        {
            //            FocusedRow->CancelEdit();
            //            if(m_pGridCore->IsGrouped() == true)
            //                m_pGridCore->GetDataRowList()->Update(true);
            //        }
            //        Invalidate(false);
            //    }
            //    break;
            //case GrKeys_ProcessKey:
            //    {
            //        if(dynamic_cast<IEditByIme^>(FocusedColumn))
            //        {
            //            _Char nnn = Win32::API::ImmGetVirtualKey(Handle);
            //            if(_Char::IsLetter(nnn))
            //            {
            //                EditCell(m_focusedCell, gcnew EditingReason((System::Char)e->KeyValue, true));
            //            }
            //        }
            //    }
            //    break;
        default:
            break;

        }
    }

    RootPressing::RootPressing()
    {

    }

    bool RootPressing::GetHitTest(GrCell* pHitted, const GrPoint& /*localLocation*/)
    {
        return pHitted->GetCellType() == GrCellType_Root;
    }

    void RootPressing::OnBegin(GrStateEventArgs* e)
    {
        m_pGridCore->SetMousePress(e->GetCell());
        m_pItemSelector->SelectAll();
    }

    void RootPressing::OnEnd(GrStateEventArgs* /*e*/)
    {
        m_pGridCore->SetMouseUnpress();
    }

    ColumnPressing::ColumnPressing()
    {
        m_cursor     = GrCursor_Default;
        m_pColumn    = NULL;
        m_pOldDataColumn = NULL;
        m_handled    = false;
        m_targetType = TargetType_Unknown;

        m_pTimer = new GrSelectionTimer2();
        m_pTimer->Elapsed.Add(this, &ColumnPressing::timer_Elapsed);
    }

    ColumnPressing::~ColumnPressing()
    {
        delete m_pTimer;
    }

    void ColumnPressing::OnGridCoreAttached()
    {
        GrStateBase::OnGridCoreAttached();
        m_pColumnList = m_pGridCore->GetColumnList();
        m_pGridCore->AttachObject(m_pTimer);
    }

    bool ColumnPressing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        if(pHitted->GetCellType() != GrCellType_Column)
            return false;
        GrColumn* pColumn = (GrColumn*)pHitted;

        if(pColumn->GetResizable() == false)
            return true;

        int columnSplitter = m_pGridCore->GetColumnSplitter();
        if(localLocation.x < columnSplitter || localLocation.x >= pHitted->GetWidth() - columnSplitter)
            return false;
        return true;
    }

    void ColumnPressing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        GrColumnList* pColumnList = m_pGridCore->GetColumnList();
        m_pOldDataColumn = m_pColumn;
        m_pColumn = (GrColumn*)e->GetCell();

        GrColumnMouseEventArgs ce(m_pColumn, e->GetLocalHit(), e->GetModifierKeys());
        pColumnList->Invoke(L"ColumnMouseDown", &ce);
        m_handled = ce.GetHandled();

        if(m_handled == false)
        {
            if(m_pGridWindow->GetSelectionRange() == GrSelectionRange_Multi)
            {
                m_pItemSelector->SelectColumns(m_pColumn, m_pItemSelector->GetColumnAnchor(), m_pGridWindow->GetSelectionType());
                m_pFocuser->Set(m_pColumn);
            }
            else
            {
                switch(m_pGridWindow->GetSelectionType())
                {
                case GrSelectionType_Normal:
                    {
                        m_pItemSelector->SelectItems(m_pColumn, GrSelectionType_Normal);
                        m_pItemSelector->SetColumnAnchor(m_pColumn);
                        m_pFocuser->Set(m_pColumn);

                        IDataRow* pFocusedRow = m_pFocuser->GetFocusedRow();
                        if(pFocusedRow != NULL)
                            m_pItemSelector->SetRowAnchor(pFocusedRow);
                    }
                    break;
                default:
                    {
                        m_pItemSelector->SelectItems(m_pColumn, GrSelectionType_Add);
                        m_pFocuser->Set(m_pColumn);
                    }
                    break;
                }
            }
            m_pGridCore->SetMousePress(m_pColumn);
            m_locationStart = e->GetLocation();
        }
        else
        {
            m_locationStart = GrPoint::Empty;
        }

        m_pGridCore->Invalidate(m_pColumn->GetRect());
    }

    void ColumnPressing::OnMouseDragBegin(const GrPoint& /*location*/)
    {
        m_pTimer->Start();
    }

    void ColumnPressing::OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest)
    {
        m_cursor = GrCursor_No;

        if(hitTest.pHitted == NULL)
            return;

        m_pTimer->SetMouseLocation(location);

        m_targetType = TargetType_Unknown;

        switch(hitTest.pHitted->GetCellType())
        {
        case GrCellType_Column:
            {
                if(m_pGridCore->CanBeColumnMoving() == false || m_pColumn->GetMovable() == false)
                    break;

                if(m_pTimer->CanHScroll() == true)
                    break;

                GrColumn* pTarget = (GrColumn*)hitTest.pHitted;
                if(pTarget != m_pColumn)
                {
                    if(pTarget->GetFrozen() == true)
                    {
                        uint targetIndex;
                        if(hitTest.localHit.x < pTarget->GetWidth() / 2)
                            targetIndex = pTarget->GetFrozenIndex();
                        else
                            targetIndex = pTarget->GetFrozenIndex() + 1;

                        if(m_pColumn->GetFrozen() == false)
                        {
                            if(m_pGridCore->CanBeColumnFrozing() == false)
                                m_targetType = TargetType_Unknown;
                            else
                                m_targetType = TargetType_Frozen;
                        }
                        else if(targetIndex != m_pColumn->GetFrozenIndex() + 1 && targetIndex != m_pColumn->GetFrozenIndex()) 
                        {
                            m_targetType = TargetType_Frozen;
                        }
                        else
                        {
                            m_targetType = TargetType_Unknown;
                        }

                        if(targetIndex < m_pColumnList->GetFrozenColumnCount())
                            m_targetCell = m_pColumnList->GetFrozenColumn(targetIndex);
                        else
                            m_targetCell = NULL;
                    }
                    else
                    {
                        uint targetIndex;
                        if(hitTest.localHit.x < pTarget->GetWidth() / 2)
                            targetIndex = pTarget->GetUnfrozenIndex();
                        else
                            targetIndex = pTarget->GetUnfrozenIndex() + 1;

                        if(m_pColumn->GetFrozen() == true)
                        {
                            if(m_pGridCore->CanBeColumnFrozing() == false)
                                m_targetType = TargetType_Unknown;
                            else
                                m_targetType = TargetType_Unfrozen;
                        }
                        else if(targetIndex != m_pColumn->GetUnfrozenIndex() + 1 && targetIndex != m_pColumn->GetUnfrozenIndex()) 
                        {
                            m_targetType = TargetType_Unfrozen;
                        }
                        else
                        {
                            m_targetType = TargetType_Unknown;
                        }

                        if(targetIndex < m_pColumnList->GetUnfrozenColumnCount())
                            m_targetCell = m_pColumnList->GetUnfrozenColumn(targetIndex);
                        else
                            m_targetCell = NULL;
                    }
                }
            }
            break;
        case GrCellType_GroupingInfo:
            {
                if(m_pColumn->CanBeGrouping() == false)
                    break;

                if(m_pColumn->GetGrouped() == false)
                {
                    m_targetCell = (GrGroupingInfo*)hitTest.pHitted;
                    m_targetType = TargetType_GroupingList;
                }
            }
            break;
        case GrCellType_Row:
            {
                if(m_pColumn->CanBeGrouping() == false)
                    break;

                if(m_pColumn->GetGrouped() == false && dynamic_cast<GrGroupingList*>(hitTest.pHitted) != NULL)
                {
                    m_targetCell = NULL;
                    m_targetType = TargetType_GroupingList;
                }
            }
            break;
        default:
            break;
        }

        switch(m_targetType)
        {
        case TargetType_GroupingList:
            {
                m_cursor = GrCursor_Add;
            }
            break;
        case TargetType_Frozen:
        case TargetType_Unfrozen:
            {
                m_cursor = GrCursor_Move;
            }
            break;
        default:
            break;
        }

        m_pGridCore->Invalidate();
    }

    void ColumnPressing::OnMouseDragEnd(bool cancel, const GrHitTest& /*hitTest*/)
    {
        if(cancel == false)
        {
            switch(m_targetType)
            {
            case TargetType_Frozen:
                {
                    m_pColumnList->MoveToFrozen(m_pColumn, (GrColumn*)m_targetCell);
                }
                break;
            case TargetType_Unfrozen:
                {
                    m_pColumnList->MoveToUnfrozen(m_pColumn, (GrColumn*)m_targetCell);
                }
                break;
            case TargetType_GroupingList:
                {
                    if(m_targetCell != NULL)
                    {
                        GrGroupingInfo* pTargetGroupingInfo = (GrGroupingInfo*)m_targetCell;
                        GrGroupingInfo* pGroupingInfo = m_pColumn->GetGroupingInfo();
                        pGroupingInfo->SetGroupingLevel(pTargetGroupingInfo->GetGroupingLevel());
                    }
                    m_pColumn->SetGrouped(true);
                }
                break;
            default:
                break;
            }
            m_pGridCore->Invalidate();
        }
        m_targetType = TargetType_Unknown;
        m_cursor = GrCursor_Default;
        m_pTimer->Stop();
    }

    void ColumnPressing::OnMouseUp(GrStateMouseEventArgs* e)
    {
        if(m_handled == true)
        {
            GrColumnList* pColumnList = m_pGridCore->GetColumnList();
            GrColumnMouseEventArgs ce(m_pColumn, e->GetLocalHit(), e->GetModifierKeys());
            pColumnList->Invoke(L"ColumnMouseUp", &ce);
            m_handled = ce.GetHandled();
        }

        if(m_handled == false)
        {
            if(EnableSorting(e->GetLocation()) == true)
            {
                int n = ((int)m_pColumn->GetSortType() + 1) % (int)GrSort_Count;
                GrSort sortType = (GrSort)n;

                m_pColumn->SetSortType(sortType);

                m_pGridCore->Update();
                m_pGridCore->BringIntoView(m_pFocuser->GetItem());
            }
        }

        m_pTimer->Stop();
        m_pGridCore->Invalidate(m_pColumn->GetRect());
        m_pGridCore->SetMouseUnpress();
    }

    void ColumnPressing::OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect)
    {
        const int padding = 2;
        switch(m_targetType)
        {
        case TargetType_Frozen:
            {
                int x;
                if(m_targetCell == NULL)
                {
                    x = m_pColumnList->GetColumnSplitter()->GetX();
                }
                else
                {
                    x = ((GrColumn*)m_targetCell)->GetX();
                }
                GrRect paintRect(x - padding, m_pColumnList->GetY(), x + padding, m_pColumnList->GetY() + m_pColumnList->GetHeight());
                g->FillRectangle(paintRect, GrColor::Black);
            }
            break;
        case TargetType_Unfrozen:
            {
                int x;
                if(m_targetCell == NULL)
                {
                    x = std::min(displayRect.right, m_pColumnList->GetBounds().right);
                }
                else
                {
                    x = ((GrColumn*)m_targetCell)->GetX();
                }
                GrRect paintRect(x - padding, m_pColumnList->GetY(), x + padding, m_pColumnList->GetY() + m_pColumnList->GetHeight());
                g->FillRectangle(paintRect, GrColor::Black);
            }
            break;
        case TargetType_GroupingList:
            {
                if(m_targetCell != NULL)
                {
                    int x = m_targetCell->GetX();
                    GrRect paintRect(x - padding, m_targetCell->GetY(), x + padding, m_targetCell->GetY() + m_targetCell->GetHeight());
                    g->FillRectangle(paintRect, GrColor::Black);
                }
            }
            break;
        default:
            return;
        }
    }

    bool ColumnPressing::EnableSorting(const GrPoint& location)
    {
        if(m_pGridCore->CanBeColumnSorting() == false)
            return false;

        if(m_targetType == TargetType_Unknown && 
            m_pOldDataColumn == m_pColumn &&
            location == m_locationStart)
        {
            return true;
        }
        return false;
    }

    void ColumnPressing::timer_Elapsed(GrObject* /*pSender*/, GrElapsedEventArgs* /*e*/)
    {
        m_pTimer->DoScroll();
        m_pGridCore->Invalidate();
    }

    ColumnPressing::GrSelectionTimer2::GrSelectionTimer2()
    {
        SetVScrollEnabled(false);
        SetIntervalAcceleration(false);
        SetScrollAcceleration(false);
    }

    GrRect ColumnPressing::GrSelectionTimer2::GetInsideRectangle() const
    {
        GrColumnList* pColumnList = m_pGridCore->GetColumnList();
        const int gap = 20;
        GrRect rect = m_pGridCore->GetDataRect();
        return GrRect(rect.left + gap, pColumnList->GetY(), rect.right - gap, rect.bottom);
    }

    GrRect ColumnPressing::GrSelectionTimer2::GetOutsideRectangle() const
    {
        GrColumnList* pColumnList = m_pGridCore->GetColumnList();
        GrRect rect = m_pGridCore->GetDataRect();
        return GrRect(rect.left, pColumnList->GetY(), rect.right, rect.bottom);
    }


    ColumnResizing::ColumnResizing()
    {

    }

    bool ColumnResizing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        if(pHitted->GetCellType() != GrCellType_Column)
            return false;

        GrColumn* pColumn = (GrColumn*)pHitted;
        return GetResizingColumn(pColumn, localLocation) != NULL;
    }

    GrColumn* ColumnResizing::GetResizingColumn(GrColumn* pColumn, const GrPoint& localLocation)
    {
        int columnSplitter = m_pGridCore->GetColumnSplitter();
        if(localLocation.x >= pColumn->GetWidth() - columnSplitter)
        {
            if(pColumn->GetResizable() == true)
                return pColumn;
        }
        else if(localLocation.x < columnSplitter)
        {
            uint index = pColumn->GetVisibleIndex();
            if((int)index > 0)
            {
                GrColumnList* pColumnList = pColumn->GetColumnList();
                pColumn = pColumnList->GetVisibleColumn(index - 1);
                if(pColumn->GetResizable() == true)
                    return pColumn;
            }
        }
        return NULL;
    }

    void ColumnResizing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        m_pColumn = GetResizingColumn(e->GetCell<GrColumn*>(), e->GetLocalHit());

        GrRect columnRect = m_pColumn->GetRect();

        m_resizingStart    = columnRect.right;
        m_resizingLocation = columnRect.right;
        m_resizingMin      = columnRect.left + m_pColumn->GetMinWidth();
        m_resizingMax      = columnRect.left + m_pColumn->GetMaxWidth();

        m_downX = e->GetX();
    }

    void ColumnResizing::OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect)
    {
        g->DrawResizingLine(m_resizingLocation, displayRect.top, m_resizingLocation, displayRect.bottom);
    }

    void ColumnResizing::OnMouseDragMove(const GrPoint& location, const GrHitTest& /*hitTest*/)
    {
        int oldX = m_resizingLocation;
        m_resizingLocation = (location.x - m_downX) + m_resizingStart;
        m_resizingLocation = std::max(m_resizingLocation, m_resizingMin);
        m_resizingLocation = std::min(m_resizingLocation, m_resizingMax);

        if(oldX != m_resizingLocation)
        {
            m_pGridCore->Invalidate();
        }
    }

    void ColumnResizing::OnMouseDragEnd(bool cancel, const GrHitTest& /*hitTest*/)
    {
        if(cancel == true)
        {
            m_pGridCore->Invalidate();
            return;
        }

        int newWidth = m_pColumn->GetWidth() + (m_resizingLocation - m_resizingStart);
        if(m_pColumn->GetWidth() != newWidth)
        {
            GrColumnList* pColumnList = m_pColumn->GetColumnList();
            int x = INT_MAX;
            if(m_pColumn->GetFullSelected() == true)
            {
                for(uint i=0 ; i<pColumnList->GetVisibleColumnCount() ; i++)
                {
                    GrColumn* pColumn = pColumnList->GetVisibleColumn(i);
                    if(pColumn->GetFullSelected() == false)
                        continue;
                    if(pColumn->GetResizable() == false)
                        continue;
                    pColumn->SetWidth(newWidth);
                    x = std::min(x, pColumn->GetX());
                }
                if(x == INT_MAX)
                    x = 0;
            }
            else
            {
                m_pColumn->SetWidth(newWidth);
                x = m_pColumn->GetX();
            }
            GrRect displayRect = m_pGridCore->GetDisplayRect();
            m_pGridCore->Invalidate(x, displayRect.top, displayRect.right, displayRect.bottom);
        }
        else
        {
            m_pGridCore->Invalidate();
        }
    }

    void ColumnResizing::OnMouseDoubleClick(GrStateMouseEventArgs* /*e*/)
    {
        GrColumnList* pColumnList = m_pGridCore->GetColumnList();

        if(m_pColumn->GetFullSelected() == true)
        {
            for(uint i=0 ; i<pColumnList->GetVisibleColumnCount() ; i++)
            {
                GrColumn* pColumn = pColumnList->GetVisibleColumn(i);
                if(pColumn->GetFullSelected() == false)
                    continue;
                if(pColumn->GetResizable() == false)
                    continue;
                pColumn->SetFit();
            }
        }
        else
        {
            m_pColumn->SetFit();
        }
        m_pGridCore->Invalidate();
    }

    ColumnSplitterMoving::ColumnSplitterMoving()
    {

    }

    bool ColumnSplitterMoving::GetHitTest(GrCell* pHitted, const GrPoint& /*localLocation*/)
    {
        return pHitted->GetCellType() == GrCellType_Splitter;    
    }

    void ColumnSplitterMoving::OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect)
    {
        GrRect paintRect(GrPoint(m_location, displayRect.top), GrSize(GrColumnSplitter::DefaultSplitterWidth, displayRect.GetHeight()));
        g->DrawSplitterMovingLine(paintRect);
    }

    void ColumnSplitterMoving::OnBegin(GrStateEventArgs* e)
    {
        m_pColumnSplitter = dynamic_cast<GrColumnSplitter*>(e->GetCell());
    }

    void ColumnSplitterMoving::OnMouseDragBegin(const GrPoint& /*location*/)
    {
        m_location = m_pColumnSplitter->GetX();
        m_pGridCore->Invalidate();
    }

    void ColumnSplitterMoving::OnMouseDragMove(const GrPoint& location, const GrHitTest& /*hitTest*/)
    {
        GrColumnList* pColumnList = m_pColumnSplitter->GetColumnList();

        int oldLocation = m_location;

        for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
        {
            const GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
            GrRect columnRect = pColumn->GetRect();
            if(location.x < columnRect.left || location.x >= columnRect.right)
                continue;
            int centerValue = columnRect.GetCenter().x;

            if(location.x > centerValue)
            {
                if(pColumn->GetFrozen())
                {
                    m_location = columnRect.right;
                    m_frozenIndex = i;
                }
                else
                {
                    m_location = columnRect.right - GrColumnSplitter::DefaultSplitterWidth;
                    m_frozenIndex = i;
                }
            }
            else
            {
                if(pColumn->GetFrozen())
                    m_location = columnRect.left;
                else
                    m_location = columnRect.left - GrColumnSplitter::DefaultSplitterWidth;
                m_frozenIndex = i - 1;
            }
        }

        if(oldLocation != m_location)
        {
            GrRect displayRect = m_pGridCore->GetDisplayRect();
            m_pGridCore->Invalidate(oldLocation - 2, displayRect.top, oldLocation + GrColumnSplitter::DefaultSplitterWidth + 2, displayRect.bottom);
            m_pGridCore->Invalidate(m_location - 2, displayRect.top, m_location + GrColumnSplitter::DefaultSplitterWidth + 2, displayRect.bottom);
        }
    }

    void ColumnSplitterMoving::OnMouseDragEnd(bool cancel, const GrHitTest& /*hitTest*/)
    {
        if(cancel == true)
        {
            GrRect displayRect = m_pGridCore->GetDisplayRect();
            m_pGridCore->Invalidate(m_location - 2, displayRect.top, m_location + GrColumnSplitter::DefaultSplitterWidth + 2, displayRect.bottom);
            return;
        }

        GrColumnList* pColumnList = m_pColumnSplitter->GetColumnList();

        for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
        {
            GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
            bool oldFrozen = pColumn->GetFrozen();
            if(m_frozenIndex == -1)
                pColumn->SetFrozen(false);
            else
                pColumn->SetFrozen(i <= (uint)m_frozenIndex);

            if(oldFrozen != pColumn->GetFrozen())
            {

            }
        }

        m_pGridCore->Invalidate();
    }

    ItemPressing::ItemPressing()
    {
        m_pTimer = new GrSelectionTimer();
    }

    ItemPressing::~ItemPressing()
    {
        delete m_pTimer;
    }

    bool ItemPressing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        if(pHitted->GetCellType() != GrCellType_Item)
            return false;

        GrItem* pItem = (GrItem*)pHitted;
        if(pItem->HitMouseOverTest(localLocation) != GrMouseOverState_In)
            return false;

        if(pItem->GetReadOnly() == true)
            return true;

        GrClickEditing editingType = pItem->GetColumn()->GetItemClickEditing();
        if(editingType == GrClickEditing_Default)
        {
            editingType = m_pGridCore->GetClickEditing();
        }

        switch(editingType)
        {
        case GrClickEditing_Click:
        case GrClickEditing_DoubleClick:
            {
                GrEditingReason reason(localLocation);
                if(m_pGridWindow->CanEdit(pItem, reason) == false)
                    return true;
            }
            break;
        case GrClickEditing_FocusedClick:
            {
                if(pItem->GetFocused() == true)
                    return false;
            }
            break;
        }
        

        //if(pItem->GetFocused() == true)
        //    return false;

        return true;
    }

    void ItemPressing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        m_pItem = dynamic_cast<GrItem*>(e->GetCell());

        if(m_pGridWindow->GetSelectionRange() == GrSelectionRange_Multi)
        {
            if(m_pItemSelector->CanSelect(m_pItem) == true)
            {
                GrIndexRange columnRange = m_pItemSelector->GetColumnSelections(m_pItem);
                GrIndexRange rowRange    = m_pItemSelector->GetRowSelections(m_pItem);

                switch(m_pGridWindow->GetSelectionType())
                {
                case GrSelectionType_Add:
                    break;
                case GrSelectionType_Normal:
                    m_pItemSelector->ClearSelection();
                    break;
                default:
                    break;
                }

                if(rowRange != GrIndexRange::Empty)
                {
                    m_pItemSelector->BeginSelecting();
                    m_pItemSelector->Selecting(columnRange, rowRange);
                    m_pFocuser->SetFocusing(m_pItem);
                }
            }
        }
        else
        {
            switch(m_pGridWindow->GetSelectionType())
            {
            case GrSelectionType_Add:
                {
                    if(m_pItemSelector->CanSelect(m_pItem) == true)
                    {
                        if(m_pGridCore->GetFullRowSelect() == true)
                            m_pItemSelector->SelectItems(m_pItem->GetDataRow(), GrSelectionType_Add);
                        else
                            m_pItemSelector->SelectItem(m_pItem, GrSelectionType_Add);
                        m_pItemSelector->SetAnchor(m_pItem);
                        m_pFocuser->Set(m_pItem);
                    }
                }
                break;
            case GrSelectionType_Normal:
                {
                    if(m_pGridCore->GetFullRowSelect() == true)
                        m_pItemSelector->SelectItems(m_pItem->GetDataRow(), GrSelectionType_Normal);
                    else
                        m_pItemSelector->SelectItem(m_pItem, GrSelectionType_Normal);

                    m_pItemSelector->SetSelectionGroup(m_pItem);
                    m_pItemSelector->SetAnchor(m_pItem);
                    m_pFocuser->Set(m_pItem);

                    if(m_pItem->GetReadOnly() == false)
                    {
                        static GrEditingReason editingReason;
                        GrClickEditing clickEdit = m_pItem->GetColumn()->GetItemClickEditing();
                        if(clickEdit == GrClickEditing_Default)
                            clickEdit = m_pGridCore->GetClickEditing();

                        GrEditingReason reason(e->GetLocalHit());
                        if(clickEdit == GrClickEditing_Click && m_pGridWindow->CanEdit(m_pItem, reason) == true)
                        {
                            editingReason.editingType = GrEditingType_Mouse;
                            editingReason.location = e->GetLocation();
                            e->SetNextState(GrGridState_ItemEditing);
                            e->SetUserData(&editingReason);
                        }
                    }
                }
                break;
            default:
                break;
            }

            m_pGridCore->SetMousePress(m_pItem);
            if(e->GetNextState() == GrGridState_ItemPressing)
                m_pGridCore->BringIntoView(m_pFocuser->GetItem());
        }
    }

    void ItemPressing::OnMouseClick(GrStateMouseEventArgs* e)
    {
        GrItemMouseEventArgs e1(m_pItem, e->GetLocation(), e->GetModifierKeys());
        m_pGridCore->Invoke(L"ItemMouseClick", &e1);
    }

    void ItemPressing::OnMouseDoubleClick(GrStateMouseEventArgs* e)
    {
        GrEditingReason reason(e->GetLocalHit());
        if(m_pItem->GetReadOnly() == true || m_pGridWindow->CanEdit(m_pItem, reason) == false)
        {
            GrItemMouseEventArgs e1(m_pItem, e->GetLocation(), e->GetModifierKeys());
            m_pGridCore->Invoke(L"ItemMouseDoubleClick", &e1);
        }
    }

    void ItemPressing::OnMouseUp(GrStateMouseEventArgs* /*e*/)
    {
        if(m_pItemSelector->IsSelecting())
        {
            m_pItemSelector->EndSelecting(m_pGridWindow->GetSelectionType());
            m_pGridCore->BringIntoView(m_pFocuser->GetItem());
        }
    }

    void ItemPressing::OnMouseDragBegin(const GrPoint& /*location*/)
    {
        if(m_pItemSelector->IsSelecting() == false)
            m_pItemSelector->BeginSelecting();

        switch(m_pGridWindow->GetSelectionType())
        {
        case GrSelectionType_Add:
            break;
        case GrSelectionType_Normal:
            m_pItemSelector->ClearSelection();
            break;
        default:
            break;
        }

        m_pGridCore->SetMouseOver(NULL, GrPoint::Empty);
        m_pTimer->Start();
    }

    void ItemPressing::OnMouseDragMove(const GrPoint& location, const GrHitTest& /*hitTest*/)
    {
        FindSelection fs(m_pGridCore, location);
        m_pItemSelector->Selecting(fs.GetColumnRange(), fs.GetRowRange());
        m_pFocuser->SetFocusing(fs.GetFocus());
        m_pTimer->SetMouseLocation(m_pGridWindow->ClientToScreen(location));
        m_location = location;
    }

    void ItemPressing::OnMouseDragEnd(bool /*cancel*/, const GrHitTest& /*hitTest*/)
    {
        m_pTimer->Stop();

        if(m_pItemSelector->IsSelecting())
        {
            m_pItemSelector->EndSelecting(m_pGridWindow->GetSelectionType());
            m_pGridCore->BringIntoView(m_pFocuser->GetItem());
        }
    }

    void ItemPressing::OnGridCoreAttached()
    {
        GrStateBase::OnGridCoreAttached();
        m_pGridCore->AttachObject(m_pTimer);

        m_pTimer->SetInterval(300);
        m_pTimer->Elapsed.Add(this, &ItemPressing::timer_Elapsed);
    }

    void ItemPressing::timer_Elapsed(GrObject* /*pSender*/, GrElapsedEventArgs* /*e*/)
    {
        if(m_pTimer->DoScroll() == true)
        {
            FindSelection fs(m_pGridCore, m_location);

            m_pItemSelector->Selecting(fs.GetColumnRange(), fs.GetRowRange());
            m_pFocuser->SetFocusing(fs.GetFocus());
            m_pGridCore->Invalidate(); 
        }
    }

    ItemPressing::FindSelection::FindSelection(GrGridCore* pGridCore, const GrPoint& location)
        : m_pGridCore(pGridCore)
    {
        m_pItemSelector = pGridCore->GetItemSelector();
        m_pFocuser      = pGridCore->GetFocuser();


        GrIndexRange columnRange = m_pGridCore->GetColumnList()->HitTest(location.x, m_pItemSelector->GetColumnAnchor());
        GrIndexRange rowRange    = m_pGridCore->GetDataRowList()->HitTest(location.y, m_pItemSelector->GetRowAnchor());

        m_columnBegin    = columnRange.GetMinValue();
        m_columnEnd        = columnRange.GetMaxValue();
        m_rowBegin        = rowRange.GetMinValue();
        m_rowEnd        = rowRange.GetMaxValue();

        GrColumn* pFocusColumn = GetFocusColumn(m_columnBegin, m_columnEnd);
        IDataRow* pFocusRow    = GetFocusRow(m_rowBegin, m_rowEnd);

        m_pFocus = pFocusRow->GetFocusable(pFocusColumn);
    }

    IDataRow* ItemPressing::FindSelection::GetFocusRow(uint begin, uint end)
    {
        const GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
        const IDataRow* pAnchorRow = m_pItemSelector->GetRowAnchor();
        if(pAnchorRow->GetVisibleIndex() == begin)
            return pDataRowList->GetVisibleRow(end-1);
        return pDataRowList->GetVisibleRow(begin);
    }

    GrColumn* ItemPressing::FindSelection::GetFocusColumn(uint begin, uint end)
    {
        const GrColumnList* pColumnList = m_pGridCore->GetColumnList();
        const GrColumn* pAnchorColumn = m_pItemSelector->GetColumnAnchor();

        if(pAnchorColumn->GetVisibleIndex() == begin)
            return pColumnList->GetVisibleColumn(end-1);
        return pColumnList->GetVisibleColumn(begin);
    }

    ItemEditing::ItemEditing()
    {
        
    }

    bool ItemEditing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        GrItem* pItem = dynamic_cast<GrItem*>(pHitted);
        if(pItem == NULL || pItem->GetFocused() == false)
            return false;
        GrEditingReason reason(localLocation);
        return m_pGridWindow->CanEdit(pItem, reason);
    }

    void ItemEditing::OnBegin(GrStateEventArgs* e)
    {
        m_pItem = e->GetCell<GrItem*>();

        GrEditingReason* pReason = (GrEditingReason*)e->GetUserData();

        if(pReason != NULL && m_pItem != NULL)
            e->SetNextState(OnBegin(*pReason));
    }

    void ItemEditing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        GrEditingReason reason(e->GetLocation());
        e->SetNextState(OnBegin(reason));
    }

    void ItemEditing::OnMouseMove(GrStateMouseEventArgs* e)
    {
        GrColumnList* pColumnList = m_pGridCore->GetColumnList();
        GrCell* pCell = e->GetCell();
        GrCell* pOldCell = m_pGridCore->GetMouseOver();

        m_pGridCore->SetMouseOver(pCell, e->GetLocalHit());

        if(pCell != pOldCell)
        {
            if(pOldCell != NULL)
            {
                switch(pOldCell->GetCellType())
                {
                case GrCellType_Item:
                    {
                        GrItemMouseEventArgs e1((GrItem*)pOldCell, e->GetLocalHit(), e->GetModifierKeys());
                        m_pGridCore->Invoke(L"ItemMouseLeave", &e1);
                    }
                    break;
                case GrCellType_Column:
                    {
                        GrColumnMouseEventArgs e2((GrColumn*)pOldCell, e->GetLocalHit(), e->GetModifierKeys());
                        pColumnList->Invoke(L"ColumnMouseLeave", &e2);
                    }
                    break;
                default:
                    break;
                }
            }

            if(pCell != NULL)
            {
                switch(pCell->GetCellType())
                {
                case GrCellType_Item:
                    {
                        GrItemMouseEventArgs e1((GrItem*)pCell, e->GetLocalHit(), e->GetModifierKeys());
                        m_pGridCore->Invoke(L"ItemMouseEnter", &e1);
                        m_pGridCore->Invoke(L"ItemMouseMove", &e1);
                        if(e1.GetHandled() == true)
                        {
                            e->SetHandled(true);
                        }
                    }
                    break;
                case GrCellType_Column:
                    {
                        GrColumnMouseEventArgs e2((GrColumn*)pCell, e->GetLocalHit(), e->GetModifierKeys());
                        pColumnList->Invoke(L"ColumnMouseEnter", &e2);
                        pColumnList->Invoke(L"ColumnMouseMove", &e2);
                        if(e2.GetHandled() == true)
                        {
                            e->SetHandled(true);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    void ItemEditing::OnMouseUp(GrStateMouseEventArgs* e)
    {
        e->SetNextState(this->GetState());
        m_pGridCore->SetMouseUnpress();
        m_pGridCore->Invalidate();
    }

    GrGridState ItemEditing::OnBegin(GrEditingReason reason)
    {
        m_result  = GrEditingResult_None;
        m_pColumn = m_pItem->GetColumn();
        m_pItem->LockColor(true);

        GrEditEventArgs e(m_pItem, reason);
        m_pGridCore->Invoke(L"EditValue", &e);

        if(e.GetHandled() == true && m_pGridCore->IsGrouped() == true)
        {
            m_pGridCore->GetDataRowList()->SetListChanged();
        }
        return GrGridState_Normal;
    }

    void ItemEditing::OnEnd(GrStateEventArgs* /*e*/)
    {
        m_pItem->LockColor(false);
        m_pGridCore->Invalidate();
    }

    ItemButtonPressing::ItemButtonPressing()
    {

    }

    bool ItemButtonPressing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        if(pHitted->GetCellType() != GrCellType_Item)
            return false;
        return ((GrItem*)pHitted)->HitMouseOverTest(localLocation) == GrMouseOverState_Control;
    }

    void ItemButtonPressing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        m_pItem = e->GetCell<GrItem*>();
        m_pFocuser->Set(m_pItem);
        m_pGridCore->SetMousePress(m_pItem);
        m_pGridCore->Invalidate();
        m_mouseDownPoint = e->GetLocation();
    }

    void ItemButtonPressing::OnMouseUp(GrStateMouseEventArgs* e)
    {
        static GrEditingReason reason;
        
        if(m_pItem->HitMouseOverTest(e->GetLocalHit()) == GrMouseOverState_Control)
        {
            reason = GrEditingReason(e->GetLocation());

            e->SetNextState(GrGridState_ItemEditing);
            e->SetCell(m_pItem);
            e->SetUserData(&reason);
        }

        m_pGridCore->SetMouseUnpress();
        m_pGridCore->Invalidate();

        m_mouseDownPoint = GrPoint::Empty;
    }

    GroupingExpandPressing::GroupingExpandPressing()
    {

    }

    bool GroupingExpandPressing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        GrGroupingCell* pGroupingCell = dynamic_cast<GrGroupingCell*>(pHitted);
        if(pGroupingCell == NULL)
            return false;

        return pGroupingCell->HitMouseOverTest(localLocation) == GrMouseOverState_Control;
    }

    void GroupingExpandPressing::OnBegin(GrStateEventArgs* e)
    {
        GrGroupingCell* pGroupingCell = dynamic_cast<GrGroupingCell*>(e->GetCell());

        m_pGroupingRow = (GrGroupingRow*)pGroupingCell->GetRow();
    }

    void GroupingExpandPressing::OnMouseUp(GrStateMouseEventArgs* /*e*/)
    {
        m_pGroupingRow->Expand(!m_pGroupingRow->IsExpanded());
        m_pGridCore->Invalidate();
    }

    GroupingCellPressing::GroupingCellPressing()
    {

    }

    bool GroupingCellPressing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        GrGroupingCell* pGroupingCell = dynamic_cast<GrGroupingCell*>(pHitted);
        if(pGroupingCell == NULL)
            return false;

        return pGroupingCell->HitMouseOverTest(localLocation) == GrMouseOverState_In;
    }

    void GroupingCellPressing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        m_pGroupingCell = e->GetCell<GrGroupingCell*>();
        IDataRow* pDataRow = (IDataRow*)m_pGroupingCell->GetRow();

        if(m_pGridWindow->GetSelectionRange() == GrSelectionRange_Multi)
        {
            if(m_pItemSelector->CanSelect(pDataRow) == true)
            {
                m_pItemSelector->SelectDataRows(pDataRow, m_pItemSelector->GetRowAnchor(), m_pGridWindow->GetSelectionType());
                m_pFocuser->Set(m_pGroupingCell);
            }
        }
        else
        {
            switch(m_pGridWindow->GetSelectionType())
            {
            case GrSelectionType_Normal:
                {
                    m_pItemSelector->SetSelectionGroup(pDataRow);
                    m_pItemSelector->SetRowAnchor(pDataRow);
                    GrColumnList* pColumnList = m_pGridCore->GetColumnList();

                    GrColumn* pColumn = pColumnList->HitTest(e->GetX());
                    if(pColumn)
                        m_pItemSelector->SetColumnAnchor(pColumn);
                    m_pItemSelector->ClearSelection();
                    m_pFocuser->Set(m_pGroupingCell);
                }
                break;
            default:
                {
                    if(m_pItemSelector->CanSelect(pDataRow) == true)
                    {
                        //m_pItemSelector->SelectItems(pDataRow, GrSelectionType_Add);
                        m_pFocuser->Set(m_pGroupingCell);
                        m_pGridCore->SetMousePress(m_pGroupingCell);
                    }
                }
                break;
            }
        }
        m_pGridCore->Invalidate();
    }

    void GroupingCellPressing::OnMouseUp(GrStateMouseEventArgs* /*e*/)
    {
        m_pGridCore->SetMouseUnpress();
        m_pGridCore->Invalidate();
    }

    void GroupingCellPressing::OnMouseDoubleClick(GrStateMouseEventArgs* /*e*/)
    {
        GrGroupingRow* pGroupingRow = (GrGroupingRow*)m_pGroupingCell->GetRow();
        pGroupingRow->Expand(!pGroupingRow->IsExpanded());
        m_pGridCore->Invalidate();
    }

    GroupingInfoPressing::GroupingInfoPressing()
    {
        m_cursor = GrCursor_Default;
    }

    bool GroupingInfoPressing::GetHitTest(GrCell* pHitted, const GrPoint& /*localLocation*/)
    {
        return pHitted->GetCellType() == GrCellType_GroupingInfo;
    }

    void GroupingInfoPressing::OnPaintAdornments(GrGridPainter* g, const GrRect& /*displayRect*/)
    {
        if(m_where == INVALID_INDEX)
            return;

        GrGroupingList* pGroupingList = m_pGridCore->GetGroupingList();
        GrRect paintRect;
        if(m_where < pGroupingList->GetGroupingCount())
        {
            GrGroupingInfo* pGroupingInfo = pGroupingList->GetGrouping(m_where);
            paintRect = pGroupingInfo->GetRect();
        }
        else
        {
            uint lastIndex = pGroupingList->GetGroupingCount() - 1;
            GrGroupingInfo* pGroupingInfo = pGroupingList->GetGrouping(lastIndex);
            paintRect = pGroupingList->GetRect();
            paintRect.left = pGroupingInfo->GetRect().right;
        }

        const int padding = 2;
        g->FillRectangle(GrRect(paintRect.left - padding, paintRect.top, paintRect.left + padding, paintRect.bottom), GrColor::Black);
    }

    void GroupingInfoPressing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        m_pGroupingInfo = e->GetCell<GrGroupingInfo*>();
        m_pGridCore->SetMousePress(m_pGroupingInfo);
        m_where    = INVALID_INDEX;
        m_location = e->GetLocation();
        m_cursor   = GrCursor_Default;
    }

    void GroupingInfoPressing::OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest)
    {
        GrCell* pHittedCell = hitTest.pHitted;
        m_cursor = GrCursor_No;

        if(pHittedCell == NULL)
            return;

        GrRow* pRow = pHittedCell->GetRow();

        m_where = INVALID_INDEX;
        m_targetType = TargetType_Unknown;

        if(pRow->GetRowType() == GrRowType_GroupingList)
        {
            if(hitTest.pHitted->GetCellType() == GrCellType_GroupingInfo)
            {
                GrGroupingInfo* pHittedInfo = (GrGroupingInfo*)pHittedCell;
                if(pHittedInfo != m_pGroupingInfo && m_pGroupingInfo->GetGroupingLevel() + 1 != pHittedInfo->GetGroupingLevel())
                {
                    m_where = dynamic_cast<GrGroupingInfo*>(pHittedCell)->GetGroupingLevel();
                    m_targetType = TargetType_Grouping;
                }
            }
            else
            {
                GrGroupingList* pGroupingList = (GrGroupingList*)pRow;
                uint lastIndex = pGroupingList->GetGroupingCount() - 1;

                if(m_pGroupingInfo->GetGroupingLevel() != lastIndex)
                {
                    GrGroupingInfo* pLastGrouping = pGroupingList->GetGrouping(lastIndex);
                    if(location.x > pLastGrouping->GetRect().right)
                    {
                        m_where = pGroupingList->GetGroupingCount();
                        m_targetType = TargetType_Grouping;
                    }
                }
            }
        }
        else
        {
            m_targetType = TargetType_Remove;
        }

        switch(m_targetType)
        {
        case TargetType_Grouping:
            m_cursor = GrCursor_Move;
            break;
        case TargetType_Remove:
            m_cursor = GrCursor_Remove;
            break;
        default:
            break;
        }
        m_pGridCore->Invalidate();
    }

    void GroupingInfoPressing::OnMouseUp(GrStateMouseEventArgs* e)
    {
        if(m_location == e->GetLocation())
        {
            GrSort sortType = m_pGroupingInfo->GetSortType();
            if(sortType == GrSort_Up)
                m_pGroupingInfo->SetSortType(GrSort_Down);
            else
                m_pGroupingInfo->SetSortType(GrSort_Up);
        }

        m_pGridCore->SetMouseUnpress();
        m_pGridCore->Invalidate();
    }

    void GroupingInfoPressing::OnMouseDragEnd(bool cancel, const GrHitTest& /*hitTest*/)
    {
        if(cancel == true)
            return;

        switch(m_targetType)
        {
        case TargetType_Remove:
            m_pGroupingInfo->SetGrouped(false);
            break;
        case TargetType_Grouping:
            m_pGroupingInfo->SetGroupingLevel(m_where);
            break;
        default:
            break;
        }
    }

    void GroupingInfoPressing::OnEnd(GrStateEventArgs* /*e*/)
    {
        m_cursor = GrCursor_Default;
        m_where  = INVALID_INDEX;
    }

    RowPressing::RowPressing()
    {

    }

    bool RowPressing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        if(pHitted->GetCellType() != GrCellType_Row)
            return false;
        if(localLocation.y < m_pGridCore->GetRowSplitter() || localLocation.y >= pHitted->GetHeight() - m_pGridCore->GetRowSplitter())
            return false;
        return true;
    }

    void RowPressing::OnBegin(GrStateEventArgs* e)
    {
        m_pRow = e->GetCell<GrRow*>();

        switch(m_pRow->GetRowType())
        {
        case GrRowType_DataRow:
        case GrRowType_InsertionRow:
            {
                GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(m_pRow);

                if(m_pGridWindow->GetSelectionRange() == GrSelectionRange_Multi)
                {
                    if(m_pItemSelector->CanSelect(pDataRow) == true)
                    {
                        m_pItemSelector->SelectDataRows(pDataRow, m_pItemSelector->GetRowAnchor(), m_pGridWindow->GetSelectionType());
                        m_pFocuser->Set(pDataRow);
                    }
                }
                else
                {
                    switch(m_pGridWindow->GetSelectionType())
                    {
                    case GrSelectionType_Normal:
                        {
                            m_pItemSelector->SetSelectionGroup(pDataRow->GetSelectionGroup());
                            m_pItemSelector->SetRowAnchor(pDataRow);
                            m_pItemSelector->SelectItems(pDataRow, GrSelectionType_Normal);
                            m_pFocuser->Set(pDataRow);
                        }
                        break;
                    default:
                        {
                            if(m_pItemSelector->CanSelect(pDataRow) == true)
                            {
                                m_pItemSelector->SelectItems(pDataRow, GrSelectionType_Add);
                                m_pFocuser->Set(pDataRow);
                            }
                        }
                        break;
                    }
                }
            }
            break;
        case GrRowType_GroupingRow:
            {
                GrGroupingRow* pGroupingRow = (GrGroupingRow*)m_pRow;
                if(m_pGridWindow->GetSelectionRange() == GrSelectionRange_Multi)
                {
                    if(m_pItemSelector->CanSelect(pGroupingRow) == true)
                    {
                        m_pItemSelector->SelectDataRows(pGroupingRow, m_pItemSelector->GetRowAnchor(), m_pGridWindow->GetSelectionType());
                        m_pFocuser->Set(pGroupingRow);
                    }
                }
                else
                {
                    switch(m_pGridWindow->GetSelectionType())
                    {
                    case GrSelectionType_Normal:
                        {
                            m_pItemSelector->SetSelectionGroup(pGroupingRow);
                            m_pItemSelector->SetRowAnchor(pGroupingRow);
                            m_pItemSelector->ClearSelection();
                            m_pFocuser->Set(pGroupingRow);
                        }
                        break;
                    default:
                        {
                            if(m_pItemSelector->CanSelect(pGroupingRow) == true)
                            {
                                m_pFocuser->Set(pGroupingRow);
                            }
                        }
                        break;
                    }
                }
            }
            break;
        default:
            break;
        }
        m_pGridCore->SetMousePress(m_pRow);
    }

    void RowPressing::OnEnd(GrStateEventArgs* /*e*/)
    {
        m_pGridCore->SetMouseUnpress();
    }

    RowResizing::RowResizing()
    {

    }

    bool RowResizing::GetHitTest(GrCell* pHitted, const GrPoint& localLocation)
    {
        GrRow* pRow = dynamic_cast<GrRow*>(pHitted);
        if(pRow == NULL)
            return false;
        return GetResizingRow(pRow, localLocation) != NULL;
    }

    GrRow* RowResizing::GetResizingRow(GrRow* pRow, const GrPoint& localLocation)
    {
        int rowSplitter = m_pGridCore->GetRowSplitter();

        if(localLocation.y >= pRow->GetHeight() - rowSplitter)
        {
            if(pRow->GetResizable() == true)
                return pRow;
        }
        else if(localLocation.y < rowSplitter)
        {
            IDataRow* pDataRow = dynamic_cast<IDataRow*>(pRow);
            if(pDataRow && (int)pDataRow->GetVisibleIndex() > 0)
            {
                GrDataRowList* pDataRowList = pDataRow->GetGridCore()->GetDataRowList();
                pDataRow = pDataRowList->GetVisibleRow(pDataRow->GetVisibleIndex() - 1);
                if(pDataRow->GetResizable())
                    return pDataRow;
            }
        }
        return NULL;
    }

    void RowResizing::OnMouseDown(GrStateMouseEventArgs* e)
    {
        m_pRow = GetResizingRow(e->GetCell<GrRow*>(), e->GetLocalHit());

        GrRect rowRect = m_pRow->GetRect();

        m_resizingStart    = rowRect.bottom;
        m_resizingLocation = rowRect.bottom;
        m_resizingMin      = rowRect.top + 0;
        m_resizingMax      = rowRect.top + 1000;

        m_downY            = e->GetY();
    }

    void RowResizing::OnMouseDoubleClick(GrStateMouseEventArgs* /*e*/)
    {
        GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

        IDataRow* p = dynamic_cast<IDataRow*>(m_pRow);
        if(p && p->GetFullSelected() == true)
        {
            for(uint i=0 ; i<pDataRowList->GetVisibleRowCount() ; i++)
            {
                GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(i));
                if(pDataRow == NULL)
                    continue;
                if(pDataRow->GetFullSelected() == false)
                    continue;
                if(pDataRow->GetResizable() == false)
                    continue;
                pDataRow->SetFit();
            }
        }
        else
        {
            m_pRow->SetFit();
        }
        m_pGridCore->Invalidate();
    }

    void RowResizing::OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect)
    {
        g->DrawResizingLine(displayRect.left, m_resizingLocation, displayRect.right, m_resizingLocation);
    }

    void RowResizing::OnMouseDragMove(const GrPoint& location, const GrHitTest& /*hitTest*/)
    {
        int oldY = m_resizingLocation;
        m_resizingLocation = (location.y - m_downY) + m_resizingStart;
        m_resizingLocation = std::max(m_resizingLocation, m_resizingMin);
        m_resizingLocation = std::min(m_resizingLocation, m_resizingMax);

        if(oldY != m_resizingLocation)
        {
            m_pGridCore->Invalidate();
        }
    }

    void RowResizing::OnMouseDragEnd(bool /*cancel*/, const GrHitTest& /*hitTest*/)
    {
        int newHeight = m_pRow->GetHeight() + (m_resizingLocation - m_resizingStart);
        GrRect displayRect = m_pGridCore->GetDisplayRect();
        if(m_pRow->GetHeight() != newHeight)
        {
            IDataRow* p = dynamic_cast<IDataRow*>(m_pRow);
            if(p && p->GetFullSelected() == true)
            {
                GrDataRowList* pDataRowList = m_pRow->GetGridCore()->GetDataRowList();
                int y = INT_MAX;
                for(uint i=0 ; i<pDataRowList->GetVisibleRowCount() ; i++)
                {
                    GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(i));
                    if(pDataRow == NULL)
                        continue;
                    if(pDataRow->GetFullSelected() == false)
                        continue;
                    if(pDataRow->GetResizable() == false)
                        continue;
                    pDataRow->SetHeight(newHeight);

                    y = std::min(pDataRow->GetY(), y);
                }

                if(y == INT_MAX)
                    y = 0;
                m_pGridCore->Invalidate(displayRect.left, y, displayRect.right, displayRect.bottom);
            }
            else
            {
                m_pRow->SetHeight(newHeight);
                m_pGridCore->Invalidate(displayRect.left, m_pRow->GetY(), displayRect.right, displayRect.bottom);
            }
        }
        else
        {
            m_pGridCore->Invalidate();
        }
    }

} /*namespace GridStateClass*/