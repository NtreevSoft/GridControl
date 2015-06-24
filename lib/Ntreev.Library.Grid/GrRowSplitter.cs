using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrRowSplitter : GrRow
    {
        public static readonly int DefaultSplitterHeight = 3;

        public GrRowSplitter()
        {
            SetText("splitter");
            SetHeight(DefaultSplitterHeight);
            SetResizable(false);

            SetTextVisible(false);
        }

        public override GrRowType GetRowType() { return GrRowType.Splitter; }
        public override int GetWidth()
        {
            return this.GridCore.GetBounds().Width;
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrColor backColor = GetPaintingBackColor();
            GrColor lineColor = GetPaintingLineColor();
            GrRect paintRect = GetRect();

            GrRect displayRect = this.GridCore.DisplayRectangle;
            GrColumnList columnList = this.GridCore.ColumnList;
            if (this.GridCore.GetFillBlank() == true && columnList.GetDisplayableRight() < displayRect.Right)
            {
                paintRect.Width = displayRect.Right - paintRect.Left;
            }

            painter.DrawRowSplitter(GrPaintStyle.BottomLine, paintRect, lineColor, backColor);
        }

        public override bool GetVisible()
        {
            return this.GridCore.InsertionRow.GetVisible();
        }
        public override int GetMinHeight() { return DefaultSplitterHeight; }
        public override bool GetResizable() { return false; }
    }
}
