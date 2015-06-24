using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    [Flags]
    enum ToolInfoFlags : uint
    {
        TTF_IDISHWND = 0x0001,
        TTF_CENTERTIP = 0x0002,
        TTF_RTLREADING = 0x0004,
        TTF_SUBCLASS = 0x0010,
        TTF_TRACK = 0x0020,
        TTF_ABSOLUTE = 0x0080,
        TTF_TRANSPARENT = 0x0100,
        TTF_PARSELINKS = 0x1000,
        TTF_DI_SETITEM = 0x8000,
    }
}
