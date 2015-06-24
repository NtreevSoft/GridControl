using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [AttributeUsage(AttributeTargets.Property)]
    public class FrozenAttribute : Attribute
    {
        private readonly bool isFrozen;

        public FrozenAttribute(bool isFrozen)
        {
            this.isFrozen = isFrozen;
        }

        public bool IsFrozen
        {
            get { return this.isFrozen; }
        }
    }
}
