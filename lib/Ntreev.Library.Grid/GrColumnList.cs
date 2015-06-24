﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumnList : GrUpdatableRow
    {
        private readonly List<GrColumn> m_vecColumns = new List<GrColumn>();
        private readonly List<GrColumn> m_vecColumnsRemoved = new List<GrColumn>();
        private readonly List<GrColumn> m_vecVisibleColumns = new List<GrColumn>();
        private readonly List<GrColumn> m_vecDisplayableColumns = new List<GrColumn>();
        int m_columnID;

        int m_unfrozenX;
        int m_frozenRight;
        int m_displayableRight;
        int m_visibleRight;
        int m_boundWidth;

        int m_frozenCount;
        int m_groupCount;

        GrRange m_selectingRange;
        GrColumn m_pSortColumn;
        GrColumnSplitter m_columnSplitter;

        int m_clippedWidth;
        int m_clippedIndex;

        // flag
        bool m_visibleChanged;
        bool m_fitChanged;
        bool m_widthChanged;

        public GrColumnList()
        {
            //            : Columns(m_vecColumns), Visibles(m_vecVisibleColumns), Displayables(m_vecDisplayableColumns)
            //{
            m_columnSplitter = new GrColumnSplitter(this);

            SetTextVisible(false);
            SetText("ColumnList");

            m_frozenCount = 0;
            m_groupCount = 0;
            m_pSortColumn = null;
            m_displayableRight = 0;
            m_columnID = 0;

            m_visibleChanged = false;
            m_fitChanged = false;
            m_widthChanged = false;
            m_clippedIndex = -1;

            SetResizable(false);

        }

        public void Reserve(int reserve)
        {
            m_vecColumns.Capacity = reserve;
            m_vecVisibleColumns.Capacity = reserve;
            m_vecDisplayableColumns.Capacity = reserve;
            m_vecColumnsRemoved.Capacity = reserve;
        }

        public void AddColumn(GrColumn column)
        {
            InsertColumn(column, m_vecColumns.Count);
        }

        public void RemoveColumn(GrColumn column)
        {
            //List<GrColumn>.iterator itor = std.find(m_vecColumns.begin(), m_vecColumns.end(), column);

            int index = column.GetIndex();
            //for (List<GrColumn>.iterator next = itor + 1; next != m_vecColumns.end(); next++)
            for (int i = index + 1; i < m_vecColumns.Count; i++)
            {
                m_vecColumns[i].SetIndex(i);
            }

            column.SetSelected(false);
            column.KillFocus();

            this.GridCore.DetachObject(column);

            column.SetIndex(-1);

            m_vecColumns.Remove(column);
            m_vecColumnsRemoved.Add(column);

            column.GroupChanged -= column_GroupChanged;
            GrColumnEventArgs e = new GrColumnEventArgs(column);
            OnColumnRemoved(e);

            if (this.GridCore.IsInvalidated() == false)
                throw new Exception();


        }

        public void InsertColumn(GrColumn column, int index)
        {
            if (column.GetIndex() != -1)
                throw new Exception("이미 등록되어 있습니다");

            if (column.GetColumnID() == -1)
            {
                column.SetColumnID(m_columnID++);
            }

            m_vecColumnsRemoved.Remove(column);

            m_vecColumns.Insert(index, column);
            for (int i = index; i < m_vecColumns.Count; i++)
            {
                m_vecColumns[i].SetIndex(i);
            }

            //itor = m_vecColumns.insert(m_vecColumns.begin() + index, column);
            //index = Math.Min(index, (uint)m_vecColumns.Count);
            //for( ; itor != m_vecColumns.end() ; itor++)
            //{
            //    (*itor).SetIndex(index++);
            //}

            //column.m_freezablePriority = column.GetIndex();
            //column.m_priority = column.GetIndex();

            this.GridCore.AttachObject(column);

            GrColumnEventArgs e = new GrColumnEventArgs(column);
            OnColumnInserted(e);

            column.GroupChanged += column_GroupChanged;
            if (this.GridCore.IsInvalidated() == false)
                throw new Exception();

        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.AttachObject(m_columnSplitter);

            GrGroupPanel groupList = this.GridCore.GroupPanel;
            groupList.Changed += groupPanel_Changed;

            GrFocuser focuser = this.GridCore.Focuser;
            focuser.FocusChanged += gridCore_FocusChanged;

            GrDataRowList dataRowList = this.GridCore.DataRowList;
            dataRowList.DataRowRemoved += dataRowList_DataRowRemoved;

            this.GridCore.Cleared += gridCore_Cleared;
            this.GridCore.FontChanged += gridCore_FontChanged;
            this.GridCore.DisplayRectChanged += gridCore_DisplayRectChanged;
        }

        //~GrColumnList()
        //{
        //    foreach (var value in m_vecColumns)
        //    {
        //        delete value;
        //    }

        //    foreach (var value in m_vecColumnsRemoved)
        //    {
        //        delete value;
        //    }

        //    delete m_columnSplitter;
        //}

        public override int GetWidth()
        {
            if (this.GridCore.GetRowVisible() == false)
                return 0;
            return this.GridCore.DataRowList.GetRowWidth();
        }

        public override bool ShouldUpdate()
        {
            return m_visibleChanged == true ||
                m_fitChanged == true ||
                m_widthChanged == true;
        }

        public override void Update(bool force)
        {
            if (m_visibleChanged == true || force == true)
                BuildVisibleColumnList();

            if (m_fitChanged == true)
                AdjustColumnWidth();

            if (m_widthChanged == true)
                RepositionColumnList();

            m_visibleChanged = false;
            m_fitChanged = false;
            m_widthChanged = false;


            bool visible = false;
            if (GetUnfrozenColumnCount() != 0)
            {
                visible = m_visibleRight > this.GridCore.DisplayRectangle.Right ? true : false;
            }
            this.GridCore.GetHorzScroll().IsVisible = visible;
        }

        public override GrRect GetBounds()
        {
            return GrRect.FromLTRB(GetX(), GetY(), m_displayableRight, GetY() + GetHeight());
        }

        public override GrColor GetForeColor()
        {
            GrColor color = base.GetForeColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowForeColor();

            return base.GetForeColor();
        }

        public override GrColor GetBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowBackColor();

            return base.GetBackColor();
        }

        public override GrColor GetLineColor()
        {
            GrColor color = base.GetLineColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowLineColor();

            return base.GetLineColor();
        }

        public override GrFont GetFont()
        {
            GrFont pFont = base.GetFontCore();
            if (pFont != null)
                return pFont;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetRowFont();

            return base.GetFont();
        }

        public int GetColumnCount()
        {
            return m_vecColumns.Count;
        }

        public GrColumn HitTest(int x)
        {
            foreach (var value in m_vecDisplayableColumns)
            {
                if (x >= value.GetX() && x < value.GetRight())
                    return value;
            }
            return null;
        }

        public GrRange HitTest(int x, GrColumn columnAnchor)
        {
            GrRange indexRange = new GrRange();
            int anchorIndex = columnAnchor.GetVisibleIndex();

            if (columnAnchor.GetFrozen() == true)
            {
                if (x < GetRight())
                {
                    indexRange.SetRange(0, anchorIndex + 1);
                }
                else if (x >= m_frozenRight)
                {
                    indexRange.SetRange(anchorIndex, GetFrozenColumnCount());
                }
                else
                {
                    GrColumn column = HitTest(x);
                    int columnIndex = column.GetVisibleIndex();
                    int minIndex = Math.Min(columnIndex, anchorIndex);
                    int maxIndex = Math.Max(columnIndex, anchorIndex);
                    indexRange.SetRange(minIndex, maxIndex + 1);
                }
            }
            else
            {
                if (x < m_unfrozenX)
                {
                    int index = m_clippedIndex - 1;
                    if (index >= GetUnfrozenColumnCount())
                        index = 0;

                    GrColumn column = GetUnfrozenColumn(index);
                    indexRange.SetRange(column.GetVisibleIndex(), anchorIndex + 1);
                }
                else if (x >= m_displayableRight)
                {
                    int displayableCount = GetDisplayableColumnCount();
                    GrColumn column = GetDisplayableColumn(displayableCount - 1);
                    indexRange.SetRange(anchorIndex, column.GetVisibleIndex() + 1);
                }
                else
                {
                    GrColumn column = HitTest(x);
                    int columnIndex = column.GetVisibleIndex();
                    int minIndex = Math.Min(columnIndex, anchorIndex);
                    int maxIndex = Math.Max(columnIndex, anchorIndex);
                    indexRange.SetRange(minIndex, maxIndex + 1);
                }
            }

            return indexRange;
        }

        public void BringIntoView(GrColumn column)
        {
            if (column.ShouldBringIntoView() == false || column.GetFrozen() == true)
                return;

            Update();

            GrScroll pHorzScroll = this.GridCore.GetHorzScroll();
            int visibleIndex = column.GetUnfrozenIndex();

            int newValue;
            if (visibleIndex < pHorzScroll.Value)
            {
                newValue = visibleIndex;
            }
            else
            {
                newValue = ClipTo(visibleIndex);
            }

            pHorzScroll.Value = newValue;
            this.GridCore.Invalidate();
            this.GridCore.Update();
        }

        public void DisplayFirst(GrColumn column)
        {
            GrScroll pHorzScroll = this.GridCore.GetHorzScroll();
            pHorzScroll.Value = column.GetUnfrozenIndex();
            this.GridCore.Invalidate();
            this.GridCore.Update();
        }

        public void SetFitChanged()
        {
            if (m_fitChanged == true)
                return;
            m_fitChanged = true;
            this.GridCore.Invalidate();
        }

        public void SetVisibleChanged()
        {
            if (m_visibleChanged == true)
                return;
            m_visibleChanged = true;
            this.GridCore.Invalidate();
        }

        public void SetWidthChanged()
        {
            if (m_widthChanged == true)
                return;
            m_widthChanged = true;
            this.GridCore.Invalidate();
        }

        public GrColumn GetFirstSortColumn()
        {
            return m_pSortColumn;
        }

        public override bool ShouldClip(GrRect displayRect, int horizontal, int vertical)
        {
            if (m_clippedIndex == horizontal && displayRect.Width == m_clippedWidth)
                return false;
            return true;
        }

        public override void Clip(GrRect displayRect, int horizontal, int vertical)
        {
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            int x = dataRowList.CellStart();

            foreach (var value in m_vecDisplayableColumns)
            {
                value.SetDisplayable(false);
            }
            m_vecDisplayableColumns.Clear();

            for (int i = 0; i < m_frozenCount; i++)
            {
                GrColumn column = GetVisibleColumn(i);

                if (x > displayRect.Right)
                    break;

                column.SetX(x);
                column.SetDisplayable(true);
                column.SetDisplayIndex(m_vecDisplayableColumns.Count);

                int width = column.GetWidth();

                if (x + width > displayRect.Right)
                {
                    column.SetClipped(true);
                    x = displayRect.Right;
                }
                else
                {
                    column.SetClipped(false);
                    x += width;
                }

                m_vecDisplayableColumns.Add(column);
            }

            m_columnSplitter.SetX(x);

            if (m_columnSplitter.GetVisible() == true)
                x += m_columnSplitter.GetWidth();

            for (int i = m_frozenCount + horizontal; i < GetVisibleColumnCount(); i++)
            {
                GrColumn column = GetVisibleColumn(i);

                if (x >= displayRect.Right)
                    break;

                column.SetX(x);
                column.SetDisplayable(true);
                column.SetDisplayIndex(m_vecDisplayableColumns.Count);

                int width = column.GetWidth();

                if (x + width > displayRect.Right)
                {
                    column.SetClipped(true);
                    x = displayRect.Right;
                }
                else
                {
                    column.SetClipped(false);
                    x += width;
                }

                m_vecDisplayableColumns.Add(column);
            }

            m_displayableRight = x;

            if (m_clippedWidth != displayRect.Width || m_clippedIndex != horizontal)
                UpdateHorzScroll(displayRect);

            m_clippedIndex = horizontal;
            m_clippedWidth = displayRect.Width;
        }

        public void UpdateHorzScroll(GrRect displayRect)
        {
            GrScroll pHorzScroll = this.GridCore.GetHorzScroll();
            if (pHorzScroll.IsVisible == false)
                return;

            int visibleWidth = m_visibleRight - m_unfrozenX;
            int displayableWidth = displayRect.Right - m_unfrozenX;

            int unfrozenCount = GetUnfrozenColumnCount();

            int countPerPage = 0;
            int tempWidth = displayableWidth;
            for (int i = unfrozenCount - 1; i < unfrozenCount; i--)
            {
                GrColumn column = GetUnfrozenColumn(i);
                int columnWidth = column.GetWidth();
                if (columnWidth < tempWidth)
                {
                    countPerPage++;
                    tempWidth -= columnWidth;
                }
                else
                {
                    break;
                }
            }
            countPerPage = Math.Max(1, countPerPage);

            int largeChange = (int)((float)unfrozenCount * (float)displayableWidth / (float)visibleWidth);
            largeChange = Math.Max(1, largeChange);

            pHorzScroll.Minimum = 0;
            pHorzScroll.Maximum = (int)(unfrozenCount - countPerPage + largeChange - 1);
            pHorzScroll.LargeChange = largeChange;
        }

        public int ClipFrom(int visibleFrom)
        {
            return ClipFrom(this.GridCore.DisplayRectangle, visibleFrom);
        }

        public int ClipFrom(GrRect displayRect, int visibleFrom)
        {
            int displayWidth = displayRect.Width - m_unfrozenX;
            int displayX = displayWidth;
            int visibleTo = visibleFrom;

            for (int i = visibleFrom; i < GetUnfrozenColumnCount(); i++)
            {
                GrColumn column = GetUnfrozenColumn(i);
                displayX -= column.GetWidth();
                if (displayX <= 0)
                    break;
                visibleTo = i;
            }
            return visibleTo;
        }

        public int ClipTo(int visibleTo)
        {
            return ClipTo(this.GridCore.DisplayRectangle, visibleTo);
        }

        public int ClipTo(GrRect displayRect, int visibleTo)
        {
            int displayWidth = displayRect.Width - m_unfrozenX;
            int displayX = displayWidth;
            int visibleFrom = visibleTo;

            for (int i = visibleTo; i < GetUnfrozenColumnCount(); i--)
            {
                GrColumn column = GetUnfrozenColumn(i);
                displayX -= column.GetWidth();
                if (displayX <= 0)
                    break;
                visibleFrom = i;
            }
            return visibleFrom;
        }

        //bool MoveToFrozen(GrColumn column, GrColumn pWhere)
        //{
        //    if(pWhere && pWhere.GetFrozen() == false)
        //        return false;
        //
        //    if(column == pWhere)
        //        return false;
        //
        //    List<GrColumn> vecFrozens;
        //    vecFrozens.reserve(GetColumnCount());
        //
        //    foreach(var value in m_vecColumns)
        //    {
        //        if(value == column)
        //            continue;
        //        vecFrozens.Add(value);
        //    }
        //
        //    std.sort(vecFrozens.begin(), vecFrozens.end(), SortColumnOnFreezable());
        //
        //    List<GrColumn>.iterator itorWhere = std.find(vecFrozens.begin(), vecFrozens.end(), pWhere);
        //    vecFrozens.insert(itorWhere, column);
        //    column.m_frozen = true;
        //
        //    int priority = 0;
        //    foreach(var value in vecFrozens)
        //    {
        //        value.SetFreezablePriority(priority);
        //        priority++;
        //    }
        //
        //    SetVisibleChanged();
        //
        //    return true;
        //}
        //
        //bool MoveToUnfrozen(GrColumn column, GrColumn pWhere)
        //{
        //    if(pWhere && pWhere.GetFrozen() == true)
        //        return false;
        //
        //    if(column == pWhere)
        //        return false;
        //
        //    List<GrColumn> vecUnfrozens;
        //    vecUnfrozens.reserve(GetColumnCount());
        //
        //    foreach(var value in m_vecColumns)
        //    {
        //        if(value == column)
        //            continue;
        //        vecUnfrozens.Add(value);
        //    }
        //
        //    std.sort(vecUnfrozens.begin(), vecUnfrozens.end(), SortColumnOnUnfreezable());
        //
        //    List<GrColumn>.iterator itorWhere = std.find(vecUnfrozens.begin(), vecUnfrozens.end(), pWhere);
        //    vecUnfrozens.insert(itorWhere, column);
        //    column.m_frozen = false;
        //
        //    int priority = 0;
        //
        //    foreach(var value in vecUnfrozens)
        //    {
        //        value.SetPriority(priority);
        //        priority++;
        //    }
        //
        //    SetVisibleChanged();
        //
        //    return true;
        //}

        void column_GroupChanged(object sender, EventArgs e)
        {
            GrColumnEventArgs ce = new GrColumnEventArgs((GrColumn)sender);
            OnColumnGroupChanged(ce);
        }

        void groupPanel_Changed(object sender, EventArgs e)
        {
            GrGroupPanel groupList = this.GridCore.GroupPanel;
            m_groupCount = groupList.GetGroupCount();
            m_widthChanged = true;
            m_clippedIndex = -1;

            if (this.GridCore.IsInvalidated() == false)
                throw new Exception();
        }

        void gridCore_FocusChanged(object sender, GrFocusChangeArgs e)
        {

        }

        void dataRowList_DataRowRemoved(object sender, GrDataRowEventArgs e)
        {
            SetWidthChanged();
        }

        void gridCore_Cleared(object sender, EventArgs e)
        {
            m_groupCount = 0;
            m_pSortColumn = null;
            m_frozenCount = 0;
            m_columnID = 0;

            if (m_columnSplitter.GetVisible() == true)
                m_displayableRight = m_columnSplitter.GetRight();
            else
                m_displayableRight = GetRight();

            foreach (var value in m_vecColumns)
            {
                //delete value;
            }

            foreach (var value in m_vecColumnsRemoved)
            {
                value.SetColumnID(-1);
            }

            m_vecColumns.Clear();
            m_vecVisibleColumns.Clear();
            m_vecDisplayableColumns.Clear();
        }

        void gridCore_FontChanged(object sender, EventArgs e)
        {
            GrFont pFont = GetPaintingFont();
            int height = (int)(pFont.GetHeight() + pFont.GetExternalLeading()) + GetPadding().Vertical;
            SetHeight(height);
            foreach (var value in m_vecColumns)
            {
                this.GridCore.GetTextUpdater().AddTextBounds(value);
            }
        }

        void gridCore_DisplayRectChanged(object sender, EventArgs e)
        {
            SetWidthChanged();
        }

        internal void Invoke(string eventName, EventArgs e)
        {
            if (eventName.CompareTo("ColumnSortTypeChanged") == 0)
                OnColumnSortTypeChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnFrozenChanged") == 0)
                OnColumnFrozenChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnWidthChanged") == 0)
                OnColumnWidthChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnWordwrapChanged") == 0)
                OnColumnWordwrapChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnMultilineChanged") == 0)
                OnColumnMultilineChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnHorzAlignChanged") == 0)
                OnColumnHorzAlignChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnVertAlignChanged") == 0)
                OnColumnVertAlignChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnPaddingChanged") == 0)
                OnColumnPaddingChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnGroupChanged") == 0)
                OnColumnGroupChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnVisibleIndexChanged") == 0)
                OnColumnVisibleIndexChanged((GrColumnEventArgs)e);
            else if (eventName.CompareTo("ColumnMouseMove") == 0)
                OnColumnMouseMove((GrColumnMouseEventArgs)e);
            else if (eventName.CompareTo("ColumnMouseEnter") == 0)
                OnColumnMouseEnter((GrColumnMouseEventArgs)e);
            else if (eventName.CompareTo("ColumnMouseLeave") == 0)
                OnColumnMouseLeave((GrColumnMouseEventArgs)e);
            else if (eventName.CompareTo("ColumnMouseDown") == 0)
                OnColumnMouseDown((GrColumnMouseEventArgs)e);
            else if (eventName.CompareTo("ColumnMouseUp") == 0)
                OnColumnMouseUp((GrColumnMouseEventArgs)e);
            else
                throw new Exception("Not implemented event");
        }

        public override GrCell HitTest(GrPoint location)
        {
            GrCell pHitted = base.HitTest(location);
            if (pHitted == null)
                return null;

            foreach (var value in m_vecDisplayableColumns)
            {
                int x = value.GetX();
                if (location.X >= x && location.X < x + value.GetWidth())
                    return value;
            }

            GrRect bound = m_columnSplitter.GetRect();
            if (bound.Contains(location) == true)
                return m_columnSplitter;

            if (ContainsHorz(location.X) == false)
                return null;

            return pHitted;
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            GrRect paintRect = GetRect();
            GrPaintStyle paintStyle = ToPaintStyle();
            GrColor lineColor = GetPaintingLineColor();
            GrColor backColor = GetPaintingBackColor();

            painter.DrawRow(paintStyle, paintRect, lineColor, backColor, null);

            if (dataRowList.GetMargin() != 0)
            {
                GrStyle pStyle = this.GridCore.GetStyle();

                GrColor backColor1 = pStyle != null ? pStyle.GetColumnBackColor() : this.GridCore.GetBackColor();
                GrColor lineColor1 = pStyle != null ? pStyle.GetColumnLineColor() : this.GridCore.GetLineColor();
                paintRect.X = paintRect.Right;
                paintRect.Width = dataRowList.CellStart() - paintRect.Right;
                painter.DrawColumn(GrPaintStyle.Default, paintRect, lineColor, backColor, null);
            }

            for (int j = 0; j < m_vecDisplayableColumns.Count; j++)
            {
                GrColumn column = m_vecDisplayableColumns[j];
                if (column.GetX() > clipRect.Right || column.GetRight() < clipRect.Left)
                    continue;
                column.Paint(painter, clipRect);
            }

            PaintSplitter(painter, clipRect);

            GrRect displayRect = this.GridCore.DisplayRectangle;
            if (this.GridCore.GetFillBlank() == true && m_displayableRight < displayRect.Right)
            {
                GrRect pr = paintRect;
                pr.X = m_displayableRight;
                pr.Width = displayRect.Right - m_displayableRight;
                painter.DrawColumn(paintStyle & ~GrPaintStyle.RightLine, pr, lineColor, backColor, clipRect);
            }
        }

        public void PaintSplitter(GrGridPainter painter, GrRect clipRect)
        {
            if (m_columnSplitter.GetVisible() == true)
                m_columnSplitter.Paint(painter, clipRect);
        }

        public GrColumn GetColumn(int index)
        {
            if (index >= m_vecColumns.Count)
                throw new Exception();
            return m_vecColumns[index];
        }

        protected void BuildVisibleColumnList()
        {
            m_vecVisibleColumns.Clear();
            m_vecDisplayableColumns.Clear();

            m_frozenCount = 0;
            List<GrColumn> visibleCores = new List<GrColumn>(m_vecColumns.Count);
            List<GrColumn> visibles = new List<GrColumn>(m_vecColumns.Count);

            foreach (var value in m_vecColumns)
            {
                value.SetDisplayable(false);
                value.SetDisplayIndex(-1);
                value.SetClipped(false);

                if (value.GetVisible() == false)
                    continue;

                if (value.GetFrozen() == true)
                    m_frozenCount++;

                if (value.m_visibleIndex == -1)
                    visibleCores.Add(value);
                else
                    visibles.Add(value);
                //m_vecVisibleColumns.Add(value);
            }
            visibleCores.Sort(new SortColumnByVisible());
            int index = 0;
            foreach (var value in visibleCores)
            {
                value.m_visibleIndexCore = index++;
            }

            //m_vecVisibleColumns.insert(m_vecVisibleColumns.end(), visibleCores.begin(), visibleCores.end());
            //m_vecVisibleColumns.insert(m_vecVisibleColumns.end(), visibles.begin(), visibles.end());

            m_vecVisibleColumns.AddRange(visibleCores);
            m_vecVisibleColumns.AddRange(visibles);

            m_vecVisibleColumns.Sort(new SortColumnByVisible());

            index = 0;
            foreach (var value in m_vecVisibleColumns)
            {
                value.m_visibleIndexCore = index++;
                value.m_visibleIndex = -1;
            }
            m_widthChanged = true;
        }

        public void AdjustColumnWidth()
        {
            foreach (var value in m_vecColumns)
            {
                value.AdjustWidth();
            }
            m_widthChanged = true;
        }

        public event GrColumnEventHandler ColumnInserted;
        public event GrColumnEventHandler ColumnRemoved;
        public event GrColumnEventHandler ColumnSortTypeChanged;
        public event GrColumnEventHandler ColumnFrozenChanged;
        public event GrColumnEventHandler ColumnWidthChanged;
        public event GrColumnEventHandler ColumnWordwrapChanged;
        public event GrColumnEventHandler ColumnMultilineChanged;
        public event GrColumnEventHandler ColumnHorzAlignChanged;
        public event GrColumnEventHandler ColumnVertAlignChanged;
        public event GrColumnEventHandler ColumnPaddingChanged;
        public event GrColumnEventHandler ColumnGroupChanged;
        public event GrColumnEventHandler ColumnVisibleIndexChanged;

        public event GrColumnMouseEventHandler ColumnMouseMove;
        public event GrColumnMouseEventHandler ColumnMouseEnter;
        public event GrColumnMouseEventHandler ColumnMouseLeave;
        public event GrColumnMouseEventHandler ColumnMouseDown;
        public event GrColumnMouseEventHandler ColumnMouseUp;


        protected void RepositionColumnList()
        {
            GrDataRowList dataRowList = this.GridCore.DataRowList;
            int x = dataRowList.CellStart();

            for (int i = 0; i < GetFrozenColumnCount(); i++)
            {
                GrColumn column = GetFrozenColumn(i);
                x += column.GetWidth();
            }

            m_frozenRight = x;

            if (m_columnSplitter.GetVisible() == true)
                x += m_columnSplitter.GetWidth();

            m_unfrozenX = x;

            for (int i = 0; i < GetUnfrozenColumnCount(); i++)
            {
                GrColumn column = GetUnfrozenColumn(i);
                x += column.GetWidth();
            }
            m_visibleRight = x;

            m_boundWidth = x - GetX();
            m_clippedIndex = -1;
        }

        protected virtual void OnColumnInserted(GrColumnEventArgs e)
        {
            SetVisibleChanged();
            ColumnInserted(this, e);
        }

        protected virtual void OnColumnRemoved(GrColumnEventArgs e)
        {
            SetVisibleChanged();
            ColumnRemoved(this, e);
        }

        protected virtual void OnColumnSortTypeChanged(GrColumnEventArgs e)
        {
            m_pSortColumn = e.Column;
            this.GridCore.Invalidate();

            ColumnSortTypeChanged(this, e);
        }

        protected virtual void OnColumnFrozenChanged(GrColumnEventArgs e)
        {
            SetVisibleChanged();
            ColumnFrozenChanged(this, e);
        }

        protected virtual void OnColumnWidthChanged(GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            if (column.GetDisplayIndex() != -1)
            {
                this.GridCore.Invalidate();
            }
            m_widthChanged = true;

            ColumnWidthChanged(this, e);
        }

        protected virtual void OnColumnWordwrapChanged(GrColumnEventArgs e)
        {
            ColumnWordwrapChanged(this, e);
        }

        protected virtual void OnColumnMultilineChanged(GrColumnEventArgs e)
        {
            ColumnMultilineChanged(this, e);
        }

        protected virtual void OnColumnHorzAlignChanged(GrColumnEventArgs e)
        {
            ColumnHorzAlignChanged(this, e);
        }

        protected virtual void OnColumnVertAlignChanged(GrColumnEventArgs e)
        {
            ColumnVertAlignChanged(this, e);
        }

        protected virtual void OnColumnPaddingChanged(GrColumnEventArgs e)
        {
            ColumnPaddingChanged(this, e);
        }

        protected virtual void OnColumnGroupChanged(GrColumnEventArgs e)
        {
            ColumnGroupChanged(this, e);
        }

        protected virtual void OnColumnVisibleIndexChanged(GrColumnEventArgs e)
        {
            SetVisibleChanged();
            ColumnVisibleIndexChanged(this, e);
        }

        protected virtual void OnColumnMouseMove(GrColumnMouseEventArgs e)
        {
            ColumnMouseMove(this, e);
        }

        protected virtual void OnColumnMouseEnter(GrColumnMouseEventArgs e)
        {
            ColumnMouseEnter(this, e);
        }

        protected virtual void OnColumnMouseLeave(GrColumnMouseEventArgs e)
        {
            ColumnMouseLeave(this, e);
        }

        protected virtual void OnColumnMouseDown(GrColumnMouseEventArgs e)
        {
            ColumnMouseDown(this, e);
        }

        protected virtual void OnColumnMouseUp(GrColumnMouseEventArgs e)
        {
            ColumnMouseUp(this, e);
        }

        public int GetVisibleColumnCount()
        {
            return m_vecVisibleColumns.Count;
        }

        public GrColumn GetVisibleColumn(int index)
        {
            return m_vecVisibleColumns[index];
        }

        public int GetVisibleRight()
        {
            return m_visibleRight;
        }

        public int GetFrozenColumnCount()
        {
            return m_frozenCount;
        }

        public GrColumn GetFrozenColumn(int index)
        {
            return GetVisibleColumn(index);
        }

        public int GetUnfrozenColumnCount()
        {
            return GetVisibleColumnCount() - m_frozenCount;
        }

        public GrColumn GetUnfrozenColumn(int index)
        {
            return GetVisibleColumn(index + m_frozenCount);
        }

        public int GetDisplayableColumnCount()
        {
            return m_vecDisplayableColumns.Count;
        }

        public GrColumn GetDisplayableColumn(int index)
        {
            return m_vecDisplayableColumns[index];
        }

        public int GetDisplayableRight() 
        {
            return m_displayableRight; }

        public GrColumnSplitter GetColumnSplitter() {
            return m_columnSplitter; 
        }

        class SortColumnByVisible : IComparer<GrColumn>
        {

            public int Compare(GrColumn x, GrColumn y)
            {
                if (x.m_frozen == y.m_frozen)
                {
                    int v1, v2;

                    if (x.m_visibleIndex != GrDefineUtility.INVALID_INDEX)
                        v1 = x.m_visibleIndex * 1000 + 1;
                    else
                        v1 = x.m_visibleIndexCore * 1000 + 2;

                    if (y.m_visibleIndex != GrDefineUtility.INVALID_INDEX)
                        v2 = y.m_visibleIndex * 1000 + 1;
                    else
                        v2 = y.m_visibleIndexCore * 1000 + 2;

                    if (v1 == v2)
                        return x.m_index.CompareTo(y.m_index);
                    return v1.CompareTo(v2);


                }

                return y.m_frozen.CompareTo(x.m_frozen);
            }
        }

        public int GetUnfrozenX()
        {
            return m_unfrozenX;
        }
    }
}