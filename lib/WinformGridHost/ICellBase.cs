using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    public interface ICellBase
    {
        Color ForeColor
        {
            get;
        }

        Color BackColor
        {
            get;
        }

        Font Font
        {
            get;
        }

        Color PaintingForeColor
        {
            get;
        }

        Color PaintingBackColor
        {
            get;
        }

        Rectangle Bounds
        {
            get;
        }

        int X
        {
            get;
        }

        int Y
        {
            get;
        }

        int Width
        {
            get;
        }

        int Height
        {
            get;
        }

        Point Location
        {
            get;
        }

        Size Size
        {
            get;
        }

        int Left
        {
            get;
        }

        int Top
        {
            get;
        }

        int Right
        {
            get;
        }

        int Bottom
        {
            get;
        }

        Rectangle ClientRectangle
        {
            get;
        }

        Padding Padding
        {
            get;
        }

        bool Multiline
        {
            get;
        }

        bool WordWrap
        {
            get;
        }

        StringAlignment Alignment
        {
            get;
        }

        StringAlignment LineAlignment
        {
            get;
        }

        CellState State
        {
            get;
        }
    }
}
