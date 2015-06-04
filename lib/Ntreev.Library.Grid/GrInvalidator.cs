using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public interface GrInvalidator
    {
        void Invalidate();
        void Invalidate(int x, int y, int width, int height);

        void Lock();
        void Unlock();
        void Reset();

        bool IsInvalidated();
    }

}
