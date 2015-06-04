using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrFocusChangeArgs : EventArgs
    {
        private readonly IFocusable pFocusable;

        public GrFocusChangeArgs(IFocusable pFocusable)
        {
            this.pFocusable = pFocusable;
        }

        public IFocusable GetFocusable()
        {
            return this.pFocusable;
        }
    }
}
