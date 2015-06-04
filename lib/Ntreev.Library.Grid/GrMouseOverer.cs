using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    class GrMouseOverer : GrObject
    {
        private GrCell m_pMouseOvered;
        private int m_mouseOverState;

        public GrMouseOverer()
        {
            m_pMouseOvered = null;
        }

        public bool SetMouseOver(GrCell pCell, GrPoint localLocation)
        {
            bool success = false;
            if (m_pMouseOvered != pCell)
                success = true;

            if (success == true)
            {
                if (m_pMouseOvered != null)
                    this.GridCore.Invalidate(m_pMouseOvered.GetRect());
                if (pCell != null)
                    this.GridCore.Invalidate(pCell.GetRect());
            }

            m_pMouseOvered = pCell;

            int state;
            if (m_pMouseOvered != null)
                state = m_pMouseOvered.HitMouseOverTest(localLocation);
            else
                state = 0;

            if (m_mouseOverState != state)
            {
                success = true;
                if (m_pMouseOvered != null)
                    this.GridCore.Invalidate(m_pMouseOvered.GetRect());
            }
            m_mouseOverState = state;
            return success;
        }

        public GrCell GetMouseOver()
        {
            return m_pMouseOvered;
        }

        public int GetMouseOverState()
        {
            return m_mouseOverState;
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.Cleared += gridCore_Cleared;
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            m_pMouseOvered = null;
            m_mouseOverState = 0;
        }
    }
}
