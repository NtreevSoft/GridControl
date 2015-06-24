using Ntreev.Windows.Forms.Grid.Design.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class EnumTypeEditor : TypeEditor
    {
        public EnumTypeEditor(Type dataType)
            : base(dataType)
        {

        }

        public override object EditValue(IEditorService editorService, ICell cell, object value)
        {
            InternalListBox listBox = new InternalListBox(editorService, this.DataType, value);

            listBox.BackColor = cell.BackColor;
            listBox.ForeColor = cell.ForeColor;
            listBox.Font = cell.Font;

            listBox.CreateGraphics().Dispose();
            listBox.Size = listBox.PreferredSize;

            if (listBox.Height > listBox.ItemHeight * 10)
            {
                listBox.Height = listBox.ItemHeight * 10;
                editorService.IsDropDownResizable = true;
            }
            editorService.DropDownControl(listBox);
            return listBox.Value; ;
        }

        public override EditStyle GetEditStyle()
        {
            return EditStyle.DropDown;
        }
    }
}
