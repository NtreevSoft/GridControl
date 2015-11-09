using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public interface IFocusable
    {
        
        GrRect GetDisplayRect();
        
        //GrCellType GetCellType();
        void Invalidate();

        bool IsDisplayable
        {
            get;
        }

        IDataRow DataRow
        {
            get;
        }
    }
}
