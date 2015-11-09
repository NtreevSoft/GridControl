using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid.States
{
    class ColumnResizing : GrStateBase
    {
        GrColumn m_column;
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
            if (pHitted is GrColumn == false)
                return false;

            GrColumn column = pHitted as GrColumn;
            return GetResizingColumn(column, localLocation) != null;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_column = GetResizingColumn(e.GetCell() as GrColumn, e.GetLocalHit());

            GrRect columnRect = m_column.Bounds;

            m_resizingStart = columnRect.Right;
            m_resizingLocation = columnRect.Right;
            m_resizingMin = columnRect.Left + m_column.MinWidth;

            if (m_column.MaxWidth == 0)
                m_resizingMax = int.MaxValue;
            else
                m_resizingMax = columnRect.Left + m_column.MaxWidth;

            m_downX = e.GetX();

        }

        public override void OnMouseDoubleClick(GrStateMouseEventArgs e)
        {
            GrColumnList columnList = this.GridCore.ColumnList;

            if (m_column.IsFullSelected == true)
            {
                for (int i = 0; i < columnList.GetVisibleColumnCount(); i++)
                {
                    GrColumn column = columnList.GetVisibleColumn(i);
                    if (column.IsFullSelected == false)
                        continue;
                    if (column.IsResizable == false)
                        continue;
                    column.SetFit();
                }
            }
            else
            {
                m_column.SetFit();
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

            int newWidth = m_column.Width + (m_resizingLocation - m_resizingStart);
            if (m_column.Width != newWidth)
            {
                GrColumnList columnList = m_column.ColumnList;
                int x = int.MaxValue;
                if (m_column.IsFullSelected == true)
                {
                    for (int i = 0; i < columnList.GetVisibleColumnCount(); i++)
                    {
                        GrColumn column = columnList.GetVisibleColumn(i);
                        if (column.IsFullSelected == false)
                            continue;
                        if (column.IsResizable == false)
                            continue;
                        column.Width = newWidth;
                        x = Math.Min(x, column.X);
                    }
                    if (x == int.MaxValue)
                        x = 0;
                }
                else
                {
                    m_column.Width = newWidth;
                    x = m_column.X;
                }
                GrRect displayRect = this.GridCore.DisplayRectangle;
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

        private GrColumn GetResizingColumn(GrColumn column, GrPoint localLocation)
        {
            if (this.GridCore.GetColumnResizable() == false)
                return null;

            int margin = column.GetResizingMargin();

            int width = column.Width;
            if (margin * 3 > width)
                margin = (int)((float)width / 3.0f);

            if (column.IsClipped == true)
            {
                int x = localLocation.X + column.X;
                if (x >= this.GridCore.DisplayRectangle.Right - margin)
                    return column;
            }
            else if (localLocation.X >= column.Width - margin)
            {
                if (column.IsResizable == true)
                    return column;
            }
            else if (localLocation.X < margin)
            {
                int index = column.GetVisibleIndex();
                if ((int)index > 0)
                {
                    GrColumnList columnList = column.ColumnList;
                    column = columnList.GetVisibleColumn(index - 1);
                    if (column.IsResizable == true)
                        return column;
                }
            }
            return null;
        }
    }
}
