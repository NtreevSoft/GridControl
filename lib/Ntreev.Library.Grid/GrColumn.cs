using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumn : GrCell
    {
        private GrColumnList columnList;

        //private int x;
        //private int width = 100;
        private int fitWidth;

        private int minWidth;
        private int maxWidth;

        private bool isReadOnly;
        private bool isSortable = true;
        private bool isGroupable = true;
        private bool isMovable = true;
        private bool isResizable = true;
        internal bool isFrozen;
        internal int selected;
        private bool fitting;
        private bool isGrouped;

        private int displayIndex = -1;
        internal int visibleIndex = -1;
        internal int visibleIndexCore = -1;
        internal int index = -1;
        private int columnID = -1;

        private bool displayable;
        private bool clipped;

        private GrItemType itemType;
        private GrItemTypeShow itemTypeShow = GrItemTypeShow.SelectedOnly;
        private GrHorzAlign itemHorzAlign;
        private GrVertAlign itemVertAlign;
        private bool itemWordWrap;
        private bool itemMultiline;
        private GrColor itemBackColor = GrColor.Empty;
        private GrColor itemForeColor = GrColor.Empty;
        private GrPadding itemPadding = GrPadding.Empty;
        private GrFont itemFont;
        private GrClickEditing itemClickEditing;
        private bool itemTextVisible = true;
        private bool itemIcon;
        private GrSize itemMinSize;

        private readonly GrGroup group;

        private GrSort sortType;
        private readonly GrFuncSortRow[] comparer;

        private string tooltip;

        public GrColumn()
        {
            //this.columnList = null;
            //this.sortable = true;
            //this.groupable = true;
            //this.movable = true;
            //this.isResizable = true;
            //this.frozen = false;
            //this.selected = 0;
            //this.fitting = false;
            //this.displayable = false;
            //this.clipped = false;
            //this.grouped = false;

            //this.visibleIndex = -1;
            //this.visibleIndexCore = -1;
            //this.displayIndex = -1;
            //this.index = -1;
            //this.columnID = -1;

            //this.x = 0;
            //this.width = 100;
            //this.minWidth = 0;
            //this.maxWidth = 0;
            //this.sortType = GrSort.None;
            //this.fitWidth = 0;

            ////this.freezablePriority = GetID();
            ////this.priority = GetID();

            //this.itemType = GrItemType.Control;
            //this.itemTypeShow = GrItemTypeShow.SelectedOnly;
            //this.itemHorzAlign = GrHorzAlign.Left;
            //this.itemVertAlign = GrVertAlign.Top;
            //this.itemWordWrap = false;
            //this.itemMultiline = false;
            //this.itemBackColor = GrColor.Empty;
            //this.itemForeColor = GrColor.Empty;
            //this.itemPadding = GrPadding.Empty;
            //this.pItemFont = null;
            //this.itemClickEditing = GrClickEditing.Default;
            //this.itemTextVisible = true;
            //this.itemIcon = false;

            this.comparer = new GrFuncSortRow[Enum.GetNames(typeof(GrSort)).Length];
            ////this.comparer[GrSort_None] = 0;
            ////this.comparer[GrSort_Up] = 0;
            ////this.comparer[GrSort_Down] = 0;

            //this.customItemPaint = false;

            //this.fnColumnBackgroundPaint = null;
            //this.fnColumnContentsPaint = null;
            //this.columnPaintData = null;

            this.group = new GrGroup(this);
            this.Width = 100;

        }

        public GrColumnList ColumnList
        {
            get { return this.columnList; }
        }

        public GrItemType ItemType
        {
            get { return this.itemType; }
            set
            {
                if (this.itemType == value)
                    return;
                this.itemType = value;
            }
        }

        public GrItemTypeShow ItemTypeShow
        {
            get { return this.itemTypeShow; }
        }

        public GrHorzAlign ItemHorzAlign
        {
            get { return this.itemHorzAlign; }
            set
            {
                if (this.itemHorzAlign == value)
                    return;
                this.itemHorzAlign = value;
                if (this.index != -1)
                {
                    GrColumnEventArgs e = new GrColumnEventArgs(this);
                    this.columnList.Invoke("ColumnHorzAlignChanged", e);
                }
            }
        }

        public GrVertAlign ItemVertAlign
        {
            get { return this.itemVertAlign; }
            set
            {
                if (this.itemVertAlign == value)
                    return;
                this.itemVertAlign = value;
                if (this.index != -1)
                {
                    GrColumnEventArgs e = new GrColumnEventArgs(this);
                    this.columnList.Invoke("ColumnVertAlignChanged", e);
                }
            }
        }

        public bool ItemWordWrap
        {
            get { return this.itemWordWrap; }
            set
            {
                if (this.itemWordWrap == value)
                    return;
                this.itemWordWrap = value;
                if (this.index != -1)
                {
                    GrColumnEventArgs e = new GrColumnEventArgs(this);
                    this.columnList.Invoke("ColumnWordwrapChanged", e);
                }
            }
        }

        public bool ItemMultiline
        {
            get { return this.itemMultiline; }
            set
            {
                if (this.itemMultiline == value)
                    return;
                this.itemMultiline = value;
                if (this.index != -1)
                {
                    GrColumnEventArgs e = new GrColumnEventArgs(this);
                    this.columnList.Invoke("ColumnMultilineChanged", e);
                }
            }
        }

        public GrColor ItemForeColor
        {
            get { return this.itemForeColor; }
            set
            {
                this.itemForeColor = value;
                if (this.GridCore != null)
                    this.GridCore.Invalidate();
            }
        }

        public GrColor ItemBackColor
        {
            get { return this.itemBackColor; }
            set
            {
                this.itemBackColor = value;
                if (this.GridCore != null)
                    this.GridCore.Invalidate();
            }
        }

        public GrFont ItemFont
        {
            get { return this.itemFont; }
            set
            {
                this.itemFont = value;
                if (this.GridCore != null)
                    this.GridCore.Invalidate();
            }
        }

        public GrPadding ItemPadding
        {
            get { return this.itemPadding; }
            set
            {
                this.itemPadding = value;
                if (this.GridCore != null)
                    this.GridCore.Invalidate();
                if (this.index != -1)
                {
                    GrColumnEventArgs e = new GrColumnEventArgs(this);
                    this.columnList.Invoke("ColumnPaddingChanged", e);
                }
            }
        }

        public GrClickEditing ItemClickEditing
        {
            get { return this.itemClickEditing; }
            set
            {
                this.itemClickEditing = value;
            }
        }

        public bool ItemTextVisible
        {
            get { return this.itemTextVisible; }
            set
            {
                this.itemTextVisible = value;
            }

        }

        public bool GetItemIcon()
        {
            return this.itemIcon;
        }

        public GrSize GetItemMinSize()
        {
            return this.itemMinSize;
        }

        public int GetDisplayIndex()
        {
            return this.displayIndex;
        }

        public int GetVisibleIndex()
        {
            return this.visibleIndexCore;
        }

        public void SetVisibleIndex(int index)
        {
            this.visibleIndex = index;

            if (this.index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                this.columnList.Invoke("ColumnVisibleIndexChanged", e);
            }
        }

        public int GetFrozenIndex()
        {
            if (this.isFrozen == true)
                throw new Exception();
            return this.visibleIndexCore;
        }

        public int GetUnfrozenIndex()
        {
            if (this.isFrozen == false)
                throw new Exception();
            return this.visibleIndexCore - this.GridCore.ColumnList.GetFrozenColumnCount();
        }

        public int GetIndex()
        {
            return this.index;
        }

        public int GetColumnID()
        {
            return this.columnID;
        }

        public int GetResizingMargin()
        {
            GrFont pFont = this.GetPaintingFont();
            int margin = (int)((float)pFont.GetHeight() * 0.75f);

            int width = this.Width;
            if (margin * 3 > width)
                margin = (int)((float)width / 3.0f);

            return margin;
        }

        public void SetItemIcon(bool b)
        {
            this.itemIcon = b;
        }

        public void SetItemMinSize(GrSize size)
        {
            this.itemMinSize = size;
        }

        public bool IsMovable
        {
            get { return this.isMovable; }
            set { this.isMovable = value; }
        }

        public bool IsResizable
        {
            get { return this.isResizable; }
            set { this.isResizable = value; }
        }

        public bool IsFrozen
        {
            get { return this.isFrozen; }
            set
            {
                if (this.isFrozen == value)
                    return;
                this.isFrozen = value;
                if (this.index != -1)
                {
                    GrColumnEventArgs e = new GrColumnEventArgs(this);
                    this.columnList.Invoke("ColumnFrozenChanged", e);
                }
            }
        }

        public bool IsReadOnly
        {
            get
            {
                if (this.GridCore != null && this.GridCore.GetReadOnly() == true)
                    return true;
                return this.isReadOnly;
            }
            set
            {
                if (this.isReadOnly == value)
                    return;
                this.isReadOnly = value;
            }
        }

        protected override void OnVisibleChanged(EventArgs e)
        {
            if (this.GridCore != null)
            {
                if (this.IsVisible == false)
                {
                    GrFocuser focuser = this.GridCore.Focuser;
                    if (focuser.GetFocusedColumn() == this)
                        focuser.Set(null as IFocusable);
                    if (this.IsSelected == true)
                        this.IsSelected = false;
                }

                this.columnList.SetVisibleChanged();
            }

            base.OnVisibleChanged(e);
        }

        public int MinWidth
        {
            get { return this.minWidth; }
            set
            {
                this.minWidth = Math.Max(value, 0);
                this.Width = this.Width;
            }
        }

        public int MaxWidth
        {
            get { return this.maxWidth; }
            set
            {
                this.maxWidth = Math.Max(value, 0);
                this.Width = this.Width;
            }
        }

        public bool IsSelected
        {
            get { return this.selected > 0; }
            set
            {
                if (this.GridCore == null)
                    return;

                if (this.IsSelected == value)
                    return;

                GrItemSelector pItemSelector = this.GridCore.ItemSelector;
                if (value == true)
                    pItemSelector.SelectColumn(this, GrSelectionType.Add);
                else
                    pItemSelector.SelectColumn(this, GrSelectionType.Remove);
            }
        }

        public bool IsFullSelected
        {
            get
            {
                if (this.GridCore == null)
                    return false;
                GrDataRowList dataRowList = this.GridCore.DataRowList;
                if (dataRowList.InsertionRow.IsSelected == true)
                    return false;
                int visibles = (int)dataRowList.GetVisibleDataRowCount();
                if (visibles == 0)
                    return false;
                return this.selected == visibles;
            }
        }

        public bool IsSelecting
        {
            get
            {
                if (this.GridCore == null)
                    return false;
                GrItemSelectorInternal pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
                return pItemSelector.IsSelecting(this);
            }
        }

        public bool HasFocused
        {
            get
            {
                GrFocuser focuser = this.GridCore.Focuser;
                if (focuser.GetFocusedColumn() == this)
                    return true;
                return false;
            }
        }

        public void KillFocus()
        {
            if (this.HasFocused == true)
                return;
            this.GridCore.Focuser.Reset();
        }

        public void AdjustWidth()
        {
            if (this.fitting == false)
                return;

            GrDataRowList dataRowList = this.GridCore.DataRowList;

            int width = this.minWidth;

            if (this.GridCore.GetAutoFitColumnType() == GrAutoFitColumnType.ColumnIncluded)
            {
                int columnWidth = PreferredSize.Width;
                width = Math.Max(this.minWidth, columnWidth);
            }

            for (int i = 0; i < dataRowList.GetVisibleDataRowCount(); i++)
            {
                GrDataRow pDataRow = dataRowList.GetVisibleDataRow(i);
                GrItem pItem = pDataRow.GetItem(this);
                int itemWidth = pItem.PreferredSize.Width;
                width = Math.Max(width, itemWidth);
            }

            // insertion Row
            {
                GrDataRow pDataRow = this.GridCore.InsertionRow;
                GrItem pItem = pDataRow.GetItem(this);
                int itemWidth = pItem.PreferredSize.Width;
                width = Math.Max(width, itemWidth);
            }

            if (this.ItemType != GrItemType.Control)
                width += GrDefineUtility.DEF_CONTROL_WIDTH;

            if (this.maxWidth != 0)
                width = Math.Min(width, this.maxWidth);

            if (this.Width != width)
            {
                this.fitWidth = this.Width = width;

                GrColumnEventArgs e = new GrColumnEventArgs(this);
                this.columnList.Invoke("ColumnWidthChanged", e);
            }
            this.fitting = false;
        }

        public void SetFit()
        {
            if (this.fitting == true)
                return;
            this.fitting = true;
            this.columnList.SetFitChanged();
        }

        public bool IsSortable
        {
            get { return this.isSortable; }
            set { this.isSortable = value; }
        }

        public bool IsGroupable
        {
            get { return this.isGroupable; }
            set { this.isGroupable = value; }
        }

        public bool IsClipped
        {
            get { return this.clipped; }
            internal set { this.clipped = value; }
        }

        public bool ShouldBringIntoView()
        {
            if (this.displayable == false || this.clipped == true)
                return true;
            if (this.isFrozen == true)
                return false;
            return false;
        }

        protected override void SetBounds(int x, int y, int width, int height)
        {
            if (this.minWidth != 0)
                width = Math.Max(width, this.minWidth);
            if (this.maxWidth != 0)
                width = Math.Min(width, this.maxWidth);

            base.SetBounds(x, y, width, height);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            if (this.index != -1)
            {
                this.columnList.Invoke("ColumnWidthChanged", new GrColumnEventArgs(this));

                GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
                pTextUpdater.AddTextBoundsByColumn(this);
            }
        }

        //public void SetWidth(int width)
        //{
        //    if (this.minWidth != 0)
        //        width = Math.Max(width, this.minWidth);
        //    if (this.maxWidth != 0)
        //        width = Math.Min(width, this.maxWidth);

        //    if (this.width == width)
        //        return;

        //    this.width = width;

        //    if (this.index != -1)
        //    {
        //        GrColumnEventArgs e = new GrColumnEventArgs(this);
        //        this.columnList.Invoke("ColumnWidthChanged", e);

        //        GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
        //        pTextUpdater.AddTextBoundsByColumn(this);
        //    }
        //}

        public void SetTooltip(string tooltip)
        {
            this.tooltip = tooltip;
        }

        public string GetTooltip()
        {
            return this.tooltip;
        }

        public bool IsGrouped
        {
            get { return this.isGrouped; }
            set
            {
                if (this.isGrouped == value)
                    return;

                this.isGrouped = value;
                this.GroupChanged(this, EventArgs.Empty);
            }
        }

        public GrGroup Group
        {
            get { return this.group; }
        }

        public void SetSortType(GrSort sortType)
        {
            if (this.isSortable == false)
                return;
            if (this.columnList == null)
                return;
            this.sortType = sortType;
            if (this.index != -1)
            {
                GrColumnEventArgs e = new GrColumnEventArgs(this);
                this.columnList.Invoke("ColumnSortTypeChanged", e);
            }
        }

        public GrSort GetSortType()
        {
            if (this.columnList == null)
                return GrSort.None;
            if (this.columnList.GetFirstSortColumn() != this)
                return GrSort.None;
            return this.sortType;
        }

        public void SetSortComparer(GrSort sortType, GrFuncSortRow comparer)
        {
            this.comparer[(int)sortType] = comparer;
        }

        public GrFuncSortRow GetSortComparer(GrSort sortType)
        {
            return this.comparer[(int)sortType];
        }

        //public override GrCellType GetCellType() { return GrCellType.Column; }

        //public override int X
        //{
        //    get { return this.x; }
        //}

        //public override int Y
        //{
        //    get
        //    {
        //        if (this.columnList == null)
        //            return 0;
        //        return this.columnList.Y;
        //    }
        //}

        //public override int Width
        //{
        //    get { return this.width; }
        //}

        //public override int Height
        //{
        //    get
        //    {
        //        if (this.columnList == null)
        //            return 0;
        //        return this.columnList.Height;
        //    }
        //}

        //public override bool IsVisible
        //{
        //    get { return this.isVisible; }
        //}

        public override GrHorzAlign TextHorzAlign
        {
            get { return GrHorzAlign.Center; }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return GrVertAlign.Center; }
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
            return this.columnList;
        }

        public override GrPaintStyle ToPaintStyle()
        {
            GrPaintStyle flag = base.ToPaintStyle();
            if (this.IsSelecting == true || this.IsSelected == true)
                flag |= GrPaintStyle.Selected;
            if (this.HasFocused == true)
                flag |= GrPaintStyle.Focused;
            if (this.IsClipped == true)
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
            if (this.IsGrouped == true)
                backColor = pStyle.GetGroupBackColor(this.Group.GetGroupLevel());
            else
                backColor = base.GetPaintingBackColor();

            return backColor;
        }

        public override bool IsDisplayable
        {
            get
            {
                if (this.IsVisible == false)
                    return false;
                return this.displayable;
            }
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;

            if (this.fnColumnBackgroundPaint == null || this.fnColumnBackgroundPaint(painter, this, paintRect, this.columnPaintData) == false)
            {
                GrPaintStyle paintStyle = this.ToPaintStyle();
                GrColor backColor = GetPaintingBackColor();
                if (this.IsClipped == true)
                    painter.DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, clipRect);
                else
                    painter.DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);
            }

            if (this.fnColumnContentsPaint == null || this.fnColumnContentsPaint(painter, this, paintRect, this.columnPaintData) == false)
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
                    if (this.IsClipped == true)
                        painter.DrawColumnText(GetPaintingFont(), this.Text + cl.textBegin, cl.length, textRect, foreColor, clipRect);
                    else
                        painter.DrawColumnText(GetPaintingFont(), this.Text + cl.textBegin, cl.length, textRect, foreColor, null);
                }
            }
        }

        public event EventHandler GroupChanged;

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.columnList = this.GridCore.ColumnList;
            this.GridCore.AttachObject(this.group);
        }

        protected override void OnGridCoreDetached()
        {
            if (this.GridCore.GetMouseOver() == this)
                this.GridCore.SetMouseOver(null, GrPoint.Empty);

            if (this.GridCore.GetMousePress() == this)
                this.GridCore.SetMouseUnpress();

            base.OnGridCoreDetached();
            this.GridCore.DetachObject(this.group);
            this.columnList = null;
        }

        protected override void OnTextChanged()
        {
            base.OnTextChanged();
            if (this.GridCore == null)
                return;

            if (this.IsDisplayable == false)
                return;
            this.GridCore.Invalidate(this.Bounds);
        }

        //internal void SetX(int x) { this.x = x; }

        internal void SetDisplayable(bool b)
        {
            this.displayable = b;
        }

        internal void SetDisplayIndex(int index)
        {
            this.displayIndex = index;
        }

        internal void SetIndex(int index)
        {
            this.index = index;
        }

        internal void SetColumnID(int id)
        {
            this.columnID = id;
        }

        public bool customItemPaint;

        public GrFuncColumnPaint fnColumnBackgroundPaint;

        public GrFuncColumnPaint fnColumnContentsPaint;

        public object columnPaintData;

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
            if (this.Width == 100)
                return false;
            return this.Width != this.fitWidth;
        }

        public bool ShouldSerializeVisibleIndex()
        {
            return this.visibleIndex != GrDefineUtility.INVALID_INDEX;
        }
    }
}
