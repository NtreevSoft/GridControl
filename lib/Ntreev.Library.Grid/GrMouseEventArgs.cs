using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrMouseEventArgs : EventArgs
    {
        private GrPoint location;
        private GrKeys modifierKeys;
        private int delta;

        public GrMouseEventArgs(GrPoint location, GrKeys modifierKeys)
            : this(location, modifierKeys, 0)
        {

        }

        public GrMouseEventArgs(GrPoint location, GrKeys modifierKeys, int delta)
        {
            this.location = location;
            this.modifierKeys = modifierKeys;
            this.delta = delta;
        }

        public GrPoint GetLocation()
        {
            return this.location;
        }

        public int GetX()
        {
            return this.location.X;
        }

        public int GetY()
        {
            return this.location.Y;
        }

        public bool GetControlKey()
        {
            return (this.modifierKeys & GrKeys.Control) == GrKeys.Control;
        }

        public bool GetAltKey()
        {
            return (this.modifierKeys & GrKeys.Alt) == GrKeys.Control;
        }

        public bool GetShiftKey()
        {
            return (this.modifierKeys & GrKeys.Shift) == GrKeys.Control;
        }

        public GrKeys GetModifierKeys()
        {
            return this.modifierKeys;
        }

        public int GetDelta()
        {
            return this.delta;
        }
    }
}
