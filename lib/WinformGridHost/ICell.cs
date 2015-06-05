using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    public interface ICell : ICellBase
    {
        object Value
        {
            get;
        }

        object Tag
        {
            get;
        }

        IColumn Column
        {
            get;
        }
    }
}
