using Ntreev.Windows.Forms.Grid.Columns;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApplication
{
    class ColumnDataTypes : ColumnComboBox
    {
        protected override void SetControlValue(object value)
        {
            //if (value != null && value.GetType() == typeof(string))
            //{
            //    Type type = Type.GetType(value.ToString());
            //    this.Control.SelectedItem = type;
            //    return;
            //}
            base.SetControlValue(value);
        }

        protected override object GetControlValue()
        {
            return base.GetControlValue();
        }
    }
}
