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
            return this.GridCore.GetBounds().GetWidth();
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrColor backColor = GetPaintingBackColor();
            GrColor lineColor = GetPaintingLineColor();
            GrRect paintRect = GetRect();

            GrRect displayRect = this.GridCore.GetDisplayRect();
            GrColumnList pColumnList = this.GridCore.GetColumnList();
            if (this.GridCore.GetFillBlank() == true && pColumnList.GetDisplayableRight() < displayRect.Right)
            {
                paintRect.Width = displayRect.Right - paintRect.Left;
            }

            pPainter.DrawRowSplitter(GrPaintStyle.BottomLine, paintRect, lineColor, backColor);
        }

        public override bool GetVisible()
        {
            return this.GridCore.GetInsertionRow().GetVisible();
        }
        public override int GetMinHeight() { return DefaultSplitterHeight; }
        public override bool GetResizable() { return false; }
    }
}
