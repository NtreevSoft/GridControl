using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class IDataRow : GrRow
    {
        protected GrDataRowList m_pDataRowList;

        private bool m_expanded;
        private bool m_displayable;
        private bool m_clipped;
        private int m_visibleIndex;
        private int m_displayIndex;
        private int m_selectionGroup;

        private GrExpander m_pExpander;

        protected IDataRow()
        {
            m_displayable = false;
            m_clipped = false;
            m_visibleIndex = 0;
            m_displayIndex = 0;
            m_selectionGroup = 0;
            m_expanded = false;

            m_pExpander = new GrExpander(this);
        }

        public override int GetY()
        {
            if (m_pDataRowList == null)
                throw new Exception("GridCore에 붙지 않았음");

            if (m_visibleIndex < 0)
                return base.GetY();
            return base.GetY() - m_pDataRowList.GetDisplayOffset();
        }

        public override int GetWidth()
        {
            if (m_pDataRowList == null)
                return 0;
            if (this.GridCore.GetRowVisible() == false)
                return 0;
            return m_pDataRowList.GetRowWidth();
        }

        public override bool GetVisible()
        {
            IDataRow pParent = this.GetParent() as IDataRow;
            if (pParent == null)
                return base.GetVisible();

            return pParent.IsExpanded();
        }

        public override bool GetDisplayable()
        {
            if (this.GetVisible() == false)
                return false;
            return m_displayable;
        }

        public override sealed GrCell HitTest(GrPoint location)
        {
            if (ContainsVert(location.Y) == false)
                return null;

            if (m_pExpander.GetVisible() == true && m_pExpander.ContainsHorz(location.X) == true)
                return m_pExpander;

            GrCell pCell = OnHitTest(location.X);
            if (pCell != null)
                return pCell;

            if (ContainsHorz(location.X) == false)
                return null;

            return this;
        }

        public void SetDisplayable(bool b)
        {
            if (m_displayable == b)
                return;
            m_displayable = b;
            OnDisplayableChanged();
        }

        public void SetVisibleIndex(int index)
        {
            m_visibleIndex = index;
            OnVisibleChanged();
        }

        public int GetVisibleIndex()
        {
            return m_visibleIndex;
        }

        public bool IsLastVisible()
        {
            return (m_pDataRowList.GetVisibleRowCount() - 1) == GetVisibleIndex();
        }

        public void SetDisplayIndex(int index)
        {
            m_displayIndex = index;

            m_selectionGroup = index == GrDefineUtility.INSERTION_ROW ? 1 : 0;
        }

        public int GetDisplayIndex()
        {
            return m_displayIndex;
        }

        public void SetClipped(bool b)
        {
            m_clipped = b;
        }

        public bool GetClipped()
        {
            return m_clipped;
        }

        public override bool ShouldBringIntoView()
        {
            if (GetVisible() == false)
                throw new Exception("");
            if (m_displayable == false || m_clipped == true)
                return true;
            return false;
        }

        public bool HasFocused()
        {
            GrFocuser focuser = this.GridCore.GetFocuser();
            if (focuser.GetFocusedRow() == this)
                return true;
            return false;
        }

        public void KillFocus()
        {
            if (HasFocused() == true)
                return;
            this.GridCore.GetFocuser().Reset();
        }

        public void Expand(bool b)
        {
            if (m_expanded == b)
                return;
            if (m_pDataRowList != null)
                m_pDataRowList.SetVisibleChanged();
            m_expanded = b;
        }

        public bool IsExpanded()
        {
            if (GetChildCount() == 0)
                return false;
            return m_expanded;
        }

        public GrExpander GetExpander()
        {
            return m_pExpander;

        }

        public int GetDataDepth()
        {
            return GetDepth() - (m_pDataRowList.GetDepth() + 1);
        }

        public int GetSelectionGroup() { return m_selectionGroup; }

        public abstract IFocusable GetFocusable(GrColumn column);

        public virtual bool GetFullSelected() { return false; }

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

        public override GrPaintStyle ToPaintStyle()
        {
            GrPaintStyle flag = base.ToPaintStyle();
            if (HasFocused() == true)
                flag |= GrPaintStyle.Focused;
            return flag;
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();
            GrPaintStyle paintStyle = ToPaintStyle();
            GrColor foreColor = GetPaintingForeColor();
            GrColor backColor = GetPaintingBackColor();

            if (this.GridCore.GetRowVisible() == true)
            {
                if (GetClipped() == true)
                    painter.DrawRow(paintStyle, paintRect, GetPaintingLineColor(), backColor, clipRect);
                else
                    painter.DrawRow(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);
            }

            if (m_pDataRowList.GetMargin() > 0)
            {
                int left = paintRect.Right;
                int right = m_pDataRowList.CellStart();
                paintRect.X = left;
                paintRect.Width = right - left;
                if (GetRowType() == GrRowType.InsertionRow)
                    painter.DrawItem(GrPaintStyle.Default, paintRect, GetCellLineColor(), GetCellBackColor(), null);
                else if (GetVisibleIndex() == m_pDataRowList.GetVisibleRowCount() - 1)
                    painter.DrawItem(GrPaintStyle.Default, paintRect, GetCellLineColor(), GetCellBackColor(), null);
                else
                    painter.DrawItem(GrPaintStyle.RightLine, paintRect, GetCellLineColor(), GetCellBackColor(), clipRect);
            }

            DrawExpander(painter, clipRect);
        }

        protected override void OnFitted()
        {
            base.OnFitted();
            m_pDataRowList.SetFitChanged();
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            m_pDataRowList = this.GridCore.GetDataRowList();
            this.GridCore.AttachObject(m_pExpander);
            m_pDataRowList.SetHeightChanged();
        }

        protected override void OnHeightChanged()
        {
            base.OnHeightChanged();
            if (m_pDataRowList != null)
                m_pDataRowList.SetHeightChanged();
        }

        protected override void OnChildAdded(GrRow row)
        {
            base.OnChildAdded(row);

            if (m_pDataRowList != null)
            {
                m_pDataRowList.SetVisibleChanged();
                this.GridCore.GetColumnList().SetWidthChanged();
            }
        }

        protected virtual void OnDisplayableChanged()
        {

        }

        protected virtual void OnVisibleChanged()
        {

        }

        protected virtual GrCell OnHitTest(int x)
        {
            return null;
        }

        protected void DrawExpander(GrGridPainter painter, GrRect clipRect)
        {
            if (m_pExpander.GetVisible() == false)
                return;
            m_pExpander.Paint(painter, clipRect);
        }
        
        protected GrColor GetCellBackColor()
        {
            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.BackColor;
            return this.GridCore.GetBackColor();
        }

        protected GrColor GetCellLineColor()
        {
            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.LineColor;
            return this.GridCore.GetLineColor();
        }
        
        internal void InvokeOnYChanged()
        {
            this.OnYChanged();
        }
    }
}
