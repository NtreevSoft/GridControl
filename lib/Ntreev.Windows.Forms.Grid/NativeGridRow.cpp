#include "stdafx.h"
#include "NativeGridRow.h"
#include "ChildGridControl.h"
#include "NativeClasses.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    GrGridRow::GrGridRow(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
    {
        m_gridControl = gridControl;
        m_pCell = new GrGridCell(this, m_gridControl);
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

        paintRect.left = paintRect.right;
        paintRect.right = m_pCell->GetX();

        pPainter->DrawItem(GrPaintStyle_NoBottomLine | GrPaintStyle_NoRightLine, paintRect, GetCellLineColor(), GetCellBackColor());
    }

    void GrGridRow::SetDisplayable(bool b)
    {
        IDataRow::SetDisplayable(b);
        m_gridControl->Visible = b;
    }

    int GrGridRow::GetMinHeight() const
    {
        return m_gridControl->DataRectangle.Height;
    }

    void GrGridRow::SetDataSource(System::Object^ dataSource)
    {
        m_gridControl->DataSource = dataSource;
        m_gridControl->Update();


        GrGridCore* pGridCore = m_gridControl->GridCore;

        GrRect rect = pGridCore->GetVisibleBounds();
        m_gridControl->Width = 500;
        this->SetHeight(rect.GetHeight() + m_gridControl->Padding.Vertical + System::Windows::Forms::SystemInformation::HorizontalScrollBarHeight);
        //int height = m_gridControl->Height;
        //this->SetHeight(height);
        //int width = m_gridControl->Width;
    }

    void GrGridRow::OnGridCoreAttached()
    {
        IDataRow::OnGridCoreAttached();
        m_pGridCore->AttachObject(m_pCell);
        GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        pFocuser->FocusChanged.Add(this, &GrGridRow::focuser_FocusChanged);
    }

    void GrGridRow::OnYChanged()
    {
        IDataRow::OnYChanged();
        
        m_gridControl->Location = GrPoint(m_pCell->GetX(), this->GetY());
        m_gridControl->Visible = true;
    }

    void GrGridRow::OnHeightChanged()
    {
        IDataRow::OnHeightChanged();
        m_gridControl->Height = this->GetHeight();
    }

    void GrGridRow::focuser_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e)
    {
        GrFocuser* pFocuser = (GrFocuser*)pSender;
        IFocusable* pFocusable = pFocuser->Get();

        if(pFocusable == m_pCell)
        {
            m_gridControl->Focus();
            Cell^ cell = m_gridControl->GetFirstVisibleCell();
            if(cell != nullptr)
                cell->Focus();
        }
    }

    GrGridCell::GrGridCell(GrGridRow* pGridRow, Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : m_pGridRow(pGridRow), m_gridControl(gridControl)
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