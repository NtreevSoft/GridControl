using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowBindingEventArgs : EventArgs
    {
        private readonly object component;
        private bool cancel;

        public RowBindingEventArgs(object component)
        {
            this.component = component;
        }

        public object Component
        {
            get { return this.component; }
        }

        public bool Cancel
        {
            get { return this.cancel; }
            set { this.cancel = value; }
        }
    }
}
