using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class FlagTypeEditor : TypeEditor
    {
        public FlagTypeEditor(Type dataType)
            : base(dataType)
        {

        }

        public override object EditValue(IEditorService editorService, ICell cell, object value)
        {
            Controls.FlagControl flagControl = new Controls.FlagControl(editorService, this.DataType);

            flagControl.Value = value;
            flagControl.BackColor = cell.BackColor;
            flagControl.ForeColor = cell.ForeColor;
            flagControl.Font = cell.Font;

            editorService.DropDownControl(flagControl);
            return flagControl.Value;
        }

        public override EditStyle GetEditStyle()
        {
            return EditStyle.DropDown;
        }
    }
}
