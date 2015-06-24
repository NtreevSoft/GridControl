using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumn : GrCell
    {
        GrColumnList m_columnList;


        int m_x;
        int m_width;
        int m_fitWidth;

        int m_defaultWidth;
        int m_minWidth;
        int m_maxWidth;

        bool m_visible;
        bool m_readOnly;
        bool m_sortable;
        bool m_groupable;
        bool m_movable;
        bool m_resizable;
        internal bool m_frozen;
        internal int m_selected;
        bool m_fitting;
        bool m_grouped;

        int m_displayIndex;
        internal int m_visibleIndex;
        internal int m_visibleIndexCore;
        int m_selectedIndex;
        internal int m_index;
        int m_columnID;

        bool m_displayable;
        bool m_clipped;

        GrItemType m_itemType;
        GrItemTypeShow m_itemTypeShow;
        GrHorzAlign m_itemHorzAlign;
        GrVertAlign m_itemVertAlign;
        bool m_itemWordWrap;
        bool m_itemMultiline;
        GrColor m_itemBackColor;
        GrColor m_itemForeColor;
        GrPadding m_itemPadding;
        GrFont m_pItemFont;
        GrClickEditing m_itemClickEditing;
        bool m_itemTextVisible;
        bool m_itemIcon;
        GrSize m_itemMinSize;

        GrGroup m_pGroup;

        GrSort m_sortType;
        GrFuncSortRow[] m_comparer;

        string m_tooltip;


        public GrColumn()
        {
            m_columnList = null;
            m_visible = true;
            m_readOnly = false;
            m_sortable = true;
            m_groupable = true;
            m_movable = true;
            m_resizable = true;
            m_frozen = false;
            m_selected = 0;
            m_fitting = false;
            m_displayable = false;
            m_clipped = false;
            m_grouped = false;

            m_visibleIndex = -1;
            m_visibleIndexCore = -1;
            m_displayIndex = -1;
            m_index = -1;
            m_columnID = -1;

            m_x = 0;
            m_width = 100;
            m_minWidth = 0;
            m_maxWidth = 0;
            m_sortType = GrSort.None;
            m_fitWidth = 0;

            //m_freezablePriority = GetID();
            //m_priority = GetID();

            m_itemType = GrItemType.Control;
            m_itemTypeShow = GrItemTypeShow.SelectedOnly;
            m_itemHorzAlign = GrHorzAlign.Left;
            m_itemVertAlign = GrVertAlign.Top;
            m_itemWordWrap = false;
            m_itemMultiline = false;
            m_itemBackColor = GrColor.Empty;
            m_itemForeColor = GrColor.Empty;
            m_itemPadding = GrPadding.Empty;
            m_pItemFont = null;
            m_itemClickEditing = GrClickEditing.Default;
            m_itemTextVisible = true;
            m_itemIcon = false;

            m_comparer = new GrFuncSortRow[Enum.GetNames(typeof(GrSort)).Length];
            //m_comparer[GrSort_None] = 0;
            //m_comparer[GrSort_Up] = 0;
            //m_comparer[GrSort_Down] = 0;

            m_customItemPaint = false;

            m_fnColumnBackgroundPaint = null;
            m_fnColumnContentsPaint = null;
            m_columnPaintData = null;

            m_pGroup = new GrGroup(this);

        }

        public GrColumnList GetColumnList()
        {
            return m_columnList;
        }

        public GrItemType GetItemType()
        {
            return m_itemType;
        }

        public GrItemTypeShow GetItemTypeShow()
        {
            return m_itemTypeShow;
        }

        public GrHorzAlign GetItemHorzAlign()
        {
            return m_itemHorzAlign;
        }

        public GrVertAlign GetItemVertAlign()
        {
            return m_itemVertAlign;
        }

        public bool GetItemWordWrap()
        {
            return m_itemWordWrap;
        }

        public bool GetItemMultiline()
        {
            return m_itemMultiline;
        }

        public GrColor GetItemForeColor()
        {
            return m_itemForeColor;
        }

        public GrColor GetItemBackColor()
        {
            return m_itemBackColor;
        }

        public GrFont GetItemFont()
        {
            return m_pItemFont;
        }

        public GrPadding GetItemPadding()
        {
            return m_itemPadding;
        }

        public GrClickEditing GetItemClickEditing()
        {
            return m_itemClickEditing;
        }

        public bool GetItemTextVisible()
        {
            return m_itemTextVisible;
        }

        public bool GetItemIcon()
        {
            return m_itemIcon;
        }

        public GrSize GetItemMinSize()
        {
            return m_itemMinSize;
        }

        public int GetDisplayIndex()
        {
            return m_displayIndex;
        }

        public int GetVisibleIndex()
        {
            return m_visibleIndexCore;
        }

        public void SetVisibleIndex(int index)
        {
            m_visibleIndex = index;

            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnVisibleIndexChanged", e);
            }
        }

        public int GetFrozenIndex()
        {
            if (m_frozen == true)
                throw new Exception();
            return m_visibleIndexCore;
        }

        public int GetUnfrozenIndex()
        {
            if (m_frozen == false)
                throw new Exception();
            return m_visibleIndexCore - this.GridCore.ColumnList.GetFrozenColumnCount();
        }

        public int GetIndex()
        {
            return m_index;
        }

        public int GetColumnID()
        {
            return m_columnID;
        }

        public int GetResizingMargin()
        {
            GrFont pFont = this.GetPaintingFont();
            int margin = (int)((float)pFont.GetHeight() * 0.75f);

            int width = this.GetWidth();
            if (margin * 3 > width)
                margin = (int)((float)width / 3.0f);

            return margin;
        }

        public void SetItemType(GrItemType type)
        {
            if (m_itemType == type)
                return;
            m_itemType = type;
        }

        public void SetItemHorzAlign(GrHorzAlign horzAlign)
        {
            if (m_itemHorzAlign == horzAlign)
                return;
            m_itemHorzAlign = horzAlign;
            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnHorzAlignChanged", e);
            }
        }

        public void SetItemVertAlign(GrVertAlign vertAlign)
        {
            if (m_itemVertAlign == vertAlign)
                return;
            m_itemVertAlign = vertAlign;
            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnVertAlignChanged", e);
            }
        }

        public void SetItemWordWrap(bool b)
        {
            if (m_itemWordWrap == b)
                return;
            m_itemWordWrap = b;
            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnWordwrapChanged", e);
            }
        }

        public void SetItemMultiline(bool b)
        {
            if (m_itemMultiline == b)
                return;
            m_itemMultiline = b;
            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnMultilineChanged", e);
            }
        }

        public void SetItemForeColor(GrColor color)
        {
            m_itemForeColor = color;
            if (this.GridCore != null)
                this.GridCore.Invalidate();
        }

        public void SetItemBackColor(GrColor color)
        {
            m_itemBackColor = color;
            if (this.GridCore != null)
                this.GridCore.Invalidate();
        }

        public void SetItemFont(GrFont pFont)
        {
            m_pItemFont = pFont;
            if (this.GridCore != null)
                this.GridCore.Invalidate();
        }

        public void SetItemPadding(GrPadding padding)
        {
            m_itemPadding = padding;
            if (this.GridCore != null)
                this.GridCore.Invalidate();
            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnPaddingChanged", e);
            }
        }

        public void SetItemClickEditing(GrClickEditing clickEditing)
        {
            m_itemClickEditing = clickEditing;
        }

        public void SetItemTextVisible(bool b)
        {
            m_itemTextVisible = b;
        }

        public void SetItemIcon(bool b)
        {
            m_itemIcon = b;
        }

        public void SetItemMinSize(GrSize size)
        {
            m_itemMinSize = size;
        }

        public bool GetMovable()
        {
            return m_movable;
        }

        public void SetMovable(bool b)
        {
            m_movable = b;
        }

        public bool GetResizable()
        {
            return m_resizable;
        }

        public void SetResizable(bool b)
        {
            m_resizable = b;
        }

        public bool GetFrozen()
        {
            return m_frozen;
        }

        public void SetFrozen(bool b)
        {
            if (m_frozen == b)
                return;
            m_frozen = b;
            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnFrozenChanged", e);
            }
        }

        public bool GetReadOnly()
        {
            if (this.GridCore != null && this.GridCore.GetReadOnly() == true)
                return true;
            return m_readOnly;
        }

        public void SetReadOnly(bool b)
        {
            if (m_readOnly == b)
                return;
            m_readOnly = b;
        }

        public void SetVisible(bool b)
        {
            if (m_visible == b)
                return;

            if (this.GridCore != null)
            {
                if (b == false)
                {
                    GrFocuser focuser = this.GridCore.Focuser;
                    if (focuser.GetFocusedColumn() == this)
                        focuser.Set(null as IFocusable);
                    if (GetSelected() == true)
                        SetSelected(false);
                }

                m_columnList.SetVisibleChanged();
            }

            m_visible = b;
        }

        public void SetMinWidth(int minWidth)
        {
            if (minWidth < 0)
                minWidth = 0;

            m_minWidth = minWidth;

            if (m_width < m_minWidth)
                this.SetWidth(m_minWidth);
        }

        public void SetMaxWidth(int maxWidth)
        {
            if (maxWidth < 0)
                maxWidth = 0;

            m_maxWidth = maxWidth;

            if (m_width > maxWidth)
                this.SetWidth(maxWidth);
        }

        public int GetMinWidth()
        {
            return m_minWidth;
        }

        public int GetMaxWidth()
        {
            return m_maxWidth;
        }

        public void SetSelected(bool b)
        {
            if (this.GridCore == null)
                return;

            if (GetSelected() == b)
                return;

            GrItemSelector pItemSelector = this.GridCore.ItemSelector;
            if (b == true)
                pItemSelector.SelectColumn(this, GrSelectionType.Add);
            else
                pItemSelector.SelectColumn(this, GrSelectionType.Remove);
        }

        public bool GetSelected()
        {
            return m_selected > 0;
        }

        public bool GetFullSelected()
        {
            if (this.GridCore == null)
                return false;
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            if (dataRowList.GetInsertionRow().GetSelected() == true)
                return false;
            int visibles = (int)dataRowList.GetVisibleDataRowCount();
            if (visibles == 0)
                return false;
            return m_selected == visibles;
        }

        public bool GetSelecting()
        {
            if (this.GridCore == null)
                return false;
            GrItemSelectorInternal pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
            return pItemSelector.IsSelecting(this);
        }

        public bool HasFocused()
        {
            GrFocuser focuser = this.GridCore.Focuser;
            if (focuser.GetFocusedColumn() == this)
                return true;
            return false;
        }

        public void KillFocus()
        {
            if (this.HasFocused() == true)
                return;
            this.GridCore.Focuser.Reset();
        }

        public void AdjustWidth()
        {
            if (m_fitting == false)
                return;

            GrDataRowList dataRowList = this.GridCore.DataRowList;

            int width = m_minWidth;

            if (this.GridCore.GetAutoFitColumnType() == GrAutoFitColumnType.ColumnIncluded)
            {
                int columnWidth = GetPreferredSize().Width;
                width = Math.Max(m_minWidth, columnWidth);
            }

            for (int i = 0; i < dataRowList.GetVisibleDataRowCount(); i++)
            {
                GrDataRow pDataRow = dataRowList.GetVisibleDataRow(i);
                GrItem pItem = pDataRow.GetItem(this);
                int itemWidth = pItem.GetPreferredSize().Width;
                width = Math.Max(width, itemWidth);
            }

            // insertion Row
            {
                GrDataRow pDataRow = this.GridCore.InsertionRow;
                GrItem pItem = pDataRow.GetItem(this);
                int itemWidth = pItem.GetPreferredSize().Width;
                width = Math.Max(width, itemWidth);
            }

            if (GetItemType() != GrItemType.Control)
                width += GrDefineUtility.DEF_CONTROL_WIDTH;

            if (m_maxWidth != 0)
                width = Math.Min(width, m_maxWidth);

            if (m_width != width)
            {
                m_fitWidth = m_width = width;

                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnWidthChanged", e);
            }
            m_fitting = false;
        }

        public void SetFit()
        {
            if (m_fitting == true)
                return;
            m_fitting = true;
            m_columnList.SetFitChanged();
        }

        public void SetSortable(bool b)
        {
            m_sortable = b;
        }

        public bool GetSortable()
        {
            return m_sortable;
        }

        public void SetGroupable(bool b)
        {
            m_groupable = b;
        }

        public bool GetGroupable()
        {
            return m_groupable;
        }

        public void SetClipped(bool b)
        {
            m_clipped = b;
        }

        public bool GetClipped()
        {
            return m_clipped;
        }

        public bool ShouldBringIntoView()
        {
            if (m_displayable == false || m_clipped == true)
                return true;
            if (m_frozen == true)
                return false;
            return false;
        }

        public void SetWidth(int width)
        {
            if (m_minWidth != 0)
                width = Math.Max(width, m_minWidth);
            if (m_maxWidth != 0)
                width = Math.Min(width, m_maxWidth);

            if (m_width == width)
                return;

            m_width = width;

            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnWidthChanged", e);

                GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
                pTextUpdater.AddTextBoundsByColumn(this);
            }
        }

        public void SetTooltip(string tooltip)
        {
            m_tooltip = tooltip;
        }

        public string GetTooltip()
        {
            return m_tooltip;
        }

        public bool GetGrouped()
        {
            return m_grouped;
        }

        public void SetGrouped(bool b)
        {
            if (m_grouped == b)
                return;

            m_grouped = b;
            GroupChanged(this, EventArgs.Empty);
        }

        public GrGroup GetGroup()
        {
            return m_pGroup;
        }

        public void SetSortType(GrSort sortType)
        {
            if (m_sortable == false)
                return;
            if (m_columnList == null)
                return;
            m_sortType = sortType;
            if (m_index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                m_columnList.Invoke("ColumnSortTypeChanged", e);
            }
        }

        public GrSort GetSortType()
        {
            if (m_columnList == null)
                return GrSort.None;
            if (m_columnList.GetFirstSortColumn() != this)
                return GrSort.None;
            return m_sortType;
        }

        public void SetSortComparer(GrSort sortType, GrFuncSortRow comparer)
        {
            m_comparer[(int)sortType] = comparer;
        }

        public GrFuncSortRow GetSortComparer(GrSort sortType)
        {
            return m_comparer[(int)sortType];
        }

        public override GrCellType GetCellType() { return GrCellType.Column; }

        public override int GetX()
        {
            return m_x;
        }

        public override int GetY()
        {
            if (m_columnList == null)
                return 0;
            return m_columnList.GetY();
        }

        public override int GetWidth()
        {
            return m_width;
        }

        public override int GetHeight()
        {
            if (m_columnList == null)
                return 0;
            return m_columnList.GetHeight();
        }

        public override bool GetVisible()
        {

            return m_visible;
        }

        public override GrHorzAlign GetTextHorzAlign()
        {
            return GrHorzAlign.Center;
        }

        public override GrVertAlign GetTextVertAlign()
        {
            return GrVertAlign.Center;
        }

        public override GrColor GetForeColor()
        {
            GrColor color = base.GetForeColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetColumnForeColor();

            return base.GetForeColor();
        }

        public override GrColor GetBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetColumnBackColor();

            return base.GetBackColor();
        }

        public override GrColor GetLineColor()
        {
            GrColor color = base.GetLineColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetColumnLineColor();

            return base.GetLineColor();
        }

        public override GrFont GetFont()
        {
            GrFont pFont = base.GetFontCore();
            if (pFont != GrFont.Empty)
                return pFont;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetColumnFont();

            return base.GetFont();
        }

        public override GrRow GetRow()
        {
            return m_columnList;
        }

        public override GrPaintStyle ToPaintStyle()
        {
            GrPaintStyle flag = base.ToPaintStyle();
            if (GetSelecting() == true || GetSelected() == true)
                flag |= GrPaintStyle.Selected;
            if (HasFocused() == true)
                flag |= GrPaintStyle.Focused;
            if (GetClipped() == true)
                flag &= ~GrPaintStyle.RightLine;

            return flag;
        }

        public override GrColor GetPaintingForeColor()
        {
            GrColor foreColor = base.GetPaintingForeColor();
            return foreColor;
        }

        public override GrColor GetPaintingBackColor()
        {
            if (this.GridCore == null)
                return base.GetPaintingBackColor();

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle == null)
                pStyle = GrStyle.Default;
            GrColor backColor;
            if (GetGrouped() == true)
                backColor = pStyle.GetGroupBackColor(GetGroup().GetGroupLevel());
            else
                backColor = base.GetPaintingBackColor();

            return backColor;
        }

        public override bool GetDisplayable()
        {
            if (this.GetVisible() == false)
                return false;
            return m_displayable;
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();

            if (m_fnColumnBackgroundPaint == null || m_fnColumnBackgroundPaint(painter, this, paintRect, m_columnPaintData) == false)
            {
                GrPaintStyle paintStyle = this.ToPaintStyle();
                GrColor backColor = GetPaintingBackColor();
                if (GetClipped() == true)
                    painter.DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, clipRect);
                else
                    painter.DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);
            }

            if (m_fnColumnContentsPaint == null || m_fnColumnContentsPaint(painter, this, paintRect, m_columnPaintData) == false)
            {
                GrColor foreColor = GetPaintingForeColor();

                GrSort sortType = GetSortType();
                GrRect textRect = paintRect;
                if (sortType != GrSort.None)
                {

                    int right = paintRect.Right - 6;
                    int left = right - 10;
                    int top = (paintRect.Bottom + paintRect.Top) / 2 - 5;
                    int bottom = top + 10;

                    GrRect sortRect = GrRect.FromLTRB(left, top, right, bottom);
                    painter.DrawSortGlyph(sortRect, sortType);

                    //textRect.Right = sortRect.Left;
                    textRect.Width = sortRect.Left - textRect.Left;
                }

                if (GetTextLineCount() > 0)
                {
                    GrLineDesc cl = GetTextLine(0);
                    if (GetClipped() == true)
                        painter.DrawColumnText(GetPaintingFont(), GetText() + cl.textBegin, cl.length, textRect, foreColor, clipRect);
                    else
                        painter.DrawColumnText(GetPaintingFont(), GetText() + cl.textBegin, cl.length, textRect, foreColor, null);
                }
            }
        }

        public event EventHandler GroupChanged;

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            m_columnList = this.GridCore.ColumnList;
            this.GridCore.AttachObject(m_pGroup);
        }

        protected override void OnGridCoreDetached()
        {
            if (this.GridCore.GetMouseOver() == this)
                this.GridCore.SetMouseOver(null, GrPoint.Empty);

            if (this.GridCore.GetMousePress() == this)
                this.GridCore.SetMouseUnpress();

            base.OnGridCoreDetached();
            this.GridCore.DetachObject(m_pGroup);
            m_columnList = null;
        }

        protected override void OnTextChanged()
        {
            base.OnTextChanged();
            if (this.GridCore == null)
                return;

            if (this.GetDisplayable() == false)
                return;
            this.GridCore.Invalidate(GetRect());
        }

        internal void SetX(int x) { m_x = x; }

        internal void SetDisplayable(bool b)
        {
            m_displayable = b;
        }

        internal void SetDisplayIndex(int index)
        {
            m_displayIndex = index;
        }

        internal void SetIndex(int index)
        {
            m_index = index;
        }

        internal void SetColumnID(int id)
        {
            m_columnID = id;
        }

        public bool m_customItemPaint;

        public GrFuncColumnPaint m_fnColumnBackgroundPaint;

        public GrFuncColumnPaint m_fnColumnContentsPaint;

        public object m_columnPaintData;

        class SortColumnByVisible : IComparer<GrColumn>
        {
            public int Compare(GrColumn x, GrColumn y)
            {
                throw new NotImplementedException();
            }
        }


        //#ifdef _MANAGED
        //    friend ref class Ntreev.Windows.Forms.Grid.Column;
        //private:
        public bool ShouldSerializeWidth()
        {
            if (m_width == 100)
                return false;
            return m_width != m_fitWidth;
        }

        public bool ShouldSerializeVisibleIndex()
        {
            return m_visibleIndex != GrDefineUtility.INVALID_INDEX;
        }
    }
}
