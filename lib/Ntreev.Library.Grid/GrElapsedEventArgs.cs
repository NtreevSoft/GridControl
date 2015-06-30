using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrElapsedEventArgs : EventArgs
    {
        private readonly TimeSpan signalTime;

        public GrElapsedEventArgs(TimeSpan signalTime)
        {
            this.signalTime = signalTime;
        }

        public TimeSpan GetSignalTime()
        {
            return this.signalTime;
        }

    }
}
