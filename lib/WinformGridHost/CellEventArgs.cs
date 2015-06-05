using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class CellEventArgs : EventArgs
    {
        private readonly Cell m_cell;

        public CellEventArgs(Cell cell)
        {
            m_cell = cell;
        }

        public Cell Cell
        {
            get { return m_cell; }
        }
    }
}
