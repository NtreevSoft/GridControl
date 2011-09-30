#region License
//Ntreev Grid for .Net 1.0
//https://github.com/NtreevSoft/GridControl

//Released under the MIT License.

//Copyright (c) 2010 Ntreev Soft co., Ltd.

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
//rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
//persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
//Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
//WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
//OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endregion
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