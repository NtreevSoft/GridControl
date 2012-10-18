#pragma once
#include "GrGridCell.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    class GrGridCell;

    ref class ChildGridProcesser
    {
    public:
        ChildGridProcesser(GridControl^ gridControl)
        {
            m_gridControl->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ChildGridProcesser::gridControl_KeyDown);
        }

    private:
        void gridControl_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
        {
            int wqer = 0;
        }

    private:
        GridControl^ m_gridControl;
    };

    class GrGridRow : public IDataRow
    {
    public:
        GrGridRow(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        virtual IFocusable* GetFocusable(GrColumn* pColumn) const;

        virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
        virtual void SetDisplayable(bool b);
        virtual int GetMinHeight() const;

        void SetDataSource(System::Object^ dataSource);

    protected:
        virtual void OnGridCoreAttached();
        virtual void OnYChanged();
        virtual void OnHeightChanged();

    private:
        void focuser_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e);

    private:
        GrGridCell* m_pCell;
    public:
        gcroot<Ntreev::Windows::Forms::Grid::GridControl^> m_gridControl;
    };

    class GrGridCell : public GrCell, public IFocusable
    {
    public:
        GrGridCell(GrGridRow* pGridRow, Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        virtual int GetX() const;
        virtual int GetY() const;
        virtual int GetWidth() const;
        virtual int GetHeight() const;
        virtual GrRow* GetRow() const;

        virtual GrCellType GetCellType() const;
        virtual bool GetVisible() const;
        virtual bool GetDisplayable() const;
        virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;

        virtual IDataRow* GetDataRow() const;

    private:
        virtual GrRect GetDisplayRect() const { return GetRect(); }
    private:
        GrGridRow* m_pGridRow;
        gcroot<Ntreev::Windows::Forms::Grid::GridControl^> m_gridControl;
    };

    
} /*namespace Native*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/