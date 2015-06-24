using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class BindingListTypeEditor : TypeEditor
    {
        public BindingListTypeEditor(Type dataType)
            : base(dataType)
        {

        }

        public override object EditValue(IEditorService editorService, ICell cell, object value)
        {
            if (Utility.IsNullOrDBNull(value) == true)
                return value;

            GridControl gridControl = new GridControl();
            gridControl.Dock = System.Windows.Forms.DockStyle.Fill;
            gridControl.Caption = cell.Column.Title;
            gridControl.DataSource = value as IBindingList;

            editorService.ShowDialog(gridControl);
            return value;
        }

        public override EditStyle GetEditStyle()
        {
            return Ntreev.Windows.Forms.Grid.Design.EditStyle.Modal;
        }
    }
}
