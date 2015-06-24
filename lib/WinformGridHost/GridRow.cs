using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class GridRow : RowBase
    {
        private GrGridRow m_pGridRow;

        internal GridRow(GrGridRow pGridRow)
            : base(pGridRow)
        {

        }

        public GridControl ChildGrid
        {
            get { return m_pGridRow.GetChildGrid(); }
        }
    }
}
