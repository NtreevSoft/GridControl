#region License
//Ntreev Grid for .Net 2.0.5792.31442
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
using EnvDTE;
using VSLangProj;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public partial class TypeSelectorForm : Form
    {
        Type type;
        Assembly projectAssembly;
        Assembly[] referenceAssemblies;
        Type filter;

        static public TypeSelectorForm FromVSProject(IServiceProvider provider)
        {
            ITypeResolutionService resService = provider.GetService(typeof(ITypeResolutionService)) as ITypeResolutionService;
            DTE dte = provider.GetService(typeof(DTE)) as DTE;

            Project project = null;
            try
            {
                object[] projects = dte.ActiveSolutionProjects as object[];
                if (projects.Length == 0)
                {
                    return null;
                }

                project = projects[0] as Project;

            }
            catch (Exception e)
            {
                return null;
            }

            if (project == null)
            {
                return null;
            }

            TypeSelectorForm typeSelectorForm = new TypeSelectorForm();
            VSProject vsproj = project.Object as VSProject;

            try
            {
                string assemblyName = string.Empty;
                foreach (Property prop in project.Properties)
                {
                    if (prop.Name == "AssemblyName")
                    {
                        assemblyName = prop.Value.ToString();
                        break;
                    }
                }

                Assembly projectAseembly = Assembly.Load(assemblyName);
                if (projectAseembly != null)
                {
                    typeSelectorForm.projectAssembly = projectAseembly;
                }
            }
            catch (Exception)
            {
            }

            List<Assembly> assemblies = new List<Assembly>();
            for (int i = 1; i <= vsproj.References.Count; i++)
            {
                Reference reference = vsproj.References.Item(i);
                Assembly assembly = resService.GetAssembly(new AssemblyName(reference.Name));
                assemblies.Add(assembly);
            }

            typeSelectorForm.referenceAssemblies = assemblies.ToArray();

            return typeSelectorForm;
        }

        public TypeSelectorForm()
        {
            InitializeComponent();
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            InitializeNode(string.Empty);
        }

        public Type Filter
        {
            get { return this.filter; }
            set { this.filter = value; }
        }

        public Assembly ProjectAssembly
        {
            get { return this.projectAssembly; }
            set { this.projectAssembly = value; }
        }

        public Assembly[] ReferenceAssemblies
        {
            get { return this.referenceAssemblies; }
            set { this.referenceAssemblies = value; }
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
            this.toolStripComboBox1.Size = new Size(this.Width - 25, this.toolStripComboBox1.Height);
        }

        private void InitializeNode(string filter)
        {
            this.treeView1.Nodes.Clear();

            if (this.projectAssembly != null)
            {
                AddTypeToTreeView(this.projectAssembly, true, filter);
            }

            if (this.referenceAssemblies != null)
            {
                foreach (Assembly item in this.referenceAssemblies)
                {
                    if(item != null)
                        AddTypeToTreeView(item, false, filter);
                }
            }

            this.treeView1.Sort();
        }

        private void AddTypeToTreeView(Assembly assembly, bool isProjectAsssembly, string filter)
        {
            Type[] types;
            //Type[] types = assembly.GetExportedTypes();
            if (isProjectAsssembly == true)
            {
                types = assembly.GetTypes();
            }
            else
            {
                types = assembly.GetExportedTypes();
            }

            string assemblyName = assembly.GetName().Name;

            TreeNode assemblyNode = null;

            foreach (Type type in types)
            {
                if(string.IsNullOrEmpty(filter) == false)
                {
                    if (type.Name.IndexOf(filter, StringComparison.InvariantCultureIgnoreCase) == -1)
                        continue;
                }

                if (this.filter != null)
                {
                    if (this.filter.IsAssignableFrom(type) == false)
                        continue;

                    if (type.IsAbstract == true)
                        continue;

                    if (type.GetConstructor(new Type[] { }) == null)
                        continue;
                }
                else if (type.IsValueType == false)
                {
                    continue;
                }

                if (assemblyNode == null)
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

                int imageIndex = GetImageIndex(type);
                TreeNode typeNode = namespaceNode.Nodes.Add(type.Name, type.Name, imageIndex, imageIndex);
                typeNode.Tag = type;
            }
        }

        private int GetImageIndex(Type type)
        {
            string imageName = "VSObject";


            if (type.IsClass == true)
            {
                imageName += "_Class";
            }
            else if (type.IsEnum == true)
            {
                imageName += "_Enum";
            }
            else if (type.IsValueType == true)
            {
                imageName += "_Structure";
            }
            else
            {
                throw new ArgumentException();
            }

            if (type.IsNested == false)
            {
                if (type.IsPublic == false)
                {
                    imageName += "_Sealed";
                }
            }
            else
            {
                if (type.IsNestedPrivate == true)
                {
                    imageName += "_Private";
                }
                else if (type.IsNestedFamily == true)
                {
                    imageName += "_Sealed";
                }
                else if (type.IsNestedPublic == false)
                {
                    imageName += "_Protected";
                }
            }

            imageName += ".bmp";

            return this.imageList1.Images.IndexOfKey(imageName);
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

        private void toolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            InitializeNode(this.toolStripComboBox1.Text);
        }

        private void toolStripComboBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode != Keys.Enter)
                return;

            if (this.toolStripComboBox1.Text != string.Empty)
            {
                this.toolStripComboBox1.Items.Add(this.toolStripComboBox1.Text);
            }

            InitializeNode(this.toolStripComboBox1.Text);
            e.SuppressKeyPress = true;
        }
    }
}