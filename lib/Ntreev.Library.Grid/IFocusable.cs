using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public interface IFocusable
    {
        IDataRow GetDataRow();
        GrRect GetDisplayRect();
        bool GetDisplayable();
        GrCellType GetCellType();
        void Invalidate();
    }
}
