using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrCaption : GrUpdatableRow
    {
        private GrHorzAlign m_horzAlign;
        private GrVertAlign m_vertAlign;

        private bool m_visible1;

        public GrCaption()
        {
            m_horzAlign = GrHorzAlign.Left;
            m_vertAlign = GrVertAlign.Center;

            m_visible1 = true;
        }

        public override GrHorzAlign GetTextHorzAlign()
        {
            return m_horzAlign;
        }

        public override GrVertAlign GetTextVertAlign()
        {
            return m_vertAlign;
        }

        public void SetTextHorzAlign(GrHorzAlign align)
        {
            if (m_horzAlign == align)
                return;
            m_horzAlign = align;
            SetTextAlignChanged();
        }

        public void SetTextVertAlign(GrVertAlign align)
        {
            if (m_vertAlign == align)
                return;
            m_vertAlign = align;
            SetTextAlignChanged();
        }

        public override GrRowType GetRowType() { return GrRowType.Caption; }

        public override int GetWidth()
        {
            return this.GridCore.GetBounds().GetWidth();
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();
            GrRect displayRect = this.GridCore.GetDisplayRect();
            GrColumnList columnList = this.GridCore.GetColumnList();
            if (this.GridCore.GetFillBlank() == true && columnList.GetDisplayableRight() < displayRect.Right)
            {
                paintRect.Width = displayRect.Right - paintRect.Left;
            }

            if (paintRect.Top >= clipRect.Bottom || paintRect.Bottom < clipRect.Top)
                return;

            GrPaintStyle paintStyle = ToPaintStyle() & ~GrPaintStyle.RightLine;
            GrColor foreColor = GetPaintingForeColor();
            GrColor backColor = GetPaintingBackColor();

            painter.DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);

            DrawText(painter, foreColor, paintRect, paintRect);
        }

        public override bool GetVisible()
        {
            return m_visible1;
        }

        public void SetVisible(bool b)
        {
            if (m_visible1 == b)
                return;

            m_visible1 = b;

            if (this.GridCore == null)
                return;
            this.GridCore.GetRootRow().SetVisibleChanged();
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
            OnHeightChanged(EventArgs.Empty);
        }
        
        private void gridCore_FontChanged(object sender, EventArgs e)
        {
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            pTextUpdater.AddTextBounds(this);
        }
    }
}
