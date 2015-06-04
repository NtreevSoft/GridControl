using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrEditEventArgs : EventArgs
    {
        private readonly GrItem item;
        private readonly GrEditingReason reason;

        private bool handled;

        public GrEditEventArgs(GrItem item, GrEditingReason reason)
        {
            this.item = item;
            this.reason = reason;
        }

        public GrItem GetItem()
        {
            return this.item;
        }

        public GrEditingReason GetReason()
        {
            return this.reason;
        }

        public bool GetHandled()
        {
            return this.handled;
        }

        public void SetHandled(bool b)
        {
            this.handled = b;
        }
    }
}
