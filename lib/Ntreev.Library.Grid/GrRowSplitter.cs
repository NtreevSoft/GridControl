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
            this.Text = "splitter";
            this.Height = DefaultSplitterHeight;
            SetResizable(false);

            this.IsTextVisible = false;
        }

        public override GrRowType GetRowType() { return GrRowType.Splitter; }

        //public override int Width
        //{
        //    get { return this.GridCore.GetBounds().Width; }
        //}

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrColor backColor = GetPaintingBackColor();
            GrColor lineColor = GetPaintingLineColor();
            GrRect paintRect = this.Bounds;

            GrRect displayRect = this.GridCore.DisplayRectangle;
            GrColumnList columnList = this.GridCore.ColumnList;
            if (this.GridCore.GetFillBlank() == true && columnList.GetDisplayableRight() < displayRect.Right)
            {
                paintRect.Width = displayRect.Right - paintRect.Left;
            }

            painter.DrawRowSplitter(GrPaintStyle.BottomLine, paintRect, lineColor, backColor);
        }

        public override bool IsVisible
        {
            get { return this.GridCore.InsertionRow.IsVisible; }
            set { }
        }

        public override int GetMinHeight() { return DefaultSplitterHeight; }

        public override bool GetResizable() { return false; }
    }
}
