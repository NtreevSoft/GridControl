using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class ExpanderPressing : GrStateBase
    {
        IDataRow m_pDataRow;

        public ExpanderPressing()
        {

        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            GrExpander pGroupCell = pHitted as GrExpander;
            if (pGroupCell == null)
                return false;

            return true;
        }

        public override void OnBegin(GrStateEventArgs e)
        {
            GrExpander pGroupCell = e.GetCell() as GrExpander;

            m_pDataRow = pGroupCell.GetRow() as IDataRow;
        }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            m_pDataRow.IsExpanded = !m_pDataRow.IsExpanded;
            this.GridCore.Invalidate();
        }

        public override GrGridState GetState()
        {
            return GrGridState.ExpanderPressing;
        }
    }
}
