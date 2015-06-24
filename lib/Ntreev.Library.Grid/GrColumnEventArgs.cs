using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumnEventArgs : EventArgs
    {
        private readonly GrColumn m_column;

        public GrColumnEventArgs(GrColumn column)
        {
            m_column = column;
        }

        public GrColumn GetColumn()
        {
            return m_column;
        }
    }
}
