using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroupHeader : GrCell, IFocusable
    {
        GrGroupRow m_row;

        public GrGroupHeader(GrGroupRow row)
        {
            m_row = row;
        }

        public override GrCellType GetCellType() { return GrCellType.GroupHeader; }
        public override int GetX()
        {
            GrExpander pExpander = m_row.GetExpander();
            return pExpander.GetRight();
        }

        public override int GetY()
        {
            return m_row.GetY();
        }


        public override int GetWidth()
        {
            return this.GridCore.GetBounds().Right - GetX();
        }

        public override int GetHeight()
        {
            return m_row.GetHeight();
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
            return m_row.GetDisplayable();
        }


        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrColumn column = m_row.GetColumn();
            GrRect paintRect = GetRect();
            GrPaintStyle paintStyle = ToPaintStyle() & ~GrPaintStyle.RightLine;
            GrColor backColor = column.GetPaintingBackColor();
            GrColor foreColor = column.GetPaintingForeColor();
            GrColor lineColor = column.GetPaintingLineColor();

            GrRect displayRect = this.GridCore.GetDisplayRect();
            GrColumnList columnList = this.GridCore.GetColumnList();
            if (this.GridCore.GetFillBlank() == true && columnList.GetDisplayableRight() < displayRect.Right)
            {
                int left = paintRect.Left;
                int top = paintRect.Top;
                int right = displayRect.Right;
                int bottom = paintRect.Bottom;
                paintRect = GrRect.FromLTRB(left, top, right, bottom);
            }

            painter.DrawGroupHeader(paintStyle, paintRect, lineColor, backColor, null);
            DrawText(painter, foreColor, paintRect, null);
        }

        public override void Invalidate()
        {
            base.Invalidate();
        }

        public override GrRow GetRow()
        {
            return m_row;
        }
        public override GrPaintStyle ToPaintStyle()
        {
            GrPaintStyle flag = base.ToPaintStyle();

            GrFocuser focuser = this.GridCore.GetFocuser();
            if (focuser.Get() == this)
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
            return m_row;
        }




    }
}
