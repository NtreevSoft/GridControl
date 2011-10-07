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
    public partial class AssembliesForm : Form
    {
        Type type = null;

        public AssembliesForm()
            :this(null)
        {

        }

        public AssembliesForm(Assembly[] assemblies)
        {
            InitializeComponent();

           
            //string[] paths = { @"C:\Windows\Microsoft.NET\Framework\v2.0.50727\mscorlib.dll", };

            foreach (Assembly assembly in assemblies)
            {
                //Assembly assembly = Assembly.GetAssembly(typeof(int));

                Type[] types = assembly.GetExportedTypes();

                TreeNode node = this.treeView1.Nodes.Add(assembly.GetName().Name, assembly.GetName().Name, 0, 0);
                foreach (Type type in types)
                {
                    if (type.IsValueType == false)
                        continue;

                    TreeNode namespaceNode = null;
                    if (node.Nodes.Find(type.Namespace, false).Length != 0)
                    {
                        namespaceNode = node.Nodes[type.Namespace];
                    }
                    else
                    {
                        namespaceNode = node.Nodes.Add(type.Namespace, type.Namespace, 1, 1);
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
