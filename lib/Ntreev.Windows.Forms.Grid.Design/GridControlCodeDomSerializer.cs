using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Design.Serialization;
using System.CodeDom;
using System.Windows.Forms;
using System.ComponentModel;

namespace Ntreev.Windows.Forms.Grid.Design
{
    
    public class GridControlCodeDomSerializer : CodeDomSerializer
    {
        public override object Serialize(IDesignerSerializationManager manager, object value)
        {
            GridControl gridControl = value as GridControl;

            CodeDomSerializer userControlSerializer = (CodeDomSerializer)manager.GetSerializer(typeof(UserControl), typeof(CodeDomSerializer));
            CodeStatementCollection codes = new CodeStatementCollection();

            CodeDomSerializer columnSerializer = (CodeDomSerializer)manager.GetSerializer(typeof(Component), typeof(CodeDomSerializer));

            foreach (Column item in gridControl.Columns)
            {
                object columnCodes = columnSerializer.Serialize(manager, item);
                if (columnCodes == null)
                {
                    continue;
                }

                if (columnCodes is CodeStatementCollection)
                {
                    codes.AddRange(columnCodes as CodeStatementCollection);
                }
            }

            object codeObject = userControlSerializer.Serialize(manager, value);
          

            if (codeObject is CodeStatementCollection)
            {
                codes.AddRange(codeObject as CodeStatementCollection);
            }

            return codes;

        }
    }
}
