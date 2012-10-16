#pragma once
#include "GrGridCell.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    class GrGridCell;

    class GrGridRow : public IDataRow
    {
    public:
        GrGridRow(Ntreev::Windows::Forms::Grid::GridControl^ parent);

        virtual IFocusable* GetFocusable(GrColumn* pColumn) const;

        virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
        virtual void SetDisplayable(bool b);

        void SetDataSource(System::Object^ dataSource);

    protected:
        virtual void OnGridCoreAttached();
        virtual void OnYChanged();

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