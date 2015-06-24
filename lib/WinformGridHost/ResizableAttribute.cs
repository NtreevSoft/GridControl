using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [AttributeUsage(AttributeTargets.Property)]
    public class ResizableAttribute : Attribute
    {
        private readonly bool isResizable;

        public ResizableAttribute(bool isResizable)
        {
            this.isResizable = isResizable;
        }

        public bool IsResizable
        {
            get { return this.isResizable; }
        }
    }
}
