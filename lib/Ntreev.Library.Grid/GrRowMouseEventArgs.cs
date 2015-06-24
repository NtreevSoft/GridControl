using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrRowMouseEventArgs : GrMouseEventArgs
    {
        private readonly GrRow m_row;
        private bool m_handled;

        public GrRowMouseEventArgs(GrRow row, GrPoint location, GrKeys modifierKeys)
            : base(location, modifierKeys)
        {
            m_row = row;
        }

        public GrRow GetRow()
        {
            return m_row;
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
