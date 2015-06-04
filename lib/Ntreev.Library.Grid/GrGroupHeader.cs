using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroupHeader : GrCell, IFocusable
    {
        GrGroupRow m_pRow;

        public GrGroupHeader(GrGroupRow pRow)
        {
            m_pRow = pRow;
        }

        public override GrCellType GetCellType() { return GrCellType.GroupHeader; }
        public override int GetX()
        {
            GrExpander pExpander = m_pRow.GetExpander();
            return pExpander.GetRight();
        }

        public override int GetY()
        {
            return m_pRow.GetY();
        }


        public override int GetWidth()
        {
            return this.GridCore.GetBounds().Right - GetX();
        }

        public override int GetHeight()
        {
            return m_pRow.GetHeight();
        }

        public override bool GetVisible() { return true; }

        public override GrHorzAlign GetTextHorzAlign()
        {
            return GrHorzAlign.Left;
        }

        public override GrVertAlign GetTextVertAlign()
        {
            return GrVertAlign.Center;
        }

        public override int HitMouseOverTest(GrPoint localLocation)
        {
            if (base.HitMouseOverTest(localLocation) == 0)
                return 0;

            if (localLocation.X < GrDefineUtility.DEF_GROUP_WIDTH)
                return (int)GrMouseOverState.Control;
            return (int)GrMouseOverState.In;
        }

        public override bool GetDisplayable()
        {
            return m_pRow.GetDisplayable();
        }


        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrColumn pColumn = m_pRow.GetColumn();
            GrRect paintRect = GetRect();
            GrPaintStyle paintStyle = ToPaintStyle() & ~GrPaintStyle.RightLine;
            GrColor backColor = pColumn.GetPaintingBackColor();
            GrColor foreColor = pColumn.GetPaintingForeColor();
            GrColor lineColor = pColumn.GetPaintingLineColor();

            GrRect displayRect = this.GridCore.GetDisplayRect();
            GrColumnList pColumnList = this.GridCore.GetColumnList();
            if (this.GridCore.GetFillBlank() == true && pColumnList.GetDisplayableRight() < displayRect.Right)
            {
                int left = paintRect.Left;
                int top = paintRect.Top;
                int right = displayRect.Right;
                int bottom = paintRect.Bottom;
                paintRect = GrRect.FromLTRB(left, top, right, bottom);
            }

            pPainter.DrawGroupHeader(paintStyle, paintRect, lineColor, backColor, null);
            DrawText(pPainter, foreColor, paintRect, null);
        }

        public override void Invalidate()
        {
            base.Invalidate();
        }

        public override GrRow GetRow()
        {
            return m_pRow;
        }
        public override GrPaintStyle ToPaintStyle()
        {
            GrPaintStyle flag = base.ToPaintStyle();

            GrFocuser pFocuser = this.GridCore.GetFocuser();
            if (pFocuser.Get() == this)
                flag |= GrPaintStyle.Focused;
            return flag;
        }


        public override GrColor GetForeColor()
        {
            GrColor color = base.GetForeColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowForeColor();

            return base.GetForeColor();
        }
        public override GrColor GetBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowBackColor();

            return base.GetBackColor();
        }
        public override GrColor GetLineColor()
        {
            GrColor color = base.GetLineColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowLineColor();

            return base.GetLineColor();
        }
        public override GrFont GetFont()
        {
            GrFont pFont = base.GetFontCore();
            if (pFont != null)
                return pFont;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowFont();

            return base.GetFont();
        }


        public virtual GrRect GetDisplayRect() { return GetRect(); }
        public virtual IDataRow GetDataRow()
        {
            return m_pRow;
        }




    }
}
