#include "stdafx.h"
#include "NativeGridRow.h"
#include "NativeClasses.h"
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    GrGridRow::GrGridRow(GridControl^ gridControl, System::ComponentModel::PropertyDescriptor^ propertyDescriptor, Row^ parentRow, System::Object^ value)
    {
        m_parentGrid = gridControl;
        m_value = value;

        m_propertyDescriptor = propertyDescriptor;
        m_parentRow = parentRow;

        m_pCell = new GrGridCell(this);
    }

    IFocusable* GrGridRow::GetFocusable(GrColumn* /*pColumn*/) const 
    {
        return m_pCell;
    }

    void GrGridRow::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
    {
        GrRect paintRect = GetRect();
        GrFlag paintStyle = ToPaintStyle();
        GrColor foreColor = GetPaintingForeColor();
        GrColor backColor = GetPaintingBackColor();

        if(m_pGridCore->GetRowVisible() == true)
        {
            if(GetClipped() == true)
                pPainter->DrawRow(paintStyle, paintRect, GetPaintingLineColor(), backColor, &clipRect);
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

        paintRect.left = paintRect.right;
        paintRect.right = m_pCell->GetX();

        pPainter->DrawItem(GrPaintStyle_BottomLine, paintRect, GetCellLineColor(), GetCellBackColor());

        m_pCell->Paint(pPainter, clipRect);
    }

    int GrGridRow::GetMinHeight() const
    {
        int borderSize = m_gridControl->Height - m_gridControl->ClientSize.Height;
        GrGridCore* pGridCore = m_gridControl->GridCore;
        GrRect rect = pGridCore->GetVisibleBounds();

        return rect.GetHeight() + m_gridControl->Padding.Vertical + borderSize + m_pCell->GetPadding().GetVertical();
    }

    bool GrGridRow::SetFocus()
    {
        if(this->HasGridControl() == true && m_gridControl->FocusedCell == nullptr)
        {
            m_gridControl->ClearSelection();
            m_gridControl->FocusedCell = m_gridControl->GetFirstVisibleCell();
            m_gridControl->Select();
            return true;
        }
        return false;
    }

    System::ComponentModel::PropertyDescriptor^ GrGridRow::GetPropertyDescriptor()
    {
        return m_propertyDescriptor;
    }
     
    Row^ GrGridRow::GetParentRow()
    {
        return m_parentRow;
    }

    void GrGridRow::OnGridCoreAttached()
    {
        IDataRow::OnGridCoreAttached();
        m_pGridCore->AttachObject(m_pCell);
        GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        pFocuser->FocusChanged.Add(this, &GrGridRow::focuser_FocusChanged);
    }

    void GrGridRow::OnGridCoreDetached()
    {
        GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        pFocuser->FocusChanged.Remove(this, &GrGridRow::focuser_FocusChanged);
        m_pGridCore->DetachObject(m_pCell);
        IDataRow::OnGridCoreDetached();
    }

    void GrGridRow::OnYChanged()
    {
        IDataRow::OnYChanged();
        if(this->HasGridControl() == true)
        {
            GrRect clientRect = m_pCell->GetClientRect();
            m_gridControl->Location = clientRect.GetLocation() + m_pCell->GetLocation();
        }
        //m_gridControl->Visible = true;
    }

    void GrGridRow::OnHeightChanged()
    {
        IDataRow::OnHeightChanged();
        if(this->HasGridControl() == true)
        {
            GrRect clientRect = m_pCell->GetClientRect();
            m_gridControl->Size = clientRect.GetSize();
            m_gridControl->Visible = this->GetDisplayable();
        }
    }

    void GrGridRow::OnDisplayableChanged()
    {
        IDataRow::OnDisplayableChanged();
        if(this->HasGridControl() == true)
        {
            m_gridControl->Visible = this->GetDisplayable();
        }
    }

    void GrGridRow::OnVisibleChanged()
    {
        IDataRow::OnVisibleChanged();
        bool visibled = this->GetVisible();

        if(this->HasGridControl() == false)
        {
            m_gridControl = m_parentGrid->InvokeNewChildGridControl(this);

            m_gridControl->DataSource = m_value;
            m_gridControl->Refresh();

            GrGridCore* pGridCore = m_gridControl->GridCore;
            GrRect rect = pGridCore->GetVisibleBounds();

            GrDataRowList* pDataRowList = pGridCore->GetDataRowList();
            pDataRowList->VisibleHeightChanged.Add(this, &GrGridRow::dataRowList_VisibleHeightChanged);

            this->SetHeight(GetMinHeight());
        }
    }

    void GrGridRow::focuser_FocusChanged(GrObject* pSender, GrFocusChangeArgs* /*e*/)
    {
        GrFocuser* pFocuser = (GrFocuser*)pSender;
        IFocusable* pFocusable = pFocuser->Get();

        if(pFocusable != m_pCell && this->HasGridControl() == true)
        {
            m_gridControl->ClearSelection();
        }
    }

    void GrGridRow::dataRowList_VisibleHeightChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
    {
        this->SetFit();
    }

    bool GrGridRow::HasGridControl() const
    {
        return (GridControl^)m_gridControl != nullptr;
    }

    GrGridCell::GrGridCell(GrGridRow* pGridRow)
        : m_pGridRow(pGridRow)
    {

    }

    int GrGridCell::GetX() const
    {
        IDataRow* pParent = (IDataRow*)m_pGridRow->GetParent();
        GrExpander* pExpander = pParent->GetExpander();
        return pExpander->GetX();
    }

    int GrGridCell::GetY() const
    {
        return m_pGridRow->GetY();
    }

    int GrGridCell::GetWidth() const
    {
        GrRect displayRect = m_pGridCore->GetDisplayRect();
        return displayRect.right - GetX();
    }

    int GrGridCell::GetHeight() const
    {
        return m_pGridRow->GetHeight();
    }

    GrRow* GrGridCell::GetRow() const
    {
        return m_pGridRow;
    }

    GrCellType GrGridCell::GetCellType() const
    {
        return GrCellType_Unknown;
    }

    bool GrGridCell::GetVisible() const
    {
        return m_pGridRow->GetVisible();
    }

    bool GrGridCell::GetDisplayable() const
    {
        return m_pGridRow->GetDisplayable();
    }

    GrColor GrGridCell::GetPaintingBackColor() const
    {
        GrColor color = GrCell::GetPaintingBackColor();
        GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        if(pFocuser->Get() == this)
        {
            color = RGBHSL::ModifyBrightness(color, 0.95f);
        }
        return color;
    }

    void GrGridCell::Paint(GrGridPainter* pPainter, const GrRect& /*clipRect*/) const
    {
        GrRect paintRect = GetRect();
        GrFlag paintStyle = ToPaintStyle();
        GrColor backColor = GetPaintingBackColor();
        GrColor foreColor = GetPaintingForeColor();

        pPainter->DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor);
    }

    void GrGridCell::Invalidate()
    {
        GrCell::Invalidate();
    }

    IDataRow* GrGridCell::GetDataRow() const
    {
        return m_pGridRow;
    }

    GrPadding GrGridCell::GetPadding() const
    {
        GrFont* pFont = GetFont();
        return GrPadding(pFont->GetHeight());
    }

    GridControl^ GrGridCell::GetGridControl() const
    {
        return m_pGridRow->GetChildGrid();
    }
} /*namespace Native*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/