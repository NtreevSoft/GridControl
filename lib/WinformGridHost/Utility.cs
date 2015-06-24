using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    static class Utility
    {
        //public static bool operator ==(GrColor color1, Color color2)
        //{
        //    return false;
        //}

        public static bool IsNullOrDBNull(object value)
        {
            if (value == null || value == DBNull.Value)
                return true;
            return false;
        }

        public static Font ToManaged(this GrFont pFont)
        {
            if (pFont == null)
                return null;
            if (pFont == GrFont.GetDefaultFont())
                return Control.DefaultFont;

            Font font = pFont.Tag as Font;
            if (font == null)
            {
                IntPtr ptr = GrFontCreator.GetFontHandle(pFont);
                font = System.Drawing.Font.FromHfont(ptr);
                font = new Font(font.FontFamily, font.SizeInPoints, font.Style, System.Windows.Forms.Control.DefaultFont.Unit, font.GdiCharSet);
                pFont.Tag = font;
            }
            return font;
        }

        public static GrFont FromManaged(this Font font)
        {
            if (font == null)
                return null;
            if (Control.DefaultFont == font)
                return GrFont.GetDefaultFont();
            return GrFontCreator.Create(font.ToHfont());
        }
    }
}
