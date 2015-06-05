using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    public abstract class GridObject
    {
        GridControl m_gridControl;

        protected GridObject()
        {

        }

        protected GridObject(GridControl gridControl)
        {
            m_gridControl = gridControl;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public GridControl GridControl
        {
            get { return m_gridControl; }
            internal set
            {
                if (value != null)
                {
                    m_gridControl = value;
                    //m_pGridCore = gridControl.GridCore;
                    //m_pGridPainter = gridControl.GridPainter;
                    //m_pItemSelector = m_pGridCore.GetItemSelector();
                    //m_pFocuser = m_pGridCore.GetFocuser();

                    OnGridControlAttachedInternal();
                }
                else
                {
                    OnGridControlDetachedInternal();

                    m_gridControl = null;
                    //m_pGridCore = null;
                    //m_pGridPainter = null;
                    //m_pItemSelector = null;
                    //m_pFocuser = null;
                }
            }
        }

        internal virtual void OnGridControlAttachedInternal()
        {

        }

        internal virtual void OnGridControlDetachedInternal()
        {

        }

        internal GridObject(GrCell pCell)
        {
            WinFormGridCore pGridCore = pCell.GetGridCore() as WinFormGridCore;
            if (pGridCore != null)
            {
                m_gridControl = pGridCore.m_gridControl;
                //m_pGridCore = pGridCore; 
                //m_pGridPainter = m_gridControl.GridPainter;
                //m_pItemSelector = m_pGridCore.GetItemSelector();
                //m_pFocuser = m_pGridCore.GetFocuser();
            }
        }

        internal GrGridCore GridCore
        {
            get { return m_gridControl.GridCore; }
        }

        internal GrItemSelector Selector
        {
            get { return m_gridControl.GridCore.GetItemSelector(); }
        }

        internal GrFocuser Focuser
        {
            get { return m_gridControl.GridCore.GetFocuser(); }
        }
    }
}
