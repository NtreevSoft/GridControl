using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class GroupRow : RowBase
    {
        private readonly GrGroupRow m_pGroupRow;

        internal GroupRow(GrGroupRow pGroupRow)
            : base(pGroupRow)
        {
            m_pGroupRow = pGroupRow;
        }

        public override string ToString()
        {
            return this.Text;
        }

        public string Text
        {
            get
            {
                GrGroupHeader pGroupHeader = m_pGroupRow.GetLabel();
                return pGroupHeader.Text;
            }
        }
    }
}
