using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroupPanel : GrUpdatableRow
    {
        List<GrGroup> m_vecGroups;
        bool m_enableGroup;

        bool m_groupChanged;
        public GrGroupPanel()
        {
            m_enableGroup = true;
            m_visible = true;
            m_groupChanged = false;
        }
        //virtual ~GrGroupPanel();

        public int GetGroupCount()
        {
            if (m_enableGroup == false)
                return 0;
            return m_vecGroups.Count;
        }

        public GrGroup GetGroup(int level)
        {
            return m_vecGroups[level];
        }

        public void ExpandGroup(int level, bool expand)
        {
            GrGroup pGroup = m_vecGroups[level];
            pGroup.SetExpanded(true);
        }

        public void SetGroupSortState(int level, GrSort sortType)
        {
            GrGroup pGroup = m_vecGroups[level];
            pGroup.SetSortType(sortType);
        }

        public void SetVisible(bool b)
        {
            m_visible = b;
            GrRootRow pHeaderList = GetParent() as GrRootRow;
            pHeaderList.SetVisibleChanged();
        }

        public bool GetGroupable()
        {
            return m_enableGroup;
        }

        public void SetGroupable(bool b)
        {
            if (m_enableGroup == b)
                return;
            m_enableGroup = b;
            Changed(this, EventArgs.Empty);
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
            return m_groupChanged == true;
        }

        public override void Update(bool force)
        {
            if (m_groupChanged == true)
                RepositionGroup();

            if (m_vecGroups.Count == 0)
                SetTextVisible(true);
            else
                SetTextVisible(false);
            m_groupChanged = false;
        }

        public override int GetUpdatePriority() { return GrDefineUtility.UPDATEPRIORITY_GROUPPANEL; }

        public override GrRowType GetRowType() { return GrRowType.GroupPanel; }

        public override int GetWidth()
        {
            return this.GridCore.GetBounds().GetWidth();
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();
            GrRect displayRect = this.GridCore.GetDisplayRect();
            GrColumnList pColumnList = this.GridCore.GetColumnList();
            if (this.GridCore.GetFillBlank() == true && pColumnList.GetDisplayableRight() < displayRect.right)
            {
                paintRect.Right = displayRect.Right;
            }

            if (paintRect.Top >= clipRect.Bottom || paintRect.Bottom < clipRect.Top)
                return;

            GrPaintStyle paintStyle = ToPaintStyle() & ~GrPaintStyle.RightLine;
            GrColor foreColor = GetPaintingForeColor();
            GrColor backColor = GetPaintingBackColor();

            pPainter.DrawItem(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);

            foreach (var value in m_vecGroups)
            {
                value.Paint(pPainter, clipRect);
            }

            DrawText(pPainter, foreColor, paintRect, clipRect);
        }


        public override GrHorzAlign GetTextHorzAlign() { return GrHorzAlign.Left; }

        public virtual bool GetTextMultiline() { return false; }

        public override bool GetVisible()
        {
            if (m_enableGroup == false)
                return false;
            return m_visible;
        }

        public override int GetMinHeight()
        {
            if (m_vecGroups.Count == 0)
            {
                return base.GetMinHeight();
            }
            return m_vecGroups[0].GetHeight() + 20;
        }

        public override GrCell HitTest(GrPoint location)
        {
            if (ContainsVert(location.Y) == false)
                return null;

            foreach (var value in m_vecGroups)
            {
                GrRect rect = value.GetRect();
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
            RepositionGroup();
        }


        private void gridCore_Cleared(object pSender, EventArgs e)
        {
            m_vecGroups.Clear();
            SetFit();
        }

        private void gridCore_Created(object pSender, EventArgs e)
        {
            GrColumnList pColumnList = this.GridCore.GetColumnList();
            pColumnList.ColumnGroupChanged += columnList_ColumnGroupChanged;
            pColumnList.ColumnInserted+= columnList_ColumnInserted;
            pColumnList.ColumnRemoved += columnList_ColumnRemoved;
        }

        private void gridCore_FontChanged(object pSender, EventArgs e)
        {
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            pTextUpdater.AddTextBounds(this);

            foreach (var value in m_vecGroups)
            {
                pTextUpdater.AddTextBounds(value);
            }
        }

        private void columnList_ColumnInserted(object pSender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();
            if (pColumn.GetGrouped() == false)
                return;
            AddGroup(pColumn.GetGroup());
        }

        private void columnList_ColumnRemoved(object pSender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();
            if (pColumn.GetGrouped() == false)
                return;
            RemoveGroup(pColumn.GetGroup());
        }

        private void columnList_ColumnGroupChanged(object pSender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();
            if (pColumn.GetGrouped() == true)
                AddGroup(pColumn.GetGroup());
            else
                RemoveGroup(pColumn.GetGroup());
        }

        private void groupInfo_LevelChanged(object pSender, EventArgs e)
        {
            GrGroup pGroup = pSender as GrGroup;
            _Groups.iterator itor = std.find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);
            *itor = null;

            int index = std.min(pGroup.GetGroupLevel(), m_vecGroups.Count);

            m_vecGroups.insert(m_vecGroups.begin() + index, pGroup);
            m_vecGroups.Remove(std.find(m_vecGroups.begin(), m_vecGroups.end(), (GrGroup*)null));

            index = 0;
            foreach (var value in m_vecGroups)
            {
                value.SetGroupLevelCore(index++);
            }

            Changed.Raise(this, EventArgs.Empty);
        }

        private void ResetGroupLevel()
        {
            int index = 0;
            foreach (var value in m_vecGroups)
            {
                value.SetGroupLevelCore(index++);
            }
        }
        private void AddGroup(GrGroup pGroup)
        {
            _Groups.iterator itor = find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);

            if (itor != m_vecGroups.end())
                throw new Exception("이미 Group이 되어 있습니다.");

            uint level = pGroup.GetGroupLevel();
            if (level > m_vecGroups.Count)
                m_vecGroups.push_back(pGroup);
            else
                m_vecGroups.insert(m_vecGroups.begin() + level, pGroup);
            pGroup.SetText();
            pGroup.LevelChanged.Add(this, &GrGroupPanel.groupInfo_LevelChanged);

            ResetGroupLevel();
            SetFit();
            this.GridCore.Invalidate();
            m_groupChanged = true;
            Changed.Raise(this, EventArgs.Empty);
        }

        private void RemoveGroup(GrGroup pGroup)
        {
            _Groups.iterator itor = find(m_vecGroups.begin(), m_vecGroups.end(), pGroup);

            if (itor == m_vecGroups.end())
                throw new Exception("Group이 되어 있지 않은데 해제하려고 합니다.");

            pGroup.LevelChanged -= groupInfo_LevelChanged;
            (*itor).SetGroupLevelCore(GrDefineUtility.INVALID_INDEX);
            m_vecGroups.Remove(itor);

            ResetGroupLevel();
            SetFit();
            this.GridCore.Invalidate();
            m_groupChanged = true;
            Changed.Raise(this, EventArgs.Empty);
        }
        private void RepositionGroup()
        {
            GrPoint pt = new GrPoint();
            pt.X = GetX();
            pt.Y = GetY();

            pt.X += 10;
            pt.Y += 10;
            foreach (var value in m_vecGroups)
            {
                value.SetPosition(pt);
                pt.X += value.GetWidth() + 10;
            }
        }
    }
}
