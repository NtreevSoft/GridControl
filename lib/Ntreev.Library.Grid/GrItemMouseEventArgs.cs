using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrItemMouseEventArgs : GrMouseEventArgs
    {
        private readonly GrItem item;
        private bool handled;

        public GrItemMouseEventArgs(GrItem item, GrPoint location, GrKeys modifierKeys)
            : base(location, modifierKeys)
        {
            this.item = item;
        }

        public GrItem GetItem()
        {
            return this.item;
        }

        public bool GetHandled()
        {
            return this.handled;
        }

        public void SetHandled(bool value)
        {
            this.handled = value;
        }
    }
}
