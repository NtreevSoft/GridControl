using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowEventArgs : EventArgs
    {
        private Row row;

        public RowEventArgs(Row row)
        {
            this.row = row;
        }

        internal RowEventArgs()
        {

        }

        public Row Row
        {
            get { return this.row; }
            internal set { this.row = value; }
        }
    }
}
