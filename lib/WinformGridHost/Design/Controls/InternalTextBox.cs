using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Design.Controls
{
    class InternalTextBox : System.Windows.Forms.TextBox
    {
        private IEditorService m_editorService;

        public InternalTextBox(IEditorService editorService)
        {
            m_editorService = editorService;
            this.BorderStyle = BorderStyle.None;
            //this.AutoCompleteMode = AutoCompleteMode.Suggest;
            this.AutoCompleteMode = AutoCompleteMode.None;
            this.AutoCompleteSource = AutoCompleteSource.CustomSource;
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Enter:
                    if (this.Multiline == true && e.Alt == true)
                    {
                        this.SelectedText = "\r\n";
                        return;
                    }
                    e.Handled = true;
                    break;
            }
            base.OnKeyDown(e);
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            if ((e.KeyChar == 13) || (e.KeyChar == 27))
            {
                e.Handled = true;
            }

            base.OnKeyPress(e);
        }
    }
}
