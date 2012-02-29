#include "GrGridWindow.h"
#include "GrGridCore.h"

void GrTimer::InvokeElapsed(time_t signalTime)
{
    GrElapsedEventArgs e(signalTime);
    this->Elapsed.Raise(this, &e);
}

int GrScroll::ValidateValue(int value) const
{
    value = std::max(value, GetMinimum());
    value = std::min(value, GetMaximum() - GetLargeChange() + 1);

    return value;
}

bool GrScroll::DoScroll(int value)
{
    int newValue = ValidateValue(value);

    if(newValue != GetValue())
    {
        SetValue(newValue);
        m_pGridCore->Invalidate();
        return true;
    }
    return false;
}

GrRect GrGridWindow::ClientToScreen(const GrRect& rect) const
{
    GrPoint clientLocation = rect.GetLocation();
    GrPoint screenLocation = ClientToScreen(clientLocation);
    GrRect value = rect;
    value.Offset(screenLocation.x - clientLocation.x, screenLocation.y - clientLocation.y);
    return value;
}

GrSelectionType GrGridWindow::GetSelectionType() const
{
    if((GetModifierKeys() & GrKeys_Control))
        return GrSelectionType_Add;
    return GrSelectionType_Normal;
}

GrSelectionRange GrGridWindow::GetSelectionRange() const
{
    if((GetModifierKeys() & GrKeys_Shift))
        return GrSelectionRange_Multi;
    return GrSelectionRange_One;
}

void GrGridWindow::OnMouseDown(const GrPoint& location)
{
    m_pGridCore->GetStateManager()->OnMouseDown(location, GetModifierKeys());
}

void GrGridWindow::OnMouseMove(const GrPoint& location, bool pressed)
{
    m_pGridCore->GetStateManager()->OnMouseMove(location, GetModifierKeys(), pressed);
}

void GrGridWindow::OnMouseUp(const GrPoint& location)
{
    m_pGridCore->GetStateManager()->OnMouseUp(location, GetModifierKeys());
}

void GrGridWindow::OnMouseClick(const GrPoint& location)
{
    m_pGridCore->GetStateManager()->OnMouseClick(location, GetModifierKeys());
}

void GrGridWindow::OnMouseDoubleClick(const GrPoint& location)
{
    m_pGridCore->GetStateManager()->OnMouseDoubleClick(location, GetModifierKeys());
}

void GrGridWindow::OnMouseWheel(const GrPoint& location, int delta)
{
    m_pGridCore->GetStateManager()->OnMouseWheel(location, GetModifierKeys(), delta);
}

void GrGridWindow::OnMouseEnter()
{

}

void GrGridWindow::OnMouseLeave()
{
    m_pGridCore->SetMouseOver(NULL, GrPoint::Empty);
}

void GrGridWindow::OnKeyDown(GrKeys key)
{
    m_pGridCore->GetStateManager()->OnKeyDown(key, GetModifierKeys());
}

void GrGridWindow::OnKeyUp(GrKeys key)
{
    m_pGridCore->GetStateManager()->OnKeyUp(key, GetModifierKeys());
}

void GrGridWindow::OnGotFocus()
{

}

void GrGridWindow::OnLostFocus()
{

}

void GrGridWindow::OnSizeChanged(const GrRect& rect)
{
    m_pGridCore->SetDisplayRect(rect);
}

void GrGridWindow::OnUpdate()
{
    m_pGridCore->Update();
}

void GrGridWindow::OnPaint(void* painterDevice, const GrRect& clipping)
{
    GrRect clippingRect(clipping.left, clipping.top, clipping.right,clipping.bottom);
    GrGridPainter* pPainter = GetGridPainter();
    pPainter->BeginPaint(painterDevice);
    m_pGridCore->Paint(pPainter, clippingRect);
    pPainter->EndPaint();
}

void GrGridWindow::OnFontChanged()
{
    m_pGridCore->Invoke(L"FontChanged", &GrEventArgs::Empty);
}

void GrGridWindow::OnGridCoreAttached()
{
    GrObject::OnGridCoreAttached();

    GrScroll* pHorzScroll = GetHorzScroll();
    GrScroll* pVertScroll = GetVertScroll();

    m_pGridCore->AttachObject(pHorzScroll);
    m_pGridCore->AttachObject(pVertScroll);

    pHorzScroll->SetVisible(false);
    pVertScroll->SetVisible(false);
}

bool GrGridWindow::CanEdit(GrItem* pItem, GrEditingReason reason)
{
    if(pItem->GetReadOnly() == true)
        return false;

    switch(reason.editingType)
    {
    case GrEditingType_Mouse:
        return pItem->HitMouseOverTest(reason.location) != 0;
    case GrEditingType_Key:
        {
            if(reason.key == GrKeys_F2)
                return true;
            else if(reason.key == GrKeys_Enter)
                return true;
        }
        break;
    default:
        return false;
    }
    return false;
}