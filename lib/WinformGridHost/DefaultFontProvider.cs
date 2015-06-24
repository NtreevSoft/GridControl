using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    class DefaultFontProvider : IDefaultFontProvider
    {
        public GrFont GetFont()
        {
            return new GrFontDC(Control.DefaultFont.ToHfont());
        }
    }
}
