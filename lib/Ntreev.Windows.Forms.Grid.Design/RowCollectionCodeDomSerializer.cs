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
using System.Linq;
using System.Text;
using System.ComponentModel.Design.Serialization;
using System.CodeDom;
using System.Reflection;
using System.ComponentModel;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class RowCollectionCodeDomSerializer : CodeDomSerializer
    {
        private static readonly Attribute[] _designTimeFilter = new Attribute[] { DesignOnlyAttribute.Yes };
        private static readonly Attribute[] _runTimeFilter = new Attribute[] { DesignOnlyAttribute.No };

        public override object Deserialize(IDesignerSerializationManager manager, object codeObject)
        {
            if (codeObject is CodeStatementCollection)
            {
                CodeStatementCollection statements = (CodeStatementCollection)codeObject;
            }
            return base.Deserialize(manager, codeObject);
        }

        public override object Serialize(IDesignerSerializationManager manager, object value)
        {
            object codeObject = base.Serialize(manager, value);
                        
            if (codeObject is CodeStatementCollection)
            {
                CodeStatementCollection statements = codeObject as CodeStatementCollection;

                CodeExpression target = base.SerializeToExpression(manager, value);

                RowCollection rows = value as RowCollection;

                if (rows != null)
                {
                    foreach (Row item in rows)
                    {
                        object rowCode = SerializeRow(manager, target, item);

                        if (rowCode is CodeStatementCollection)
                        {
                            statements.AddRange(rowCode as CodeStatementCollection);
                        }
                    }
                }

                foreach (CodeObject item in statements)
                {
                    item.UserData["statement-ordering"] = "end";
                }
            }

            return codeObject;
        }

        object SerializeRow(IDesignerSerializationManager manager, CodeExpression target, Row row)
        {
            object codeObject = null;
            ExpressionContext context = null;
            ExpressionContext context2 = manager.Context[typeof(ExpressionContext)] as ExpressionContext;
            CodeMethodInvokeExpression codeIndexer = new CodeMethodInvokeExpression(target, "GetAt", new CodePrimitiveExpression(row.Index));
            if (context2 != null)
            {
                //CodeIndexerExpression codeIndexer = new CodeIndexerExpression(target, new CodePrimitiveExpression(row.Index));
                context = new ExpressionContext(codeIndexer, typeof(RowCollection), context2.PresetValue, row);
                manager.Context.Push(context);
            }
            try
            {
                CodeDomSerializer rowSerialzier = (CodeDomSerializer)manager.GetSerializer(row.GetType(), typeof(CodeDomSerializer));
              
                //codeObject = rowSerialzier.Serialize(manager, row);
                codeObject = rowSerialzier.SerializeAbsolute(manager, row);
            }
            finally
            {
                foreach (Cell item in row.Cells)
                {
                    object cellCodeObject = SerializeCell(manager, codeIndexer, item);

                    if(cellCodeObject is CodeStatementCollection && codeObject is CodeStatementCollection)
                    {
                        (codeObject as CodeStatementCollection).AddRange(cellCodeObject as CodeStatementCollection);
                    }
                }

                if (context != null)
                {
                    manager.Context.Pop();
                }
            }

            return codeObject;
        }

        object SerializeCell(IDesignerSerializationManager manager, CodeExpression target, Cell cell)
        {
            object codeObject = null;
            ExpressionContext context = null;
            ExpressionContext context2 = manager.Context[typeof(ExpressionContext)] as ExpressionContext;
            if (context2 != null)
            {
                CodeMethodInvokeExpression codeIndexer = new CodeMethodInvokeExpression(target, "GetAt", new CodePrimitiveExpression(cell.Column.Index));
                context = new ExpressionContext(codeIndexer, typeof(RowCollection), context2.PresetValue, cell);
                manager.Context.Push(context);
            }
            try
            {
                CodeDomSerializer rowSerialzier = (CodeDomSerializer)manager.GetSerializer(cell.GetType(), typeof(CodeDomSerializer));

                //codeObject = rowSerialzier.Serialize(manager, row);
                codeObject = rowSerialzier.SerializeAbsolute(manager, cell);
            }
            finally
            {
                if (context != null)
                {
                    manager.Context.Pop();
                }
            }

            return codeObject;
        }

    }
}