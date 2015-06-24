using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowMovingEventArgs : RowEventArgs
    {
        private readonly int m_index;
        private object m_component;
        private bool m_cancel;

        public RowMovingEventArgs(Row row, int index)
            : base(row)
        {
            m_index = index;
        }

        internal RowMovingEventArgs()
        {

        }

        public bool Cancel
        {
            get { return m_cancel; }
            set { m_cancel = value; }
        }

        public int Index
        {
            get { return m_index; }
        }

        public object Component
        {
            get { return m_component; }
            internal set { m_component = value; }
        }
    }
}
