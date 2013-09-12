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

        }

        protected override void OnGridControlAttached()
        {
            base.OnGridControlAttached();
        }

        protected override void SetControlLayout(EnablePicker control, ICell cell)
        {
            base.SetControlLayout(control, cell);
        }
    }
}
