using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public static class Extensions
    {

        public static void Invalidate(this GrInvalidator invalidator, GrRect rect)
        {
            invalidator.Invalidate(rect.Left, rect.Top, rect.GetWidth(), rect.GetHeight());
        }
    }
}
