using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrItem : GrCell, IFocusable
    {
        internal GrDataRow m_pDataRow;
        internal GrColumn m_column;
        GrSize m_textBounds;

        bool m_readOnly;
        internal bool m_selected;
        bool m_colorLocked;


        public GrItem()
        {

        }

        public GrItem(GrColumn column, GrDataRow pDataRow)
        {
            m_column = column;
            m_pDataRow = pDataRow;
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
            if (m_column.ItemType == GrItemType.Control)
                return false;

            bool showControl = false;

            switch (m_column.GetItemTypeShow())
            {
                case GrItemTypeShow.SelectedOnly:
                    {
                        if (IsItemSelecting() == true || GetSelected() == true)
                            showControl = true;
                    }
                    break;

                case GrItemTypeShow.FocusedOnly:
                    {
                        if (GetFocused() == true)
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

            if (GetReadOnly() == true)
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

        public GrDataRow GetDataRow()
        {
            return m_pDataRow;
        }

        IDataRow IFocusable.GetDataRow()
        {
            return this.GetDataRow();
        }

        public GrColumn GetColumn()
        {
            return m_column;
        }

        public bool GetSelected()
        {
            if (m_selected == true)
                return true;
            if (m_pDataRow.GetFullSelected() == true ||
                (m_pDataRow.IsInsertionRow() == false && m_column.IsFullSelected == true))
                return true;
            return false;
        }

        public bool GetFocused()
        {
            GrFocuser focuser = this.GridCore.Focuser;
            return focuser.Get() == this;
        }

        public bool IsItemSelecting()
        {
            GrItemSelectorInternal pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
            if (pItemSelector.IsSelecting(m_column) == false)
                return false;
            if (pItemSelector.IsSelecting(m_pDataRow) == false)
                return false;
            return true;
        }

        public void SetReadOnly(bool b)
        {
            m_readOnly = b;
        }

        public void SetSelected(bool b)
        {
            if (m_selected == b)
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
            if (m_textBounds == size)
                return;
            m_textBounds = size;
            OnTextSizeChanged();
        }

        public bool ShouldBringIntoView()
        {
            return (m_column.ShouldBringIntoView() || m_pDataRow.ShouldBringIntoView());
        }

        public void BringIntoView()
        {
            this.GridCore.BringIntoView(this);
        }

        public override int HitMouseOverTest(GrPoint localLocation)
        {
            if (base.HitMouseOverTest(localLocation) == 0)
                return 0;

            if (GetReadOnly() == true)
                return (int)GrMouseOverState.In;

            if (m_column.ItemType == GrItemType.Control)
                return (int)GrMouseOverState.In;

            GrRect controlRect = GetControlRect();
            if (controlRect.Contains(localLocation) == true)
            {
                switch (m_column.GetItemTypeShow())
                {
                    case GrItemTypeShow.FocusedOnly:
                        {
                            if (GetFocused() == true)
                                return (int)GrMouseOverState.Control;
                        }
                        break;
                    case GrItemTypeShow.SelectedOnly:
                        {
                            if (GetSelected() == true)
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
            get { return m_column.IsVisible; }
            set { }
        }

        public virtual bool GetReadOnly()
        {
            if (m_pDataRow.GetDataRowID() == GrDefineUtility.INSERTION_ROW)
            {
                if (m_readOnly == true)
                    return true;
                return m_pDataRow.IsReadOnly;
            }
            return (m_column.IsReadOnly == true || m_pDataRow.IsReadOnly == true || m_readOnly == true);
        }

        public override GrHorzAlign TextHorzAlign
        {
            get { return m_column.GetItemHorzAlign(); }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return m_column.GetItemVertAlign(); }
        }

        public override bool IsTextWordWrap
        {
            get { return m_column.GetItemWordWrap(); }
        }

        public override bool IsTextMulitiline
        {
            get { return m_column.GetItemMultiline(); }
        }

        public virtual bool GetClipped()
        {
            if (m_column.IsClipped == true || m_pDataRow.GetClipped() == true)
                return true;
            return false;
        }

        //public override int X
        //{
        //    get { return m_column.X; }
        //}

        //public override int Y
        //{
        //    get { return m_pDataRow.Y; }
        //}

        //public override int Width
        //{
        //    get { return m_column.Width; }
        //}

        //public override int Height
        //{
        //    get { return m_pDataRow.Height; }
        //}

        public override GrSize GetPreferredSize()
        {
            GrSize size = m_column.GetItemMinSize();
            GrSize bounds = m_textBounds == GrSize.Empty ? GetTextBounds() : m_textBounds;

            if (size.Width != 0)
                bounds.Width = Math.Max(bounds.Width, size.Width);
            if (size.Height != 0)
                bounds.Height = Math.Max(bounds.Height, size.Height);

            bounds.Width += GetPadding().Horizontal;
            bounds.Height += GetPadding().Vertical;

            return bounds;
        }

        //public override GrCellType GetCellType() { return GrCellType.Item; }

        public override bool IsDisplayable
        {
            get { return m_column.IsDisplayable && m_pDataRow.IsDisplayable; }
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

            if (m_column.customItemPaint == true)
            {
                return;
            }

            if (m_column.GetItemTextVisible() == true)
            {
                if (GetClipped() == true)
                    DrawText(painter, foreColor, paintRect, clipRect);
                else
                    DrawText(painter, foreColor, paintRect, null);
            }

            if (GetControlVisible() == true)
            {
                GrRect buttonRect = GetControlRect() + paintRect.Location;
                switch (m_column.ItemType)
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
            return GetDataRow();
        }

        public override GrPaintStyle ToPaintStyle()
        {
            if (m_colorLocked == true)
                return GrPaintStyle.Default;
            GrPaintStyle flag = base.ToPaintStyle();
            if (this.GridCore.GetSelectionVisible() == false)
                return flag;

            if (IsItemSelecting() == true || GetSelected() == true)
            {
                flag |= GrPaintStyle.Selected;
            }

            if (GetFocused() == true)
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

            if (m_colorLocked == true)
                return foreColor;

            if (GetFocused() == true)
            {
                color = pStyle.FocusedForeColor;
            }
            else if (GetSelected() == true || IsItemSelecting() == true)
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

            if (m_colorLocked == true)
                return backColor;

            if (GetFocused() == true)
            {
                color = pStyle.FocusedBackColor;
            }
            else if (GetSelected() == true || IsItemSelecting() == true)
            {
                if (this.GridCore.GetSelectionVisible() == true)
                    color = pStyle.SelectedBackColor;
            }
            else if (m_pDataRow.HasFocused() == true &&
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

            color = m_pDataRow.GetItemForeColor();
            if (color != GrColor.Empty)
                return color;

            color = m_column.GetItemForeColor();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemForeColors.Count > 0)
                return pStyle.GetItemForeColor(m_pDataRow.GetVisibleDataRowIndex());

            return base.GetForeColor();
        }

        public override GrColor GetBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            color = m_pDataRow.GetItemBackColor();
            if (color != GrColor.Empty)
                return color;

            color = m_column.GetItemBackColor();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemBackColors.Count > 0)
                return pStyle.GetItemBackColor(m_pDataRow.GetVisibleDataRowIndex());

            return base.GetBackColor();
        }

        public override GrPadding GetPadding()
        {
            GrPadding padding = base.GetPaddingCore();
            if (padding == GrPadding.Empty)
                padding = m_column.GetItemPadding();

            if (padding == GrPadding.Empty)
                padding = base.GetPadding();

            if (m_column.GetItemIcon() == true)
                padding.Left += (GrDefineUtility.DEF_ICON_SIZE + padding.Left);

            return padding;
        }

        public override GrFont GetFont()
        {
            GrFont pFont = base.GetFontCore();
            if (pFont != null)
                return pFont;

            pFont = m_pDataRow.GetItemFont();
            if (pFont != null)
                return pFont;

            pFont = m_column.GetItemFont();
            if (pFont != null)
                return pFont;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null && pStyle.ItemFonts.Count > 0)
                return pStyle.GetItemFont(m_pDataRow.GetVisibleDataRowIndex());

            return base.GetFont();
        }


        public void LockColor(bool b)
        {
            if (m_colorLocked == b)
                return;
            m_colorLocked = b;
        }

        public GrRect GetDisplayRect() { return this.Bounds; }

        protected override void OnTextSizeChanged()
        {
            base.OnTextSizeChanged();

            if (this.GridCore.AutoFitColumn == true)
            {
                m_column.SetFit();
            }

            if (this.GridCore.AutoFitRow == true)
            {
                m_pDataRow.SetFit();
            }
        }

        private void DoSelected(bool b)
        {
            throw new NotImplementedException();
        }


    }
}
