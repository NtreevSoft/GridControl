using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Design;
using System.ComponentModel.Design.Serialization;

namespace Ntreev.Windows.Forms.Grid.Design
{
    [DesignerSerializer(typeof(RowCollectionCodeDomSerializer), typeof(CodeDomSerializer))]
    public class RowCollectionEditor : CollectionEditor
    {
        public RowCollectionEditor(Type type)
            : base(type)
        {
            
        }


        protected override object CreateInstance(Type itemType)
        {
            GridControl gridControl = this.Context.Instance as GridControl;
            
            if(gridControl == null)
            {
                GridControlDesigner.ActionList actionList = this.Context.Instance as GridControlDesigner.ActionList;
                gridControl = actionList.Component as GridControl;
            }

            if (gridControl == null)
                return null;
            return new Row(gridControl);
        }
    }
}
