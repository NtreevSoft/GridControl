using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public interface ITimer : IDisposable
    {
        void Start();

        void Stop();

        void SetInterval(TimeSpan interval);

        event GrElapsedEventHandler Elapsed;
    }
}
