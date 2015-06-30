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
            return pHitted is GrColumnList;
        }

        public override void OnBegin(GrStateEventArgs e)
        {
            this.GridCore.SetMousePress(e.GetCell());

            m_pItemSelector.SelectAll();

            GrItem pItem = null;
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            GrColumnList columnList = this.GridCore.ColumnList;

            if (dataRowList.GetVisibleDataRowCount() > 0 && columnList.GetVisibleColumnCount() > 0)
            {
                GrColumn column = columnList.GetVisibleColumn(0);
                GrDataRow pDataRow = dataRowList.GetVisibleDataRow(0);
                pItem = pDataRow.GetItem(column);
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
