using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    [StructLayout(LayoutKind.Sequential)]
    struct LOGBRUSH
    {
        public BrushStyle lbStyle;        //brush style

        public UInt32 lbColor;    //colorref RGB(...)

        public HatchStyle lbHatch;        //hatch style
    }
}
