using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrGroupEventArgs : EventArgs
    {
        private readonly GrGroup group;

        public GrGroupEventArgs(GrGroup group)
        {
            this.group = group;
        }

        public GrGroup Group
        {
            get { return this.group; }
        }
    }
}
