using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrDataRowList : GrUpdatableRow
    {
        int m_rowWidth;

        List<GrDataRow> m_vecDataRowsRemoved;
        List<GrDataRow> m_vecDataRows;
        List<GrDataRow> m_vecVisibleDataRows;
        List<IDataRow> m_vecVisibleRows;
        List<IDataRow> m_vecDisplayableRows;
        List<GrGroupRow> m_vecGroupRows;

        Dictionary<string, GrCache> m_mapCache;
        List<GrColumn> m_vecColumns;

        int m_usedGroupRow;
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
            m_usedGroupRow = 0;
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

        public GrRange HitTest(int y, IDataRow pRowAnchor)
        {
            int anchorIndex = pRowAnchor.GetVisibleIndex();
            GrRange indexRange = new GrRange();

            if (pRowAnchor.GetRowType() == GrRowType.InsertionRow)
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

        public void Sort(GrColumn pColumn)
        {
            FuncSortRow fnSort = (FuncSortRow)pColumn.GetSortComparer(pColumn.GetSortType());
            if (fnSort == null)
            {
                switch (pColumn.GetSortType())
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
                base.Sort(fnSort, pColumn);
            }
            else
            {
                for (int i = 0; i < m_usedGroupRow; i++)
                {
                    GrGroupRow pGroupRow = m_vecGroupRows[i];
                    if (pGroupRow.GetGroupLevel() == m_groupCount - 1)
                        pGroupRow.Sort(fnSort, pColumn);
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

        public uint GetMargin()
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

        public virtual GrRect GetBounds()
        {
            GrRect bound = m_bound;
            bound.right = this.GridCore.GetColumnList().GetBounds().right;
            return bound;
        }

        public virtual bool ShouldClip(GrRect displayRect, uint horizontal, uint vertical)
        {
            if (m_clippedIndex == vertical && displayRect.GetHeight() == m_clippedHeight)
                return false;
            return true;
        }

        public virtual void Clip(GrRect displayRect, uint horizontal, uint vertical)
{
	int displayY = GetY();

	std.set<IDataRow*> oldDisplayableRows(m_vecDisplayableRows.begin(), m_vecDisplayableRows.end());

	m_vecDisplayableRows.Clear();

	uint displayIndex = 0;

	m_offset = 0;
	for(int i=0 ; i<vertical ; i++)
	{
		m_offset += m_vecVisibleRows[i].GetHeight();
	}

	_IDataRows vecVisibleRows(m_vecVisibleRows.begin() + vertical, m_vecVisibleRows.end());
	foreach(var item in vecVisibleRows)
	{
		if(displayY > displayRect.bottom)
			break;

		bool clipped = displayY + item.GetHeight() > displayRect.bottom;

		//item.SetY(displayY);
		item.SetDisplayable(true);
		item.SetDisplayIndex(displayIndex);
		item.SetClipped(clipped);
		item.OnYChanged();

		m_vecDisplayableRows.Add(item);
		displayY += item.GetHeight();
		displayIndex++;

		oldDisplayableRows.Remove(item);
	}

	foreach(var item in oldDisplayableRows)
	{
		item.SetDisplayable(false);
		item.SetDisplayIndex(INVALID_INDEX);
		item.SetClipped(false);
	}

	m_bound.left = GetX();
	m_bound.top = GetY();
	m_bound.right = GetRight();
	m_bound.bottom = displayY;

	m_displayableHeight = displayY - GetY();
	m_displayableBottom = std.min(displayRect.bottom, displayY);

	if(m_clippedHeight != displayRect.GetHeight() || m_clippedIndex != vertical)
		UpdateVertScroll(displayRect);

	m_clippedHeight = displayRect.GetHeight();
	m_clippedIndex = vertical;
}

        public virtual int GetClipPriority() { return 1; }
        public virtual bool ShouldUpdate()
        {
            return m_listChanged == true ||
                m_visibleChanged == true ||
                m_fitChanged == true ||
                m_heightChanged == true;
        }

        public virtual void Update(bool force)
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
                visible = m_visibleBottom > this.GridCore.GetDisplayRect().bottom ? true : false;
            }

            this.GridCore.GetVertScroll().SetVisible(visible);
        }

        public override int GetUpdatePriority() { return GrDefineUtility.UPDATEPRIORITY_DATAROWLIST; }

        public override GrRowType GetRowType() { return GrRowType.DataRowList; }
        public override GrCell HitTest(GrPoint location)
        {
            GrCell pHitted = GrUpdatableRow.HitTest(location);
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
        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            foreach (var value in m_vecDisplayableRows)
            {
                int y = value.GetY();
                int b = value.GetBottom();
                if (y >= clipRect.bottom || b < clipRect.top)
                    continue;

                value.Paint(pPainter, clipRect);
            }

            if (m_pFocusedDataRow != null && m_pFocusedDataRow.GetDisplayable() == true)
            {
                if (this.GridCore.GetRowHighlight() == true && this.GridCore.GetRowHighlightType() != GrRowHighlightType_Fill)
                {
                    GrRect highlightRect = m_pFocusedDataRow.GetRect();
                    GrStyle pStyle = this.GridCore.GetStyle();
                    if (pStyle == null)
                        pStyle = GrStyle.Default;
                    GrRect displayRect = this.GridCore.GetDisplayRect();
                    highlightRect.top--;
                    if (this.GridCore.GetFillBlank() == false)
                        highlightRect.right = this.GridCore.GetColumnList().GetBounds().right;
                    else
                        highlightRect.right = displayRect.right;
                    pPainter.DrawRectangle(highlightRect, pStyle.RowHighlightLineColor);
                }
            }
        }



        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            m_vecDataRows.Capacity = this.GridCore.GetReservedRow();

            GrGroupPanel pGroupPanel = this.GridCore.GetGroupPanel();

            pGroupPanel.Changed += groupPanel_Changed;
            pGroupPanel.Expanded += groupPanel_Expanded;
            pGroupPanel.SortChanged += groupPanel_SortChanged;

            GrFocuser pFocuser = this.GridCore.GetFocuser();
            pFocuser.FocusChanged += focuser_FocusedChanged;

            this.GridCore.Cleared += gridCore_Cleared;
            this.GridCore.Created += gridCore_Created;
            this.GridCore.FontChanged += gridCore_FontChanged;

            foreach (var item in m_vecDataRows)
            {
                this.GridCore.AttachObject(item);
            }

            foreach (var item in m_vecGroupRows)
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


        private GrGroupRow CreateGroupRow(GrRow pParent, GrColumn pColumn, string itemText)
        {
            GrGroupRow pGroupRow;
            if (m_vecGroupRows.Count == m_usedGroupRow)
            {
                pGroupRow = new GrGroupRow();
                m_vecGroupRows.Add(pGroupRow);
            }
            else
            {
                pGroupRow = m_vecGroupRows[m_usedGroupRow];
            }

            pParent.AddChild(pGroupRow);

            pGroupRow.SetTextVisible(false);
            pGroupRow.SetReference(pColumn, itemText);
            pGroupRow.m_groupLevel = 0;

            auto itor = m_mapCache.find(pGroupRow.GetKey());
            if (itor == m_mapCache.end())
            {
                GrPadding padding = pGroupRow.GetPadding();
                int height = GetPaintingFont().GetHeight() + padding.GetVertical();
                pGroupRow.SetHeight(height);
                pGroupRow.Expand(true);
            }
            else
            {
                pGroupRow.SetHeight(itor.second.height);
                pGroupRow.Expand(itor.second.expanded);
            }

            this.GridCore.AttachObject(pGroupRow);

            m_usedGroupRow++;
            return pGroupRow;
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

        private void BuildGroup(GrRow pParent, uint groupLevel)
        {
            GrGroupPanel pGroupPanel = this.GridCore.GetGroupPanel();
            this.GridCore.AttachObject(pGroupPanel);
            GrColumn pColumn = pGroupPanel.GetGroup(groupLevel).GetColumn();

            List<GrDataRow> vecSort;

            FuncSortRow fnSort;
            switch (pColumn.GetGroup().GetSortType())
            {
                case GrSort_Up:
                    fnSort = GrSortFunc.SortDataRowSortUp;
                    break;
                default:
                    fnSort = GrSortFunc.SortDataRowSortDown;
                    break;
            }
            pParent.Sort(fnSort, (void*)pColumn);

            vecSort.reserve(pParent.GetChildCount());
            for (int i = 0; i < pParent.GetChildCount(); i++)
            {
                GrRow pChild = pParent.GetChild(i);
                if (pChild.GetRowType() == GrRowType_DataRow)
                    vecSort.Add((GrDataRow*)pChild);
            }

            pParent.ClearChild();

            std.wstring itemText;
            auto itor = vecSort.begin();
            GrGroupRow pGroupRow = null;

            while (itor != vecSort.end())
            {
                GrDataRow pRow = *itor;
                GrItem pItem = pRow.GetItem(pColumn);
                std.wstring nextItemText = pItem.GetText();

                if (itemText != nextItemText || itor == vecSort.begin())
                {
                    itemText = nextItemText;
                    pGroupRow = CreateGroupRow(pParent, pColumn, itemText.c_str());
                }
                pGroupRow.AddChild(pRow);
                itor++;
            }

            uint nextGroupLevel = groupLevel + 1;
            for (int i = 0; i < pParent.GetChildCount(); i++)
            {
                GrGroupRow pRow = (GrGroupRow*)pParent.GetChild(i);
                pRow.ProcessAfterGroup();
                if (nextGroupLevel < m_groupCount)
                    BuildGroup(pRow, nextGroupLevel);
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
                m_usedGroupRow = 0;
                m_mapCache.Clear();

                GrColumn pColumn = this.GridCore.GetColumnList().GetFirstSortColumn();
                if (pColumn)
                    Sort(pColumn);
                else
                    base.Sort(GrSortFunc.SortDataRowSortNone, null);
            }
            else
            {
                BuildCache();

                foreach (var value in m_vecGroupRows)
                {
                    value.ClearChild();
                }
                m_usedGroupRow = 0;
                BuildGroup(this, 0);
            }

            SetVisibleChanged();
        }

        private void BuildVisibleRowList()
{
	if(m_updating == true)
		return;
	m_updating = true;
	GrRowArray vecVisibles;
	vecVisibles.reserve(this.GridCore.GetReservedRow());
	uint level = 0;
	GetVisibleList(this, &vecVisibles);
	GetMaxDepth(this, &level);

	uint temp = level - GetDepth() ;
	if(temp <= 1)
		m_margin = 0;
	else
		m_margin = (temp - 1) * DEF_GROUP_WIDTH;

	std.set<IDataRow*> oldVisibles(m_vecVisibleRows.begin(), m_vecVisibleRows.end());

	m_vecVisibleRows.Clear();
	m_vecVisibleRows.reserve(vecVisibles.Count);
	m_vecVisibleDataRows.Clear();
	m_vecVisibleDataRows.reserve(vecVisibles.Count);

	foreach(var value in vecVisibles)
	{
		IDataRow pDataRowBase = value as IDataRow;
		if(pDataRowBase == null)
			continue;
		GrDataRow pDataRow = pDataRowBase as GrDataRow;
		if(pDataRow)
		{
			pDataRow.SetVisibleDataRowIndex(m_vecVisibleDataRows.Count);
			m_vecVisibleDataRows.Add(pDataRow);
		}
		pDataRowBase.SetVisibleIndex(m_vecVisibleRows.Count);
		m_vecVisibleRows.Add(pDataRowBase);

		oldVisibles.Remove(pDataRowBase);
	}

	foreach(var value in oldVisibles)
	{
		value.SetVisibleIndex(INVALID_INDEX);
	}

	foreach(var value in m_vecDisplayableRows)
	{
		value.SetDisplayable(false);
		value.SetDisplayIndex(INVALID_INDEX);
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
            m_clippedIndex = INVALID_INDEX;

            if (m_visibleHeight != oldVisibleHeight)
            {
                OnVisibleHeightChanged(&GrEventArgs.Empty);
            }
        }

        private void BuildCache()
        {
            m_mapCache.Clear();
            for (int i = 0; i < m_usedGroupRow; i++)
            {
                GrGroupRow pGroupRow = m_vecGroupRows[i];

                GrCache cache;
                cache.height = pGroupRow.GetHeight();
                cache.expanded = pGroupRow.IsExpanded();

                m_mapCache.insert(_MapCaches.value_type(pGroupRow.m_key, cache));
            }
        }

        private void DeleteObjects()
        {
            foreach (var value in m_vecDataRows)
            {
                delete value;
            }

            foreach (var value in m_vecDataRowsRemoved)
            {
                delete value;
            }

            foreach (var value in m_vecGroupRows)
            {
                delete value;
            }

            m_vecDataRows.Clear();
            m_vecDataRowsRemoved.Clear();
            m_vecGroupRows.Clear();
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
        private void GetVisibleList(GrRow pRow, List<GrRow> pVisible)
        {
            if (pRow.GetVisible() == false)
                return;
            pVisible.Add(pRow);
            for (int i = 0; i < pRow.GetChildCount(); i++)
            {
                GrRow pChild = pRow.GetChild(i);
                GetVisibleList(pChild, pVisible);
            }
        }

        private void GetMaxDepth(GrRow pRow, uint depth)
        {
            *depth = Math.Max(*depth, pRow.GetDepth());
            for (int i = 0; i < pRow.GetChildCount(); i++)
            {
                GrRow pChild = pRow.GetChild(i);

                GetMaxDepth(pChild, depth);
            }
        }

        private void groupPanel_Changed(object pSender, EventArgs e)
        {
            GrGroupPanel pGroupPanel = this.GridCore.GetGroupPanel();
            m_groupCount = pGroupPanel.GetGroupCount();
            m_clippedIndex = INVALID_INDEX;

            uint groupCount = pGroupPanel.GetGroupCount();
            if (groupCount == 0)
                m_margin = 0;
            else
                m_margin = (groupCount + 1) * DEF_GROUP_WIDTH;

            BuildChildRowList();
        }

        private void groupPanel_Expanded(object pSender, GrGroupEventArgs e)
        {
            GrColumn pColumn = e.m_pGroup.GetColumn();
            foreach (var value in m_vecGroupRows)
            {
                if (value.GetColumn() == pColumn)
                    value.Expand(e.m_pGroup.GetExpanded());
            }
            SetVisibleChanged();
        }

        private void groupPanel_SortChanged(object pSender, GrGroupEventArgs e)
        {
            GrColumn pColumn = e.m_pGroup.GetColumn();

            std.set<GrRow*> parentRows;
            for (int i = 0; i < m_usedGroupRow; i++)
            {
                GrGroupRow pGroupRow = m_vecGroupRows[i];

                if (pGroupRow.GetColumn() == pColumn)
                    parentRows.insert(pGroupRow.GetParent());
            }

            foreach (var value in parentRows)
            {
                value.Sort(e.m_pGroup.GetSortType());
            }
            SetVisibleChanged();
        }

        private void gridCore_Created(object pSender, EventArgs e)
{
	GrColumnList pColumnList = this.GridCore.GetColumnList();
	pColumnList.ColumnInserted+=columnList_ColumnInserted);
	pColumnList.ColumnRemoved+=columnList_ColumnRemoved);
	pColumnList.ColumnSortTypeChanged+=columnList_ColumnSortTypeChanged);
	pColumnList.ColumnPaddingChanged+=columnList_ColumnPaddingChanged);
}

        private void gridCore_Cleared(object pSender, EventArgs e)
        {
            ClearChild();

            m_groupCount = 0;
            m_usedGroupRow = 0;
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
        private void gridCore_FontChanged(object pSender, EventArgs e)
        {
            GrColumnList pColumnList = this.GridCore.GetColumnList();
            List<GrDataRow> vecDataRows = m_vecDataRows;
            vecDataRows.Add(GetInsertionRow());

            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();

            foreach (var value in vecDataRows)
            {
                GrDataRow pDataRow = value;
                pTextUpdater.AddTextBounds(pDataRow);

                for (int i = 0; i < pColumnList.GetColumnCount(); i++)
                {
                    GrColumn pColumn = pColumnList.GetColumn(i);
                    GrItem pItem = pDataRow.GetItem(pColumn);
                    pTextUpdater.AddTextBounds(pItem);
                }
            }

            foreach (var value in m_vecGroupRows)
            {
                pTextUpdater.AddTextBounds(value);
                pTextUpdater.AddTextBounds(value.GetLabel());
            }
        }

        private void columnList_ColumnInserted(object pSender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();

            if (pColumn.GetColumnID() >= m_vecColumns.Count)
            {
                m_vecColumns.Add(pColumn);
            }

            GetInsertionRow().AddItem(pColumn);
            foreach (var value in m_vecDataRows)
            {
                value.AddItem(pColumn);
            }

            foreach (var value in m_vecDataRowsRemoved)
            {
                value.AddItem(pColumn);
            }
        }

        private void columnList_ColumnRemoved(object pSender, GrColumnEventArgs e)
        {
            GrItem pItem = GetInsertionRow().GetItem(e.GetColumn());
            this.GridCore.DetachObject(pItem);

            foreach (var value in m_vecDataRows)
            {
                pItem = value.GetItem(e.GetColumn());
                this.GridCore.DetachObject(pItem);
            }
        }

        private void columnList_ColumnSortTypeChanged(object pSender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();
            Sort(pColumn);
        }

        private void columnList_ColumnPaddingChanged(object pSender, GrColumnEventArgs e)
        {
            m_pInsertionRow.SetFit();
            foreach (var value in m_vecDataRows)
            {
                value.SetFit();
            }
        }

        private void focuser_FocusedChanged(object pSender, GrFocusChangeArgs e)
        {
            GrFocuser pFocuser = pSender as GrFocuser;
            m_pFocusedDataRow = pFocuser.GetFocusedRow() as GrDataRow;
        }


        struct GrCache
        {
            public int height;
            public bool expanded;
        }

    }
}
