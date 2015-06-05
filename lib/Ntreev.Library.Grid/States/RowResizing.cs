using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class RowResizing : GrStateBase
    {
        GrRow m_pRow;
        int m_resizingStart;
        int m_resizingLocation;
        int m_resizingMin;
        int m_resizingMax;

        int m_downY;

        public RowResizing()
        {

        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            GrRow pRow = pHitted as GrRow;
            if (pRow == null || pRow.GetResizable() == false || this.GridCore.GetRowResizable() == false)
                return false;
            return GetResizingRow(pRow, localLocation) != null;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_pRow = GetResizingRow(e.GetCell() as GrRow, e.GetLocalHit());

            GrRect rowRect = m_pRow.GetRect();

            m_resizingStart = rowRect.Bottom;
            m_resizingLocation = rowRect.Bottom;
            m_resizingMin = rowRect.Top + 0;
            m_resizingMax = rowRect.Top + 1000;

            m_downY = e.GetY();
        }

        public override void OnMouseDoubleClick(GrStateMouseEventArgs e)
        {
            GrDataRowList pDataRowList = this.GridCore.GetDataRowList();

            IDataRow p = m_pRow as IDataRow;
            if (p != null && p.GetFullSelected() == true)
            {
                for (int i = 0; i < pDataRowList.GetVisibleRowCount(); i++)
                {
                    GrDataRow pDataRow = pDataRowList.GetVisibleRow(i) as GrDataRow;
                    if (pDataRow == null)
                        continue;
                    if (pDataRow.GetFullSelected() == false)
                        continue;
                    if (pDataRow.GetResizable() == false)
                        continue;
                    pDataRow.SetFit();
                }
            }
            else
            {
                m_pRow.SetFit();
            }
            this.GridCore.Invalidate();
        }

        public override void OnPaintAdornments(GrGridPainter g, GrRect displayRect)
        {
            g.DrawResizingLine(displayRect.Left, m_resizingLocation, displayRect.Right, m_resizingLocation);
        }

        public override void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {
            int oldY = m_resizingLocation;
            m_resizingLocation = (location.Y - m_downY) + m_resizingStart;
            m_resizingLocation = Math.Max(m_resizingLocation, m_resizingMin);
            m_resizingLocation = Math.Min(m_resizingLocation, m_resizingMax);

            if (oldY != m_resizingLocation)
            {
                this.GridCore.Invalidate();
            }
        }

        public override void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {
            int newHeight = m_pRow.GetHeight() + (m_resizingLocation - m_resizingStart);
            GrRect displayRect = this.GridCore.GetDisplayRect();
            if (m_pRow.GetHeight() != newHeight)
            {
                IDataRow p = m_pRow as IDataRow;
                if (p != null && p.GetFullSelected() == true)
                {
                    GrDataRowList pDataRowList = m_pRow.GetGridCore().GetDataRowList();
                    int y = int.MaxValue;
                    for (int i = 0; i < pDataRowList.GetVisibleRowCount(); i++)
                    {
                        GrDataRow pDataRow = pDataRowList.GetVisibleRow(i) as GrDataRow;
                        if (pDataRow == null)
                            continue;
                        if (pDataRow.GetFullSelected() == false)
                            continue;
                        if (pDataRow.GetResizable() == false)
                            continue;
                        pDataRow.SetHeight(newHeight);

                        y = Math.Min(pDataRow.GetY(), y);
                    }

                    if (y == int.MaxValue)
                        y = 0;
                    this.GridCore.Invalidate(displayRect.Left, y, displayRect.Right, displayRect.Bottom);
                }
                else
                {
                    m_pRow.SetHeight(newHeight);
                    this.GridCore.Invalidate(displayRect.Left, m_pRow.GetY(), displayRect.Right, displayRect.Bottom);
                }
            }
            else
            {
                this.GridCore.Invalidate();
            }
        }

        public override GrGridState GetState() { return GrGridState.RowResizing; }

        public override bool GetDragable() { return true; }

        public override GrCursor GetCursor() { return GrCursor.HSplit; }

        private GrRow GetResizingRow(GrRow pRow, GrPoint localLocation)
        {
            int margin = pRow.GetResizingMargin();

            if (localLocation.Y >= pRow.GetHeight() - margin)
            {
                if (pRow.GetResizable() == true)
                    return pRow;
            }
            else if (localLocation.Y < margin)
            {
                IDataRow pDataRow = pRow as IDataRow;
                if (pDataRow != null && pDataRow.GetVisibleIndex() > 0)
                {
                    GrDataRowList pDataRowList = pDataRow.GetGridCore().GetDataRowList();
                    pDataRow = pDataRowList.GetVisibleRow(pDataRow.GetVisibleIndex() - 1);
                    if (pDataRow.GetResizable())
                        return pDataRow;
                }
            }
            return null;
        }
    }
}
