using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    class GrFocuserInternal : GrFocuser
    {
        IFocusable m_pFocusing;

        public GrFocuserInternal()
        {

        }

        public void SetFocusing(IFocusable pFocusable)
        {
            if (m_pFocusing == pFocusable)
                return;

            if (m_pFocusing != null && m_pFocusing.GetDisplayable() == true)
            {
                IDataRow pDataRow = m_pFocusing.GetDataRow();
                if (this.GridCore.GetRowHighlight() == true || this.GridCore.GetFullRowSelect() == true)
                {
                    GrRect rect = pDataRow.GetRect();
                    rect.Width = this.GridCore.DisplayRectangle.Right - rect.Left;
                    rect.Expand(2);
                    this.GridCore.Invalidate(rect);
                }
                else
                {
                    m_pFocusing.Invalidate();
                }
            }

            m_pFocusing = pFocusable;

            if (pFocusable != null && pFocusable.GetDisplayable() == true)
            {
                pFocusable.Invalidate();
            }

            GrItemSelectorInternal pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
            pItemSelector.Selecting(pFocusable);
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.Created += gridCore_Created;
            this.GridCore.Cleared += gridCore_Cleared;
        }

        protected override IFocusable GetFocusing()
        {
            return m_pFocusing;
        }

        protected override void OnFocusChanging(GrFocusChangeArgs e)
        {
            if (this.GridCore.GetMultiSelect() == false)
            {
                IFocusable pFocusable = e.GetFocusable();
                if (pFocusable != null)
                {
                    GrDataRow pDataRow = pFocusable.GetDataRow() as GrDataRow;
                    GrItemSelectorInternal pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
                    if (pDataRow != null && pDataRow.GetFullSelected() == true)
                        pItemSelector.SelectDataRow(pDataRow, GrSelectionType.Normal);
                    else
                        pItemSelector.Select(pFocusable);

                }
            }

            base.OnFocusChanging(e);
        }

        private void itemSelector_SelectingEnd(object sender, EventArgs e)
        {
            if (m_pFocusing != null)
            {
                Set(m_pFocusing);
                m_pFocusing = null;
            }
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            m_pFocusing = null;
        }

        private void gridCore_Created(object sender, EventArgs e)
        {
            GrItemSelectorInternal pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
            pItemSelector.SelectingEnd += itemSelector_SelectingEnd;
        }
    }
}
