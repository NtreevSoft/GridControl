using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrDataRowList : GrUpdatableRow
    {
        int m_rowWidth;

        List<GrDataRow> m_vecDataRowsRemoved = new List<GrDataRow>();
        List<GrDataRow> m_vecDataRows = new List<GrDataRow>();
        List<GrDataRow> m_vecVisibleDataRows = new List<GrDataRow>();
        List<IDataRow> m_vecVisibleRows = new List<IDataRow>();
        List<IDataRow> m_vecDisplayableRows = new List<IDataRow>();
        List<GrGroupRow> m_vecGrourows = new List<GrGroupRow>();

        Dictionary<string, GrCache> m_mapCache = new Dictionary<string, GrCache>();
        List<GrColumn> m_vecColumns = new List<GrColumn>();

        int m_usedGrourow;
        int m_groupCount;
        int m_dataRowID;
        int m_margin;
        int m_offset;

        int m_displayableBottom;
        int m_visibleBottom;
        int m_visibleHeight;
        int m_displayableHeight;

        GrRange m_selectingRange;
        GrDataRow m_pFocusedDataRow;
        GrRect m_bound;
        GrDataRow m_pInsertionRow;

        GrDataRow m_pItems;
        int m_itemsCount;

        bool m_visibleRowNumber;
        bool m_zeroBasedRowIndex;
        bool m_updating;

        int m_clippedHeight;
        int m_clippedIndex;

        // flags
        bool m_listChanged;
        bool m_visibleChanged;
        bool m_fitChanged;
        bool m_heightChanged;

        public GrDataRowList()
        {
            SetText("DataRowList");

            m_rowWidth = 50;
            m_displayableBottom = 0;
            m_margin = 0;

            m_groupCount = 0;
            m_usedGrourow = 0;
            m_pFocusedDataRow = null;
            m_pInsertionRow = CreateInsertionRow();
            m_dataRowID = 0;
            m_displayableHeight = 0;
            m_visibleRowNumber = true;
            m_clippedIndex = GrDefineUtility.INVALID_INDEX;
            m_zeroBasedRowIndex = false;

            m_listChanged = false;
            m_visibleChanged = false;
            m_fitChanged = false;
            m_updating = false;

            m_pItems = null;
            m_itemsCount = 0; ;
        }
        //virtual ~GrDataRowList();

        public void Reserve(int reserve)
        {
            m_vecDataRows.Capacity = reserve;
            m_vecDataRowsRemoved.Capacity = reserve;
        }

        public void AdjustRowHeight()
        {
            foreach (var value in m_vecVisibleRows)
            {
                value.AdjustHeight();
            }

            GetInsertionRow().AdjustHeight();

            m_fitChanged = false;
            m_heightChanged = true;

            this.GridCore.Invalidate();
        }

        public int GetRowWidth()
        {
            return m_rowWidth;
        }

        public void SetRowWidth(int width)
        {
            m_rowWidth = width;
        }

        public int GetVisibleRowCount()
        {
            return m_vecVisibleRows.Count;
        }

        public IDataRow GetVisibleRow(int index)
        {
            if (index == GrDefineUtility.INSERTION_ROW)
                return this.GridCore.GetInsertionRow();
            if (index >= m_vecVisibleRows.Count)
                throw new Exception("잘못된 인덱스");
            return m_vecVisibleRows[index];
        }

        public int GetVisibleDataRowCount()
        {
            return m_vecVisibleDataRows.Count;
        }

        public GrDataRow GetVisibleDataRow(int index)
        {
            if (index == GrDefineUtility.INSERTION_ROW)
                return GetInsertionRow();
            if (index >= m_vecVisibleDataRows.Count)
                throw new Exception("잘못된 인덱스");
            return m_vecVisibleDataRows[index];
        }

        public GrDataRow NewDataRow()
        {
            GrDataRow pDataRow = new GrDataRow();
            pDataRow.SetDataRowID(m_dataRowID++);
            pDataRow.Reserve(m_vecColumns.Count);
            foreach (var value in m_vecColumns)
            {
                pDataRow.AddItem(value);
            }
            m_vecDataRowsRemoved.Add(pDataRow);
            return pDataRow;
        }

        public GrDataRow GetInsertionRow()
        {
            return m_pInsertionRow;
        }

        public void AddDataRow(GrDataRow pDataRow)
        {
            InsertDataRow(pDataRow, m_vecDataRows.Count);
        }

        public void RemoveDataRow(GrDataRow pDataRow)
        {
            if (pDataRow.GetDataRowIndex() == GrDefineUtility.INVALID_INDEX)
                return;

            int index = m_vecDataRows.IndexOf(pDataRow);

            //List<GrDataRow>.iterator itor = std.find(m_vecDataRows.begin(), m_vecDataRows.end(), pDataRow);
            //if(itor == m_vecDataRows.end())
            //    return;

            if (index < 0)
                return;

            int dataRowIndex = pDataRow.GetDataRowIndex();
            for (int i = index; i < m_vecDataRows.Count; i++)
            //for(List<GrDataRow>.iterator next = itor + 1 ; next != m_vecDataRows.end() ; next++)
            {
                m_vecDataRows[i].SetDataRowIndexCore(dataRowIndex);
                dataRowIndex++;
            }

            m_vecDataRowsRemoved.Add(pDataRow);
            m_vecDataRows.Remove(pDataRow);

            GrDataRowEventArgs e = new GrDataRowEventArgs(pDataRow);
            OnDataRowRemoved(e);
        }

        public void InsertDataRow(GrDataRow pDataRow, int index)
        {
            if (pDataRow.GetDataRowIndex() != GrDefineUtility.INVALID_INDEX)
                throw new Exception("이미 등록되어 있습니다");

            GrDataRowInsertType insertType = pDataRow == m_pInsertionRow ? GrDataRowInsertType.Insertion : GrDataRowInsertType.New;

            GrDataRowInsertingEventArgs e1 = new GrDataRowInsertingEventArgs(pDataRow, insertType);
            OnDataRowInserting(e1);

            if (e1.GetCancel() == true)
                return;

            if (pDataRow.GetDataRowID() == GrDefineUtility.INSERTION_ROW)
            {
                pDataRow.SetDataRowID(m_dataRowID++);
            }
            else
            {
                //List<GrDataRow>.iterator itor = std.find(m_vecDataRowsRemoved.begin(), m_vecDataRowsRemoved.end(), pDataRow);
                //m_vecDataRowsRemoved.Remove(itor);
                m_vecDataRowsRemoved.Remove(pDataRow);
            }

            this.GridCore.AttachObject(pDataRow);

            //List<GrDataRow>.iterator itor = m_vecDataRows.insert(m_vecDataRows.begin() + index, pDataRow);
            //index = std.min(index, (uint)m_vecDataRows.Count);
            //for( ; itor != m_vecDataRows.end() ; itor++)
            //{
            //    (*itor).SetDataRowIndexCore(index++);
            //}

            m_vecDataRows.Insert(index, pDataRow);

            index = Math.Min(index, m_vecDataRows.Count);
            for (int i = index; i < m_vecDataRows.Count; i++)
            {
                m_vecDataRows[i].SetDataRowIndexCore(index++);
            }

            if (pDataRow == m_pInsertionRow)
            {
                m_pInsertionRow = CreateInsertionRow();
                this.GridCore.AttachObject(m_pInsertionRow);

                m_pInsertionRow.Reserve(m_vecColumns.Count);
                foreach (var value in m_vecColumns)
                {
                    m_pInsertionRow.AddItem(value);
                }
            }

            GrDataRowInsertedEventArgs e2 = new GrDataRowInsertedEventArgs(pDataRow, insertType);
            OnDataRowInserted(e2);
        }

        public void MoveDataRow(GrDataRow pDataRow, int index)
        {
            if (pDataRow.GetDataRowIndex() == index)
                return;

            GrDataRowMovingEventArgs e1 = new GrDataRowMovingEventArgs(pDataRow, index);
            OnDataRowMoving(e1);

            if (e1.GetCancel() == true)
                return;

            //index = std.min(index, m_vecDataRows.Count - 1);

            //List<GrDataRow>.iterator itor = std.find(m_vecDataRows.begin(), m_vecDataRows.end(), pDataRow);
            //size_t oldIndex = std.distance(m_vecDataRows.begin(), itor);
            m_vecDataRows.Remove(pDataRow);
            m_vecDataRows.Insert(index, pDataRow);

            //int oldIndex = items.IndexOf(dataRow);
            //var targetRow = items[oldIndex];
            //items.RemoveAt(oldIndex);
            //items.Insert(index, targetRow);
            //
            //if(index >= m_vecDataRows.Count)
            //{
            //	m_vecDataRows.Remove(itor);
            //	m_vecDataRows.Add(pDataRow);
            //}
            //else
            //{
            //	GrDataRow pTargetDataRow = m_vecDataRows[index];
            //	m_vecDataRows.Remove(itor);
            //	List<GrDataRow>.iterator itorAt = std.find(m_vecDataRows.begin(), m_vecDataRows.end(), pTargetDataRow);
            //	m_vecDataRows.insert(itorAt, pDataRow);
            //}

            index = 0;
            foreach (var item in m_vecDataRows)
            {
                item.SetDataRowIndexCore(index++);
            }

            GrDataRowEventArgs e = new GrDataRowEventArgs(pDataRow);
            OnDataRowMoved(e);
        }

        public void MoveDataRow(GrDataRow pDataRow, int oldIndex, int newIndex)
        {
            m_vecDataRows.RemoveAt(oldIndex);
            m_vecDataRows.Insert(newIndex, pDataRow);

            int index = 0;
            foreach (var item in m_vecDataRows)
            {
                item.SetDataRowIndexCore(index++);
            }

            GrDataRowEventArgs e = new GrDataRowEventArgs(pDataRow);
            OnDataRowMoved(e);
        }

        public void Reset(List<GrDataRow> rows)
        {
            m_vecDataRows.Clear();
            m_vecDataRows.AddRange(rows);

            int index = 0;
            foreach (var item in m_vecDataRows)
            {
                item.SetDataRowIndexCore(index++);
            }

            SetListChanged();
            this.GridCore.Invalidate();
            this.GridCore.Update();
        }

        public int GetDataRowCount()
        {
            return m_vecDataRows.Count;
        }

        public GrDataRow GetDataRow(int index)
        {
            if (index == GrDefineUtility.INSERTION_ROW)
                return this.GridCore.GetInsertionRow();
            return m_vecDataRows[index];
        }

        public void Clear()
        {
            this.GridCore.GetItemSelector().ClearSelection();
            this.GridCore.GetFocuser().Reset();
            foreach (var value in m_vecDataRows)
            {
                value.SetDataRowIndexCore(GrDefineUtility.INVALID_INDEX);
            }

            //m_vecDataRowsRemoved.insert(m_vecDataRowsRemoved.end(), m_vecDataRows.begin(), m_vecDataRows.end());
            m_vecDataRowsRemoved.AddRange(m_vecDataRows);
            m_vecDataRows.Clear();

            Update(true);
            this.GridCore.Invalidate();
        }

        public int GetDisplayableRowCount()
        {
            return m_vecDisplayableRows.Count;
        }

        public IDataRow GetDisplayableRow(int index)
        {
            return m_vecDisplayableRows[index];
        }

        public int GetDisplayableBottom()
        {
            return m_displayableBottom;
        }

        public int GetVisibleBottom()
        {
            return m_visibleBottom;
        }

        public int ClipFrom(int visibleFrom)
        {
            return ClipFrom(this.GridCore.GetDisplayRect(), visibleFrom);
        }

        public int ClipFrom(GrRect displayRect, int visibleFrom)
        {
            int displayHeight = displayRect.GetHeight() - GetY();
            int displayY = displayHeight;
            int visibleTo = visibleFrom;

            for (int i = visibleFrom; i < GetVisibleRowCount(); i++)
            {
                IDataRow pDataRow = GetVisibleRow(i);
                displayY -= pDataRow.GetHeight();
                if (displayY <= 0)
                    break;
                visibleTo = i;
            }
            return visibleTo;
        }

        public int ClipTo(int visibleTo)
        {
            return ClipTo(this.GridCore.GetDisplayRect(), visibleTo);
        }


        public int ClipTo(GrRect displayRect, int visibleTo)
        {
            int displayHeight = displayRect.GetHeight() - GetY();
            int displayY = displayHeight;
            int visibleFrom = visibleTo;

            for (int i = visibleTo; i < m_vecVisibleRows.Count; i--)
            {
                IDataRow item = m_vecVisibleRows[i];
                displayY -= item.GetHeight();
                if (displayY <= 0)
                    break;
                visibleFrom = i;
            }
            return visibleFrom;
        }

        public IDataRow HitTest(int y)
        {
            foreach (var value in m_vecDisplayableRows)
            {
                if (y >= value.GetY() && y < value.GetBottom())
                    return value;
            }
            return null;
        }

        public GrRange HitTest(int y, IDataRow rowAnchor)
        {
            int anchorIndex = rowAnchor.GetVisibleIndex();
            GrRange indexRange = new GrRange();

            if (rowAnchor.GetRowType() == GrRowType.InsertionRow)
            {
                indexRange.SetRange(GrDefineUtility.INSERTION_ROW, GrDefineUtility.INSERTION_ROW + 1);
            }
            else
            {
                if (y >= GetBottom())
                {
                    int displayCount = GetDisplayableRowCount();
                    IDataRow pDataRow = GetDisplayableRow(displayCount - 1);
                    indexRange.SetRange(anchorIndex, pDataRow.GetVisibleIndex() + 1);
                }
                else if (y < GetY())
                {
                    IDataRow pFirst = GetDisplayableRow(0);
                    indexRange.SetRange(pFirst.GetVisibleIndex(), anchorIndex + 1);
                }
                else
                {
                    int index = 0;
                    IDataRow pDataRow = HitTest(y);
                    if (pDataRow != null)
                        index = pDataRow.GetVisibleIndex();

                    if (index < anchorIndex)
                        indexRange.SetRange(index, anchorIndex + 1);
                    else
                        indexRange.SetRange(anchorIndex, index + 1);
                }
            }

            return indexRange;
        }

        public void BringIntoView(IDataRow pDataRow)
        {
            if (pDataRow.ShouldBringIntoView() == false)
                return;

            Update();

            GrRect dataRect = this.GridCore.GetDataRect();
            GrScroll pVertScroll = this.GridCore.GetVertScroll();

            if (pDataRow.GetHeight() > dataRect.GetHeight())
            {
                pVertScroll.SetValue(pDataRow.GetVisibleIndex());
            }
            else
            {
                int visibleIndex = pDataRow.GetVisibleIndex();

                int newValue;
                if (visibleIndex < pVertScroll.GetValue())
                {
                    newValue = visibleIndex;
                }
                else
                {
                    newValue = ClipTo(visibleIndex);
                }
                pVertScroll.SetValue(newValue);
            }
            this.GridCore.Invalidate();
            this.GridCore.Update();
        }

        public void DisplayFirst(IDataRow pDataRow)
        {
            GrScroll pVertScroll = this.GridCore.GetVertScroll();
            pVertScroll.SetValue(pDataRow.GetVisibleIndex());
            this.GridCore.Invalidate();
            this.GridCore.Update();
        }

        public int GetDisplayOffset()
        {
            return m_offset;
        }

        public void Sort(GrColumn column)
        {
            FuncSortRow fnSort = (FuncSortRow)column.GetSortComparer(column.GetSortType());
            if (fnSort == null)
            {
                switch (column.GetSortType())
                {
                    case GrSort.Up:

                        fnSort = GrSortFunc.SortDataRowSortUp;
                        break;
                    case GrSort.Down:
                        fnSort = GrSortFunc.SortDataRowSortDown;
                        break;
                    default:
                        fnSort = GrSortFunc.SortDataRowSortNone;
                        break;
                }
            }

            if (m_groupCount == 0)
            {
                base.Sort(fnSort, column);
            }
            else
            {
                for (int i = 0; i < m_usedGrourow; i++)
                {
                    GrGroupRow groupRow = m_vecGrourows[i];
                    if (groupRow.GetGroupLevel() == m_groupCount - 1)
                        groupRow.Sort(fnSort, column);
                }
            }
            SetVisibleChanged();
        }

        public void SetZeroBasedRowIndex(bool b)
        {
            if (m_zeroBasedRowIndex == b)
                return;

            m_zeroBasedRowIndex = b;

            int index = 0;
            foreach (var item in m_vecDataRows)
            {
                item.SetDataRowIndexCore(index++);
            }
        }

        public bool GetZeroBasedRowIndex()
        {
            return m_zeroBasedRowIndex;
        }

        public bool GetRowNumberVisible()
        {
            return m_visibleRowNumber;
        }

        public void SetRowNumberVisible(bool b)
        {
            m_visibleRowNumber = b;
            this.GridCore.Invalidate();
        }

        public int CellStart()
        {
            int start = GetX();
            if (this.GridCore.GetRowVisible() == true)
                start += m_rowWidth;
            return start + m_margin;
        }

        public int GetMargin()
        {
            return m_margin;
        }

        public void SetFitChanged()
        {
            m_fitChanged = true;
        }

        public void SetVisibleChanged()
        {
            if (m_visibleChanged == false)
            {
                this.GridCore.Invalidate();
            }

            m_visibleChanged = true;
        }

        public void SetHeightChanged()
        {
            if (m_heightChanged == false)
            {
                this.GridCore.Invalidate();
            }
            m_heightChanged = true;
        }

        public void SetListChanged()
        {
            if (m_listChanged == false)
            {
                this.GridCore.Invalidate();
            }
            m_listChanged = true;
        }

        public event EventHandler VisibleChanged;

        public event DataRowInsertingEventHandler DataRowInserting;
        public event DataRowInsertedEventHandler DataRowInserted;
        public event DataRowEventHandler DataRowRemoved;
        public event DataRowMovingEventHandler DataRowMoving;
        public event DataRowEventHandler DataRowMoved;
        public event EventHandler VisibleHeightChanged;

        public override GrRect GetBounds()
        {
            GrRect bound = m_bound;
            //bound.Right = this.GridCore.GetColumnList().GetBounds().Right;
            bound.Width = this.GridCore.GetColumnList().GetBounds().Right - bound.Left;
            return bound;
        }

        public override bool ShouldClip(GrRect displayRect, int horizontal, int vertical)
        {
            if (m_clippedIndex == vertical && displayRect.GetHeight() == m_clippedHeight)
                return false;
            return true;
        }

        public override void Clip(GrRect displayRect, int horizontal, int vertical)
        {
            int displayY = GetY();

            HashSet<IDataRow> oldDisplayableRows = new HashSet<IDataRow>(m_vecDisplayableRows);

            m_vecDisplayableRows.Clear();

            int displayIndex = 0;

            m_offset = 0;
            for (int i = 0; i < vertical; i++)
            {
                m_offset += m_vecVisibleRows[i].GetHeight();
            }

            List<IDataRow> vecVisibleRows = new List<IDataRow>(m_vecVisibleRows);
            foreach (var item in vecVisibleRows)
            {
                if (displayY > displayRect.Bottom)
                    break;

                bool clipped = displayY + item.GetHeight() > displayRect.Bottom;

                //item.SetY(displayY);
                item.SetDisplayable(true);
                item.SetDisplayIndex(displayIndex);
                item.SetClipped(clipped);
                item.InvokeOnYChanged();

                m_vecDisplayableRows.Add(item);
                displayY += item.GetHeight();
                displayIndex++;

                oldDisplayableRows.Remove(item);
            }

            foreach (var item in oldDisplayableRows)
            {
                item.SetDisplayable(false);
                item.SetDisplayIndex(GrDefineUtility.INVALID_INDEX);
                item.SetClipped(false);
            }

            int left = GetX();
            int top = GetY();
            int right = GetRight();
            int bottom = displayY;
            m_bound = GrRect.FromLTRB(left, top, right, bottom);

            m_displayableHeight = displayY - GetY();
            m_displayableBottom = Math.Min(displayRect.Bottom, displayY);

            if (m_clippedHeight != displayRect.GetHeight() || m_clippedIndex != vertical)
                UpdateVertScroll(displayRect);

            m_clippedHeight = displayRect.GetHeight();
            m_clippedIndex = vertical;
        }

        public override int GetClipPriority() { return 1; }
        public override bool ShouldUpdate()
        {
            return m_listChanged == true ||
                m_visibleChanged == true ||
                m_fitChanged == true ||
                m_heightChanged == true;
        }

        public override void Update(bool force)
        {
            if (m_listChanged == true || force == true)
                BuildChildRowList();
            if (m_visibleChanged == true)
                BuildVisibleRowList();
            if (m_fitChanged == true)
                AdjustRowHeight();
            if (m_heightChanged == true)
                RepositionVisibleRowList();

            m_listChanged = false;
            m_visibleChanged = false;
            m_fitChanged = false;
            m_heightChanged = false;

            bool visible = false;
            if (GetVisibleRowCount() != 0)
            {
                visible = m_visibleBottom > this.GridCore.GetDisplayRect().Bottom ? true : false;
            }

            this.GridCore.GetVertScroll().SetVisible(visible);
        }

        public override int GetUpdatePriority() { return GrDefineUtility.UPDATEPRIORITY_DATAROWLIST; }

        public override GrRowType GetRowType() { return GrRowType.DataRowList; }
        public override GrCell HitTest(GrPoint location)
        {
            GrCell pHitted = base.HitTest(location);
            if (pHitted == null)
                return null;

            foreach (var item in m_vecDisplayableRows)
            {
                GrCell pSubHitted = item.HitTest(location);
                if (pSubHitted != null)
                    return pSubHitted;
            }

            return null;
        }

        public override int GetWidth() { return 0; }
        public override int GetHeight() { return m_displayableHeight; }
        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            foreach (var value in m_vecDisplayableRows)
            {
                int y = value.GetY();
                int b = value.GetBottom();
                if (y >= clipRect.Bottom || b < clipRect.Top)
                    continue;

                value.Paint(painter, clipRect);
            }

            if (m_pFocusedDataRow != null && m_pFocusedDataRow.GetDisplayable() == true)
            {
                if (this.GridCore.GetRowHighlight() == true && this.GridCore.GetRowHighlightType() != GrRowHighlightType.Fill)
                {
                    GrRect highlightRect = m_pFocusedDataRow.GetRect();
                    GrStyle pStyle = this.GridCore.GetStyle();
                    if (pStyle == null)
                        pStyle = GrStyle.Default;
                    GrRect displayRect = this.GridCore.GetDisplayRect();
                    highlightRect.Expand(0, 1, 0, 0);
                    if (this.GridCore.GetFillBlank() == false)
                        highlightRect.Width = this.GridCore.GetColumnList().GetBounds().Right - highlightRect.Left;
                    else
                        highlightRect.Width = displayRect.Right - highlightRect.Left;

                    painter.DrawRectangle(highlightRect, pStyle.RowHighlightLineColor);
                }
            }
        }



        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            m_vecDataRows.Capacity = this.GridCore.GetReservedRow();

            GrGroupPanel groupPanel = this.GridCore.GetGroupPanel();

            groupPanel.Changed += groupPanel_Changed;
            groupPanel.Expanded += groupPanel_Expanded;
            groupPanel.SortChanged += groupPanel_SortChanged;

            GrFocuser focuser = this.GridCore.GetFocuser();
            focuser.FocusChanged += focuser_FocusedChanged;

            this.GridCore.Cleared += gridCore_Cleared;
            this.GridCore.Created += gridCore_Created;
            this.GridCore.FontChanged += gridCore_FontChanged;

            foreach (var item in m_vecDataRows)
            {
                this.GridCore.AttachObject(item);
            }

            foreach (var item in m_vecGrourows)
            {
                this.GridCore.AttachObject(item);
            }

            this.GridCore.AttachObject(m_pInsertionRow);
        }

        protected override void OnYChanged()
        {
            RepositionVisibleRowList();
        }

        protected virtual void OnDataRowInserting(GrDataRowInsertingEventArgs e)
        {
            DataRowInserting(this, e);
        }

        protected virtual void OnDataRowInserted(GrDataRowInsertedEventArgs e)
        {
            SetListChanged();
            this.GridCore.Invalidate();

            DataRowInserted(this, e);
        }

        protected virtual void OnDataRowRemoved(GrDataRowEventArgs e)
        {
            GrDataRow pDataRow = e.GetDataRow();

            pDataRow.SetDataRowIndexCore(GrDefineUtility.INVALID_INDEX);
            pDataRow.SetSelected(false);
            pDataRow.KillFocus();

            SetListChanged();
            this.GridCore.Invalidate();

            DataRowRemoved(this, e);
        }

        protected virtual void OnDataRowMoving(GrDataRowMovingEventArgs e)
        {
            DataRowMoving(this, e);
        }

        protected virtual void OnDataRowMoved(GrDataRowEventArgs e)
        {
            SetListChanged();
            this.GridCore.Invalidate();
            this.GridCore.Update();
            DataRowMoved(this, e);
        }

        protected virtual void OnVisibleHeightChanged(EventArgs e)
        {
            VisibleHeightChanged(this, e);
        }


        private GrGroupRow CreateGrourow(GrRow pParent, GrColumn column, string itemText)
        {
            GrGroupRow groupRow;
            if (m_vecGrourows.Count == m_usedGrourow)
            {
                groupRow = new GrGroupRow();
                m_vecGrourows.Add(groupRow);
            }
            else
            {
                groupRow = m_vecGrourows[m_usedGrourow];
            }

            pParent.AddChild(groupRow);

            groupRow.SetTextVisible(false);
            groupRow.SetReference(column, itemText);
            groupRow.m_groupLevel = 0;

            if (m_mapCache.ContainsKey(groupRow.GetKey()) == false)
            {
                GrPadding padding = groupRow.GetPadding();
                int height = GetPaintingFont().GetHeight() + padding.GetVertical();
                groupRow.SetHeight(height);
                groupRow.Expand(true);
            }
            else
            {
                var value = m_mapCache[groupRow.GetKey()];
                groupRow.SetHeight(value.height);
                groupRow.Expand(value.expanded);
            }

            this.GridCore.AttachObject(groupRow);

            m_usedGrourow++;
            return groupRow;
        }

        private GrDataRow CreateInsertionRow()
        {
            GrDataRow pInsertionRow = new GrDataRow();
            pInsertionRow.SetText("☞");
            pInsertionRow.SetVisibleDataRowIndex(GrDefineUtility.INSERTION_ROW);
            pInsertionRow.SetVisibleIndex(GrDefineUtility.INSERTION_ROW);
            pInsertionRow.SetDisplayIndex(GrDefineUtility.INSERTION_ROW);
            pInsertionRow.SetDataRowID(GrDefineUtility.INSERTION_ROW);
            pInsertionRow.SetDisplayable(true);

            return pInsertionRow;
        }

        private void BuildGroup(GrRow pParent, int groupLevel)
        {
            GrGroupPanel groupPanel = this.GridCore.GetGroupPanel();
            this.GridCore.AttachObject(groupPanel);
            GrColumn column = groupPanel.GetGroup(groupLevel).GetColumn();

            List<GrDataRow> vecSort = new List<GrDataRow>();

            FuncSortRow fnSort;
            switch (column.GetGroup().GetSortType())
            {
                case GrSort.Up:
                    fnSort = GrSortFunc.SortDataRowSortUp;
                    break;
                default:
                    fnSort = GrSortFunc.SortDataRowSortDown;
                    break;
            }
            pParent.Sort(fnSort, column);

            vecSort.Capacity = pParent.GetChildCount();
            for (int i = 0; i < pParent.GetChildCount(); i++)
            {
                GrRow pChild = pParent.GetChild(i);
                if (pChild.GetRowType() == GrRowType.DataRow)
                    vecSort.Add(pChild as GrDataRow);
            }

            pParent.ClearChild();

            string itemText = null;

            GrGroupRow groupRow = null;

            for (int i = 0; i < vecSort.Count; i++)
            {
                GrDataRow row = vecSort[i];
                GrItem pItem = row.GetItem(column);
                string nextItemText = pItem.GetText();

                if (itemText != nextItemText || i == 0)
                {
                    itemText = nextItemText;
                    groupRow = CreateGrourow(pParent, column, itemText);
                }
                groupRow.AddChild(row);
            }

            int nextGroupLevel = groupLevel + 1;
            for (int i = 0; i < pParent.GetChildCount(); i++)
            {
                GrGroupRow row = pParent.GetChild(i) as GrGroupRow;
                row.ProcessAfterGroup();
                if (nextGroupLevel < m_groupCount)
                    BuildGroup(row, nextGroupLevel);
            }
        }

        private void BuildChildRowList()
        {
            ClearChild();

            ReserveChild(m_vecDataRows.Count);
            foreach (var value in m_vecDataRows)
            {
                AddChild(value);
            }

            if (m_groupCount == 0)
            {
                m_usedGrourow = 0;
                m_mapCache.Clear();

                GrColumn column = this.GridCore.GetColumnList().GetFirstSortColumn();
                if (column != null)
                    Sort(column);
                else
                    base.Sort(GrSortFunc.SortDataRowSortNone, null);
            }
            else
            {
                BuildCache();

                foreach (var value in m_vecGrourows)
                {
                    value.ClearChild();
                }
                m_usedGrourow = 0;
                BuildGroup(this, 0);
            }

            SetVisibleChanged();
        }

        private void BuildVisibleRowList()
        {
            if (m_updating == true)
                return;
            m_updating = true;
            List<GrRow> vecVisibles = new List<GrRow>(this.GridCore.GetReservedRow());

            int level = 0;
            GetVisibleList(this, vecVisibles);
            GetMaxDepth(this, ref level);

            int temp = level - GetDepth();
            if (temp <= 1)
                m_margin = 0;
            else
                m_margin = (temp - 1) * GrDefineUtility.DEF_GROUP_WIDTH;

            HashSet<IDataRow> oldVisibles = new HashSet<IDataRow>(m_vecVisibleRows);

            m_vecVisibleRows.Clear();
            m_vecVisibleRows.Capacity = vecVisibles.Count;
            m_vecVisibleDataRows.Clear();
            m_vecVisibleDataRows.Capacity = vecVisibles.Count;

            foreach (var value in vecVisibles)
            {
                IDataRow pDataRowBase = value as IDataRow;
                if (pDataRowBase == null)
                    continue;
                GrDataRow pDataRow = pDataRowBase as GrDataRow;
                if (pDataRow != null)
                {
                    pDataRow.SetVisibleDataRowIndex(m_vecVisibleDataRows.Count);
                    m_vecVisibleDataRows.Add(pDataRow);
                }
                pDataRowBase.SetVisibleIndex(m_vecVisibleRows.Count);
                m_vecVisibleRows.Add(pDataRowBase);

                oldVisibles.Remove(pDataRowBase);
            }

            foreach (var value in oldVisibles)
            {
                value.SetVisibleIndex(GrDefineUtility.INVALID_INDEX);
            }

            foreach (var value in m_vecDisplayableRows)
            {
                value.SetDisplayable(false);
                value.SetDisplayIndex(GrDefineUtility.INVALID_INDEX);
                value.SetClipped(false);
            }

            m_vecDisplayableRows.Clear();
            m_heightChanged = true;

            m_updating = false;
        }

        private void RepositionVisibleRowList()
        {
            int oldVisibleHeight = m_visibleHeight;
            int y = GetY();
            foreach (var value in m_vecVisibleRows)
            {
                value.SetY(y);
                y += value.GetHeight();
            }
            m_visibleBottom = y;
            m_visibleHeight = m_visibleBottom - GetY();
            m_clippedIndex = GrDefineUtility.INVALID_INDEX;

            if (m_visibleHeight != oldVisibleHeight)
            {
                OnVisibleHeightChanged(EventArgs.Empty);
            }
        }

        private void BuildCache()
        {
            m_mapCache.Clear();
            for (int i = 0; i < m_usedGrourow; i++)
            {
                GrGroupRow groupRow = m_vecGrourows[i];

                GrCache cache;
                cache.height = groupRow.GetHeight();
                cache.expanded = groupRow.IsExpanded();

                m_mapCache.Add(groupRow.m_key, cache);
            }
        }

        private void DeleteObjects()
        {
            //foreach (var value in m_vecDataRows)
            //{
            //    delete value;
            //}

            //foreach (var value in m_vecDataRowsRemoved)
            //{
            //    delete value;
            //}

            //foreach (var value in m_vecGrourows)
            //{
            //    delete value;
            //}

            m_vecDataRows.Clear();
            m_vecDataRowsRemoved.Clear();
            m_vecGrourows.Clear();
        }

        private void UpdateVertScroll(GrRect displayRect)
        {
            GrScroll pVertScroll = this.GridCore.GetVertScroll();

            if (pVertScroll.GetVisible() == false)
                return;

            int visibleRowCount = GetVisibleRowCount();
            int visibleHeight = m_visibleBottom - GetY();
            int displayableHeight = displayRect.Bottom - GetY();

            int countPerPage = 0;
            int tempHeight = displayableHeight;
            for (int i = visibleRowCount - 1; i < visibleRowCount; i--)
            {
                IDataRow pDataRow = GetVisibleRow(i);
                int rowHeight = pDataRow.GetHeight();
                if (rowHeight < tempHeight)
                {
                    countPerPage++;
                    tempHeight -= rowHeight;
                }
                else
                {
                    break;
                }
            }
            countPerPage = Math.Max(1, countPerPage);

            int largeChange = (int)((float)visibleRowCount * (float)displayableHeight / (float)visibleHeight);
            largeChange = Math.Max(1, largeChange);

            pVertScroll.SetMinimum(0);
            pVertScroll.SetMaximum(visibleRowCount - countPerPage + largeChange - 1);
            pVertScroll.SetLargeChange(largeChange);
        }
        private void GetVisibleList(GrRow row, List<GrRow> pVisible)
        {
            if (row.GetVisible() == false)
                return;
            pVisible.Add(row);
            for (int i = 0; i < row.GetChildCount(); i++)
            {
                GrRow pChild = row.GetChild(i);
                GetVisibleList(pChild, pVisible);
            }
        }

        private void GetMaxDepth(GrRow row, ref int depth)
        {
            depth = Math.Max(depth, row.GetDepth());
            for (int i = 0; i < row.GetChildCount(); i++)
            {
                GrRow pChild = row.GetChild(i);

                GetMaxDepth(pChild, ref depth);
            }
        }

        private void groupPanel_Changed(object sender, EventArgs e)
        {
            GrGroupPanel groupPanel = this.GridCore.GetGroupPanel();
            m_groupCount = groupPanel.GetGroupCount();
            m_clippedIndex = GrDefineUtility.INVALID_INDEX;

            int groupCount = groupPanel.GetGroupCount();
            if (groupCount == 0)
                m_margin = 0;
            else
                m_margin = (groupCount + 1) * GrDefineUtility.DEF_GROUP_WIDTH;

            BuildChildRowList();
        }

        private void groupPanel_Expanded(object sender, GrGroupEventArgs e)
        {
            GrColumn column = e.Group.GetColumn();
            foreach (var value in m_vecGrourows)
            {
                if (value.GetColumn() == column)
                    value.Expand(e.Group.GetExpanded());
            }
            SetVisibleChanged();
        }

        private void groupPanel_SortChanged(object sender, GrGroupEventArgs e)
        {
            GrColumn column = e.Group.GetColumn();

            HashSet<GrRow> parentRows = new HashSet<GrRow>();
            for (int i = 0; i < m_usedGrourow; i++)
            {
                GrGroupRow groupRow = m_vecGrourows[i];

                if (groupRow.GetColumn() == column)
                    parentRows.Add(groupRow.GetParent());
            }

            foreach (var value in parentRows)
            {
                value.Sort(e.Group.GetSortType());
            }
            SetVisibleChanged();
        }

        private void gridCore_Created(object sender, EventArgs e)
        {
            GrColumnList columnList = this.GridCore.GetColumnList();
            columnList.ColumnInserted += columnList_ColumnInserted;
            columnList.ColumnRemoved += columnList_ColumnRemoved;
            columnList.ColumnSortTypeChanged += columnList_ColumnSortTypeChanged;
            columnList.ColumnPaddingChanged += columnList_ColumnPaddingChanged;
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            ClearChild();

            m_groupCount = 0;
            m_usedGrourow = 0;
            m_pFocusedDataRow = null;
            m_dataRowID = 0;
            m_margin = 0;

            GetInsertionRow().ClearItem();

            DeleteObjects();

            m_vecVisibleDataRows.Clear();
            m_vecVisibleRows.Clear();
            m_vecDisplayableRows.Clear();

            m_mapCache.Clear();
            m_vecColumns.Clear();
        }
        private void gridCore_FontChanged(object sender, EventArgs e)
        {
            GrColumnList columnList = this.GridCore.GetColumnList();
            List<GrDataRow> vecDataRows = m_vecDataRows;
            vecDataRows.Add(GetInsertionRow());

            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();

            foreach (var value in vecDataRows)
            {
                GrDataRow pDataRow = value;
                pTextUpdater.AddTextBounds(pDataRow);

                for (int i = 0; i < columnList.GetColumnCount(); i++)
                {
                    GrColumn column = columnList.GetColumn(i);
                    GrItem pItem = pDataRow.GetItem(column);
                    pTextUpdater.AddTextBounds(pItem);
                }
            }

            foreach (var value in m_vecGrourows)
            {
                pTextUpdater.AddTextBounds(value);
                pTextUpdater.AddTextBounds(value.GetLabel());
            }
        }

        private void columnList_ColumnInserted(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.GetColumn();

            if (column.GetColumnID() >= m_vecColumns.Count)
            {
                m_vecColumns.Add(column);
            }

            GetInsertionRow().AddItem(column);
            foreach (var value in m_vecDataRows)
            {
                value.AddItem(column);
            }

            foreach (var value in m_vecDataRowsRemoved)
            {
                value.AddItem(column);
            }
        }

        private void columnList_ColumnRemoved(object sender, GrColumnEventArgs e)
        {
            GrItem pItem = GetInsertionRow().GetItem(e.GetColumn());
            this.GridCore.DetachObject(pItem);

            foreach (var value in m_vecDataRows)
            {
                pItem = value.GetItem(e.GetColumn());
                this.GridCore.DetachObject(pItem);
            }
        }

        private void columnList_ColumnSortTypeChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.GetColumn();
            Sort(column);
        }

        private void columnList_ColumnPaddingChanged(object sender, GrColumnEventArgs e)
        {
            m_pInsertionRow.SetFit();
            foreach (var value in m_vecDataRows)
            {
                value.SetFit();
            }
        }

        private void focuser_FocusedChanged(object sender, GrFocusChangeArgs e)
        {
            GrFocuser focuser = sender as GrFocuser;
            m_pFocusedDataRow = focuser.GetFocusedRow() as GrDataRow;
        }


        struct GrCache
        {
            public int height;
            public bool expanded;
        }

    }
}
