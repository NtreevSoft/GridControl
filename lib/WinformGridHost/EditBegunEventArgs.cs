using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class EditBegunEventArgs : CellEventArgs
    {
        private bool m_cancel;

        public EditBegunEventArgs(Cell cell)
            : base(cell)
        {

        }

        public bool Cancel
        {
            get { return m_cancel; }
            set { m_cancel = value; }
        }
    }
}
