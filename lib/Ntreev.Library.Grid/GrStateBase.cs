using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrStateBase : GrObject
    {
        internal GrItemSelectorInternal m_pItemSelector;
        internal GrFocuserInternal m_pFocuser;
        internal GrGridWindow m_pGridWindow;

        public GrStateBase()
        {

        }

        public virtual bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            return false;
        }

        public virtual void OnBegin(GrStateEventArgs e)
        {

        }

        public virtual void OnPaintAdornments(GrGridPainter g, GrRect displayRect)
        {

        }

        public virtual void OnEnd(GrStateEventArgs e)
        {

        }

        public virtual void OnMouseDown(GrStateMouseEventArgs e)
        {

        }

        public virtual void OnMouseMove(GrStateMouseEventArgs e)
        {

        }

        public virtual void OnMouseUp(GrStateMouseEventArgs e)
        {

        }

        public virtual void OnMouseClick(GrStateMouseEventArgs e)
        {

        }

        public virtual void OnMouseDoubleClick(GrStateMouseEventArgs e)
        {

        }

        public virtual void OnMouseWheel(GrMouseEventArgs e)
        {

        }

        public virtual bool OnKeyDown(GrKeys key, GrKeys modifierKeys)
        {
            return false;
        }

        public virtual void OnKeyUp(GrKeys key, GrKeys modifierKeys)
        {

        }

        public virtual void OnMouseDragBegin(GrPoint location)
        {

        }

        public virtual void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {

        }

        public virtual void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {

        }

        public abstract GrGridState GetState();

        public virtual bool GetDragable() { return false; }

        public virtual GrCursor GetCursor() { return GrCursor.Default; }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            m_pItemSelector = this.GridCore.ItemSelector as GrItemSelectorInternal;
            m_pFocuser = this.GridCore.Focuser as GrFocuserInternal;
            m_pGridWindow = this.GridCore.GetGridWindow();
        }

        protected void InvokeMouseEvent(GrCell pOldCell, GrCell pNewCell, GrStateMouseEventArgs e)
        {
            if (pOldCell == pNewCell)
            {
                if (pNewCell != null)
                {
                    switch (pNewCell.GetCellType())
                    {
                        case GrCellType.Item:
                            {
                                GrItemMouseEventArgs e1 = new GrItemMouseEventArgs(pNewCell as GrItem, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("ItemMouseMove", e1);
                                if (e1.GetHandled() == true)
                                {
                                    e.SetHandled(true);
                                }
                            }
                            break;
                        case GrCellType.Column:
                            {
                                GrColumnMouseEventArgs e2 = new GrColumnMouseEventArgs(pNewCell as GrColumn, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("ColumnMouseMove", e2);
                                if (e2.IsHandled == true)
                                {
                                    e.SetHandled(true);
                                }
                            }
                            break;
                        case GrCellType.Row:
                            {
                                GrRowMouseEventArgs e2 = new GrRowMouseEventArgs(pNewCell as GrRow, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("RowMouseMove", e2);
                                if (e2.GetHandled() == true)
                                {
                                    e.SetHandled(true);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            else
            {

                if (pOldCell != null)
                {
                    switch (pOldCell.GetCellType())
                    {
                        case GrCellType.Item:
                            {
                                GrItemMouseEventArgs e1 = new GrItemMouseEventArgs(pOldCell as GrItem, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("ItemMouseLeave", e1);
                            }
                            break;
                        case GrCellType.Column:
                            {
                                GrColumnMouseEventArgs e2 = new GrColumnMouseEventArgs(pOldCell as GrColumn, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("ColumnMouseLeave", e2);
                            }
                            break;
                        case GrCellType.Row:
                            {
                                GrRowMouseEventArgs e2 = new GrRowMouseEventArgs(pOldCell as GrRow, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("RowMouseLeave", e2);
                            }
                            break;
                        default:
                            break;
                    }
                }

                if (pNewCell != null)
                {
                    switch (pNewCell.GetCellType())
                    {
                        case GrCellType.Item:
                            {
                                GrItemMouseEventArgs e1 = new GrItemMouseEventArgs(pNewCell as GrItem, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("ItemMouseEnter", e1);
                                this.GridCore.Invoke("ItemMouseMove", e1);
                                if (e1.GetHandled() == true)
                                {
                                    e.SetHandled(true);
                                }
                            }
                            break;
                        case GrCellType.Column:
                            {
                                GrColumnMouseEventArgs e2 = new GrColumnMouseEventArgs(pNewCell as GrColumn, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("ColumnMouseEnter", e2);
                                this.GridCore.Invoke("ColumnMouseMove", e2);
                                if (e2.IsHandled == true)
                                {
                                    e.SetHandled(true);
                                }
                            }
                            break;
                        case GrCellType.Row:
                            {
                                GrRowMouseEventArgs e2 = new GrRowMouseEventArgs(pNewCell as GrRow, e.GetLocalHit(), e.GetModifierKeys());
                                this.GridCore.Invoke("RowMouseEnter", e2);
                                this.GridCore.Invoke("RowMouseMove", e2);
                                if (e2.GetHandled() == true)
                                {
                                    e.SetHandled(true);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}
