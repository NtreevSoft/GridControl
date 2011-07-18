using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Ntreev.Windows.Forms.Grid;
using System.Drawing;

namespace ColumnMouseEvent
{
    class UserColumnPainter : ColumnPainter
    {
        public override bool PaintBackground(System.Drawing.Graphics g, System.Drawing.Rectangle renderRect, IColumnDescriptor columnDescriptor, IStyle style)
        {
            return false;
        }

        public override bool PaintContents(System.Drawing.Graphics g, System.Drawing.Rectangle renderRect, IColumnDescriptor columnDescriptor, IStyle style)
        {
            Rectangle buttonRectangle = ComputeButtonRectangle(renderRect);
            g.DrawImage(Properties.Resources.ButtonImage, buttonRectangle);
            return false;
        }

        public Rectangle ComputeButtonRectangle(Rectangle columnRectangle)
        {
            Bitmap bitmap = Properties.Resources.ButtonImage;

            int left = columnRectangle.Left + 7;
            int top = columnRectangle.Top + (columnRectangle.Height - bitmap.Height) / 2;

            return new Rectangle(left, top, bitmap.Width, bitmap.Height);
        }
    }
}
