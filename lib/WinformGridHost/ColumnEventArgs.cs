using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class ColumnEventArgs : EventArgs
    {
        private readonly Column column;

        public ColumnEventArgs(Column column)
        {
            this.column = column;
        }

        public Column Column
        {
            get { return this.column; }
        }
    }
}
