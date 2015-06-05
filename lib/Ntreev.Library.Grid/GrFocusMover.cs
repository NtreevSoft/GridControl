using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrFocusMover : GrObject
    {
        GrDataRowList m_pDataRowList;
        GrColumnList m_pColumnList;
        GrColumn m_pLastDataColumn;
        GrScroll m_pHorzScroll;
        GrScroll m_pVertScroll;
        GrFocuser m_pFocuser;
        GrItemSelector m_pItemSelector;
        GrGridWindow m_pGridWindow;

        public GrFocusMover()
        {

        }

        public bool FirstCell(GrSelectionRange range)
        {
            GrItem pFocused = m_pFocuser.GetItem();
            if (pFocused == null)
                return false;

            GrColumn pColumn = pFocused.GetColumn();
            GrDataRow pDataRow = pFocused.GetDataRow();

            if (pColumn.GetFrozen() == true)
            {
                int index = pColumn.GetFrozenIndex();
                if (index == 0)
                    return false;

                index = index - 1;
                pColumn = m_pColumnList.GetFrozenColumn(0);
            }
            else
            {
                int frozenColumnCount = m_pColumnList.GetFrozenColumnCount();

                int index = pColumn.GetUnfrozenIndex();
                if (index == 0 && frozenColumnCount > 0)
                {
                    pColumn = m_pColumnList.GetFrozenColumn(0);
                }
                else
                {
                    pColumn = m_pColumnList.GetUnfrozenColumn(0);
                }
            }

            pFocused = pDataRow.GetItem(pColumn);

            if (range == GrSelectionRange.One)
            {
                m_pItemSelector.SetColumnAnchor(pColumn);
                m_pItemSelector.SelectItem(pFocused, m_pGridWindow.GetSelectionType());
            }
            else
            {
                m_pItemSelector.SelectItems(
                    m_pItemSelector.GetColumnSelections(pFocused),
                    m_pItemSelector.GetRowSelections(pFocused),
                    m_pGridWindow.GetSelectionType());
            }
            m_pFocuser.Set(pFocused);
            DoHorzScroll(GrScrollEventType.First);
            BringIntoView(pColumn);
            return true;
        }

        public bool LastCell(GrSelectionRange range)
        {
            GrItem pFocused = m_pFocuser.GetItem();
            if (pFocused == null)
                return false;

            GrColumn pColumn = pFocused.GetColumn();
            GrDataRow pDataRow = pFocused.GetDataRow();

            int visibleColumnCount = m_pColumnList.GetVisibleColumnCount();
            int index = visibleColumnCount - 1;

            if (index == pColumn.GetVisibleIndex())
                return false;

            pColumn = m_pColumnList.GetVisibleColumn(index);
            pFocused = pDataRow.GetItem(pColumn);

            if (range == GrSelectionRange.One)
            {
                m_pItemSelector.SelectItem(pFocused, m_pGridWindow.GetSelectionType());
                m_pItemSelector.SetAnchor(pFocused);
            }
            else
            {
                m_pItemSelector.SelectItems(
                    m_pItemSelector.GetColumnSelections(pFocused),
                    m_pItemSelector.GetRowSelections(pFocused),
                    m_pGridWindow.GetSelectionType());
            }
            m_pFocuser.Set(pFocused);

            DoHorzScroll(GrScrollEventType.Last);
            BringIntoView(pColumn);
            return true;
        }

        public bool PageUp(GrSelectionRange range)
        {
            IFocusable pFocused = m_pFocuser.Get();
            if (pFocused == null)
                return false;
            if (m_pDataRowList.GetVisibleRowCount() == 0)
                return false;

            IDataRow pDataRow = pFocused.GetDataRow();
            int index = pDataRow.GetVisibleIndex();

            switch (index)
            {
                case 0:
                    if (this.GridCore.GetInsertionRowVisible() == true)
                        index = GrDefineUtility.INSERTION_ROW;
                    else
                        return false;
                    break;
                case -8://GrDefineUtility.INSERTION_ROW:
                    return false;
                default:
                    index = m_pDataRowList.ClipTo(index);
                    break;
            }

            IDataRow pNewDataRow = m_pDataRowList.GetVisibleRow(index);

            if (range == GrSelectionRange.One)
                SelectOne(pNewDataRow);
            else
                SelectMulti(pDataRow, pNewDataRow);

            if (pDataRow.GetDisplayable() == true)
                DoVertScroll(GrScrollEventType.LargeDecrement);
            else
                BringIntoView(pNewDataRow);
            return true;
        }

        public bool PageDown(GrSelectionRange range)
        {
            IFocusable pFocused = m_pFocuser.Get();
            if (pFocused == null)
                return false;
            if (m_pDataRowList.GetVisibleRowCount() == 0)
                return false;

            IDataRow pDataRow = pFocused.GetDataRow();
            int index = pDataRow.GetVisibleIndex();
            int newIndex;

            if (index == GrDefineUtility.INSERTION_ROW)
                newIndex = 0;
            else
                newIndex = m_pDataRowList.ClipFrom(index);

            if (newIndex == index)
            {
                if (newIndex + 1 != m_pDataRowList.GetVisibleRowCount())
                    newIndex++;
            }

            IDataRow pNewDataRow = m_pDataRowList.GetVisibleRow(newIndex);

            if (range == GrSelectionRange.One)
                SelectOne(pNewDataRow);
            else
                SelectMulti(pDataRow, pNewDataRow);

            if (pDataRow != this.GridCore.GetInsertionRow() && pDataRow.GetDisplayable() == true)
                DoVertScroll(GrScrollEventType.LargeIncrement);
            else
                BringIntoView(pNewDataRow);
            return true;
        }

        public bool FirstRow(GrSelectionRange range)
        {
            IFocusable pFocused = m_pFocuser.Get();
            if (pFocused == null)
                return false;
            if (m_pDataRowList.GetVisibleRowCount() == 0)
                return false;

            IDataRow pDataRow = pFocused.GetDataRow();
            int newIndex = 0;

            IDataRow pNewDataRow = m_pDataRowList.GetVisibleRow(newIndex);

            if (range == GrSelectionRange.One)
                SelectOne(pNewDataRow);
            else
                SelectMulti(pDataRow, pNewDataRow);

            DoVertScroll(GrScrollEventType.First);
            return true;
        }

        public bool LastRow(GrSelectionRange range)
        {
            IFocusable pFocused = m_pFocuser.Get();
            if (pFocused == null)
                return false;
            if (m_pDataRowList.GetVisibleRowCount() == 0)
                return false;

            IDataRow pDataRow = pFocused.GetDataRow();
            int newIndex = m_pDataRowList.GetVisibleRowCount() - 1;

            IDataRow pNewDataRow = m_pDataRowList.GetVisibleRow(newIndex);

            if (range == GrSelectionRange.One)
                SelectOne(pNewDataRow);
            else
                SelectMulti(pDataRow, pNewDataRow);

            DoVertScroll(GrScrollEventType.Last);
            return true;
        }

        public bool MoveLeft(GrSelectionRange range)
        {
            IFocusable pFocused = m_pFocuser.Get();
            if (pFocused == null)
                return false;

            if (m_pDataRowList.GetVisibleRowCount() == 0)
                return false;

            IDataRow pDataRow = pFocused.GetDataRow();
            int index = pDataRow.GetVisibleIndex();
            int lastIndex = m_pDataRowList.GetVisibleRowCount() - 1;

            if (index == lastIndex)
                return false;
            else if (index == GrDefineUtility.INSERTION_ROW)
                index = 0;
            else
                index = index + 1;

            IDataRow pNewDataRow = m_pDataRowList.GetVisibleRow(index);

            if (range == GrSelectionRange.One)
                SelectOne(pNewDataRow);
            else
                SelectMulti(pDataRow, pNewDataRow);

            BringIntoView(pNewDataRow);
            return true;
        }

        public bool MoveUp(GrSelectionRange range)
        {
            IFocusable pFocused = m_pFocuser.Get();
            if (pFocused == null)
                return false;
            if (m_pDataRowList.GetVisibleRowCount() == 0)
                return false;

            IDataRow pDataRow = pFocused.GetDataRow();
            int index = pDataRow.GetVisibleIndex();

            switch (index)
            {
                case 0:
                    if (this.GridCore.GetInsertionRowVisible() == true)
                        index = GrDefineUtility.INSERTION_ROW;
                    else
                        return false;
                    break;
                case -8://GrDefineUtility.INSERTION_ROW:
                    return false;
                default:
                    index = index - 1;
                    break;
            }

            IDataRow pNewDataRow = m_pDataRowList.GetVisibleRow(index);

            if (range == GrSelectionRange.One)
                SelectOne(pNewDataRow);
            else
                SelectMulti(pDataRow, pNewDataRow);

            BringIntoView(pNewDataRow);
            return true;
        }

        public bool MoveRight(GrSelectionRange range)
        {
            GrItem pFocused = m_pFocuser.GetItem();
            if (pFocused == null)
                return false;

            GrColumn pColumn = pFocused.GetColumn();
            GrDataRow pDataRow = pFocused.GetDataRow();

            int visibleColumnCount = m_pColumnList.GetVisibleColumnCount();
            int index = pColumn.GetVisibleIndex() + 1;
            if (index >= visibleColumnCount)
                index = visibleColumnCount - 1;

            if (index == pColumn.GetVisibleIndex())
                return false;

            pColumn = m_pColumnList.GetVisibleColumn(index);
            pFocused = pDataRow.GetItem(pColumn);

            if (range == GrSelectionRange.One)
            {
                if (this.GridCore.GetFullRowSelect() == true)
                    m_pItemSelector.SelectDataRow(pDataRow, GrSelectionType.Normal);
                else
                    m_pItemSelector.SelectItem(pFocused, GrSelectionType.Normal);
                m_pItemSelector.SetAnchor(pFocused);
            }
            else
            {
                m_pItemSelector.SelectItems(
                    m_pItemSelector.GetColumnSelections(pFocused),
                    m_pItemSelector.GetRowSelections(pFocused),
                    m_pGridWindow.GetSelectionType());
            }
            m_pFocuser.Set(pFocused);

            BringIntoView(pColumn);
            return true;
        }

        public bool MoveDown(GrSelectionRange range)
        {
            GrItem pFocused = m_pFocuser.GetItem();
            if (pFocused == null)
                return false;

            GrColumn pColumn = pFocused.GetColumn();
            GrDataRow pDataRow = pFocused.GetDataRow();

            int index = pColumn.GetVisibleIndex();
            if (index != 0)
                index = index - 1;

            if (index == pColumn.GetVisibleIndex())
                return false;

            pColumn = m_pColumnList.GetVisibleColumn(index);
            pFocused = pDataRow.GetItem(pColumn);

            if (range == GrSelectionRange.One)
            {
                if (this.GridCore.GetFullRowSelect() == true)
                    m_pItemSelector.SelectDataRow(pDataRow, GrSelectionType.Normal);
                else
                    m_pItemSelector.SelectItem(pFocused, GrSelectionType.Normal);
                m_pItemSelector.SetAnchor(pFocused);
            }
            else
            {
                m_pItemSelector.SelectItems(
                    m_pItemSelector.GetColumnSelections(pFocused),
                    m_pItemSelector.GetRowSelections(pFocused),
                    m_pGridWindow.GetSelectionType());
            }
            m_pFocuser.Set(pFocused);

            BringIntoView(pColumn);
            return true;
        }

        public void BringIntoView(IDataRow pDataRow)
        {
            m_pDataRowList.BringIntoView(pDataRow);
        }

        public void BringIntoView(GrColumn pColumn)
        {
            m_pColumnList.BringIntoView(pColumn);
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.Created += gridCore_Created;
        }

        private void focuser_FocusChanged(object sender, GrFocusChangeArgs e)
        {
            GrItem pItem = m_pFocuser.GetItem();
            if (pItem != null)
            {
                m_pLastDataColumn = pItem.GetColumn();
            }
        }

        private void gridCore_Created(object sender, EventArgs e)
        {
            m_pDataRowList = this.GridCore.GetDataRowList();
            m_pColumnList = this.GridCore.GetColumnList();
            m_pHorzScroll = this.GridCore.GetHorzScroll();
            m_pVertScroll = this.GridCore.GetVertScroll();
            m_pFocuser = this.GridCore.GetFocuser();
            m_pItemSelector = this.GridCore.GetItemSelector();
            m_pGridWindow = this.GridCore.GetGridWindow();

            m_pFocuser.FocusChanged += focuser_FocusChanged;
        }

        private void SelectOne(IDataRow pDataRow)
        {
            switch (pDataRow.GetRowType())
            {
                case GrRowType.InsertionRow:
                case GrRowType.DataRow:
                    {
                        GrDataRow pRow = pDataRow as GrDataRow;
                        GrItem pOldFocusItem = m_pFocuser.GetItem();
                        GrItem pFocusItem;

                        GrColumn pColumn;

                        if (pOldFocusItem != null)
                            pColumn = pOldFocusItem.GetColumn();
                        else
                            pColumn = m_pItemSelector.GetColumnAnchor();

                        if (pColumn == null)
                        {
                            pColumn = m_pColumnList.GetVisibleColumn(0);
                        }
                        pFocusItem = pRow.GetItem(pColumn);
                        m_pItemSelector.SetSelectionGroup(pDataRow);
                        if (this.GridCore.GetFullRowSelect() == true)
                            m_pItemSelector.SelectDataRow(pRow, GrSelectionType.Normal);
                        else
                            m_pItemSelector.SelectItem(pFocusItem, GrSelectionType.Normal);
                        m_pItemSelector.SetAnchor(pFocusItem);
                        m_pFocuser.Set(pFocusItem);
                    }
                    break;
                default:
                    {
                        m_pItemSelector.ClearSelection();
                        m_pFocuser.Set(pDataRow.GetFocusable(null));
                    }
                    break;
            }

        }

        private void SelectMulti(IDataRow pBegin, IDataRow pEnd)
        {
            if (pBegin.GetRowType() == GrRowType.InsertionRow || pEnd.GetRowType() == GrRowType.InsertionRow)
                return;

            GrRange columnRange = new GrRange(m_pItemSelector.GetColumnAnchor().GetVisibleIndex(), m_pLastDataColumn.GetVisibleIndex());
            GrRange rowRange;

            IDataRow pRowAnchor = m_pItemSelector.GetRowAnchor();
            if (pRowAnchor.GetRowType() == GrRowType.InsertionRow)
            {
                m_pItemSelector.SetRowAnchor(pBegin);
                rowRange = new GrRange(pBegin.GetVisibleIndex(), pEnd.GetVisibleIndex());
            }
            else
            {
                rowRange = new GrRange(pRowAnchor.GetVisibleIndex(), pEnd.GetVisibleIndex());
            }

            columnRange.SetMaxValue(columnRange.GetMaxValue() + 1);
            rowRange.SetMaxValue(rowRange.GetMaxValue() + 1);

            m_pItemSelector.SelectItems(columnRange, rowRange, m_pGridWindow.GetSelectionType());

            switch (pEnd.GetRowType())
            {
                case GrRowType.DataRow:
                    {
                        GrDataRow pRow = pEnd as GrDataRow;
                        GrItem pOldFocusItem = m_pFocuser.GetItem();
                        GrItem pFocusItem;

                        GrColumn pColumn;

                        if (pOldFocusItem != null)
                            pColumn = pOldFocusItem.GetColumn();
                        else
                            pColumn = m_pLastDataColumn;

                        if (pColumn == null)
                        {
                            pColumn = m_pColumnList.GetVisibleColumn(0);
                        }
                        pFocusItem = pRow.GetItem(pColumn);

                        m_pFocuser.Set(pFocusItem);
                    }
                    break;
                default:
                    {
                        m_pFocuser.Set(pEnd.GetFocusable(null));
                    }
                    break;
            }
        }

        private bool DoHorzScroll(GrScrollEventType type)
        {
            int value = m_pHorzScroll.GetValue();
            switch (type)
            {
                case GrScrollEventType.First:
                    value = m_pHorzScroll.GetMinimum();
                    break;
                case GrScrollEventType.SmallDecrement:
                    value -= m_pHorzScroll.GetSmallChange();
                    break;
                case GrScrollEventType.LargeDecrement:
                    {
                        int oldValue = value;
                        value = m_pColumnList.ClipTo(value);
                        if (value == oldValue)
                        {
                            if (value != 0)
                                value--;
                        }
                    }
                    break;
                case GrScrollEventType.LargeIncrement:
                    {
                        int oldValue = value;
                        value = m_pColumnList.ClipFrom(value);
                        if (value == oldValue)
                        {
                            if ((uint)value + 1 != m_pColumnList.GetVisibleColumnCount())
                                value++;
                        }
                    }
                    break;
                case GrScrollEventType.SmallIncrement:
                    value += m_pHorzScroll.GetSmallChange();
                    break;
                case GrScrollEventType.Last:
                    value = m_pHorzScroll.ValidateValue(m_pHorzScroll.GetMaximum());
                    break;
                default:
                    throw new Exception("");
            }

            return m_pHorzScroll.DoScroll(value);
        }

        private bool DoVertScroll(GrScrollEventType type)
        {
            GrDataRowList pDataRowList = this.GridCore.GetDataRowList();

            int value = m_pVertScroll.GetValue();
            switch (type)
            {
                case GrScrollEventType.First:
                    value = m_pVertScroll.GetMinimum();
                    break;
                case GrScrollEventType.SmallDecrement:
                    value -= m_pVertScroll.GetSmallChange();
                    break;
                case GrScrollEventType.LargeDecrement:
                    {
                        int oldValue = value;
                        value = pDataRowList.ClipTo(value);
                        if (value == oldValue)
                        {
                            if (value != 0)
                                value--;
                        }
                    }
                    break;
                case GrScrollEventType.LargeIncrement:
                    {
                        int oldValue = value;
                        value = pDataRowList.ClipFrom(value);
                        if (value == oldValue)
                        {
                            if (value + 1 != pDataRowList.GetVisibleRowCount())
                                value++;
                        }
                    }
                    break;
                case GrScrollEventType.SmallIncrement:
                    value += m_pVertScroll.GetSmallChange();
                    break;
                case GrScrollEventType.Last:
                    value = m_pVertScroll.ValidateValue(m_pVertScroll.GetMaximum());
                    break;
                default:
                    throw new Exception("");
            }

            return m_pVertScroll.DoScroll(value);
        }
    }
}
