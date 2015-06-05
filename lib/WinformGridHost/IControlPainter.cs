using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    interface IControlPainter
    {
        void BitBlt(IntPtr hdc, Control control, Rectangle paintRect, object value);

        void TransparentBlt(IntPtr hdc, Control control, Rectangle paintRect, object value);
    }
}
