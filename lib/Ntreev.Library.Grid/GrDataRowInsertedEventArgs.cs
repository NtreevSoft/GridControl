using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrDataRowInsertedEventArgs : GrDataRowEventArgs
    {
        private GrDataRowInsertType insertType;

        public GrDataRowInsertedEventArgs(GrDataRow pDataRow, GrDataRowInsertType insertType)
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
    }
}
