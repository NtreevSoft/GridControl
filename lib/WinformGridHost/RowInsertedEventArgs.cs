using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowInsertedEventArgs : RowEventArgs
    {
        public RowInsertedEventArgs(Row row)
            : base(row)
        {

        }

        public object Component
        {
            get { return this.Row.Component; }
        }
    }
}
