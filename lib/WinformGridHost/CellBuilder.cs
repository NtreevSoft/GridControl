using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class CellBuilder
    {
        internal CellBuilder()
        {

        }

        internal GridControl GridControl
        {
            get;
            set;

        }
        internal GrItem NativeRef
        {
            get;
            set;
        }
    }
}
