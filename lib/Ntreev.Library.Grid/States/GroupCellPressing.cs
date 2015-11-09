using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class GroupCellPressing : GrStateBase
    {
        GrGroupHeader m_pGroupCell;

        public GroupCellPressing()
        {

        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            GrGroupHeader pGroupCell = pHitted as GrGroupHeader;
            if (pGroupCell == null)
                return false;

            return pGroupCell.HitMouseOverTest(localLocation) == (int)GrMouseOverState.In;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_pGroupCell = e.GetCell() as GrGroupHeader;
            IDataRow pDataRow = m_pGroupCell.GetRow() as IDataRow;

            if (m_pGridWindow.GetSelectionRange() == GrSelectionRange.Multi)
            {
                if (m_pItemSelector.CanSelect(pDataRow) == true)
                {
                    m_pItemSelector.SelectDataRows(pDataRow, m_pItemSelector.GetRowAnchor(), m_pGridWindow.GetSelectionType());
                    m_pFocuser.Set(m_pGroupCell);
                }
            }
            else
            {
                switch (m_pGridWindow.GetSelectionType())
                {
                    case GrSelectionType.Normal:
                        {
                            m_pItemSelector.SetSelectionGroup(pDataRow);
                            m_pItemSelector.SetRowAnchor(pDataRow);
                            GrColumnList columnList = this.GridCore.ColumnList;

                            GrColumn column = columnList.HitTest(e.GetX());
                            if (column != null)
                                m_pItemSelector.SetColumnAnchor(column);
                            m_pItemSelector.ClearSelection();
                            m_pFocuser.Set(m_pGroupCell);
                        }
                        break;
                    default:
                        {
                            if (m_pItemSelector.CanSelect(pDataRow) == true)
                            {
                                //m_pItemSelector.SelectItems(pDataRow, GrSelectionType.Add);
                                m_pFocuser.Set(m_pGroupCell);
                                this.GridCore.SetMousePress(m_pGroupCell);
                            }
                        }
                        break;
                }
            }
            this.GridCore.Invalidate();
        }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            this.GridCore.SetMouseUnpress();
            this.GridCore.Invalidate();
        }

        public override void OnMouseDoubleClick(GrStateMouseEventArgs e)
        {
            GrGroupRow groupRow = m_pGroupCell.GetRow() as GrGroupRow;
            groupRow.IsExpanded = !groupRow.IsExpanded;
            this.GridCore.Invalidate();
        }

        public override GrGridState GetState()
        {
            return GrGridState.GroupHeaderPressing;
        }
    }
}
