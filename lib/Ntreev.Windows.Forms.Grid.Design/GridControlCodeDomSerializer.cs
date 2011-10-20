#region License
//Ntreev Grid for .Net 1.0.4300.26762
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
using System.ComponentModel.Design.Serialization;
using System.CodeDom;
using System.Windows.Forms;
using System.ComponentModel;
using System.ComponentModel.Design;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class GridControlCodeDomSerializer : CodeDomSerializer
    {
        public override object Serialize(IDesignerSerializationManager manager, object value)
        {
            if (IsSerialized(manager, value) == true)
                return GetExpression(manager, value);

            GridControl gridControl = value as GridControl;

            CodeDomSerializer userControlSerializer = (CodeDomSerializer)manager.GetSerializer(typeof(UserControl), typeof(CodeDomSerializer));
            CodeDomSerializer columnSerializer = (CodeDomSerializer)manager.GetSerializer(typeof(Column), typeof(CodeDomSerializer));

            CodeStatementCollection statements = new CodeStatementCollection();


            //SerializeValue(manager, gridControl.DataSource, statements);


            foreach (Column item in gridControl.Columns)
            {
                object columnCodes = columnSerializer.Serialize(manager, item);
                if (columnCodes == null)
                {
                    continue;
                }

                if (columnCodes is CodeStatementCollection)
                {
                    statements.AddRange(columnCodes as CodeStatementCollection);
                }
            }

            object codeObject = userControlSerializer.Serialize(manager, value);

            if (codeObject is CodeStatementCollection)
            {
                statements.AddRange(codeObject as CodeStatementCollection);
            }

            return statements;
        }

        void SerializeValue(IDesignerSerializationManager manager, object value, CodeStatementCollection statements)
        {
            if (value == null)
                return;

            CodeDomSerializer serializer = (CodeDomSerializer)manager.GetSerializer(value.GetType(), typeof(CodeDomSerializer));

            object codes = serializer.Serialize(manager, value);

            if (codes is CodeStatementCollection)
            {
                statements.AddRange(codes as CodeStatementCollection);
            }

            if (value is IComponent)
            {
                IDesigner designer = System.ComponentModel.TypeDescriptor.CreateDesigner(value as IComponent, typeof(IDesigner));
                if (designer is ComponentDesigner)
                {
                    ComponentDesigner componentDesigner = designer as ComponentDesigner;
                    foreach (object item in componentDesigner.AssociatedComponents)
                    {
                        SerializeValue(manager, item, statements);
                    }
                }
            }
        }
    }
}