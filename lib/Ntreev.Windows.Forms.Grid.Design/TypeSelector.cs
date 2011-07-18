using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing.Design;
using System.Windows.Forms.Design;
using System.Windows.Forms;
using System.ComponentModel.Design;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class TypeSelector : UITypeEditor
    {
        class EditingControl : ListBox
        {
            IWindowsFormsEditorService editorService;

            public EditingControl(IWindowsFormsEditorService editorService)
            {
                this.editorService = editorService;
                this.BorderStyle = BorderStyle.None;
            }

            protected override void OnSelectedIndexChanged(EventArgs e)
            {
                base.OnSelectedIndexChanged(e);
                this.editorService.CloseDropDown();
            }
        }

        public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            IWindowsFormsEditorService editorService = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));
            if (editorService != null)
            {
                EditingControl listBox = new EditingControl(editorService);
                listBox.Items.AddRange(new object[] {
                    typeof(string), typeof(int), typeof(float), typeof(bool), 
                    typeof(System.Drawing.Color), typeof(System.Drawing.Point), typeof(System.Drawing.Rectangle),
                });
                listBox.Items.Add("찾아보기...");

                listBox.SelectedItem = value;

                //ITypeResolutionService typeService = provider.GetService(typeof(ITypeResolutionService)) as ITypeResolutionService;

                //if (typeService != null)
                //{
                //    int qwer = 0;
                //}

                editorService.DropDownControl(listBox);

                

                if (listBox.SelectedItem != null && listBox.SelectedItem.GetType() != typeof(string))
                    return listBox.SelectedItem;
            }
            return base.EditValue(context, provider, value);
        }

        public override UITypeEditorEditStyle GetEditStyle(System.ComponentModel.ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.DropDown;
        }

        public override bool IsDropDownResizable
        {
            get
            {
                return true;
            }
        }
    }
}
