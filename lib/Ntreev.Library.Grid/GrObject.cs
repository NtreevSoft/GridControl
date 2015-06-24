using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrObject
    {
        private GrGridCore gridCore;

        public bool IsGridCoreAttached() 
        {
            return this.gridCore != null; 
        }

        public GrGridCore GridCore
        {
            get { return this.gridCore; }
            internal set { this.gridCore = value; }
        }

        internal void InvokeOnGridCoreAttached()
        {
            this.OnGridCoreAttached();
        }

        internal void InvokeOnGridCoreDetached()
        {
            this.OnGridCoreDetached();
        }

        protected virtual void OnGridCoreAttached()
        {

        }

        protected virtual void OnGridCoreDetached()
        {

        }
    }
}
