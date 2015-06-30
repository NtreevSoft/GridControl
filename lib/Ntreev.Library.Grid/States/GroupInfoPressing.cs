using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class GroupInfoPressing : GrStateBase
    {
        GrGroup m_pGroup;
        GrPoint m_location;
        int m_where;
        GrCursor m_cursor;
        TargetType m_targetType;

        public GroupInfoPressing()
        {
            m_cursor = GrCursor.Default;
        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            return pHitted is GrGroup;
        }

        public override void OnPaintAdornments(GrGridPainter g, GrRect displayRect)
        {
            if (m_where == GrDefineUtility.INVALID_INDEX)
                return;

            GrGroupPanel groupList = this.GridCore.GroupPanel;
            GrRect paintRect;
            if (m_where < groupList.Groups.Count)
            {
                GrGroup pGroup = groupList.Groups[m_where];
                paintRect = pGroup.Bounds;
            }
            else
            {
                int lastIndex = groupList.Groups.Count - 1;
                GrGroup pGroup = groupList.Groups[lastIndex];
                paintRect = groupList.Bounds;
                int left = pGroup.Bounds.Right;
                int top = paintRect.Top;
                int right = paintRect.Right;
                int bottom = paintRect.Bottom;

                paintRect = GrRect.FromLTRB(left, top, right, bottom);
            }

            const int padding = 2;
            g.FillRectangle(GrRect.FromLTRB(paintRect.Left - padding, paintRect.Top, paintRect.Left + padding, paintRect.Bottom), GrColor.Black);
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_pGroup = e.GetCell() as GrGroup;
            this.GridCore.SetMousePress(m_pGroup);
            m_where = GrDefineUtility.INVALID_INDEX;
            m_location = e.GetLocation();
            m_cursor = GrCursor.Default;
        }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            if (m_location == e.GetLocation())
            {
                GrSort sortType = m_pGroup.GetSortType();
                if (sortType == GrSort.Up)
                    m_pGroup.SetSortType(GrSort.Down);
                else
                    m_pGroup.SetSortType(GrSort.Up);
            }

            this.GridCore.SetMouseUnpress();
            this.GridCore.Invalidate();
        }

        public override void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {
            GrCell pHittedCell = hitTest.pHitted;
            m_cursor = GrCursor.No;

            if (pHittedCell == null)
                return;

            GrRow row = pHittedCell.GetRow();

            m_where = GrDefineUtility.INVALID_INDEX;
            m_targetType = TargetType.Unknown;

            if (row.GetRowType() == GrRowType.GroupPanel)
            {
                if (hitTest.pHitted is GrGroup == true)
                {
                    GrGroup pHittedInfo = pHittedCell as GrGroup;
                    if (pHittedInfo != m_pGroup && m_pGroup.GetGroupLevel() + 1 != pHittedInfo.GetGroupLevel())
                    {
                        m_where = (pHittedCell as GrGroup).GetGroupLevel();
                        m_targetType = TargetType.Group;
                    }
                }
                else
                {
                    GrGroupPanel groupList = row as GrGroupPanel;
                    int lastIndex = groupList.Groups.Count - 1;

                    if (m_pGroup.GetGroupLevel() != lastIndex)
                    {
                        GrGroup pLastGroup = groupList.Groups[lastIndex];
                        if (location.X > pLastGroup.Bounds.Right)
                        {
                            m_where = groupList.Groups.Count;
                            m_targetType = TargetType.Group;
                        }
                    }
                }
            }
            else
            {
                m_targetType = TargetType.Remove;
            }

            switch (m_targetType)
            {
                case TargetType.Group:
                    m_cursor = GrCursor.Move;
                    break;
                case TargetType.Remove:
                    m_cursor = GrCursor.Remove;
                    break;
                default:
                    break;
            }
            this.GridCore.Invalidate();
        }

        public override void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {
            if (cancel == true)
                return;

            switch (m_targetType)
            {
                case TargetType.Remove:
                    m_pGroup.IsGrouped = false;
                    break;
                case TargetType.Group:
                    m_pGroup.SetGroupLevel(m_where);
                    break;
                default:
                    break;
            }
        }

        public override void OnEnd(GrStateEventArgs e)
        {
            m_cursor = GrCursor.Default;
            m_where = GrDefineUtility.INVALID_INDEX;
        }

        public override GrGridState GetState() { return GrGridState.GroupPressing; }

        public override bool GetDragable() { return true; }

        public override GrCursor GetCursor() { return m_cursor; }

        enum TargetType
        {
            Group,
            Remove,
            Unknown,
        }
    }
}
