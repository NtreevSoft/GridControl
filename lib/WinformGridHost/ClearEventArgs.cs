using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class ClearEventArgs : EventArgs
    {
        private readonly bool dataSourceOnly;

        public ClearEventArgs(bool dataSourceOnly)
        {
            this.dataSourceOnly = dataSourceOnly;
        }

        public bool DataSourceOnly
        {
            get { return dataSourceOnly; }
        }
    }
}