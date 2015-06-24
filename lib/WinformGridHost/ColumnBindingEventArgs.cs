using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class ColumnBindingEventArgs
    {
        private readonly PropertyDescriptor propertyDescriptor;
        private Column column;
        private bool cancel;

        public ColumnBindingEventArgs(PropertyDescriptor propertyDescriptor, Column column)
        {
            this.propertyDescriptor = propertyDescriptor;
            this.column = column;
        }

        public PropertyDescriptor PropertyDescriptor
        {
            get { return this.propertyDescriptor; }
        }

        public Column BindingColumn
        {
            get { return this.column; }
            set { this.column = value; }
        }

        public bool Cancel
        {
            get { return this.cancel; }
            set { this.cancel = value; }
        }
    }
}
