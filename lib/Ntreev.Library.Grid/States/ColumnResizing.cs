using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid.States
{
    class ColumnResizing : GrStateBase
    {
        GrColumn m_pColumn;
        int m_resizingStart;
        int m_resizingLocation;
        int m_resizingMin;
        int m_resizingMax;

        int m_downX;

        public ColumnResizing()
        {
        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            if (pHitted.GetCellType() != GrCellType.Column)
                return false;

            GrColumn pColumn = pHitted as GrColumn;
            return GetResizingColumn(pColumn, localLocation) != null;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_pColumn = GetResizingColumn(e.GetCell() as GrColumn, e.GetLocalHit());

            GrRect columnRect = m_pColumn.GetRect();

            m_resizingStart = columnRect.Right;
            m_resizingLocation = columnRect.Right;
            m_resizingMin = columnRect.Left + m_pColumn.GetMinWidth();

            if (m_pColumn.GetMaxWidth() == 0)
                m_resizingMax = int.MaxValue;
            else
                m_resizingMax = columnRect.Left + m_pColumn.GetMaxWidth();

            m_downX = e.GetX();

        }

        public override void OnMouseDoubleClick(GrStateMouseEventArgs e)
        {
            GrColumnList pColumnList = this.GridCore.GetColumnList();

            if (m_pColumn.GetFullSelected() == true)
            {
                for (int i = 0; i < pColumnList.GetVisibleColumnCount(); i++)
                {
                    GrColumn pColumn = pColumnList.GetVisibleColumn(i);
                    if (pColumn.GetFullSelected() == false)
                        continue;
                    if (pColumn.GetResizable() == false)
                        continue;
                    pColumn.SetFit();
                }
            }
            else
            {
                m_pColumn.SetFit();
            }
            this.GridCore.Invalidate();
        }

        public override void OnPaintAdornments(GrGridPainter g, GrRect displayRect)
        {
            g.DrawResizingLine(m_resizingLocation, displayRect.Top, m_resizingLocation, displayRect.Bottom);
        }

        public override void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {
            int oldX = m_resizingLocation;
            m_resizingLocation = (location.X - m_downX) + m_resizingStart;
            m_resizingLocation = Math.Max(m_resizingLocation, m_resizingMin);
            m_resizingLocation = Math.Min(m_resizingLocation, m_resizingMax);

            if (oldX != m_resizingLocation)
            {
                this.GridCore.Invalidate();
            }
        }

        public override void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {
            if (cancel == true)
            {
                this.GridCore.Invalidate();
                return;
            }

            int newWidth = m_pColumn.GetWidth() + (m_resizingLocation - m_resizingStart);
            if (m_pColumn.GetWidth() != newWidth)
            {
                GrColumnList pColumnList = m_pColumn.GetColumnList();
                int x = int.MaxValue;
                if (m_pColumn.GetFullSelected() == true)
                {
                    for (int i = 0; i < pColumnList.GetVisibleColumnCount(); i++)
                    {
                        GrColumn pColumn = pColumnList.GetVisibleColumn(i);
                        if (pColumn.GetFullSelected() == false)
                            continue;
                        if (pColumn.GetResizable() == false)
                            continue;
                        pColumn.SetWidth(newWidth);
                        x = Math.Min(x, pColumn.GetX());
                    }
                    if (x == int.MaxValue)
                        x = 0;
                }
                else
                {
                    m_pColumn.SetWidth(newWidth);
                    x = m_pColumn.GetX();
                }
                GrRect displayRect = this.GridCore.GetDisplayRect();
                this.GridCore.Invalidate(x, displayRect.Top, displayRect.Right, displayRect.Bottom);
            }
            else
            {
                this.GridCore.Invalidate();
            }
        }

        public override GrGridState GetState() { return GrGridState.ColumnResizing; }

        public override bool GetDragable() { return true; }

        public override GrCursor GetCursor() { return GrCursor.VSplit; }

        private GrColumn GetResizingColumn(GrColumn pColumn, GrPoint localLocation)
        {
            if (this.GridCore.GetColumnResizable() == false)
                return null;

            int margin = pColumn.GetResizingMargin();

            int width = pColumn.GetWidth();
            if (margin * 3 > width)
                margin = (int)((float)width / 3.0f);

            if (pColumn.GetClipped() == true)
            {
                int x = localLocation.X + pColumn.GetX();
                if (x >= this.GridCore.GetDisplayRect().Right - margin)
                    return pColumn;
            }
            else if (localLocation.X >= pColumn.GetWidth() - margin)
            {
                if (pColumn.GetResizable() == true)
                    return pColumn;
            }
            else if (localLocation.X < margin)
            {
                int index = pColumn.GetVisibleIndex();
                if ((int)index > 0)
                {
                    GrColumnList pColumnList = pColumn.GetColumnList();
                    pColumn = pColumnList.GetVisibleColumn(index - 1);
                    if (pColumn.GetResizable() == true)
                        return pColumn;
                }
            }
            return null;
        }
    }
}
