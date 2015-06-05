using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrStateMouseEventArgs : GrMouseEventArgs
    {
        private readonly GrPoint m_localHit;
        private GrCell m_pCell;
        private GrGridState m_state;
        private object m_userData;

        bool m_handled;

        public GrStateMouseEventArgs(GrPoint location, GrKeys modifierKeys, GrCell pHitted, GrPoint localHit, GrGridState state, object userData)
            : base(location, modifierKeys)
        {
            m_localHit = localHit;
            m_pCell = pHitted;
            m_state = state;
            m_userData = userData;
        }

        public GrStateMouseEventArgs(GrPoint location, GrKeys modifierKeys, GrCell pHitted, GrPoint localHit, GrGridState state)
            : base(location, modifierKeys)
        {
            m_localHit = localHit;
            m_pCell = pHitted;
            m_state = state;
        }

        public GrPoint GetLocalHit() { return m_localHit; }


        public GrCell GetCell() { return m_pCell; }

        //public T GetCell<T>() where T : GrCell
        //{
        //    return m_pCell as T; }
        public GrGridState GetNextState() { return m_state; }
        public object GetUserData() { return m_userData; }

        public void SetNextState(GrGridState state) { m_state = state; }
        public void SetUserData(object userData) { m_userData = userData; }
        public void SetCell(GrCell pCell) { m_pCell = pCell; }
        public bool GetHandled() { return m_handled; }
        public void SetHandled(bool b) { m_handled = b; }
    }
}
