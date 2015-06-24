using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    class CurrencyManagerChangedEventArgs : EventArgs
    {
        private readonly CurrencyManager currencyManager;

        public CurrencyManagerChangedEventArgs(CurrencyManager currencyManager)
        {
            this.currencyManager = currencyManager;
        }

        public CurrencyManager CurrecnyManager
        {
            get { return this.currencyManager; }
        }
    }
}
