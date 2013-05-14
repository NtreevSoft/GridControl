using Ntreev.Windows.Forms.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApplication
{
    class ColumnDataLocation : ColumnControl<DataLocationPicker>
    {
        public ColumnDataLocation()
        {
            this.DataType = typeof(DataLocation);
        }

        protected override void SetControlValue(DataLocationPicker control, object value)
        {
            if (value == null)
                control.DataLocation = DataLocation.Both;
            else
                control.DataLocation = (DataLocation)value;
        }

        protected override object GetControlValue(DataLocationPicker control)
        {
            return control.DataLocation;
        }
    }
}
