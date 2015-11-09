using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrItem : GrCell, IFocusable
    {
        private readonly GrColumn column;
        private readonly GrDataRow dataRow;
        
        private GrSize textBounds;

        private bool readOnly;
        internal bool selected;
        private bool colorLocked;

        internal GrItem(GrColumn column, GrDataRow dataRow)
        {
            this.column = column;
            this.dataRow = dataRow;
        }

        public GrRect GetControlRect()
        {
            int left = this.Width - GrDefineUtility.DEF_CONTROL_WIDTH;
            int top = 0;
            int right = this.Width - 1;
            int bottom = this.Height - 1;
            return GrRect.FromLTRB(left, top, right, bottom);
        }

        public bool GetControlVisible()
        {
            if (this.column.ItemType == GrItemType.Control)
                return false;

            bool showControl = false;

            switch (this.column.ItemTypeShow)
            {
                case GrItemTypeShow.SelectedOnly:
                    {
                        if (IsItemSelecting() == true || this.IsSelected == true)
                            showControl = true;
                    }
                    break;

                case GrItemTypeShow.FocusedOnly:
                    {
                        if (this.IsFocused == true)
                            showControl = true;
                    }
                    break;
                case GrItemTypeShow.Always:
                    {
                        showControl = true;
                    }
                    break;
                default:
                    break;
            }

            if (this.IsReadOnly == true)
                showControl = false;

            return showControl;
        }

        public GrControlState GetControlState()
        {
            if (GetMouseOvered() == false)
                return GrControlState.Normal;
            if (this.GridCore.GetMouseOverState() == (int)GrMouseOverState.Control)
            {
                if (GetMousePressed() == true)
                    return GrControlState.Pressed;
                else
                    return GrControlState.Hot;
            }
            return GrControlState.Normal;
        }

        public GrDataRow DataRow
        {
            get { return this.dataRow; }
        }

        IDataRow IFocusable.DataRow
        {
            get { return this.DataRow; }
        }

        public GrColumn Column
        {
            get { return this.column; }
        }

        public bool IsSelected
        {
            get
            {
                if (this.selected == true)
                    return true;
                if (this.dataRow.GetFullSelected() == true ||
                    (this.dataRow.IsInsertionRow() == false && this.column.IsFullSelected == true))
                    return true;
                return false;
            }
        }

        public bool IsFocused
        {
            get { return this.GridCore.Focuser.Get() == this; }
        }

        public bool IsItemSelecting()
        {
            GrItemSelectorInternal pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
            if (pItemSelector.IsSelecting(this.column) == false)
                return false;
            if (pItemSelector.IsSelecting(this.dataRow) == false)
                return false;
            return true;
        }

        public void SetSelected(bool b)
        {
            if (this.selected == b)
                return;
            GrItemSelector pItemSelector = this.GridCore.ItemSelector;
            if (b == true)
                pItemSelector.SelectItem(this, GrSelectionType.Add);
            else
                pItemSelector.SelectItem(this, GrSelectionType.Remove);
        }

        public void SetFocused(bool b)
        {
            GrFocuser focuser = this.GridCore.Focuser;
            if (b == true)
                focuser.Set(this);
            else
                focuser.Set(null as IFocusable);
        }

        public void SetTextBounds(GrSize size)
        {
            if (this.textBounds == size)
                return;
            this.textBounds = size;
            OnTextSizeChanged();
        }

        public bool ShouldBringIntoView()
        {
            return (this.column.ShouldBringIntoView() || this.dataRow.ShouldBringIntoView());
        }

        public void BringIntoView()
        {
            this.GridCore.BringIntoView(this);
        }

        public override int HitMouseOverTest(GrPoint localLocation)
        {
            if (base.HitMouseOverTest(localLocation) == 0)
                return 0;

            if (this.IsReadOnly == true)
                return (int)GrMouseOverState.In;

            if (this.column.ItemType == GrItemType.Control)
                return (int)GrMouseOverState.In;

            GrRect controlRect = GetControlRect();
            if (controlRect.Contains(localLocation) == true)
            {
                switch (this.column.ItemTypeShow)
                {
                    case GrItemTypeShow.FocusedOnly:
                        {
                            if (this.IsFocused == true)
                                return (int)GrMouseOverState.Control;
                        }
                        break;
                    case GrItemTypeShow.SelectedOnly:
                        {
                            if (this.IsSelected == true)
                                return (int)GrMouseOverState.Control;
                        }
                        break;
                    case GrItemTypeShow.Always:
                        return (int)GrMouseOverState.Control;
                    default:
                        break;
                }
            }

            return (int)GrMouseOverState.In;
        }

        public override bool IsVisible
        {
            get { return this.column.IsVisible; }
            set { }
        }

        public bool IsReadOnly
        {
            get
            {
                if (this.dataRow.DataRowID == GrDefineUtility.INSERTION_ROW)
                {
                    if (this.readOnly == true)
                        return true;
                    return this.dataRow.IsReadOnly;
                }
                return (this.column.IsReadOnly == true || this.dataRow.IsReadOnly == true || this.readOnly == true);
            }
            set
            {
                this.readOnly = value;
            }
        }

        public override GrHorzAlign TextHorzAlign
        {
            get { return this.column.ItemHorzAlign; }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return this.column.ItemVertAlign; }
        }

        public override bool IsTextWordWrap
        {
            get { return this.column.ItemWordWrap; }
        }

        public override bool IsTextMulitiline
        {
            get { return this.column.ItemMultiline; }
        }

        public virtual bool GetClipped()
        {
            if (this.column.IsClipped == true || this.dataRow.GetClipped() == true)
                return true;
            return false;
        }

        //public override int X
        //{
        //    get { return this.column.X; }
        //}

        //public override int Y
        //{
        //    get { return this.pDataRow.Y; }
        //}

        //public override int Width
        //{
        //    get { return this.column.Width; }
        //}

        //public override int Height
        //{
        //    get { return this.pDataRow.Height; }
        //}

        public override GrSize PreferredSize
        {
            get
            {
                GrSize size = this.column.GetItemMinSize();
                GrSize bounds = this.textBounds == GrSize.Empty ? GetTextBounds() : this.textBounds;

                if (size.Width != 0)
                    bounds.Width = Math.Max(bounds.Width, size.Width);
                if (size.Height != 0)
                    bounds.Height = Math.Max(bounds.Height, size.Height);

                bounds.Width += GetPadding().Horizontal;
                bounds.Height += GetPadding().Vertical;

                return bounds;
            }
        }

        //public override GrCellType GetCellType() { return GrCellType.Item; }

        public override bool IsDisplayable
        {
            get { return this.column.IsDisplayable && this.dataRow.IsDisplayable; }
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;

            if (paintRect.Left >= clipRect.Right || paintRect.Right < clipRect.Left)
                return;

            GrPaintStyle paintStyle = ToPaintStyle();

            GrColor backColor = GetPaintingBackColor();
            GrColor foreColor = GetPaintingForeColor();

            if (GetClipped() == true)
                painter.DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor, clipRect);
            else
                painter.DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);

            if (this.column.customItemPaint == true)
            {
                return;
            }

            if (this.column.ItemTextVisible == true)
            {
                if (GetClipped() == true)
                    DrawText(painter, foreColor, paintRect, clipRect);
                else
                    DrawText(painter, foreColor, paintRect, null);
            }

            if (GetControlVisible() == true)
            {
                GrRect buttonRect = GetControlRect() + paintRect.Location;
                switch (this.column.ItemType)
                {
                    case GrItemType.DropDown:
                        if (GetClipped() == true)
                            painter.DrawDropDown(buttonRect, GetControlState(), clipRect);
                        else
                            painter.DrawDropDown(buttonRect, GetControlState(), null);
                        break;
                    case GrItemType.Modal:
                        if (GetClipped() == true)
                            painter.DrawModal(buttonRect, GetControlState(), clipRect);
                        else
                            painter.DrawModal(buttonRect, GetControlState(), null);
                        break;
                    default:
                        break;
                }
            }
        }

        public override void Invalidate()
        {
            base.Invalidate();
        }

        public override GrRow GetRow()
        {
            return this.DataRow;
        }

        public override GrPaintStyle ToPaintStyle()
        {
            if (this.colorLocked == true)
                return GrPaintStyle.Default;
            GrPaintStyle flag = base.ToPaintStyle();
            if (this.GridCore.GetSelectionVisible() == false)
                return flag;

            if (IsItemSelecting() == true || this.IsSelected == true)
            {
                flag |= GrPaintStyle.Selected;
            }

            if (this.IsFocused == true)
            {
                flag |= GrPaintStyle.Focused;
            }
            return flag;
        }

        public override GrFont GetPaintingFont()
        {
            return GetFont();
        }

        public override GrColor GetPaintingForeColor()
        {
            GrColor foreColor = GetForeColor();
            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle == null)
                pStyle = GrStyle.Default;

            GrColor color = foreColor;

            if (this.colorLocked == true)
                return foreColor;

            if (this.IsFocused == true)
            {
                color = pStyle.FocusedForeColor;
            }
            else if (this.IsSelected == true || IsItemSelecting() == true)
            {
                if (this.GridCore.GetSelectionVisible() == true)
                    color = pStyle.SelectedForeColor;
            }
            return color;
        }

        public override GrColor GetPaintingBackColor()
        {
            GrColor backColor = GetBackColor();
            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle == null)
                pStyle = GrStyle.Default;

            GrColor color = backColor;

            if (this.colorLocked == true)
                return backColor;

            if (this.IsFocused == true)
            {
                color = pStyle.FocusedBackColor;
            }
            else if (this.IsSelected == true || IsItemSelecting() == true)
            {
                if (this.GridCore.GetSelectionVisible() == true)
                    color = pStyle.SelectedBackColor;
            }
            else if (this.dataRow.HasFocused() == true &&
                this.GridCore.GetRowHighlight() == true &&
                this.GridCore.RowHighlightType != GrRowHighlightType.Line)
            {
                color = pStyle.RowHighlightFillColor;
            }

            if (GetMouseOvered() == true)
            {
                color *= 0.9f;
            }

            return color;
        }

        public override GrColor GetForeColor()
        {
            GrColor color = base.GetForeColorCore();
            if (color != GrColor.Empty)
                return color;

            color = this.dataRow.GetItemForeColor();
            if (color != GrColor.Empty)
                return color;

            color = this.column.ItemForeColor;
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemForeColors.Count > 0)
                return pStyle.GetItemForeColor(this.dataRow.GetVisibleDataRowIndex());

            return base.GetForeColor();
        }

        public override GrColor GetBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            color = this.dataRow.GetItemBackColor();
            if (color != GrColor.Empty)
                return color;

            color = this.column.ItemBackColor;
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemBackColors.Count > 0)
                return pStyle.GetItemBackColor(this.dataRow.GetVisibleDataRowIndex());

            return base.GetBackColor();
        }

        public override GrPadding GetPadding()
        {
            GrPadding padding = base.GetPaddingCore();
            if (padding == GrPadding.Empty)
                padding = this.column.ItemPadding;

            if (padding == GrPadding.Empty)
                padding = base.GetPadding();

            if (this.column.GetItemIcon() == true)
                padding.Left += (GrDefineUtility.DEF_ICON_SIZE + padding.Left);

            return padding;
        }

        public override GrFont GetFont()
        {
            GrFont pFont = base.GetFontCore();
            if (pFont != null)
                return pFont;

            pFont = this.dataRow.GetItemFont();
            if (pFont != null)
                return pFont;

            pFont = this.column.ItemFont;
            if (pFont != null)
                return pFont;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemFonts.Count > 0)
                return pStyle.GetItemFont(this.dataRow.GetVisibleDataRowIndex());

            return base.GetFont();
        }

        public void LockColor(bool b)
        {
            if (this.colorLocked == b)
                return;
            this.colorLocked = b;
        }

        public GrRect GetDisplayRect() { return this.Bounds; }

        protected override void OnTextSizeChanged()
        {
            base.OnTextSizeChanged();

            if (this.GridCore.AutoFitColumn == true)
            {
                this.column.SetFit();
            }

            if (this.GridCore.AutoFitRow == true)
            {
                this.dataRow.SetFit();
            }
        }

        private void DoSelected(bool b)
        {
            throw new NotImplementedException();
        }


    }
}
