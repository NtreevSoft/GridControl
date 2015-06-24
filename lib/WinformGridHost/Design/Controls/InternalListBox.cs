using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Design.Controls
{
    class InternalListBox : ListBox
    {
        private readonly IEditorService editorService;
        private readonly Type dataType;
        private object value;

        public InternalListBox(IEditorService editorService)
        {
            this.editorService = editorService;
            this.BorderStyle = BorderStyle.None;
        }

        public InternalListBox(IEditorService editorService, Type dataType, object value)
        {
            this.editorService = editorService;
            this.value = value;
            this.BorderStyle = BorderStyle.None;

            if (dataType.IsEnum == true)
            {
                foreach (string item in Enum.GetNames(dataType))
                {
                    this.Items.Add(item);
                }

                if (value != null)
                    this.SelectedItem = value.ToString();
            }
            else if (dataType.IsArray == true)
            {
                throw new NotImplementedException();
            }

            this.dataType = dataType;
        }

        public object Value
        {
            get { return this.value; }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            int selIndex = this.IndexFromPoint(e.Location);
            this.SelectedIndex = selIndex;
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                UpdateValue();
                this.editorService.Close();
            }
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            switch (e.KeyCode)
            {
                case System.Windows.Forms.Keys.Enter:
                    {
                        UpdateValue();
                        this.editorService.Close();
                    }
                    break;
            }
        }

        private void UpdateValue()
        {
            if (this.SelectedItem != null)
            {
                if (this.dataType.IsEnum == true)
                {
                    this.value = Enum.Parse(this.dataType, this.SelectedItem as string);
                }
            }
        }

    }
}
