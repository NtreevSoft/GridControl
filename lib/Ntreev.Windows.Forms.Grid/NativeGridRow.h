#pragma once
#include "GrGridCell.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    class GrGridCell;

    class GrGridRow : public IDataRow
    {
    public:
        GrGridRow(GridControl^ gridControl, System::ComponentModel::PropertyDescriptor^ propertyDescriptor, Row^ row, System::Object^ value);

        virtual IFocusable* GetFocusable(GrColumn* pColumn) const;
        virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
        virtual int GetMinHeight() const;

        bool SetFocus();

        System::ComponentModel::PropertyDescriptor^ GetPropertyDescriptor();
        Row^ GetParentRow();

        GridControl^ GetChildGrid() 
        {
            return m_gridControl; 
        }

    protected:
        virtual void OnGridCoreAttached();
        virtual void OnGridCoreDetached();
        virtual void OnYChanged();
        virtual void OnHeightChanged();
        virtual void OnDisplayableChanged();
        virtual void OnVisibleChanged();

    private:
        void focuser_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e);
        void dataRowList_VisibleHeightChanged(GrObject* pSender, GrEventArgs* e);
        bool HasGridControl() const;

    private:
        GrGridCell* m_pCell;
    public:
        gcroot<GridControl^> m_parentGrid;
        gcroot<System::ComponentModel::PropertyDescriptor^> m_propertyDescriptor;
        gcroot<Row^> m_parentRow;
        gcroot<GridControl^> m_gridControl;
        gcroot<System::Object^> m_value;
    };

    class GrGridCell : public GrCell, public IFocusable
    {
    public:
        GrGridCell(GrGridRow* pGridRow);

        virtual int GetX() const;
        virtual int GetY() const;
        virtual int GetWidth() const;
        virtual int GetHeight() const;
        virtual GrRow* GetRow() const;

        virtual GrCellType GetCellType() const;
        virtual bool GetVisible() const;
        virtual bool GetDisplayable() const;
        virtual GrPadding GetPadding() const;
        virtual GrColor GetPaintingBackColor() const;
        virtual void Paint(GrGridPainter* pPainter, const GrRect& clipRect) const;
        virtual void Invalidate();

        virtual IDataRow* GetDataRow() const;

    protected:
        virtual GrRect GetDisplayRect() const { return GetRect(); }

    private:
        GridControl^ GetGridControl() const;

    private:
        GrGridRow* m_pGridRow;
    };
} /*namespace Native*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/