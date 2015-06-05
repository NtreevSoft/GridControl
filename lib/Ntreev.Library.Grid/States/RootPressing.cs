using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class RootPressing : GrStateBase
    {
        public RootPressing()
        {

        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            return pHitted.GetCellType() == GrCellType.Root;
        }

        public override void OnBegin(GrStateEventArgs e)
        {
            this.GridCore.SetMousePress(e.GetCell());

            m_pItemSelector.SelectAll();

            GrItem pItem = null;
            GrDataRowList pDataRowList = this.GridCore.GetDataRowList();
            GrColumnList pColumnList = this.GridCore.GetColumnList();

            if (pDataRowList.GetVisibleDataRowCount() > 0 && pColumnList.GetVisibleColumnCount() > 0)
            {
                GrColumn pColumn = pColumnList.GetVisibleColumn(0);
                GrDataRow pDataRow = pDataRowList.GetVisibleDataRow(0);
                pItem = pDataRow.GetItem(pColumn);
            }

            m_pFocuser.Set(pItem);
        }

        public override void OnEnd(GrStateEventArgs e)
        {
            this.GridCore.SetMouseUnpress();
        }

        public override GrGridState GetState() { return GrGridState.RootPressing; }
    }
}
