using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrScroll : GrObject
    {
        public int ValidateValue(int value)
        {
            value = Math.Max(value, this.Minimum);
            value = Math.Min(value, this.Maximum - this.LargeChange + 1);

            return value;
        }

        public bool DoScroll(int value)
        {
            int newValue = this.ValidateValue(value);

            if (newValue != Value)
            {
                this.Value = newValue;
                this.GridCore.Invalidate();
                return true;
            }
            return false;
        }

        public abstract int Value
        {
            get;
            set;
        }

        public abstract int SmallChange
        {
            get;
            set;
        }

        public abstract int LargeChange
        {
            get;
            set;
        }

        public abstract int Maximum
        {
            get;
            set;
        }

        public abstract int Minimum
        {
            get;
            set;
        }

        public abstract bool IsVisible
        {
            get;
            set;
        }
    }
}
