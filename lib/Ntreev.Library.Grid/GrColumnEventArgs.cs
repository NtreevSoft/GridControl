using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrColumnEventArgs : EventArgs
    {
        private readonly GrColumn column;

        public GrColumnEventArgs(GrColumn column)
        {
            this.column = column;
        }

        public GrColumn Column
        {
            get { return this.column; }
        }
    }
}
