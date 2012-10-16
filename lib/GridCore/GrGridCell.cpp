//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4646.22417
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


#include "GrGridCell.h"
#include "GrGridCore.h"
#include "GrGridPainter.h"
#include "GrGridInternal.h"
#include "GrColumnList.h"
#include <assert.h>
#include <limits.h>

IFocusable* IFocusable::Null = 0;
uint GrCell::m_snID = 0;

int GrRow::DefaultHeight = 21;

GrEventArgs GrEventArgs::Empty;

GrMouseEventArgs::GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys)
    : m_location(location), m_modifierKeys(modifierKeys), m_delta(0)
{

}

GrMouseEventArgs::GrMouseEventArgs(const GrPoint& location, GrKeys modifierKeys, int delta)
    : m_location(location), m_modifierKeys(modifierKeys), m_delta(delta)
{

}

const GrPoint& GrMouseEventArgs::GetLocation() const
{
    return m_location;
}

int GrMouseEventArgs::GetX() const
{
    return m_location.x;
}

int GrMouseEventArgs::GetY() const
{
    return m_location.y;
}

GrKeys GrMouseEventArgs::GetModifierKeys() const
{
    return m_modifierKeys;
}

bool GrMouseEventArgs::GetControlKey() const
{
    return m_modifierKeys & GrKeys_Control ? true : false;
}

bool GrMouseEventArgs::GetAltKey() const
{
    return m_modifierKeys & GrKeys_Alt ? true : false;
}

bool GrMouseEventArgs::GetShiftKey() const
{
    return m_modifierKeys & GrKeys_Shift ? true : false;
}

int GrMouseEventArgs::GetDelta() const
{
    return m_delta;
}

GrColumnMouseEventArgs::GrColumnMouseEventArgs(GrColumn* pColumn, const GrPoint& location, GrKeys modifierKeys)
    : GrMouseEventArgs(location, modifierKeys), m_pColumn(pColumn), m_handled(false)
{

}

GrColumn* GrColumnMouseEventArgs::GetColumn() const
{
    return m_pColumn;
}

bool GrColumnMouseEventArgs::GetHandled() const
{
    return m_handled;
}

void GrColumnMouseEventArgs::SetHandled(bool value)
{
    m_handled = value;
}

GrCellMouseEventArgs::GrCellMouseEventArgs(GrCell* pCell, const GrPoint& location, GrKeys modifierKeys)
    : GrMouseEventArgs(location, modifierKeys), m_pCell(pCell)
{

}

GrCell* GrCellMouseEventArgs::GetCell() const
{
    return m_pCell;
}

GrItemMouseEventArgs::GrItemMouseEventArgs(GrItem* pItem, const GrPoint& location, GrKeys modifierKeys)
    : GrMouseEventArgs(location, modifierKeys), m_pItem(pItem), m_handled(false)
{

}

GrItem* GrItemMouseEventArgs::GetItem() const
{
    return m_pItem;
}

bool GrItemMouseEventArgs::GetHandled() const
{
    return m_handled;
}

void GrItemMouseEventArgs::SetHandled(bool value)
{
    m_handled = value;
}

GrRowMouseEventArgs::GrRowMouseEventArgs(GrRow* pRow, const GrPoint& location, GrKeys modifierKeys)
    : GrMouseEventArgs(location, modifierKeys), m_pRow(pRow), m_handled(false)
{

}

GrRow* GrRowMouseEventArgs::GetRow() const
{
    return m_pRow;
}

bool GrRowMouseEventArgs::GetHandled() const
{
    return m_handled;
}

void GrRowMouseEventArgs::SetHandled(bool value)
{
    m_handled = value;
}

GrFocusChangeArgs::GrFocusChangeArgs(IFocusable* pFocusable)
    : m_pFocusable(pFocusable)
{

}

IFocusable* GrFocusChangeArgs::GetFocusable() const 
{ 
    return m_pFocusable; 
}



GrCell::GrCell()
    : m_padding(GrPadding::Default)
{
    m_pFont = NULL;
    m_id = m_snID++;
    m_pTextUpdater = NULL;

    m_textBoundsChanged = false;
    m_textAlignChanged = false;
    m_textClipped = false;
    m_textVisible = true;

    Tag = NULL;
}

GrCell::~GrCell()
{
#ifdef _MANAGED
    ManagedRef = nullptr;
#endif
}

void GrCell::SetPadding(const GrPadding& padding)
{
    m_padding = padding;
    SetTextBoundsChanged();
}

GrRect GrCell::GetRect() const
{
    GrRect rect;
    rect.left = GetX();
    rect.top = GetY();
    rect.right = rect.left + GetWidth();
    rect.bottom = rect.top + GetHeight();
    return rect;
}

GrRect GrCell::GetClientRect() const
{
    GrRect clientRect;
    GrPadding padding = GetPadding();
    clientRect.left = padding.left;
    clientRect.top = padding.top;
    clientRect.right = GetWidth() - (padding.right);
    clientRect.bottom = GetHeight() - (padding.bottom);

    return clientRect;
}

GrPoint GrCell::GetLocation() const
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

const GrLineDesc& GrCell::GetTextLine(uint index) const
{
    return m_vecTextLine[index];
}

const GrRect& GrCell::GetTextBounds() const
{
    return m_textBounds;
}

void GrCell::AlignText()
{
    GrFont* pFont = GetPaintingFont();
    GrPoint startLocation;
    const GrPadding padding = GetPadding();
    const int width = GetWidth() - (padding.left + padding.right);
    const int height = GetHeight() - (padding.top + padding.bottom);

    int lineHeight = pFont->GetHeight() + pFont->GetExternalLeading();

    for(uint i=0 ; i<m_vecTextLine.size() ; i++)
    {
        GrLineDesc& cl = m_vecTextLine[i];

        switch(GetTextHorzAlign())
        {
        case GrHorzAlign_Left:
            startLocation.x = 0;
            break;
        case GrHorzAlign_Center:
            startLocation.x = (width - cl.width) / 2;
            break;
        case GrHorzAlign_Right:
            startLocation.x = width - cl.width;
            break;
        default:
            break;
        }

        switch(GetTextVertAlign())
        {
        case GrVertAlign_Top:
            startLocation.y = i * lineHeight;
            break;
        case GrVertAlign_Center:
            startLocation.y = (height - GetTextLineCount() * lineHeight) / 2 + i * lineHeight;
            break;
        case GrVertAlign_Bottom:
            startLocation.y = height - ((i+1) * lineHeight);
            break;
        default:
            break;
        }

        if(height < lineHeight)
            startLocation.y = i * lineHeight;

        cl.x = startLocation.x + padding.left;
        cl.y = startLocation.y + padding.top;

        if(i==0)
        {
            m_textBounds.SetLocation(cl.x, cl.y);
        }
    }
}

void GrCell::ComputeTextBounds()
{
    GrFont* pFont = GetPaintingFont();
    if(pFont == NULL)
        GetPaintingFont();
    GrPadding padding = GetPadding();
    m_vecTextLine.clear();

    int maxWidth = 0;
    int maxHeight = 0;
    GrRect oldTextBounds = m_textBounds;

    if(m_text.length() > 0)
    {
        int cellWidth = GetWidth() - (padding.left + padding.right);

        if(GetTextMulitiline() == false)
        {
            GrLineDesc textLine;
            GrTextUtil::SingleLine(&textLine, m_text, pFont);
            m_vecTextLine.push_back(textLine);
        }
        else
        {
            GrTextUtil::MultiLine(&m_vecTextLine, m_text, cellWidth, pFont, GetTextWordWrap());
        }

        for_each(_TextLines, m_vecTextLine, value)
        {
            const GrLineDesc& lineDesc = value;
            maxWidth = std::max(maxWidth, lineDesc.width);
            maxHeight += pFont->GetHeight() + pFont->GetExternalLeading();
        }
    }
    else
    {
        maxHeight = pFont->GetHeight() + pFont->GetExternalLeading();
    }

    if(maxWidth + (padding.left + padding.right) > GetWidth() || 
        maxHeight + padding.top + padding.bottom > GetHeight())
        m_textClipped = true;
    else
        m_textClipped = false;

    m_textBounds.SetSize(GrSize(maxWidth, maxHeight));
    if(m_textBounds != oldTextBounds)
        OnTextSizeChanged();
}

bool GrCell::GetTextVisible() const
{
    return m_textVisible;
}

void GrCell::SetTextVisible(bool b)
{
    m_textVisible = b;
}

bool GrCell::GetTextClipped() const
{
    return m_textClipped;
}

void GrCell::DrawText(GrGridPainter* pPainter, GrColor foreColor, const GrRect& paintRect, const GrRect* pClipRect) const
{
    if(GetTextVisible() == false)
        return;

    GrFont* pFont = GetPaintingFont();
    GrRect textRect;
    for(uint i=0 ; i<GetTextLineCount() ; i++)
    {
        const GrLineDesc& cl = GetTextLine(i);
        textRect.left = paintRect.left + cl.x;
        textRect.top = paintRect.top + cl.y;
        textRect.right = paintRect.left + cl.x + cl.width;
        textRect.bottom = paintRect.top + cl.y + pFont->GetHeight();
        if(textRect.top > paintRect.bottom || textRect.bottom <= paintRect.top)
            continue;

        if(GetTextClipped() == true || pClipRect != NULL)
        {
            GrRect clipRect = paintRect;
            clipRect.Contract(GetPadding());

            if(pClipRect != NULL)
            {
                clipRect.right = std::min(clipRect.right, pClipRect->right);
                clipRect.bottom = std::min(clipRect.bottom, pClipRect->bottom);
            }

            pPainter->DrawText(pFont, GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor, &clipRect);
        }
        else
        {
            pPainter->DrawText(pFont, GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor);
        }
    }
}

void GrCell::OnTextSizeChanged()
{

}

void GrCell::OnGridCoreAttached()
{
    GrObject::OnGridCoreAttached();
    m_pTextUpdater = m_pGridCore->GetTextUpdater();
    m_pTextUpdater->AddTextBounds(this);
    m_pTextUpdater->AddTextAlign(this);
}

void GrCell::OnGridCoreDetached()
{
    if(m_textAlignChanged == true)
        m_pTextUpdater->RemoveTextAlign(this);
    if(m_textBoundsChanged == true)
        m_pTextUpdater->RemoveTextBounds(this);
    m_pTextUpdater = NULL;
    GrObject::OnGridCoreDetached();
}

bool GrCell::Contains(const GrPoint& point) const
{
    return ContainsHorz(point.x) && ContainsVert(point.y);
}

bool GrCell::ContainsHorz(int x) const
{
    if(x < GetX() || x >= GetRight())
        return false;
    return true;
}

bool GrCell::ContainsVert(int y) const
{
    if(y < GetY() || y >= GetBottom())
        return false;
    return true;
}

const std::wstring& GrCell::GetText() const
{
    return m_text;
}

void GrCell::SetText(const std::wstring& text)
{
    if(m_text.compare(text) == 0)
        return;
    m_text = text;
    OnTextChanged();
}

void GrCell::SetTextBoundsChanged()
{
    if(m_pTextUpdater != NULL)
    {
        m_pTextUpdater->AddTextBounds(this);
        if(GetDisplayable() == true)
        {
            Invalidate();
        }
    }
}

void GrCell::SetTextAlignChanged()
{
    if(m_pTextUpdater != NULL)
    {
        m_pTextUpdater->AddTextAlign(this);
        if(GetDisplayable() == true)
        {
            Invalidate();
        }
    }
}

void GrCell::Invalidate(bool thisOnly)
{
    if(m_pGridCore == NULL || GetDisplayable() == false)
        return;

    if(thisOnly == true)
        m_pGridCore->Invalidate(GetRect());
    else
        m_pGridCore->Invalidate();
}

void GrCell::OnTextChanged()
{
    SetTextBoundsChanged();
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

GrFont* GrCell::GetPaintingFont() const
{
    return GetFont();
}

GrColor GrCell::GetPaintingForeColor() const
{
    return GetForeColor();
}

GrColor GrCell::GetPaintingBackColor() const
{
    return GetBackColor();
}

GrColor GrCell::GetPaintingLineColor() const
{
    return GetLineColor();
}

GrColor GrCell::GetForeColor() const
{
    if(m_foreColor != GrColor::Empty)
        return m_foreColor;

    if(m_pGridCore != NULL)
    {
        GrStyle* pStyle = m_pGridCore->GetStyle();
        if(pStyle != NULL)
            return pStyle->ForeColor;
        return m_pGridCore->GetForeColor();
    }
    return GrStyle::DefaultStyle.ForeColor;
}

GrColor GrCell::GetBackColor() const
{
    if(m_backColor != GrColor::Empty)
        return m_backColor;

    if(m_pGridCore != NULL)
    {
        GrStyle* pStyle = m_pGridCore->GetStyle();
        if(pStyle != NULL)
            return pStyle->BackColor;
        return m_pGridCore->GetBackColor();
    }
    return GrStyle::DefaultStyle.BackColor;
}

GrColor GrCell::GetLineColor() const
{
    if(m_lineColor != GrColor::Empty)
        return m_lineColor;

    if(m_pGridCore != NULL)
    {
        GrStyle* pStyle = m_pGridCore->GetStyle();
        if(pStyle != NULL)
            return pStyle->LineColor;
        return m_pGridCore->GetLineColor();
    }
    return GrStyle::DefaultStyle.LineColor;
}

GrFont* GrCell::GetFont() const
{
    if(m_pFont != NULL)
        return m_pFont;

    if(m_pGridCore != NULL)
    {
        GrStyle* pStyle = m_pGridCore->GetStyle();
        if(pStyle != NULL)
            return pStyle->Font;
        return m_pGridCore->GetFont();
    }
    return m_pFont;
} 

GrColor GrCell::GetForeColorCore() const
{
    return m_foreColor;
}

GrColor GrCell::GetBackColorCore() const
{
    return m_backColor;
}

GrColor GrCell::GetLineColorCore() const
{
    return m_lineColor;
}

GrFont*	GrCell::GetFontCore() const
{
    return m_pFont;
}

GrPadding GrCell::GetPadding(bool /*inherited*/) const
{
    return m_padding;
}

GrFlag GrCell::ToPaintStyle() const
{
    GrFlag flag;
    if(GetMouseOvered() == true)
        flag += GrPaintStyle_Mouseover;
    if(GetMousePressed() == true)
        flag += GrPaintStyle_Pressed;
    return flag;
}

uint GrCell::GetID() const
{
    return m_id;
}

void GrCell::SetBackColor(const GrColor& color)
{
    m_backColor = color;
    Invalidate();
}

void GrCell::SetForeColor(const GrColor& color)
{ 
    m_foreColor = color;
    Invalidate();
}

void GrCell::SetLineColor(const GrColor& color)
{
    m_lineColor = color;
    Invalidate();
}

void GrCell::SetFont(GrFont* pFont)
{
    if(m_pFont == pFont)
        return;
    m_pFont = pFont;
    SetTextBoundsChanged();
}

int GrCell::HitMouseOverTest(const GrPoint& localLocation) const
{
    if(localLocation.x < 0 || localLocation.y < 0 || 
        localLocation.x >= GetWidth() || localLocation.y >= GetHeight())
        return 0;
    return 1;
}

bool GrCell::GetMouseOvered() const
{
    if(m_pGridCore == NULL)
        return false;
    return m_pGridCore->GetMouseOver() == this;
}

int GrCell::GetMouseOverState() const
{
    if(m_pGridCore == NULL)
        return 0;
    return m_pGridCore->GetMouseOverState();
}

bool GrCell::GetMousePressed() const
{
    if(m_pGridCore == NULL)
        return false;
    return m_pGridCore->GetMousePress() == this;
}

GrItem::GrItem(GrColumn* pColumn, GrDataRow* pDataRow)
{
    m_pColumn = pColumn;
    m_pDataRow = pDataRow;
    m_readOnly = false;
    m_selected = false;
    m_colorLocked= false;
}

int GrItem::HitMouseOverTest(const GrPoint& localLocation) const
{
    if(GrCell::HitMouseOverTest(localLocation) == 0)
        return 0;

    if(GetReadOnly() == true)
        return GrMouseOverState_In;

    if(m_pColumn->GetItemType() == GrItemType_Control)
        return GrMouseOverState_In;

    GrRect controlRect = GetControlRect();
    if(controlRect.Contains(localLocation) == true)
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
        default:
            break;
        }
    }

    return GrMouseOverState_In;
}

GrRect GrItem::GetControlRect() const
{
    GrRect rect;
    rect.left = GetWidth() - DEF_CONTROL_WIDTH;
    rect.top = 0;
    rect.right = GetWidth() - 1;
    rect.bottom = GetHeight() - 1;
    return rect;
}

bool GrItem::GetControlVisible() const
{
    if(m_pColumn->GetItemType() == GrItemType_Control)
        return false;

    bool showControl = false;

    switch(m_pColumn->GetItemTypeShow())
    {
    case GrItemTypeShow_SelectedOnly:
        if(IsItemSelecting() == true || GetSelected() == true)
            showControl = true;
        break;
    case GrItemTypeShow_FocusedOnly:
        if(GetFocused() == true)
            showControl = true;
        break;
    case GrItemTypeShow_Always:
        showControl = true;
        break;
    default:
        break;
    }

    if(GetReadOnly() == true)
        showControl = false;

    return showControl;
}

GrColumn* GrItem::GetColumn() const
{
    return m_pColumn; 
}

GrDataRow* GrItem::GetDataRow() const
{ 
    return m_pDataRow; 
}

GrRow* GrItem::GetRow() const
{
    return GetDataRow();
}

bool GrItem::GetVisible() const
{
    return m_pColumn->GetVisible(); 
}

bool GrItem::GetReadOnly() const
{
    if(m_pDataRow->GetDataRowID() == INSERTION_ROW)
        return m_pDataRow->GetReadOnly();
    return (m_pColumn->GetReadOnly() == true || m_pDataRow->GetReadOnly() == true || m_readOnly == true);
}

void GrItem::SetReadOnly(bool b)
{
    m_readOnly = b;
}

void GrItem::SetSelected(bool b)
{
    if(m_selected == b)
        return;
    GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
    if(b == true)
        pItemSelector->SelectItem(this, GrSelectionType_Add);
    else
        pItemSelector->SelectItem(this, GrSelectionType_Remove);
}

bool GrItem::GetSelected() const
{
    if(m_pDataRow->GetFullSelected() == true || m_pColumn->GetFullSelected() == true)
        return true;
    return m_selected;
}

void GrItem::SetFocused(bool b)
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(b == true)
        pFocuser->Set(this);
    else
        pFocuser->Set(IFocusable::Null);
}

const std::wstring& GrItem::GetErrorDescription() const
{
    return m_errorDescription;
}

void GrItem::SetErrorDescription(const std::wstring& errorDescription)
{
    if(m_errorDescription.compare(errorDescription) == 0)
        return;
    m_errorDescription = errorDescription;
}

void GrItem::LockColor(bool b)
{
    if(m_colorLocked == b)
        return;
    m_colorLocked = b;
    Invalidate();
}

bool GrItem::GetFocused() const
{
    const GrFocuser* pFocuser = m_pGridCore->GetFocuser();
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

bool GrItem::ShouldBringIntoView() const
{
    return (m_pColumn->ShouldBringIntoView() || m_pDataRow->ShouldBringIntoView());
}

int GrItem::GetMinHeight() const
{
    int itemHeight = std::max(GetTextBounds().GetHeight(), m_pColumn->GetItemMinHeight());
    itemHeight += GetPadding().GetVertical();
    return itemHeight;
}

bool GrItem::GetDisplayable() const
{
    return (m_pColumn->GetDisplayable() && m_pDataRow->GetDisplayable());
}

void GrItem::BringIntoView()
{
    m_pGridCore->BringIntoView(this);
}

bool GrItem::IsItemSelecting() const
{
    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    if(pItemSelector->IsSelecting(m_pColumn) == false)
        return false;
    if(pItemSelector->IsSelecting(m_pDataRow) == false)
        return false;
    return true;
}

GrFlag GrItem::ToPaintStyle() const
{
    if(m_colorLocked == true)
        return 0;
    GrFlag flag = GrCell::ToPaintStyle();
    if(m_pGridCore->GetSelectionVisible() == false)
        return flag;

    if(IsItemSelecting() == true || GetSelected() == true)
    {
        flag += GrPaintStyle_Selected;
    }

    if(GetFocused() == true)
    {
        flag += GrPaintStyle_Focused;
    }
    return flag;
}

GrFont* GrItem::GetPaintingFont() const
{
    return GetFont();
}

GrColor GrItem::GetPaintingForeColor() const
{
    const GrColor foreColor = GetForeColor();
    const GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle == NULL)
        pStyle = &GrStyle::DefaultStyle;

    GrColor color = foreColor;

    if(m_colorLocked == true)
        return foreColor;

    if(GetFocused() == true)
    {
        color = pStyle->FocusedForeColor;
    }
    else if(GetSelected() == true || IsItemSelecting() == true)
    {
        if(m_pGridCore->GetSelectionVisible() == true)
            color = pStyle->SelectedForeColor;
    }
    return color;
}

GrColor GrItem::GetPaintingBackColor() const
{
    const GrColor backColor = GetBackColor();
    const GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle == NULL)
        pStyle = &GrStyle::DefaultStyle;

    GrColor color = backColor;

    if(m_colorLocked == true)
        return backColor;

    if(GetFocused() == true)
    {
        color = pStyle->FocusedBackColor;
    }
    else if(GetSelected() == true || IsItemSelecting() == true)
    {
        if(m_pGridCore->GetSelectionVisible() == true)
            color = pStyle->SelectedBackColor;
    }
    else if(m_pDataRow->HasFocused() == true && 
        m_pGridCore->GetRowHighlight() == true && 
        m_pGridCore->GetRowHighlightType() != GrRowHighlightType_Line)
    {
        color = pStyle->RowHighlightFillColor;
    }

    if(GetMouseOvered() == true)
    {
        color *= 0.9f;
    }

    return color;
}

GrColor GrItem::GetForeColor() const
{
    GrColor color = GrCell::GetForeColorCore();
    if(color != GrColor::Empty)
        return color;

    color = m_pDataRow->GetItemForeColor();
    if(color != GrColor::Empty)
        return color;

    color = m_pColumn->GetItemForeColor();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL && pStyle->ItemForeColors.size() > 0)
        return pStyle->GetItemForeColor(m_pDataRow->GetVisibleDataRowIndex());

    return GrCell::GetForeColor();
}

GrColor GrItem::GetBackColor() const
{
    GrColor color = GrCell::GetBackColorCore();
    if(color != GrColor::Empty)
        return color;

    color = m_pDataRow->GetItemBackColor();
    if(color != GrColor::Empty)
        return color;

    color = m_pColumn->GetItemBackColor();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL && pStyle->ItemBackColors.size() > 0)
        return pStyle->GetItemBackColor(m_pDataRow->GetVisibleDataRowIndex());

    return GrCell::GetBackColor();
}

GrFont* GrItem::GetFont() const
{
    GrFont* pFont = GrCell::GetFontCore();
    if(pFont != NULL)
        return pFont;

    pFont = m_pDataRow->GetItemFont();
    if(pFont != NULL)
        return pFont;

    pFont = m_pColumn->GetItemFont();
    if(pFont != NULL)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL && pStyle->ItemFonts.size() > 0)
        return pStyle->GetItemFont(m_pDataRow->GetVisibleDataRowIndex());

    return GrCell::GetFont();
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
    if(m_pColumn->GetItemIcon() == true)
        padding.left += (DEF_ICON_SIZE + padding.left);

    if(padding != GrPadding::Default)
        return padding;

    return GrPadding::Default;
}

GrControlState GrItem::GetControlState() const
{
    if(GetMouseOvered() == false)
        return GrControlState_Normal;
    if(m_pGridCore->GetMouseOverState() == GrMouseOverState_Control)
    {
        if(GetMousePressed() == true)
            return GrControlState_Pressed;
        else 
            return GrControlState_Hot;
    }
    return GrControlState_Normal;
}

void GrItem::OnTextSizeChanged()
{
    GrCell::OnTextSizeChanged();

    if(m_pGridCore->GetAutoFitColumn() == true)
    {
        m_pColumn->SetFit();
    }

    if(m_pGridCore->GetAutoFitRow() == true)
    {
        m_pDataRow->SetFit();
    }
}

void GrItem::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();

    if(paintRect.left > clipRect.right || paintRect.right <= clipRect.left)
        return;

    GrFlag paintStyle = ToPaintStyle();

    GrColor backColor = GetPaintingBackColor();
    GrColor foreColor = GetPaintingForeColor();

    if(GetClipped() == true)
        pPainter->DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor, &clipRect);
    else
        pPainter->DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor);

    if(m_pColumn->m_customItemPaint == true)
    {
        return;
    }

    if(m_pColumn->GetItemTextVisible() == true)
    {
        if(GetClipped() == true)
            DrawText(pPainter, foreColor, paintRect, &clipRect);
        else
            DrawText(pPainter, foreColor, paintRect);
    }

    if(GetControlVisible() == true)
    {
        GrRect buttonRect = GetControlRect() + paintRect.GetLocation();
        switch(m_pColumn->GetItemType())
        {
        case GrItemType_DropDown:
            pPainter->DrawDropDown(buttonRect, GetControlState());
            break;
        case GrItemType_Modal:
            pPainter->DrawModal(buttonRect, GetControlState());
            break;
        default:
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
    if(m_pGridCore != NULL && m_pGridCore->GetReadOnly() == true)
        return true;
    return m_readOnly;
}

void GrDataRow::SetReadOnly(bool b)
{
    m_readOnly = b;
}

void GrDataRow::SetSelected(bool b)
{
    if(GetSelected() == b)
        return;
    GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
    if(b == true)
        pItemSelector->SelectDataRow(this, GrSelectionType_Add);
    else
        pItemSelector->SelectDataRow(this, GrSelectionType_Remove);
}

bool GrDataRow::GetSelected() const
{
    return m_selected == true || m_selectedCells.size() > 0;
}

bool GrDataRow::GetSelecting() const
{
    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    return pItemSelector->IsSelecting(this);
}

bool GrDataRow::GetFullSelected() const
{
    //uint visibleColumns = m_pGridCore->GetColumnList()->GetVisibleColumnCount();
    return m_selected;
    //if(visibleColumns == 0)
    //    return false;
    //if(m_selectedCells == visibleColumns)
    //    return true;
    //return false;
}

bool GrDataRow::GetVisible() const
{
    bool visible = GrRow::GetVisible();
    if(visible == false)
        return false;
    GrGroupRow* pGroupRow = dynamic_cast<GrGroupRow*>(GetParent());
    if(pGroupRow == NULL)
        return visible;

    return pGroupRow->IsExpanded();
}

void GrDataRow::SetVisibleDataRowIndex(uint index)
{
    m_visibleDataRowIndex = index;
}

void GrDataRow::SetDataRowIndex(uint index)
{
    wchar_t numberText[10];
    if(m_pDataRowList->GetZeroBasedRowIndex() == true)
        swprintf(numberText, 10, L"%d", index);
    else
        swprintf(numberText, 10, L"%d", index + 1);

    m_dataRowIndex = index;
    SetText(numberText);
}

void GrDataRow::SetDataRowID(uint index)
{
    m_dataRowID = index;
}

uint GrDataRow::GetVisibleDataRowIndex() const
{
    if(GetVisible() == false)
        return INVALID_INDEX;
    return m_visibleDataRowIndex;
}

uint GrDataRow::GetDataRowIndex() const
{
    return m_dataRowIndex;
}

uint GrDataRow::GetDataRowID() const
{
    return m_dataRowID;
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

void GrDataRow::SetItemBackColor(const GrColor& color)
{
    if(m_itemBackColor == color)
        return;
    m_itemBackColor = color;
    InvalidateRow();
}

void GrDataRow::SetItemForeColor(const GrColor& color)
{
    if(m_itemForeColor == color)
        return;
    m_itemForeColor = color;
    InvalidateRow();
}

void GrDataRow::SetItemFont(GrFont* pFont)
{
    if(m_pItemFont == pFont)
        return;
    m_pItemFont = pFont;
    InvalidateRow();
}

IFocusable* GrDataRow::GetFocusable(GrColumn* pColumn) const
{
    return GetItem(pColumn);
}

int GrDataRow::GetMinHeight() const
{
    int height = IDataRow::GetMinHeight();

    for_each(_Items, m_vecItems, value)
    {
        if(value->GetVisible() == false)
            continue;
        height = std::max(height, value->GetMinHeight());
    }
    return height;
}

GrCell* GrDataRow::HitTest(const GrPoint& location) const
{
    if(ContainsVert(location.y) == false)
        return NULL;

    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
    {
        GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
        if(pColumn->ContainsHorz(location.x) == true)
            return GetItem(pColumn);
    }

    GrRect bound = pColumnList->GetColumnSplitter()->GetRect();
    if(bound.Contains(location) == true)
        return pColumnList->GetColumnSplitter();

    if(ContainsHorz(location.x) == false)
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
        if(value->IsGridCoreAttached() == true)
            m_pTextUpdater->AddTextAlign(value);
    }
    m_pGridCore->Invalidate();
}

void GrDataRow::OnHeightChanged()
{
    IDataRow::OnHeightChanged();
    GrRootRow* pHeaderRow = dynamic_cast<GrRootRow*>(GetParent());
    if(pHeaderRow != NULL)
        pHeaderRow->SetHeightChanged();
}

GrColumn::GrColumn()
{
    m_pColumnList = NULL;
    m_visible = true;
    m_readOnly = false;
    m_sortable = true;
    m_groupable = true;
    m_movable = true;
    m_resizable = true;
    m_frozen = false;
    m_selected = false;
    m_fitting = false;
    m_displayable = false;
    m_clipped = false;
    m_grouped = false;

    m_visibleIndex = INVALID_INDEX;
    m_displayIndex = INVALID_INDEX;
    m_index = INVALID_INDEX;
    m_columnID = INVALID_INDEX;

    m_x = 0;
    m_width = 100;
    m_minWidth = 30;
    m_maxWidth = 10000;
    m_sortType = GrSort_None;
    m_fitWidth = 0;

    m_freezablePriority = GetID();
    m_unfreezablePriority = GetID();

    m_itemType = GrItemType_Control;
    m_itemTypeShow = GrItemTypeShow_SelectedOnly;
    m_itemHorzAlign = GrHorzAlign_Left;
    m_itemVertAlign = GrVertAlign_Top;
    m_itemWordWrap = false;
    m_itemMultiline = false;
    m_itemBackColor = GrColor::Empty;
    m_itemForeColor = GrColor::Empty;
    m_itemPadding = GrPadding::Default;
    m_pItemFont = NULL;
    m_itemMinHeight = 0;
    m_itemClickEditing = GrClickEditing_Default;
    m_itemTextVisible = true;
    m_itemIcon = false;

    m_comparer[GrSort_None] = 0;
    m_comparer[GrSort_Up] = 0;
    m_comparer[GrSort_Down] = 0;

    m_customItemPaint = false;

    m_fnColumnBackgroundPaint= NULL;
    m_fnColumnContentsPaint = NULL;
    m_pColumnPaintData = NULL;

    m_pGroup = new GrGroup(this);
}

void GrColumn::OnGridCoreAttached()
{
    GrCell::OnGridCoreAttached();
    m_pColumnList = m_pGridCore->GetColumnList();
    m_pGridCore->AttachObject(m_pGroup);
}

void GrColumn::OnGridCoreDetached()
{
    if(m_pGridCore->GetMouseOver() == this)
        m_pGridCore->SetMouseOver(NULL, GrPoint::Empty);

    if(m_pGridCore->GetMousePress() == this)
        m_pGridCore->SetMouseUnpress();

    GrCell::OnGridCoreDetached();
    m_pGridCore->DetachObject(m_pGroup);
    m_pColumnList = NULL;
}

void GrColumn::OnTextChanged()
{
    GrCell::OnTextChanged();
    if(m_pGridCore == NULL)
        return;

    if(GetDisplayable() == false)
        return;
    m_pGridCore->Invalidate(GetRect());
}

GrColumn::~GrColumn()
{
    m_visibleIndex = INVALID_INDEX;
    m_displayIndex = INVALID_INDEX;
    m_index = INVALID_INDEX;
    m_columnID = INVALID_INDEX;

    delete m_pGroup;
    m_pGroup = NULL;

#ifdef _MANAGED
    System::Object^ managedRef = this->ManagedRef;
    System::ComponentModel::IComponent^ component = safe_cast<System::ComponentModel::IComponent^>(managedRef);
    if(component != nullptr)
    {
        delete this->ManagedRef;
        this->ManagedRef = nullptr;
    }
#endif
}

void GrColumn::SetSelected(bool b)
{
    if(m_pGridCore == NULL)
        return;

    GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
    if(b == true)
        pItemSelector->SelectColumn(this, GrSelectionType_Add);
    else
        pItemSelector->SelectColumn(this, GrSelectionType_Remove);
}

bool GrColumn::GetSelected() const
{
    return m_selected == true || m_selectedCells.size() > 0;
}

bool GrColumn::GetFullSelected() const
{
    if(m_pGridCore == NULL)
        return false;
    return m_selected == true;
}

void GrColumn::AddSelection(GrItem* pItem)
{
    m_selectedCells.insert(pItem);
}
    
void GrColumn::RemoveSelection(GrItem* pItem)
{
    m_selectedCells.erase(pItem);
    if(m_selectedCells.size() == 0)
        m_selected = false;
}

void GrColumn::ClearSelection()
{
    m_selectedCells.clear();
    m_selected = false;
}
    
void GrColumn::SetFullSelected()
{
    m_selected = true;
}

bool GrColumn::HasFocused() const
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->GetFocusedColumn() == this)
        return true;
    return false;
}

int GrColumn::GetFreezablePriority() const
{
    return m_freezablePriority;
}

int GrColumn::GetUnfreezablePriority() const
{
    return m_unfreezablePriority;
}

void GrColumn::SetFreezablePriority(int priority)
{
    if(m_freezablePriority == priority) 
        return;
    m_freezablePriority = priority;
    if(m_pColumnList != NULL)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnFrozenChanged", &e);
    }
}

void GrColumn::SetUnfreezablePriority(int priority)
{
    if(m_unfreezablePriority == priority)
        return;
    m_unfreezablePriority = priority;
    if(m_pColumnList != NULL)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnFrozenChanged", &e);
    }
}

bool GrColumn::GetGrouped() const
{
    return m_grouped;
}

void GrColumn::SetGrouped(bool b)
{
    if(m_grouped == b)
        return;

    m_grouped = b;
    GroupChanged(this, &GrEventArgs::Empty);
}

GrGroup* GrColumn::GetGroup() const
{
    return m_pGroup;
}

void GrColumn::SetDisplayable(bool b)
{
    m_displayable = b;
}

bool GrColumn::GetDisplayable() const
{
    if(GetVisible() == false)
        return false;
    return m_displayable;
}

void GrColumn::SetDisplayIndex(uint index)
{
    m_displayIndex = index;
}

uint GrColumn::GetDisplayIndex() const
{
    return m_displayIndex;
}

void GrColumn::SetVisibleIndex(uint index)
{
    m_visibleIndex = index;
}

uint GrColumn::GetVisibleIndex() const
{
    return m_visibleIndex;
}

uint GrColumn::GetFrozenIndex() const
{
    assert(m_frozen == true);
    return m_visibleIndex;
}

uint GrColumn::GetUnfrozenIndex() const
{
    assert(m_frozen == false);
    return m_visibleIndex - m_pGridCore->GetColumnList()->GetFrozenColumnCount();
}

uint GrColumn::GetIndex() const
{
    return m_index;
}

void GrColumn::SetIndex(uint index)
{
    m_index = index;
}

uint GrColumn::GetColumnID() const
{
    return m_columnID;
}

void GrColumn::SetColumnID(uint id)
{
    m_columnID = id;
}

#ifdef _MANAGED
bool GrColumn::ShouldSerializeWidth()
{
    if(m_width == 100)
        return false;
    return m_width != m_fitWidth;
}

bool GrColumn::ShouldSerializePriorityOnFrozen()
{
    return m_freezablePriority != (int)GetIndex();
}

bool GrColumn::ShouldSerializePriorityOnUnfrozen()
{
    return m_unfreezablePriority != (int)GetIndex();
}
#endif

void GrColumn::SetClipped(bool b)
{
    m_clipped = b;
}

GrRow* GrColumn::GetRow() const
{
    return m_pColumnList; 
}

bool GrColumn::GetClipped() const
{
    return m_clipped;
}

bool GrColumn::ShouldBringIntoView() const
{
    if(m_displayable == false || m_clipped == true)
        return true;
    if(m_frozen == true)
        return false;
    return false;
}

void GrColumn::SetSortType(GrSort sortType)
{
    if(m_sortable == false)
        return;
    if(m_pColumnList == NULL)
        return;
    m_sortType = sortType;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnSortTypeChanged", &e);
    }
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

void GrColumn::SetSortable(bool b)
{
    m_sortable = b;
}

bool GrColumn::GetSortable() const
{
    return m_sortable;
}

void GrColumn::SetGroupable(bool b)
{
    m_groupable = b;
}

bool GrColumn::GetGroupable() const
{
    return m_groupable;
}

void GrColumn::SetWidth(int width) 
{ 
    width = std::max(width, m_minWidth);
    width = std::min(width, m_maxWidth);

    if(m_width == width)
        return;

    m_width = width;

    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnWidthChanged", &e);
    }
}

void GrColumn::SetMinWidth(int minWidth)
{
    m_minWidth = minWidth;
}

void GrColumn::SetMaxWidth(int maxWidth)
{
    m_maxWidth = maxWidth;
}

int GrColumn::GetMinWidth() const
{
    return m_minWidth;
}

int GrColumn::GetMaxWidth() const
{
    return m_maxWidth;
}

bool GrColumn::GetMovable() const
{
    return m_movable;
}

void GrColumn::SetMovable(bool b)
{
    m_movable = b;
}

bool GrColumn::GetResizable() const
{
    return m_resizable;
}

void GrColumn::SetResizable(bool b)
{
    m_resizable = b;
}

bool GrColumn::GetFrozen() const
{
    return m_frozen;
}

void GrColumn::SetFrozen(bool b)
{
    if(m_frozen == b)
        return;
    m_frozen = b;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnFrozenChanged", &e);
    }
}

void GrColumn::AdjustWidth()
{
    if(m_fitting == false)
        return;

    const GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();

    int width = m_minWidth;
    if(m_pGridCore->GetAutoFitColumnType() == GrAutoFitColumnType_ColumnIncluded)
    {
        int columnWidth = GetTextBounds().GetWidth() + GetPadding().GetHorizontal();
        width = std::max(m_minWidth, columnWidth);
    }

    for(uint i=0 ; i<pDataRowList->GetVisibleDataRowCount() ; i++)
    {
        GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
        GrItem* pItem = pDataRow->GetItem(this);
        int itemWidth = pItem->GetTextBounds().GetWidth() + pItem->GetPadding().GetHorizontal();
        width = std::max(width, itemWidth);
    }

    // insertion Row
    {
        GrDataRow* pDataRow = m_pGridCore->GetInsertionRow();
        GrItem* pItem = pDataRow->GetItem(this);
        int itemWidth = pItem->GetTextBounds().GetWidth() + pItem->GetPadding().GetHorizontal();
        width = std::max(width, itemWidth);
    }

    if(GetItemType() != GrItemType_Control)
        width += DEF_CONTROL_WIDTH;

    m_fitWidth = m_width = width;
    m_fitting = false;
}

void GrColumn::SetFit()
{
    if(m_fitting == true)
        return;
    m_fitting = true;
    m_pColumnList->SetFitChanged();
}

int GrColumn::GetX() const 
{ 
    return m_x; 
}

int GrColumn::GetY() const
{ 
    if(m_pColumnList == NULL)
        return 0;
    return m_pColumnList->GetY(); 
}

int GrColumn::GetWidth() const
{
    return m_width;
}

int GrColumn::GetHeight() const
{
    if(m_pColumnList == NULL)
        return 0;
    return m_pColumnList->GetHeight();
}

bool GrColumn::GetVisible() const
{
    return m_visible;
}

bool GrColumn::GetReadOnly() const
{
    if(m_pGridCore != NULL && m_pGridCore->GetReadOnly() == true)
        return true;
    return m_readOnly;
}

GrHorzAlign GrColumn::GetTextHorzAlign() const
{
    return GrHorzAlign_Center;
}

GrVertAlign GrColumn::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

GrColor GrColumn::GetForeColor() const
{
    GrColor color = GrCell::GetForeColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetColumnForeColor();

    return GrCell::GetForeColor();
}

GrColor GrColumn::GetBackColor() const
{
    GrColor color = GrCell::GetBackColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetColumnBackColor();

    return GrCell::GetBackColor();
}

GrColor GrColumn::GetLineColor() const
{
    GrColor color = GrCell::GetLineColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetColumnLineColor();

    return GrCell::GetLineColor();
}

GrFont* GrColumn::GetFont() const
{
    GrFont* pFont = GrCell::GetFontCore();
    if(pFont != GrFont::Empty)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetColumnFont();

    return GrCell::GetFont();
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

const GrColor& GrColumn::GetItemForeColor() const
{
    return m_itemForeColor;
}

const GrColor& GrColumn::GetItemBackColor() const
{
    return m_itemBackColor;
}

const GrPadding& GrColumn::GetItemPadding() const
{
    return m_itemPadding;
}

GrFont* GrColumn::GetItemFont() const
{
    return m_pItemFont;
}

int GrColumn::GetItemMinHeight() const
{
    return m_itemMinHeight;
}

GrClickEditing GrColumn::GetItemClickEditing() const
{
    return m_itemClickEditing;
}

bool GrColumn::GetItemTextVisible() const
{
    return m_itemTextVisible;
}

bool GrColumn::GetItemIcon() const
{
    return m_itemIcon;
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
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnHorzAlignChanged", &e);
    }
}

void GrColumn::SetItemVertAlign(GrVertAlign vertAlign)
{
    if(m_itemVertAlign == vertAlign)
        return;
    m_itemVertAlign = vertAlign;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnVertAlignChanged", &e);
    }
}

void GrColumn::SetItemWordWrap(bool b)
{
    if(m_itemWordWrap == b)
        return;
    m_itemWordWrap = b;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnWordwrapChanged", &e);
    }
}

void GrColumn::SetItemMultiline(bool b)
{
    if(m_itemMultiline == b)
        return;
    m_itemMultiline = b;
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnMultilineChanged", &e);
    }
}

void GrColumn::SetItemForeColor(const GrColor& color)
{
    m_itemForeColor = color;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
}

void GrColumn::SetItemBackColor(const GrColor& color)
{
    m_itemBackColor = color;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
}

void GrColumn::SetItemPadding(const GrPadding& padding)
{
    m_itemPadding = padding;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
    if(m_index != INVALID_INDEX)
    {
        GrColumnEventArgs e(this);
        m_pColumnList->Invoke(L"ColumnPaddingChanged", &e);
    }
}

void GrColumn::SetItemFont(GrFont* pFont)
{
    m_pItemFont = pFont;
    if(m_pGridCore != NULL)
        m_pGridCore->Invalidate();
}

void GrColumn::SetItemMinHeight(int height)
{
    if(m_itemMinHeight == height)
        return;
    m_itemMinHeight = height;

    if(m_pGridCore != NULL && m_pGridCore->GetAutoFitRow() == true)
    {
        GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
        for(uint i=0 ; i<pDataRowList->GetVisibleDataRowCount() ; i++)
        {
            GrDataRow* pDataRow = pDataRowList->GetVisibleDataRow(i);
            pDataRow->SetFit();
        }
        m_pGridCore->GetInsertionRow()->SetFit();
        m_pGridCore->Invalidate();
    }
}

void GrColumn::SetItemClickEditing(GrClickEditing clickEditing)
{
    m_itemClickEditing = clickEditing;
}

void GrColumn::SetItemTextVisible(bool b)
{
    m_itemTextVisible = b;
}

void GrColumn::SetItemIcon(bool b)
{
    m_itemIcon = b;
}

void GrColumn::SetVisible(bool b)
{
    if(m_visible == b)
        return;
    m_visible = b;

    if(m_pGridCore == NULL)
        return;

    if(m_visible == false)
    {
        GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        if(pFocuser->GetFocusedColumn() == this)
            pFocuser->Set(IFocusable::Null);
    }

    m_pColumnList->SetVisibleChanged();
}

void GrColumn::SetReadOnly(bool b)
{
    if(m_readOnly == b)
        return;
    m_readOnly = b;
}

void GrColumn::SetItemType(GrItemType type)
{
    if(m_itemType == type)
        return;
    m_itemType = type;
}

void GrColumn::SetTooltip(const wchar_t* tooltip)
{
    m_tooltip = tooltip;
}

const wchar_t* GrColumn::GetTooltip() const
{
    return m_tooltip.c_str();
}

bool GrColumn::GetSelecting() const
{
    if(m_pGridCore == NULL)
        return false;
    GrItemSelectorInternal* pItemSelector = dynamic_cast<GrItemSelectorInternal*>(m_pGridCore->GetItemSelector());
    return pItemSelector->IsSelecting(this);
}

GrColor GrColumn::GetPaintingForeColor() const
{
    GrColor foreColor = GrCell::GetPaintingForeColor();
    return foreColor;
}

GrColor GrColumn::GetPaintingBackColor() const
{
    if(m_pGridCore == NULL)
        return GrCell::GetPaintingBackColor();

    const GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle == NULL)
        pStyle = &GrStyle::DefaultStyle;
    GrColor backColor;
    if(GetGrouped() == true)
        backColor = pStyle->GetGroupBackColor(GetGroup()->GetGroupLevel());
    else
        backColor = GrCell::GetPaintingBackColor();

    return backColor;
}

GrFlag GrColumn::ToPaintStyle() const
{
    GrFlag flag = GrCell::ToPaintStyle();
    if(GetSelecting() == true || GetSelected() == true)
        flag.Add(GrPaintStyle_Selected);
    if(HasFocused() == true)
        flag.Add(GrPaintStyle_Focused);

    return flag;
}

void GrColumn::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();

    if(m_fnColumnBackgroundPaint == NULL || (*m_fnColumnBackgroundPaint)(pPainter, this, paintRect, m_pColumnPaintData) == false)
    {
        GrFlag paintStyle = ToPaintStyle();
        GrColor backColor = GetPaintingBackColor();
        if(GetClipped() == true)
            pPainter->DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, &clipRect);
        else
            pPainter->DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor);
    }

    if(m_fnColumnContentsPaint == NULL || (*m_fnColumnContentsPaint)(pPainter, this, paintRect, m_pColumnPaintData) == false)
    {
        GrColor foreColor = GetPaintingForeColor();

        GrSort sortType = GetSortType();
        GrRect textRect = paintRect;
        if(sortType != GrSort_None)
        {
            GrRect sortRect;
            sortRect.right = paintRect.right - 6;
            sortRect.left = sortRect.right - 10;
            sortRect.top = (paintRect.bottom + paintRect.top) / 2 - 5;
            sortRect.bottom = sortRect.top + 10;
            pPainter->DrawSortGlyph(sortRect, sortType);

            textRect.right = sortRect.left;
        }

        if(GetTextLineCount() > 0)
        {
            const GrLineDesc& cl = GetTextLine(0);
            if(GetClipped() == true)
                pPainter->DrawColumnText(GetPaintingFont(), GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor, &clipRect);
            else
                pPainter->DrawColumnText(GetPaintingFont(), GetText().c_str() + cl.textBegin, cl.length, textRect, foreColor);
        }
    }
}

void GrRow::SetY(int y)
{
    if(m_y == y)
        return;
    m_y = y;
    OnYChanged();
}

void GrRow::SetHeight(int height)
{
    if(m_height == height)
        return;

    m_height = height;
    OnHeightChanged();
}

void GrRow::SetFit()
{
    m_fitting = true;

    OnFitted();
}

void GrRow::OnGridCoreAttached()
{
    GrCell::OnGridCoreAttached();

    for_each(GrRowArray, m_vecChilds, value)
    {
        m_pGridCore->AttachObject(value);
    }
}

void GrRow::OnTextSizeChanged()
{
    GrCell::OnTextSizeChanged();

    if(m_pGridCore->GetAutoFitRow() == true && GetHeight() != GetTextBounds().GetHeight())
    {
        SetFit();
    }
}

void GrRow::OnHeightChanged()
{
    this->SetTextAlignChanged();
}

void GrRow::OnFitted()
{
    SetTextAlignChanged();
}

int GrRow::GetMinHeight() const
{
    GrFont* pFont = GetPaintingFont();
    int minHeight1 = pFont->GetHeight() + pFont->GetExternalLeading();
    int minHeight2 = GetTextBounds().GetHeight() + GetPadding(true).GetVertical();
    if(minHeight2 < minHeight1)
        return minHeight1;
    return minHeight2;
}

int GrRow::GetHeight() const
{
    return m_height;
}

void GrRow::AdjustHeight()
{
    if(m_fitting == false)
        return;

    int height = GetMinHeight();

    if(m_height != height)
    {
        SetHeight(height);
        OnHeightAdjusted();
    }
    m_fitting = false;
}

GrRow::GrRow() 
{
    m_y = 0;
    m_visible = true;
    m_resizable = true;

    m_height = GrRow::DefaultHeight;
    m_pParent = NULL;
    m_depth= 0;
    m_fitting = false;
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
    default:
        break;
    }
}

class SortDesc
{
public:
    SortDesc(GrGridCore* pGridCore, FuncSortRow fn, void* userData) : 
      m_pGridCore(pGridCore), m_fn(fn), m_userData(userData) {}

      bool operator () (const GrRow* pRow1, const GrRow* pRow2)
      {
          return (*m_fn)(m_pGridCore, pRow1, pRow2, m_userData);
      }

      GrGridCore* m_pGridCore;
      FuncSortRow m_fn;
      void* m_userData;
};

void GrRow::Sort(FuncSortRow fnSort, void* userData)
{
    std::sort(m_vecChilds.begin(), m_vecChilds.end(), SortDesc(m_pGridCore, fnSort, userData));
}

GrCell* GrRow::HitTest(const GrPoint& location) const
{
    if(GetDisplayable() == false)
        return NULL;

    GrRect bound = GetBounds();
    if(bound.Contains(location) == false)
        return NULL;

    return const_cast<GrRow*>(this);
}

//void GrRow::GetVisibleList(GrRowArray* pVisible) const
//{
//    if(GetVisible() == false)
//        return;
//    pVisible->push_back(const_cast<GrRow*>(this));
//    for(uint i=0 ; i<GetChildCount() ; i++)
//    {
//        const GrRow* pChild = GetChild(i);
//        pChild->GetVisibleList(pVisible);
//    }
//}

GrDataRow::GrDataRow()
{
    SetDisplayable(false);

    m_readOnly = false;
    m_selected = false;
    m_pItemFont = NULL;
    m_dataRowIndex = INVALID_INDEX;
    m_dataRowID = INVALID_INDEX;
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

void GrDataRow::AddSelection(GrItem* pItem)
{
    m_selectedCells.insert(pItem);
}
    
void GrDataRow::RemoveSelection(GrItem* pItem)
{
    m_selectedCells.erase(pItem);
    if(m_selectedCells.size() == 0)
        m_selected = false;
}

void GrDataRow::ClearSelection()
{
    m_selectedCells.clear();
    m_selected = false;
}
    
void GrDataRow::SetFullSelected()
{
    m_selected = true;
}

GrDataRow::~GrDataRow()
{
    ClearItem(); 
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
    if(m_visible == b)
        return;

    m_visible = b;

    if(m_pDataRowList == NULL)
        return;

    m_pDataRowList->SetVisibleChanged();
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->GetFocusedRow() == this)
        pFocuser->Set(IFocusable::Null);

    GrRootRow* pHeaderList = dynamic_cast<GrRootRow*>(GetParent());

    if(pHeaderList != NULL)
    {
        pHeaderList->SetVisibleChanged();
    }

    if(m_visible == false && GetSelected() == true)
    {
        GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
        pItemSelector->SelectDataRow(this, GrSelectionType_Remove);
    }
}

int GrRow::GetX() const
{
    return m_pGridCore->GetDisplayRect().left;
}

int GrRow::GetY() const
{
    return m_y;
}

GrFlag GrDataRow::ToPaintStyle() const
{
    GrFlag flag = GrRow::ToPaintStyle();
    if(GetSelecting() == true || GetSelected() == true)
        flag.Add(GrPaintStyle_Selected);
    if(HasFocused() == true)
        flag.Add(GrPaintStyle_Focused);
    return flag;
}

void GrDataRow::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    GrFlag paintStyle = ToPaintStyle();
    GrColor foreColor = GetPaintingForeColor();
    GrColor backColor = GetPaintingBackColor();

    if(m_pGridCore->GetRowVisible() == true)
    {
        if(GetClipped() == true)
            pPainter->DrawRow(paintStyle, paintRect, GetPaintingLineColor(),backColor, &clipRect);
        else
            pPainter->DrawRow(paintStyle, paintRect, GetPaintingLineColor(), backColor);

        if(m_pDataRowList->GetRowNumberVisible() == true)
        {
            if(GetClipped() == true)
                DrawText(pPainter, foreColor, paintRect, &clipRect);
            else
                DrawText(pPainter, foreColor, paintRect);
        }
    }

    if(m_pDataRowList->GetMargin() > 0)
    {
        paintRect.left = paintRect.right;
        paintRect.right = m_pDataRowList->CellStart();
        if(GetRowType() == GrRowType_InsertionRow)
            pPainter->DrawItem(0, paintRect, GetCellLineColor(), GetCellBackColor());
        else if(GetVisibleIndex() == m_pDataRowList->GetVisibleRowCount() - 1)
            pPainter->DrawItem(0, paintRect, GetCellLineColor(), GetCellBackColor());
        else
            pPainter->DrawItem(GrPaintStyle_NoBottomLine, paintRect, GetCellLineColor(), GetCellBackColor(), &clipRect);
    }

    const GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
    {
        const GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
        if(pColumn->GetX() > clipRect.right || pColumn->GetRight() < clipRect.left)
            continue;
        const GrItem* pItem = GetItem(pColumn);
        pItem->Paint(pPainter, clipRect);
    }
}

bool GrRow::GetVisible() const
{
    return m_visible;
}

void GrRow::SetResizable(bool b)
{
    m_resizable = b;
}

bool GrRow::GetResizable() const
{
    return m_resizable;
}

void GrRow::InvalidateRow()
{
    if(m_pGridCore == NULL)
        return;
    GrRect rect = GetRect();
    rect.right = m_pGridCore->GetDisplayRect().right;
    rect.Expand(2, 2, 2, 2);

    m_pGridCore->Invalidate(rect);
}

GrHorzAlign GrRow::GetTextHorzAlign() const
{
    return GrHorzAlign_Center;
}

GrVertAlign GrRow::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

IDataRow::IDataRow() : m_pDataRowList(0)
{
    m_displayable = true;
    m_clipped = false;
    m_visibleIndex = 0;
    m_displayIndex = 0;
    m_selectionGroup = 0;
}

int IDataRow::GetY() const
{
    if(m_pDataRowList == NULL)
        throw _Exception("GridCore에 붙지 않았음");

    if(GetDisplayable() == false)
    {
        uint paintingCount = m_pDataRowList->GetDisplayableRowCount();
        if(paintingCount == 0)
            return GrRow::GetY();

        IDataRow* pFirstPainting = m_pDataRowList->GetDisplayableRow(0);
        int offset = 0;
        if(pFirstPainting != this)
        {
            offset = pFirstPainting->GetY() - GrRow::GetY();
        }
        return m_pDataRowList->GetY() - offset;
    }

    return GrRow::GetY();
}

void IDataRow::SetVisibleIndex(uint index)
{
    m_visibleIndex = index;
}

uint IDataRow::GetVisibleIndex() const
{
    return m_visibleIndex;
}

void IDataRow::SetDisplayIndex(uint index)
{
    m_displayIndex = index;

    m_selectionGroup = index == INSERTION_ROW ? 1 : 0;
}

uint IDataRow::GetDisplayIndex() const
{
    return m_displayIndex;
}

void IDataRow::SetClipped(bool b)
{
    m_clipped = b;
}

bool IDataRow::GetClipped() const
{
    return m_clipped;
}

bool IDataRow::ShouldBringIntoView() const
{
    if(GetVisible() == false)
        throw _Exception("");
    if(m_displayable == false || m_clipped == true)
        return true;
    return false;
}

void IDataRow::SetDisplayable(bool b)
{ 
    m_displayable = b; 
}

bool IDataRow::GetDisplayable() const
{
    if(GetVisible() == false)
        return false;
    return m_displayable; 
}

int IDataRow::GetWidth() const
{
    if(m_pDataRowList == NULL)
        return 0;
    if(m_pGridCore->GetRowVisible() == false)
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
    GrRow::OnHeightChanged();
    if(m_pDataRowList != NULL)
        m_pDataRowList->SetHeightChanged();
}

GrColor IDataRow::GetCellBackColor() const
{
    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->BackColor;
   return m_pGridCore->GetBackColor();
}
 
GrColor IDataRow::GetCellLineColor() const
{
    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->LineColor;
   return m_pGridCore->GetLineColor();
}

bool IDataRow::HasFocused() const
{
    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->GetFocusedRow() == this)
        return true;
    return false;
}

GrColor IDataRow::GetForeColor() const
{
    GrColor color = GrRow::GetForeColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowForeColor();

    return GrCell::GetForeColor();
}

GrColor IDataRow::GetBackColor() const
{
    GrColor color = GrRow::GetBackColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowBackColor();

    return GrRow::GetBackColor();
}

GrColor IDataRow::GetLineColor() const
{
    GrColor color = GrRow::GetLineColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowLineColor();

    return GrRow::GetLineColor();
}

GrFont* IDataRow::GetFont() const
{
    GrFont* pFont = GrRow::GetFontCore();
    if(pFont != NULL)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowFont();

    return GrRow::GetFont();
}

GrRowSplitter::GrRowSplitter()
{
    SetText(L"splitter");
    SetHeight(DefaultSplitterHeight);
    SetResizable(false);

    SetTextVisible(false);
}

bool GrRowSplitter::GetVisible() const
{
    return m_pGridCore->GetInsertionRow()->GetVisible();
}

int GrRowSplitter::GetWidth() const
{
    return m_pGridCore->GetBounds().GetWidth();
}

void GrRowSplitter::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
{
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    
    GrStyle* pStyle = m_pGridCore->GetStyle();
    GrColor backColor = pStyle != NULL ? pStyle->GetRowBackColor() : m_pGridCore->GetBackColor();
    GrColor lineColor = pStyle != NULL ? pStyle->GetRowLineColor() : m_pGridCore->GetLineColor();
    GrRect paintRect = GetRect();
    paintRect.right = paintRect.left + pColumnList->GetWidth();
    pPainter->DrawRowSplitter(GrPaintStyle_Default, paintRect, lineColor, backColor);

    paintRect.left = paintRect.right;
    paintRect.right = GetRight();
    pPainter->DrawRowSplitter(GrPaintStyle_Default, paintRect, GetPaintingLineColor(), GetPaintingBackColor());
}

GrGroupRow::GrGroupRow() 
{
    m_groupLevel = 0;
    m_expanded = true;
    m_pColumn = NULL;

    m_pLabel = new GrGroupHeader(this);
}

GrGroupRow::~GrGroupRow()
{
    delete m_pLabel;
}

GrGroupHeader* GrGroupRow::GetLabel() const
{
    return m_pLabel;
}

void GrGroupRow::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    if(paintRect.top > clipRect.bottom || paintRect.bottom <= clipRect.top)
        return;

    if(m_pGridCore->GetRowVisible() == true)
    {
        GrColor backColor = GetPaintingBackColor();
        GrFlag paintStyle = ToPaintStyle();
        pPainter->DrawRow(paintStyle, paintRect, GetPaintingLineColor(), backColor);
    }

    paintRect.left = paintRect.right;
    paintRect.right = paintRect.left + m_groupLevel * DEF_GROUP_WIDTH;

    pPainter->DrawItem(GrPaintStyle_NoBottomLine | GrPaintStyle_NoRightLine, paintRect, GetCellLineColor(), GetCellBackColor());
    m_pLabel->Paint(pPainter, clipRect);
}

bool GrGroupRow::GetVisible() const
{
    GrGroupRow* pParentGroupRow = dynamic_cast<GrGroupRow*>(GetParent());
    if(pParentGroupRow == NULL)
        return GrRow::GetVisible();

    return pParentGroupRow->IsExpanded();
}

IFocusable* GrGroupRow::GetFocusable(GrColumn* /*pColumn*/) const
{
    return m_pLabel;
}

GrCell* GrGroupRow::HitTest(const GrPoint& location) const
{
    if(ContainsVert(location.y) == false)
        return NULL;

    if(m_pLabel->ContainsHorz(location.x) == true)
        return m_pLabel;

    if(ContainsHorz(location.x) == false)
        return NULL;

    return const_cast<GrGroupRow*>(this);
}

GrCaption::GrCaption()
{
    m_horzAlign = GrHorzAlign_Left;
    m_vertAlign = GrVertAlign_Center;

    m_visible = true;
}

GrHorzAlign GrCaption::GetTextHorzAlign() const
{
    return m_horzAlign;
}

GrVertAlign GrCaption::GetTextVertAlign() const
{
    return m_vertAlign;
}

void GrCaption::SetVisible(bool b)
{
    if(m_visible == b)
        return;

    m_visible = b;

    if(m_pGridCore == NULL)
        return;
    m_pGridCore->GetRootRow()->SetVisibleChanged();
    m_pGridCore->Invalidate();
}

bool GrCaption::GetVisible() const
{
    return m_visible;
}

GrColor GrCaption::GetForeColor() const
{
    GrColor color = GrUpdatableRow::GetForeColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetCaptionForeColor();

    return GrUpdatableRow::GetForeColor();
}

GrColor GrCaption::GetBackColor() const
{
    GrColor color = GrUpdatableRow::GetBackColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetCaptionBackColor();

    return GrUpdatableRow::GetBackColor();
}

GrColor GrCaption::GetLineColor() const
{
    GrColor color = GrUpdatableRow::GetLineColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        pStyle->GetCaptionLineColor();

    return GrUpdatableRow::GetLineColor();
}

GrFont* GrCaption::GetFont() const
{
    GrFont* pFont = GrUpdatableRow::GetFontCore();
    if(pFont != NULL)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetCaptionFont();

    return GrUpdatableRow::GetFont();
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

void GrCaption::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    if(paintRect.top > clipRect.bottom || paintRect.bottom <= clipRect.top)
        return;

    GrFlag paintStyle = ToPaintStyle();
    GrColor foreColor = GetPaintingForeColor();
    GrColor backColor = GetPaintingBackColor();

    pPainter->DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor);

    DrawText(pPainter, foreColor, paintRect, &paintRect);
}

int GrCaption::GetWidth() const
{
    return m_pGridCore->GetBounds().GetWidth();
}

void GrCaption::OnGridCoreAttached()
{
    GrUpdatableRow::OnGridCoreAttached();
    m_pGridCore->FontChanged.Add(this, &GrCaption::gridCore_FontChanged);
}

void GrCaption::OnHeightChanged(GrEventArgs* e)
{
    this->HeightChanged(this, e);
}

void GrCaption::OnHeightChanged()
{
    GrUpdatableRow::OnHeightChanged();
    OnHeightChanged(&GrEventArgs::Empty);
}

void GrCaption::gridCore_FontChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_pTextUpdater->AddTextBounds(this);
}

#ifdef _MANAGED
bool GrCaption::ShouldSerializeHeight()
{
    return GetHeight() != GetMinHeight();
}
#endif

GrGroupPanel::GrGroupPanel()
{
    m_enableGroup = true;
    m_visible = true;
    m_groupChanged = false;
}

GrGroupPanel::~GrGroupPanel()
{

}

uint GrGroupPanel::GetGroupCount() const
{
    if(m_enableGroup == false)
        return 0;
    return m_vecGroups.size();
}

GrGroup* GrGroupPanel::GetGroup(uint level) const
{
    return m_vecGroups[level];
}

void GrGroupPanel::ExpandGroup(uint level, bool /*expand*/)
{
    GrGroup* pGroup = m_vecGroups[level];
    pGroup->SetExpanded(true);
}

void GrGroupPanel::SetGroupSortState(uint level, GrSort sortType)
{
    GrGroup* pGroup = m_vecGroups[level];
    pGroup->SetSortType(sortType);
}

GrCell* GrGroupPanel::HitTest(const GrPoint& location) const
{
    if(ContainsVert(location.y) == false)
        return NULL;

    for_each(_Groups, m_vecGroups, value)
    {
        GrRect rect = value->GetRect();
        if(rect.Contains(location) == true)
            return value;
    }

    return const_cast<GrGroupPanel*>(this);
}

bool GrGroupPanel::GetVisible() const 
{
    if(m_enableGroup == false)
        return false;
    return m_visible; 
}

int GrGroupPanel::GetMinHeight() const
{
    if(m_vecGroups.size() == 0)
    {
        return GrRow::GetMinHeight();
    }
    return m_vecGroups[0]->GetHeight() + 20;
}

void GrGroupPanel::SetVisible(bool b)
{
    m_visible = b;
    GrRootRow* pHeaderList = dynamic_cast<GrRootRow*>(GetParent());
    pHeaderList->SetVisibleChanged();
}

bool GrGroupPanel::GetGroupable() const
{
    return m_enableGroup;
}

void GrGroupPanel::SetGroupable(bool b)
{
    if(m_enableGroup == b)
        return;
    m_enableGroup = b;
    Changed(this, &GrEventArgs::Empty);
}

void GrGroupPanel::OnYChanged()
{
    GrUpdatableRow::OnYChanged();
    RepositionGroup();
}

void GrGroupPanel::RepositionGroup()
{
    GrPoint pt;
    pt.x = GetX();
    pt.y = GetY();

    pt.x += 10;
    pt.y += 10;
    for_each(_Groups, m_vecGroups, value)
    {
        value->SetPosition(pt);
        pt.x += value->GetWidth() + 10;
    }
}

bool GrGroupPanel::ShouldUpdate() const
{
    return m_groupChanged == true;
}

void GrGroupPanel::Update(bool /*force*/)
{
    if(m_groupChanged == true)
        RepositionGroup();

    if(m_vecGroups.size() == 0)
        SetTextVisible(true);
    else
        SetTextVisible(false);
    m_groupChanged = false;
}

void GrGroupPanel::ResetGroupLevel()
{
    for_each(_Groups, m_vecGroups, value)
    {
        value->SetGroupLevelCore(value.GetIndex());
    }
}

void GrGroupPanel::AddGroup(GrGroup* pGroup)
{
    _Groups::iterator itor = find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);

    if(itor != m_vecGroups.end())
        throw _Exception("이미 Group이 되어 있습니다.");

    uint level = pGroup->GetGroupLevel();
    if(level > m_vecGroups.size())
        m_vecGroups.push_back(pGroup);
    else
        m_vecGroups.insert(m_vecGroups.begin() + level, pGroup);
    pGroup->SetText();
    pGroup->LevelChanged.Add(this, &GrGroupPanel::groupInfo_LevelChanged);

    ResetGroupLevel();
    SetFit();
    m_pGridCore->Invalidate();
    m_groupChanged = true;
    Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupPanel::RemoveGroup(GrGroup* pGroup)
{
    _Groups::iterator itor = find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);

    if(itor == m_vecGroups.end())
        throw _Exception("Group이 되어 있지 않은데 해제하려고 합니다.");

    pGroup->LevelChanged.Remove(this, &GrGroupPanel::groupInfo_LevelChanged);
    (*itor)->SetGroupLevelCore(INVALID_INDEX);
    m_vecGroups.erase(itor);

    ResetGroupLevel();
    SetFit();
    m_pGridCore->Invalidate();
    m_groupChanged = true;
    Changed.Raise(this, &GrEventArgs::Empty);
}


GrColor GrGroupPanel::GetForeColor() const
{
    GrColor color = GrUpdatableRow::GetForeColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetGroupPanelForeColor();

    return GrUpdatableRow::GetForeColor();
}

GrColor GrGroupPanel::GetBackColor() const
{
    GrColor color = GrUpdatableRow::GetBackColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetGroupPanelBackColor();

    return GrUpdatableRow::GetBackColor();
}

GrColor GrGroupPanel::GetLineColor() const
{
    GrColor color = GrUpdatableRow::GetLineColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetGroupPanelLineColor();

    return GrUpdatableRow::GetLineColor();
}

GrFont* GrGroupPanel::GetFont() const
{
    GrFont* pFont = GrUpdatableRow::GetFontCore();
    if(pFont != NULL)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetGroupPanelFont();

    return GrUpdatableRow::GetFont();
}

void GrGroupPanel::groupInfo_LevelChanged(GrObject* pSender, GrEventArgs* /*e*/)
{
    GrGroup* pGroup = (GrGroup*)pSender;
    _Groups::iterator itor = std::find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);
    *itor = NULL;

    uint index = std::min(pGroup->GetGroupLevel(), m_vecGroups.size());

    m_vecGroups.insert(m_vecGroups.begin() + index, pGroup);
    m_vecGroups.erase(std::find(m_vecGroups.begin(), m_vecGroups.end(), (GrGroup*)NULL));

    for_each(_Groups, m_vecGroups, value)
    {
        value->SetGroupLevelCore(value.GetIndex());
    }

    Changed.Raise(this, &GrEventArgs::Empty);
}

void GrGroupPanel::NotifyExpanded(GrGroup* pGroup)
{
    GrGroupEventArgs e(pGroup);
    Expanded.Raise(this, &e);
}

void GrGroupPanel::NotifySortChanged(GrGroup* pGroup)
{
    GrGroupEventArgs e(pGroup);
    SortChanged.Raise(this, &e);;
}

void GrGroupPanel::OnGridCoreAttached()
{
    GrRow::OnGridCoreAttached();
    m_pGridCore->Cleared.Add(this, &GrGroupPanel::gridCore_Cleared);
    m_pGridCore->Created.Add(this, &GrGroupPanel::gridCore_Created);
    m_pGridCore->FontChanged.Add(this, &GrGroupPanel::gridCore_FontChanged);
}

void GrGroupPanel::gridCore_Cleared(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_vecGroups.clear();
    SetFit();
}

void GrGroupPanel::gridCore_Created(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    GrColumnList* pColumnList = m_pGridCore->GetColumnList();
    pColumnList->ColumnGroupChanged.Add(this, &GrGroupPanel::columnList_ColumnGroupChanged);
    pColumnList->ColumnInserted.Add(this, &GrGroupPanel::columnList_ColumnInserted);
    pColumnList->ColumnRemoved.Add(this, &GrGroupPanel::columnList_ColumnRemoved);
}

void GrGroupPanel::gridCore_FontChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
{
    m_pTextUpdater->AddTextBounds(this);

    for_each(_Groups, m_vecGroups, value)
    {
        m_pTextUpdater->AddTextBounds(value);
    }
}

void GrGroupPanel::columnList_ColumnInserted(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetGrouped() == false)
        return;
    AddGroup(pColumn->GetGroup());
}

void GrGroupPanel::columnList_ColumnRemoved(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetGrouped() == false)
        return;
    RemoveGroup(pColumn->GetGroup());
}

void GrGroupPanel::columnList_ColumnGroupChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
{
    GrColumn* pColumn = e->GetColumn();
    if(pColumn->GetGrouped() == true)
        AddGroup(pColumn->GetGroup());
    else
        RemoveGroup(pColumn->GetGroup());
}

void GrGroupPanel::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    GrRect paintRect = GetRect();
    if(paintRect.top > clipRect.bottom || paintRect.bottom <= clipRect.top)
        return;

    GrFlag paintStyle = ToPaintStyle();
    GrColor foreColor = GetPaintingForeColor();
    GrColor backColor = GetPaintingBackColor();

    pPainter->DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor);

    for_each(_Groups, m_vecGroups, value)
    {
        value->Paint(pPainter, clipRect);
    }

    DrawText(pPainter, foreColor, paintRect, &clipRect);
}

int GrGroupPanel::GetWidth() const
{
    return m_pGridCore->GetBounds().GetWidth();
}

uint GrRow::GetChildCount() const
{
    return m_vecChilds.size();
}

GrRow* GrRow::GetChild(uint index) const
{
    assert(index < m_vecChilds.size());
    return m_vecChilds[index];
}

void GrRow::AddChild(GrRow* pRow)
{
    m_vecChilds.push_back(pRow);
    pRow->m_pParent = this;
    UpdateDepth(pRow);

    if(m_pGridCore != NULL)
        m_pGridCore->AttachObject(pRow);
}

void GrRow::UpdateDepth(GrRow* pRow)
{
    GrRow* pParent = pRow->GetParent();
    if(pParent == NULL)
        pRow->m_depth = 0;
    else
        pRow->m_depth = pParent->m_depth + 1;

    for_each(GrRowArray, pRow->m_vecChilds, value)
    {
        UpdateDepth(value);
    }
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
        value->m_depth = 0;
    }
    m_vecChilds.clear();
}

GrRow* GrRow::GetParent() const
{
    return m_pParent;
}

uint GrRow::GetDepth() const
{
    return m_depth;
}

void GrGroupRow::Expand(bool b)
{
    if(m_expanded == b)
        return;
    if(m_pDataRowList != NULL)
        m_pDataRowList->SetVisibleChanged();
    m_expanded = b;
}

bool GrGroupRow::IsExpanded() const
{
    return m_expanded;
}

void GrGroupRow::OnHeightChanged()
{
    IDataRow::OnHeightChanged();
    SetTextAlignChanged();
}

void GrGroupRow::OnUpdatePositionCell(int x, GrRect* pBounds)
{
    pBounds->left = x;
    pBounds->right = m_pLabel->GetX() + m_pLabel->GetWidth();
}

void GrGroupRow::OnGridCoreAttached()
{
    IDataRow::OnGridCoreAttached();
    m_pGridCore->AttachObject(m_pLabel);
}

void GrGroupRow::SetReference(GrColumn* pColumn, const std::wstring& itemText)
{
    m_pColumn = pColumn;
    m_itemText = itemText;

    m_key = pColumn->GetText();
    m_key += L" | ";
    m_key += itemText;
}

void GrGroupRow::ProcessAfterGroup()
{
    GrGroupRow* pParent = dynamic_cast<GrGroupRow*>(GetParent());
    if(pParent)
    {
        m_groupLevel = pParent->m_groupLevel + 1;
    }
    else
    {
        m_groupLevel = 0;
    }

    std::wstring text = GetText();


    //wchar_t itemText[30];
    //swprintf(itemText, 30, L" - %d items", GetChildCount());
    //text = m_itemText + itemText;
    //m_pLabel->SetText(text.c_str());
    m_pLabel->SetText(m_itemText.c_str());
}

GrColumn* GrGroupRow::GetColumn() const
{
    return m_pColumn;
}



GrColumnSplitter::GrColumnSplitter(GrColumnList* pColumnList) : m_pColumnList(pColumnList)
{
    SetTextVisible(false);
    SetText(L"ColumnSplitter");

    m_visible = true;
}

int GrColumnSplitter::GetX() const
{
    return m_x; 
}

int GrColumnSplitter::GetY() const
{ 
    return m_pColumnList->GetY(); 
}

int GrColumnSplitter::GetWidth() const
{ 
    return DefaultSplitterWidth;
}

int GrColumnSplitter::GetHeight() const
{
    GrDataRowList* pDataRowList = m_pGridCore->GetDataRowList();
    return pDataRowList->GetDisplayableBottom() - m_pColumnList->GetY();
}

bool GrColumnSplitter::GetVisible() const
{
    return m_visible;
}

void GrColumnSplitter::SetVisible(bool b)
{
    if(m_visible == b)
        return;
    m_visible = b;

    m_pColumnList->SetVisibleChanged();
}

bool GrColumnSplitter::GetDisplayable() const 
{
    return true;
}

void GrColumnSplitter::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const 
{
    pPainter->DrawItem(0, GetRect(), GetPaintingLineColor(), GetPaintingBackColor());
}

GrRow* GrColumnSplitter::GetRow() const
{
    return m_pColumnList; 
}

GrColumnList* GrColumnSplitter::GetColumnList() const
{
    return m_pColumnList; 
}

GrGroupHeader::GrGroupHeader(GrGroupRow* pRow) : m_pRow(pRow)
{

}

int GrGroupHeader::HitMouseOverTest(const GrPoint& localLocation) const
{
    if(GrCell::HitMouseOverTest(localLocation) == 0)
        return 0;

    if(localLocation.x < DEF_GROUP_WIDTH)
        return GrMouseOverState_Control;
    return GrMouseOverState_In;
}

GrFlag GrGroupHeader::ToPaintStyle() const
{
    GrFlag flag = GrCell::ToPaintStyle();

    GrFocuser* pFocuser = m_pGridCore->GetFocuser();
    if(pFocuser->Get() == this)
        flag += GrPaintStyle_Focused;
    return flag;
}

GrColor GrGroupHeader::GetForeColor() const
{
    GrColor color = GrCell::GetForeColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowForeColor();

    return GrCell::GetForeColor();
}

GrColor GrGroupHeader::GetBackColor() const
{
    GrColor color = GrCell::GetBackColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowBackColor();

    return GrCell::GetBackColor();
}

GrColor GrGroupHeader::GetLineColor() const
{
    GrColor color = GrCell::GetLineColorCore();
    if(color != GrColor::Empty)
        return color;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowLineColor();

    return GrCell::GetLineColor();
}

GrFont* GrGroupHeader::GetFont() const
{
    GrFont* pFont = GrCell::GetFontCore();
    if(pFont != NULL)
        return pFont;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        return pStyle->GetRowFont();

    return GrCell::GetFont();
}

GrRow* GrGroupHeader::GetRow() const 
{
    return m_pRow; 
}

IDataRow* GrGroupHeader::GetDataRow() const
{
    return m_pRow; 
}

bool GrGroupHeader::GetDisplayable() const
{
    return m_pRow->GetDisplayable();
}

int GrGroupHeader::GetX() const
{
    return m_pRow->GetRect().right + (m_pRow->GetGroupLevel() * DEF_GROUP_WIDTH);
}

int GrGroupHeader::GetY() const
{
    return m_pRow->GetY();
}

int GrGroupHeader::GetWidth() const
{
    return m_pGridCore->GetBounds().right - GetX();
}

int GrGroupHeader::GetHeight() const
{
    return m_pRow->GetHeight();
}

GrHorzAlign GrGroupHeader::GetTextHorzAlign() const
{
    return GrHorzAlign_Left;
}

GrVertAlign GrGroupHeader::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

void GrGroupHeader::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
{
    GrColumn* pColumn = m_pRow->GetColumn();
    GrRect paintRect = GetRect();
    GrRect displayRect = m_pGridCore->GetDisplayRect();

    paintRect.left += DEF_GROUP_WIDTH;
    paintRect.right = std::min(paintRect.right, displayRect.right);

    GrFlag paintStyle = ToPaintStyle();
    GrColor backColor = pColumn->GetPaintingBackColor();
    GrColor foreColor = pColumn->GetPaintingForeColor();
    GrColor lineColor = pColumn->GetPaintingLineColor();

    pPainter->DrawGroupHeader(paintStyle, paintRect, lineColor, backColor);

    DrawText(pPainter, foreColor, paintRect);

    GrRect glyphRect = paintRect;
    glyphRect.left = GetX();
    glyphRect.right = GetX() + DEF_GROUP_WIDTH;

    GrStyle* pStyle = m_pGridCore->GetStyle();
    if(pStyle != NULL)
        backColor = pStyle->BackColor;
    else 
        backColor = m_pGridCore->GetBackColor();

    pPainter->DrawExpander(glyphRect, (GrControlState)0, m_pRow->IsExpanded(), foreColor, backColor);
}

GrGroup::GrGroup(GrColumn* pColumn) 
    : m_pColumn(pColumn)
{
    m_pGroupPanel = NULL;
    m_grouped = false;
    m_expanded = true;
    m_sortType = GrSort_Up;
    m_level = INVALID_INDEX;
}

void GrGroup::OnGridCoreAttached()
{
    GrCell::OnGridCoreAttached();
    m_pGroupPanel = m_pGridCore->GetGroupPanel();
}

void GrGroup::OnGridCoreDetached()
{
    m_pGroupPanel = NULL;
    GrCell::OnGridCoreDetached();
}

bool GrGroup::GetGrouped() const
{
    return m_pColumn->GetGrouped();
}

void GrGroup::SetGrouped(bool b)
{
    m_pColumn->SetGrouped(b);
}

void GrGroup::SetExpanded(bool b)
{
    if(m_pGroupPanel->GetGroupable() == false)
        return;
    m_expanded = b;
    m_pGroupPanel->NotifyExpanded(this);
}

bool GrGroup::GetExpanded() const
{
    return m_expanded;
}

GrSort GrGroup::GetSortType() const
{
    return m_sortType;
}

void GrGroup::SetSortType(GrSort sortType)
{
    if(m_pGroupPanel->GetGroupable() == false)
        return;
    m_sortType = (sortType == GrSort_Up) ? GrSort_Up : GrSort_Down;
    m_pGroupPanel->NotifySortChanged(this);
}

uint GrGroup::GetGroupLevel() const
{
    return m_level;
}

void GrGroup::SetGroupLevel(uint level)
{
    if(m_level == level)
        return;

    m_level = level;
    if(GetGrouped() == true)
        LevelChanged(this, &GrEventArgs::Empty);
}

void GrGroup::SetGroupLevelCore(uint level)
{
    m_level = level;
}

void GrGroup::SetText()
{
    GrCell::SetText(m_pColumn->GetText());
}

int GrGroup::GetX() const
{
    return m_pt.x;
}

int GrGroup::GetY() const
{
    return m_pt.y;
}

int GrGroup::GetWidth() const
{
     GrFont* pFont = GetPaintingFont();
    return GetTextBounds().GetWidth() + 
        (int)((pFont->GetHeight() + pFont->GetExternalLeading()) * 0.25f) + 
        GetPadding().GetHorizontal() + SortGlyphSize;
}

int GrGroup::GetHeight() const
{
    GrFont* pFont = GetPaintingFont();
    return (int)((pFont->GetHeight() + pFont->GetExternalLeading()) * 1.25f) + GetPadding().GetVertical();
}

GrHorzAlign GrGroup::GetTextHorzAlign() const
{
    return GrHorzAlign_Left;
}

GrVertAlign GrGroup::GetTextVertAlign() const
{
    return GrVertAlign_Center;
}

bool GrGroup::GetDisplayable() const
{
    return m_pGroupPanel->GetDisplayable();
}

void GrGroup::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
{
    GrRect paintRect = GetRect();
    GrFlag paintStyle = ToPaintStyle();

    GrColor backColor = m_pColumn->GetPaintingBackColor();
    GrColor foreColor = m_pColumn->GetPaintingForeColor();
    GrPadding padding = GetPadding();

    pPainter->DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor);

    GrRect sortRect;
    sortRect.right = paintRect.right - padding.right;
    sortRect.left = sortRect.right - SortGlyphSize;
    sortRect.top = (paintRect.bottom + paintRect.top - SortGlyphSize) / 2;
    sortRect.bottom = sortRect.top + SortGlyphSize;

    pPainter->DrawSortGlyph(sortRect, m_sortType);
    DrawText(pPainter, foreColor, paintRect);
}

void GrGroup::SetPosition(GrPoint pt)
{
    m_pt = pt;
}

GrColumn* GrGroup::GetColumn() const
{
    return m_pColumn;
}

GrRootRow::GrRootRow()
{
    m_width = 0;
    m_height = 0;
    SetText(L"Root");
    m_visibleChanged = true;
    m_fitChanged = false;
    m_heightChanged = false;
}

void GrRootRow::Update(bool force)
{
    for_each(_Updatables, m_vecUpdatables, value)
    {
        value->Update(force);
    }

    if(m_visibleChanged == true || force == true)
        BuildVisibleList();

    if(m_fitChanged == true || force == true)
        AdjustRowHeight();

    if(m_heightChanged == true || force == true)
        RepositionVisibleList();

    m_width = m_pColumnList->GetBounds().GetWidth();

    m_heightChanged = false;
    m_visibleChanged = false;
    m_fitChanged = false;
}

bool GrRootRow::ShouldClip() const
{
    return true;
}

void GrRootRow::Clip(const GrRect& displayRect, uint horizontal, uint vertical)
{
    int y = GetY();
    for_each(_Rows, m_vecVisibleRows, value)
    {
        y += value->GetHeight();
    }
    m_height = y - GetY();

    m_bound.SetLocation(displayRect.GetLocation());
    m_bound.right = displayRect.left;
    m_bound.bottom = displayRect.top;

    for_each(_Updatables, m_vecUpdatables, value)
    {
        if(value->ShouldClip(displayRect, horizontal, vertical) == true)
            value->Clip(displayRect, horizontal, vertical);
    }

    for_each(_Updatables, m_vecUpdatables, value)
    {
        m_bound.right = std::max(value->GetBounds().right, m_bound.right);
        m_bound.bottom = std::max(value->GetBounds().bottom, m_bound.bottom);
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
    m_heightChanged = true;
}

void GrRootRow::AdjustRowHeight()
{
    for(uint i=0 ; i<GetChildCount() ; i++)
    {
        GrRow* pChild = GetChild(i);
        pChild->AdjustHeight();
    }
    m_heightChanged = true;
}

void GrRootRow::RepositionVisibleList()
{
    int y = GetY();
    for_each(_Rows, m_vecVisibleRows, value)
    {
        value->SetY(y);
        y += value->GetHeight();
    }
    m_height = y - GetY();
}

void GrRootRow::SetVisibleChanged()
{
    m_visibleChanged = true;
    m_pGridCore->Invalidate();
}

void GrRootRow::SetFitChanged()
{
    m_fitChanged = true;
}

void GrRootRow::SetHeightChanged()
{
    m_heightChanged = true;
}

int GrRootRow::GetX() const
{
    return m_pGridCore->GetDisplayRect().left;
}

int GrRootRow::GetY() const
{
    return m_pGridCore->GetDisplayRect().top;
}

void GrRootRow::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
{
    for_each(_Rows, m_vecVisibleRows, value)
    {
        value->Paint(pPainter, clipRect);
    }
}

GrCell* GrRootRow::HitTest(const GrPoint& location) const
{
    GrCell* pHitted = GrRow::HitTest(location);
    if(pHitted == NULL)
        return NULL;

    for_each(_Rows, m_vecVisibleRows, value)
    {
        GrCell* pSubHitted = value->HitTest(location);
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
    }

    std::sort(m_vecUpdatables.begin(), m_vecUpdatables.end(), SortUpdatable());

    m_pColumnList = m_pGridCore->GetColumnList();
    m_pInsertionRow = m_pGridCore->GetInsertionRow();

    m_pGridCore->GetDataRowList()->DataRowInserted.Add(this, &GrRootRow::dataRowList_DataRowInserted);
}

void GrRootRow::dataRowList_DataRowInserted(GrObject* /*pSender*/, GrDataRowInsertedEventArgs* e)
{
    if(e->GetInsertType() == GrDataRowInsertType_New)
        return;

    GrRowArray childs;

    for(uint i=0 ; i<GetChildCount() ; i++)
    {
        GrRow* pChild = GetChild(i);
        if(pChild == m_pInsertionRow)
        {
            m_pInsertionRow = m_pGridCore->GetInsertionRow();
            childs.push_back(m_pInsertionRow);
        }
        else
        {
            childs.push_back(pChild);
        }
    }

    ClearChild();

    for_each(GrRowArray, childs, value)
    {
        AddChild(value);
    }


    m_visibleChanged = true;
    Update(true);
}

bool GrSortFunc::SortRowsDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData)
{
    return SortRowsUp(pGridCore, pRow2, pRow1, userData);
}

bool GrSortFunc::SortRowsUp(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*userData*/)
{
    int result = pRow1->GetText().compare(pRow2->GetText());
    if(result == 0)
        return pRow1->GetID() < pRow2->GetID();
    return result < 0 ? true : false;
}

bool GrSortFunc::SortRowsNone(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*userData*/)
{
    return pRow1->GetID() < pRow2->GetID();
}

bool GrSortFunc::SortDataRowSortDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* userData)
{
    return SortDataRowSortUp(pGridCore, pRow2, pRow1, userData);
}

bool GrSortFunc::SortDataRowSortNone(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* /*userData*/)
{
    GrDataRow* pDataRow1 = (GrDataRow*)pRow1;
    GrDataRow* pDataRow2 = (GrDataRow*)pRow2;
    return pDataRow1->GetDataRowIndex() < pDataRow2->GetDataRowIndex();
}

bool GrSortFunc::SortDataRowSortUp(GrGridCore* /*pGridCore*/, const GrRow* pRow1, const GrRow* pRow2, void* userData)
{
    if(pRow1->GetRowType() == pRow2->GetRowType())
    {
        if(pRow1->GetRowType() == GrRowType_DataRow)
        {
            GrColumn* pColumn = (GrColumn*)userData;
            const GrItem* pItem1= ((const GrDataRow*)pRow1)->GetItem(pColumn);
            const GrItem* pItem2 = ((const GrDataRow*)pRow2)->GetItem(pColumn);
            return pItem1->GetText().compare(pItem2->GetText()) < 0 ? true : false;
        }
        else if(pRow1->GetRowType() == GrRowType_GroupRow)
        {
            return pRow1->GetText().compare(pRow2->GetText()) < 0 ? true : false;
        }
        else
        {
            return pRow1->GetID() < pRow2->GetID();
        }
    }
    return pRow1->GetRowType() < pRow2->GetRowType();
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

bool GrUpdatableRow::ShouldClip(const GrRect& /*displayRect*/, uint /*horizontal*/, uint /*vertical*/) const
{
    return false; 
}

void GrUpdatableRow::Clip(const GrRect& /*displayRect*/, uint /*horizontal*/, uint /*vertical*/)
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

