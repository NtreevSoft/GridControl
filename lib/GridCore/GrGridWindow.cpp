//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4475.19551
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


#include "GrGridWindow.h"
#include "GrGridCore.h"

GrEditingReason::GrEditingReason()
{
    editingType = GrEditingType_None;
}

GrEditingReason::GrEditingReason(wchar_t character, bool ime)
{
    if(ime == true)
        editingType = GrEditingType_Ime;
    else
        editingType = GrEditingType_Char;
    this->character = character;
}

GrEditingReason::GrEditingReason(GrKeys key)
{
    editingType = GrEditingType_Key;
    this->key = key;
}

GrEditingReason::GrEditingReason(GrPoint location)
{
    editingType = GrEditingType_Mouse;
    this->location = location;
}

GrEditEventArgs::GrEditEventArgs(GrItem* pItem, GrEditingReason reason)
: m_pItem(pItem), m_reason(reason), m_handled(false)
{

}

GrItem* GrEditEventArgs::GetItem() const 
{
    return m_pItem; 
}

GrEditingReason GrEditEventArgs::GetReason() const 
{
    return m_reason; 
}

bool GrEditEventArgs::GetHandled() const 
{
    return m_handled; 
}

void GrEditEventArgs::SetHandled(bool b)
{
    m_handled = b; 
}

GrElapsedEventArgs::GrElapsedEventArgs(time_t signalTime)
: m_signalTime(signalTime) 
{

}

time_t GrElapsedEventArgs::GetSignalTime() const
{
    return m_signalTime; 
}

void GrInvalidator::Invalidate(const GrRect& rect)
{
    Invalidate(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()); 
}

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

bool GrGridWindow::OnKeyDown(GrKeys key)
{
    m_pGridCore->GetStateManager()->OnKeyDown(key, GetModifierKeys());
    return false;
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
    GrGridPainter* pPainter = GetGridPainter();
    pPainter->BeginPaint(painterDevice);
    m_pGridCore->BeginPaint();
    m_pGridCore->PrePaint(pPainter, clipping);
    m_pGridCore->Paint(pPainter, clipping);
    m_pGridCore->PostPaint(pPainter, clipping);
    m_pGridCore->EndPaint();
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