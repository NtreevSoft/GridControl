using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrCellMouseEventArgs : GrMouseEventArgs
    {
        private readonly GrCell cell;

        public GrCellMouseEventArgs(GrCell cell, GrPoint location, GrKeys modifierKeys)
            : base(location, modifierKeys)
        {
            this.cell = cell;
        }

        public GrCell GetCell()
        {
            return this.cell;
        }
    }
}
