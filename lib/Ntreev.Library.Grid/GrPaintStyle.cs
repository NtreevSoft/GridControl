using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    [Flags]
    public enum GrPaintStyle
    {
        None = 0,
        Focused = 0x00000001,
        Mouseover = 0x00000002,
        Selected = 0x00000004,
        Pressed = 0x00000008,
        BottomLine = 0x00010000,
        RightLine = 0x00020000,
        TopLine = 0x00040000,
        LeftLine = 0x00080000,

        Default = RightLine | BottomLine,
    }
}
