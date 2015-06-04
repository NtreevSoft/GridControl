using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrObject
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

        protected virtual void OnGridCoreAttached() { }
        protected virtual void OnGridCoreDetached() { }

    }
}
