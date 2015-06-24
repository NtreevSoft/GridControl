using Ntreev.Library.Grid;
using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    static class GrFontCreator
    {
        private static Dictionary<IntPtr, GrFont> fonts = new Dictionary<IntPtr, GrFont>();

        public static GrFont Create(IntPtr fontHandle)
        {
            if (fonts.ContainsKey(fontHandle) == false)
            {
                GrFont pFont = new GrFontDC(fontHandle);
                fonts.Add(fontHandle, pFont);
            }

            return fonts[fontHandle];
        }

        public static IntPtr GetFontHandle(GrFont pFont)
        {
            return (pFont as GrFontDC).GetFontHandle();
        }
    }
}
