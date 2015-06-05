using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid.States
{
    class ColumnSplitterMoving : GrStateBase
    {
        int m_location;
        int m_freezableIndex;
        GrColumnSplitter m_pColumnSplitter;

        public ColumnSplitterMoving()
        {

        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            if (this.GridCore.GetColumnFreezable() == false)
                return false;
            return pHitted.GetCellType() == GrCellType.Splitter;
        }

        public override void OnBegin(GrStateEventArgs e)
        {
            m_pColumnSplitter = e.GetCell() as GrColumnSplitter;
        }

        public override void OnPaintAdornments(GrGridPainter g, GrRect displayRect)
        {
            GrRect paintRect = new GrRect(new GrPoint(m_location, displayRect.Top), new GrSize(GrColumnSplitter.DefaultSplitterWidth, displayRect.GetHeight()));
            g.DrawSplitterMovingLine(paintRect);
        }

        public override void OnMouseDragBegin(GrPoint location)
        {
            m_location = m_pColumnSplitter.GetX();
            this.GridCore.Invalidate();
        }

        public override void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {
            GrColumnList pColumnList = m_pColumnSplitter.GetColumnList();

            int oldLocation = m_location;

            for (int i = 0; i < pColumnList.GetDisplayableColumnCount(); i++)
            {
                GrColumn pColumn = pColumnList.GetDisplayableColumn(i);
                GrRect columnRect = pColumn.GetRect();
                if (location.X < columnRect.Left || location.X >= columnRect.Right)
                    continue;
                int centerValue = columnRect.GetCenter().X;

                if (location.X > centerValue)
                {
                    if (pColumn.GetFrozen())
                    {
                        m_location = columnRect.Right;
                        m_freezableIndex = i;
                    }
                    else
                    {
                        m_location = columnRect.Right - GrColumnSplitter.DefaultSplitterWidth;
                        m_freezableIndex = i;
                    }
                }
                else
                {
                    if (pColumn.GetFrozen())
                        m_location = columnRect.Left;
                    else
                        m_location = columnRect.Left - GrColumnSplitter.DefaultSplitterWidth;
                    m_freezableIndex = i - 1;
                }
            }

            if (oldLocation != m_location)
            {
                GrRect displayRect = this.GridCore.GetDisplayRect();
                this.GridCore.Invalidate(oldLocation - 2, displayRect.Top, oldLocation + GrColumnSplitter.DefaultSplitterWidth + 2, displayRect.Bottom);
                this.GridCore.Invalidate(m_location - 2, displayRect.Top, m_location + GrColumnSplitter.DefaultSplitterWidth + 2, displayRect.Bottom);
            }
        }

        public override void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {
            if (cancel == true)
            {
                GrRect displayRect = this.GridCore.GetDisplayRect();
                this.GridCore.Invalidate(m_location - 2, displayRect.Top, m_location + GrColumnSplitter.DefaultSplitterWidth + 2, displayRect.Bottom);
                return;
            }

            GrColumnList pColumnList = m_pColumnSplitter.GetColumnList();

            for (int i = 0; i < pColumnList.GetDisplayableColumnCount(); i++)
            {
                GrColumn pColumn = pColumnList.GetDisplayableColumn(i);
                bool oldFrozen = pColumn.GetFrozen();
                if (m_freezableIndex == -1)
                    pColumn.SetFrozen(false);
                else
                    pColumn.SetFrozen(i <= (uint)m_freezableIndex);

                if (oldFrozen != pColumn.GetFrozen())
                {

                }
            }

            this.GridCore.Invalidate();
        }

        public override GrGridState GetState() { return GrGridState.ColumnSplitterMoving; }

        public override bool GetDragable() { return true; }

        public override GrCursor GetCursor() { return GrCursor.SizeWE; }
    }
}
