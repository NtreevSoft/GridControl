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

        public override int GetX()
        {
            IDataRow pParent = m_pGridRow.GetParent() as IDataRow;
            GrExpander pExpander = pParent.GetExpander();
            return pExpander.GetX();
        }

        public override int GetY()
        {
            return m_pGridRow.GetY();
        }

        public override int GetWidth()
        {
            GrRect displayRect = this.GridCore.GetDisplayRect();
            return displayRect.Right - GetX();
        }

        public override int GetHeight()
        {
            return m_pGridRow.GetHeight();
        }

        public override GrRow GetRow()
        {
            return m_pGridRow;
        }

        public override GrCellType GetCellType()
        {
            return GrCellType.Unknown;
        }

        public override bool GetVisible()
        {
            return m_pGridRow.GetVisible();
        }

        public override bool GetDisplayable()
        {
            return m_pGridRow.GetDisplayable();
        }

        public override GrPadding GetPadding()
        {
            GrFont pFont = GetFont();
            return new GrPadding(pFont.GetHeight());
        }

        public override GrColor GetPaintingBackColor()
        {
            GrColor color = base.GetPaintingBackColor();
            GrFocuser pFocuser = this.GridCore.GetFocuser();
            if (pFocuser.Get() == this)
            {
                color = GrColor.ModifyBrightness(color, 0.95f);
            }
            return color;
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();
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

        public GrRect GetDisplayRect() { return GetRect(); }

        private GridControl GetGridControl()
        {
            return m_pGridRow.GetChildGrid();
        }
    }
}
