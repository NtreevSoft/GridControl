using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public sealed class RowBuilder
    {
        internal RowBuilder()
        {

        }

        internal GridControl GridControl
        {
            get;
            set;
        }

        internal GrDataRow NativeRef
        {
            get;
            set;
        }
    }
}
