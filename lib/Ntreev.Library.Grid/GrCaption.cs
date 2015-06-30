using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrCaption : GrUpdatableRow
    {
        private GrHorzAlign horzAlign;
        private GrVertAlign vertAlign;

        //private bool visible1;

        public GrCaption()
        {
            this.horzAlign = GrHorzAlign.Left;
            this.vertAlign = GrVertAlign.Center;

            //this.visible1 = true;
        }

        public override GrHorzAlign TextHorzAlign
        {
            get { return this.horzAlign; }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return this.vertAlign; }
        }

        public void SetTextHorzAlign(GrHorzAlign align)
        {
            if (this.horzAlign == align)
                return;
            this.horzAlign = align;
            this.SetTextAlignChanged();
        }

        public void SetTextVertAlign(GrVertAlign align)
        {
            if (this.vertAlign == align)
                return;
            this.vertAlign = align;
            this.SetTextAlignChanged();
        }

        public override GrRowType GetRowType() { return GrRowType.Caption; }

        //public override int Width
        //{
        //    get { return this.GridCore.GetBounds().Width; }
        //}

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;
            GrRect displayRect = this.GridCore.DisplayRectangle;
            GrColumnList columnList = this.GridCore.ColumnList;
            if (this.GridCore.GetFillBlank() == true && columnList.GetDisplayableRight() < displayRect.Right)
            {
                paintRect.Width = displayRect.Right - paintRect.Left;
            }

            if (paintRect.Top >= clipRect.Bottom || paintRect.Bottom < clipRect.Top)
                return;

            GrPaintStyle paintStyle = ToPaintStyle() & ~GrPaintStyle.RightLine;
            GrColor foreColor = this.GetPaintingForeColor();
            GrColor backColor = this.GetPaintingBackColor();

            painter.DrawItem(paintStyle, paintRect, this.GetPaintingLineColor(), backColor, null);

            this.DrawText(painter, foreColor, paintRect, paintRect);
        }

        //public override bool IsVisible
        //{
        //    get { return this.visible1; }
        //}

        //public void SetVisible(bool b)
        //{
        //    if (this.visible1 == b)
        //        return;

        //    this.visible1 = b;

        //    if (this.GridCore == null)
        //        return;
        //    this.GridCore.RootRow.SetVisibleChanged();
        //    this.GridCore.Invalidate();
        //}

        protected override void OnVisibleChanged(EventArgs e)
        {
            if (this.GridCore != null)
            {
                this.GridCore.RootRow.SetVisibleChanged();
                this.GridCore.Invalidate();
            }
            base.OnVisibleChanged(e);
        }

        public override GrColor GetForeColor()
        {
            GrColor color = base.GetForeColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetCaptionForeColor();

            return base.GetForeColor();
        }

        public override GrColor GetBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetCaptionBackColor();

            return base.GetBackColor();
        }

        public override GrColor GetLineColor()
        {
            GrColor color = base.GetLineColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                pStyle.GetCaptionLineColor();

            return base.GetLineColor();
        }

        public override GrFont GetFont()
        {
            GrFont pFont = base.GetFontCore();
            if (pFont != null)
                return pFont;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetCaptionFont();

            return base.GetFont();
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.FontChanged += gridCore_FontChanged;
            this.GridCore.DisplayRectangleChanged += GridCore_DisplayRectangleChanged;
        }

        void GridCore_DisplayRectangleChanged(object sender, EventArgs e)
        {
            this.Width = this.GridCore.DisplayRectangle.Width;
        }

        private void gridCore_FontChanged(object sender, EventArgs e)
        {
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            pTextUpdater.AddTextBounds(this);
        }
    }
}
