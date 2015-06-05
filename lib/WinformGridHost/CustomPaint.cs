using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    class CustomPaint
    {
        Column m_column;
        public CustomPaint(Column column)
        {
            m_column = m_column;
        }

        static bool ColumnBackgroundPaint(GrGridPainter pPainter, GrColumn pColumn, GrRect paintRect, object dwUserData)
        {
            CustomPaint pCustomPainter = (CustomPaint)dwUserData;
            return pCustomPainter.OnColumnBackGroundPaint(pPainter, pColumn, paintRect);
        }

        static bool ColumnContentsPaint(GrGridPainter pPainter, GrColumn pColumn, GrRect paintRect, object dwUserData)
        {
            CustomPaint pCustomPainter = (CustomPaint)dwUserData;
            return pCustomPainter.OnColumnContentsPaint(pPainter, pColumn, paintRect);
        }

        bool OnColumnBackGroundPaint(GrGridPainter pPainter, GrColumn pColumn, GrRect paintRect)
        {
            IntPtr hdc = new IntPtr(pPainter.GetDevice());
            Graphics g = Graphics.FromHdc(hdc);
            Column column = m_column;
            ColumnPainter columnPainter = column.ColumnPainter;
            bool b = columnPainter.PaintBackground(g, paintRect, column);
            g.Dispose();
            pPainter.ReleaseDevice(hdc.ToPointer());
            return b;
        }

        bool OnColumnContentsPaint(GrGridPainter pPainter, GrColumn pColumn, GrRect paintRect)
        {
            IntPtr hdc = new IntPtr(pPainter.GetDevice());
            Graphics g = Graphics.FromHdc(hdc);
            Column column = m_column;
            ColumnPainter columnPainter = column.ColumnPainter;
            bool b = columnPainter.PaintContents(g, paintRect, column);
            g.Dispose();
            pPainter.ReleaseDevice(hdc.ToPointer());
            return b;
        }
    }
}
