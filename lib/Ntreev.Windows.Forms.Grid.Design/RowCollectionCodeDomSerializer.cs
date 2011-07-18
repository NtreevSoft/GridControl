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

                foreach (Row item in rows)
                {
                    object rowCode = SerializeRow(manager, target, item);

                    if (rowCode is CodeStatementCollection)
                    {
                        statements.AddRange(rowCode as CodeStatementCollection);
                    }
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

            //ExpressionContext contextParent = manager.Context[typeof(ExpressionContext)] as ExpressionContext;


            //ExpressionContext expression = new ExpressionContext(codeIndexer, typeof(Row), rows, row);

            //manager.Context.Push(expression);

            //CodeDomSerializer rowSerialzier = (CodeDomSerializer)manager.GetSerializer(typeof(Row), typeof(CodeDomSerializer));

            //object codeObject = rowSerialzier.Serialize(manager, row);

            // //if(codeObject is CodeStatementCollection)
            ////{
            ////    CodeStatementCollection statements = codeObject as CodeStatementCollection;

            ////    foreach (Cell item in row.Cells)
            ////    {
            ////        CodeStatementCollection cellwCodeStatements = SerializeCell(manager, codeIndexer, item, row.Cells);

            ////        statements.AddRange(cellwCodeStatements);
            ////    }
            ////}

            //manager.Context.Pop();
            //if (codeObject == null)
            //    return null;

            //return codeObject as CodeStatementCollection;
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
