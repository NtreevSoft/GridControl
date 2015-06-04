using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrDataRowEventArgs : EventArgs
    {
        private readonly GrDataRow m_pDataRow;

        public GrDataRowEventArgs(GrDataRow pDataRow)
        {
            this.m_pDataRow = pDataRow;
        }

        public GrDataRow GetDataRow()
        {
            return m_pDataRow;
        }

    }
}
