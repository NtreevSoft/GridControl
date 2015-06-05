using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [Flags]
    public enum CellState
    {
        Normal = 0,
        Focused = 1,
        Hot = 2,
        Selected = 4,
        Pressed = 8,

        All = 0x0000000f,
    }
}
