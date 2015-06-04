using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrDataRowInsertingEventArgs : GrDataRowEventArgs
    {
        private bool cancel;
        private GrDataRowInsertType insertType;

        public GrDataRowInsertingEventArgs(GrDataRow pDataRow, GrDataRowInsertType insertType)
            : base(pDataRow)
        {
            this.insertType = insertType;
        }

        public GrDataRowInsertType GetInsertType()
        {
            return this.insertType;
        }

        public void SetInsertType(GrDataRowInsertType insertType)
        {
            this.insertType = insertType;
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
