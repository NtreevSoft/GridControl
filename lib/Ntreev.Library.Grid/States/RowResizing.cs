using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class RowResizing : GrStateBase
    {
        GrRow m_row;
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
            GrRow row = pHitted as GrRow;
            if (row == null || row.GetResizable() == false || this.GridCore.GetRowResizable() == false)
                return false;
            return GetResizingRow(row, localLocation) != null;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_row = GetResizingRow(e.GetCell() as GrRow, e.GetLocalHit());

            GrRect rowRect = m_row.Bounds;

            m_resizingStart = rowRect.Bottom;
            m_resizingLocation = rowRect.Bottom;
            m_resizingMin = rowRect.Top + 0;
            m_resizingMax = rowRect.Top + 1000;

            m_downY = e.GetY();
        }

        public override void OnMouseDoubleClick(GrStateMouseEventArgs e)
        {
            GrDataRowList dataRowList = this.GridCore.DataRowList;

            IDataRow p = m_row as IDataRow;
            if (p != null && p.GetFullSelected() == true)
            {
                for (int i = 0; i < dataRowList.GetVisibleRowCount(); i++)
                {
                    GrDataRow pDataRow = dataRowList.GetVisibleRow(i) as GrDataRow;
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
                m_row.SetFit();
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
            int newHeight = m_row.Height + (m_resizingLocation - m_resizingStart);
            GrRect displayRect = this.GridCore.DisplayRectangle;
            if (m_row.Height != newHeight)
            {
                IDataRow p = m_row as IDataRow;
                if (p != null && p.GetFullSelected() == true)
                {
                    GrDataRowList dataRowList = m_row.GridCore.DataRowList;
                    int y = int.MaxValue;
                    for (int i = 0; i < dataRowList.GetVisibleRowCount(); i++)
                    {
                        GrDataRow pDataRow = dataRowList.GetVisibleRow(i) as GrDataRow;
                        if (pDataRow == null)
                            continue;
                        if (pDataRow.GetFullSelected() == false)
                            continue;
                        if (pDataRow.GetResizable() == false)
                            continue;
                        pDataRow.Height = newHeight;

                        y = Math.Min(pDataRow.Y, y);
                    }

                    if (y == int.MaxValue)
                        y = 0;
                    this.GridCore.Invalidate(displayRect.Left, y, displayRect.Right, displayRect.Bottom);
                }
                else
                {
                    m_row.Height = newHeight;
                    this.GridCore.Invalidate(displayRect.Left, m_row.Y, displayRect.Right, displayRect.Bottom);
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

        private GrRow GetResizingRow(GrRow row, GrPoint localLocation)
        {
            int margin = row.GetResizingMargin();

            if (localLocation.Y >= row.Height - margin)
            {
                if (row.GetResizable() == true)
                    return row;
            }
            else if (localLocation.Y < margin)
            {
                IDataRow pDataRow = row as IDataRow;
                if (pDataRow != null && pDataRow.GetVisibleIndex() > 0)
                {
                    GrDataRowList dataRowList = pDataRow.GridCore.DataRowList;
                    pDataRow = dataRowList.GetVisibleRow(pDataRow.GetVisibleIndex() - 1);
                    if (pDataRow.GetResizable())
                        return pDataRow;
                }
            }
            return null;
        }
    }
}
