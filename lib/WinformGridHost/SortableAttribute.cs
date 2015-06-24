using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [AttributeUsage(AttributeTargets.Property)]
    public class SortableAttribute : Attribute
    {
        private readonly bool isSortable;

        public SortableAttribute(bool isSortable)
        {
            this.isSortable = isSortable;
        }

        public bool IsSortable
        {
            get { return this.isSortable; }
        }
    }
}
