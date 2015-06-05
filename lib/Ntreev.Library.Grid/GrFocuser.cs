using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrFocuser : GrObject
    {
        IFocusable m_pFocus;
        GrItem m_pFocusItem;

        GrItem m_pLastFocusedItem;
        GrColumn m_pLastFocusedColumn;
        IDataRow m_pLastFocusedRow;

        public GrFocuser()
        {
            ResetVariables();
        }

        public void Reset()
        {
            Set(null as IFocusable);
        }
        public void Set(IFocusable pFocusable)
        {
            if (m_pFocus == pFocusable)
                return;

            GrFocusChangeArgs e = new GrFocusChangeArgs(pFocusable);
            OnFocusChanging(e);

            if (pFocusable != null)
            {
                m_pFocus = pFocusable;
                m_pLastFocusedRow = pFocusable.GetDataRow();
            }
            else
            {
                m_pFocus = null;
            }

            OnFocusChanged(e);
        }
        public void Set(GrColumn pColumn)
        {
            GrDataRow pDataRow = GetFocusableDataRow();
            if (pDataRow == null)
                return;

            GrItem pItem = pDataRow.GetItem(pColumn);
            Set(pItem);
        }
        public void Set(IDataRow pDataRow)
        {
            GrColumn pColumn = GetFocusableColumn();
            IFocusable pFocusable = null;
            if (pColumn != null)
                pFocusable = pDataRow.GetFocusable(pColumn);
            Set(pFocusable);
        }

        public GrColumn GetLastFocusedColumn()
        {
            return m_pLastFocusedColumn;
        }
        public IDataRow GetLastFocusedRow()
        {
            return m_pLastFocusedRow;
        }

        public GrColumn GetFocusedColumn()
        {
            if (m_pFocusItem == null)
                return null;
            return m_pFocusItem.GetColumn();
        }
        public IDataRow GetFocusedRow()
        {
            if (m_pFocus == null)
                return null;
            return m_pFocus.GetDataRow();
        }

        public IFocusable Get()
        {
            IFocusable pFocusing = GetFocusing();
            if (pFocusing != null)
                return pFocusing;
            return m_pFocus;
        }
        public GrItem GetItem()
        {
            return Get() as GrItem;
        }
        public bool Has()
        {
            return m_pFocus != null ? true : false;
        }
        public void Delete() { ResetVariables(); }


        public event FocusEventHandler FocusChanging;

        public event FocusEventHandler FocusChanged;


        protected virtual void OnFocusChanging(GrFocusChangeArgs e)
        {
            FocusChanging(this, e);
            Invalidate(m_pFocus);
        }
        protected virtual void OnFocusChanged(GrFocusChangeArgs e)
        {
            FocusChanged(this, e);

            GrItem pItem = m_pFocus as GrItem;
            if (pItem != null)
            {
                m_pLastFocusedColumn = pItem.GetColumn();
                m_pLastFocusedColumn.Invalidate();
                m_pLastFocusedItem = pItem;
            }

            Invalidate(e.GetFocusable());
        }
        protected virtual void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            this.GridCore.Cleared += gridCore_Cleared;
            this.GridCore.Created += gridCore_Created;
        }
        protected virtual IFocusable GetFocusing()
        {
            return null;
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            ResetVariables();
        }
        private void gridCore_Created(object sender, EventArgs e)
        {

        }

        private void ResetVariables()
        {
            m_pFocus = null;
            m_pFocusItem = null;

            m_pLastFocusedItem = null;
            m_pLastFocusedColumn = null;
            m_pLastFocusedRow = null;
        }
        private void Invalidate(IFocusable pFocusable)
        {
            if (pFocusable == null)
                return;

            GrRect rect = GrRect.Empty;
            IDataRow pDataRow = pFocusable.GetDataRow();
            if (this.GridCore.GetRowHighlight() == true || this.GridCore.GetFullRowSelect() == true)
            {
                rect = pDataRow.GetRect();
                rect.Width = this.GridCore.GetDisplayRect().Right - rect.Left;
                rect.Expand(2);
            }
            else
            {
                pFocusable.Invalidate();
                this.GridCore.Invalidate(pDataRow.GetRect());

                if (m_pLastFocusedColumn != null)
                    m_pLastFocusedColumn.Invalidate();
            }
            this.GridCore.Invalidate(rect);
        }

        private GrColumn GetFocusableColumn()
        {
            GrColumn pColumn = GetFocusedColumn();

            if (pColumn == null)
            {
                pColumn = m_pLastFocusedColumn;
            }

            if (pColumn == null)
            {
                GrColumnList pColumnList = this.GridCore.GetColumnList();
                if (pColumnList.GetVisibleColumnCount() == 0)
                    return null;
                pColumn = pColumnList.GetVisibleColumn(0);
            }

            return pColumn;
        }
        private GrDataRow GetFocusableDataRow()
        {
            IDataRow pDataRow = GetFocusedRow();

            if (pDataRow == null)
            {
                //GrItemSelector pItemSelector = this.GridCore.GetItemSelector();
                GrDataRowList pDataRowList = this.GridCore.GetDataRowList();

                GrDataRow pInsertionRow = pDataRowList.GetInsertionRow();
                if (pInsertionRow.GetVisible() == true)
                    return pInsertionRow;
                if (pDataRowList.GetVisibleDataRowCount() == 0)
                    return null;
                pDataRow = pDataRowList.GetVisibleDataRow(0);
            }

            return pDataRow as GrDataRow;
        }


    }
}
