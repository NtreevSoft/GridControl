using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    public class ValueChangingEventArgs : CellEventArgs
    {
        private readonly object m_newValue;
        private readonly object m_oldValue;
        bool m_cancel;


        public ValueChangingEventArgs(Cell cell, object newValue, object oldValue)
            : base(cell)
        {
            m_newValue = newValue;
            m_oldValue = oldValue;
        }

        public bool Cancel
        {
            get { return m_cancel; }
            set { m_cancel = value; }
        }

        public object NewValue
        {
            get { return m_newValue; }
        }

        public object OldValue
        {
            get { return m_oldValue; }
        }
    }
}
