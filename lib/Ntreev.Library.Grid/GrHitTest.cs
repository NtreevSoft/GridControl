using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrHitTest
    {
        public GrCell pHitted;
        public GrPoint localHit;

        public static GrHitTest Empty = new GrHitTest();
    }
}
