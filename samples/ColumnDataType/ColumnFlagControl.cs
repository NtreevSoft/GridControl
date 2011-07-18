using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Ntreev.Windows.Forms.Grid;

namespace ColumnAdvancedExtension
{
    class ColumnFlagControl : ColumnDropDown<FlagControl>
    {
        public ColumnFlagControl()
        {
            this.DataType = typeof(NtreevGames);
            this.EditingControl.FlagType = typeof(NtreevGames);

            this.EditingControl.EditOK += new EventHandler(EditingControl_EditOK);
            this.EditingControl.EditCanceled += new EventHandler(EditingControl_EditCanceled);
        }

        public override Size GetPreferredSize(Size proposedSize)
        {
            return new Size(proposedSize.Width, this.EditingControl.Height);
        }

        protected override object GetEditingValue(FlagControl control)
        {
            return (NtreevGames)control.Value;
        }

        protected override void SetEditingValue(FlagControl control, object value)
        {
            if (value == null)
                control.Value = 0;
            else
                control.Value = (int)value;
        }

        void EditingControl_EditOK(object sender, EventArgs e)
        {
            this.EditingResult = EditingResult.Ok;
        }

        void EditingControl_EditCanceled(object sender, EventArgs e)
        {
            this.EditingResult = EditingResult.Cancel;
        }
    }
}