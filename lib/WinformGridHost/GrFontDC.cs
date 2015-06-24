using Ntreev.Library.Grid;
using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    class GrFontDC : GrFont
    {
        private int height;
        private int internalLeading;
        private int externalLeading;

        private IntPtr hFont;
        private IntPtr hdc;

        private int[] characterWidth;

        static GrFontDC()
        {
            GrFont.DefaultFont = new GrFontDC(Control.DefaultFont.ToHfont());
        }

        public GrFontDC(object fontHandle)
        {
            this.hFont = (IntPtr)fontHandle;
            this.hdc = NativeMethods.CreateCompatibleDC(IntPtr.Zero);

            NativeMethods.SelectObject(this.hdc, this.hFont);

            TEXTMETRIC textMetric;
            NativeMethods.GetTextMetrics(this.hdc, out textMetric);
            //wchar_t s[2] = {L'a',0};
            //SIZE sz;
            //GetTextExtentPoint32W(this.hdc, s, 1, &sz);
            this.height = textMetric.tmHeight;
            this.internalLeading = textMetric.tmInternalLeading;
            this.externalLeading = textMetric.tmExternalLeading;

            this.characterWidth = new int[0xffff];
            for (uint i = 0; i < 0xffff; i++)
            {
                this.characterWidth[i] = -1;
            }
        }

        public override void Dispose()
        {
            NativeMethods.DeleteDC(this.hdc);
        }

        public override int GetCharacterWidth(char w)
        {
            SIZE size;

            if (this.characterWidth[w] == -1)
            {
                NativeMethods.GetTextExtentPoint32(this.hdc, w.ToString(), 1, out size);
                this.characterWidth[w] = size.cx;
            }
            return this.characterWidth[w];
        }

        public override int GetHeight() { return this.height; }

        public override int GetInternalLeading() { return this.internalLeading; }

        public override int GetExternalLeading() { return this.externalLeading; }

        public override int GetStringWidth(string text)
        {
            int width = 0;
            foreach (var item in text)
            {
                width += this.GetCharacterWidth(item);
            }

            return width;
        }

        public IntPtr GetFontHandle() { return this.hFont; }
    }
}
