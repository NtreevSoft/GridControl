using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class ItemButtonPressing : GrStateBase
    {
        GrItem m_pItem;
        GrPoint m_mouseDownPoint;

        public ItemButtonPressing()
        {

        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            if (pHitted.GetCellType() != GrCellType.Item)
                return false;
            return (pHitted as GrItem).HitMouseOverTest(localLocation) == (int)GrMouseOverState.Control;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_pItem = e.GetCell() as GrItem;
            m_pFocuser.Set(m_pItem);
            this.GridCore.SetMousePress(m_pItem);
            this.GridCore.Invalidate();
            m_mouseDownPoint = e.GetLocation();
        }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            if (m_pItem.HitMouseOverTest(e.GetLocalHit()) == (int)GrMouseOverState.Control)
            {
                GrEditingReason reason = new GrEditingReason(e.GetLocation());

                e.SetNextState(GrGridState.ItemEditing);
                e.SetCell(m_pItem);
                e.SetUserData(reason);
            }

            this.GridCore.SetMouseUnpress();
            this.GridCore.Invalidate();

            m_mouseDownPoint = GrPoint.Empty;
        }

        public override GrGridState GetState()
        {
            return GrGridState.ItemButtonPressing;
        }
    }
}
