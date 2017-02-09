using Ntreev.Windows.Forms.Grid;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApplication
{
    class ColumnEnable : ColumnControl<EnablePicker>
    {
        public ColumnEnable()
        {
            this.Control.CheckedChanged += Control_CheckedChanged;
            this.DataType = typeof(bool);
        }

        void Control_CheckedChanged(object sender, EventArgs e)
        {
            this.CloseControl();
        }

        protected override void SetControlValue(EnablePicker control, ICell cell, object value)
        {
            if (value == null)
                control.Checked = true;
            else
                control.Checked = (bool)value;
        }

        protected override object GetControlValue(EnablePicker control)
        {
            return control.Checked;
        }
    }
}
