using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumnMouseEventArgs : GrMouseEventArgs
    {
        private readonly GrColumn column;
        private bool handled;

        public GrColumnMouseEventArgs(GrColumn column, GrPoint location, GrKeys modifierKeys)
            : base(location, modifierKeys)
        {
            this.column = column;
        }

        public GrColumn Column
        {
            get { return this.column; }
        }

        public bool IsHandled
        {
            get { return this.handled; }
            set { this.handled = value; }
        }
    }
}
