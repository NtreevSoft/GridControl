using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrDataRowMovingEventArgs : GrDataRowEventArgs
    {
        private readonly int index;
        private bool cancel;

        public GrDataRowMovingEventArgs(GrDataRow pDataRow, int index)
            : base(pDataRow)
        {
            this.index = index;
        }

        public int GetIndex()
        {
            return this.index;
        }

        public bool GetCancel()
        {
            return this.cancel;
        }

        public void SetCancel(bool b)
        {
            this.cancel = b;
        }
    }
}
