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

        public GrColumnSplitter(GrColumnList columnList)
        {
            this.m_columnList = columnList;
            this.IsTextVisible = false;
            this.Text = "ColumnSplitter";
            this.Width = DefaultSplitterWidth;
        }


        //public override GrCellType GetCellType() { return GrCellType.Splitter; }

        //public override int X
        //{
        //    get { return m_x; }
        //}

        //public override int Y
        //{
        //    get { return m_columnList.Y; }
        //}

        //public override int Width
        //{
        //    get { return DefaultSplitterWidth; }
        //}

        //public override int Height
        //{
        //    get
        //    {
        //        GrDataRowList dataRowList = this.GridCore.DataRowList;
        //        return dataRowList.GetDisplayableBottom() - m_columnList.Y;
        //    }
        //}

        public override GrHorzAlign TextHorzAlign
        {
            get { return GrHorzAlign.Left; }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return GrVertAlign.Top; }
        }

        public override bool IsDisplayable
        {
            get { return true; }
        }
        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;

            painter.DrawItem(GrPaintStyle.Default, paintRect, this.GetPaintingLineColor(), this.GetPaintingBackColor(), null);
        }

        public override GrRow GetRow()
        {
            return m_columnList;
        }

        

        public GrColumnList GetColumnList()
        {
            return m_columnList;
        }

        protected override void OnVisibleChanged(EventArgs e)
        {
            base.OnVisibleChanged(e);
            m_columnList.SetVisibleChanged();
        }


        //internal void SetX(int x) { m_x = x; }
    }
}
