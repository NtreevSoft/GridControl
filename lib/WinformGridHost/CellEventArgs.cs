using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class CellEventArgs : EventArgs
    {
        private readonly Cell cell;

        public CellEventArgs(Cell cell)
        {
            this.cell = cell;
        }

        public Cell Cell
        {
            get { return this.cell; }
        }
    }
}
