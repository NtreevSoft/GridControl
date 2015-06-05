using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class ItemEditing : GrStateBase
    {
        GrColumn m_pColumn;
        GrItem m_pItem;

        GrEditingResult m_result;

        public ItemEditing()
        {

        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            GrItem pItem = pHitted as GrItem;
            if (pItem == null || pItem.GetFocused() == false)
                return false;
            GrEditingReason reason = new GrEditingReason(localLocation);
            return m_pGridWindow.CanEdit(pItem, reason);
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            GrEditingReason reason = new GrEditingReason(e.GetLocation());
            e.SetNextState(OnBegin(reason));
        }

        public override void OnMouseMove(GrStateMouseEventArgs e)
        {
            GrCell pCell = e.GetCell();
            GrCell pOldCell = this.GridCore.GetMouseOver();

            this.GridCore.SetMouseOver(pCell, e.GetLocalHit());
            InvokeMouseEvent(pOldCell, pCell, e);
        }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            e.SetNextState(this.GetState());
            this.GridCore.SetMouseUnpress();
            this.GridCore.Invalidate();
        }

        public override void OnBegin(GrStateEventArgs e)
        {
            m_pItem = e.GetCell() as GrItem;

            GrEditingReason pReason = e.GetUserData() as GrEditingReason;

            if (pReason != null && m_pItem != null)
                e.SetNextState(OnBegin(pReason));
        }

        public override void OnEnd(GrStateEventArgs e)
        {
            m_pItem.LockColor(false);
        }

        public override GrGridState GetState() { return GrGridState.ItemEditing; }

        private GrGridState OnBegin(GrEditingReason reason)
        {
            m_result = GrEditingResult.None;
            m_pColumn = m_pItem.GetColumn();
            m_pItem.LockColor(true);

            GrEditEventArgs e = new GrEditEventArgs(m_pItem, reason);
            this.GridCore.Invoke("EditValue", e);

            if (e.GetHandled() == true && this.GridCore.IsGrouped() == true)
            {
                this.GridCore.GetDataRowList().SetListChanged();
            }
            return GrGridState.Normal;
        }

        enum GrEditingResult
        {
            None,
            OK,
            Cancel,
        };
    }
}
