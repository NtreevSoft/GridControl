using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Crema.Library
{
    public static class DateTimeExtension
    {
        public static long GetTotalSeconds(this DateTime dateTime)
        {
            TimeSpan delta = dateTime - new DateTime(1970, 1, 1);
            return Convert.ToInt64(delta.TotalSeconds);
        }
    }
}
