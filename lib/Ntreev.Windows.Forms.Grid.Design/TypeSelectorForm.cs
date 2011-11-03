#region License
//Ntreev Grid for .Net 1.1.4324.22060
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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.Windows.Forms.Design;
using System.ComponentModel.Design;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public partial class TypeSelectorForm : Form
    {
        Type type = null;

        public TypeSelectorForm()
            :this(null)
        {

        }

        public TypeSelectorForm(Assembly[] assemblies)
            :this(assemblies, null)
        {
            
        }

        public TypeSelectorForm(Assembly[] assemblies, Type classType)
        {
            InitializeComponent();

            foreach (Assembly assembly in assemblies)
            {
                Type[] types = assembly.GetExportedTypes();
                string assemblyName = assembly.GetName().Name;

                TreeNode assemblyNode = null;

                foreach (Type type in types)
                {
                    if (classType != null)
                    {
                        if (classType.IsAssignableFrom(type) == false)
                            continue;

                        if (type.IsAbstract == true)
                            continue;

                        if (type.GetConstructor(new Type[] {}) == null)
                            continue;
                    }
                    else if (type.IsValueType == false)
                    {
                        continue;
                    }

                    if(assemblyNode == null)
                        assemblyNode = this.treeView1.Nodes.Add(assemblyName, assemblyName, 0, 0);

                    TreeNode namespaceNode = null;
                    if (assemblyNode.Nodes.Find(type.Namespace, false).Length != 0)
                    {
                        namespaceNode = assemblyNode.Nodes[type.Namespace];
                    }
                    else
                    {
                        namespaceNode = assemblyNode.Nodes.Add(type.Namespace, type.Namespace, 1, 1);
                    }

                    int imageIndex = type.IsEnum == true ? 2 : 3;
                    TreeNode typeNode = namespaceNode.Nodes.Add(type.Name, type.Name, imageIndex, imageIndex);
                    typeNode.Tag = type;

                }
            }
            this.treeView1.Sort();
        }

        public Type SelectedType
        {
            get
            {
                if (this.type == null)
                    return typeof(string);
                return this.type; 
            }
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
        }

        private void treeView1_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node.Tag != null)
            {
                this.type = e.Node.Tag as Type;
                this.DialogResult = DialogResult.OK;
            }
        }

        private void treeView1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode != Keys.Enter)
                return;

            if (this.treeView1.SelectedNode.Tag is Type == false)
                return;

            this.DialogResult = DialogResult.OK;
        }
    }
}