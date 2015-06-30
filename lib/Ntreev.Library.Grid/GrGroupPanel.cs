using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroupPanel : GrUpdatableRow
    {
        private readonly List<GrGroup> groups = new List<GrGroup>();
        private bool isGroupable = true;
        private bool groupChanged;

        public GrGroupPanel()
        {
            
        }

        public IReadOnlyList<GrGroup> Groups
        {
            get { return this.groups; }
        }

        public void ExpandGroup(int level, bool expand)
        {
            GrGroup pGroup = this.groups[level];
            pGroup.SetExpanded(true);
        }

        public void SetGroupSortState(int level, GrSort sortType)
        {
            GrGroup pGroup = this.groups[level];
            pGroup.SetSortType(sortType);
        }

        protected override void OnVisibleChanged(EventArgs e)
        {
            GrRootRow pHeaderList = this.GetParent() as GrRootRow;
            pHeaderList.SetVisibleChanged();

            base.OnVisibleChanged(e);
        }

        public bool IsGroupable
        {
            get { return this.isGroupable; }
            set
            {
                if (this.isGroupable == value)
                    return;
                this.isGroupable = value;
                this.OnChanged(EventArgs.Empty);
            }
        }

        public void NotifyExpanded(GrGroup pGroup)
        {
            GrGroupEventArgs e = new GrGroupEventArgs(pGroup);
            Expanded(this, e);
        }

        public void NotifySortChanged(GrGroup pGroup)
        {
            GrGroupEventArgs e = new GrGroupEventArgs(pGroup);
            SortChanged(this, e); ;
        }

        public override bool ShouldUpdate()
        {
            return this.groupChanged == true;
        }

        public override void Update(bool force)
        {
            if (this.groupChanged == true)
                RepositionGroup();

            if (this.groups.Count == 0)
                this.IsTextVisible = true;
            else
                this.IsTextVisible = false;
            this.groupChanged = false;
        }

        public override int GetUpdatePriority() { return GrDefineUtility.UPDATEPRIORITY_GROUPPANEL; }

        public override GrRowType GetRowType() { return GrRowType.GroupPanel; }

        //public override int Width
        //{
        //    get { return this.GridCore.GetBounds().Width; }
        //}

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;
            GrRect displayRect = this.GridCore.DisplayRectangle;
            GrColumnList columnList = this.GridCore.ColumnList;
            if (this.GridCore.GetFillBlank() == true && columnList.GetDisplayableRight() < displayRect.Right)
            {
                paintRect.Width = displayRect.Right - paintRect.Left;
            }

            if (paintRect.Top >= clipRect.Bottom || paintRect.Bottom < clipRect.Top)
                return;

            GrPaintStyle paintStyle = ToPaintStyle() & ~GrPaintStyle.RightLine;
            GrColor foreColor = GetPaintingForeColor();
            GrColor backColor = GetPaintingBackColor();

            painter.DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);

            foreach (var value in this.groups)
            {
                value.Paint(painter, clipRect);
            }

            DrawText(painter, foreColor, paintRect, clipRect);
        }

        public override GrHorzAlign TextHorzAlign
        {
            get { return GrHorzAlign.Left; }
        }

        public virtual bool GetTextMultiline() { return false; }

        public override bool IsVisible
        {
            get
            {
                if (this.isGroupable == false)
                    return false;
                return base.IsVisible;
            }
            set { base.IsVisible = value; }
        }

        public override int GetMinHeight()
        {
            if (this.groups.Count == 0)
            {
                return base.GetMinHeight();
            }
            return this.groups[0].Height + 20;
        }

        public override GrCell HitTest(GrPoint location)
        {
            if (ContainsVert(location.Y) == false)
                return null;

            foreach (var value in this.groups)
            {
                GrRect rect = value.Bounds;
                if (rect.Contains(location) == true)
                    return value;
            }

            return this as GrGroupPanel;
        }

        public override GrColor GetForeColor()
        {
            GrColor color = base.GetForeColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetGroupPanelForeColor();

            return base.GetForeColor();
        }

        public override GrColor GetBackColor()
        {
            GrColor color = base.GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetGroupPanelBackColor();

            return base.GetBackColor();
        }

        public override GrColor GetLineColor()
        {
            GrColor color = base.GetLineColorCore();
            if (color != GrColor.Empty)
                return color;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetGroupPanelLineColor();

            return base.GetLineColor();
        }

        public override GrFont GetFont()
        {
            GrFont pFont = base.GetFontCore();
            if (pFont != null)
                return pFont;

            GrStyle pStyle = this.GridCore.GetStyle();
            if (pStyle != null)
                return pStyle.GetGroupPanelFont();

            return base.GetFont();
        }

        public override bool GetResizable() { return false; }

        public event EventHandler Changed;

        public event GroupEventHandler Expanded;

        public event GroupEventHandler SortChanged;

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.Cleared += gridCore_Cleared;
            this.GridCore.Created += gridCore_Created;
            this.GridCore.FontChanged += gridCore_FontChanged;
        }

        protected override void OnYChanged()
        {
            base.OnYChanged();
            this.RepositionGroup();
        }

        protected virtual void OnChanged(EventArgs e)
        {
            if (this.Changed != null)
            {
                this.Changed(this, e);
            }
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            this.groups.Clear();
            SetFit();
        }

        private void gridCore_Created(object sender, EventArgs e)
        {
            GrColumnList columnList = this.GridCore.ColumnList;
            columnList.ColumnGroupChanged += columnList_ColumnGroupChanged;
            columnList.ColumnInserted += columnList_ColumnInserted;
            columnList.ColumnRemoved += columnList_ColumnRemoved;
        }

        private void gridCore_FontChanged(object sender, EventArgs e)
        {
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            pTextUpdater.AddTextBounds(this);

            foreach (var value in this.groups)
            {
                pTextUpdater.AddTextBounds(value);
            }
        }

        private void columnList_ColumnInserted(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            if (column.IsGrouped == false)
                return;
            AddGroup(column.Group);
        }

        private void columnList_ColumnRemoved(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            if (column.IsGrouped == false)
                return;
            RemoveGroup(column.Group);
        }

        private void columnList_ColumnGroupChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            if (column.IsGrouped == true)
                AddGroup(column.Group);
            else
                RemoveGroup(column.Group);
        }

        private void groupInfo_LevelChanged(object sender, EventArgs e)
        {
            GrGroup pGroup = sender as GrGroup;

            this.groups[this.groups.IndexOf(pGroup)] = null;

            int index = Math.Min(pGroup.GetGroupLevel(), this.groups.Count);

            this.groups.Insert(index, pGroup);
            this.groups.Remove(null);

            index = 0;
            foreach (var value in this.groups)
            {
                value.SetGroupLevelCore(index++);
            }

            Changed(this, EventArgs.Empty);
        }

        private void ResetGroupLevel()
        {
            int index = 0;
            foreach (var value in this.groups)
            {
                value.SetGroupLevelCore(index++);
            }
        }

        private void AddGroup(GrGroup pGroup)
        {
            int index = this.groups.IndexOf(pGroup);

            if (index >= 0)
                throw new Exception("이미 Group이 되어 있습니다.");

            int level = pGroup.GetGroupLevel();
            if (level > this.groups.Count)
                this.groups.Add(pGroup);
            else
                this.groups.Insert(level, pGroup);
            pGroup.Text = pGroup.Column.Text;
            pGroup.LevelChanged += groupInfo_LevelChanged;

            ResetGroupLevel();
            SetFit();
            this.GridCore.Invalidate();
            this.groupChanged = true;
            Changed(this, EventArgs.Empty);
        }

        private void RemoveGroup(GrGroup pGroup)
        {
            int index = this.groups.IndexOf(pGroup);

            if (index < 0)
                throw new Exception("Group이 되어 있지 않은데 해제하려고 합니다.");

            pGroup.LevelChanged -= groupInfo_LevelChanged;
            pGroup.SetGroupLevelCore(GrDefineUtility.INVALID_INDEX);
            this.groups.Remove(pGroup);

            ResetGroupLevel();
            SetFit();
            this.GridCore.Invalidate();
            this.groupChanged = true;
            Changed(this, EventArgs.Empty);
        }

        private void RepositionGroup()
        {
            GrPoint pt = new GrPoint();
            pt.X = this.X;
            pt.Y = this.Y;

            pt.X += 10;
            pt.Y += 10;

            foreach (var item in this.groups)
            {
                item.Location = pt;
                pt.X += item.Width + 10;
            }
        }
    }
}
