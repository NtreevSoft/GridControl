using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    [StructLayout(LayoutKind.Sequential)]
    public struct SCROLLINFO
    {
        public int cbSize;

        public uint fMask;

        public int nMin;

        public int nMax;

        public uint nPage;

        public int nPos;

        public int nTrackPos;
    }
}
