using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    class ControlPainter : IControlPainter
    {
        private IntPtr m_dc;
        private int m_width;
        private int m_height;

        private IntPtr m_colorKeyBrush;
        private readonly uint m_colorKey;

        public ControlPainter()
        {
            m_colorKey = NativeMethods.RGB(0, 128, 128);
            m_width = 500;
            m_height = 500;

            m_dc = NativeMethods.CreateCompatibleDC(IntPtr.Zero);
            IntPtr hBitmap = NativeMethods.CreateBitmap(m_width, m_height, 1, 32, IntPtr.Zero);
            NativeMethods.SelectObject(m_dc, hBitmap);
            NativeMethods.SetBkMode(m_dc, BkMode.TRANSPARENT);

            m_colorKeyBrush = NativeMethods.CreateSolidBrush(240, 240, 240);
        }

        public void BitBlt(IntPtr hdc, Control control, Rectangle paintRect, object value)
        {
            if (m_width < paintRect.Width || m_height < paintRect.Height)
            {
                m_width = Math.Max(m_width, paintRect.Width);
                m_height = Math.Max(m_height, paintRect.Height);
                IntPtr hh = NativeMethods.CreateBitmap(m_width, m_height, 1, 32, IntPtr.Zero);
                IntPtr hOldBitmap = NativeMethods.SelectObject(m_dc, hh);
                NativeMethods.DeleteObject(hOldBitmap);
            }

            IntPtr hControl = control.Handle;

            RECT r;
            NativeMethods.SetRect(out r, 0, 0, m_width, m_height);
            NativeMethods.FillRect((IntPtr)m_dc, ref r, m_colorKeyBrush);

            NativeMethods.SendMessage(hControl, WindowMessages.WM_PRINT, m_dc, new IntPtr((int)(NativeMethods.PRF_CLIENT | NativeMethods.PRF_CHILDREN)));

            IntPtr hRgn = NativeMethods.CreateRectRgn(paintRect.X, paintRect.Y, paintRect.Right, paintRect.Bottom);
            IntPtr restoreRegion = NativeMethods.CreateRectRgn(0, 0, 0, 0);
            if (NativeMethods.GetClipRgn(hdc, restoreRegion) != 1)
            {
                NativeMethods.DeleteObject(restoreRegion);
                restoreRegion = IntPtr.Zero;
            }

            NativeMethods.SelectClipRgn(hdc, hRgn);

            NativeMethods.BitBlt(hdc, paintRect.X + control.Left, paintRect.Y + control.Top, control.Width, control.Height, m_dc, 0, 0, TernaryRasterOperations.SRCCOPY);

            NativeMethods.SelectClipRgn(hdc, restoreRegion);
            NativeMethods.DeleteObject(hRgn);
            NativeMethods.DeleteObject(restoreRegion);
        }

        public void TransparentBlt(IntPtr hdc, System.Windows.Forms.Control control, System.Drawing.Rectangle paintRect, object value)
        {
            IntPtr hControl = control.Handle;

            int sgm = NativeMethods.SetGraphicsMode(hdc, WinGdi.GM_ADVANCED);
            XFORM form, form2;
            NativeMethods.GetWorldTransform(hdc, out form);
            form2 = form;
            form2.eDx = (float)paintRect.X;
            form2.eDy = (float)paintRect.Y;
            NativeMethods.SetWorldTransform(hdc, ref form2);

            IntPtr hRgn = NativeMethods.CreateRectRgn(paintRect.X, paintRect.Y, paintRect.Right, paintRect.Bottom);
            IntPtr restoreRegion = NativeMethods.CreateRectRgn(0, 0, 0, 0);
            if (NativeMethods.GetClipRgn(hdc, restoreRegion) != 1)
            {
                NativeMethods.DeleteObject(restoreRegion);
                restoreRegion = IntPtr.Zero;
            }

            NativeMethods.SelectClipRgn(hdc, hRgn);

            NativeMethods.SendMessage(hControl, WindowMessages.WM_PRINT, hdc, new IntPtr((int)(NativeMethods.PRF_CLIENT | NativeMethods.PRF_CHILDREN)));

            NativeMethods.SelectClipRgn(hdc, restoreRegion);
            NativeMethods.DeleteObject(hRgn);
            NativeMethods.DeleteObject(restoreRegion);

            NativeMethods.SetWorldTransform(hdc, ref form);
            NativeMethods.SetGraphicsMode(hdc, sgm);
        }
    }
}
