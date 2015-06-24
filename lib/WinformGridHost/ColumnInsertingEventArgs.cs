using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class ColumnInsertingEventArgs : ColumnEventArgs
    {
        private bool m_cancel;

        public ColumnInsertingEventArgs(Column column)
            : base(column)
        {

        }

        public bool Cancel
        {
            get { return m_cancel; }
            set { m_cancel = value; }
        }
    }
}
