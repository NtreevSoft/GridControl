using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Ntreev.Library.Grid
{
    public class GrItemSelector : GrObject
    {
        //HashSet<GrItem> m_selectedItems;
        HashSet<GrColumn> m_selectedColumns = new HashSet<GrColumn>();
        HashSet<GrDataRow> m_selectedRows = new HashSet<GrDataRow>();

        //HashSet<GrItem> m_oldSelectedItems;
        HashSet<GrColumn> m_oldSelectedColumns = new HashSet<GrColumn>();
        HashSet<GrDataRow> m_oldSelectedRows = new HashSet<GrDataRow>();


        List<GrColumn> m_externalSelectedColumns = new List<GrColumn>();
        List<GrDataRow> m_externalSelectedRows = new List<GrDataRow>();

        int m_selectionLock;
        IDataRow m_pSelectionGroup;
        //int m_fullSelectedCount;
        //uint m_selectionGroup;

        GrRange m_columnSelecting;
        GrRange m_rowSelecting;

        GrColumn m_pAnchorColumn;
        IDataRow m_pAnchorDataRow;

        GrRect m_invalidate;

        public GrItemSelector()
        {

        }

        public void SelectItem(GrItem pItem, GrSelectionType selectType)
        {
            if (pItem == null)
                throw new Exception("item is null");

            HashSet<GrItem> items = new HashSet<GrItem>();
            items.Add(pItem);
            SelectItems(items, selectType);
        }

        public void SelectItems(HashSet<GrItem> pItems, GrSelectionType selectType)
        {
            if (this.GridCore.GetMultiSelect() == false)
                return;

            BeginSelection();
            switch (selectType)
            {
                case GrSelectionType.Normal:
                    {
                        ClearSelectionCore();

                        foreach (var value in pItems)
                        {
                            DoSelect(value);
                        }
                    }
                    break;
                case GrSelectionType.Add:
                    {
                        foreach (var value in pItems)
                        {
                            DoSelect(value);
                        }
                    }
                    break;
                case GrSelectionType.Remove:
                    {
                        foreach (var value in pItems)
                        {
                            DoDeselect(value);
                        }
                    }
                    break;
            }
            EndSelection();
        }

        public void SelectItems(GrItem pBegin, GrItem pEnd, GrSelectionType selectType)
        {
            GrColumnList columnList = this.GridCore.ColumnList;
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            HashSet<GrItem> items = new HashSet<GrItem>();

            int beginColumn, endColumn, beginRow, endRow;

            beginColumn = pBegin.GetColumn().GetVisibleIndex();
            endColumn = pEnd.GetColumn().GetVisibleIndex();

            beginRow = pBegin.GetDataRow().GetVisibleDataRowIndex();
            endRow = pEnd.GetDataRow().GetVisibleDataRowIndex();

            if (beginColumn > endColumn)
            {
                beginColumn = Interlocked.Exchange(ref endColumn, beginColumn);
                //std::swap(beginColumn, endColumn);
            }

            if (beginRow > endRow)
            {
                beginRow = Interlocked.Exchange(ref endRow, beginRow);
                //std::swap(beginRow, endRow);
            }

            for (int i = beginRow; i <= endRow; i++)
            {
                GrDataRow pDataRow = dataRowList.GetVisibleDataRow(i);
                for (int j = beginColumn; j <= endColumn; j++)
                {
                    GrColumn column = columnList.GetVisibleColumn(j);
                    GrItem pItem = pDataRow.GetItem(column);
                    items.Add(pItem);
                }
            }
            SelectItems(items, selectType);
        }

        public void SelectItems(GrRange visibleColumnIndex, GrRange visibleRowIndex, GrSelectionType selectType)
        {
            GrColumnList columnList = this.GridCore.ColumnList;
            GrDataRowList dataRowList = this.GridCore.DataRowList;

            HashSet<GrItem> items = new HashSet<GrItem>();
            for (int y = visibleRowIndex.Minimum; y < visibleRowIndex.Maximum; y++)
            {
                GrDataRow pDataRow = dataRowList.GetVisibleRow(y) as GrDataRow;
                if (pDataRow == null)
                    continue;

                for (int x = visibleColumnIndex.Minimum; x < visibleColumnIndex.Maximum; x++)
                {
                    GrColumn column = columnList.GetVisibleColumn(x);
                    GrItem pItem = pDataRow.GetItem(column);
                    items.Add(pItem);
                }
            }
            SelectItems(items, selectType);
        }

        public void SelectColumn(GrColumn column, GrSelectionType selectType)
        {
            HashSet<GrColumn> dataColumns = new HashSet<GrColumn>();
            if (column != null)
                dataColumns.Add(column);
            SelectColumns(dataColumns, selectType);
        }

        public void SelectDataRow(GrDataRow pDataRow, GrSelectionType selectType)
        {
            HashSet<GrDataRow> dataRows = new HashSet<GrDataRow>();
            if (pDataRow != null)
                dataRows.Add(pDataRow);
            SelectDataRows(dataRows, selectType);
        }

        public void SelectColumns(HashSet<GrColumn> columns, GrSelectionType selectType)
        {
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            GrDataRow pInsertionRow = dataRowList.GetInsertionRow();
            HashSet<GrItem> items = new HashSet<GrItem>();
            foreach (var value in columns)
            {
                if ((m_pSelectionGroup == null || m_pSelectionGroup == pInsertionRow) && pInsertionRow.GetVisible() == true)
                {
                    GrItem pItem = dataRowList.GetInsertionRow().GetItem(value);
                    items.Add(pItem);
                }
                else
                {
                    for (int i = 0; i < dataRowList.GetDataRowCount(); i++)
                    {
                        GrDataRow pDataRow = dataRowList.GetDataRow(i);
                        GrItem pItem = pDataRow.GetItem(value);
                        items.Add(pItem);
                    }
                }
            }

            SelectItems(items, selectType);
        }

        public void SelectColumns(GrColumn pFrom, GrColumn pTo, GrSelectionType selectType)
        {
            GrRange indexRange = new GrRange(pFrom.GetVisibleIndex(), pTo.GetVisibleIndex());

            HashSet<GrColumn> dataRows = new HashSet<GrColumn>();
            GrColumnList columnList = this.GridCore.ColumnList;
            for (int i = indexRange.Minimum; i <= indexRange.Maximum; i++)
            {
                GrColumn column = columnList.GetVisibleColumn(i) as GrColumn;
                if (column != null)
                    dataRows.Add(column);
            }

            SelectColumns(dataRows, selectType);
        }

        public void SelectDataRows(HashSet<GrDataRow> pDataRows, GrSelectionType selectType)
        {
            BeginSelection();
            switch (selectType)
            {
                case GrSelectionType.Normal:
                    {
                        ClearSelectionCore();

                        foreach (var value in pDataRows)
                        {
                            DoSelectDataRow(value);
                        }
                    }
                    break;
                case GrSelectionType.Add:
                    {
                        foreach (var value in pDataRows)
                        {
                            DoSelectDataRow(value);
                        }
                    }
                    break;
                case GrSelectionType.Remove:
                    {
                        foreach (var value in pDataRows)
                        {
                            DoDeselectDataRow(value);
                        }
                    }
                    break;
            }
            EndSelection();
        }

        public void SelectDataRows(GrDataRow pFrom, GrDataRow pTo, GrSelectionType selectType)
        {
            if (pFrom.GetSelectionGroup() != pTo.GetSelectionGroup())
                throw new Exception("");

            GrRange indexRange = new GrRange(pFrom.GetVisibleDataRowIndex(), pTo.GetVisibleDataRowIndex());

            HashSet<GrDataRow> dataRows = new HashSet<GrDataRow>();
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            for (int i = indexRange.Minimum; i <= indexRange.Maximum; i++)
            {
                GrDataRow pDataRow = dataRowList.GetVisibleDataRow(i);
                dataRows.Add(pDataRow);
            }

            SelectDataRows(dataRows, selectType);
        }

        public void SelectDataRows(IDataRow pFrom, IDataRow pTo, GrSelectionType selectType)
        {
            if (pFrom.GetSelectionGroup() != pTo.GetSelectionGroup())
                throw new Exception("");

            GrRange indexRange = new GrRange(pFrom.GetVisibleIndex(), pTo.GetVisibleIndex());

            HashSet<GrDataRow> dataRows = new HashSet<GrDataRow>();
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            for (int i = indexRange.Minimum; i <= indexRange.Maximum; i++)
            {
                GrDataRow pDataRow = dataRowList.GetVisibleRow(i) as GrDataRow;
                if (pDataRow != null)
                    dataRows.Add(pDataRow);
            }

            SelectDataRows(dataRows, selectType);
        }

        public void SelectAll()
        {
            BeginSelection();
            ClearSelection();
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            HashSet<GrDataRow> dataRows = new HashSet<GrDataRow>();
            for (int i = 0; i < dataRowList.GetDataRowCount(); i++)
            {
                GrDataRow pDataRow = dataRowList.GetDataRow(i);
                dataRows.Add(pDataRow);
            }
            SelectDataRows(dataRows, GrSelectionType.Add);
            this.GridCore.Invalidate();
            EndSelection();
        }

        public void ClearSelection()
        {
            ClearSelectionCore();
            m_pSelectionGroup = null;
        }

        public List<GrColumn> GetSelectedColumns()
        {
            return m_externalSelectedColumns;
        }

        public List<GrDataRow> GetSelectedRows()
        {
            return m_externalSelectedRows;
        }


        //void LockSelectionUpdate();
        //void UnlockSelectionUpdate();

        public bool CanSelect(IDataRow pDataRow)
        {
            if (m_pSelectionGroup == null)
                return true;
            return pDataRow.GetSelectionGroup() == m_pSelectionGroup.GetSelectionGroup();
        }

        public bool CanSelect(GrItem pItem)
        {
            return CanSelect(pItem.GetDataRow());
        }

        public IDataRow GetSelectionGroup()
        {
            return m_pSelectionGroup;
        }

        public void SetSelectionGroup(IDataRow pDataRow)
        {
            m_pSelectionGroup = pDataRow;
        }

        public void SetSelectionGroup(GrItem pItem)
        {
            SetSelectionGroup(pItem.GetDataRow());
        }

        // anchor
        public void SetAnchor(GrItem pItem)
        {
            SetColumnAnchor(pItem);
            SetRowAnchor(pItem);
        }

        public void SetColumnAnchor(GrItem pItem)
        {
            SetColumnAnchor(pItem.GetColumn());
        }

        public void SetColumnAnchor(GrColumn column)
        {
            m_pAnchorColumn = column;
        }

        public void SetRowAnchor(GrItem pItem)
        {
            SetRowAnchor(pItem.GetDataRow());
        }

        public void SetRowAnchor(IDataRow pDataRow)
        {
            m_pAnchorDataRow = pDataRow;
        }

        public GrColumn GetColumnAnchor()
        {
            if (m_pAnchorColumn == null)
            {
                GrColumnList columnList = this.GridCore.ColumnList;
                int visibleColumnCount = columnList.GetVisibleColumnCount();
                if (visibleColumnCount == 0)
                    return null;

                return columnList.GetVisibleColumn(0);
            }
            return m_pAnchorColumn;
        }

        public IDataRow GetRowAnchor()
        {
            if (m_pAnchorDataRow == null)
            {
                GrDataRowList dataRowList = this.GridCore.DataRowList;
                int visibleRowCount = dataRowList.GetVisibleRowCount();
                if (visibleRowCount == 0)
                    throw new Exception("");

                return dataRowList.GetVisibleRow(0);
            }
            return m_pAnchorDataRow;
        }

        public GrRange GetColumnSelections(GrItem pItem)
        {
            return GetColumnSelections(pItem.GetColumn());
        }

        public GrRange GetColumnSelections(GrColumn column)
        {
            GrRange range = new GrRange(m_pAnchorColumn.GetVisibleIndex(), column.GetVisibleIndex());
            range.Maximum = range.Maximum + 1;
            return range;
        }

        public GrRange GetRowSelections(GrItem pItem)
        {
            return GetRowSelections(pItem.GetDataRow());
        }

        public GrRange GetRowSelections(IDataRow pDataRow)
        {
            if (pDataRow.GetSelectionGroup() != m_pAnchorDataRow.GetSelectionGroup())
                return GrRange.Empty;

            GrRange range = new GrRange(m_pAnchorDataRow.GetVisibleIndex(), pDataRow.GetVisibleIndex());
            range.Maximum = range.Maximum + 1;
            return range;
        }

        public event EventHandler SelectionChanged;

        public event EventHandler SelectedRowsChanged;

        public event EventHandler SelectedColumnsChanged;

        protected virtual void OnSelectionChanged(EventArgs e)
        {
            SelectionChanged(this, e);
        }

        protected virtual void OnSelectedColumnsChanged(EventArgs e)
        {
            List<GrColumn> selections = m_externalSelectedColumns;
            selections.Clear();
            selections.Capacity = m_selectedColumns.Count;
            selections.AddRange(m_selectedColumns);
            selections.Sort((x, y) => x.GetVisibleIndex().CompareTo(y.GetVisibleIndex()));
            SelectedColumnsChanged(this, e);
        }

        protected virtual void OnSelectedRowsChanged(EventArgs e)
        {
            List<GrDataRow> selections = m_externalSelectedRows;
            selections.Clear();
            selections.Capacity = m_selectedRows.Count;
            selections.AddRange(m_selectedRows);
            selections.Sort((x, y) => x.GetVisibleIndex().CompareTo(y.GetVisibleIndex()));
            SelectedRowsChanged(this, e);
        }

        protected override void OnGridCoreAttached()
        {
            this.GridCore.Cleared += gridCore_Cleared;
            this.GridCore.Created += gridCore_Created;
        }

        protected void DoSelect(GrItem pItem)
        {
            if (pItem.m_selected == true)
                return;

            GrColumn column = pItem.GetColumn();
            GrDataRow pDataRow = pItem.GetDataRow();
            if (CanSelect(pDataRow) == false)
                return;

            pItem.m_selected = true;
            if (pDataRow.m_selected == 0)
                m_selectedRows.Add(pDataRow);
            if (column.m_selected == 0)
                m_selectedColumns.Add(column);

            pDataRow.m_selected++;
            column.m_selected++;

            if (pItem.GetDisplayable() == true)
                AddInvalidatedRectangle(pItem.GetDisplayRect());
        }

        protected void DoDeselect(GrItem pItem)
        {
            if (pItem.m_selected == false)
                return;

            GrColumn column = pItem.GetColumn();
            GrDataRow pDataRow = pItem.GetDataRow();

            pItem.m_selected = false;
            pDataRow.m_selected--;
            column.m_selected--;

            if (pDataRow.m_selected < 0)
                throw new Exception();
            if (column.m_selected < 0)
                throw new Exception();

            if (pDataRow.m_selected == 0)
                m_selectedRows.Remove(pDataRow);
            if (column.m_selected == 0)
                m_selectedColumns.Remove(column);

            if (pItem.GetDisplayable() == true)
                AddInvalidatedRectangle(pItem.GetDisplayRect());
        }

        protected void BeginSelection()
        {
            if (m_selectionLock == 0)
            {
                m_oldSelectedColumns = m_selectedColumns;
                m_oldSelectedRows = m_selectedRows;

                m_invalidate.DoEmpty();
            }
            m_selectionLock++;
        }

        protected void EndSelection()
        {
            m_selectionLock--;
            if (m_selectionLock == 0)
            {
                bool changed = false;
                if (m_oldSelectedColumns != m_selectedColumns)
                {
                    OnSelectedColumnsChanged(EventArgs.Empty);
                    changed = true;
                }

                if (m_oldSelectedRows != m_selectedRows)
                {
                    OnSelectedRowsChanged(EventArgs.Empty);
                    changed = true;
                }

                if (changed == true)
                {
                    OnSelectionChanged(EventArgs.Empty);
                }

                this.GridCore.Invalidate(m_invalidate);
            }
        }

        protected void DoSelectDataRow(GrDataRow pDataRow)
        {
            if (pDataRow.GetFullSelected() == true)
                return;

            if (CanSelect(pDataRow) == false)
                return;

            GrColumnList columnList = this.GridCore.ColumnList;
            for (int i = 0; i < columnList.GetVisibleColumnCount(); i++)
            {
                GrColumn column = columnList.GetVisibleColumn(i);
                GrItem pItem = pDataRow.GetItem(column);
                if (pItem.m_selected == true)
                    continue;
                pItem.m_selected = true;
                pDataRow.m_selected++;
                if (column.m_selected == 0)
                    m_selectedColumns.Add(column);
                column.m_selected++;
            }

            m_selectedRows.Add(pDataRow);

            if (pDataRow.GetDisplayable() == true)
            {
                GrRect dataRowRect = pDataRow.GetRect();
                dataRowRect.Width = this.GridCore.DisplayRectangle.Right - dataRowRect.Left;
                AddInvalidatedRectangle(dataRowRect);
            }
        }

        protected void DoDeselectDataRow(GrDataRow pDataRow)
        {
            if (pDataRow.GetSelected() == false)
                return;

            GrColumnList columnList = this.GridCore.ColumnList;
            for (int i = 0; i < columnList.GetVisibleColumnCount(); i++)
            {
                GrColumn column = columnList.GetVisibleColumn(i);
                GrItem pItem = pDataRow.GetItem(column);
                if (pItem.m_selected == false)
                    continue;
                pItem.m_selected = false;
                pDataRow.m_selected--;
                column.m_selected--;
                if (column.m_selected < 0)
                    throw new Exception();
                if (column.m_selected == 0)
                    m_selectedColumns.Remove(column);

            }
            if (pDataRow.m_selected != 0)
                throw new Exception();

            m_selectedRows.Remove(pDataRow);

            if (pDataRow.GetDisplayable() == true)
            {
                GrRect dataRowRect = pDataRow.GetRect();
                int left = dataRowRect.Left;
                int top = dataRowRect.Top;
                int right = this.GridCore.DisplayRectangle.Right;
                int bottom = dataRowRect.Bottom;

                //dataRowRect.Right = this.GridCore.GetDisplayRect().Right;
                dataRowRect = GrRect.FromLTRB(left, top, right, bottom);
                AddInvalidatedRectangle(dataRowRect);
            }
        }

        private void AddInvalidatedRectangle(GrRect rect)
        {
            if (m_invalidate.IsEmpty() == true)
            {
                m_invalidate = rect;
            }
            else
            {
                int left = Math.Min(m_invalidate.Left, rect.Left);
                int top = Math.Min(m_invalidate.Top, rect.Top);
                int right = Math.Max(m_invalidate.Right, rect.Right);
                int bottom = Math.Max(m_invalidate.Bottom, rect.Bottom);
                m_invalidate = GrRect.FromLTRB(left, top, right, bottom);
            }
        }

        private void ResetVariables()
        {
            //m_selectedItems.Clear();
            m_selectedColumns.Clear();
            m_selectedRows.Clear();

            m_selectionLock = 0;
            m_pSelectionGroup = null;

            m_pAnchorColumn = null;
            m_pAnchorDataRow = null;
        }

        private void ClearSelectionCore()
        {
            BeginSelection();

            HashSet<GrDataRow> selectedRows = new HashSet<GrDataRow>(m_selectedRows);
            foreach (var value in selectedRows)
            {
                DoDeselectDataRow(value);
            }

            foreach (var value in m_selectedColumns)
            {
                value.m_selected = 0;
            }
            m_selectedColumns.Clear();
            m_selectedRows.Clear();

            EndSelection();
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            ResetVariables();
        }

        private void gridCore_Created(object sender, EventArgs e)
        {
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            dataRowList.VisibleChanged += dataRowList_RowVisibleChanged;
        }

        private void dataRowList_RowVisibleChanged(object sender, EventArgs e)
        {
            OnSelectedRowsChanged(EventArgs.Empty);
        }


        //class SortSelections<T>
        //{
        //public static bool operator () ( T p1,  T p2)
        //    {

        //        //return p1.GetVisibleIndex() < p2.GetVisibleIndex();
        //    }
        //};
    }
}
