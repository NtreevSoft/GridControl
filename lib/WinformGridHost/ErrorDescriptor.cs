using Ntreev.Windows.Forms.Grid.Properties;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Timers;

namespace Ntreev.Windows.Forms.Grid
{
    class ErrorDescriptor : GridObject
    {
        private readonly List<Row> m_rows = new List<Row>();
        private readonly Timer m_timer = new Timer();
        private int m_errorCount;
        private readonly Pen m_pen;

        public ErrorDescriptor(GridControl gridControl)
            : base(gridControl)
        {
            m_timer.Elapsed += errorTimer_Elapsed;
            m_timer.Interval = 300;
            m_errorCount = 0;

            m_pen = new Pen(Color.Firebrick, 2);
            m_pen.Alignment = PenAlignment.Inset;
        }

        public void Paint(Graphics g)
        {
            if (m_errorCount % 2 != 0)
                return;

            foreach (Row row in m_rows)
            {
                if (row.IsDisplayable == false)
                    continue;

                if (row.Error != string.Empty || row.SourceError != string.Empty)
                {
                    this.Paint(g, row);
                }

                if (row.Errors != null)
                {
                    foreach (Cell cell in row.Errors.Keys)
                    {
                        if (cell.IsDisplayable == false)
                            continue;

                        this.Paint(g, cell);
                    }
                }
                else if (row.SourceErrors != null)
                {
                    foreach (Cell cell in row.SourceErrors.Keys)
                    {
                        if (cell.IsDisplayable == false)
                            continue;

                        this.Paint(g, cell);
                    }
                }
                else if (row.InvalidValues != null)
                {
                    foreach (Cell cell in row.InvalidValues.Keys)
                    {
                        if (cell.IsDisplayable == false)
                            continue;

                        this.Paint(g, cell);
                    }
                }
            }
        }

        public void Add(Row row)
        {
            if (m_rows.Contains(row) == false)
                m_rows.Add(row);
            m_errorCount = 0;

            if (this.GridControl.Visible == true)
            {
                m_timer.Start();
            }
        }

        public void Remove(Row row)
        {
            m_rows.Remove(row);
            if (m_rows.Count == 0)
                m_errorCount = 0;
        }

        private void errorTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            m_errorCount++;
            if (m_errorCount > 8)
            {
                m_timer.Stop();
                m_errorCount = 0;
            }
            this.GridControl.Invalidate();
        }

        private void gridControl_Cleared(object sender, ClearEventArgs e)
        {
            m_rows.Clear();
            m_timer.Stop();
            m_errorCount = 0;
        }

        private void gridControl_VisibleChanged(object sender, EventArgs e)
        {
            if (this.GridControl.Visible == false)
            {
                m_timer.Stop();
                m_errorCount = 0;
            }
            else
            {
                if (m_rows.Count > 0)
                {
                    m_timer.Start();
                }
            }
        }

        private void gridControl_RowUnbinded(object sender, RowEventArgs e)
        {
            this.Remove(e.Row);
        }

        private void Paint(Graphics g, Cell cell)
        {
            Bitmap errorBitmap = Resources.Error;
            Rectangle bounds = cell.Bounds;
            bounds.Width--;
            bounds.Height--;

            g.DrawRectangle(m_pen, bounds);
            g.DrawImage(errorBitmap, bounds.Left + cell.Padding.Left, bounds.Top + cell.Padding.Top, errorBitmap.Width, errorBitmap.Height);
        }

        private void Paint(Graphics g, Row row)
        {
            Bitmap errorBitmap = Resources.Error;
            Rectangle bounds = row.Bounds;
            bounds.Width = this.GridCore.ColumnList.Bounds.Width;
            bounds.Height--;
            g.DrawRectangle(m_pen, bounds);

            bounds = row.Bounds;
            g.DrawImage(errorBitmap, bounds.Left + row.Padding.Left, bounds.Top + row.Padding.Top, errorBitmap.Width, errorBitmap.Height);
        }
    }
}
