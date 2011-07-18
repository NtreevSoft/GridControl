using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Design;
using Ntreev.Windows.Forms.Grid;
using Ntreev.Windows.Forms.Grid.Columns;
using System.Reflection;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class ColumnCollectionEditor : CollectionEditor
    {
        public ColumnCollectionEditor(Type type)
            : base(type)
        {
            
        }

        protected override Type[] CreateNewItemTypes()
        {
            return new Type[] { typeof(ColumnTextBox), typeof(ColumnComboBox), typeof(ColumnCheckBox), };
        }

        protected override bool CanRemoveInstance(object value)
        {
            Column column = value as Column;
            if (column.PropertyDescriptor != null)
                return false;
            return base.CanRemoveInstance(value);
        }
    }
}
