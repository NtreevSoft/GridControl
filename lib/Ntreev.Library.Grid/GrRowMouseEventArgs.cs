using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrRowMouseEventArgs : GrMouseEventArgs
    {
        private readonly GrRow m_pRow;
        private bool m_handled;

        public GrRowMouseEventArgs(GrRow pRow, GrPoint location, GrKeys modifierKeys)
            : base(location, modifierKeys)
        {
            m_pRow = pRow;
        }

        public GrRow GetRow()
        {
            return m_pRow;
        }

        public bool GetHandled()
        {
            return m_handled;
        }

        public void SetHandled(bool value)
        {
            m_handled = value;
        }

    }
}
