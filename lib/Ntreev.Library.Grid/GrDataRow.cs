using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrDataRow : IDataRow
    {
        private readonly List<GrItem> m_vecItems = new List<GrItem>();
        private bool m_readOnly;

        private GrColor m_itemBackColor;
        private GrColor m_itemForeColor;
        private GrFont m_pItemFont;

        private int m_visibleDataRowIndex;
        private int m_dataRowIndex;
        private int m_dataRowID;

        private GrItem[] m_pItems;
        private int m_itemsCount;

        internal int m_selected;

        public GrDataRow()
        {
            SetDisplayable(false);

            m_readOnly = false;
            m_selected = 0;
            m_pItemFont = null;
            m_dataRowIndex = -1;
            m_dataRowID = -1;

            m_itemBackColor = GrColor.Empty;
            m_itemForeColor = GrColor.Empty;

            m_pItems = null;
            m_itemsCount = 0;
        }

        public GrItem GetItem(GrColumn column)
        {
            int columnID = column.GetColumnID();
            return m_vecItems[columnID];
        }

        public bool GetReadOnly()
        {
            if (this.GridCore != null && this.GridCore.GetReadOnly() == true)
                return true;
            return m_readOnly;
        }

        public void SetReadOnly(bool b = true)
        {
            m_readOnly = b;
        }

        public void SetSelected(bool b)
        {
            if (GetSelected() == b)
                return;
            GrItemSelector pItemSelector = this.GridCore.GetItemSelector();
            if (b == true)
                pItemSelector.SelectDataRow(this, GrSelectionType.Add);
            else
                pItemSelector.SelectDataRow(this, GrSelectionType.Remove);
        }

        public bool GetSelected()
        {
            return m_selected > 0;
        }

        public bool GetSelecting()
        {
            GrItemSelectorInternal pItemSelector = this.GridCore.GetItemSelector() as GrItemSelectorInternal;
            return pItemSelector.IsSelecting(this);
        }

        public int GetVisibleDataRowIndex()
        {
            if (GetVisible() == false)
                return -1;
            return m_visibleDataRowIndex;
        }

        public int GetDataRowIndex()
        {
            return m_dataRowIndex;
        }

        public void SetDataRowIndex(int index)
        {
            if (m_pDataRowList == null)
                return;
            m_pDataRowList.MoveDataRow(this, index);
        }

        public int GetDataRowID()
        {
            return m_dataRowID;
        }

        public bool IsInsertionRow()
        {
            return m_dataRowID == GrDefineUtility.INSERTION_ROW;
        }

        public GrColor GetItemBackColor()
        {
            return m_itemBackColor;
        }

        public GrColor GetItemForeColor()
        {
            return m_itemForeColor;
        }

        public GrFont GetItemFont()
        {
            return m_pItemFont;
        }

        public void SetItemBackColor(GrColor color)
        {
            if (m_itemBackColor == color)
                return;
            m_itemBackColor = color;
            InvalidateRow();
        }

        public void SetItemForeColor(GrColor color)
        {
            if (m_itemForeColor == color)
                return;
            m_itemForeColor = color;
            InvalidateRow();
        }

        public void SetItemFont(GrFont pFont)
        {
            if (m_pItemFont == pFont)
                return;
            m_pItemFont = pFont;
            InvalidateRow();
        }

        public GrColor GetPaintingItemBackColor()
        {
            GrColor color = GetItemBackColor();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemBackColors.Count > 0)
                return pStyle.GetItemBackColor(GetVisibleDataRowIndex());

            return base.GetBackColor();
        }

        public override bool GetFullSelected()
        {
            if (this.GridCore == null)
                return false;
            int visibleColumns = (int)this.GridCore.GetColumnList().GetVisibleColumnCount();
            return m_selected == visibleColumns;
        }

        public virtual void SetVisible(bool b)
        {
            if (m_visible == b)
                return;

            m_visible = b;

            if (m_pDataRowList == null)
                return;

            m_pDataRowList.SetVisibleChanged();
            GrFocuser focuser = this.GridCore.GetFocuser();
            if (focuser.GetFocusedRow() == this)
                focuser.Set(null as IFocusable);

            GrRootRow pHeaderList = GetParent() as GrRootRow;

            if (pHeaderList != null)
            {
                pHeaderList.SetVisibleChanged();
            }

            if (m_visible == false && GetSelected() == true)
            {
                GrItemSelector pItemSelector = this.GridCore.GetItemSelector();
                pItemSelector.SelectDataRow(this, GrSelectionType.Remove);
            }
        }

        public override GrRowType GetRowType() { return GetDataRowID() == GrDefineUtility.INSERTION_ROW ? GrRowType.InsertionRow : GrRowType.DataRow; }

        public override GrPaintStyle ToPaintStyle()
        {
            GrPaintStyle flag = base.ToPaintStyle();
            if (GetSelecting() == true || GetSelected() == true)
                flag |= GrPaintStyle.Selected;
            return flag;
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            base.Paint(painter, clipRect);

            GrRect paintRect = GetRect();
            GrPaintStyle paintStyle = ToPaintStyle();
            GrColor foreColor = GetPaintingForeColor();
            GrColor backColor = GetPaintingBackColor();

            if (this.GridCore.GetRowVisible() == true)
            {
                if (m_pDataRowList.GetRowNumberVisible() == true)
                {
                    if (GetClipped() == true)
                        DrawText(painter, foreColor, paintRect, clipRect);
                    else
                        DrawText(painter, foreColor, paintRect, null);
                }
            }

            GrColumnList columnList = this.GridCore.GetColumnList();
            GrRect displayRect = this.GridCore.GetDisplayRect();
            for (int i = 0; i < columnList.GetDisplayableColumnCount(); i++)
            {
                GrColumn column = columnList.GetDisplayableColumn(i);
                if (column.GetX() > clipRect.Right || column.GetRight() < clipRect.Left)
                    continue;
                GrItem pItem = GetItem(column);
                pItem.Paint(painter, clipRect);
            }

            if (this.GridCore.GetFillBlank() == true && columnList.GetDisplayableRight() < displayRect.Right)
            {
                GrRect pr = paintRect;
                int left = columnList.GetDisplayableRight();
                int right = displayRect.Right;
                pr.X = left;
                pr.Width = right - left;
                painter.DrawItem(GrPaintStyle.BottomLine, pr, GetPaintingLineColor(), GetBlankPaintingBackColor(), clipRect);
            }
        }

        public override IFocusable GetFocusable(GrColumn column)
        {
            return GetItem(column);
        }

        public override int GetMinHeight()
        {
            int height = base.GetMinHeight();

            foreach (var value in m_vecItems)
            {
                if (value.GetVisible() == false)
                    continue;
                height = Math.Max(height, value.GetPreferredSize().Height);
            }
            return height;
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            foreach (var value in m_vecItems)
            {
                this.GridCore.AttachObject(value);
            }
        }

        protected override void OnGridCoreDetached()
        {
            GrFocuser focuser = this.GridCore.GetFocuser();
            if (focuser.GetFocusedRow() == this)
                focuser.Set(null as IFocusable);
            SetSelected(false);

            foreach (var value in m_vecItems)
            {
                this.GridCore.DetachObject(value);
            }

            base.OnGridCoreDetached();
        }

        protected override void OnHeightAdjusted()
        {
            base.OnHeightAdjusted();
            if (this.GridCore == null)
                return;

            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            foreach (var value in m_vecItems)
            {
                if (value.IsGridCoreAttached() == true)
                    pTextUpdater.AddTextAlign(value);
            }
            this.GridCore.Invalidate();
        }

        protected override void OnHeightChanged()
        {
            base.OnHeightChanged();
            GrRootRow pHeaderRow = GetParent() as GrRootRow;
            if (pHeaderRow != null)
                pHeaderRow.SetHeightChanged();
        }

        protected override GrCell OnHitTest(int x)
        {
            GrColumnList columnList = this.GridCore.GetColumnList();
            for (int i = 0; i < columnList.GetDisplayableColumnCount(); i++)
            {
                GrColumn column = columnList.GetDisplayableColumn(i);
                if (column.ContainsHorz(x) == true)
                    return GetItem(column);
            }

            GrColumnSplitter pSplitter = columnList.GetColumnSplitter();

            if (pSplitter.ContainsHorz(x) == true)
                return pSplitter;

            return null;
        }

        internal void SetVisibleDataRowIndex(int index)
        {
            m_visibleDataRowIndex = index;
        }

        internal void SetDataRowIndexCore(int index)
        {
            string numberText;
            if (m_pDataRowList.GetZeroBasedRowIndex() == true)
                numberText = string.Format("{0}", index);
            else
                numberText = string.Format("{0}", index + 1);

            m_dataRowIndex = index;
            SetText(numberText);
        }

        internal void SetDataRowID(int index)
        {
            m_dataRowID = index;
        }

        internal void AddItem(GrColumn column)
        {
            int columnID = column.GetColumnID();
            GrItem pItem = null;
            if (columnID >= m_vecItems.Count)
            {
                if ((int)columnID < m_itemsCount)
                {
                    m_pItems[columnID] = new GrItem();
                    pItem = m_pItems[columnID];
                    pItem.m_column = column;
                    pItem.m_pDataRow = this;
                }
                else
                {
                    pItem = new GrItem(column, this);
                }

                m_vecItems.Add(pItem);
            }
            else
            {
                pItem = GetItem(column);
            }

            if (this.GridCore != null)
                this.GridCore.AttachObject(pItem);
        }

        internal void Reserve(int count)
        {
            m_vecItems.Capacity = count;
            m_itemsCount = count;
            if (count != 0)
            {
                //throw std.exception();
                m_pItems = new GrItem[count];
            }
        }

        internal void ClearItem()
        {
            foreach (var value in m_vecItems)
            {
                //if(value < m_pItems || value >= m_pItems + m_itemsCount)
                //    delete value;
            }
            m_vecItems.Clear();
            if (m_pItems != null)
            {
                //delete [] m_pItems;
                m_pItems = null;
            }
            m_itemsCount = 0;
        }

        private GrColor GetBlankBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            color = GetItemBackColor();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemBackColors.Count > 0)
                return pStyle.GetItemBackColor(GetVisibleDataRowIndex());

            return base.GetBackColor();
        }

        private GrColor GetBlankPaintingBackColor()
        {
            GrColor color = GetBlankBackColor();

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle == null)
                pStyle = GrStyle.Default;

            if (GetFullSelected() == true)
            {
                if (this.GridCore.GetSelectionVisible() == true)
                    color = pStyle.SelectedBackColor;
            }
            else if (HasFocused() == true &&
                this.GridCore.GetRowHighlight() == true &&
                this.GridCore.GetRowHighlightType() != GrRowHighlightType.Line)
            {
                color = pStyle.RowHighlightFillColor;
            }

            return color;
        }
    }
}
