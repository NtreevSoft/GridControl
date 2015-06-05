using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    static class GrGridUtility
    {
        public static bool Equals<T>(T[] left, T[] right)
        {
            if (left == null && right == null)
            {
                return true;
            }

            if (left == null || left == null)
            {
                return false;
            }

            return left.SequenceEqual(right);
        }

        public static int GetHashCode<T>(T[] obj)
        {
            if (obj == null)
                return 0;

            return obj.Aggregate(0, (code, item) => code ^ item.GetHashCode());
        }

        public static int GetHashCode<T>(T obj)
        {
            if (obj == null)
                return 0;

            return obj.GetHashCode();
        }
    }
}
