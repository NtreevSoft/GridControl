using Ntreev.Library.Grid.States;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrStateManager : GrObject
    {
        private readonly List<GrStateBase> m_states = new List<GrStateBase>();
        GrStateBase m_state;
        GrStateBase m_downState;
        GrStateBase m_defaultState;

        GrPoint m_downLocation;
        GrRect m_dragRectangle;
        bool m_dragging;

        GrGridWindow m_pGridWindow;
        public GrStateManager()
        {
            m_states.Add(new Normal());
            m_states.Add(new RootPressing());
            m_states.Add(new ColumnPressing());
            m_states.Add(new ColumnResizing());
            m_states.Add(new ColumnSplitterMoving());
            m_states.Add(new RowPressing());
            m_states.Add(new RowResizing());
            m_states.Add(new GroupInfoPressing());
            m_states.Add(new GroupCellPressing());
            m_states.Add(new ExpanderPressing());
            m_states.Add(new ItemPressing());
            m_states.Add(new ItemButtonPressing());
            m_states.Add(new ItemEditing());

            m_state = null;
            m_defaultState = m_states[0];
            m_dragging = false;
        }


        public void ChangeDefaultState()
        {
            if (m_state != m_defaultState)
                ChangeState(m_defaultState, null, null);
        }

        public void ChangeState(GrGridState state, GrCell pCell, object userData)
        {
            ChangeState(m_states[(int)state], pCell, userData);
        }

        public bool SetCursor()
        {
            return m_pGridWindow.SetCursor((int)m_state.GetCursor());
        }

        public GrGridState GetHitTest(GrPoint location)
        {
            GrHitTest hitTest;
            if (this.GridCore.HitTest(location, out hitTest) == false)
                return GrGridState.Unknown;
            GrStateBase base1 = GetHitTest(hitTest.pHitted, hitTest.localHit);
            return base1 != null ? base1.GetState() : GrGridState.Unknown;
        }

        public GrGridState GetGridState() { return m_state.GetState(); }

        public void OnMouseDown(GrPoint location, GrKeys modifierKeys)
        {
            using (Invalidator invalidator = new Invalidator(this.GridCore))
            {

                m_downState = null;
                GrHitTest hitTest;

                if (this.GridCore.HitTest(location, out hitTest) == true)
                {
                    GrStateBase hittedState = GetHitTest(hitTest.pHitted, hitTest.localHit);
                    if (hittedState == null)
                    {
                        ChangeDefaultState();
                        return;
                    }
                    ChangeState(hittedState, hitTest.pHitted, null);
                    m_downState = m_state;

                    if (m_state == hittedState)
                    {
                        GrStateMouseEventArgs e = new GrStateMouseEventArgs(location, modifierKeys, hitTest.pHitted, hitTest.localHit, hittedState.GetState(), null);
                        hittedState.OnMouseDown(e);

                        if (e.GetNextState() != hittedState.GetState())
                        {
                            ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
                        }

                        m_downLocation = location;
                        m_dragRectangle = ComputeDragRectangle(location);
                    }
                }
            }
        }

        public void OnMouseMove(GrPoint location, GrKeys modifierKeys, bool pressed)
        {
            using (Invalidator invalidator = new Invalidator(this.GridCore))
            {

                GrHitTest hitTest;
                this.GridCore.HitTest(location, out hitTest);

                if (pressed == true)
                {
                    if (m_state == null || m_state.GetDragable() == false)
                        return;

                    if (m_dragging == false && m_dragRectangle.Contains(location) == false)
                    {
                        m_dragging = true;
                        m_state.OnMouseDragBegin(location);
                    }

                    if (m_dragging == true)
                    {
                        m_state.OnMouseDragMove(location, hitTest);
                        m_pGridWindow.SetCursor((int)m_state.GetCursor());
                    }
                }
                else
                {
                    GrCursor cursor = m_defaultState.GetCursor();

                    if (hitTest.pHitted != null)
                    {
                        GrStateBase hittedState = GetHitTest(hitTest.pHitted, hitTest.localHit);
                        if (hittedState != null)
                            cursor = hittedState.GetCursor();
                    }

                    GrStateMouseEventArgs e = new GrStateMouseEventArgs(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_state.GetState());
                    m_state.OnMouseMove(e);

                    if (e.GetHandled() == false)
                        m_pGridWindow.SetCursor((int)cursor);
                }
            }
        }

        public void OnMouseUp(GrPoint location, GrKeys modifierKeys)
        {
            using (Invalidator invalidator = new Invalidator(this.GridCore))
            {

                if (m_state != null)
                {
                    GrHitTest hitTest;
                    this.GridCore.HitTest(location, out hitTest);

                    if (m_dragging == true)
                    {
                        m_state.OnMouseDragEnd(false, hitTest);
                        m_dragging = false;
                    }

                    GrStateMouseEventArgs e = new GrStateMouseEventArgs(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_defaultState.GetState());
                    m_state.OnMouseUp(e);

                    if (e.GetNextState() != m_state.GetState())
                    {
                        ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
                    }
                }
            }
        }

        public void OnMouseClick(GrPoint location, GrKeys modifierKeys)
        {
            using (Invalidator invalidator = new Invalidator(this.GridCore))
            {

                if (m_downState != null)
                {
                    GrHitTest hitTest;
                    this.GridCore.HitTest(location, out hitTest);

                    GrStateMouseEventArgs e = new GrStateMouseEventArgs(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_downState.GetState());

                    m_downState.OnMouseClick(e);

                    if (e.GetNextState() != m_downState.GetState())
                    {
                        ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
                    }
                }
            }
        }

        public void OnMouseDoubleClick(GrPoint location, GrKeys modifierKeys)
        {
            using (Invalidator invalidator = new Invalidator(this.GridCore))
            {

                if (m_downState != null)
                {
                    GrHitTest hitTest;
                    this.GridCore.HitTest(location, out hitTest);

                    GrStateMouseEventArgs e = new GrStateMouseEventArgs(location, modifierKeys, hitTest.pHitted, hitTest.localHit, m_downState.GetState());
                    m_downState.OnMouseDoubleClick(e);

                    if (e.GetNextState() != m_downState.GetState())
                    {
                        ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
                    }
                }
            }
        }

        public void OnMouseWheel(GrPoint location, GrKeys modifierKeys, int delta)
        {
            using (Invalidator invalidator = new Invalidator(this.GridCore))
            {

                if (m_state != null)
                {
                    GrMouseEventArgs e = new GrMouseEventArgs(location, modifierKeys, delta);
                    m_state.OnMouseWheel(e);
                }
            }
        }

        public bool OnKeyDown(GrKeys key, GrKeys modifierKeys)
        {
            using (Invalidator invalidator = new Invalidator(this.GridCore))
            {

                if (m_dragging == true)
                {
                    if (key == GrKeys.Escape)
                    {
                        m_state.OnMouseDragEnd(true, GrHitTest.Empty);
                        m_dragging = false;

                        GrStateMouseEventArgs e = new GrStateMouseEventArgs(GrPoint.Empty, modifierKeys, null, GrPoint.Empty, m_defaultState.GetState());
                        m_state.OnMouseUp(e);

                        if (e.GetNextState() == m_defaultState.GetState())
                            ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
                        return true;
                    }
                }
                else if (m_state != null)
                {
                    return m_state.OnKeyDown(key, modifierKeys);
                }
                return false;
            }
        }

        public void OnKeyUp(GrKeys key, GrKeys modifierKeys)
        {
            if (m_state != null)
            {
                m_state.OnKeyUp(key, modifierKeys);
            }
        }

        public void OnPaint(GrGridPainter pGridPainter)
        {
            if (m_state != null)
                m_state.OnPaintAdornments(pGridPainter, this.GridCore.GetDisplayRect());
        }

        public void OnClearing()
        {
            ChangeDefaultState();
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            m_pGridWindow = this.GridCore.GetGridWindow();

            foreach (var value in m_states)
            {
                this.GridCore.AttachObject(value);
            }

            ChangeDefaultState();
        }

        private void ChangeState(GrStateBase state, GrCell pHitted, object userData)
        {
            GrStateBase oldState = m_state;
            if (oldState != null)
            {
                GrStateEventArgs e = new GrStateEventArgs(null, oldState.GetState(), null);
                oldState.OnEnd(e);
                if (e.GetNextState() != oldState.GetState())
                {
                    ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
                    return;
                }
            }

            {
                GrStateEventArgs e = new GrStateEventArgs(pHitted, state.GetState(), userData);
                state.OnBegin(e);

                m_state = state;
                //m_pGridWindow.SetCursor(m_state.GetCursor());

                if (e.GetNextState() != m_state.GetState())
                    ChangeState(e.GetNextState(), e.GetCell(), e.GetUserData());
            }
        }

        private GrStateBase GetHitTest(GrCell pHitted, GrPoint localHitted)
        {
            foreach (var value in m_states)
            {
                if (value.GetHitTest(pHitted, localHitted) == true)
                {
                    return value;
                }
            }
            return null;
        }

        private GrRect ComputeDragRectangle(GrPoint location)
        {
            GrSize dragSize = m_pGridWindow.GetDragSize();
            return new GrRect(new GrPoint(location.X - (dragSize.Width / 2), location.Y - (dragSize.Height / 2)), dragSize);
        }

        class Invalidator : IDisposable
        {
            private readonly GrGridCore gridCore;

            public Invalidator(GrGridCore pGridCore)
            {
                this.gridCore = pGridCore;
                this.gridCore.LockInvalidate();
            }

            public void Dispose()
            {
                this.gridCore.UnlockInvalidate();
            }

        }
    }
}
