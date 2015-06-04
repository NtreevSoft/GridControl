using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    class GrItemSelectorInternal : GrItemSelector
    {
        private bool m_bSelecting;

        private GrRange m_columnSelecting;
        private GrRange m_rowSelecting;

        public GrItemSelectorInternal()
        {

        }

        public void BeginSelecting()
        {
            if (m_bSelecting == true)
                EndSelecting(GrSelectionType.Normal);

            m_bSelecting = true;
            if (this.GridCore.GetMultiSelect() == false)
                ClearSelection();
            OnSelectingBegin(EventArgs.Empty);
        }

        public void Selecting(GrRange visibleColumnRange, GrRange visibleRowRange)
        {
            if (m_bSelecting == false || this.GridCore.GetMultiSelect() == false)
                return;

            if (m_columnSelecting != visibleColumnRange || m_rowSelecting != visibleRowRange)
            {
                GrRect rect = GetInvalidateRect(m_columnSelecting, m_rowSelecting);
                if (rect.IsEmpty() == false)
                    this.GridCore.Invalidate(rect);

                rect = GetInvalidateRect(visibleColumnRange, visibleRowRange);
                if (rect.IsEmpty() == false)
                    this.GridCore.Invalidate(rect);
            }

            SetColumnSelectingRange(visibleColumnRange);
            SetRowSelectingRange(visibleRowRange);
        }

        public void EndSelecting(GrSelectionType selectionType)
        {
            if (m_bSelecting == false)
                return;

            HashSet<GrItem> items = new HashSet<GrItem>();
            GrColumnList pColumnList = this.GridCore.GetColumnList();
            GrDataRowList pDataRowList = this.GridCore.GetDataRowList();

            for (int y = m_rowSelecting.GetMinValue(); y < m_rowSelecting.GetMaxValue(); y++)
            {
                GrDataRow pDataRow = pDataRowList.GetVisibleRow(y) as GrDataRow;
                if (pDataRow == null)
                    continue;
                for (int x = m_columnSelecting.GetMinValue(); x < m_columnSelecting.GetMaxValue(); x++)
                {
                    GrColumn pColumn = pColumnList.GetVisibleColumn(x);
                    GrItem pItem = pDataRow.GetItem(pColumn);
                    if (pItem == null)
                        throw new Exception();
                    items.Add(pItem);
                }
            }

            SelectItems(items, selectionType);

            m_columnSelecting = GrRange.Empty;
            m_rowSelecting = GrRange.Empty;

            m_bSelecting = false;
            OnSelectingEnd(EventArgs.Empty);
        }

        public bool IsSelecting()
        {
            return m_bSelecting;
        }

        public bool IsSelecting(GrColumn pColumn)
        {
            if (m_bSelecting == false)
                return false;
            return m_columnSelecting.IsIn(pColumn.GetVisibleIndex());
        }

        public bool IsSelecting(GrDataRow pDataRow)
        {
            if (m_bSelecting == false)
                return false;
            return m_rowSelecting.IsIn(pDataRow.GetVisibleIndex());
        }

        public void SetColumnSelectingRange(GrRange range)
        {
            m_columnSelecting = range;
        }

        public void SetRowSelectingRange(GrRange range)
        {
            m_rowSelecting = range;
        }

        public GrRect GetInvalidateRect(GrRange visibleColumnRange, GrRange visibleRowRange)
        {
            GrColumnList pColumnList = this.GridCore.GetColumnList();
            GrDataRowList pDataRowList = this.GridCore.GetDataRowList();

            if (visibleColumnRange.GetLength() == 0 || visibleRowRange.GetLength() == 0)
                return GrRect.Empty;

            GrRect rect = GrRect.FromLTRB(int.MaxValue, int.MaxValue, int.MinValue, int.MinValue);

            for (int i = visibleColumnRange.GetMinValue(); i < visibleColumnRange.GetMaxValue(); i++)
            {
                GrColumn pColumn = pColumnList.GetVisibleColumn(i);
                if (pColumn.GetDisplayable() == false)
                    continue;
                GrRect displayRect = pColumn.GetRect();
                int left = Math.Min(rect.Left, displayRect.Left);
                int top = displayRect.Top;
                int right = Math.Max(rect.Right, displayRect.Right);
                int bottom = displayRect.Bottom;
                displayRect = GrRect.FromLTRB(left, top, right, bottom);
                //rect.Left = Math.Min(rect.Left, displayRect.Left);
                //rect.Right = Math.Max(rect.Right, displayRect.Right);
            }

            for (int y = visibleRowRange.GetMinValue(); y < visibleRowRange.GetMaxValue(); y++)
            {
                IDataRow pDataRow = pDataRowList.GetVisibleRow(y);
                if (pDataRow.GetDisplayable() == false)
                    continue;
                GrRect displayRect = pDataRow.GetRect();
                int left = displayRect.Left;
                int top = Math.Min(rect.Top, displayRect.Top);
                int right = displayRect.Right;
                int bottom = Math.Max(rect.Bottom, displayRect.Bottom);
                displayRect = GrRect.FromLTRB(left, top, right, bottom);
                //rect.Top = Math.Min(rect.Top, displayRect.Top);
                //rect.Bottom = Math.Max(rect.Bottom, displayRect.Bottom);
            }
            return rect;
        }

        public void Select(IFocusable pFocusable)
        {
            GrItem pFocusedItem = pFocusable as GrItem;
            if (pFocusedItem == null)
                return;

            BeginSelection();
            ClearSelection();

            if (pFocusedItem != null)
            {
                if (this.GridCore.GetFullRowSelect() == true)
                {
                    GrColumnList pColumnList = this.GridCore.GetColumnList();
                    GrDataRow pDataRow = pFocusedItem.GetDataRow();
                    for (int i = 0; i < pColumnList.GetVisibleColumnCount(); i++)
                    {
                        GrColumn pColumn = pColumnList.GetVisibleColumn(i);
                        GrItem pItem = pDataRow.GetItem(pColumn);
                        DoSelect(pItem);
                    }
                }
                else
                {
                    DoSelect(pFocusedItem);
                }
            }

            SetColumnSelectingRange(GrRange.Empty);
            SetRowSelectingRange(GrRange.Empty);

            EndSelection();
        }

        public void Selecting(IFocusable pFocusable)
        {
            if (this.GridCore.GetMultiSelect() == true)
                return;

            GrItem pItem = pFocusable as GrItem;
            if (pItem != null)
            {
                GrColumn pColumn = pItem.GetColumn();
                GrDataRow pDataRow = pItem.GetDataRow();

                int visibleColumnIndex = pColumn.GetVisibleIndex();
                int visibleRowIndex = pDataRow.GetVisibleIndex();

                if (this.GridCore.GetFullRowSelect() == true)
                {
                    GrColumnList pColumnList = this.GridCore.GetColumnList();
                    SetColumnSelectingRange(new GrRange(0, pColumnList.GetVisibleColumnCount()));
                }
                else
                {
                    SetColumnSelectingRange(new GrRange(visibleColumnIndex, visibleColumnIndex + 1));
                }

                SetRowSelectingRange(new GrRange(visibleRowIndex, visibleRowIndex + 1));
            }
            else
            {
                SetColumnSelectingRange(GrRange.Empty);
                SetRowSelectingRange(GrRange.Empty);
            }
        }

        public event EventHandler SelectingBegin;

        public event EventHandler SelectingEnd;

        protected virtual void OnSelectingBegin(EventArgs e)
        {
            SelectingBegin(this, e);
        }

        protected virtual void OnSelectingEnd(EventArgs e)
        {
            SelectingEnd(this, e);
        }

    }
}
