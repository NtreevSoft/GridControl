using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    class GrMousePresser : GrObject
    {
        private GrCell m_pMousePressed;

        public GrMousePresser()
        {
            m_pMousePressed = null;
        }

        public void SetMousePress(GrCell pCell)
        {
            if (m_pMousePressed != null)
                this.GridCore.Invalidate(m_pMousePressed.Bounds);
            if (pCell != null)
                this.GridCore.Invalidate(pCell.Bounds);

            m_pMousePressed = pCell;
        }

        public void SetMouseUnpress()
        {
            if (m_pMousePressed != null)
                this.GridCore.Invalidate(m_pMousePressed.Bounds);
            m_pMousePressed = null;
        }

        public GrCell GetMousePress()
        {
            return m_pMousePressed;
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.Cleared += gridCore_Cleared;
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            m_pMousePressed = null;
        }
    }
}
