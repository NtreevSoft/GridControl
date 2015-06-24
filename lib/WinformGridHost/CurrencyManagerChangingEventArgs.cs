using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    class CurrencyManagerChangingEventArgs : CurrencyManagerChangedEventArgs
    {
        private bool cancel;
        private string cancelReason;

        public CurrencyManagerChangingEventArgs(CurrencyManager currencyManager)
            : base(currencyManager)
        {

        }

        public bool Cancel
        {
            get { return this.cancel; }
            set { this.cancel = value; }
        }

        public string CancelReason
        {
            get { return this.cancelReason; }
            set { this.cancelReason = value; }
        }
    }
}
