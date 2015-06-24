using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowInsertingEventArgs : RowEventArgs
    {
        private object component;
        private bool cancel;

        public RowInsertingEventArgs(Row row)
            : base(row)
        {

        }

        internal RowInsertingEventArgs()
        {

        }

        public bool Cancel
        {
            get { return this.cancel; }
            set { this.cancel = value; }
        }

        public object Component
        {
            get { return this.component; }
            internal set { this.component = value; }
        }
    }
}
