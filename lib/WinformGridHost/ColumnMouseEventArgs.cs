using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class ColumnMouseEventArgs : ColumnEventArgs
    {
        private readonly Point location;
        private bool handled;

        public ColumnMouseEventArgs(Column column, Point location)
            : base(column)
        {
            this.location = location;
        }

        public int X
        {
            get { return this.location.X; }
        }

        public int Y
        {
            get { return this.location.Y; }
        }

        public Point Location
        {
            get { return this.location; }
        }

        public bool Handled
        {
            get { return this.handled; }
            set { this.handled = value; }
        }
    }
}
