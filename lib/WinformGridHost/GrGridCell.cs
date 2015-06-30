using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    class GrGridCell : GrCell, IFocusable
    {
        private GrGridRow m_pGridRow;

        public GrGridCell(GrGridRow pGridRow)
        {
            m_pGridRow = pGridRow;
        }

        //public override int X
        //{
        //    get
        //    {
        //        IDataRow pParent = m_pGridRow.GetParent() as IDataRow;
        //        GrExpander pExpander = pParent.GetExpander();
        //        return pExpander.X;
        //    }
        //}

        //public override int Y
        //{
        //    get { return m_pGridRow.Y; }
        //}

        //public override int Width
        //{
        //    get
        //    {
        //        GrRect displayRect = this.GridCore.DisplayRectangle;
        //        return displayRect.Right - this.X;
        //    }
        //}

        //public override int Height
        //{
        //    get { return m_pGridRow.Height; }
        //}

        public override GrRow GetRow()
        {
            return m_pGridRow;
        }

        //public override GrCellType GetCellType()
        //{
        //    return GrCellType.Unknown;
        //}

        public override bool IsVisible
        {
            get { return m_pGridRow.IsVisible; }
        }

        public override bool IsDisplayable
        {
            get { return m_pGridRow.IsDisplayable; }
        }

        public override GrPadding GetPadding()
        {
            GrFont pFont = GetFont();
            return new GrPadding(pFont.GetHeight());
        }

        public override GrColor GetPaintingBackColor()
        {
            GrColor color = base.GetPaintingBackColor();
            GrFocuser pFocuser = this.GridCore.Focuser;
            if (pFocuser.Get() == this)
            {
                color = GrColor.ModifyBrightness(color, 0.95f);
            }
            return color;
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;
            GrPaintStyle paintStyle = ToPaintStyle();
            GrColor backColor = GetPaintingBackColor();
            GrColor foreColor = GetPaintingForeColor();

            pPainter.DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);
        }

        public override void Invalidate()
        {
            base.Invalidate();
        }

        public IDataRow GetDataRow()
        {
            return m_pGridRow;
        }

        public GrRect GetDisplayRect() { return this.Bounds; }

        private GridControl GetGridControl()
        {
            return m_pGridRow.GetChildGrid();
        }
    }
}
