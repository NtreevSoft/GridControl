using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrElapsedEventArgs : EventArgs
    {
        private readonly TimeSpan m_signalTime;

        public GrElapsedEventArgs(TimeSpan signalTime)
        {
            m_signalTime = signalTime;
        }

        public TimeSpan GetSignalTime()
        {
            return m_signalTime;
        }

    }
}
