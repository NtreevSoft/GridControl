using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [AttributeUsage(AttributeTargets.Property)]
    public class MovableAttribute : Attribute
    {
        private readonly bool isMovable;

        public MovableAttribute(bool isMovable)
        {
            this.isMovable = isMovable;
        }

        public bool IsMovable
        {
            get { return this.isMovable; }
        }
    }
}
