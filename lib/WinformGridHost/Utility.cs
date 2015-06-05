using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
    }
}
