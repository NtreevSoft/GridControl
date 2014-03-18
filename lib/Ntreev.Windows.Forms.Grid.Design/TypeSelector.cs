#region License
//Ntreev Grid for .Net 2.0.5190.32793
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
        class TypeSelectorCore
        {
            readonly ListBox listBox = new ListBox();

            IWindowsFormsEditorService editorService;
            IServiceProvider provider;
            Type selectedType = null;

            public TypeSelectorCore(IWindowsFormsEditorService editorService, IServiceProvider provider, Type baseType)
            {
                this.editorService = editorService;
                this.provider = provider;
                this.selectedType = baseType;
                this.listBox.BorderStyle = BorderStyle.None;

                object[] types = { typeof(string), typeof(int), typeof(bool), typeof(float), 
                    typeof(System.Drawing.Color), typeof(System.Drawing.Point), typeof(System.Drawing.Rectangle) };

                if (types.Contains(baseType) == false)
                    this.listBox.Items.Add(baseType);
                this.listBox.Items.AddRange(types);
                this.listBox.Items.Add("Etc...");

                this.listBox.SelectedItem = baseType;

                this.listBox.SelectedIndexChanged += new EventHandler(listBox_SelectedIndexChanged);
            }

            public void DropDownControl()
            {
                this.editorService.DropDownControl(this.listBox);
            }

            public Type SelectedType
            {
                get { return this.selectedType; }
            }

            void listBox_SelectedIndexChanged(object sender, EventArgs e)
            {
                if (this.listBox.SelectedItem.ToString() == "Etc...")
                {
                    TypeSelectorForm assembliesForm = TypeSelectorForm.FromVSProject(this.provider);
                    if (assembliesForm.ShowDialog() == DialogResult.OK)
                    {
                        this.selectedType = assembliesForm.SelectedType;
                    }
                }
                else
                {
                    this.selectedType = this.listBox.SelectedItem as Type;
                }
                this.editorService.CloseDropDown();
            }
        }

        public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            IWindowsFormsEditorService editorService = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));
            if (editorService != null)
            {
                TypeSelectorCore typeSelector = new TypeSelectorCore(editorService, provider, value as Type);
                typeSelector.DropDownControl();
                return typeSelector.SelectedType;
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