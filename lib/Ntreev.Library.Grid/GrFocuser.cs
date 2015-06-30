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
        public void Set(GrColumn column)
        {
            GrDataRow pDataRow = GetFocusableDataRow();
            if (pDataRow == null)
                return;

            GrItem pItem = pDataRow.GetItem(column);
            Set(pItem);
        }
        public void Set(IDataRow pDataRow)
        {
            GrColumn column = GetFocusableColumn();
            IFocusable pFocusable = null;
            if (column != null)
                pFocusable = pDataRow.GetFocusable(column);
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


        public event GrFocusEventHandler FocusChanging;

        public event GrFocusEventHandler FocusChanged;


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
        protected override void OnGridCoreAttached()
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
                rect = pDataRow.Bounds;
                rect.Width = this.GridCore.DisplayRectangle.Right - rect.Left;
                rect.Expand(2);
            }
            else
            {
                pFocusable.Invalidate();
                this.GridCore.Invalidate(pDataRow.Bounds);

                if (m_pLastFocusedColumn != null)
                    m_pLastFocusedColumn.Invalidate();
            }
            this.GridCore.Invalidate(rect);
        }

        private GrColumn GetFocusableColumn()
        {
            GrColumn column = GetFocusedColumn();

            if (column == null)
            {
                column = m_pLastFocusedColumn;
            }

            if (column == null)
            {
                GrColumnList columnList = this.GridCore.ColumnList;
                if (columnList.GetVisibleColumnCount() == 0)
                    return null;
                column = columnList.GetVisibleColumn(0);
            }

            return column;
        }
        private GrDataRow GetFocusableDataRow()
        {
            IDataRow pDataRow = GetFocusedRow();

            if (pDataRow == null)
            {
                //GrItemSelector pItemSelector = this.GridCore.GetItemSelector();
                GrDataRowList dataRowList = this.GridCore.DataRowList;

                GrDataRow pInsertionRow = dataRowList.InsertionRow;
                if (pInsertionRow.IsVisible == true)
                    return pInsertionRow;
                if (dataRowList.GetVisibleDataRowCount() == 0)
                    return null;
                pDataRow = dataRowList.GetVisibleDataRow(0);
            }

            return pDataRow as GrDataRow;
        }


    }
}
