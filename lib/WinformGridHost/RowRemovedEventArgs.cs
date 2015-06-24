using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowRemovedEventArgs : EventArgs
    {
        private bool newSelect;
        private int newIndex;

        public RowRemovedEventArgs(int newIndex)
        {
            this.newIndex = newIndex;
        }

        public bool NewSelect
        {
            get { return this.newSelect; }
            set { this.newSelect = value; }
        }

        public int NewSelectionIndex
        {
            get { return this.newIndex; }
            set { this.newIndex = value; }
        }
    }
}
