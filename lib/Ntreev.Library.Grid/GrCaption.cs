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

        private bool visible1;

        public GrCaption()
        {
            this.horzAlign = GrHorzAlign.Left;
            this.vertAlign = GrVertAlign.Center;

            this.visible1 = true;
        }

        public override GrHorzAlign GetTextHorzAlign()
        {
            return this.horzAlign;
        }

        public override GrVertAlign GetTextVertAlign()
        {
            return this.vertAlign;
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

        public override int GetWidth()
        {
            return this.GridCore.GetBounds().Width;
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.GetRect();
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

        public override bool GetVisible()
        {
            return this.visible1;
        }

        public void SetVisible(bool b)
        {
            if (this.visible1 == b)
                return;

            this.visible1 = b;

            if (this.GridCore == null)
                return;
            this.GridCore.RootRow.SetVisibleChanged();
            this.GridCore.Invalidate();
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

        public event EventHandler HeightChanged;

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.FontChanged += gridCore_FontChanged;
        }

        protected virtual void OnHeightChanged(EventArgs e)
        {
            this.HeightChanged(this, e);
        }

        protected override void OnHeightChanged()
        {
            base.OnHeightChanged();
            this.OnHeightChanged(EventArgs.Empty);
        }

        private void gridCore_FontChanged(object sender, EventArgs e)
        {
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            pTextUpdater.AddTextBounds(this);
        }
    }
}
