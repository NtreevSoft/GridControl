using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Design.Controls
{
    class InternalCheckBox : System.Windows.Forms.CheckBox
    {
        private readonly IEditorService editorService;

        public InternalCheckBox(IEditorService editorService)
        {
            this.editorService = editorService;
            this.CheckAlign = ContentAlignment.MiddleCenter;
        }

        protected override void OnCheckedChanged(EventArgs e)
        {
            base.OnCheckedChanged(e);

            if (this.Parent != null)
            {
                this.editorService.Close();
            }
        }
    }
}
