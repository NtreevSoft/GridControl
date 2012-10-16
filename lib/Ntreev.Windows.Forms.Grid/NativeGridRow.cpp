#include "stdafx.h"
#include "NativeGridRow.h"
#include "ChildGridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    GrGridRow::GrGridRow(Ntreev::Windows::Forms::Grid::GridControl^ parent)
    {
        m_gridControl = gcnew ChildGridControl(this);
        parent->Controls->Add(m_gridControl);
        m_pCell = new GrGridCell(this, m_gridControl);
        this->SetHeight(m_gridControl->Height);
    }

    IFocusable* GrGridRow::GetFocusable(GrColumn* pColumn) const 
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

        m_pCell->Paint(pPainter, clipRect);
    }

    void GrGridRow::SetDisplayable(bool b)
    {
        IDataRow::SetDisplayable(b);
        m_gridControl->Visible = b;
    }

    void GrGridRow::SetDataSource(System::Object^ dataSource)
    {
        m_gridControl->DataSource = dataSource;
        int height = m_gridControl->Height;
        this->SetHeight(height);
        int width = m_gridControl->Width;
    }

    void GrGridRow::OnGridCoreAttached()
    {
        IDataRow::OnGridCoreAttached();
        m_pGridCore->AttachObject(m_pCell);
    }

    void GrGridRow::OnYChanged()
    {
        IDataRow::OnYChanged();
        m_gridControl->Location = GrPoint(this->GetRight(), this->GetY());
        m_gridControl->Visible = true;
    }

    GrGridCell::GrGridCell(GrGridRow* pGridRow, Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : m_pGridRow(pGridRow), m_gridControl(gridControl)
    {

    }

    int GrGridCell::GetX() const
    {
        return m_pGridRow->GetRight();
    }

    int GrGridCell::GetY() const
    {
        return m_pGridRow->GetY();
    }

    int GrGridCell::GetWidth() const
    {
        return m_pGridCore->GetBounds().right - GetX();
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

    void GrGridCell::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
    {
        //m_pChildGrid->Paint(pPainter, clipRect);
    }

    IDataRow* GrGridCell::GetDataRow() const
    {
        return m_pGridRow;
    }
} /*namespace Native*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/