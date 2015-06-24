using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class Normal : GrStateBase
    {
        GrCell m_pCell;

        public Normal()
        {

        }

        public override void OnBegin(GrStateEventArgs e)
        {
            this.GridCore.SetMouseUnpress();
        }

        public override void OnMouseMove(GrStateMouseEventArgs e)
        {
            GrCell pCell = e.GetCell();
            this.GridCore.SetMouseOver(pCell, e.GetLocalHit());
            InvokeMouseEvent(m_pCell, pCell, e);
            m_pCell = pCell;
        }

        public override void OnMouseWheel(GrMouseEventArgs e)
        {
            GrScroll pVertScroll = this.GridCore.GetVertScroll();
            GrDataRowList dataRowList = this.GridCore.GetDataRowList();

            if (pVertScroll.GetVisible() == false)
                return;

            int scrollLine = m_pGridWindow.GetMouseWheelScrollLines();

            int value = e.GetDelta() / m_pGridWindow.GetMouseWheelScrollDelta() * scrollLine;
            int rowCount = (int)dataRowList.GetDisplayableRowCount();

            if (rowCount < Math.Abs(value))
            {
                if (value < 0)
                    value = -rowCount;
                else
                    value = rowCount;
            }

            value = pVertScroll.GetValue() - value;
            value = pVertScroll.ValidateValue(value);
            pVertScroll.SetValue(value);
            this.GridCore.Invalidate();
        }

        public override bool OnKeyDown(GrKeys key, GrKeys modifierKeys)
        {
            GrFocusMover pMover = this.GridCore.GetFocusMover();

            GrSelectionRange selectionRange = m_pGridWindow.GetSelectionRange();

            switch (key)
            {
                case GrKeys.Escape:
                    return true;
                case GrKeys.Add:
                    {
                        IDataRow pDataRow = m_pFocuser.GetFocusedRow();
                        if (pDataRow != null)
                        {
                            pDataRow.Expand(true);
                            return true;
                        }
                    }
                    return false;
                case GrKeys.Subtract:
                    {
                        IDataRow pDataRow = m_pFocuser.GetFocusedRow();
                        if (pDataRow != null)
                        {
                            pDataRow.Expand(false);
                            return true;
                        }
                    }
                    return false;
                case GrKeys.Tab:
                    {
                        if ((modifierKeys & (GrKeys.Control | GrKeys.Alt)) == 0)
                        {
                            if ((modifierKeys & GrKeys.Shift) == GrKeys.Shift)
                                return pMover.MoveLeft(GrSelectionRange.One);
                            return pMover.MoveRight(GrSelectionRange.One);
                        }
                    }
                    return false;
                case GrKeys.Left:
                    {
                        IFocusable pFocusable = m_pFocuser.Get();
                        if (pFocusable != null)
                        {
                            if ((modifierKeys & GrKeys.Alt) != 0)
                            {
                                IDataRow pDataRow = pFocusable.GetDataRow();
                                pDataRow.Expand(false);
                            }
                            else
                            {
                                return pMover.MoveLeft(GrSelectionRange.One);
                            }
                        }
                    }
                    return false;
                case GrKeys.Up:
                    return pMover.MoveUp(GrSelectionRange.One);

                case GrKeys.Right:
                    {
                        IFocusable pFocusable = m_pFocuser.Get();
                        if (pFocusable != null)
                        {
                            if ((modifierKeys & GrKeys.Alt) != 0)
                            {
                                IDataRow pDataRow = pFocusable.GetDataRow();
                                pDataRow.Expand(true);
                            }
                            else
                            {
                                return pMover.MoveRight(GrSelectionRange.One);
                            }
                        }
                    }
                    return false;
                case GrKeys.Down:
                    return pMover.MoveDown(GrSelectionRange.One);

                case GrKeys.End:
                    if ((modifierKeys & GrKeys.Control) == GrKeys.Control)
                        return pMover.LastRow(selectionRange);
                    return pMover.LastCell(selectionRange);

                case GrKeys.Home:
                    if ((modifierKeys & GrKeys.Control) == GrKeys.Control)
                        return pMover.FirstRow(selectionRange);
                    return pMover.FirstCell(selectionRange);

                case GrKeys.PageUp:
                    return pMover.PageUp(selectionRange);
                case GrKeys.PageDown:
                    return pMover.PageDown(selectionRange);
                default:
                    return false;
            }
        }

        public override GrGridState GetState() { return GrGridState.Normal; }


        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.Cleared += gridCore_Cleared;
        }

        private void gridCore_Cleared(object sender, EventArgs e)
        {
            m_pCell = null;
        }

    }
}
