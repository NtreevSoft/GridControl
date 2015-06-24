using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    [StructLayout(LayoutKind.Sequential)]
    public struct GRADIENT_RECT
    {
        public uint UpperLeft;
        public uint LowerRight;

        public GRADIENT_RECT(uint upLeft, uint lowRight)
        {
            this.UpperLeft = upLeft;
            this.LowerRight = lowRight;
        }
    }

}
