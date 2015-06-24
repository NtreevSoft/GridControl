using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumnSplitter : GrCell
    {
        public static readonly int DefaultSplitterWidth = 5;

        private readonly GrColumnList m_columnList;
        int m_x;
        bool m_visible;

        public GrColumnSplitter(GrColumnList columnList)
        {
            this.m_columnList = columnList;
            SetTextVisible(false);
            SetText("ColumnSplitter");

            m_visible = true;
        }


        public override GrCellType GetCellType() { return GrCellType.Splitter; }
        public override int GetX()
        {
            return m_x;
        }
        public override int GetY()
        {
            return m_columnList.GetY(); 
        }
        public override int GetWidth()
        {
            return DefaultSplitterWidth;
        }
        public override int GetHeight()
        {
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            return dataRowList.GetDisplayableBottom() - m_columnList.GetY();
        }
        public override bool GetVisible()
        {
            return m_visible;
        }

        public override GrHorzAlign GetTextHorzAlign() { return GrHorzAlign.Left; }
        public override GrVertAlign GetTextVertAlign() { return GrVertAlign.Top; }

        public override bool GetDisplayable()
        {
            return true;
        }
        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.GetRect();

            painter.DrawItem(GrPaintStyle.Default, paintRect, this.GetPaintingLineColor(), this.GetPaintingBackColor(), null);
        }

        public override GrRow GetRow()
        {
            return m_columnList;
        }

        public void SetVisible(bool b)
        {
            if (m_visible == b)
                return;
            m_visible = b;

            m_columnList.SetVisibleChanged();
        }
        public GrColumnList GetColumnList()
        {
            return m_columnList;
        }


        internal void SetX(int x) { m_x = x; }
    }
}
