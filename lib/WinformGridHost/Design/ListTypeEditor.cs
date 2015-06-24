using Ntreev.Windows.Forms.Grid.Design.Controls;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class ListTypeEditor : TypeEditor
    {
        public ListTypeEditor(Type dataType)
            : base(dataType)
        {

        }

        public override object EditValue(IEditorService editorService, ICell cell, object value)
        {
            InternalListBox listBox = new InternalListBox(editorService, this.DataType, value);

            listBox.BackColor = cell.BackColor;
            listBox.ForeColor = cell.ForeColor;
            listBox.Font = cell.Font;

            listBox.Size = listBox.PreferredSize;

            editorService.DropDownControl(listBox);
            return listBox.SelectedItem;
        }
    }
}
