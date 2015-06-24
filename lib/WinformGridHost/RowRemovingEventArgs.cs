using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowRemovingEventArgs : RowEventArgs
    {
        private bool m_cancel;

        public RowRemovingEventArgs(Row row)
            : base(row)
        {

        }

        public bool Cancel
        {
            get { return m_cancel; }
            set { m_cancel = value; }
        }
    }
}
