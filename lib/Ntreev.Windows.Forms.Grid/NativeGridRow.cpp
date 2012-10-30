#include "stdafx.h"
#include "NativeGridRow.h"
#include "ChildGridControl.h"
#include "NativeClasses.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    GrGridRow::GrGridRow(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrDataRow* pParent, System::Object^ value)
    {
        m_gridControl = gridControl;        
        m_pCell = new GrGridCell(this, m_gridControl);

        pParent->AddChild(this);

        int borderSize = m_gridControl->Height - m_gridControl->ClientSize.Height;

        m_gridControl->DataSource = value;
        m_gridControl->Update();


        GrGridCore* pGridCore = m_gridControl->GridCore;
        GrRect rect = pGridCore->GetVisibleBounds();

        GrDataRowList* pDataRowList = pGridCore->GetDataRowList();
        pDataRowList->VisibleHeightChanged.Add(this, &GrGridRow::dataRowList_VisibleHeightChanged);

        //this->SetHeight(rect.GetHeight() + m_gridControl->Padding.Vertical + borderSize/* + System::Windows::Forms::SystemInformation::HorizontalScrollBarHeight*/);
        this->SetHeight(GetMinHeight());
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

    //void GrGridRow::SetDataSource(System::Object^ dataSource)
    //{
    //    m_gridControl->DataSource = dataSource;
    //    m_gridControl->Update();

    //    GrGridCore* pGridCore = m_gridControl->GridCore;
    //    GrRect rect = pGridCore->GetVisibleBounds();
    //    m_gridControl->Width = 500;
    //    this->SetHeight(rect.GetHeight() + m_gridControl->Padding.Vertical + System::Windows::Forms::SystemInformation::HorizontalScrollBarHeight);
    //    //int height = m_gridControl->Height;
    //    //this->SetHeight(height);
    //    //int width = m_gridControl->Width;
    //}

    bool GrGridRow::SetFocus()
    {
        if(m_gridControl->FocusedCell == nullptr)
        {
            m_gridControl->ClearSelection();
            m_gridControl->FocusedCell = m_gridControl->GetFirstVisibleCell();
            m_gridControl->Select();
            return true;
        }
        return false;
    }

    //bool GrGridRow::KillFocus()
    //{
    //    if(m_gridControl->FocusedCell != nullptr)
    //    {
    //        m_gridControl->ClearSelection();
    //        return true;
    //    }
    //    return false;
    //}

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
        GrRect clientRect = m_pCell->GetClientRect();
        m_gridControl->Location = clientRect.GetLocation() + m_pCell->GetLocation();
        //m_gridControl->Visible = true;
    }

    void GrGridRow::OnHeightChanged()
    {
        IDataRow::OnHeightChanged();
        GrRect clientRect = m_pCell->GetClientRect();
        m_gridControl->Size = clientRect.GetSize();
        m_gridControl->Visible = this->GetDisplayable();
    }

    void GrGridRow::OnDisplayableChanged()
    {
        IDataRow::OnDisplayableChanged();
        m_gridControl->Visible = this->GetDisplayable();
    }

    void GrGridRow::focuser_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e)
    {
        GrFocuser* pFocuser = (GrFocuser*)pSender;
        IFocusable* pFocusable = pFocuser->Get();

        if(pFocusable == m_pCell)
        {
            //m_gridControl->ClearSelection();
            //if(m_gridControl->Focused == false)
            //{
            //    m_gridControl->FocusedCell = m_gridControl->GetFirstVisibleCell();
            //}
            //m_gridControl->Select();
        }
        else
        {
            m_gridControl->ClearSelection();
            //m_gridControl->FocusedCell = nullptr;
        }
    }

    void GrGridRow::dataRowList_VisibleHeightChanged(GrObject* pSender, GrEventArgs* e)
    {
        m_gridControl->Visible = false;
        this->SetFit();
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

    GrFlag GrGridCell::ToPaintStyle() const
    {
        GrFlag flag = GrCell::ToPaintStyle();

        //GrFocuser* pFocuser = m_pGridCore->GetFocuser();
        //if(pFocuser->Get() == this)
        //    flag += GrPaintStyle_Focused;
        return flag;
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

    void GrGridCell::Paint(GrGridPainter* pPainter, const GrRect& clipRect) const
    {
        GrRect paintRect = GetRect();
        GrFlag paintStyle = ToPaintStyle();
        GrColor backColor = GetPaintingBackColor();
        GrColor foreColor = GetPaintingForeColor();

        pPainter->DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor);
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

    void GrGridCell::OnGridCoreAttached()
    {
        GrCell::OnGridCoreAttached();
        m_pGridCore->DisplayRectChanged.Add(this, &GrGridCell::gridCore_DisplayRectChanged);
    }

    void GrGridCell::OnGridCoreDetached()
    {
        m_pGridCore->DisplayRectChanged.Remove(this, &GrGridCell::gridCore_DisplayRectChanged);
        GrCell::OnGridCoreDetached();
    }

    void GrGridCell::gridCore_DisplayRectChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
    {
        GrRect clientRect = GetClientRect();
        m_gridControl->Width = clientRect.GetWidth();
    }
} /*namespace Native*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/