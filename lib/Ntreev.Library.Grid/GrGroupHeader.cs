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

        //public override GrCellType GetCellType() { return GrCellType.GroupHeader; }
        //public override int X
        //{
        //    get
        //    {
        //        GrExpander pExpander = m_row.GetExpander();
        //        return pExpander.Right;
        //    }
        //}

        //public override int Y
        //{
        //    get { return m_row.Y; }
        //}

        //public override int Width
        //{
        //    get { return this.GridCore.GetBounds().Right - this.X; }
        //}

        //public override int Height
        //{
        //    get { return m_row.Height; }
        //}

        public override bool IsVisible
        {
            get { return true; }
            set { }
        }

        public override GrHorzAlign TextHorzAlign
        {
            get { return GrHorzAlign.Left; }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return GrVertAlign.Center; }
        }

        public override int HitMouseOverTest(GrPoint localLocation)
        {
            if (base.HitMouseOverTest(localLocation) == 0)
                return 0;

            if (localLocation.X < GrDefineUtility.DEF_GROUP_WIDTH)
                return (int)GrMouseOverState.Control;
            return (int)GrMouseOverState.In;
        }

        public override bool IsDisplayable
        {
            get { return m_row.IsDisplayable; }
        }


        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrColumn column = m_row.GetColumn();
            GrRect paintRect = this.Bounds;
            GrPaintStyle paintStyle = ToPaintStyle() & ~GrPaintStyle.RightLine;
            GrColor backColor = column.GetPaintingBackColor();
            GrColor foreColor = column.GetPaintingForeColor();
            GrColor lineColor = column.GetPaintingLineColor();

            GrRect displayRect = this.GridCore.DisplayRectangle;
            GrColumnList columnList = this.GridCore.ColumnList;
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

            GrFocuser focuser = this.GridCore.Focuser;
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


        public virtual GrRect GetDisplayRect() { return this.Bounds; }
        public virtual IDataRow GetDataRow()
        {
            return m_row;
        }




    }
}
