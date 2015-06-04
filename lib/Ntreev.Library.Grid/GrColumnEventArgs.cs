using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumnEventArgs : EventArgs
    {
        private readonly GrColumn m_pColumn;

        public GrColumnEventArgs(GrColumn pColumn)
        {
            m_pColumn = pColumn;
        }

        public GrColumn GetColumn()
        {
            return m_pColumn;
        }
    }
}
