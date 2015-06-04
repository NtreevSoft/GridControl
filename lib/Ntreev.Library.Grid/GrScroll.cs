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
            value = Math.Max(value, GetMinimum());
            value = Math.Min(value, GetMaximum() - GetLargeChange() + 1);

            return value;
        }
        public bool DoScroll(int value)
        {
            int newValue = ValidateValue(value);

            if (newValue != GetValue())
            {
                SetValue(newValue);
                this.GridCore.Invalidate();
                return true;
            }
            return false;
        }

        public abstract int GetValue();
        public abstract void SetValue(int value);
        public abstract int GetSmallChange();
        public abstract void SetSmallChange(int value);
        public abstract int GetLargeChange();
        public abstract void SetLargeChange(int value);
        public abstract int GetMaximum();
        public abstract void SetMaximum(int value);
        public abstract int GetMinimum();
        public abstract void SetMinimum(int value);
        public abstract bool GetVisible();
        public abstract void SetVisible(bool value);
    }
}
