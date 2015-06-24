using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace Ntreev.Windows.Forms.Grid
{
    public abstract class ColumnPainter : Component
    {
        private VisualStyleRenderer m_sortedDownRenderer;
        private VisualStyleRenderer m_sortedUpPainter;

        public ColumnPainter()
        {
            try
            {
                m_sortedDownRenderer = new VisualStyleRenderer(VisualStyleElement.Header.SortArrow.SortedDown);
            }
            catch (Exception)
            {

            }

            try
            {
                m_sortedUpPainter = new VisualStyleRenderer(VisualStyleElement.Header.SortArrow.SortedUp);
            }
            catch (Exception)
            {

            }
        }

        public abstract bool PaintBackground(Graphics g, Rectangle paintRect, IColumn column);

        public abstract bool PaintContents(Graphics g, Rectangle paintRect, IColumn column);

        protected void DrawSortArrow(Graphics g, Rectangle paintRect, SortType sortType)
        {
            switch (sortType)
            {
                case SortType.Up:
                    if (m_sortedUpPainter != null)
                        m_sortedUpPainter.DrawBackground(g, paintRect);
                    else
                        ControlPaint.DrawScrollButton(g, paintRect, ScrollButton.Up, ButtonState.Flat);
                    break;
                case SortType.Down:
                    if (m_sortedDownRenderer != null)
                        m_sortedDownRenderer.DrawBackground(g, paintRect);
                    else
                        ControlPaint.DrawScrollButton(g, paintRect, ScrollButton.Down, ButtonState.Flat);
                    break;
            }
        }
    }
}
