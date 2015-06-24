using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public interface GrTimer : IDisposable
    {
        void Start();
        void Stop();
        void SetInterval(TimeSpan interval);


        event ElapsedEventHandler Elapsed;

        //protected:
        //    void InvokeElapsed(time_t signalTime);
    }
}
